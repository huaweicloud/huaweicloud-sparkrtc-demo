// MusicDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UserListDlg.h"
#include "afxdialogex.h"
#include "HwRtcLive.h"
#include "HwRtcMsgDef.h"
#include "VideoDlg.h"
#include "StringUtil.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <vector>
using namespace std;
vector<UserWatchInfo> g_LiveUser;
extern int g_rtcMaxWatchNum;

TCHAR g_ownUserId[HRTCConstant::HRTC_MAX_USERID_LEN];

// CMusicDlg 对话框
CUserListDlg::CUserListDlg(CWnd* pParent )
    : CDialogEx(CUserListDlg::IDD, pParent) {
    m_pVideoObject = NULL;
}

void CUserListDlg::DoDataExchange(CDataExchange* pDX) {
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_USER, m_listUser);
    DDX_Control(pDX, IDC_STATIC_USER_COUNT, m_txtCount);
}

BEGIN_MESSAGE_MAP(CUserListDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_ERASEBKGND()
    ON_WM_LBUTTONDBLCLK()

END_MESSAGE_MAP()


// CMusicDlg 消息处理程序
BOOL CUserListDlg::OnInitDialog() {
    CDialogEx::OnInitDialog();
    SetWindowPos(NULL, 0, 0, 310, 490, SWP_NOMOVE);

    RefreshListBox();
    return TRUE;  
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUserListDlg::OnPaint() {
    if (IsIconic()) {
        CPaintDC dcPaint(this); // 用于绘制的设备上下文

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dcPaint.GetSafeHdc()), 0);
    } else {
        CDialogEx::OnPaint();
    }
}

BOOL CUserListDlg::OnEraseBkgnd(CDC* pDC) {
    return TRUE;
}


void CUserListDlg::AddOwn(TCHAR *userId) {
    if (m_pVideoObject == NULL) {
        return;
    }
    UserWatchInfo info;
    memset(&info, 0, sizeof(UserWatchInfo));
    _tcscpy_s(info.userId, HRTCConstant::HRTC_MAX_USERID_LEN, userId);
    g_LiveUser.insert(g_LiveUser.begin(), info);

    memset(g_ownUserId, 0, HRTCConstant::HRTC_MAX_USERID_LEN);
    _tcscpy_s(g_ownUserId, HRTCConstant::HRTC_MAX_USERID_LEN, userId);
    TCHAR * userName = NULL;
    char *userIdC = StringUtility::HW_UnicodeToUtf8(userId);
    string uid(userIdC);
    if (HRTC_USER_NAME_MAP.find(uid) != HRTC_USER_NAME_MAP.end()) {
        userName = StringUtility::HW_Utf8ToUnicodeW(HRTC_USER_NAME_MAP[uid].c_str());
    }
    if (userIdC != NULL)         {
        free(userIdC);
    }
    if (userName) {
        m_listUser.InsertString(0, userName);
        if (userName != NULL) {
            free(userName);
        }
    } else {
        m_listUser.InsertString(0, userId);
    }

    Invalidate();
}

void CUserListDlg::AddUser(TCHAR *userId) {
    if (m_pVideoObject == NULL) {
        return;
    }
    UserWatchInfo info;
    memset(&info, 0, sizeof(UserWatchInfo));
    _tcscpy_s(info.userId, HRTCConstant::HRTC_MAX_USERID_LEN, userId);
    int count = m_pVideoObject->GetWndSize();
    if (count < g_rtcMaxWatchNum || m_pVideoObject->IsTeacherUserId(userId)) {
        info.bWatch = TRUE;
    } else {
        info.bWatch = FALSE;
    }

    g_LiveUser.push_back(info);

    TCHAR * userName = NULL;
    char *userIdC = StringUtility::HW_UnicodeToUtf8(userId);
    string uid(userIdC);
    if (HRTC_USER_NAME_MAP.find(uid) != HRTC_USER_NAME_MAP.end()) {
        userName = StringUtility::HW_Utf8ToUnicodeW(HRTC_USER_NAME_MAP[uid].c_str());
    }
    if (userIdC != NULL) {
        free(userIdC);
    }
    if (userName) {
        m_listUser.AddString(userName);
        if (userName != NULL) {
            free(userName);
        }
    } else {
        m_listUser.AddString(userId);
    }
    RefreshListBox();
    Invalidate();
}

void CUserListDlg::UpdateUser(TCHAR *userId) {
    for (unsigned int index = 0; index < g_LiveUser.size(); index++) {
        UserWatchInfo info = g_LiveUser[index];
        if (wcsncmp(info.userId, userId, wcslen(info.userId)) == 0) {
            g_LiveUser[index].bWatch = false;
            break;
        }
    }
    Invalidate();
}

void CUserListDlg::DeleteUser(TCHAR *userId) {

    vector <UserWatchInfo> ::iterator it;
    int index = -1;
    for (it = g_LiveUser.begin(); it != g_LiveUser.end(); ++it) {
        index++;
        UserWatchInfo info = (*it);
        if (wcsncmp(info.userId, userId, wcslen(info.userId)) == 0) {
            g_LiveUser.erase(it);
            break;
        }
    }
    if (index > -1) {
        m_listUser.DeleteString(index);
    }
    RefreshListBox();
    Invalidate();
}

bool CUserListDlg::IsExist(TCHAR *userId) {

    vector <UserWatchInfo> ::iterator it;
    for (it = g_LiveUser.begin(); it != g_LiveUser.end(); ++it) {
        UserWatchInfo info = (*it);
        if (wcsncmp(info.userId, userId, wcslen(info.userId)) == 0) {
            return true;
        }
    }
    return false;
}

void CUserListDlg::ClearData() {
    m_listUser.ResetContent();
    g_LiveUser.clear();
    return;
}

void CUserListDlg::RefreshListBox() {
    for (int i = 0; i < m_listUser.GetCount(); i++) {
        m_listUser.SetItemHeight(i, 32);
    }
    CFont fntUser;
    fntUser.CreatePointFont(10, _T("MS Sans Serif"));
    m_listUser.SetFont(&fntUser);
    CFont fntCount;
    fntCount.CreatePointFont(20, _T("MS Sans Serif"));
    m_txtCount.SetFont(&fntCount);

    CString text;
    text.Format(_T("User List(%d)"), g_LiveUser.size());
    m_txtCount.SetWindowTextW(text);
    m_listUser.Invalidate(TRUE);
}

void CUserListDlg::SetVideoObject(CVideoDlg * videoObject) {
    m_pVideoObject = videoObject;
}

