#include "referee.h"
#include <ctime>
#include <QProcess>
#include <QDebug>
#include <QTimer>

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

void Referee::changeColor()
{
    if(m_color == BLACK)
        m_color = WHITE;
    else
        m_color = BLACK;
    return;
}
// 进行一个回合
// 包括送信、收信、判断、更新三个阶段
void Referee::oneTurn()
{
    /*if(m_player == BOT)
    {
        m_cur_action = botOneStep();
        m_player = HUMAN;
        QTimer::singleShot(1000, this, SLOT(judge()));
    }
    else    // 等待人类响应
    {
        int start = clock();
        while(m_player == HUMAN &&  (clock()-start < m_time_limit * CLOCKS_PER_SEC))
        {
            // 空循环定时
        }
        if(m_player == HUMAN)
        {
            // 超时处理
            qDebug() << tr("TLE");
            return;
        }

    }*/

}

void Referee::judge()
{
    int x = m_cur_action.x;
    int y = m_cur_action.y;
    if(judgeAvailable(x, y))
    {
        QJsonArray arr = {x, y};
        QJsonValue value(arr);
        m_history.append(value);
        if(m_color == BLACK)
            m_board[m_cur_action.x][m_cur_action.y] = 1;
        else
            m_board[m_cur_action.x][m_cur_action.y] = -1;
        emit updateSignal();
    }
    else
    {
        qDebug() << tr("INVALID_POS");
        return;
        //pass;对非法放置的处理
    }
    if(judgeWin())
    {
        // 送信报道胜者
        qDebug() << tr("winner is:") << m_color;
        return;
    }
    changeColor();
    if(m_player == BOT)
        m_player = HUMAN;
    else if(m_player == HUMAN)
    {
        m_player = BOT;
        botOneStep();
    }
}

bool Referee::inBorder(int x, int y)
{
    return x >= 0 && y >= 0 && x < 9 && y < 9;
}

bool Referee::judgeWin()
{
    changeColor();
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (judgeAvailable(i, j))
            {
                changeColor();
                return false;
            }
    changeColor();
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

Action Referee::botOneStep()
{
    QTimer::singleShot(1000, this, SLOT(judge()));
    AIMCTS ai;
    m_cur_action =  ai.aiAction(m_color, m_board);
    return m_cur_action;
}
