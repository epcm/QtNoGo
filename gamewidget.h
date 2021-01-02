#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include "referee.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QVBoxLayout>
namespace Ui {
class GameWidget;
}

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);
    ~GameWidget();
    Referee* referee = new Referee;
    QMenuBar* menubar;
    // hint弹窗
    HintWidget* h;
signals:
    void display(int num);
    void showGameSignal(int number);
    void showWelcomeSignal(int number);
private:
    Ui::GameWidget *ui;
    int clickPosRow, clickPosCol; // 存储将点击的位置
protected:
    // 绘制
    void paintEvent(QPaintEvent *event);
    // 监听鼠标移动情况，方便落子
    void mouseMoveEvent(QMouseEvent *event);
    // 实际落子
    void mouseReleaseEvent(QMouseEvent *event);
public slots:
    void startUpdate();
    void initGame();
private slots:
    void on_RestartPushButton_clicked();
    void on_PausePushButton_clicked();
    void on_ReactPushButton_clicked();
    void on_HintPushButton_clicked();
    void on_ReturnPushButton_clicked();
    void displayHint(int number);
};

#endif // GAMEWIDGET_H
