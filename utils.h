#ifndef UTILS_H
#define UTILS_H

/*
*   共用的方法及变量
*   @author: epcm
*   @time: 2020/12/13
*/

//游戏模式
enum GameMode{PVP, PVC, CVC, NOCHOICE};
// 玩家身份标识
enum Player{HUMAN, BOT};
// 颜色标识
enum Color{BLACK = 1, WHITE = -1};
// 合法的移动方式
const int DX[] = {-1, 0, 1, 0};
const int DY[] = {0, -1, 0, 1};
// 用于记录一步操作的结构体
struct Action
{
    int x = -1;
    int y = -1;
};

#endif // UTILS_H
