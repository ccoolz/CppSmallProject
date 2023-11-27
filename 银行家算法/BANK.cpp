#include <iostream>
#include <vector>
#include <stdlib.h>
using namespace std;

#define PN 4	//总进程数
#define SN 3	//资源种类

int all_resources[SN] = { 8, 5, 7 }; //总资源数目
int available[SN] = { 8, 5, 7 };	//当前可用各资源的数目，应为 all_resources - allocated
int request[SN];	//请求各资源的个数，运行时自己输入

int max_need[PN][SN] = { {3, 1, 2}, {5, 1, 3}, {3, 1, 4}, {4, 2, 2} };	//PN个进程对SN种资源的总需求量
int allocated[PN][SN] = { {1, 0, 0}, {5, 1, 2}, {2, 1, 1}, {0, 0, 2} };	//PN个进程已分配的SN种资源数量
int need[PN][SN]; //PN进程还需要SN种资源的数量，应为 max_need - allocated
   
void StartUI();									//程序开始界面
void InitResources();							//初始化资源数量
void ShowTables();								//展示资源表格
void Banker();									//执行银行家算法
void Allocation(int pid);						//试给进程pid分配需求资源
bool IsSafe(int pid);							//安全性检查函数
void Restore(int pid);							//恢复被分配的资源，以重新进行银行家算法
bool CanAllocate(int pid, int* simulation_avai);//判断当前所剩资源是否满足进程所需资源

int main()
{
	StartUI();
    InitResources();
	ShowTables();
	Banker();

	return 0;
}

/*********************************************************************************************************/

void StartUI()
{
	cout << "\n\n\n\n\n";
	cout << "				----------------------- \n";
	cout << "				| 银行家算法演示程序  | \n";
	cout << "				----------------------- \n\n\n\n\n";
	cin.get();
}

void InitResources()
{
	for (int sn = 0; sn < SN; sn++)
	{
		for (int pn = 0; pn < PN; pn++)
		{
			available[sn] = available[sn] - allocated[pn][sn];
		}
	}	//可用资源数 = 总资源数 - 所有进程中已分配资源数的和

	for (int pn = 0; pn < PN; pn++)
	{
		for (int sn = 0; sn < SN; sn++)
		{
			need[pn][sn] = max_need[pn][sn] - allocated[pn][sn];
		}
	}
}

void ShowTables()
{
	system("cls");	//清屏
	int pn, sn;

	cout << "| 系统可用的资源数量: ["; for (sn = 0; sn < SN; sn++) { cout << " " << available[sn]; } cout << " ] |\n";
	cout << "-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n";
	cout
		<< "          各进程已得资源			  各进程所需资源       \n"
		<< "---------------------------------	---------------------------------\n"
		<< "| p / s | 资源0 | 资源1 | 资源2 |	| p / s | 资源0 | 资源1 | 资源2 |\n"
		<< "---------------------------------	---------------------------------\n";
	for (pn = 0; pn < PN; pn++)
	{
		cout << "|进程 p" << pn << "|   "; for (sn = 0; sn < SN; sn++) { cout << allocated[pn][sn] << "   |   "; } cout << "	"; cout << "|进程 p" << pn << "|   "; for (sn = 0; sn < SN; sn++) { cout << need[pn][sn] << "   |   "; }
		cout << "\n---------------------------------	---------------------------------\n";
	}

	cout << "-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n";
}

void Banker()
{
	int pid, sn;
	int keep = 1;
	while (keep)
	{
		cout << "需要申请资源的进程号[p0~p" << PN - 1 << "]: p";
		cin >> pid;
		while (pid < 0 || pid > PN - 1)
		{
			cout << "进程号不存在，重新输入: p";
			cin >> pid;
		}

		cout << "按序输入进程p" << pid << "所需各资源的数量 \n";
		for (sn = 0; sn < SN; sn++)
		{
			cout << "资源" << sn << ": ";
			cin >> request[sn];

			while (request[sn] > need[pid][sn])
			{
				cout << "申请资源数大于进程p" << pid << "还需要的" << sn << "类资源数，请重试 \n";
				cout << "资源" << sn << ": ";
				cin >> request[sn];
			}
			while (request[sn] > available[sn])
			{
				cout << "申请资源数大于可用的" << sn << "类资源数，请重试 \n";
				cout << "资源" << sn << ": ";
				cin >> request[sn];
			}
		}   //检查用户输入的需求资源的合理性

		if (keep)
		{
			Allocation(pid); //试分配需求资源

			if (IsSafe(pid)) //对进程pid分配资源的操作进行安全性检查
			{
				cout << "\n【通过了安全性检测，可以为进程" << pid << "分配资源!】 \n";
				cin.get(); cin.get(); system("cls");
				cout << "[继续银行家算法输入1，否则输入0]: ";
				cin >> keep;
				if (keep)
				{
					Restore(pid);
					ShowTables();
				}
			}
			else
			{
				cout << "\n【没能找到安全序列，不能为进程" << pid << "分配资源!】 \n";
				cin.get(); cin.get(); system("cls");
				cout << "[继续银行家算法输入1，否则输入0]: ";
				cin >> keep;
				if (keep)
				{
					Restore(pid);
					ShowTables();
				}
			}
		}
	}

	system("cls");
	cout << "\n\n\n\n\n";
	cout << "				----------------------- \n";
	cout << "				| 银行家算法演示结束! | \n";
	cout << "				----------------------- \n\n\n\n\n";
}

