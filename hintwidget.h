#ifndef HINTWIDGET_H
#define HINTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <Qt>
#include <QTimer>

class HintWidget : public QLabel
{
    Q_OBJECT
public:
    explicit HintWidget(QString text="",QWidget *parent = Q_NULLPTR);
    QTimer* timer;
private slots:
    void onTimeDestroy();

};

#endif // HINTWIDGET_H
