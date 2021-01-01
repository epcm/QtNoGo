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
    void displaySetting();       //显示配置页的信号
private slots:

    void on_NewGamePushButton_clicked();


private:
    Ui::WelcomeWidget *ui;
};

#endif // WELCOMEWIDGET_H
