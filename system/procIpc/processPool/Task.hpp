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

typedef function<void()> func;  // ���ں����ص�

vector<func> callbacks;    // �����б�
unordered_map<int, string> desc;   // ��¼������Ϣ

// ���Խ���
void readMySQL()
{
    cout << "sub process[ " << getpid() << " ] ִ�з������ݿ������\n" << endl;
}

void execuleUrl()
{
    cout << "sub process[ " << getpid() << " ] ִ��url����\n" << endl;
}

void cal()
{
    cout << "sub process[ " << getpid() << " ] ִ�м�������\n" << endl;
}

void save()
{
    cout << "sub process[ " << getpid() << " ] ִ�����ݳ־û�����\n" << endl;
}


void load()   // ���������б�
{
    desc.insert({callbacks.size(), "readMySQL: ��ȡ���ݿ�"});
    callbacks.push_back(readMySQL);

    desc.insert({callbacks.size(), "execuleUrl: ����url����"});
    callbacks.push_back(execuleUrl);

    desc.insert({callbacks.size(), "cal: ���м��ܼ���"});
    callbacks.push_back(cal);

    desc.insert({callbacks.size(), "save: �������ݵ��ļ�����"});
    callbacks.push_back(save);
}

void showHandler()   // չʾ�����б�
{
    for (const auto &iter : desc)
    {
        cout << iter.first << "\t" << iter.second << endl;
    }
}

int handlerSize()  // �����б��С
{
    return callbacks.size();
}

