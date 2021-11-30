// ToastDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HwRtcLive.h"
#include "ToastDlg.h"
#include "afxdialogex.h"

#define TIMER_EVENT_ID_BASE 0x00ff00

// CToastDlg dialog

IMPLEMENT_DYNAMIC(CToastDlg, CDialogEx)

CToastDlg::CToastDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_TOAST_DIALOG, pParent), m_timerCount(0) {
}

CToastDlg::~CToastDlg() {
}

void CToastDlg::DoDataExchange(CDataExchange* pDX) {
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TOAST_TEXT, m_ctrText);
}


BEGIN_MESSAGE_MAP(CToastDlg, CDialogEx)
    ON_WM_TIMER()
    ON_WM_CREATE()
END_MESSAGE_MAP()


// CToastDlg message handlers
void CToastDlg::OnTimer(UINT_PTR nIDEvent) {
    ShowWindow(FALSE);
    m_ctrText.SetWindowText(_T(""));
    KillTimer(nIDEvent);
    CDialogEx::OnTimer(nIDEvent);
}

void CToastDlg::ShowToast(CString text) {
    CRect self;
    GetClientRect(self);

    int cx = GetSystemMetrics(SM_CXFULLSCREEN);
    int cy = GetSystemMetrics(SM_CYFULLSCREEN);

    int left = (cx - self.Width()) / 2;
    int top = (cy - self.Height()) / 2 + 50;

    MoveWindow(left, top, self.Width(), self.Height());

    m_ctrText.SetWindowText(text);

    UpdateData(TRUE);
    ShowWindow(TRUE);
    SetTimer(TIMER_EVENT_ID_BASE + m_timerCount, 1200, NULL);
}

int CToastDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) {
    if (CWnd::OnCreate(lpCreateStruct) == -1) {
        return -1;
    }
    ::SetWindowPos(GetSafeHwnd(), HWND_TOPMOST, 0, 0, 100, 100, SWP_NOMOVE | SWP_NOSIZE);
    SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(0, 150, LWA_ALPHA);
    return 0;
}
