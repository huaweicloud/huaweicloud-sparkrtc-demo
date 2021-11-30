#include "stdafx.h"
#include <cstring>
#include <string>
#include <mutex>
#include <Shlwapi.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include "RDLogSingleton.h"
#include "RDLogManager.h"
#include "HwRtcEngineMgr.h"


#pragma comment(lib, "shlwapi.lib")

IMPLEMENT_SINGLETON(RDLogManager)

#define INVALID_FD          (-1)
#define RTC_LOG_FILE        "rtc_demo.log"

// 构造函数
RDLogManager::RDLogManager()
    :m_fd(INVALID_FD) {
}


// 写日志总入口
void RDLogManager::WriteLog(std::string &log) {
    std::unique_lock<std::mutex> lock{ this->logMutex };
    if (INVALID_FD == m_fd) {
        return;
    }
    _write(m_fd, log.c_str(), log.size());
}

int RDLogManager::OpenLog() {
    std::unique_lock<std::mutex> lock{ this->logMutex };
    if (m_fd != INVALID_FD) {
        return 0;
    }
    const char* path = RTC_LOG_PATH;
    if (!PathIsDirectoryA(path)) {
        CreateDirectoryA(path, NULL);
        if (!PathIsDirectoryA(path)) {
            return 0;
        }
    }
    std::string filePath = RTC_LOG_PATH;
    filePath.append(RTC_LOG_FILE);
    char canonical_filepath[MAX_PATH] = { 0 };
    if (!PathCanonicalizeA(canonical_filepath, filePath.c_str())) {
        return 0;
    }
    m_fd = _open(canonical_filepath, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    if (INVALID_FD == m_fd) {
        return 0;
    }
    return 1;
}

void RDLogManager::CloseLog() {
    std::unique_lock<std::mutex> lock{ this->logMutex };
    if (INVALID_FD != m_fd) {
        _close(m_fd);
        m_fd = INVALID_FD;
    }
}


