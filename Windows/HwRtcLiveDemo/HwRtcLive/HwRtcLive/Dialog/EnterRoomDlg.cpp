// EnterChannelDlg.cpp : implement file
//

#include "stdafx.h"
#include "HwRtcLive.h"
#include "EnterRoomDlg.h"
#include "HwRtcMsgDef.h"
#include "afxdialogex.h"
#include "HwRtcEngineMgr.h"
#include "StringUtil.h"
using namespace std;
#include <string.h>
// CEnterChannelDlg dialog

IMPLEMENT_DYNAMIC(CEnterRoomDlg, CDialogEx)

static const CHAR g_buildTime[] = " (" __DATE__ " " __TIME__ ")";

CEnterRoomDlg::CEnterRoomDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CEnterRoomDlg::IDD, pParent) {

}

CEnterRoomDlg::~CEnterRoomDlg() {

}

void CEnterRoomDlg::DoDataExchange(CDataExchange* pDX) {
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_USERID, m_ctrUserId);
    DDX_Control(pDX, IDC_RADIO_TEACHER, m_radioTeacher);
    DDX_Control(pDX, IDC_RADIO_STUDENT, m_radioStudent);
    DDX_Control(pDX, IDC_BTN_JOIN_CHANNEL, m_btnJoin);
    DDX_Control(pDX, IDC_BTN_CREATE_CHANNEL, m_btnCreat);
    DDX_Control(pDX, IDC_BTN_PRE_SET, m_btnPreSet);
    DDX_Control(pDX, IDC_ENCODE_LD, m_encodeLD);
    DDX_Control(pDX, IDC_ENCODE_SD, m_encodeSD);
    DDX_Control(pDX, IDC_ENCODE_HD, m_encodeHD);
    DDX_Control(pDX, IDC_ENCODE_FHD, m_encodeFHD);
    DDX_Control(pDX, IDC_COMBO_GRADE, m_cboxGrade);
}

BEGIN_MESSAGE_MAP(CEnterRoomDlg, CDialogEx)
    ON_WM_NCHITTEST()
    ON_WM_PAINT()
    ON_BN_CLICKED(IDC_BTN_JOIN_CHANNEL, &CEnterRoomDlg::OnBnClickedBtnJoinRoom)
    ON_BN_CLICKED(IDC_RADIO_TEACHER, &CEnterRoomDlg::OnBnClickedRadioTeacher)
    ON_BN_CLICKED(IDC_RADIO_STUDENT, &CEnterRoomDlg::OnBnClickedRadioStudent)
    ON_BN_CLICKED(IDC_BTN_PRE_SET, &CEnterRoomDlg::OnBnClickedBtnPreSet)
    ON_BN_CLICKED(IDC_BTN_CREATE_CHANNEL, &CEnterRoomDlg::OnBnClickedBtnCreate)
END_MESSAGE_MAP()

