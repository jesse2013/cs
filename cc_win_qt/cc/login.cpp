#include "login.h"
#include "ui_login.h"
#include <QMessageBox>

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    //move(1200,50);
    setFixedSize(240,600);

    //设置背景图片
    //setObjectName("lg");
    //setStyleSheet("#lg {border-image: url(:/img/bj.jpg);}");

    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket,SIGNAL(connected()),this,SLOT(sendMessage()));
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(receiveMessage()));
}

login::~login()
{
    delete ui;
}

void login::conectServer()
{
    tcpSocket->abort(); //取消已经建立的连接
    tcpSocket->connectToHost("192.168.20.225", 8888);   //连接服务器
}

void login::sendMessage()
{
    totalBytes = 0;
    QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
    //设置数据流版本，这里要和服务器端相同
    sendOut.setVersion(QDataStream::Qt_5_0);
    //依次写入请求类型,用户名,密码等信息
    sendOut<<":2:troy:troy:::";
    //实际发送数据的总大小
    totalBytes += outBlock.size();
    qDebug()<<totalBytes;
    sendOut.device()->seek(0);
    bytesWritten = tcpSocket->write(outBlock);
    //发送完头数据后剩余数据的大小
    bytesToWrite = totalBytes - bytesWritten;
    outBlock.resize(0);
}

void login::receiveMessage()
{
    blockSize = 0; //初始化其为0
    QDataStream in(tcpSocket);
    //设置数据流版本，这里要和服务器端相同
    in.setVersion(QDataStream::Qt_5_0);
    qDebug()<<blockSize;
    if(blockSize==0) //如果是刚开始接收数据
    {
        //判断接收的数据是否有两字节，也就是文件的大小信息
        //如果有则保存到blockSize变量中，没有则返回，继续接收数据
        if(tcpSocket->bytesAvailable() < (int)sizeof(quint8)) return;
        in >> blockSize;
    }
    qDebug()<<blockSize;
    //如果没有得到全部的数据，则返回，继续接收数据
    if(tcpSocket->bytesAvailable() < blockSize) return;

    in >> message;
    qDebug()<<message;
    //将接收到的数据存放到变量中
    //ui->messageLabel->setText(message);
    //显示接收到的数据
}

void login::on_btn_login_clicked()
{
    //连接服务器
    conectServer();

    //处理接收数据,并依据接收数据执行对应代码
    if (ui->le_username->text() == tr("aaa") &&
            ui->le_passwd->text() == tr("bbb")) {
        accept();
    } else {
        QMessageBox::warning(this, tr("Waring"),
                             tr("username or password error!"),
                             QMessageBox::Yes);

        ui->le_username->clear();
        ui->le_passwd->clear();
        ui->le_username->setFocus();
    }
}
