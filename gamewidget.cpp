#include "gamewidget.h"
#include "ui_gamewidget.h"
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <cmath>
#include <QTimer>
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

GameWidget::GameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameWidget)
{
    ui->setupUi(this);
    // 设置棋盘大小
    //setFixedSize(kBoardMargin * 2 + kBlockSize * (kBoardSizeNum-1), kBoardMargin * 2 + kBlockSize * (kBoardSizeNum-1));
    //    setStyleSheet("background-color:yellow;");

    // 开启鼠标hover功能，这两句一般要设置window的
    setMouseTracking(true);
    //centralWidget()->setMouseTracking(true);
    connect(referee, SIGNAL(updateSignal()), this, SLOT(startUpdate()));
    qDebug() << "init gamewidget";
}

GameWidget::~GameWidget()
{
    delete ui;
}

void GameWidget::startUpdate()
{
    repaint();
}

void GameWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    //    centralWidget()->setMouseTracking(true);
    // 绘制棋盘
    painter.setRenderHint(QPainter::Antialiasing, true); // 抗锯齿
    QPen pen; // 调整线条宽度
    pen.setWidth(2);
    pen.setColor(QColor("#404b69"));
    painter.setPen(pen);
    QRectF target(0.0, 0.0, 580.0, 400.0);
    QRectF source(0.0, 0.0, 580.0, 400.0);
    QImage image(":/pic/QTNoGo.png");
    painter.drawImage(target, image, source);

    /*pen.setColor(Qt::black);
    painter.setPen(pen);
    for (int i = 0; i < kBoardSizeNum; i++)
    {
        painter.drawLine(kBoardMargin + kBlockSize * i, kBoardMargin, kBoardMargin + kBlockSize * i, size().height() - kBoardMargin);
        painter.drawLine(kBoardMargin, kBoardMargin + kBlockSize * i, size().width() - kBoardMargin, kBoardMargin + kBlockSize * i);
    }*/
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    // 绘制落子标记(防止鼠标出框越界)
    if (clickPosRow >= 0 && clickPosRow < kBoardSizeNum && clickPosCol >= 0 && clickPosCol < kBoardSizeNum)
    {
        /*if (referee->m_color == WHITE)
            brush.setColor(Qt::white);
        else
            brush.setColor(Qt::black);*/
        brush.setColor(QColor("#404b69"));
        painter.setBrush(brush);
        painter.drawRect(kBoardMargin + kBlockSize * clickPosCol - kMarkSize / 2, kBoardMargin + kBlockSize * clickPosRow - kMarkSize / 2, kMarkSize, kMarkSize);
    }

    pen.setWidth(5);
    // 绘制棋子
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

    // 棋盘边缘不能落子
    /*if (x >= kBoardMargin + kBlockSize / 2 &&
            x < size().width() - kBoardMargin &&
            y >= kBoardMargin + kBlockSize / 2 &&
            y < size().height()- kBoardMargin)
    {*/
    // 获取最近的左上角的点
    int col = (x-kBoardMargin+kBlockSize-10) / kBlockSize;
    int row = (y-kBoardMargin+kBlockSize-10) / kBlockSize;

    double leftTopPosX = kBoardMargin + kBlockSize * col;
    double leftTopPosY = kBoardMargin + kBlockSize * row;

    // 根据距离算出合适的点击位置,一共四个点，根据半径距离选最近的
    clickPosRow = -1; // 初始化最终的值
    clickPosCol = -1;
    double len = 0; // 计算完后取整就可以了

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
    if(clickPosCol == -1 || clickPosRow == -1)
    {
        qDebug() << "OUT_OF_RANGE";
        return;
    }
    if(referee->m_player != BOT)
    {
        Action a;
        a.x = clickPosRow;
        a.y = clickPosCol;
        referee->m_cur_action = a;
        referee->judge();
        update();
    }
}
