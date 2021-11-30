#include "stdafx.h"
#include <dbghelp.h>
#include <Shlwapi.h>
#include "HwCatchDump.h"
#include <time.h>
#include "HwRtcEngineMgr.h"
#pragma comment(lib,"shlwapi.lib")
#pragma comment(lib, "dbghelp.lib")

namespace HwDumpCatch {
    void Hwlog(const std::wstring& log_msg) {
        std::wstring strLogWnd = L"hw_debug_log";
        HWND hSend = ::FindWindow(NULL, strLogWnd.c_str());
        COPYDATASTRUCT copydate;
        copydate.cbData = (DWORD)(log_msg.length() + 1) * sizeof(wchar_t);
        copydate.lpData = (PVOID)log_msg.c_str();
        ::SendMessage(hSend, WM_COPYDATA, 0, (LPARAM)&copydate);
    }

    void CHwCrashDumpCatch::HwPureCallHandler(void) {
        throw std::invalid_argument("");
    }

    void CHwCrashDumpCatch::HwInvalidParameterHandler(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, uintptr_t pReserved) {
        throw std::invalid_argument("");
    }

    void CHwCrashDumpCatch::HwSetInvalidHandle() {
#if _MSC_VER >= 1400  // MSVC 2005/8
        m_preIph = _set_invalid_parameter_handler(HwInvalidParameterHandler);
#endif  // _MSC_VER >= 1400
        m_prePch = _set_purecall_handler(HwPureCallHandler);   //At application, this call can stop show the error message box.
    }
    void CHwCrashDumpCatch::HwUnSetInvalidHandle() {
#if _MSC_VER >= 1400  // MSVC 2005/8
        _set_invalid_parameter_handler(m_preIph);
#endif  // _MSC_VER >= 1400
        _set_purecall_handler(m_prePch); //At application this can stop show the error message box.
    }

    LPTOP_LEVEL_EXCEPTION_FILTER WINAPI CHwCrashDumpCatch::HwSetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter) {
        return NULL;
    }

    BOOL CHwCrashDumpCatch::HwAddExceptionHandle() {
        m_preFilter = ::SetUnhandledExceptionFilter(UnhandledExceptionFilterEx);
        HwPreSetUnhandledExceptionFilter();
        return TRUE;
    }

    BOOL CHwCrashDumpCatch::HwRmExceptionHandle() {
        if (m_preFilter != NULL) {
            ::SetUnhandledExceptionFilter(m_preFilter);
            m_preFilter = NULL;
        }
        return TRUE;
    }
    CHwCrashDumpCatch::CHwCrashDumpCatch() {
        HwSetInvalidHandle();
        HwAddExceptionHandle();
    }

    CHwCrashDumpCatch::~CHwCrashDumpCatch() {
        HwUnSetInvalidHandle();
        HwRmExceptionHandle();
    }

    BOOL CHwCrashDumpCatch::HwReleaseDumpFile(const std::wstring& strPath, EXCEPTION_POINTERS *pException) {
        HANDLE hDumpFile = ::CreateFile(strPath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hDumpFile == INVALID_HANDLE_VALUE) {
            return FALSE;
        }
        MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
        dumpInfo.ExceptionPointers = pException;
        dumpInfo.ThreadId = ::GetCurrentThreadId();
        dumpInfo.ClientPointers = TRUE;
        BOOL bRet = ::MiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
        ::CloseHandle(hDumpFile);
        return bRet;
    }

    LONG WINAPI CHwCrashDumpCatch::UnhandledExceptionFilterEx(struct _EXCEPTION_POINTERS *pException) {
        wchar_t szPath[MAX_PATH] = { 0 };
        ::GetModuleFileName(NULL, szPath, MAX_PATH);
        ::PathRemoveFileSpec(szPath);
        std::wstring strPath = szPath;
        time_t myt = time(NULL);
        time_t t;
        time(&t);
        long long timestamp = t;
        std::wstring strTime = std::to_wstring(timestamp);

        strPath += L"\\HwCrashDump" + strTime + L".dmp";
        BOOL bRelease = HwReleaseDumpFile(strPath.c_str(), pException);
        if (bRelease) {
            //拷贝dump文件到日志目录下，以备日志上传收集使用
            std::wstring strTargetPath = _T(RTC_LOG_PATH);
            strTargetPath = strTargetPath + L"HwCrashDump" + strTime + L".dmp";
            CopyFile(strPath.c_str(), strTargetPath.c_str(), FALSE);
            return EXCEPTION_EXECUTE_HANDLER;
        }

        return EXCEPTION_CONTINUE_SEARCH;
    }

    BOOL CHwCrashDumpCatch::HwPreSetUnhandledExceptionFilter() {
        HMODULE hKernel32 = LoadLibrary(L"kernel32.dll");
        if (hKernel32 == NULL) {
            return FALSE;
        }
        void *pOrgEntry = ::GetProcAddress(hKernel32, "SetUnhandledExceptionFilter");
        if (pOrgEntry == NULL) {
            FreeLibrary(hKernel32);
            return FALSE;
        }
        unsigned char newJump[5];
        DWORD dwOrgEntryAddr = (DWORD)pOrgEntry;
        dwOrgEntryAddr += 5;
        void *pNewFunc = &HwSetUnhandledExceptionFilter;
        DWORD dwNewEntryAddr = (DWORD)pNewFunc;
        DWORD dwRelativeAddr = dwNewEntryAddr - dwOrgEntryAddr;
        newJump[0] = 0xE9;
        errno_t err = memcpy_s(&newJump[1], sizeof(DWORD), &dwRelativeAddr, sizeof(DWORD));
        if (err != 0) {
            FreeLibrary(hKernel32);
            return FALSE;
        }
        SIZE_T bytesWritten;
        BOOL bRet = ::WriteProcessMemory(::GetCurrentProcess(), pOrgEntry, newJump, 5, &bytesWritten);
        FreeLibrary(hKernel32);
        return bRet;
    }

}

