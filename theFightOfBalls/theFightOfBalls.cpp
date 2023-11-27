#include <easyx.h>
#include <graphics.h>
#include <iostream>
#include <math.h>
//----------以下两个用于播放音乐
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
//----------以下两个头文件用于设置随机数种子
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 640
#define FOOD_NUM 200


/* 球球大作战中都是圆，玩家控制的是圆，吃的也是圆。
	圆需要的属性：圆心坐标，半径，颜色，是否存在的标志（被吃掉就不在了）
	以上属性可以用结构体或类表示 */

class Ball
{
public:
	float x, y;				//圆心坐标
	double r;					//半径
	unsigned long color;	//颜色
	bool exist;				//存在的标志：1代表存在 0代表不存在
};

Ball foods[FOOD_NUM];		//声明可以吃的圆
Ball player;				//声明玩家的圆

void GameInit()
{
	srand((unsigned int)time(NULL));		//设置随机数种子(set random)，参数应为不断变化的量，故选取时间作为参数

	//初始化食物------------------------------------------------------------------------------------
	for (int i = 0; i < FOOD_NUM; i++)
	{
		foods[i].x = rand() % SCREEN_WIDTH;		//随机化每个食物的坐标，且不能超出屏幕大小
		foods[i].y = rand() % SCREEN_HEIGHT;
		foods[i].exist = true;					//一开始食物都存在
		foods[i].r = rand() % 8 + 1;			//半径不能过大，且不能为0
		foods[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);	//RGB：即red green blue三原色，每个颜色的最大值为255
	}

	//初始化玩家------------------------------------------------------------------------------------
	player.x = rand() % SCREEN_WIDTH;
	player.y = rand() % SCREEN_HEIGHT;
	player.r = 18;
	player.exist = true;					
	player.color = RGB(255, 180, 210);

}

void GameDraw()
{
	BeginBatchDraw();	//与EndBatchDraw()一起起到防止闪屏的功能

	setbkcolor(WHITE);	//与清屏函数出现在一起才能设置背景颜色
	cleardevice();		//清屏

	//绘制食物------------------------------------------------------------------
	for (int i = 0; i < FOOD_NUM; i++)
	{
		if (foods[i].exist)	//判断：若圆存在就画出来
		{
			setfillcolor(foods[i].color);						//设置填充的颜色
			solidcircle(foods[i].x, foods[i].y, foods[i].r);	//画一个填充的圆
		}
	}	

	//绘制玩家------------------------------------------------------------------
	setfillcolor(player.color);
	solidcircle(player.x, player.y, player.r);	//画一个填充的圆
	settextcolor(RED);
	settextstyle(15, 0, L"宋体");
	setbkmode(0);		//设置文字背景透明
	outtextxy(player.x - 23, player.y - 35, L"player");

	EndBatchDraw();
}

void KeyControl(float speed)		//键盘控制玩家移动
{
	if (GetAsyncKeyState('w') || GetAsyncKeyState('W') || GetAsyncKeyState(VK_UP))
	{
		player.y = player.y - speed;
	}
	if (GetAsyncKeyState('s') || GetAsyncKeyState('S') || GetAsyncKeyState(VK_DOWN))
	{
		player.y = player.y + speed;
	}
	if (GetAsyncKeyState('a') || GetAsyncKeyState('A') || GetAsyncKeyState(VK_LEFT))
	{
		player.x = player.x - speed;
	}
	if (GetAsyncKeyState('d') || GetAsyncKeyState('D') || GetAsyncKeyState(VK_RIGHT))
	{
		player.x = player.x + speed;
	}
}

double distance(Ball a, Ball b)	//求圆心之间的距离
{
	return sqrt( (double)(a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) );
}

void EatFood()
{
	for (int i = 0; i < FOOD_NUM; i++)
	{
		if (foods[i].exist && distance(foods[i], player) < player.r)
		{
			foods[i].exist = false;
			player.r = player.r + foods[i].r / 4;
		}
	}
}

int main()
{
	initgraph(SCREEN_WIDTH, SCREEN_HEIGHT);		//初始化窗口，坐标的原点在窗口左上角，向右为x轴正方向，向下为y轴正方向
	GameInit();

	while (1)
	{
		KeyControl(0.2);
		EatFood();
		GameDraw();

		if (GetAsyncKeyState('e') || GetAsyncKeyState('E'))
		{
			break;
		}
	}
	std::cin.get();
}


/*
本项目出现的bug：

	1.循环若使用 for(BALL food : foods) { food.x = ....} ，并不会成功画出初始的那些圆，可能与
	使用这种循环的开销过大有关，使用for(int i = 0; i < FOOD_NUM; i++) {...}成功解决

	2.EatFood()函数中的if判断条件中要加判断食物是否存在，否则会停留在消失的食物处一直增长自身大小
*/