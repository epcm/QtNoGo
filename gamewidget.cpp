#include "gamewidget.h"
#include "ui_gamewidget.h"
#include "./ai/aimcts.h"
#include "hintwidget.h"
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <cmath>
#include <QTimer>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QFileDialog>

GameWidget::GameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameWidget)
{
    ui->setupUi(this);

    // 开启鼠标实时监测
    setMouseTracking(true);
    menubar = new QMenuBar(this);
    menubar->setStyleSheet("font: 24px 'Arial Rounded MT Bold'");
    menubar->setGeometry(0,0, this->width(),40);
    QAction* saveAction = menubar->addAction("Save");
    QAction* loadAction = menubar->addAction("Load");
    QAction* aboutAction = menubar->addAction("About");
    menubar->show();
    // 菜单栏动作
    connect(saveAction, &QAction::triggered, referee, &Referee::saveGame);
    connect(loadAction, &QAction::triggered, referee, &Referee::loadGame);
    connect(aboutAction, &QAction::triggered, this, &GameWidget::showAboutWidget);
    // 裁判动作
    connect(referee, SIGNAL(updateSignal()), this, SLOT(startUpdate()));
    connect(referee, &Referee::displayHintSignal, this, &GameWidget::displayHint);
    connect(referee, &Referee::pauseSignal, this, &GameWidget::on_PausePushButton_clicked);
}

GameWidget::~GameWidget()
{
    delete ui;
}

void GameWidget::showAboutWidget()
{
    QMessageBox AboutBox;
    AboutBox.setStyleSheet("font: 18px 'Arial Rounded MT Bold'");
    AboutBox.setText("Copyright © 2020-2021 epcm. All rights reserved.<br><br>"
                     "Powered by QT 5.15.2<br><br>"
                     "Github: <a href='https://github.com/epcm/QtNoGo'>https://github.com/epcm/QtNoGo(待作业提交后开源)</a>");
    AboutBox.exec();
}

void GameWidget::displayHint(int number)
{
    switch (number)
    {
    case 0: //红方超时
        h = new HintWidget("RED_TIME_LIMIT_EXCEEDED", this);
        h->setStyleSheet("background-color:rgba(247,56,89,200);border-radius:20px;"
                         "font: 24px 'Arial Rounded MT Bold'");
        break;
    case 1: //蓝方超时
        h = new HintWidget("BLUE_TIME_LIMIT_EXCEEDED", this);
        h->setStyleSheet("background-color:rgba(219,237,243,200);border-radius:20px;"
                         "font: 24px 'Arial Rounded MT Bold'");
        break;
    case 2: //位置无效
        h = new HintWidget("INVALID_POS", this);
        if(referee->m_color == BLACK) //红方无效
            h->setStyleSheet("background-color:rgba(247,56,89,200);border-radius:20px;"
                             "font: 24px 'Arial Rounded MT Bold'");
        else if(referee->m_color == WHITE) //蓝方无效
            h->setStyleSheet("background-color:rgba(219,237,243,200);border-radius:20px;"
                             "font: 24px 'Arial Rounded MT Bold'");
        break;
    case 3: //红方胜
        h = new HintWidget("WINNER: RED", this);
        h->setStyleSheet("background-color:rgba(247,56,89,200);border-radius:20px;"
                         "font: 24px 'Arial Rounded MT Bold'");
        break;
    case 4: //蓝方胜
        h = new HintWidget("WINNER: LIGHT BLUE", this);
        h->setStyleSheet("background-color:rgba(219,237,243,200);border-radius:20px;"
                         "font: 24px 'Arial Rounded MT Bold'");
        break;
    default:
        break;
    }
    h->show();
}

void GameWidget::startUpdate()
{
    if(referee->m_game_mode == REPLAY)
        ui->TimeBar->setValue(0);
    else
    {
        int t;
        if(referee->m_player == BOT)
            t = referee->m_bot_time_limit*1000;
        else if(referee->m_player == HUMAN)
            t = referee->m_human_time_limit*1000;
        if(referee->m_paused)
            ui->TimeBar->setValue(1000-(clock()-referee->m_time_when_paused)*1000.0/t);
        else
        {
            //qDebug() << clock()-referee->m_start_time;
            //qDebug() << (clock()-referee->m_start_time)*1000/t;
            ui->TimeBar->setValue(1000-(clock()-referee->m_start_time)*1000.0/t);
            //qDebug() << ui->TimeBar->value();
        }
    }
    repaint();
}

void GameWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true); // 抗锯齿
    QPen pen; // 调整线条宽度
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);


    // 绘制棋盘
    QRectF target(0.0, 0.0, this->width(), this->height());
    QRectF source(0.0, 0.0, this->width(), this->height());
    QImage image(":/pic/QTNoGo.png");
    painter.drawImage(target, image, source);

    /*
    // 绘制按钮（用图片做按钮背景锯齿太明显了）
    pen.setWidth(6);
    QRect button;
    // 左侧
    pen.setColor(QColor("#f73859"));
    painter.setPen(pen);
    //button = ui->HintPushButton->geometry();
    painter.drawEllipse(button);
    button = ui->ReactPushButton->geometry();
    painter.drawEllipse(button);
    // 右侧
    pen.setColor(QColor("#dbedf3"));
    painter.setPen(pen);
    button = ui->RestartPushButton->geometry();
    painter.drawEllipse(button);
    button = ui->PausePushButton->geometry();
    painter.drawEllipse(button);
    */
    // 绘制落子标记(防止鼠标出框越界)
    if (clickPosRow >= 0 && clickPosRow < kBoardSizeNum && clickPosCol >= 0 && clickPosCol < kBoardSizeNum)
    {
        pen.setWidth(4);
        pen.setColor(QColor("#404b69"));
        painter.setPen(pen);
        brush.setColor(QColor("#404b69"));
        painter.setBrush(brush);
        painter.drawRect(kBoardMargin + kBlockSize * clickPosCol - kMarkSize / 2, kBoardMargin + kBlockSize * clickPosRow - kMarkSize / 2, kMarkSize, kMarkSize);
    }


    // 绘制棋子
    pen.setWidth(15);
    for (int i = 0; i < kBoardSizeNum; i++)
        for (int j = 0; j < kBoardSizeNum; j++)
        {
            if (referee->m_board[i][j] == -1)
            {
                pen.setColor(QColor("#dbedf3"));
                brush.setColor(QColor("#283149"));
                painter.setBrush(brush);
                painter.setPen(pen);
                painter.drawEllipse(kBoardMargin + kBlockSize * j - kRadius, kBoardMargin + kBlockSize * i - kRadius, kRadius * 2, kRadius * 2);
            }
            else if (referee->m_board[i][j] == 1)
            {
                pen.setColor(QColor("#f73859"));
                brush.setColor(QColor("#283149"));
                painter.setBrush(brush);
                painter.setPen(pen);
                painter.drawEllipse(kBoardMargin + kBlockSize * j - kRadius, kBoardMargin + kBlockSize * i - kRadius, kRadius * 2, kRadius * 2);
            }
        }
}

void GameWidget::mouseMoveEvent(QMouseEvent *event)
{
    // 通过鼠标的hover确定落子的标记
    int x = event->x();
    int y = event->y();

    // 获取最近的左上角的点
    int col = (x-kBoardMargin+kBlockSize-10) / kBlockSize;
    int row = (y-kBoardMargin+kBlockSize-10) / kBlockSize;

    double leftTopPosX = kBoardMargin + kBlockSize * col;
    double leftTopPosY = kBoardMargin + kBlockSize * row;

    // 根据距离算出合适的点击位置,一共四个点，根据半径距离选最近的
    clickPosRow = -1; // 初始化最终的值
    clickPosCol = -1;
    double len = 0;

    // 确定一个误差在范围内的点，且只可能确定一个出来
    len = (x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY);
    if (len < kPosDelta*kPosDelta)
    {
        clickPosRow = row;
        clickPosCol = col;
    }
    len = (x - leftTopPosX - kBlockSize) * (x - leftTopPosX - kBlockSize) + (y - leftTopPosY) * (y - leftTopPosY);
    if (len < kPosDelta*kPosDelta)
    {
        clickPosRow = row;
        clickPosCol = col + 1;
    }
    len = (x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY - kBlockSize) * (y - leftTopPosY - kBlockSize);
    if (len < kPosDelta*kPosDelta)
    {
        clickPosRow = row + 1;
        clickPosCol = col;
    }
    len = (x - leftTopPosX - kBlockSize) * (x - leftTopPosX - kBlockSize) + (y - leftTopPosY - kBlockSize) * (y - leftTopPosY - kBlockSize);
    if (len < kPosDelta*kPosDelta)
    {
        clickPosRow = row + 1;
        clickPosCol = col + 1;
    }


    // 存了坐标后也要重绘
    update();
}

void GameWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(clickPosCol < 0 || clickPosCol >= 9 || clickPosRow < 0 || clickPosRow >= 9)
    {
        qDebug() << "OUT_OF_RANGE";
        return;
    }
    if(referee->m_player == HUMAN)
    {
        Action a;
        a.x = clickPosRow;
        a.y = clickPosCol;
        referee->m_cur_action = a;
        referee->judge();
    }
}

void GameWidget::initGame()
{
    emit showGameSignal(1);
    referee->resetReferee();
    referee->m_start_time = clock();
    if(referee->m_game_mode == PVP)
        referee->m_player = HUMAN;
    if(referee->m_player == BOT)
        referee->botOneStep();
    referee->m_timer->start(10);
}

void GameWidget::initReplay()
{
    QJsonArray temp = referee->m_history;
    referee->resetReferee();
    referee->m_history = temp;
    referee->m_replay_turn = 0;
    referee->m_game_mode = REPLAY;
    referee->m_player = NEITHER;
    //referee->m_replay_timer->start(1500);
}

void GameWidget::on_RestartPushButton_clicked()
{
    if(referee->m_game_mode == REPLAY)
    {
        initReplay();
        referee->m_replay_timer->start(REPLAYSPEED*1000);
        return;
    }
    referee->resetReferee();
    referee->m_start_time = clock();
    repaint();
    if(referee->m_player == BOT)
        referee->botOneStep();
    referee->m_timer->start(10);
}

void GameWidget::on_PausePushButton_clicked()
{
    if(referee->m_game_mode == REPLAY)
    {
        if(referee->m_paused) //恢复播放
        {
            referee->continueReplay();
            referee->m_paused = false;
            ui->PauseLabel->setText("Pause");
        }
        else if(!referee->m_paused) //暂停播放
        {
            referee->endReplay();
            referee->m_paused = true;
            ui->PauseLabel->setText("Continue");
        }
        return;
    }
    if(referee->m_paused) //解除暂停
    {
        referee->m_start_time = clock()-referee->m_time_when_paused;
        //qDebug() << "paused" << referee->m_time_when_paused;
        referee->m_timer->start(10);
        referee->m_paused = false;
        referee->m_player = HUMAN;
        ui->PauseLabel->setText("Pause");
    }
    else if(referee->m_player == HUMAN && !referee->m_paused) //只有人一方回合才能暂停
    {
        referee->m_time_when_paused = clock()-referee->m_start_time;
        referee->m_timer->stop();
        referee->m_paused = true;
        referee->m_player = NEITHER;
        ui->PauseLabel->setText("Continue");
    }

}

void GameWidget::on_ReactPushButton_clicked()
{
    if(referee->m_player == HUMAN && !referee->m_history.empty())
    {
        int num = 2;
        for(int i = 0; i < num; i++)
        {
            int x, y;
            QJsonArray arr = referee->m_history[referee->m_history.size()-1].toArray();
            QVariantList vl = arr.toVariantList();
            x = vl[0].toInt();
            y = vl[1].toInt();
            referee->m_board[x][y] = 0;
            referee->m_history.pop_back();
        }
        referee->m_start_time = clock();
        repaint();
    }
}

void GameWidget::on_HintPushButton_clicked()
{
    AIMCTS ai;
    Action a =  ai.aiAction(referee->m_color, referee->m_board);
    HintWidget* hintwidget = new HintWidget("", this);
    int x, y;
    x = kBoardMargin+kBlockSize*a.x;
    y = kBoardMargin+kBlockSize*a.y;
    hintwidget->setStyleSheet("border-width: 6px; border-style:solid;border-color:#dbedf3;border-radius:4px");
    //rgb(64, 75, 105)
    //设置控件显示的位置
    hintwidget->setGeometry(y-kMarkSize/2,x-kMarkSize/2, kMarkSize,kMarkSize);
    hintwidget->show();

}

void GameWidget::on_ReturnPushButton_clicked()
{
    emit showWelcomeSignal(0);
}
