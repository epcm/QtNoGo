#include "referee.h"
#include <time.h>
#include <QProcess>

/*
*   @file: referee.cpp
*   @brief: 这是裁判类实现
*           实现游戏的逻辑控制，包括进程调度，局面判断
*   @author: epcm
*   @time: 2020/12/13
*/

// 构造函数
Referee::Referee(QObject *parent) : QObject(parent)
{
    memset(m_board, 0, sizeof(m_board));
    m_game_mode = NOCHOICE;
    m_player = HUMAN;
}

// 进行一个回合
// 包括送信、收信、判断、更新三个阶段
void Referee::oneTurn()
{

    if(m_player == BOT)
    {
        m_jsonobj = botOneStep();
    }
    else    // 等待人类响应
    {
        int start = clock();
        while(!m_human_respond &&  (clock()-start < m_time_limit * CLOCKS_PER_SEC))
        {
            // 空循环定时
        }
        if(!m_human_respond);
            // 超时处理
    }
    int x = m_jsonobj["response"]["x"];
    int y = m_jsonobj["response"]["y"];
    if(judgeAvailable(x, y))
    {
        QJsonArray arr = {x, y};
        QJsonValue value(arr);
        m_history.append(value);
        m_board[x][y] = m_color;
    }
    else
    {
        pass;// 对非法放置的处理
    }
    if(judgeWin())
    {
        // 送信报道胜者
        return;
    }
    if(m_color == BLACK)
        m_color = WHITE;
    else
        m_color = BLACK;
    oneTurn();
}

bool Referee::inBorder(int x, int y)
{
    return x >= 0 && y >= 0 && x < 9 && y < 9;
}

bool Referee::judgeWin()
{
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (judgeAvailable(i, j))
                return false;
    return true;
}

bool Referee::dfsAir(int x, int y)
{
    m_dfs_air_visit[x][y] = true;
    bool flag = false;
    for (int dir = 0; dir < 4; dir++) //某一位置有气的条件为周围有空格或者有一个邻近的同色有气(反证法进一步证明邻近的有一个有气即都有气)
    {
        int dx = x + DX[dir], dy = y + DY[dir];
        if (inBorder(dx, dy))
        {
            if (m_board[dx][dy] == 0)
                flag = true;
            if (m_board[dx][dy] == m_board[x][y] && !m_dfs_air_visit[dx][dy])
                if (dfsAir(dx, dy))
                    flag = true;
        }
    }
    return flag;
}

bool Referee::judgeAvailable(int x, int y)
{
    if (m_board[x][y])
           return false;
       m_board[x][y] = int(m_color);
       memset(m_dfs_air_visit, 0, sizeof(m_dfs_air_visit));
       if (!dfsAir(x, y)) //判定自杀
       {
           m_board[x][y] = 0;
           return false;
       }
       for (int dir = 0; dir < 4; dir++) //不围对方
       {
           int dx = x + DX[dir], dy = y + DY[dir];
           if (inBorder(dx, dy))
           {
               if (m_board[dx][dy] && !m_dfs_air_visit[dx][dy])
               {
                   if (!dfsAir(dx, dy))
                   {
                       m_board[x][y] = 0;
                       return false;
                   }
               }
           }
       }
       m_board[x][y] = 0;
       return true;
}

void Referee::botOneStep()
{
    AIMCTS ai;
    Action ai.aiAction(m_color, board);
}
