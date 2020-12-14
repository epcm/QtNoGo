#include "ai_mcts.h"

ai_mcts::ai_mcts(QObject *parent) : QObject(parent)
{

}
// 不围棋（NoGo）
// MCTS策略
// 作者：AAA
// 游戏信息：http://www.botzone.org/games#NoGo
//v3 继承自v1 expand节点选择改为随机法,简单的inline

#include <cstdio>
#include <cstring>
#include <iostream>
#include <random>
#include <string>
#include <vector>
using namespace std;

#define TIME_OUT_SET 0.98

int board[9][9] = {0};
int node_count = 0;

/******************规则部分**********************/
bool dfs_air_visit[9][9] = {0};
const int cx[] = {-1, 0, 1, 0};
const int cy[] = {0, -1, 0, 1};

inline bool inBorder(int x, int y) { return x >= 0 && y >= 0 && x < 9 && y < 9; }

/*******************MCTS实现********************/

struct Action
{
    int x = -1;
    int y = -1;
};

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
double State::quickEvaluate()
{
    int n1 = 0, n2 = 0;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (judgeAvailable(i, j))
                n1++;
    col = -col;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (judgeAvailable(i, j))
                n2++;
    col = -col;
    return (n2 - n1) / 81.0;
}
bool State::dfsAir(int fx, int fy)
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
                    flag = true;
        }
    }
    return flag;
}

bool State::judgeAvailable(int fx, int fy)
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

bool State::isTerminal()
{
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (judgeAvailable(i, j))
                return false;
    return true;
}
void State::getAviliableAction()
{
    available_choices.clear();
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (judgeAvailable(i, j))
            {
                Action a;
                a.x = i;
                a.y = j;
                available_choices.push_back(a);
            }
}
void State::generateNextState()
{
    if (available_choices.size() == 0) //修改
        return;
    int n = available_choices.size();
    int i = rand() % n;
    Action a = available_choices[i];
    current_board[a.x][a.y] = col;
    col = -col;
    getAviliableAction();
}

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
bool Node::isAllExpanded()
{
    return state.available_choices.empty();
}

//使用UCB算法，权衡exploration和exploitation后选择得分最高的子节点，注意如果是预测阶段直接选择当前Q值得分最高的。
Node *bestChild(Node *node, bool is_explor)
{
    double max_score = -2e50;//参数开始设成了2e-50，难怪会返回NULL
    Node *best_child = NULL;
    double C = 0.0;
    if (is_explor)
        C = 1 / sqrt(2);
    for (int i = 0; i < (int)(node->children.size()); i++) //Key!!!!加了1防止除0
    {
        Node *p = node->children[i];
        double score = p->quality_value / (p->visit_times) + 2 * C * sqrt(log(2 * node->visit_times) / (p->visit_times));
        if (score > max_score)
        {
            max_score = score;
            best_child = p;
        }
    }
    return best_child;
}

Node *expand(Node *node)
{
    Node *new_node = new Node;
    int i = rand() % node->state.available_choices.size();
    Action a = node->state.available_choices[i];
    node->state.available_choices.erase(node->state.available_choices.begin()+i); //清除已经展开的节点
    *new_node = *node;
    new_node->state.col = -node->state.col;
    /*new_node->quality_value = 0.0;
    new_node->visit_times = 0;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            new_node->state.current_board[i][j] = node->state.current_board[i][j];*/
    new_node->state.current_board[a.x][a.y] = node->state.col;
    new_node->state.getAviliableAction();
    new_node->parent = node;
    node->children.push_back(new_node);
    return new_node;
}

Node *treePolicy(Node *node)
{
    //Selection与Expansion阶段。传入当前需要开始搜索的节点，根据UCB1值返回最好的需要expend的节点，注意如果节点是叶子结点直接返回。
    //基本策略是先找当前未选择过的子节点，如果有多个则随机选。如果都选择过就找权衡过exploration/exploitation的UCB值最大的，如果UCB值相等则随机选。
    if (node->state.available_choices.empty() && node->children.empty()) //当treePolicy到达叶节点时(node->state.available_choices.empty() && node->children.empty())
    {
        //cout << "leaf";
        return node;
    }
    if (node->isAllExpanded())
    {
        Node *p = bestChild(node, true);
        return treePolicy(p);
    }

    else
        return expand(node);
}

//Simulation阶段，从当前节点快速落子模拟运算至终局，返回reward
inline double defaultPolicy(Node *node)
{
    /*State simu_state = node->state;
    simu_state.col = node->state.col;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            simu_state.current_board[i][j] = node->state.current_board[i][j];
    simu_state.getAviliableAction();
    int curCol = simu_state.col;
    while (!simu_state.isTerminal())
        simu_state.generateNextState();
    if (simu_state.col == curCol)
        return 1;
    else
        return -1;*/
    return node->state.quickEvaluate();
}

