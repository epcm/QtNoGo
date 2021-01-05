#include "hintwidget.h"
#include <QTimer>
#include "utils.h"

HintWidget::HintWidget(QString text,QWidget *parent) : QLabel(text, parent)
{
    //设置去掉窗口边框
    //this->setWindowFlags(Qt::FramelessWindowHint);
    //text为要显示的信息
    this->setText(text);
    //设置定时器，到时自我销毁
    timer = new QTimer(this);
    timer->start(1000);//时间1秒
    timer->setSingleShot(true);//仅触发一次
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeDestroy()));
    // 默认样式表
    this->setStyleSheet("background-color:rgba(219,237,243,200);border-radius:5px;"
                        "font: 10pt 'Arial Rounded MT Bold'");
    this->setAlignment(Qt::AlignCenter);
    // 控件默认的显示位置
    if(parent)
        this->setGeometry(parent->width()/2-hintWidgetWidth/2,parent->height()/2-hintWidgetHeight/2,
                          hintWidgetWidth,hintWidgetHeight);
    else
        this->setGeometry(640/2-hintWidgetWidth/2,460/2-hintWidgetHeight/2,
                          hintWidgetWidth,hintWidgetHeight);
}

void HintWidget::onTimeDestroy()
{
    this->close();
    delete this;
}
