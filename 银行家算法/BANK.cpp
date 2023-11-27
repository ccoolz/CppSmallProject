#include <iostream>
#include <vector>
#include <stdlib.h>
using namespace std;

#define PN 4	//�ܽ�����
#define SN 3	//��Դ����

int all_resources[SN] = { 8, 5, 7 }; //����Դ��Ŀ
int available[SN] = { 8, 5, 7 };	//��ǰ���ø���Դ����Ŀ��ӦΪ all_resources - allocated
int request[SN];	//�������Դ�ĸ���������ʱ�Լ�����

int max_need[PN][SN] = { {3, 1, 2}, {5, 1, 3}, {3, 1, 4}, {4, 2, 2} };	//PN�����̶�SN����Դ����������
int allocated[PN][SN] = { {1, 0, 0}, {5, 1, 2}, {2, 1, 1}, {0, 0, 2} };	//PN�������ѷ����SN����Դ����
int need[PN][SN]; //PN���̻���ҪSN����Դ��������ӦΪ max_need - allocated
   
void StartUI();									//����ʼ����
void InitResources();							//��ʼ����Դ����
void ShowTables();								//չʾ��Դ���
void Banker();									//ִ�����м��㷨
void Allocation(int pid);						//�Ը�����pid����������Դ
bool IsSafe(int pid);							//��ȫ�Լ�麯��
void Restore(int pid);							//�ָ����������Դ�������½������м��㷨
bool CanAllocate(int pid, int* simulation_avai);//�жϵ�ǰ��ʣ��Դ�Ƿ��������������Դ

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
	cout << "				| ���м��㷨��ʾ����  | \n";
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
	}	//������Դ�� = ����Դ�� - ���н������ѷ�����Դ���ĺ�

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
	system("cls");	//����
	int pn, sn;

	cout << "| ϵͳ���õ���Դ����: ["; for (sn = 0; sn < SN; sn++) { cout << " " << available[sn]; } cout << " ] |\n";
	cout << "-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n";
	cout
		<< "          �������ѵ���Դ			  ������������Դ       \n"
		<< "---------------------------------	---------------------------------\n"
		<< "| p / s | ��Դ0 | ��Դ1 | ��Դ2 |	| p / s | ��Դ0 | ��Դ1 | ��Դ2 |\n"
		<< "---------------------------------	---------------------------------\n";
	for (pn = 0; pn < PN; pn++)
	{
		cout << "|���� p" << pn << "|   "; for (sn = 0; sn < SN; sn++) { cout << allocated[pn][sn] << "   |   "; } cout << "	"; cout << "|���� p" << pn << "|   "; for (sn = 0; sn < SN; sn++) { cout << need[pn][sn] << "   |   "; }
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
		cout << "��Ҫ������Դ�Ľ��̺�[p0~p" << PN - 1 << "]: p";
		cin >> pid;
		while (pid < 0 || pid > PN - 1)
		{
			cout << "���̺Ų����ڣ���������: p";
			cin >> pid;
		}

		cout << "�����������p" << pid << "�������Դ������ \n";
		for (sn = 0; sn < SN; sn++)
		{
			cout << "��Դ" << sn << ": ";
			cin >> request[sn];

			while (request[sn] > need[pid][sn])
			{
				cout << "������Դ�����ڽ���p" << pid << "����Ҫ��" << sn << "����Դ���������� \n";
				cout << "��Դ" << sn << ": ";
				cin >> request[sn];
			}
			while (request[sn] > available[sn])
			{
				cout << "������Դ�����ڿ��õ�" << sn << "����Դ���������� \n";
				cout << "��Դ" << sn << ": ";
				cin >> request[sn];
			}
		}   //����û������������Դ�ĺ�����

		if (keep)
		{
			Allocation(pid); //�Է���������Դ

			if (IsSafe(pid)) //�Խ���pid������Դ�Ĳ������а�ȫ�Լ��
			{
				cout << "\n��ͨ���˰�ȫ�Լ�⣬����Ϊ����" << pid << "������Դ!�� \n";
				cin.get(); cin.get(); system("cls");
				cout << "[�������м��㷨����1����������0]: ";
				cin >> keep;
				if (keep)
				{
					Restore(pid);
					ShowTables();
				}
			}
			else
			{
				cout << "\n��û���ҵ���ȫ���У�����Ϊ����" << pid << "������Դ!�� \n";
				cin.get(); cin.get(); system("cls");
				cout << "[�������м��㷨����1����������0]: ";
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
	cout << "				| ���м��㷨��ʾ����! | \n";
	cout << "				----------------------- \n\n\n\n\n";
}

void Allocation(int pid)
{	//�������pid�������Դ
	for (int sn = 0; sn < SN; sn++)
	{
		available[sn] -= request[sn];		//������Դ - ��Ҫ�������Դ
		allocated[pid][sn] += request[sn];	//pid�ѷ�����Դ + �������Դ
		need[pid][sn] -= request[sn];		//pid����Ҫ��Դ - �������Դ
	}
}

void Restore(int pid)
{	//�ָ��Է����pid���̵���Դ
	for (int sn = 0; sn < SN; sn++)
	{
		available[sn] += request[sn];		//������Դ + �ѷ������Դ
		allocated[pid][sn] -= request[sn];	//pid�ѷ�����Դ - �ѷ������Դ
		need[pid][sn] += request[sn];		//pid����Ҫ��Դ + �ѷ������Դ
	}
}

bool IsSafe(int pid)
{
	int sn;
	vector<int> p_hasnot_allocated;	//��δ��ȫ������Դ������
	for (int pn = 0; pn < PN; pn++) { p_hasnot_allocated.emplace_back(pn); }
	vector<int> p_has_allocated;	//����ȫ�������Դ�����飬size�ﵽPN��˵�����ҵ���ȫ����
	int pn_can_allocate;	//ÿ�ּ���п�����Դ����Ӧ������һ�����̵�ʣ��������Դ���������ܣ�˵���Ҳ�����ȫ����
	int* simulation_avai = new int[5];	//ģ�� ���������ϵͳ��ǰ�Ŀ�����Դ��
	

	cout << "��Դ�Է��������p" << pid << "��ϵͳ������ԴΪ: ";
	for (sn = 0; sn < SN; sn++)
	{
		simulation_avai[sn] = available[sn];
		cout << simulation_avai[sn] << " ";
	}	cout << endl;

	while (p_has_allocated.size() < PN)	//ɨ��ʽһ���ּ����Է�����Դ�Ľ���
	{
		pn_can_allocate = 0;

		for (int pn = 0; pn < PN; pn++)	//��Ȧѭ��
		{	//PN��ѭ��Ҫôÿ���ҵ�һ����ִ�н���(ÿ�ζ��� pn_can_allocate = 1)��Ҫô��ĳ��ѭ����һֱ�Ҳ���(�Ǵ�ѭ����ʼһֱ�� pn_can_allocate = 0)
			for (auto iter = p_hasnot_allocated.begin(); iter != p_hasnot_allocated.end();)
			{
				int p = (*iter);
				if (CanAllocate(p, simulation_avai))
				{	//���Է���
					//ֱ�Ӹ��¸����������Դ���黹��Ľ��
					cout << "ִ�н���p" << p << "... \n" << "ϵͳ��ǰ������Դ: ";
					for (int sn = 0; sn < SN; sn++)
					{
						simulation_avai[sn] += allocated[p][sn];
						cout << simulation_avai[sn] << " ";
					}	cout << endl;

					pn_can_allocate++;
					p_has_allocated.emplace_back(p);
					p_hasnot_allocated.erase(iter);
					break; //ʹ��erase�����������ֻ�ܲ�ȡbreak�˳�ѭ�������������µ������������ܼ�����ʹ��break����Ҫ��Ȧѭ��ʹ��������������
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

	//ͨ���˰�ȫ�Լ��
	cout << "�ҵ������µİ�ȫ����\n[ ";
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