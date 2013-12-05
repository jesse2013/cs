#include "cc.h"
#include "ui_cc.h"

cc::cc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cc)
{
    ui->setupUi(this);
    //move(1200,50);
    setFixedSize(200,600);

    //无边框
    //setWindowOpacity(1);
    //setWindowFlags(Qt::FramelessWindowHint);
    //setAttribute(Qt::WA_TranslucentBackground);
}

cc::~cc()
{
    delete ui;
}
