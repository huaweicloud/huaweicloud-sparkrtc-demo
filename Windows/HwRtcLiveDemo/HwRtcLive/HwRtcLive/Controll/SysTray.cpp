#include "StdAfx.h"
#include "SysTray.h"
/*lint -save -e1401*/
#define SIZEOF_ARRAY(a) (sizeof(a)/sizeof(a[0]))

SystemTray::SystemTray()
    : nid_() {

}

SystemTray::~SystemTray(void) {
    try {
        BOOL Ret = NotifyDelete();
        if (Ret) {
        }
    } catch (...) {

    }
    ::Shell_NotifyIcon(NIM_DELETE, &nid_);
}

// 设置托盘图标
BOOL SystemTray::ShellNotifyIcon(DWORD dwMessage) {
    BOOL bSuccessful = ::Shell_NotifyIcon(dwMessage, &nid_);
    return bSuccessful;
}

// 新增图标
BOOL SystemTray::NotifyAdd() {
    return ShellNotifyIcon(NIM_ADD);
}

// 删除图标
BOOL SystemTray::NotifyDelete() {
    return ShellNotifyIcon(NIM_DELETE);
}

// 更新图标
BOOL SystemTray::NotifyUpdate() {
    BOOL bSuccessful = ShellNotifyIcon(NIM_MODIFY);
    if (!bSuccessful) {
        TRACE("Shell_NotifyIcon fail: %d", ::GetLastError());
        //RTC_LOG()<<"Shell_NotifyIcon fail: "<<::GetLastError();
        if (nid_.hWnd) {
            TRACE("hWnd is NULL");
            //	ERROR_LOG()<<"hWnd is NULL";
            if (ShellNotifyIcon(NIM_ADD)) {
                CString tip = nid_.szTip;
                (void)SetTip(tip);
                (void)SetCallbackMessage(nid_.uCallbackMessage);
            } else {
                bSuccessful = ShellNotifyIcon(NIM_MODIFY);
            }
        }
    }

    return bSuccessful;
}

// 设置（更新）图标
BOOL SystemTray::SetIcon(HICON hIcon) {
    nid_.uFlags = NIF_ICON;
    nid_.hIcon = hIcon;
    return NotifyUpdate();
}

BOOL SystemTray::SetCallbackMessage(UINT uCallbackMessage) {
    nid_.uFlags = NIF_MESSAGE;
    nid_.uCallbackMessage = uCallbackMessage;
    return NotifyUpdate();
}

BOOL SystemTray::SetTip(const CString& tip) {
    nid_.uFlags = NIF_TIP;
    _tcsncpy_s(nid_.szTip, tip, SIZEOF_ARRAY(nid_.szTip) - 1);
    return NotifyUpdate();
}

BOOL SystemTray::SetInfo(const CString& info) {
    const UINT TIME_OUT = 10000;
    const CString TITLE(TEXT("提示"));
    nid_.uFlags = NIF_INFO;
    nid_.uTimeout = TIME_OUT;
    _tcsncpy_s(nid_.szInfoTitle, TITLE, SIZEOF_ARRAY(nid_.szInfoTitle) - 1);
    _tcsncpy_s(nid_.szInfo, info, SIZEOF_ARRAY(nid_.szInfo) - 1);
    return NotifyUpdate();
}

BOOL SystemTray::Disable() {
    return SetCallbackMessage(0);
}

void SystemTray::Init(HWND hWnd, UINT uID) {
    nid_.cbSize = sizeof(nid_);
    nid_.hWnd = hWnd;
    nid_.uID = uID;
    // must set to 0, because we can not determine other members of nid_ here. 
    nid_.uFlags = 0;
    // 添加系统托盘图标
    BOOL Ret = NotifyAdd();
    if (Ret) {
    }
}
/*lint -restore*/