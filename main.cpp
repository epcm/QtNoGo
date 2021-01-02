#include "mainwindow.h"

#include <QApplication>
#include "settingdialog.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    MainWindow w;
    return a.exec();
}
