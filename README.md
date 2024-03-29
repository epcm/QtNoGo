# 基于Qt的不围棋对弈平台
从右上角下载release版本，可打开exe文件直接玩耍

算法原理见：https://github.com/epcm/QtNoGo/blob/master/report.pdf

## 效果展示

![](https://raw.githubusercontent.com/epcm/Pictures/master/Markdown/2.1.jpg)

![](https://raw.githubusercontent.com/epcm/Pictures/master/Markdown/2.2.jpg)

![](https://raw.githubusercontent.com/epcm/Pictures/master/Markdown/2.3.jpg)

![](https://raw.githubusercontent.com/epcm/Pictures/master/Markdown/2.4.jpg)

## 关于不围棋

不围棋是比围棋简单的游戏

### 规则

**目标**：想方设法不要让自己提走对方的棋子（围住），尽可能让自己的棋子被对方提走（围住）。

**什么是“气”**：要理解“提走”，首先要理解什么是“气”。一个棋子在棋盘上，与它直线紧邻的空点是这个棋子的“气”。 棋子直线紧邻的点上，如果有同色棋子存在，则它们便相互连接成一个不可分割的整体。它们的气也应一并计算。

**什么是“提走”**：当一个棋子没有“气”的时候它就要被提走。棋子直线紧邻的点上，如果有异色棋子存在，这口气就不复存在。如所有的气均为对方所占据，便呈无气状态。无气状态的棋子不能在棋盘上存在，也就是提子。把无气之子提出盘外的手段叫“提子”。

**棋盘的规格**：如图所示，不围棋的棋盘大小是**9\*9**。注意，这里的9*9指的是格点的数目，并不是格子的数量，因为棋子要下在格点上。

**落子先后**：黑子先手，双方轮流落子，落子后棋子不可移动。

**判负条件**：不围棋**没有**平局。一方输掉比赛，当且仅当以下至少一条发生：

1）如果一方落子后吃掉了对方的棋子，则落子一方判负；

2）对弈禁止自杀，落子自杀一方判负；

3）对弈禁止空手(pass)，空手一方判负。

## 玩耍方式

请首先安装字体文件：`arlrdbd.ttf`

之后双击`NoGo.exe`即可开始游戏

如界面大小异常，请调整windows显示缩放或更换设备

建议设备参数: windows 10，2k分辨率
