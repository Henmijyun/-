#pragma once

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdarg>
#include <ctime>

// 日志级别
#define DEBUG   0
#define NORMAL  1    // 正常
#define WARNING 2    // 警告
#define ERROR   3    // 错误
#define FATAL   4    // 致命

const char* gLevelMap[] = {
    "DEBUG",
    "NORMAL",
    "WARNING",
    "ERROR",
    "FATAL"
};

#define LOGFILE "./threadpool.log"

// 完整的日志功能，至少：日志等级，时间，日志内容，支持用户自定义(日志内容，文件行，文件名..)
void logMessage(int level, const char* format, ...)
{
#ifndef DEBUG_SHOW
    if(level == DEBUG) return;
#endif
    // va_list ap;    // va_list 底层是 char* 
    // va_start(ap, format);  // 最后一个具体参数
    // while ()         
    // int x = va_arg(ap, int);  // 循环取参数
    // va_end(ap);            // 结束


    char stdBuffer[1024];  // 标准部分
    time_t timestamp = time(nullptr);
    // struct tm* localtime = localtime(&timestamp); 

    snprintf(stdBuffer, sizeof stdBuffer, "[%s] [%ld]", gLevelMap[level], timestamp);

    char logBuffer[1024];  // 自定义部分
    va_list args;
    va_start(args, format);
    //vprintf(format, args);
    vsnprintf(logBuffer, sizeof logBuffer, format, args);
    va_end(args);


    // FILE* fp = fopen(LOGFILE, "a");     // 打开文件流
    printf("%s%s\n", stdBuffer, logBuffer);    // 日志打印到屏幕
    // fprintf(fp, "%s%s\n", stdBuffer, logBuffer);  // 日志写入文件
    // fclose(fp);
}