//蒙特卡洛树搜索的Backpropagation阶段，输入前面获取需要expend的节点和新执行Action的reward，反馈给expend节点和上游所有节点并更新对应数据。
void backup(Node *node, double reward)
{
    Node *p = node;
    while (p)
    {
        p->visit_times++;
        p->quality_value += reward;
        reward = -reward;
        p = p->parent;
    }
}

//MCTS的主体部分
/*实现蒙特卡洛树搜索算法，传入一个根节点，在有限的时间内根据之前已经探索过的树结构expand新节点和更新数据，然后返回只要exploitation最高的子节点。
  蒙特卡洛树搜索包含四个步骤，Selection、Expansion、Simulation、Backpropagation。
  前两步使用tree policy找到值得探索的节点。
  第三步使用default policy也就是在选中的节点上随机算法选一个子节点并计算reward。
  最后一步使用backup也就是把reward更新到所有经过的选中节点的节点上。
  进行预测时，只需要根据Q值选择exploitation最大的节点即可，找到下一个最优的节点。*/

int main()
{
    srand((unsigned)time(0));
    string str;
    int x, y;
    // 读入JSON
    getline(cin, str);
    //getline(cin, str);

    //计时开始并计算时限
    int start = clock();
    int timeout = (int)(TIME_OUT_SET * (double)CLOCKS_PER_SEC);

    Json::Reader reader;
    Json::Value input;
    reader.parse(str, input);

    //初始化节点
    int first = 0;
    Node *node = new Node;
    if (input["requests"][first]["x"].asInt() == -1) //确定颜色
        node->state.col = 1;                         //黑棋
    else
        node->state.col = -1;

    int color = node->state.col;
    // 分析自己收到的输入和自己过往的输出，并恢复状态
    int turnID = input["responses"].size();
    for (int i = 0; i < turnID; i++)
    {
        x = input["requests"][i]["x"].asInt(), y = input["requests"][i]["y"].asInt();
        if (x != -1)
        {
            board[x][y] = -color;
        }
        x = input["responses"][i]["x"].asInt(), y = input["responses"][i]["y"].asInt();
        if (x != -1)
        {
            board[x][y] = color;
        }
    }
    x = input["requests"][turnID]["x"].asInt(), y = input["requests"][turnID]["y"].asInt();
    if (x != -1) //对方为黑子
    {
        board[x][y] = -color;
        node_count++;
    }

    for (int i = 0; i < 9; i++) //载入棋盘
        for (int j = 0; j < 9; j++)
            node->state.current_board[i][j] = board[i][j];
    /*//输出棋盘调试
    for (int i = 0; i < 9; i++) //载入棋盘
    {
        for (int j = 0; j < 9; j++)
             printf("%3d", node->state.current_board[j][i]) ;
        cout << endl;
    }*/
    node->state.getAviliableAction();

    //开始蒙特卡洛树搜索
    while (clock() - start < timeout)
    {
        /*if(clock()%50==0)
        {
            cout << endl;
            for(auto p:node->children)
                cout << p->quality_value << '&' << p->visit_times << ' ';
        }*/
        //cout << "cal\n";
        node_count++;
        Node *expand_node = treePolicy(node);
        double reward = defaultPolicy(expand_node);
        backup(expand_node, reward);
    }

    //输出结果
    Json::Value ret;
    Json::Value action;

    //黑棋开局不能下在天元
    Node *best_child = bestChild(node, false);
    if (x == -1)
    {
        double max_score = 2e-50;
        for (int i = 0; i < (int)node->children.size(); i++)
        {
            Node *p = node->children[i];
            if (p->state.current_board[4][4] == 1)
                continue;
            double score = p->quality_value / p->visit_times;
            if (score > max_score)
            {
                max_score = score;
                best_child = p;
            }
        }
    }

    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (board[i][j] != best_child->state.current_board[i][j])
            {
                action["x"] = i;
                action["y"] = j;
                break;
            }
    ret["response"] = action;
    //char buffer[4096];
    //sprintf(buffer, "MCTS节点数:%d,当前预估胜率:%.3f", node_count, ((double)(root.children[maxI]->q)) / ((double)root.children[maxI]->n));
    //ret["debug"] = buffer;
    Json::FastWriter writer;
    char buffer[4096];
    sprintf(buffer, "MCTS节点数:%d,当前预估胜率:%.3f", node_count, ((double)(best_child->quality_value)) / ((double)best_child->visit_times) + 0.5);
    ret["debug"] = buffer;
    cout << writer.write(ret) << endl;
    //system("pause");
    return 0;
}
