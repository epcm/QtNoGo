#include "welcomewidget.h"
#include "ui_welcomewidget.h"
#include "utils.h"

WelcomeWidget::WelcomeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WelcomeWidget)
{
    ui->setupUi(this);
    setFixedSize(kBoardMargin * 2 + kBlockSize * (kBoardSizeNum-1), kBoardMargin * 2 + kBlockSize * (kBoardSizeNum-1));
}

WelcomeWidget::~WelcomeWidget()
{
    delete ui;
}

void WelcomeWidget::on_PVCPushButton_clicked()
{
    emit display(1);
    emit gameType(0);
}

void WelcomeWidget::on_PVPPushButton_clicked()
{
    emit display(1);
    emit gameType(1);
}
