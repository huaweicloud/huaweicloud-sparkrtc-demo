
// HwRtcLive.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "HwRtcLive.h"
#include "HwRtcLiveDlg.h"
#include "RDLog.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TAG		"RTCLIVE"

// CHwRtcLiveApp

BEGIN_MESSAGE_MAP(CHwRtcLiveApp, CWinApp)
    ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

//std::wstring sss;
// CHwRtcLiveApp construction

CHwRtcLiveApp::CHwRtcLiveApp() {
    m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
    m_bLogout = false;
}

CHwRtcLiveApp theApp;

// CHwRtcLiveApp initialization
BOOL CHwRtcLiveApp::InitInstance() {
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);
    CWinApp::InitInstance();
    AfxEnableControlContainer();
    CShellManager *pShellManager = new CShellManager;
    CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
    SetRegistryKey(_T("Local AppWizard-Generated Applications"));
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    RDOpenLog();
    RD_LOG(TAG, "CHwRtcLiveDlg create");
    CHwRtcLiveDlg dlg;
    m_pMainWnd = &dlg;
    INT_PTR nResponse = dlg.DoModal();
    if (nResponse == IDOK) {
    } else if (nResponse == IDCANCEL) {
    } else if (nResponse == -1) {
        TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
        TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
    }
    CCSafeDelete(pShellManager);

#ifndef _AFXDLL
    ControlBarCleanUp();
#endif
    return FALSE;
}

int CHwRtcLiveApp::ExitInstance() {
    GdiplusShutdown(gdiplusToken);
    if (m_bLogout) {
        TCHAR szPathName[MAX_PATH] = { 0 };
        (void)::GetModuleFileName(NULL, szPathName, MAX_PATH);
        StartProcess(szPathName, _T("Logout"), SW_SHOWNORMAL);
    }
    RD_LOG(TAG, "CHwRtcLiveDlg close");
    RDCloseLog();
    return CWinApp::ExitInstance();
}

bool CHwRtcLiveApp::StartProcess(LPCTSTR strPath, LPCTSTR strCommand, int nShowType) {
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = (WORD)nShowType;
    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));
    CString strCmdLine;
    strCmdLine.Format(_T("%s %s"), strPath, strCommand);
    if (!CreateProcess(NULL, const_cast<LPWSTR>(strCmdLine.GetString()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        return false;
    }
    return true;
}