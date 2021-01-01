#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include "referee.h"
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
signals:
    void display(int num);
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
private slots:
    void startUpdate();
};

#endif // GAMEWIDGET_H
