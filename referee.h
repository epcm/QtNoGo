#ifndef REFEREE_H
#define REFEREE_H

#include <QObject>
#include <vector>
#include <utils.h>
#include "./ai/aimcts.h"
// json相关库
#include<qjsondocument.h>
#include<qjsonarray.h>
#include<qjsonobject.h>
#include<qjsonvalue.h>

/*
*   @file: referee.h
*   @brief: 这是裁判类声明，
*           实现游戏的逻辑控制，包括进程调度，局面判断
*   @author: epcm
*   @time: 2020/12/13
*/

class Referee : public QObject
{
    Q_OBJECT
public:
    explicit Referee(QObject *parent = nullptr);

signals:
    void updateSignal();
public slots:
    // 判断游戏局面状况
    void judge();
    // 根据游戏类型信号初始化游戏
    void initGame(int number);
public:
    // 当前盘面
    int m_board[9][9] = {0};
    // 历史记录
    QJsonArray m_history;
    // 当前的游戏模式
    GameMode m_game_mode = NOCHOICE;
    // 处于当前回合的玩家属性
    Player m_player = HUMAN;
    // 处于当前回合的落子颜色
    Color m_color = BLACK;
    // 进行一个回合
    void oneTurn();
    // 当前的行动
    Action m_cur_action;

private:
    // 回合中得到的respond
    QJsonObject m_jsonobj;
    // 辅助判断可行性的数组，记录某一位置是否已经被访问过
    bool m_dfs_air_visit[9][9] = {0};
    // 判断位置是否在棋盘内
    bool inBorder(int x, int y);
    // 判断本回合是否决出胜负（即本回合落子者是否取胜）
    bool judgeWin();
    // 判断某一位置是否有气
    bool dfsAir(int x, int y);
    // 判断某一位置是否为合法落子点
    bool judgeAvailable(int x, int y);
    // 传数据给AI并获得输出,改变盘面
    Action botOneStep();
    // 颜色对换
    void changeColor();
};

#endif // REFEREE_H
