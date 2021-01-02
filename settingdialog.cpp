#include "settingdialog.h"
#include "ui_settingdialog.h"

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
    // 初始化选项卡
    QStringList ql;
    ql << "Human先手" << "Bot先手";
    ui->FirstPlayerSelect->addItems(ql);
    ql.clear();
    ql << "PVC" << "PVP";
    ui->GameModeSelect->addItems(ql);
    // 初始化输入框
    ui->BotTimeEdit->setText("1");
    ui->HumanTimeEdit->setText("30");
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

// 按下OK后直接跳转到游戏界面
void SettingDialog::on_OKPushButton_clicked()
{
    emit startGameSignal();
    this->close();
}


// 接收GameMode设置信息并发信给referee
void SettingDialog::on_GameModeSelect_currentIndexChanged(int index)
{
    emit gameModeSelectSignal(index);
}

// 接收FirstPlayer设置信息并发信给referee
void SettingDialog::on_FirstPlayerSelect_currentIndexChanged(int index)
{
    emit firstPlayerSelectSignal(index);
}

// 接收HumanTimeEdit设置信息并发信给referee
void SettingDialog::on_HumanTimeEdit_textChanged(const QString &arg1)
{
    emit humanTimeEditSignal(arg1);
}

void SettingDialog::on_BotTimeEdit_textChanged(const QString &arg1)
{
    emit botTimeEditSignal(arg1);
}

