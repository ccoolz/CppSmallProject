#include <iostream>
#include <vector>

using namespace std;

// 定义系统的资源和进程数量
const int numResources = 3;
const int numProcesses = 5;

// 定义进程的最大需求资源数量
int maxResources[numProcesses][numResources] = {
    {7, 5, 3},
    {3, 2, 2},
    {9, 0, 2},
    {2, 2, 2},
    {4, 3, 3}
};

// 定义系统的可用资源数量
int availableResources[numResources] = { 3, 3, 2 };

// 定义进程已经分配的资源数量
int allocatedResources[numProcesses][numResources] = {
    {0, 1, 0},
    {2, 0, 0},
    {3, 0, 2},
    {2, 1, 1},
    {0, 0, 2}
};

// 定义进程还需要的资源数量
int needResources[numProcesses][numResources];

// 定义进程的完成状态
bool finished[numProcesses] = { false };

// 检查进程是否可以分配资源
bool checkSafe(int process) {
    for (int i = 0; i < numResources; i++) {
        if (needResources[process][i] > availableResources[i]) {
            return false;
        }
    }
    return true;
}

// 分配资源给进程，并更新系统的可用资源数量
void allocateResources(int process) {
    for (int i = 0; i < numResources; i++) {
        availableResources[i] -= needResources[process][i];
        allocatedResources[process][i] += needResources[process][i];
    }
}

// 释放进程已经分配的资源，并更新系统的可用资源数量
void releaseResources(int process) {
    for (int i = 0; i < numResources; i++) {
        availableResources[i] += allocatedResources[process][i];
        allocatedResources[process][i] = 0;
    }
}

int main() {
    // 计算进程还需要的资源数量
    for (int i = 0; i < numProcesses; i++) {
        for (int j = 0; j < numResources; j++) {
            needResources[i][j] = maxResources[i][j] - allocatedResources[i][j];
        }
    }

    // 执行银行家算法
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

    // 输出结果
    if (isSafe) {
        cout << "The system is in a safe state." << endl;
    }
    else {
        cout << "The system is in an unsafe state." << endl;
    }

    return 0;
}