// CEnterChannelDlg message deal with app
BOOL CEnterRoomDlg::PreTranslateMessage(MSG* pMsg) {
    if (pMsg->message == WM_KEYDOWN) {
        switch (pMsg->wParam) {
        case VK_ESCAPE:
            return FALSE;
        case VK_RETURN: {
            if (IsWindowVisible()) {
                OnBnClickedBtnJoinRoom();
            }
            return FALSE;
        }
        }
    }

    return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CEnterRoomDlg::OnInitDialog() {
    CDialogEx::OnInitDialog();
    m_ftDesc.CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    m_ftBtn.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    m_penFrame.CreatePen(PS_SOLID, 1, HRTC_GRAY);

    SetBackgroundColor(HRTC_WHITE);
    InitCtrls();
    InitCboxClass();
    m_radioTeacher.SetCheck(TRUE);
    m_dlgDevice.Create(CDeviceDlgNew::IDD, this);
    return TRUE;
}

void CEnterRoomDlg::InitCboxClass() {
    HwRtcEnterRoomType type = RTC_ENGINEMGR()->GetEnterType();
    m_gradeIdList.push_back("room0");
    m_gradeIdList.push_back("room1");
    m_gradeIdList.push_back("room2");
    m_gradeIdList.push_back("room3");
    m_gradeIdList.push_back("room4");
    m_gradeIdList.push_back("room5");
    m_gradeIdList.push_back("room6");
    m_gradeIdList.push_back("room7");
    m_cboxGrade.ResetContent();
    for (UINT nIndex = 0; nIndex < m_gradeIdList.size(); nIndex++) {
        TCHAR* gradeId = StringUtility::HW_Utf8ToUnicodeW(m_gradeIdList[nIndex].c_str());
        m_cboxGrade.InsertString(nIndex, gradeId);
        m_cboxGrade.SetCurSel(0);
        if (gradeId != NULL) {
            free(gradeId);
        }
    }
}

void CEnterRoomDlg::InitCtrls() {
    m_ctrUserId.SetFont(&m_ftDesc);
    m_ctrUserId.ShowWindow(SW_HIDE);

    m_cboxGrade.ShowWindow(SW_SHOW);
    m_cboxGrade.SetFont(&m_ftDesc);

    m_radioTeacher.SetFont(&m_ftBtn);
    m_radioStudent.SetFont(&m_ftBtn);

    m_encodeLD.SetCheck(1);
    m_encodeSD.SetCheck(1);
    m_encodeHD.SetCheck(1);
    m_encodeSD.ShowWindow(SW_HIDE);
    m_encodeHD.ShowWindow(SW_HIDE);

    m_btnJoin.SetFont(&m_ftBtn);
    m_btnJoin.SetWindowText(_T("加入房间"));

    m_btnPreSet.SetFont(&m_ftBtn);
    m_btnPreSet.SetWindowText(_T("设置"));

    m_btnCreat.SetFont(&m_ftBtn);
    m_btnCreat.SetWindowText(_T("创建房间"));

    CMFCButton::EnableWindowsTheming(FALSE);
}

void CEnterRoomDlg::OnPaint() {
    CPaintDC dc(this); // device context for painting

    DrawClient(&dc);
}

void CEnterRoomDlg::DrawClient(CDC *lpDC) {
    CRect	rcText;
    CRect	rcClient;
    CString lpString = NULL;
    GetClientRect(&rcClient);
    HwRtcEnterRoomType type = RTC_ENGINEMGR()->GetEnterType();

    int radioWidth = 70;
    int height = 36;
    int distance = 70;
    m_radioTeacher.MoveWindow(rcClient.Width() / 2 - distance - radioWidth / 2, 25, 100, 30, TRUE);
    m_radioStudent.MoveWindow(rcClient.Width() / 2 + distance - radioWidth / 2, 25, 100, 30, TRUE);
    m_ctrUserId.MoveWindow(rcClient.Width() / 2 - 60, 162, 180, 22, TRUE);
    m_btnJoin.MoveWindow(rcClient.Width() / 2 - 60, 280, 100, height, TRUE);
    m_btnPreSet.MoveWindow(rcClient.Width() / 2 - 200, 280, 100, height, TRUE);
    m_btnCreat.MoveWindow(rcClient.Width() / 2 + 80, 280, 100, height, TRUE);

    CFont* defFont = lpDC->SelectObject(&m_ftDesc);
    lpDC->SetBkColor(HRTC_WHITE);
    lpDC->SetTextColor(RGB(0x44, 0x45, 0x46));
    lpDC->SelectObject(&m_penFrame);

    if (m_radioTeacher.GetCheck()) {
        rcText.SetRect(rcClient.Width() / 2 - 100, 108, rcClient.Width() / 2 + 130, 142);
        lpDC->RoundRect(&rcText, CPoint(32, 32));
        lpString.LoadString(IDS_STRING_CLASS);
        lpDC->TextOut(rcClient.Width() / 2 - 160, 116, lpString, _tcslen(lpString));
    } else {
        rcText.SetRect(rcClient.Width() / 2 - 100, 108, rcClient.Width() / 2 + 130, 142);
        lpDC->RoundRect(&rcText, CPoint(32, 32));
        lpString.LoadString(IDS_STRING_CLASS);
        lpDC->TextOut(rcClient.Width() / 2 - 160, 116, lpString, _tcslen(lpString));
        rcText.OffsetRect(0, 48);
        lpDC->RoundRect(&rcText, CPoint(32, 32));
        lpString.LoadString(IDS_STRING_USERNAME);
        lpDC->TextOut(rcClient.Width() / 2 - 160, 164, lpString, _tcslen(lpString));
    }
    char* versionTemp = getVersion();
    std::string const& versionAndBuildTime = std::string(versionTemp) + std::string(g_buildTime);
    char const *verTime = versionAndBuildTime.c_str();
    wchar_t *version = StringUtility::HW_Utf8ToUnicodeW(verTime);
    CString	strInfo;
    strInfo.Format(_T("V %s"), version);
    lpString = strInfo;
    lpDC->TextOut(rcClient.Width() / 2 - 90, 340, lpString, _tcslen(lpString));
    if (version != NULL) {
        free(version);
    }
}

void CEnterRoomDlg::OnBnClickedBtnJoinRoom() {
    SetClassRoleInfo();
    m_dlgDevice.ShowWindow(FALSE);
    GetParent()->SendMessage(WM_JOINROOM, 0, 0);
}

void CEnterRoomDlg::SetClassRoleInfo() {
    ClassRoleInfo classRoleInfo;
    classRoleInfo.role = CLASS_ROLE_TEACHER;
    int idx = m_cboxGrade.GetCurSel();
    if (idx > -1) {
        classRoleInfo.gradeRoomId.assign(m_gradeIdList[idx]);
    }
    classRoleInfo.userId = "teacher_" + classRoleInfo.gradeRoomId;
    classRoleInfo.userName = "teacher";
    classRoleInfo.roomId = classRoleInfo.gradeRoomId;
    if (m_radioStudent.GetCheck()) {
        classRoleInfo.role = CLASS_ROLE_STUDENT;
        CString strUserId;
        m_ctrUserId.GetWindowText(strUserId);
        classRoleInfo.userId = CT2A(strUserId.GetString());
        classRoleInfo.userName = classRoleInfo.userId;
        classRoleInfo.classRoomId = classRoleInfo.roomId;
        classRoleInfo.gradeRoomId = classRoleInfo.classRoomId;
        classRoleInfo.groupRoomId = classRoleInfo.classRoomId;
        classRoleInfo.roomId = classRoleInfo.roomId;
    }
    RTC_ENGINEMGR()->SetClassRoleInfo(classRoleInfo);
    RTC_ENGINEMGR()->SetUserRole(classRoleInfo.roomId, HRTC_ROLE_TYPE_JOINER);
}

void CEnterRoomDlg::SetUserRole(HRTCRoleType type) {
    switch (type) {
    case HRTC_ROLE_TYPE_JOINER: {
        m_radioPlayer.SetCheck(FALSE);
        m_radioJoiner.SetCheck(TRUE);
        break;
    }
    case HRTC_ROLE_TYPE_PLAYER: {
        m_radioPlayer.SetCheck(TRUE);
        m_radioJoiner.SetCheck(FALSE);
        break;
    }
    default:
        break;
    }
}

void CEnterRoomDlg::OnBnClickedRadioTeacher() {
    m_ctrUserId.ShowWindow(SW_HIDE);
    m_cboxGrade.ShowWindow(SW_SHOW);
    Invalidate();
}


void CEnterRoomDlg::OnBnClickedRadioStudent() {
    m_ctrUserId.ShowWindow(SW_SHOW);
    m_cboxGrade.ShowWindow(SW_SHOW);
    Invalidate();
}

void CEnterRoomDlg::OnBnClickedBtnPreSet() {
    m_dlgDevice.ShowWindow(SW_SHOW);
    m_dlgDevice.CenterWindow();
}

void CEnterRoomDlg::OnBnClickedBtnCreate() {
    //创建房间以学生的身份进入房间
    m_dlgDevice.ShowWindow(SW_HIDE);
    GetParent()->SendMessage(WM_USER_CREATE_ROOM, 0, 0);
}
