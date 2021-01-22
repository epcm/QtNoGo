#include "aimcts.h"
#include <string>
#include <random>
#include <cstdio>
#include <cstring>
#include <vector>
#include <QDebug>
using namespace std;

int board[9][9] = {0};
int node_count = 0;
bool dfs_air_visit[9][9] = {0};

AIMCTS::AIMCTS(QObject *parent) : QObject(parent)
{
    memset(board,0, sizeof(board));
    node_count = 0;
    memset(dfs_air_visit, 0, sizeof(dfs_air_visit));
}

bool inBorder(int x, int y)
{
    return x >= 0 && y >= 0 && x < 9 && y < 9;
}

Node::Node()
{
    memset(current_board, 0, sizeof(current_board));
    memset(available_choices, 0, sizeof(available_choices));
}

double Node::quickEvaluate()
{
    int n1 = 0, n2 = 0;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
        {
            bool f1 = judgeAvailable(i, j);
            col = -col;
            bool f2 = judgeAvailable(i, j);
            col = -col;
            if (f1 && !f2)
                n1 ++;
            else if (!f1 && f2)
                n2 ++;
        }
    return n2 - n1;
}

bool Node::dfsAir(int fx, int fy)
{
    dfs_air_visit[fx][fy] = true;
    bool flag = false;
    for (int dir = 0; dir < 4; dir++) //某一位置有气的条件为周围有空格或者有一个邻近的同色有气(反证法进一步证明邻近的有一个有气即都有气)
    {
        int dx = fx + cx[dir], dy = fy + cy[dir];
        if (inBorder(dx, dy))
        {
            if (current_board[dx][dy] == 0)
                flag = true;
            if (current_board[dx][dy] == current_board[fx][fy] && !dfs_air_visit[dx][dy])
                if (dfsAir(dx, dy))
                {
                    flag = true;

                }
        }
    }
    return flag;
}

bool Node::judgeAvailable(int fx, int fy)
{
    if (current_board[fx][fy])
        return false;
    current_board[fx][fy] = col;
    memset(dfs_air_visit, 0, sizeof(dfs_air_visit));
    if (!dfsAir(fx, fy)) //判定自杀
    {
        current_board[fx][fy] = 0;
        return false;
    }
    for (int dir = 0; dir < 4; dir++) //不围对方
    {
        int dx = fx + cx[dir], dy = fy + cy[dir];
        if (inBorder(dx, dy))
        {
            if (current_board[dx][dy] && !dfs_air_visit[dx][dy])
            {
                if (!dfsAir(dx, dy))
                {
                    current_board[fx][fy] = 0;
                    return false;
                }
            }
        }
    }
    current_board[fx][fy] = 0;
    return true;
}

void Node::getAviliableAction()
{
    memset(available_choices, 0, sizeof(available_choices));
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (judgeAvailable(i, j))
            {
                int act = i * 9 + j;
                available_choices[maxChildrenNum++] = act;
            }
}

//使用UCB算法，权衡exploration和exploitation后选择得分最高的子节点，注意如果是预测阶段直接选择当前Q值得分最高的。
Node *Node::bestChild(double C)
{
    double max_score = -2e50; //参数开始设成了2e-50，难怪会返回NULL
    Node *best_child = NULL;
    for (int i = 0; i < countChildrenNum; i++)
    {
        Node *p = children[i];
        double score = p->quality_value / (p->visit_times) + 2 * C * sqrt(log(2 * visit_times) / (p->visit_times));
        if (score > max_score)
        {
            max_score = score;
            best_child = p;
        }
    }
    return best_child;
}

Node *Node::expand()
{

    int a = available_choices[countChildrenNum];
    int x = a / 9;
    int y = a % 9;
    Node *new_node = new Node;
    children[countChildrenNum++] = new_node;
    new_node->parent = this;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            new_node->current_board[i][j] = current_board[i][j];
    new_node->col = -col;
    new_node->current_board[x][y] = col;
    new_node->getAviliableAction();
    return new_node;
}

Node *Node::treePolicy()
{
    //Selection与Expansion阶段。传入当前需要开始搜索的节点，根据UCB1值返回最好的需要expend的节点，注意如果节点是叶子结点直接返回。
    //基本策略是先找当前未选择过的子节点，如果有多个则随机选。如果都选择过就找权衡过exploration/exploitation的UCB值最大的，如果UCB值相等则随机选。
    if (maxChildrenNum == 0) //当treePolicy到达叶节点时(node->state.available_choices.empty() && node->children.empty())
    {
        //cout << "leaf";
        return this;
    }
    if (countChildrenNum >= maxChildrenNum)
    {
        Node *p = bestChild(EXPLORE);
        return p->treePolicy();
    }
    else
        return expand();
}

//Simulation阶段，从当前节点快速落子模拟运算至终局，返回reward
double Node::defaultPolicy()
{
    return quickEvaluate();
}

//蒙特卡洛树搜索的Backpropagation阶段，输入前面获取需要expend的节点和新执行Action的reward，反馈给expend节点和上游所有节点并更新对应数据。
void Node::backup(double reward)
{
    Node *p = this;
    while (p)
    {
        p->visit_times++;
        p->quality_value += reward;
        reward = -reward;
        p = p->parent;
    }
}

Action AIMCTS::aiAction(Color color, int board[][9])
{
    srand((unsigned)time(0));
    string str;
    int x, y;

    //计时开始并计算时限
    int start = clock();
    int timeout = (int)(TIME_OUT_SET * (double)CLOCKS_PER_SEC);


    //初始化节点
    int first = 0;
    Node *node = new Node;
    if (color == BLACK) //确定颜色
        node->col = 1;                         //黑棋
    else
        node->col = -1;

    for (int i = 0; i < 9; i++) //载入棋盘
        for (int j = 0; j < 9; j++)
            node->current_board[i][j] = board[i][j];
    node->getAviliableAction();

    //开始蒙特卡洛树搜索
    while (clock() - start < timeout)
    {
        node_count++;
        Node *expand_node = node->treePolicy();
        double reward = expand_node->defaultPolicy();
        expand_node->backup(reward);
    }

    Node *best_child = node->bestChild(0);
    Action a;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (board[i][j] != best_child->current_board[i][j])
            {
                a.x = i;
                a.y = j;
                break;
            }
    qDebug()<< "MCTS_Nodes"<< node_count;
    return a;
}
