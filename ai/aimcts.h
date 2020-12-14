#ifndef AIMCTS_H
#define AIMCTS_H

#include <QObject>
#include <vector>
#include <utils.h>

class AIMCTS : public QObject
{
    Q_OBJECT
public:
    explicit AIMCTS(QObject *parent = nullptr);
    Action aiAction(int color, vector<vector<int>> board);
signals:

};

#endif // AIMCTS_H
