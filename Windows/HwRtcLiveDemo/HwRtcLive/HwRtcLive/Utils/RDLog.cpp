#include "stdafx.h"
#include <ctype.h>
#include <stdarg.h>
#include <string>
#include <stdarg.h>
#include <windows.h>
#include <mutex>
#include "RDLogSingleton.h"
#include "RDLogManager.h"
#include "RDLog.h"

static int SafeSprintfV(std::string &res, const char *fmt, va_list arg) {
    int len = _vscprintf(fmt, arg);
    if (len <= 0) {
        return -1;
    }
    char *temp = (char *)malloc(len + 1);
    if (!temp) {
        return -1;
    }
    len = vsprintf_s(temp, len + 1, fmt, arg);
    if (len > 0) {
        res.assign(temp);
    }
    free(temp);
    return len;
}

static int SafeSprintf(std::string &res, const char *fmt, ...) {
    va_list arg;
    va_start(arg, fmt);
    int len = SafeSprintfV(res, fmt, arg);
    va_end(arg);
    return len;
}

static const char *GetBaseName(const char *file) {
    if (!file) {
        return "NULL";
    }

    const char * base_name = file;
    for (const char *p = file; *p; p++) {
        if (('/' == *p) || ('\\' == *p)) {
            base_name = p + 1;
        }
    }
    return base_name;
}

static void GetContent(std::string &res, const char *fmt, va_list arg) {
    INT32 len = SafeSprintfV(res, fmt, arg);
    if (!res.empty()) {
        // 去除结尾可能存在的回车、换行及其他不必要的空格
        for (INT32 i = len - 1; i >= 0 && isspace(res[i]); --i) {
            res[i] = '\0';
        }
    }
    return;
}

static void FormateLog(std::string &res,
    const char *tag,
    const char *file,
    int line,
    const char *content) {
    SYSTEMTIME local_time;
    GetLocalTime(&local_time);

    const char *fileName = GetBaseName(file);

    SafeSprintf(res,
        "[%04d-%02d-%02d %02d:%02d:%02d.%03d|%s|%s:%d|%d] %s\n",
        local_time.wYear,
        local_time.wMonth,
        local_time.wDay,
        local_time.wHour,
        local_time.wMinute,
        local_time.wSecond,
        local_time.wMilliseconds,
        tag ? tag : "NULL",
        fileName,
        line,
        GetCurrentThreadId(),
        content);
}

// 日志接口
void RDLog(const char *tag, const char *file, int line, const char *fmt, ...) {
    // 生成日志内容
    va_list arg;
    va_start(arg, fmt);
    std::string logContent;
    GetContent(logContent, fmt, arg);

    std::string log;
    FormateLog(log, tag, file, line, logContent.c_str());

    SINGLETON(RDLogManager)->WriteLog(log);
    va_end(arg);
}

// 打开本地文件日志通道
void RDOpenLog() {
    SINGLETON(RDLogManager)->OpenLog();
}

// 关闭本地文件日志通道
void RDCloseLog() {
    SINGLETON(RDLogManager)->CloseLog();
}