void Allocation(int pid)
{	//分配进程pid需求的资源
	for (int sn = 0; sn < SN; sn++)
	{
		available[sn] -= request[sn];		//可用资源 - 需要分配的资源
		allocated[pid][sn] += request[sn];	//pid已分配资源 + 分配的资源
		need[pid][sn] -= request[sn];		//pid还需要资源 - 分配的资源
	}
}

void Restore(int pid)
{	//恢复试分配给pid进程的资源
	for (int sn = 0; sn < SN; sn++)
	{
		available[sn] += request[sn];		//可用资源 + 已分配的资源
		allocated[pid][sn] -= request[sn];	//pid已分配资源 - 已分配的资源
		need[pid][sn] += request[sn];		//pid还需要资源 + 已分配的资源
	}
}

bool IsSafe(int pid)
{
	int sn;
	vector<int> p_hasnot_allocated;	//还未完全分配资源的数组
	for (int pn = 0; pn < PN; pn++) { p_hasnot_allocated.emplace_back(pn); }
	vector<int> p_has_allocated;	//已完全分配过资源的数组，size达到PN个说明已找到安全序列
	int pn_can_allocate;	//每轮检查中可用资源至少应该满足一个进程的剩余所需资源数，若不能，说明找不到安全序列
	int* simulation_avai = new int[5];	//模拟 分配过程中系统当前的可用资源数
	

	cout << "资源试分配给进程p" << pid << "后，系统可用资源为: ";
	for (sn = 0; sn < SN; sn++)
	{
		simulation_avai[sn] = available[sn];
		cout << simulation_avai[sn] << " ";
	}	cout << endl;

	while (p_has_allocated.size() < PN)	//扫描式一轮轮检查可以分配资源的进程
	{
		pn_can_allocate = 0;

		for (int pn = 0; pn < PN; pn++)	//外圈循环
		{	//PN次循环要么每次找到一个可执行进程(每次都有 pn_can_allocate = 1)，要么在某次循环后一直找不到(那次循环开始一直有 pn_can_allocate = 0)
			for (auto iter = p_hasnot_allocated.begin(); iter != p_hasnot_allocated.end();)
			{
				int p = (*iter);
				if (CanAllocate(p, simulation_avai))
				{	//可以分配
					//直接更新各数组分配资源并归还后的结果
					cout << "执行进程p" << p << "... \n" << "系统当前可用资源: ";
					for (int sn = 0; sn < SN; sn++)
					{
						simulation_avai[sn] += allocated[p][sn];
						cout << simulation_avai[sn] << " ";
					}	cout << endl;

					pn_can_allocate++;
					p_has_allocated.emplace_back(p);
					p_hasnot_allocated.erase(iter);
					break; //使用erase清除迭代器，只能采取break退出循环，否则会出错导致迭代器遍历不能继续，使用break就需要外圈循环使迭代器遍历继续
				}
				else
					iter++;
			}
		}

		if (pn_can_allocate == 0)
		{
			delete[] simulation_avai;
			return false;
		}
	}

	//通过了安全性检查
	cout << "找到了如下的安全序列\n[ ";
	for (auto iter = p_has_allocated.begin(); iter != p_has_allocated.end(); iter++)
	{
		cout << "p" << (*iter);
		if (iter != p_has_allocated.end() - 1)
			cout << "->";
	}	cout << " ] \n";
	delete[] simulation_avai;
	return true;
}

bool CanAllocate(int pid, int* simulation_avai)
{
	for (int sn = 0; sn < SN; sn++)
	{
		if (simulation_avai[sn] < need[pid][sn])
			return false;
	}
	return true;
}