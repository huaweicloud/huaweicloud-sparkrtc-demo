
// HwRtcLiveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HwRtcLive.h"
#include "HwRtcLiveDlg.h"
#include "NoticeDlg.h"
#include "afxdialogex.h"
#include "HwRtcMsgDef.h"
#include "HwRtcEngineMgr.h"
#include <iostream>
#include "StringUtil.h"
#include "HwVideoRect.h"
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace huawei::rtc;

// 需要向华为云申请并填写正确的APPID
const char* _APP_ID_ = "c45fc2ca33ad491fa7b8da5672240cd7";
const char* _DOMAIN_ = "";

int g_rtcMaxWatchNum = MAX_WATCH_NUM;
bool g_bNeedStop = false;

CHwRtcLiveDlg::CHwRtcLiveDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_HWRTCLIVE_DIALOG, pParent) {
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_trayIcon = AfxGetApp()->LoadIcon(IDI_TRAY);
    m_rtcEngineMgr = NULL;
}

void CHwRtcLiveDlg::DoDataExchange(CDataExchange* pDX) {
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_BTN_MIN, m_btnMin);
    DDX_Control(pDX, IDC_BTN_CLOSE, m_btnClose);

}

BEGIN_MESSAGE_MAP(CHwRtcLiveDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_MESSAGE(WM_JOINROOM, &CHwRtcLiveDlg::OnJoinRoom)
    ON_MESSAGE(WM_LEAVEROOM, &CHwRtcLiveDlg::OnLeaveRoom)
    ON_MESSAGE(WM_SHOW_TOAST, &CHwRtcLiveDlg::OnShowToast)
    ON_MESSAGE(WM_SHOW_NOTICE, &CHwRtcLiveDlg::OnShowNotice)
    ON_MESSAGE(WM_CHANGE_ROLE_RADIO, &CHwRtcLiveDlg::OnChangeRoleRadio)
    ON_WM_NCHITTEST()
    ON_BN_CLICKED(IDC_BTN_MIN, &CHwRtcLiveDlg::OnBnClickedBtnmin)
    ON_BN_CLICKED(IDC_BTN_CLOSE, &CHwRtcLiveDlg::OnBnClickedBtnclose)
    ON_WM_DESTROY()
    ON_COMMAND(ID_UPLOADLOG, &CHwRtcLiveDlg::onMenuUploadLog)
    ON_MESSAGE(WM_NOTIFY_MAIN_TRAY_ICON, &CHwRtcLiveDlg::OnSysTrayNotify)
    ON_MESSAGE(WM_USER_ENTER_LOGIN, &CHwRtcLiveDlg::OnEnterLogin)
    ON_MESSAGE(WM_USER_CREATE_ROOM, &CHwRtcLiveDlg::OnCreateRoom)
    ON_MESSAGE(WM_USER_RETURN, &CHwRtcLiveDlg::OnReturn)
END_MESSAGE_MAP()


// CHwRtcLiveDlg message handlers
BOOL CHwRtcLiveDlg::OnInitDialog() {
    CDialogEx::OnInitDialog();
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon
    m_ftTitle.CreateFont(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    m_ftLink.CreateFont(16, 0, 0, 0, FW_BOLD, FALSE, TRUE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    m_ftVer.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    const char *appid = _APP_ID_;
    if (strlen(appid) == 0) {
        MessageBox(_T("Please apply your own App ID to APP_ID"), _T("Notice"), MB_ICONINFORMATION);
        PostQuitMessage(0);
        return TRUE;
    }
    const char * domain = _DOMAIN_;
    m_rtcEngineMgr = CRtcEngineMgr::GetInstance(appid, domain);
    InitCtrls();
    InitChildDialog();
    m_rtcEngineMgr->SetMsgHandlerWnd(GetSafeHwnd());
    SetBackgroundImage(IDX_DLG_MAIN_BAKCGROUND);
    //默认开启打点
    IHRTCEngine* engine = CRtcEngineMgr::GetInstance()->GetEngine();
    engine->enableStats(true);
    // 加载系统托盘:初始化默认在线态
    m_sysTray.Init(m_hWnd, IDI_TRAY);
    BOOL bRetTip = m_sysTray.SetTip(_T("HwRTCLive"));
    if (!bRetTip) {
        // 加载失败重新设置
        RTC_LOG(L"m_sysTray.SetTip failed !!");
        (void)m_sysTray.SetTip(_T("HwRTCLive"));
    }
    BOOL bRetCall = m_sysTray.SetCallbackMessage(WM_NOTIFY_MAIN_TRAY_ICON);
    if (!bRetCall) {
        RTC_LOG(L"m_sysTray.SetCallbackMessage failed !!");
    }
    BOOL bRet = m_sysTray.SetIcon(m_trayIcon);
    if (!bRet) {
        RTC_LOG(L"m_sysTray.SetIcon failed!!%d", GetLastError());
    }
    return TRUE;
}

void CHwRtcLiveDlg::OnPaint() {
    CPaintDC dcPaint(this);
    if (IsIconic()) {
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dcPaint.GetSafeHdc()), 0);
        int iconWidth = GetSystemMetrics(SM_CXICON);
        int iconHeight = GetSystemMetrics(SM_CYICON);
        CRect client;
        GetClientRect(&client);
        int px = (client.Width() - iconWidth + 1) / 2;
        int py = (client.Height() - iconHeight + 1) / 2;
        dcPaint.DrawIcon(px, py, m_hIcon);
    } else {
        DrawClient(&dcPaint);
    }
}

LRESULT CHwRtcLiveDlg::OnNcHitTest(CPoint point) {
    LRESULT ret = CDialogEx::OnNcHitTest(point);
    if (ret == HTCLIENT && ::GetAsyncKeyState(MK_LBUTTON) < 0) {
        ret = HTCAPTION;
    }
    return ret;
}

HCURSOR CHwRtcLiveDlg::OnQueryDragIcon() {
    return static_cast<HCURSOR>(m_hIcon);
}

void CHwRtcLiveDlg::InitCtrls() {
    CRect ClientRect;
    CBitmap	bmpNetQuality;
    MoveWindow(0, 0, 500, 400, 1);
    GetClientRect(&ClientRect);
    m_btnMin.MoveWindow(ClientRect.Width() - 46, 1, 22, 22, TRUE);
    m_btnClose.MoveWindow(ClientRect.Width() - 23, 1, 22, 22, TRUE);
    m_btnSetting.MoveWindow(ClientRect.Width() - 69, 1, 22, 22, TRUE);
    m_btnSetting.ModifyStyle(0, BS_BITMAP);
    m_btnSetting.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDX_BTN_MAX)));
    CMFCButton::EnableWindowsTheming(FALSE);
}

