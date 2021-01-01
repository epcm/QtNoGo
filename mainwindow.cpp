#include "mainwindow.h"
#include "utils.h"
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <cmath>
#include <QTimer>
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 设置棋盘大小
    //setFixedSize(kBoardMargin * 2 + kBlockSize * (kBoardSizeNum-1), kBoardMargin * 2 + kBlockSize * (kBoardSizeNum-1));
    //setMouseTracking(true);
    //centralWidget()->setMouseTracking(true);
    gamewidget = new GameWidget;
    welcomewidget = new WelcomeWidget;
    stackLayout = new QStackedLayout;
    stackLayout->addWidget(welcomewidget);
    stackLayout->addWidget(gamewidget);
    connect(welcomewidget, &WelcomeWidget::display, stackLayout, &QStackedLayout::setCurrentIndex);             // 0
    connect(gamewidget, &GameWidget::display, stackLayout, &QStackedLayout::setCurrentIndex);             // 1
    connect(welcomewidget, &WelcomeWidget::gameType, gamewidget->referee, &Referee::initGame);
    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(stackLayout);
    setLayout(mainLayout);
    qDebug() << "init mainwindow";
}


MainWindow::~MainWindow()
{
}


