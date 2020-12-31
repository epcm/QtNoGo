#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <referee.h>
#include "utils.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Referee* pReferee = new Referee;
private:
    Ui::MainWindow *ui;
    int clickPosRow, clickPosCol; // 存储将点击的位置
protected:
    // 绘制
    void paintEvent(QPaintEvent *event);
    // 监听鼠标移动情况，方便落子
    void mouseMoveEvent(QMouseEvent *event);
    // 实际落子
    void mouseReleaseEvent(QMouseEvent *event);
private slots:
    void on_pushButton_clicked();
    void startUpdate();
};
#endif // MAINWINDOW_H
