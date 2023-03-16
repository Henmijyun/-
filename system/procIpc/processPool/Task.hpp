#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unordered_map>
#include <functional>

using namespace std;

typedef function<void()> func;  // 用于函数回调

vector<func> callbacks;    // 函数列表
unordered_map<int, string> desc;   // 记录进程信息

// 各自进程
void readMySQL()
{
    cout << "sub process[ " << getpid() << " ] 执行访问数据库的任务\n" << endl;
}

void execuleUrl()
{
    cout << "sub process[ " << getpid() << " ] 执行url解析\n" << endl;
}

void cal()
{
    cout << "sub process[ " << getpid() << " ] 执行加密任务\n" << endl;
}

void save()
{
    cout << "sub process[ " << getpid() << " ] 执行数据持久化任务\n" << endl;
}


void load()   // 创建进程列表
{
    desc.insert({callbacks.size(), "readMySQL: 读取数据库"});
    callbacks.push_back(readMySQL);

    desc.insert({callbacks.size(), "execuleUrl: 进行url解析"});
    callbacks.push_back(execuleUrl);

    desc.insert({callbacks.size(), "cal: 进行加密计算"});
    callbacks.push_back(cal);

    desc.insert({callbacks.size(), "save: 进行数据的文件保存"});
    callbacks.push_back(save);
}

void showHandler()   // 展示进程列表
{
    for (const auto &iter : desc)
    {
        cout << iter.first << "\t" << iter.second << endl;
    }
}

int handlerSize()  // 进程列表大小
{
    return callbacks.size();
}

