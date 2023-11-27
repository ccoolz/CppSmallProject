#include <iostream>
#include <vector>

using namespace std;

// ����ϵͳ����Դ�ͽ�������
const int numResources = 3;
const int numProcesses = 5;

// ������̵����������Դ����
int maxResources[numProcesses][numResources] = {
    {7, 5, 3},
    {3, 2, 2},
    {9, 0, 2},
    {2, 2, 2},
    {4, 3, 3}
};

// ����ϵͳ�Ŀ�����Դ����
int availableResources[numResources] = { 3, 3, 2 };

// ��������Ѿ��������Դ����
int allocatedResources[numProcesses][numResources] = {
    {0, 1, 0},
    {2, 0, 0},
    {3, 0, 2},
    {2, 1, 1},
    {0, 0, 2}
};

// ������̻���Ҫ����Դ����
int needResources[numProcesses][numResources];

// ������̵����״̬
bool finished[numProcesses] = { false };

// �������Ƿ���Է�����Դ
bool checkSafe(int process) {
    for (int i = 0; i < numResources; i++) {
        if (needResources[process][i] > availableResources[i]) {
            return false;
        }
    }
    return true;
}

// ������Դ�����̣�������ϵͳ�Ŀ�����Դ����
void allocateResources(int process) {
    for (int i = 0; i < numResources; i++) {
        availableResources[i] -= needResources[process][i];
        allocatedResources[process][i] += needResources[process][i];
    }
}

// �ͷŽ����Ѿ��������Դ��������ϵͳ�Ŀ�����Դ����
void releaseResources(int process) {
    for (int i = 0; i < numResources; i++) {
        availableResources[i] += allocatedResources[process][i];
        allocatedResources[process][i] = 0;
    }
}

int main() {
    // ������̻���Ҫ����Դ����
    for (int i = 0; i < numProcesses; i++) {
        for (int j = 0; j < numResources; j++) {
            needResources[i][j] = maxResources[i][j] - allocatedResources[i][j];
        }
    }

    // ִ�����м��㷨
    bool isSafe = true;
    int count = 0;

    while (count < numProcesses) {
        bool found = false;
        for (int i = 0; i < numProcesses; i++) {
            if (!finished[i] && checkSafe(i)) {
                found = true;
                cout << "Process " << i << " is allocated resources." << endl;
                allocateResources(i);
                finished[i] = true;
                count++;
            }
        }

        if (!found) {
            isSafe = false;
            break;
        }
    }

    // ������
    if (isSafe) {
        cout << "The system is in a safe state." << endl;
    }
    else {
        cout << "The system is in an unsafe state." << endl;
    }

    return 0;
}