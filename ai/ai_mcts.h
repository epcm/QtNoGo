#ifndef AI_MCTS_H
#define AI_MCTS_H

#include <QObject>
#include <QObject>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <utils.h>
using namespace std;

class ai_mcts : public QObject
{
    Q_OBJECT
public:
    /*explicit ai_mcts(QObject *parent = nullptr);
    double time_out_set = 0.98;
    int board[9][9] = {0};
    int node_count = 0;
    bool dfs_air_visit[9][9] = {0};
    const int cx[] = {-1, 0, 1, 0};
    const int cy[] = {0, -1, 0, 1};
    bool inBorder(int x, int y);*/

    Action botOneStep();
signals:
};

/*class State
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

Node *treePolicy(Node *node);
double defaultPolicy(Node *node);
void backup(Node *node, double reward);*/

#endif // AI_MCTS_H
