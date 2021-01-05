#include "mainwindow.h"
#include "utils.h"
#include "referee.h"
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <cmath>
#include <QTimer>
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QLineEdit>
#include <QComboBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 初始化各个界面，并建立信号槽关联
    gamewidget = new GameWidget;
    welcomewidget = new WelcomeWidget;
    settingdialog = new SettingDialog;

    stackLayout = new QStackedLayout;
    stackLayout->addWidget(welcomewidget);
    stackLayout->addWidget(gamewidget);
    // 一定要注意信号与槽的参数匹配
    connect(settingdialog, &SettingDialog::startGameSignal, gamewidget, &GameWidget::initGame);                         //调用游戏初始化函数
    connect(welcomewidget, &WelcomeWidget::initReplaySignal, gamewidget, &GameWidget::initReplay);                         //调用录像初始化函数
    connect(gamewidget, &GameWidget::showGameSignal, stackLayout, &QStackedLayout::setCurrentIndex);                    //切换到游戏页面
    connect(welcomewidget, &WelcomeWidget::showReplaySignal, stackLayout, &QStackedLayout::setCurrentIndex);            //切换到录像页面
    connect(gamewidget, &GameWidget::showWelcomeSignal, stackLayout, &QStackedLayout::setCurrentIndex);                 //返回开始界面
    connect(welcomewidget, &WelcomeWidget::displaySetting, this, &MainWindow::showSettingSignal);                       //显示设置对话框
    connect(settingdialog, &SettingDialog::gameModeSelectSignal, gamewidget->referee, &Referee::setGameMode);           //设置游戏模式
    connect(settingdialog, &SettingDialog::firstPlayerSelectSignal, gamewidget->referee, &Referee::setFirstPlayer);     //设置先手方
    connect(settingdialog, &SettingDialog::botTimeEditSignal, gamewidget->referee, &Referee::setBotTimeLimit);          //设置Bot TLE
    connect(settingdialog, &SettingDialog::humanTimeEditSignal, gamewidget->referee, &Referee::setHumanTimeLimit);      //设置Human TLE

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(stackLayout);
    setLayout(mainLayout);
}

void MainWindow::showSettingSignal()
{
    settingdialog->show();
}


MainWindow::~MainWindow()
{
}


