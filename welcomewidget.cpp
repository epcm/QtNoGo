#include "welcomewidget.h"
#include "ui_welcomewidget.h"
#include "utils.h"

WelcomeWidget::WelcomeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WelcomeWidget)
{
    ui->setupUi(this);
    //setFixedSize(kBoardMargin * 2 + kBlockSize * (kBoardSizeNum-1), kBoardMargin * 2 + kBlockSize * (kBoardSizeNum-1));

}

WelcomeWidget::~WelcomeWidget()
{
    delete ui;
}


void WelcomeWidget::on_NewGamePushButton_clicked()
{
    emit displaySetting();
}


void WelcomeWidget::on_ReplayPushButton_clicked()
{
    emit initReplaySignal();
    emit showReplaySignal(1);
}