void CHwRtcLiveDlg::InitChildDialog() {
    CString str;
    m_dlgTypeEnter.Create(CTypeEnterDlg::IDD, this);
    m_dlgVideo.Create(CVideoDlg::IDD, this);
    m_toastWnd.Create(CToastDlg::IDD, this);
    m_dlgCreate.Create(CCreateDlg::IDD, this);
    m_dlgTypeEnter.MoveWindow(0, 30, 500, 370, TRUE);
    m_dlgTypeEnter.ShowWindow(SW_SHOW);

}

// CEnterChannelDlg message deal with app
BOOL CHwRtcLiveDlg::PreTranslateMessage(MSG* pMsg) {
    if (pMsg->message == WM_KEYDOWN) {
        switch (pMsg->wParam) {
        case VK_ESCAPE:
            return FALSE;
        case VK_RETURN:
            return FALSE;
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}

void CHwRtcLiveDlg::DrawClient(CDC *lpDC) {
    CRect	rect;
    LPCTSTR title = NULL;
    GetClientRect(&rect);
    lpDC->SetBkColor(HRTC_BLUE);
    lpDC->SetTextColor(HRTC_WHITE);
    title = _T("Huawei RTC Live");
    lpDC->TextOut(11, 2, title, _tcslen(title));
    lpDC->SelectObject(&m_ftVer);
    lpDC->SetTextColor(RGB(0x91, 0x96, 0xA0));
    lpDC->SetBkColor(HRTC_WHITE);
}

void CHwRtcLiveDlg::SetEncParam() {
    int videoBitrate = 4096;
    vector<HRTCVideoEncParam> encVector;
    HRTCVideoEncParam param;
    param = { HRTC_STREAM_TYPE_SD,320,180,15,10,300,80,false };
    encVector.push_back(param);
    param = { HRTC_STREAM_TYPE_LD,160,90, 15,10,150,70,false };
    encVector.push_back(param);
    param = { HRTC_STREAM_TYPE_HD,640,360,15,10,1200, 200,false };
    encVector.push_back(param);
    param = { HRTC_STREAM_TYPE_FHD,1280,720,15,10,2800,500,false };
    encVector.push_back(param);
    m_rtcEngineMgr->setVideoEncParam(videoBitrate, encVector);
}

LRESULT CHwRtcLiveDlg::OnJoinRoom(WPARAM wParam, LPARAM lParam) {
    if (m_rtcEngineMgr == NULL) {
        m_toastWnd.ShowToast(L"program init rtc-sdk fail. please restart the program");
        return 0;
    }
    m_toastWnd.ShowWindow(FALSE);
    const ClassRoleInfo* classInfo = RTC_ENGINEMGR()->GetClassRoleInfo();
    CString cstrRoom;
    cstrRoom = CA2T(classInfo->roomId.c_str());
    CString cstrUid;
    cstrUid = CA2T(classInfo->userId.c_str());
    m_dlgVideo.SetRoomId(classInfo->roomId);
    m_dlgVideo.ResetData();
    m_dlgVideo.SetOwnUserId(cstrUid);
    SetEncParam();
    HRTC_USER_NAME_MAP[classInfo->userId] = classInfo->userName;
    int ret = m_rtcEngineMgr->JoinRoom(classInfo->userId.c_str(), classInfo->userName.c_str(), classInfo->roomId.c_str(), false);
    if (ret != 0) {
        return 0;
    }
    g_rtcMaxWatchNum = MAX_WATCH_NUM;
    m_rtcEngineMgr->SetMsgHandlerWnd(m_dlgVideo.GetSafeHwnd());
    m_dlgVideo.ResetWindow();
    if (RTC_ENGINEMGR()->GetUserRole(classInfo->roomId) != HRTC_ROLE_TYPE_PLAYER) {
        m_rtcEngineMgr->setupLocalVideo(m_dlgVideo.GetLocalVideoWnd());
        m_dlgVideo.ShowLocalWnd();
    }
    int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
    int height = HW_RTC_VIDEO_DLG_HIGHT;
    if (nScreenHeight <= HW_RTC_VIDEO_DLG_HIGHT) {
        height = nScreenHeight - 20;
    }
    m_dlgVideo.MoveWindow(0, 0, HW_RTC_VIDEO_DLG_WEIGHT, height, 1);
    m_dlgVideo.ShowWindow(SW_SHOW);
    m_dlgVideo.CenterWindow();
    m_dlgVideo.SetWindowText(_T("HuaweiRTCLiveDemo"));
    ShowWindow(SW_HIDE);
    return 0;
}

LRESULT CHwRtcLiveDlg::OnLeaveRoom(WPARAM wParam, LPARAM lParam) {
    m_rtcEngineMgr->leaveRoom();
    return 0;
}

LRESULT CHwRtcLiveDlg::OnEnterLogin(WPARAM wParam, LPARAM lParam) {
    m_dlgTypeEnter.ShowWindow(SW_HIDE);
    m_dlgCreate.ShowWindow(SW_HIDE);
    m_dlgEnterRoom.Create(CEnterRoomDlg::IDD, this);
    m_dlgEnterRoom.MoveWindow(0, 30, 500, 370, TRUE);
    m_dlgEnterRoom.ShowWindow(SW_SHOW);
    return 0;
}

LRESULT CHwRtcLiveDlg::OnCreateRoom(WPARAM wParam, LPARAM lParam) {
    m_dlgEnterRoom.ShowWindow(SW_HIDE);
    m_dlgCreate.MoveWindow(0, 30, 500, 370, TRUE);
    m_dlgCreate.ShowWindow(SW_SHOW);
    return 0;
}

LRESULT CHwRtcLiveDlg::OnReturn(WPARAM wParam, LPARAM lParam) {
    m_dlgEnterRoom.ShowWindow(SW_SHOW);
    m_dlgCreate.ShowWindow(SW_HIDE);
    return 0;
}

LRESULT CHwRtcLiveDlg::OnShowNotice(WPARAM wParam, LPARAM lParam) {
    TCHAR* tmpChar = (TCHAR*)lParam;
    CNoticeDlg dlgAbout;
    dlgAbout.MessageBox(tmpChar);
    dlgAbout.DoModal();
    CCSafeDeleteArray(tmpChar);
    return 0;
}

LRESULT CHwRtcLiveDlg::OnShowToast(WPARAM wParam, LPARAM lParam) {
    TCHAR* tmpChar = (TCHAR*)lParam;
    m_toastWnd.ShowToast(tmpChar);
    CCSafeDeleteArray(tmpChar);
    return 0;
}

LRESULT CHwRtcLiveDlg::OnChangeRoleRadio(WPARAM wParam, LPARAM lParam) {
    HRTCRoleType type = (HRTCRoleType)wParam;
    m_dlgEnterRoom.SetUserRole(type);
    return 0;
}

void CHwRtcLiveDlg::OnDestroy() {
    CRtcEngineMgr::FreeInstance();
    CDialogEx::OnDestroy();
}

void CHwRtcLiveDlg::OnBnClickedBtnmin() {
    ShowWindow(SW_MINIMIZE);
}

void CHwRtcLiveDlg::OnBnClickedBtnclose() {
    CRtcEngineMgr::FreeInstance();
    CDialogEx::OnCancel();
}

LRESULT CHwRtcLiveDlg::OnSysTrayNotify(WPARAM wParam, LPARAM lParam) {
    switch (lParam) {
    case WM_RBUTTONUP: {
        CreateLoginMenu();
        break;
    }
    case WM_LBUTTONDBLCLK:      // 左双击
    case WM_LBUTTONDOWN: {
        if (IsWindowVisible()) {
            ShowWindow(SW_RESTORE);
            SetForegroundWindow();
        } else {
            m_dlgVideo.SetForegroundWindow();
        }
        break;
    }
    default:
        break;
    }
    return 0L;
}

void CHwRtcLiveDlg::CreateLoginMenu() {
    // 获取鼠标指针位置
    POINT ptCursor = { 0 };
    ::GetCursorPos(&ptCursor);
    CMenu			Menu;
    MENUITEMINFO	mii;
    UINT            nFlag = 0;
    Menu.LoadMenu(IDR_MENU_MORE);
    CMenu *pMenuPopup = Menu.GetSubMenu(0);
    // pMenuPopup = NULL;
    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fMask = MIIM_STRING;

    IHRTCEngine* engine = CRtcEngineMgr::GetInstance()->GetEngine();
    IHRTCAudioDeviceManager*  audio = engine->getAudioDeviceManager();
    mii.dwTypeData = const_cast<LPTSTR>(_T("UploadLog"));
    mii.cch = _tcslen(_T("UploadLog")) + 1;
    pMenuPopup->SetMenuItemInfo(0, &mii, FALSE);
    pMenuPopup->SetMenuItemBitmaps(ID_UPLOADLOG, MF_BYCOMMAND, NULL, NULL);
    int x = ptCursor.x - 60;
    int y = ptCursor.y - 50;
    pMenuPopup->TrackPopupMenu(TPM_RIGHTBUTTON, x, y, this);
    pMenuPopup->DestroyMenu();

}

void CHwRtcLiveDlg::onMenuUploadLog() {
    IHRTCEngine* engine = CRtcEngineMgr::GetInstance()->GetEngine();
    if (engine) {
        engine->logUpload();
    }
}

