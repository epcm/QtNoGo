#include "welcomewidget.h"
#include "ui_welcomewidget.h"
#include "utils.h"
#include "hintwidget.h"

WelcomeWidget::WelcomeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WelcomeWidget)
{
    ui->setupUi(this);
    //setFixedSize(kBoardMargin * 2 + kBlockSize * (kBoardSizeNum-1), kBoardMargin * 2 + kBlockSize * (kBoardSizeNum-1));
    HintWidget* h = new HintWidget("hhhhhh",this);
    h->show();
}

WelcomeWidget::~WelcomeWidget()
{
    delete ui;
}


void WelcomeWidget::on_NewGamePushButton_clicked()
{
    emit displaySetting();
}

