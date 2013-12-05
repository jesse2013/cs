#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QtNetwork>

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

private slots:
    void on_btn_login_clicked();
    //收集,整理,发送数据
    void sendMessage();
    //接收,分析数据,然后选择执行
    void receiveMessage();

public:
    explicit login(QWidget *parent = 0);
    ~login();

    //连接服务器
    void conectServer();
    //获取数据,整理数据
    char *packageData(int req_type, char &name, char &passwd,
                      char &buddy_name, char &content, char datetime);


private:
    Ui::login *ui;
    QTcpSocket *tcpSocket;
    QString message;    //存放从服务器接收到的字符串
    quint16 blockSize;  //存放文件的大小信息
    QByteArray outBlock;  //数据缓冲区，即存放每次要发送的数据
    qint64 totalBytes;  //数据总大小
    qint64 bytesWritten;  //已经发送数据大小
    qint64 bytesToWrite;   //剩余数据大小
};

#endif // LOGIN_H
