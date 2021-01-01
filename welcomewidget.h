#ifndef WELCOMEWIDGET_H
#define WELCOMEWIDGET_H

#include <QWidget>

namespace Ui {
class WelcomeWidget;
}

class WelcomeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomeWidget(QWidget *parent = nullptr);
    ~WelcomeWidget();

signals:
    void display(int number);    //切换页面的信号
    void gameType(int number);   //游戏类型选择的信号
private slots:
    void on_PVCPushButton_clicked();

    void on_PVPPushButton_clicked();

private:
    Ui::WelcomeWidget *ui;
};

#endif // WELCOMEWIDGET_H
