// TypeEnterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HwRtcLive.h"
#include "HwRtcEngineMgr.h"
#include "TypeEnterDlg.h"
#include "HwRtcMsgDef.h"
#include "afxdialogex.h"


// CTypeEnterDlg 对话框

IMPLEMENT_DYNAMIC(CTypeEnterDlg, CDialogEx)

CTypeEnterDlg::CTypeEnterDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CTypeEnterDlg::IDD, pParent) {

}

CTypeEnterDlg::~CTypeEnterDlg() {
}

void CTypeEnterDlg::DoDataExchange(CDataExchange* pDX) {
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_BTN_1VN, m_btn1VN);
}


BEGIN_MESSAGE_MAP(CTypeEnterDlg, CDialogEx)
    ON_WM_PAINT()
    ON_BN_CLICKED(IDC_BTN_1VN, &CTypeEnterDlg::OnBnClickedBtnLogin1VN)
END_MESSAGE_MAP()

BOOL CTypeEnterDlg::OnInitDialog() {
    CDialogEx::OnInitDialog();
    m_ftDesc.CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    m_ftBtn.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    m_penFrame.CreatePen(PS_SOLID, 1, HRTC_GRAY);
    SetBackgroundColor(HRTC_WHITE);
    InitCtrls();
    return TRUE;
}

void CTypeEnterDlg::InitCtrls() {
    m_btn1VN.SetFont(&m_ftBtn);
    m_btn1VN.SetWindowText(_T("大班课模式"));
    CMFCButton::EnableWindowsTheming(FALSE);
}
void CTypeEnterDlg::OnPaint() {
    CPaintDC dc(this); // device context for painting        
    DrawClient(&dc);
}

void CTypeEnterDlg::DrawClient(CDC *lpDC) {
    CRect	rcClient;
    GetClientRect(&rcClient);
    int height = 40;
    int width = 135;
    m_btn1VN.MoveWindow(rcClient.Width() / 2 - width / 2, rcClient.Height() / 2 - height / 2, width, height, TRUE);
}

void CTypeEnterDlg::OnBnClickedBtnLogin1VN() {
    RTC_ENGINEMGR()->SetEnterType(ENTER_ROOM_1VN);
    GetParent()->SendMessage(WM_USER_ENTER_LOGIN, 0, 0);
}

