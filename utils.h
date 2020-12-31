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
const int kBoardSizeNum = 9;
const int kBoardMargin = 30; // 棋盘边缘空隙
const int kRadius = 15; // 棋子半径
const int kMarkSize = 6; // 落子标记边长
const int kBlockSize = 40; // 格子的大小
const int kPosDelta = 20; // 鼠标点击的模糊距离上限
#endif // UTILS_H
