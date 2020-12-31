#ifndef AIMCTS_H
#define AIMCTS_H

#include <QObject>
#include <vector>
#include <utils.h>
using namespace std;

#define TIME_OUT_SET 0.99


class State
{
    //Node的棋盘状态记录：包括评估值current_value,盘面current_board
    //实现有判断是否达terminal；simulation阶段的单步default policy
public:
    int current_board[9][9] = {0};
    int col = 0;
    vector<Action> available_choices;
    bool isTerminal(); //判断是否终局
    void getAviliableAction();
    void generateNextState(); //随机生成到下一状态
    bool dfsAir(int fx, int fy);
    bool judgeAvailable(int fx, int fy);
    double quickEvaluate();
};
class Node
{
public:
    Node *parent = NULL;
    vector<Node *> children;
    int visit_times = 0;
    double quality_value = 0.0;
    State state;
    bool isAllExpanded();
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
Node *bestChild(Node *node, bool is_explor);
Node *expand(Node *node);
Node *treePolicy(Node *node);
double defaultPolicy(Node *node);
void backup(Node *node, double reward);

#endif // AIMCTS_H
