#ifndef AIMCTS_H
#define AIMCTS_H

#include <QObject>
#include <vector>
#include <utils.h>
using namespace std;

#define TIME_OUT_SET 0.98
#define EXPLORE 0.1


class Node
{
    //Node的棋盘状态记录：包括评估值current_value,盘面current_board
    //实现有判断是否达terminal；simulation阶段的单步default policy
public:
    Node();
    signed char current_board[9][9] = {0};
    int col = 0;
    Node *parent = NULL;
    Node *children[81];
    int visit_times = 0;
    int countChildrenNum = 0;
    int maxChildrenNum = 0;
    double quality_value = 0.0;
    int available_choices[81];
    void getAviliableAction();           //得到可行的行动
    bool dfsAir(int fx, int fy);         //判断是否有气
    bool judgeAvailable(int fx, int fy); //判断是否可下
    double quickEvaluate();              //快速估值
    Node *bestChild(double C);
    Node *expand();
    Node *treePolicy();
    double defaultPolicy();
    void backup(double reward);
};


class AIMCTS : public QObject
{
    Q_OBJECT
private:
public:
    explicit AIMCTS(QObject *parent = nullptr);
    Action aiAction(Color color, int board[][9]);
signals:

};

extern int board[9][9];
extern int node_count;
extern bool dfs_air_visit[9][9];
const int cx[4] = {-1, 0, 1, 0};
const int cy[4] = {0, -1, 0, 1};
bool inBorder(int x, int y);

#endif // AIMCTS_H
