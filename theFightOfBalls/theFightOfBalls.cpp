#include <easyx.h>
#include <graphics.h>
#include <iostream>
#include <math.h>
//----------�����������ڲ�������
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
//----------��������ͷ�ļ������������������
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 640
#define FOOD_NUM 200


/* �������ս�ж���Բ����ҿ��Ƶ���Բ���Ե�Ҳ��Բ��
	Բ��Ҫ�����ԣ�Բ�����꣬�뾶����ɫ���Ƿ���ڵı�־�����Ե��Ͳ����ˣ�
	�������Կ����ýṹ������ʾ */

class Ball
{
public:
	float x, y;				//Բ������
	double r;					//�뾶
	unsigned long color;	//��ɫ
	bool exist;				//���ڵı�־��1������� 0��������
};

Ball foods[FOOD_NUM];		//�������ԳԵ�Բ
Ball player;				//������ҵ�Բ

void GameInit()
{
	srand((unsigned int)time(NULL));		//�������������(set random)������ӦΪ���ϱ仯��������ѡȡʱ����Ϊ����

	//��ʼ��ʳ��------------------------------------------------------------------------------------
	for (int i = 0; i < FOOD_NUM; i++)
	{
		foods[i].x = rand() % SCREEN_WIDTH;		//�����ÿ��ʳ������꣬�Ҳ��ܳ�����Ļ��С
		foods[i].y = rand() % SCREEN_HEIGHT;
		foods[i].exist = true;					//һ��ʼʳ�ﶼ����
		foods[i].r = rand() % 8 + 1;			//�뾶���ܹ����Ҳ���Ϊ0
		foods[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);	//RGB����red green blue��ԭɫ��ÿ����ɫ�����ֵΪ255
	}

	//��ʼ�����------------------------------------------------------------------------------------
	player.x = rand() % SCREEN_WIDTH;
	player.y = rand() % SCREEN_HEIGHT;
	player.r = 18;
	player.exist = true;					
	player.color = RGB(255, 180, 210);

}

void GameDraw()
{
	BeginBatchDraw();	//��EndBatchDraw()һ���𵽷�ֹ�����Ĺ���

	setbkcolor(WHITE);	//����������������һ��������ñ�����ɫ
	cleardevice();		//����

	//����ʳ��------------------------------------------------------------------
	for (int i = 0; i < FOOD_NUM; i++)
	{
		if (foods[i].exist)	//�жϣ���Բ���ھͻ�����
		{
			setfillcolor(foods[i].color);						//����������ɫ
			solidcircle(foods[i].x, foods[i].y, foods[i].r);	//��һ������Բ
		}
	}	

	//�������------------------------------------------------------------------
	setfillcolor(player.color);
	solidcircle(player.x, player.y, player.r);	//��һ������Բ
	settextcolor(RED);
	settextstyle(15, 0, L"����");
	setbkmode(0);		//�������ֱ���͸��
	outtextxy(player.x - 23, player.y - 35, L"player");

	EndBatchDraw();
}

void KeyControl(float speed)		//���̿�������ƶ�
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

double distance(Ball a, Ball b)	//��Բ��֮��ľ���
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
	initgraph(SCREEN_WIDTH, SCREEN_HEIGHT);		//��ʼ�����ڣ������ԭ���ڴ������Ͻǣ�����Ϊx������������Ϊy��������
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
����Ŀ���ֵ�bug��

	1.ѭ����ʹ�� for(BALL food : foods) { food.x = ....} ��������ɹ�������ʼ����ЩԲ��������
	ʹ������ѭ���Ŀ��������йأ�ʹ��for(int i = 0; i < FOOD_NUM; i++) {...}�ɹ����

	2.EatFood()�����е�if�ж�������Ҫ���ж�ʳ���Ƿ���ڣ������ͣ������ʧ��ʳ�ﴦһֱ���������С
*/