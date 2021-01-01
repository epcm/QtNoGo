#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gamewidget.h"
#include "welcomewidget.h"
#include "settingdialog.h"
#include <QStackedLayout>
#include <QVBoxLayout>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void showSettingSignal();
private:
    GameWidget* gamewidget;
    WelcomeWidget* welcomewidget;
    SettingDialog* settingdialog;

    QStackedLayout *stackLayout;
    QVBoxLayout *mainLayout;

};
#endif // MAINWINDOW_H
