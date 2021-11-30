// CreateDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CreateDlg.h"
#include "afxdialogex.h"
#include "HwRtcEngineMgr.h"
#include "HwRtcLive.h"
#include "StringUtil.h"
// CCreateDlg 对话框

IMPLEMENT_DYNAMIC(CCreateDlg, CDialogEx)

static const CHAR g_buildTime[] = " (" __DATE__ " " __TIME__ ")";
CCreateDlg::CCreateDlg(CWnd* pParent)
    : CDialogEx(CCreateDlg::IDD, pParent) {

}

CCreateDlg::~CCreateDlg() {
}

void CCreateDlg::DoDataExchange(CDataExchange* pDX) {
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_ROOM_ID, m_ctrRoomId);
    DDX_Control(pDX, IDC_EDIT_USER_ID, m_ctrUserId);
    DDX_Control(pDX, IDC_BTN_JOIN_ROOM, m_btnJoin);
    DDX_Control(pDX, IDC_BTN_RETURN, m_btnReturn);
}

BEGIN_MESSAGE_MAP(CCreateDlg, CDialogEx)
    ON_WM_NCHITTEST()
    ON_WM_PAINT()
    ON_BN_CLICKED(IDC_BTN_JOIN_ROOM, &CCreateDlg::OnBnClickedBtnJoinRoom)
    ON_BN_CLICKED(IDC_BTN_RETURN, &CCreateDlg::OnBnClickedBtnReturn)
END_MESSAGE_MAP()

BOOL CCreateDlg::OnInitDialog() {
    CDialogEx::OnInitDialog();
    m_ftBtn.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    m_ftText.CreateFont(30, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    m_penFrame.CreatePen(PS_SOLID, 1, HRTC_GRAY);
    SetBackgroundColor(HRTC_WHITE);
    InitCtrls();
    SetBackgroundColor(HRTC_WHITE);
    return TRUE;
}

void CCreateDlg::OnBnClickedBtnJoinRoom() {
    SetClassRoleInfo();
    GetParent()->SendMessage(WM_JOINROOM, 0, 0);
}

void CCreateDlg::OnBnClickedBtnReturn() {
    GetParent()->SendMessage(WM_USER_RETURN, 0, 0);
}

void CCreateDlg::InitCtrls() {
    m_ctrUserId.SetFont(&m_ftDesc);
    m_ctrRoomId.SetFont(&m_ftDesc);
    m_btnJoin.SetFont(&m_ftBtn);
    m_btnJoin.SetWindowText(_T("加入房间"));
    m_btnReturn.SetFont(&m_ftBtn);
    m_btnReturn.SetWindowText(_T("返回"));
    CMFCButton::EnableWindowsTheming(FALSE);
}

void CCreateDlg::SetClassRoleInfo() {
    //默认以学生身份进入，如果需要老师身份进入，请参照EnterRoomDlg.cpp
    ClassRoleInfo classRoleInfo;
    classRoleInfo.role = CLASS_ROLE_STUDENT;
    CString strUserId;
    CString strRoomId;
    m_ctrUserId.GetWindowText(strUserId);
    m_ctrRoomId.GetWindowText(strRoomId);
    classRoleInfo.userId = CT2A(strUserId.GetString());
    classRoleInfo.userName = classRoleInfo.userId;
    classRoleInfo.roomId = CT2A(strRoomId.GetString());
    classRoleInfo.classRoomId = classRoleInfo.roomId;
    classRoleInfo.gradeRoomId = classRoleInfo.classRoomId;
    classRoleInfo.groupRoomId = classRoleInfo.classRoomId;
    classRoleInfo.roomId = classRoleInfo.roomId;
    RTC_ENGINEMGR()->SetClassRoleInfo(classRoleInfo);
    RTC_ENGINEMGR()->SetUserRole(classRoleInfo.roomId, HRTC_ROLE_TYPE_JOINER);
}

void CCreateDlg::OnPaint() {
    CPaintDC dc(this); // device context for painting
    DrawClient(&dc);
}

void CCreateDlg::DrawClient(CDC * lpDC) {
    CRect	rcText;
    CRect	rcClient;
    CString lpString = NULL;
    int height = 36;
    GetClientRect(&rcClient);

    CFont* defFont = lpDC->SelectObject(&m_ftDesc);
    lpDC->SetBkColor(HRTC_WHITE);
    lpDC->SetTextColor(RGB(0x44, 0x45, 0x46));
    m_ctrUserId.MoveWindow(rcClient.Width() / 2 - 60, 162, 180, 22, TRUE);
    m_ctrRoomId.MoveWindow(rcClient.Width() / 2 - 60, 116, 180, 22, TRUE);
    m_btnJoin.MoveWindow(rcClient.Width() / 2, 280, 135, height, TRUE);
    m_btnReturn.MoveWindow(rcClient.Width() / 2 - 140, 280, 135, height, TRUE);

    lpDC->SelectObject(&m_penFrame);

    rcText.SetRect(rcClient.Width() / 2 - 100, 108, rcClient.Width() / 2 + 130, 142);
    lpDC->RoundRect(&rcText, CPoint(32, 32));
    lpString.LoadString(IDS_STRING_ROOMID) ;
    lpDC->TextOut(rcClient.Width() / 2 - 160, 116, lpString, _tcslen(lpString));
    rcText.OffsetRect(0, 48);
    lpDC->RoundRect(&rcText, CPoint(32, 32));
    lpString.LoadString(IDS_STRING_USERID);
    lpDC->TextOut(rcClient.Width() / 2 - 160, 164, lpString, _tcslen(lpString));

    lpDC->SelectObject(defFont);

    char* versionTemp = getVersion();
    std::string const& versionAndBuildTime = std::string(versionTemp) + std::string(g_buildTime);
    char const *verTime = versionAndBuildTime.c_str();
    wchar_t *version = StringUtility::HW_Utf8ToUnicodeW(verTime);
    CString	 strInfo;
    strInfo.Format(_T("V %s"), version);
    lpString = strInfo;
    lpDC->TextOut(rcClient.Width() / 2 - 90, 340, lpString, _tcslen(lpString));
    if (version != NULL) {
        free(version);
    }
}
