#include "referee.h"
#include <ctime>
#include <QProcess>
#include <QDebug>
#include <QTimer>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include "hintwidget.h"

/*
//*   @file: referee.cpp
*   @brief: 这是裁判类实现
*           实现游戏的逻辑控制，包括进程调度，局面判断
*   @author: epcm
*   @time: 2020/12/13
*/

// 构造函数
Referee::Referee(QObject *parent) : QObject(parent)
{
    memset(m_board, 0, sizeof(m_board));
    m_game_mode = PVC;
    m_player = HUMAN;
    connect(m_timer, &QTimer::timeout, this, &Referee::judgeTime);
    connect(m_replay_timer, &QTimer::timeout, this, &Referee::replayByHistory);
    connect(this, &Referee::botFinishedSignal, this, &Referee::judge);
}

void Referee::changeColor()
{
    if(m_color == BLACK)
        m_color = WHITE;
    else
        m_color = BLACK;
    return;
}


void Referee::judgeTime()
{    
    if((m_player == HUMAN && clock() - m_start_time > m_human_time_limit*1000)
            ||(m_player == BOT && clock() - m_start_time > m_bot_time_limit*1000))
    {
        if(m_color == BLACK)
            emit displayHintSignal(0);
        else if(m_color == WHITE)
            emit displayHintSignal(1);
        endGame();
    }
    emit updateSignal();
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
    else if(x >=0 && x < 9 && y >= 0 && y < 9)
    {
        qDebug() << tr("INVALID_POS");
        emit displayHintSignal(2);
        endGame();
        return;
    }
    if(judgeWin())
    {
        // 送信报道胜者
        qDebug() << tr("winner is:") << m_color;
        switch (m_color)
        {
        case BLACK:
            emit displayHintSignal(3);
            break;
        case WHITE:
            emit displayHintSignal(4);
            break;
        default:
            break;
        }
        endGame();
        return;
    }
    changeColor();
    m_start_time = clock();
    if(m_player == BOT)
        m_player = HUMAN;
    else if(m_player == HUMAN && m_game_mode == PVC)
    {
        m_player = BOT;
        botOneStep();
    }
}

void Referee::endGame()
{
    m_timer->stop();
    m_player = NEITHER;
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

void Referee::botOneStep()
{
    //QTimer::singleShot(1000*m_bot_time_limit, this, SLOT(judge()));
    AIMCTS ai;
    m_cur_action =  ai.aiAction(m_color, m_board);
    judgeTime();
    emit botFinishedSignal();
}


// 设定BOT时限
void Referee::setBotTimeLimit(const QString &text)
{
    m_bot_time_limit = text.toInt();
}
// 设定HUMAN时限
void Referee::setHumanTimeLimit(const QString &text)
{
    m_human_time_limit = text.toInt();
}
// 设定游戏类型
void Referee::setGameMode(int index)
{
    if(index == 0)
    {
        m_game_mode = PVC;
    }
    else if(index == 1)
    {
        m_game_mode = PVP;
    }
}
// 设定先手方
void Referee::setFirstPlayer(int index)
{
    if(index == 0)
    {
        m_player = HUMAN;
        m_first_player = HUMAN;
    }
    else if(index == 1)
    {
        m_player = BOT;
        m_first_player = BOT;
    }
}

// 重置
void Referee::resetReferee()
{
    memset(m_board, 0, sizeof(m_board));
    m_player = m_first_player;
    m_color = BLACK;
    while(!m_history.empty())
        m_history.pop_back();
    m_timer->stop();
}

void Referee::changePlayer()
{
    if(m_game_mode == PVC)
    {
        if(m_player == BOT)
            m_player = HUMAN;
        else if(m_player == HUMAN)
            m_player = BOT;
    }
}
void Referee::setBoardByHistory()
{
    memset(m_board, 0, sizeof(m_board));
    m_color = BLACK;
    for(int i = 0; i < m_history.size(); i++)
    {
        int x, y;
        QJsonArray arr = m_history[i].toArray();
        QVariantList vl = arr.toVariantList();
        x = vl[0].toInt();
        y = vl[1].toInt();
        switch (m_color)
        {
        case BLACK:
            m_board[x][y] = 1;
            break;
        case WHITE:
            m_board[x][y] = -1;
        default:
            break;
        }
        changeColor();
        changePlayer();
    }

}

void Referee::saveGame()
{
    QJsonObject gameObject;
    gameObject["FirstPlayer"] = m_first_player;
    gameObject["HumanTimeLimit"] = m_human_time_limit;
    gameObject["BotTimeLimit"] = m_bot_time_limit;
    gameObject["GameMode"] = m_game_mode;
    gameObject["History"] = m_history;
    QString fileName = QFileDialog::getSaveFileName(nullptr,tr("Save Game"), "",tr("JSON (*.json)"));
    if (fileName.isEmpty())
        return;
    else
    {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::information(nullptr, tr("Unable to open file"),file.errorString());

            return;
        }
        file.write(QJsonDocument(gameObject).toJson());
        file.close();
    }
}

void Referee::loadGame()
{
    resetReferee();
    QString fileName = QFileDialog::getOpenFileName(nullptr,tr("Load Game"), "",tr("JSON (*.json)"));
    if (fileName.isEmpty())
        return;
    else
    {
        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::information(nullptr, tr("Unable to open file"),file.errorString());

            return;
        }

        QByteArray saveData = file.readAll();

        QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

        QJsonObject loadData = loadDoc.object();


        if(m_game_mode != REPLAY)
        {
            switch (loadData["GameMode"].toInt())
            {
            case PVP:
                m_game_mode = PVP;
                break;
            case PVC:
                m_game_mode = PVC;
            default:
                break;
            }
        }

        switch (loadData["FirstPlayer"].toInt())
        {
        case HUMAN:
            m_first_player = HUMAN;
            break;
        case BOT:
            m_first_player = BOT;
        default:
            break;
        }

        m_human_time_limit = loadData["HumanTimeLimit"].toDouble();
        m_bot_time_limit = loadData["BotTimeLimit"].toDouble();
        m_history = loadData["History"].toArray();
        m_time_when_paused = 0;
        m_start_time = clock();


        if(m_game_mode == REPLAY)
        {
            m_replay_timer->start(REPLAYSPEED*1000);
        }
        else
        {
            setBoardByHistory();
            emit pauseSignal();
        }
    }
}

void Referee::endReplay()
{
    m_replay_timer->stop();
}

void Referee::continueReplay()
{
    m_replay_timer->start(REPLAYSPEED*1000);
}

void Referee::replayByHistory()
{
    if(m_replay_turn == m_history.size())
    {
        endReplay();
        return;
    }
    int x, y;
    QJsonArray arr = m_history[m_replay_turn++].toArray();
    QVariantList vl = arr.toVariantList();
    x = vl[0].toInt();
    y = vl[1].toInt();
    switch (m_color)
    {
    case BLACK:
        m_board[x][y] = 1;
        break;
    case WHITE:
        m_board[x][y] = -1;
    default:
        break;
    }
    emit updateSignal();
    changeColor();
    changePlayer();
}
