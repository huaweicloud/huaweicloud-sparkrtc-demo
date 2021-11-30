// VideoDlg.cpp 
//

#include "stdafx.h"
#include "HwRtcLive.h"
#include "VideoDlg.h"
#include "afxdialogex.h"
#include "HwRtcMsgDef.h"
#include "HwRtcEngineMgr.h"
#include "HRTCDefine.h"
#include "IHRTCEngine.h"
#include "StringUtil.h"
#include "NoticeDlg.h"
#include "HwVideoRect.h"
using namespace huawei::rtc;
// CVideoDlg
extern vector<UserWatchInfo> g_LiveUser;
IMPLEMENT_DYNAMIC(CVideoDlg, CDialogEx)

CVideoDlg::CVideoDlg(CWnd* pParent)
    : CDialogEx(CVideoDlg::IDD, pParent) {
    m_brHead.CreateSolidBrush(HRTC_BLUE);
    SetBackgroundColor(HRTC_BLACK);
    m_bFullScreen = FALSE;

    memset(&m_shareWndInfo, 0, sizeof(m_shareWndInfo));
    memset(&m_teacherWndInfo, 0, sizeof(m_teacherWndInfo));
    memset(&m_teacherUserId, 0, 128);

    m_watchingCount = 0;
    m_bRecordingClosed = false;
    m_bVideoClosed = false;
    m_bPlaybackClosed = false;
    m_bScreenShared = false;
}

CVideoDlg::~CVideoDlg() {
    m_brHead.DeleteObject();
}

void CVideoDlg::DoDataExchange(CDataExchange* pDX) {
    CDialogEx::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_BTN_MIN_VIDEO, m_btnMin);
    DDX_Control(pDX, IDC_BTNRSTO_VIDEO, m_btnRst);
    DDX_Control(pDX, IDC_BTN_CLOSE_VIDEO, m_btnClose);
}


BEGIN_MESSAGE_MAP(CVideoDlg, CDialogEx)
    ON_WM_SIZE()
    ON_WM_MOUSEMOVE()
    ON_WM_PAINT()
    ON_WM_NCHITTEST()
    ON_WM_DESTROY()
    ON_WM_CLOSE()
    ON_WM_SHOWWINDOW()
    ON_WM_MOVE()

    ON_COMMAND(ID_IDR_SETDEVICE, &CVideoDlg::OnIdrSetdevice)
    ON_COMMAND(ID_IDR_SHOWINFO, &CVideoDlg::OnIdrShowinfo)

    ON_BN_CLICKED(IDC_BTN_MIN_VIDEO, &CVideoDlg::OnBnClickedBtnmin)
    ON_BN_CLICKED(IDC_BTN_CLOSE_VIDEO, &CVideoDlg::OnBnClickedBtnclose)
    ON_BN_CLICKED(IDC_BTNRSTO_VIDEO, &CVideoDlg::OnBnClickedBtnrest)
    ON_BN_CLICKED(IDC_BTN_HANGUP, &CVideoDlg::OnBnClickedBtnclose)
    ON_BN_CLICKED(IDC_BTN_MICROPHONE, &CVideoDlg::OnBnClickedBtnMicrophone)
    ON_BN_CLICKED(IDC_BTN_SPEAKER, &CVideoDlg::OnBnClickedBtnPlayback)
    ON_BN_CLICKED(IDC_BTN_CAMERA, &CVideoDlg::OnBnClickedBtnCamera)
    ON_BN_CLICKED(IDC_BTN_SETTING_MORE, &CVideoDlg::OnBnClickedMore)
    ON_BN_CLICKED(IDC_BTN_SHOW_USERLIST, &CVideoDlg::OnBnClickedUserList)
    ON_BN_CLICKED(IDC_BTN_SCREEN_SHARE, &CVideoDlg::OnBnClickedBtnScreenShare)
    ON_CBN_SELCHANGE(IDC_CBX_USER_ROLE, &CVideoDlg::OnSelComChange)
    
    ON_MESSAGE(WM_LEAVE_ROOM, &CVideoDlg::OnLeaveRoom)
    ON_MESSAGE(WM_DEVICE_STATE_CHANGE, &CVideoDlg::OnDeviceStateChanged)
    ON_MESSAGE(WM_USERJOIN, &CVideoDlg::OnUserJoined)
    ON_MESSAGE(WM_USEROFFLINE, &CVideoDlg::OnUserOffline)
    ON_MESSAGE(WM_DEFAULTWATCH, &CVideoDlg::OnDefaultWatch)
    ON_MESSAGE(WM_USER_ROLE_CHANGED, &CVideoDlg::OnUserRoleChanged)
    ON_MESSAGE(WM_CHANGE_DEVICE_TYPE, &CVideoDlg::OnDeviceSetChangedType)
    ON_MESSAGE(WM_DEVICE_VOLUME_CHANGE, &CVideoDlg::OnDeviceVolumeChanged)
    ON_MESSAGE(WM_SHOW_NOTICE, &CVideoDlg::OnShowNotice)
    ON_MESSAGE(WM_VIDEO_INFO_CHANGED, &CVideoDlg::OnVideoInfoChanged)
    ON_MESSAGE(WM_JOINROOM_FAIL, &CVideoDlg::OnJoinFailure)
    ON_MESSAGE(WM_USER_ROLE_CHANGE_FAIL, &CVideoDlg::OnRoleChangeFailure)
    ON_MESSAGE(WM_AUDIO_INFO_CHANGED, &CVideoDlg::OnAudioInfoChanged)
    ON_MESSAGE(WM_SCREEN_SHARED_STARTED, &CVideoDlg::OnScreenSharedStarted)
    ON_MESSAGE(WM_SCREEN_SHARED_STOPED, &CVideoDlg::OnScreenSharedStoped)
    ON_MESSAGE(WM_SCREEN_SHARED_AVAILABLE, &CVideoDlg::OnScreenSharedAvailable)
    ON_MESSAGE(WM_SCREEN_SHARED_INFO_CHANGED, &CVideoDlg::OnScreenSharedInfoChanged)
    ON_MESSAGE(WM_JOINROOM_SUCCESS, &CVideoDlg::OnJoinRoomSuccess)
    ON_MESSAGE(WM_USER_ROOM_ROLE_CHANGE_FAIL, &CVideoDlg::OnRoomRoleChangeFailure)
END_MESSAGE_MAP()

void CVideoDlg::OnSize(UINT nType, int cx, int cy) {
    CDialogEx::OnSize(nType, cx, cy);

    if (m_btnMin.GetSafeHwnd() != NULL) {
        m_btnMin.MoveWindow(cx - 72, 0, 24, 24, TRUE);
    }
    if (m_btnRst.GetSafeHwnd() != NULL) {
        m_btnRst.MoveWindow(cx - 48, 0, 24, 24, TRUE);
    }
    if (m_btnClose.GetSafeHwnd() != NULL) {
        m_btnClose.MoveWindow(cx - 24, 0, 24, 24, TRUE);
    }

    AdjustSizeForLayout(cx, cy);
    ShowButtonsNormal();
    Invalidate(TRUE);
}

void CVideoDlg::ShowButtonsNormal() {
    CRect	rcClient;
    int nShowMode = SW_HIDE;

    if (!m_bFullScreen) {
        nShowMode = SW_SHOW;
    }

    m_btnUserList.ShowWindow(nShowMode);
    m_btnCamera.ShowWindow(nShowMode);
    if (IsTeacherUserId(m_ownUserId)) {
        m_btnScreenShare.ShowWindow(nShowMode);
    } else {
        m_btnScreenShare.ShowWindow(SW_HIDE);
    }
    m_btnMore.ModifyStyle(0, BS_BITMAP);
    m_btnMore.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDX_BTN_SETTING_VIDEO)));
    m_btnMore.ShowWindow(nShowMode);
    m_btnEndCall.ModifyStyle(0, BS_BITMAP);
    m_btnEndCall.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDX_BTN_HANGUP_VIDEO)));
    m_btnEndCall.ShowWindow(nShowMode);
    const ClassRoleInfo* classRole = RTC_ENGINEMGR()->GetClassRoleInfo();
    if (classRole->role == CLASS_ROLE_TEACHER) {
        m_cbxUserRole.ShowWindow(SW_HIDE);
    } else {
        m_cbxUserRole.ShowWindow(SW_SHOW);
    }

    GetClientRect(&rcClient);
    AdjustButtonsDistance(rcClient.Width(), rcClient.Height());
}


void CVideoDlg::AdjustButtonsDistance(int cx, int cy) {
    UINT nMargin = 60; //distance of buttons
    if (m_cbxUserRole.GetSafeHwnd() != NULL) {
        m_cbxUserRole.MoveWindow(cx / 2 - 5 * nMargin / 2 - 250, 48, 80, 48, TRUE);
    }
    if (m_btnUserList.GetSafeHwnd() != NULL) {
        m_btnUserList.MoveWindow(cx / 2 - 5 * nMargin / 2 - 144, 36, 48, 48, TRUE);
    }
    if (m_btnMore.GetSafeHwnd() != NULL) {
        m_btnMore.MoveWindow(cx / 2 - 3 * nMargin / 2 - 96, 36, 48, 48, TRUE);
    }
    if (m_btnEndCall.GetSafeHwnd() != NULL) {
        m_btnEndCall.MoveWindow(cx / 2 - nMargin / 2 - 48, 36, 48, 48, TRUE);
    }
    if (m_btnMicrophone.GetSafeHwnd() != NULL) {
        m_btnMicrophone.MoveWindow(cx / 2 + nMargin / 2, 36, 48, 48, TRUE);
    }
    if (m_btnPlayback.GetSafeHwnd() != NULL) {
        m_btnPlayback.MoveWindow(cx / 2 + 3 * nMargin / 2 + 48, 36, 48, 48, TRUE);
    }
    if (m_btnCamera.GetSafeHwnd() != NULL) {
        m_btnCamera.MoveWindow(cx / 2 + 5 * nMargin / 2 + 96, 36, 48, 48, TRUE);
    }
    if (m_btnScreenShare.GetSafeHwnd() != NULL) {
        m_btnScreenShare.MoveWindow(cx / 2 + 7 * nMargin / 2 + 144, 36, 48, 48, TRUE);
    }

}

void CVideoDlg::AdjustSizeForLayout(int cx, int cy) {
    GetClientRect(&m_rcShareArea);
    if ((m_rcShareArea.right - m_rcShareArea.left < 1000)
        && (m_rcShareArea.bottom - m_rcShareArea.top < 600)) {
        return;
    }
    int studentStart = m_rcShareArea.left + HW_RTC_VIDEO_STUDENT_WEIGHT_MARGIN;
    int studentDelay = (m_rcShareArea.right - m_rcShareArea.left - HW_RTC_VIDEO_STUDENT_WEIGHT_MARGIN * 2 - HW_RTC_VIDEO_STUDENT_WEIGHT*HW_RTC_MAX_SMALL_VIDEO) / (HW_RTC_MAX_SMALL_VIDEO - 1);

    m_rcTeacherArea.CopyRect(&m_rcShareArea);

    m_rcShareArea.top += HW_RTC_VIDEO_TOP_HIGHT + HW_RTC_VIDEO_TITLE_HIGHT;
    m_rcShareArea.bottom -= HW_RTC_VIDEO_BOTTOM_HIGHT;
    m_rcShareArea.right -= (HW_RTC_VIDEO_TEACHER_WEIGHT + 10);
    m_rcShareArea.left += HW_RTC_VIDEO_TOOL_WEIGHT + 10;

    m_rcTeacherArea.top += HW_RTC_VIDEO_TOP_HIGHT + HW_RTC_VIDEO_TITLE_HIGHT;
    m_rcTeacherArea.bottom = m_rcTeacherArea.top + HW_RTC_VIDEO_TEACHER_HIGHT;
    m_rcTeacherArea.right -= 5;
    m_rcTeacherArea.left = m_rcTeacherArea.right - HW_RTC_VIDEO_TEACHER_WEIGHT;
    m_wndShareVideo.MoveWindow(&m_rcShareArea, TRUE);
    m_wndTeacherVideo.MoveWindow(&m_rcTeacherArea, TRUE);
    m_wndTeacherInfo.MoveWindow(m_rcTeacherArea.left, m_rcTeacherArea.bottom, HW_RTC_VIDEO_TEACHER_WEIGHT, 24, TRUE);

    for (int nIndex = 0; nIndex < MAX_WATCH_NUM; nIndex++) {
        int nXPos = studentStart + (HW_RTC_VIDEO_STUDENT_WEIGHT + studentDelay) * nIndex;
        int nYPos = m_rcShareArea.bottom + 5;
        m_wndStudentVideo[nIndex].MoveWindow(nXPos, nYPos, HW_RTC_VIDEO_STUDENT_WEIGHT, HW_RTC_VIDEO_STUDENT_HEIGHT, TRUE);
        m_wndStudentInfo[nIndex].MoveWindow(nXPos, nYPos + HW_RTC_VIDEO_STUDENT_HEIGHT, HW_RTC_VIDEO_STUDENT_WEIGHT, 22, TRUE);
    }

}

void CVideoDlg::OnMouseMove(UINT nFlags, CPoint point) {
    CDialogEx::OnMouseMove(nFlags, point);
}

void CVideoDlg::OnPaint() {
    CPaintDC dc(this);
    DrawHead(&dc);

    CBrush brush(RGB(58, 58, 58));
    dc.FillRect(&m_rcShareArea, &brush);
    CBrush brush2(RGB(58, 58, 58));
    CRect rcClient;
    rcClient.CopyRect(&m_rcTeacherArea);
    rcClient.bottom += 24;
    dc.FillRect(&rcClient, &brush2);
}


LRESULT CVideoDlg::OnNcHitTest(CPoint point) {
    LRESULT lResult = CDialogEx::OnNcHitTest(point);
    if (lResult == HTCLIENT && ::GetAsyncKeyState(MK_LBUTTON) < 0) {
        lResult = HTCAPTION;
    }

    return lResult;
}

void CVideoDlg::OnBnClickedBtnmin() {
    ShowWindow(SW_MINIMIZE);
}


afx_msg LRESULT CVideoDlg::OnLeaveRoom(WPARAM wParam, LPARAM lParam) {
    m_listWndInfo.RemoveAll();

    RTC_ENGINEMGR()->CleanClassRoleInfo();

    CDialogEx::OnOK();
    m_dlgUserList.ShowWindow(SW_HIDE);
    m_dlgDevice.ShowWindow(SW_HIDE);
    m_dlgMediaInfo.ShowWindow(SW_HIDE);
    GetParent()->ShowWindow(SW_SHOW);
    return 0;
}

afx_msg LRESULT CVideoDlg::OnUserRoleChanged(WPARAM wParam, LPARAM lParam) {
    RoleChangeInfo * roleInfo = (RoleChangeInfo*)wParam;
    const ClassRoleInfo* classRole = RTC_ENGINEMGR()->GetClassRoleInfo();
    RD_LOG("VDLG", "OnUserRoomRoleChanged roomid=%s, oldRole=%d, newRole=%d", roleInfo->roomId.c_str(), roleInfo->oldRole, roleInfo->newRole);
    if (roleInfo->roomId.compare(classRole->roomId) == 0) {//本房间的角色切换
        if (roleInfo->oldRole == HRTC_ROLE_TYPE_JOINER && roleInfo->newRole == HRTC_ROLE_TYPE_PLAYER) {
            RTC_ENGINEMGR()->SetUserRole(roleInfo->roomId, HRTC_ROLE_TYPE_PLAYER);
            RTC_ENGINEMGR()->setupLocalVideo(0);
            m_dlgUserList.DeleteUser(m_ownUserId);
            if (IsTeacherUserId(m_ownUserId)) {
                m_wndTeacherVideo.ShowWindow(SW_HIDE);
            } else {
                m_wndStudentVideo[0].ShowWindow(SW_HIDE);
            }
            SwitchData();
        } else if (roleInfo->oldRole == HRTC_ROLE_TYPE_PLAYER && roleInfo->newRole == HRTC_ROLE_TYPE_JOINER) {
            RTC_ENGINEMGR()->SetUserRole(roleInfo->roomId, HRTC_ROLE_TYPE_JOINER);
            RTC_ENGINEMGR()->setupLocalVideo(GetLocalVideoWnd());
            m_dlgUserList.AddOwn(m_ownUserId);
            SwitchData();
        } else {

        }
        RebindVideoWnd();
    } else {
        if (classRole->role == CLASS_ROLE_STUDENT && classRole->classRoomId.compare(roleInfo->roomId) == 0) {
            if (roleInfo->oldRole == HRTC_ROLE_TYPE_JOINER && roleInfo->newRole == HRTC_ROLE_TYPE_PLAYER) {
                RTC_ENGINEMGR()->SetUserRole(classRole->classRoomId, HRTC_ROLE_TYPE_PLAYER);
            } else if (roleInfo->oldRole == HRTC_ROLE_TYPE_PLAYER && roleInfo->newRole == HRTC_ROLE_TYPE_JOINER) {
                RTC_ENGINEMGR()->SetUserRole(classRole->classRoomId, HRTC_ROLE_TYPE_JOINER);
            } else {

            }
        }
    }
    m_cbxUserRole.EnableWindow(TRUE);
    CCSafeDelete(roleInfo);
    return 0;
}

void CVideoDlg::OnBnClickedBtnclose() {
    if (IsZoomed()) {
        ShowWindow(SW_RESTORE);
        m_btnRst.ModifyStyle(0, BS_BITMAP);
        m_btnRst.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDX_BTN_MAX)));

    }
    m_dlgDevice.ShowWindow(SW_HIDE);
    m_dlgMediaInfo.ClearData();
    m_dlgMediaInfo.ShowWindow(SW_HIDE);
    m_dlgUserList.ClearData();
    m_dlgUserList.ShowWindow(SW_HIDE);
    RTC_ENGINEMGR()->leaveRoom();
    memset(&m_shareWndInfo, 0, sizeof(m_shareWndInfo));
    memset(&m_teacherWndInfo, 0, sizeof(m_teacherWndInfo));
    OnLeaveRoom(NULL, NULL);
}

void CVideoDlg::OnBnClickedBtnrest() {
    if (IsZoomed()) {
        m_btnRst.ModifyStyle(0, BS_BITMAP);
        m_btnRst.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDX_BTN_MAX)));
        ShowWindow(SW_RESTORE);
    } else {
        m_btnRst.ModifyStyle(0, BS_BITMAP);
        m_btnRst.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDX_BTN_ORIGIN_SIZE)));
        ShowWindow(SW_MAXIMIZE);
    }

    CRect	rcClient;

    GetClientRect(&rcClient);

    int cx = rcClient.Width();
    int cy = rcClient.Height();

    AdjustSizeForLayout(cx, cy);
    AdjustButtonsDistance(cx, cy);

    Invalidate(TRUE);
}

void CVideoDlg::ShowControlButton(BOOL bShow) {
    int nShowMode = 0;
    if (bShow) {
        nShowMode = SW_SHOW;
    } else {
        nShowMode = SW_HIDE;
    }
    m_btnUserList.ShowWindow(nShowMode);
    m_btnCamera.ShowWindow(nShowMode);
    m_btnMore.ShowWindow(nShowMode);
    m_btnScreenShare.ShowWindow(nShowMode);
    m_btnEndCall.ShowWindow(nShowMode);
}

void CVideoDlg::OnBnClickedMore() {
    CRect			rcButton;
    CMenu			Menu;

    Menu.LoadMenu(IDR_MENU_MORE);
    CMenu *pMenuPopup = Menu.GetSubMenu(0);
    SetMenu(pMenuPopup);
    m_btnMore.GetWindowRect(&rcButton);
    pMenuPopup->TrackPopupMenu(TPM_RIGHTBUTTON, rcButton.left, rcButton.top + 50, this);
    pMenuPopup->DestroyMenu();
}

void CVideoDlg::OnBnClickedBtnMicrophone() {
    IHRTCEngine* engine = CRtcEngineMgr::GetInstance()->GetEngine();
    m_bRecordingClosed = !m_bRecordingClosed;

    if (m_bRecordingClosed) {
        engine->enableLocalAudioStream(false);
        m_btnMicrophone.ModifyStyle(0, BS_BITMAP);
        m_btnMicrophone.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDX_BTN_MICROPHONE_OFF)));
    } else {
        engine->enableLocalAudioStream(true);
        m_btnMicrophone.ModifyStyle(0, BS_BITMAP);
        m_btnMicrophone.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDX_BTN_MICROPHONE_ON)));
    }
}

void CVideoDlg::OnBnClickedBtnPlayback() {
    IHRTCEngine* engine = CRtcEngineMgr::GetInstance()->GetEngine();
    m_bPlaybackClosed = !m_bPlaybackClosed;

    if (m_bPlaybackClosed) {
        engine->adjustPlaybackVolume(0);
        m_btnPlayback.ModifyStyle(0, BS_BITMAP);
        m_btnPlayback.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDX_BTN_SPEAKER_OFF)));
    } else {
        engine->adjustPlaybackVolume(m_dlgDevice.GetSoftOutVolume());
        m_btnPlayback.ModifyStyle(0, BS_BITMAP);
        m_btnPlayback.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDX_BTN_SPEAKER_ON)));
    }

}

void CVideoDlg::OnBnClickedBtnCamera() {
    IHRTCEngine* engine = CRtcEngineMgr::GetInstance()->GetEngine();
    int result = engine->enableLocalVideo(m_bVideoClosed);
    if (result == HW_OK) {
        m_bVideoClosed = !m_bVideoClosed;
    }
    if (m_bVideoClosed) {
        m_btnCamera.ModifyStyle(0, BS_BITMAP);
        m_btnCamera.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDX_BTN_CAMERA_OFF)));
    } else {
        m_btnCamera.ModifyStyle(0, BS_BITMAP);
        m_btnCamera.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDX_BTN_CAMERA_ON)));

    }
}

void CVideoDlg::OnBnClickedBtnScreenShare() {
    m_btnScreenShare.EnableWindow(0);
    IHRTCEngine* engine = CRtcEngineMgr::GetInstance()->GetEngine();
    if (!m_bScreenShared) {
        HRTCSrceenCaptureOptionalInfo optionInfo;
        memset(&optionInfo, 0, sizeof(HRTCSrceenCaptureOptionalInfo));
        IHRTCScreenShareSourceList* sourceList = engine->getScreenShareSources(HRTC_SCREENSHARE_SMALL_ICON);
        unsigned int count = sourceList->count();
        for (unsigned int index = 0; index < count; index++) {
            HRTCScreenShareSourceInfo sourceInfo = sourceList->get(index);
            if (sourceInfo.type == HRTC_SCREEN_SHARE) {
                engine->setScreenShareTarget(&sourceInfo, &optionInfo);
                engine->startScreenShare();
                engine->setAuxiliaryVideoEncodeSmooth(true);
                break;
            }
        }
    } else {
        engine->stopScreenShare();
    }

    return;
}


void CVideoDlg::OnBnClickedUserList() {
    m_dlgUserList.ShowWindow(SW_SHOW);
}

LRESULT CVideoDlg::OnJoinFailure(WPARAM wParam, LPARAM lParam) {
    m_listWndInfo.RemoveAll();
    CDialogEx::OnOK();
    GetParent()->ShowWindow(SW_SHOW);
    return 0L;
}

//收到加入成功消息 可以开始跨方
LRESULT CVideoDlg::OnJoinRoomSuccess(WPARAM wParam, LPARAM lParam) {
    const ClassRoleInfo* classRole = RTC_ENGINEMGR()->GetClassRoleInfo();
    return 0L;
}

LRESULT CVideoDlg::OnRoleChangeFailure(WPARAM wParam, LPARAM lParam) {
    HRTCRoleType role = RTC_ENGINEMGR()->GetUserRole(m_roomId);
    if (role == HRTC_ROLE_TYPE_PLAYER) {
        m_cbxUserRole.SetCurSel(1);
    } else if (role == HRTC_ROLE_TYPE_JOINER) {
        m_cbxUserRole.SetCurSel(0);
    } else {
    }
    m_cbxUserRole.EnableWindow(TRUE);
    return 0L;
}

//跨方的角色切换失败
LRESULT CVideoDlg::OnRoomRoleChangeFailure(WPARAM wParam, LPARAM lParam) {
    const ClassRoleInfo*  classInfo = RTC_ENGINEMGR()->GetClassRoleInfo();
    if (classInfo->role == CLASS_ROLE_STUDENT) {
        if (RTC_ENGINEMGR()->GetUserRole(classInfo->classRoomId) == HRTC_ROLE_TYPE_PLAYER) {
            m_cbxUserRole.SetCurSel(1);
        } else if (RTC_ENGINEMGR()->GetUserRole(classInfo->classRoomId) == HRTC_ROLE_TYPE_JOINER) {
            m_cbxUserRole.SetCurSel(0);
        } else {

        }
    }
    return 0L;
}

LRESULT CVideoDlg::OnShowNotice(WPARAM wParam, LPARAM lParam) {
    TCHAR* tmpChar = (TCHAR*)lParam;
    CNoticeDlg dlgAbout;
    dlgAbout.MessageBox(tmpChar);
    dlgAbout.DoModal();
    CCSafeDeleteArray(tmpChar);
    return 0L;
}

LRESULT CVideoDlg::OnUserJoined(WPARAM wParam, LPARAM lParam) {
    UserJoinInfo* lpData = (UserJoinInfo*)wParam;

    bool isExist = m_dlgUserList.IsExist(lpData->userId);
    if (isExist) {
        CCSafeDelete(lpData);
        return 0;
    }

    if (wcsncmp(m_ownUserId, lpData->userId, wcslen(m_ownUserId)) == 0) {
        m_dlgUserList.AddOwn(lpData->userId);
        CCSafeDelete(lpData);
        return 0;
    }

    if (RTC_ENGINEMGR()->GetUserRole(m_roomId) == HRTC_ROLE_TYPE_JOINER) {
        m_dlgUserList.AddUser(lpData->userId);
    }

    if (IsTeacherUserId(lpData->userId)) {
        memset(&m_teacherWndInfo, 0, sizeof(HWVIDEO_WNDINFO));
        _tcscpy_s(m_teacherWndInfo.userId, HRTCConstant::HRTC_MAX_USERID_LEN, lpData->userId);
        m_wndTeacherVideo.SetUserId(lpData->userId);
        m_teacherWndInfo.bLocal = FALSE;
    } else {
        int count = GetWndSize();
        if (count >= g_rtcMaxWatchNum) {
            CCSafeDelete(lpData);
            return 0;
        }

        BOOL bFound = FALSE;
        POSITION pos = m_listWndInfo.GetHeadPosition();
        while (pos != NULL) {
            HWVIDEO_WNDINFO &agvWndInfo = m_listWndInfo.GetNext(pos);
            if (wcsncmp(agvWndInfo.userId, lpData->userId, wcslen(agvWndInfo.userId)) == 0) {
                bFound = TRUE;
                break;
            }
        }

        if (!bFound) {
            HWVIDEO_WNDINFO agvWndInfo;
            memset(&agvWndInfo, 0, sizeof(HWVIDEO_WNDINFO));
            _tcscpy_s(agvWndInfo.userId, HRTCConstant::HRTC_MAX_USERID_LEN, lpData->userId);
            m_listWndInfo.AddTail(agvWndInfo);
        }
    }
    RebindVideoWnd();
    CCSafeDelete(lpData);
    return 0;
}

LRESULT CVideoDlg::OnUserOffline(WPARAM wParam, LPARAM lParam) {
    BOOL needRebind = FALSE;
    UserOfflineInfo* lpData = (UserOfflineInfo*)wParam;
    if (wcsncmp(m_ownUserId, lpData->userId, wcslen(m_ownUserId)) == 0) {
        CCSafeDelete(lpData);
        return 0;
    }
    char *userId = StringUtility::HW_UnicodeToUtf8(lpData->userId);
    CRtcEngineMgr::GetInstance()->GetEngine()->stopRemoteStreamView(userId);
    m_dlgUserList.DeleteUser(lpData->userId);

    if (wcsncmp(m_shareWndInfo.userId, lpData->userId, wcslen(m_shareWndInfo.userId)) == 0) {
        CRtcEngineMgr::GetInstance()->GetEngine()->stopRemoteAuxiliaryStreamView(userId);
        memset(&m_shareWndInfo, 0, sizeof(m_shareWndInfo));
        m_dlgMediaInfo.SetShareVisible(FALSE);
        needRebind = TRUE;
    }

    if (IsTeacherUserId(lpData->userId)) {
        memset(&m_teacherWndInfo, 0, sizeof(m_teacherWndInfo));
        needRebind = TRUE;
    } else {
        POSITION pos = m_listWndInfo.GetHeadPosition();
        while (pos != NULL) {
            if (wcsncmp(m_listWndInfo.GetAt(pos).userId, lpData->userId, wcslen(m_listWndInfo.GetAt(pos).userId)) == 0) {
                m_listWndInfo.RemoveAt(pos);
                needRebind = TRUE;
                break;
            }
            m_listWndInfo.GetNext(pos);
        }
    }
    if (needRebind) {
        RebindVideoWnd();
    }

    if (userId != NULL) {
        free(userId);
    }
    CCSafeDelete(lpData);

    return 0;
}

void CVideoDlg::TurnOff(TCHAR* userid) {
    char *userId = StringUtility::HW_UnicodeToUtf8(userid);
    CRtcEngineMgr::GetInstance()->GetEngine()->stopRemoteStreamView(userId);
    BOOL needRebind = FALSE;


    if (IsTeacherUserId(userid)) {
        memset(&m_teacherWndInfo, 0, sizeof(m_teacherWndInfo));
        needRebind = TRUE;
    }

    POSITION pos = m_listWndInfo.GetHeadPosition();
    while (pos != NULL) {
        if (wcsncmp(m_listWndInfo.GetAt(pos).userId, userid, wcslen(m_listWndInfo.GetAt(pos).userId)) == 0) {
            m_listWndInfo.RemoveAt(pos);
            needRebind = TRUE;
            break;
        }
        m_listWndInfo.GetNext(pos);
    }
    if (userId != NULL) {
        free(userId);
    }
    if (needRebind) {
        RebindVideoWnd();
    }
}

void CVideoDlg::TurnOn(TCHAR* userid) {
    if (IsTeacherUserId(userid)) {
        memset(&m_teacherWndInfo, 0, sizeof(HWVIDEO_WNDINFO));
        _tcscpy_s(m_teacherWndInfo.userId, HRTCConstant::HRTC_MAX_USERID_LEN, userid);
        m_wndTeacherVideo.SetUserId(userid);
        m_teacherWndInfo.bLocal = FALSE;
    } else {
        HWVIDEO_WNDINFO agvWndInfo;
        memset(&agvWndInfo, 0, sizeof(HWVIDEO_WNDINFO));
        _tcscpy_s(agvWndInfo.userId, HRTCConstant::HRTC_MAX_USERID_LEN, userid);
        m_listWndInfo.AddTail(agvWndInfo);
    }
    RebindVideoWnd();
}

LRESULT CVideoDlg::OnDeviceStateChanged(WPARAM wParam, LPARAM lParam) {
    DeviceInfos * info = (DeviceInfos *)wParam;
    m_dlgDevice.RefreshDeviceStateChange(info->deviceId, info->deviceType, info->deviceState);
    CCSafeDelete(info);
    return 0;
}

LRESULT CVideoDlg::OnDeviceVolumeChanged(WPARAM wParam, LPARAM lParam) {
    DeviceInfos * info = (DeviceInfos *)wParam;
    m_dlgDevice.RefreshDeviceVolumeChange(info->deviceType, info->muted, info->volume);
    CCSafeDelete(info);
    return 0;
}

LRESULT CVideoDlg::OnDefaultWatch(WPARAM wParam, LPARAM lParam) {
    DefaultWatchInfo * lpData = (DefaultWatchInfo *)wParam;

    if (wcsncmp(m_ownUserId, lpData->userId, wcslen(m_ownUserId)) == 0) {
        CCSafeDelete(lpData);
        return 0;
    }

    POSITION pos = m_listWndInfo.GetHeadPosition();
    while (pos != NULL) {
        HWVIDEO_WNDINFO &agvWndInfo = m_listWndInfo.GetNext(pos);
        if (wcsncmp(agvWndInfo.userId, lpData->userId, wcslen(agvWndInfo.userId)) == 0) {
            return 0;
        }
    }
    char *userId = StringUtility::HW_UnicodeToUtf8(lpData->userId);
    if (IsTeacherUserId(lpData->userId)) {
        if (CRtcEngineMgr::GetInstance()->GetEngine()->startRemoteStreamView(userId, m_wndTeacherVideo.GetSafeHwnd(), lpData->streamType, false) == 0) {
            CRtcEngineMgr::GetInstance()->GetEngine()->updateRemoteRenderMode(userId, HRTC_VIDEO_DISPLAY_MODE_FIT, HRTC_VIDEO_MIRROR_TYPE_DISABLE);
            m_wndTeacherVideo.SetUserId(lpData->userId);
        }
    } else {
        HWVIDEO_WNDINFO agvWndInfo;
        memset(&agvWndInfo, 0, sizeof(HWVIDEO_WNDINFO));
        _tcscpy_s(agvWndInfo.userId, HRTCConstant::HRTC_MAX_USERID_LEN, lpData->userId);
        m_listWndInfo.AddTail(agvWndInfo);
        CHWVideoWnd *wndTmp = IsTeacherUserId(m_ownUserId) ? &m_wndStudentVideo[0] : &m_wndStudentVideo[1]; //来的人是学生，如果自己是老师，放到0； 是学生放到1
        wndTmp->SetUserId(agvWndInfo.userId);
        if (CRtcEngineMgr::GetInstance()->GetEngine()->startRemoteStreamView(userId, wndTmp->GetSafeHwnd(), lpData->streamType, false) == 0) {
            CRtcEngineMgr::GetInstance()->GetEngine()->updateRemoteRenderMode(userId, HRTC_VIDEO_DISPLAY_MODE_FIT, HRTC_VIDEO_MIRROR_TYPE_DISABLE);
        }
    }

    if (userId != NULL) {
        free(userId);
    }
    DisplayLayout();
    CCSafeDelete(lpData);
    return 0;
}

LRESULT CVideoDlg::OnDeviceSetChangedType(WPARAM wParam, LPARAM lParam) {
    unsigned int changeType = (unsigned int)wParam;
    RD_LOG("VDLG", "OnDeviceSetChangedType type=%d", changeType);
    IHRTCEngine* engine = CRtcEngineMgr::GetInstance()->GetEngine();
    if (changeType & DEVICE_CHANGE_SOFT_VOLUME) {
        if (!m_bPlaybackClosed) {
            engine->adjustPlaybackVolume(m_dlgDevice.GetSoftOutVolume());
        }
        if (!m_bRecordingClosed) {
            engine->adjustRecordingVolume(m_dlgDevice.GetSoftInVolume());
        }
    }

    if (changeType & DEVICE_CHANGE_CAMERA) {
        m_bVideoClosed = false;
        if (RTC_ENGINEMGR()->GetUserRole(m_roomId) != HRTC_ROLE_TYPE_PLAYER) {
            m_btnCamera.ModifyStyle(0, BS_BITMAP);
            m_btnCamera.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDX_BTN_CAMERA_ON)));
        }
    }
    if (changeType & DEVICE_CHANGE_STREAM_TYPE) {
        RebindVideoWnd();
    }
    return 0;
}

LRESULT CVideoDlg::OnVideoInfoChanged(WPARAM wParam, LPARAM lParam) {
    list<HRTCLocalVideoStats> *localStatsList = (list<HRTCLocalVideoStats> *)wParam;
    list<HRTCRemoteVideoStats> *remoteStatsList = (list<HRTCRemoteVideoStats> *)lParam;
    if (m_dlgMediaInfo.IsWindowVisible()) {
        m_dlgMediaInfo.UpdateVideoInfo(localStatsList, remoteStatsList, TRUE);
    }
    CCSafeDelete(localStatsList);
    CCSafeDelete(remoteStatsList);

    return 0;
}

LRESULT CVideoDlg::OnAudioInfoChanged(WPARAM wParam, LPARAM lParam) {
    list<HRTCLocalAudioStats> *localStatsList = (list<HRTCLocalAudioStats> *)wParam;
    list<HRTCRemoteAudioStats> *remoteStatsList = (list<HRTCRemoteAudioStats> *)lParam;
    if (m_dlgMediaInfo.IsWindowVisible()) {
        m_dlgMediaInfo.UpdateAudioInfo(localStatsList, remoteStatsList, TRUE);
    }
    CCSafeDelete(localStatsList);
    CCSafeDelete(remoteStatsList);
    return 0;
}

LRESULT CVideoDlg::OnScreenSharedInfoChanged(WPARAM wParam, LPARAM lParam) {
    ScreenShareStats *stats = (ScreenShareStats*)wParam;

    if (m_dlgMediaInfo.IsWindowVisible()) {
        m_dlgMediaInfo.UpdateScreenSharedInfo(stats, TRUE);
    }
    CCSafeDelete(stats);

    return 0;
}

// screen share 回调结束后再将btn 设置为可用
LRESULT CVideoDlg::OnScreenSharedStarted(WPARAM wParam, LPARAM lParam) {
    m_bScreenShared = true;
    m_btnScreenShare.EnableWindow(1); 
    m_dlgMediaInfo.SetShareVisible(TRUE);
    return 0;
}

LRESULT CVideoDlg::OnScreenSharedStoped(WPARAM wParam, LPARAM lParam) {
    m_bScreenShared = false;
    m_btnScreenShare.EnableWindow(1);
    m_btnScreenShare.RedrawWindow();
    m_dlgMediaInfo.SetShareVisible(FALSE);
    return 0;
}

LRESULT CVideoDlg::OnScreenSharedAvailable(WPARAM wParam, LPARAM lParam) {
    RD_LOG("VDLG", "OnScreenSharedAvailable");
    ScreenShareInfo* lpData = (ScreenShareInfo*)wParam;

    if (wcsncmp(m_ownUserId, lpData->userId, wcslen(m_ownUserId)) == 0) {
        CCSafeDelete(lpData);
        return 0;
    }

    if (lpData->available) {
        memset(&m_shareWndInfo, 0, sizeof(m_shareWndInfo));
        _tcscpy_s(m_shareWndInfo.userId, HRTCConstant::HRTC_MAX_USERID_LEN, lpData->userId);
        m_shareWndInfo.bShare = TRUE;
        m_shareWndInfo.bWatch = TRUE;
        RebindVideoWnd();
        m_dlgMediaInfo.SetShareVisible(TRUE);
    } else {
        char *userId = StringUtility::HW_UnicodeToUtf8(lpData->userId);
        if (userId) {
            CRtcEngineMgr::GetInstance()->GetEngine()->stopRemoteAuxiliaryStreamView(userId);
            memset(&m_shareWndInfo, 0, sizeof(m_shareWndInfo));
            RebindVideoWnd();
            m_dlgMediaInfo.SetShareVisible(FALSE);
            free(userId);
        }
    }
    CCSafeDelete(lpData);
    return 0;
}

LRESULT CVideoDlg::OnRemoteVideoStat(WPARAM wParam, LPARAM lParam) {
    return 0;
}


void CVideoDlg::DrawHead(CDC *pDC) {
    CRect rcTitle;
    CString cstrRoom;
    cstrRoom = CA2T(m_roomId.c_str());
    CString strTitle = cstrRoom;

    if (m_bFullScreen)
        return;

    GetClientRect(&rcTitle);
    CBrush *lpDefaultBrush = pDC->SelectObject(&m_brHead);

    rcTitle.bottom = 24;
    pDC->FillRect(&rcTitle, &m_brHead);

    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(HRTC_WHITE);
    pDC->DrawText(strTitle, &rcTitle, DT_SINGLELINE | DT_VCENTER | DT_CENTER | DT_NOPREFIX);
    pDC->SelectObject(lpDefaultBrush);
}

void CVideoDlg::InitCtrls() {
    CRect	rcClient;
    int nScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
    int nScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);

    GetClientRect(&rcClient);

    m_btnUserList.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTN_SHOW_USERLIST);
    m_btnUserList.MoveWindow(rcClient.Width() / 2 - 60, 24, 48, 48, TRUE);
    m_btnUserList.ModifyStyle(0, BS_BITMAP);
    m_btnUserList.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDX_BTN_USER_LIST)));

    m_btnMore.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTN_SETTING_MORE);
    m_btnMore.MoveWindow(rcClient.Width() / 2 + 24, 24, 48, 48, TRUE);
    m_btnMore.ModifyStyle(0, BS_BITMAP);
    m_btnMore.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDX_BTN_SETTING_VIDEO)));

    m_btnEndCall.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTN_HANGUP);
    m_btnEndCall.MoveWindow(rcClient.Width() - 120, 24, 48, 48, TRUE);
    m_btnEndCall.ModifyStyle(0, BS_BITMAP);
    m_btnEndCall.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDX_BTN_HANGUP_VIDEO)));

    m_btnMicrophone.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTN_MICROPHONE);
    m_btnMicrophone.ModifyStyle(0, BS_BITMAP);
    m_btnMicrophone.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDX_BTN_MICROPHONE_ON)));

    m_btnPlayback.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTN_SPEAKER);
    m_btnPlayback.ModifyStyle(0, BS_BITMAP);
    m_btnPlayback.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDX_BTN_SPEAKER_ON)));

    m_btnCamera.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTN_CAMERA);
    m_btnCamera.MoveWindow(rcClient.Width() / 2 + 72, 24, 48, 48, TRUE);
    m_btnCamera.ModifyStyle(0, BS_BITMAP);
    m_btnCamera.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDX_BTN_CAMERA_ON)));

    m_cbxUserRole.Create(WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_VSCROLL | WS_BORDER,
        CRect(0, 0, 1, 1), this, IDC_CBX_USER_ROLE);
    m_cbxUserRole.MoveWindow(rcClient.Width() - 24, 50, 96, 48, TRUE);

    m_btnMin.MoveWindow(rcClient.Width() - 72, 1, 22, 22, TRUE);

    m_btnRst.MoveWindow(rcClient.Width() - 48, 1, 22, 22, TRUE);
    m_btnClose.MoveWindow(rcClient.Width() - 24, 1, 22, 22, TRUE);

    m_btnScreenShare.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTN_SCREEN_SHARE);
    m_btnScreenShare.MoveWindow(rcClient.Width() / 2 + 120, 24, 48, 48, TRUE);
    m_btnScreenShare.ModifyStyle(0, BS_BITMAP);
    m_btnScreenShare.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDX_BTN_SCREEN_SHARE)));

    for (int nIndex = 0; nIndex < MAX_WATCH_NUM; nIndex++) {

        m_wndStudentVideo[nIndex].Create(NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | SS_BITMAP, CRect(0, 0, 1, 1), this, IDC_BASEWND_VIDEO + nIndex);
        m_wndStudentVideo[nIndex].SetBackImage(IDX_VIDEO_DEFAULT_IMG, 96, 96, HRTC_LIGHT_BLACK);
        m_wndStudentVideo[nIndex].SetFaceColor(RGB(0x58, 0x58, 0x58));
        m_wndStudentVideo[nIndex].SetUserId(0);
        m_wndStudentInfo[nIndex].Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 1, 1), this, IDC_STATIC);
        m_wndStudentInfo[nIndex].ShowWindow(SW_HIDE);
        m_wndStudentVideo[nIndex].ShowWindow(SW_SHOW);

    }

    m_wndShareVideo.Create(NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | SS_BITMAP, CRect(0, 0, 1, 1), this, IDC_BASEWND_VIDEO + 4);
    m_wndShareVideo.SetBackImage(IDX_VIDEO_DEFAULT_IMG, 96, 96, HRTC_LIGHT_BLACK);
    m_wndShareVideo.ShowWindow(SW_HIDE);
    m_wndShareVideo.SetUserId(0);

    m_wndTeacherVideo.Create(NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | SS_BITMAP, CRect(0, 0, 1, 1), this, IDC_BASEWND_VIDEO + 4);
    m_wndTeacherVideo.SetBackImage(IDX_VIDEO_DEFAULT_IMG, 96, 96, HRTC_LIGHT_BLACK);
    m_wndTeacherVideo.ShowWindow(SW_HIDE);
    m_wndTeacherVideo.SetUserId(0);
    m_wndTeacherInfo.Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 1, 1), this, IDC_STATIC);
    m_wndTeacherInfo.ShowWindow(SW_HIDE);
}

BOOL CVideoDlg::IsTeacherUserId(const TCHAR * userId) {
    if (userId == NULL) {
        return FALSE;
    }
    return (wcsncmp(m_teacherUserId, userId, 8) == 0);
}

HWND CVideoDlg::GetLocalVideoWnd() {
    if (IsTeacherUserId(m_ownUserId)) {
        return m_wndTeacherVideo.GetSafeHwnd();
    }
    return m_wndStudentVideo[0].GetSafeHwnd();
}

void CVideoDlg::DisplayLayout() {
    m_wndTeacherVideo.ShowWindow(SW_HIDE);
    m_wndTeacherInfo.ShowWindow(SW_HIDE);
    m_wndShareVideo.ShowWindow(SW_HIDE);
    for (int nIndex = 0; nIndex < MAX_WATCH_NUM; nIndex++) {
        m_wndStudentVideo[nIndex].ShowWindow(SW_HIDE);
        m_wndStudentVideo[nIndex].SetBigShowFlag(FALSE);
        m_wndStudentInfo[nIndex].ShowWindow(SW_HIDE);
    }
    if (_tcslen(m_wndTeacherVideo.GetUserId()) != 0 ) { //修改
        if (!(m_teacherWndInfo.bLocal && RTC_ENGINEMGR()->GetUserRole(m_roomId) == HRTC_ROLE_TYPE_PLAYER)) {
            m_wndTeacherVideo.ShowWindow(SW_SHOW);
            m_wndTeacherVideo.SetParent(this);
            m_wndTeacherInfo.ShowUserInfo(m_wndTeacherVideo.GetUserNickName());
        }
    }

    if (_tcslen(m_wndShareVideo.GetUserId()) != 0) {
        m_wndShareVideo.ShowWindow(SW_SHOW);
        m_wndShareVideo.SetParent(this);
    }
    int listConut = GetWndSize();
    int nIndex = 0;
    if (listConut > 0 && m_listWndInfo.GetHead().bLocal && RTC_ENGINEMGR()->GetUserRole(m_roomId) == HRTC_ROLE_TYPE_PLAYER) {
        nIndex = 1;
    }
    for (; nIndex < g_rtcMaxWatchNum && nIndex < listConut; nIndex++) {
        if (_tcslen(m_wndStudentVideo[nIndex].GetUserId()) != 0) {
            m_wndStudentVideo[nIndex].ShowWindow(SW_SHOW);
            m_wndStudentVideo[nIndex].SetParent(this);
            m_wndStudentInfo[nIndex].ShowUserInfo(m_wndStudentVideo[nIndex].GetUserNickName());
            m_watchingCount++;
        }
    }

    ShowButtonsNormal();
}

BOOL CVideoDlg::OnInitDialog() {
    CDialogEx::OnInitDialog();
    this->ModifyStyleEx(0, WS_EX_APPWINDOW);
    m_dlgDevice.Create(CDeviceDlg::IDD, this);
    InitCtrls();
    m_dlgMediaInfo.Create(CMediaInfoDlg::IDD, this);
    m_dlgUserList.Create(CUserListDlg::IDD, this);
    m_dlgUserList.CenterWindow();
    m_dlgUserList.SetVideoObject(this);
    return TRUE;  // return TRUE unless you set the focus to a control
}

void CVideoDlg::SetRoomId(const string roomId) {
    m_roomId.assign(roomId);
    _tcscpy_s(m_teacherUserId, HRTCConstant::HRTC_MAX_USERID_LEN, _T("teacher_"));
}

void CVideoDlg::SetOwnUserId(const TCHAR * userId) {
    _tcscpy_s(m_ownUserId, HRTCConstant::HRTC_MAX_USERID_LEN, userId);

    if (IsTeacherUserId(m_ownUserId)) {
        memset(&m_teacherWndInfo, 0, sizeof(HWVIDEO_WNDINFO));
        _tcscpy_s(m_teacherWndInfo.userId, HRTCConstant::HRTC_MAX_USERID_LEN, m_ownUserId);
        m_teacherWndInfo.bWatch = TRUE;
        m_teacherWndInfo.bLocal = TRUE;
    } else {
        HWVIDEO_WNDINFO agvWndInfo;
        memset(&agvWndInfo, 0, sizeof(HWVIDEO_WNDINFO));
        _tcscpy_s(agvWndInfo.userId, HRTCConstant::HRTC_MAX_USERID_LEN, m_ownUserId);
        agvWndInfo.bLocal = TRUE;
        m_listWndInfo.AddHead(agvWndInfo);
    }
}
//显示本地窗口，老师固定住。
//学生移到学生0窗口，记录本地属性
void CVideoDlg::ShowLocalWnd() {
    m_watchingCount = 0;
    if (IsTeacherUserId(m_ownUserId)) {
        m_wndTeacherVideo.SetUserId(m_ownUserId);
    } else {
        m_wndStudentVideo[0].SetUserId(m_ownUserId);
    }

    DisplayLayout();
}

void CVideoDlg::RebindVideoWnd() {
    m_watchingCount = 0;
    m_wndShareVideo.SetUserId(0);
    m_wndTeacherVideo.SetUserId(0);

    bool bRemoteAdaptive = false;

    if (m_shareWndInfo.bWatch && m_shareWndInfo.bShare) {
        char *userId = StringUtility::HW_UnicodeToUtf8(m_shareWndInfo.userId);
        if (userId) {
            if (CRtcEngineMgr::GetInstance()->GetEngine()->startRemoteAuxiliaryStreamView(userId, m_wndShareVideo.GetSafeHwnd()) == 0) {
                CRtcEngineMgr::GetInstance()->GetEngine()->updateRemoteRenderMode(userId, HRTC_VIDEO_DISPLAY_MODE_FIT, HRTC_VIDEO_MIRROR_TYPE_DISABLE);
            }
            free(userId);
        }
        m_wndShareVideo.SetUserId(m_shareWndInfo.userId);
    }
    if (IsTeacherUserId(m_teacherWndInfo.userId)) {
        m_wndTeacherVideo.SetUserId(m_teacherWndInfo.userId);
        if (!m_teacherWndInfo.bLocal) {
            HRTCStreamType streamType = m_dlgDevice.GetBigStreamType();
            char *userId = StringUtility::HW_UnicodeToUtf8(m_teacherWndInfo.userId);
            if (CRtcEngineMgr::GetInstance()->GetEngine()->startRemoteStreamView(userId, m_wndTeacherVideo.GetSafeHwnd(), streamType, bRemoteAdaptive) == 0) {
                CRtcEngineMgr::GetInstance()->GetEngine()->updateRemoteRenderMode(userId, HRTC_VIDEO_DISPLAY_MODE_FIT, HRTC_VIDEO_MIRROR_TYPE_DISABLE);
            }
            free(userId);
        }
    }

    POSITION pos = m_listWndInfo.GetHeadPosition();
    for (int nIndex = 0; nIndex < g_rtcMaxWatchNum; nIndex++) {
        if (pos != NULL) {
            HWVIDEO_WNDINFO &agvWndInfo = m_listWndInfo.GetNext(pos);
            char *userId = StringUtility::HW_UnicodeToUtf8(agvWndInfo.userId);
            HRTCStreamType streamType = m_dlgDevice.GetBigStreamType();

            RD_LOG("VDLG", "RebindVideoWnd id=%d, userId=%s, streamType=%d, local=%d", nIndex, userId, streamType, agvWndInfo.bLocal);
            if (!agvWndInfo.bLocal && CRtcEngineMgr::GetInstance()->GetEngine()->startRemoteStreamView(userId, m_wndStudentVideo[nIndex].GetSafeHwnd(), streamType, bRemoteAdaptive) == 0) {
                CRtcEngineMgr::GetInstance()->GetEngine()->updateRemoteRenderMode(userId, HRTC_VIDEO_DISPLAY_MODE_FIT, HRTC_VIDEO_MIRROR_TYPE_DISABLE);
            }
            if (userId != NULL) {
                free(userId);
            }
            m_wndStudentVideo[nIndex].SetUserId(agvWndInfo.userId);
        } else {
            m_wndStudentVideo[nIndex].SetUserId(0);
            m_wndStudentVideo[nIndex].SetBackImage(IDX_VIDEO_DEFAULT_IMG, 96, 96, HRTC_LIGHT_BLACK);
            m_wndStudentVideo[nIndex].UpdateWindow();
            m_wndStudentVideo[nIndex].Invalidate(TRUE);
        }
    }
    DisplayLayout();
}

BOOL CVideoDlg::PreTranslateMessage(MSG* pMsg) {
    if (pMsg->message == WM_KEYDOWN) {
        switch (pMsg->wParam) {
        case VK_RETURN:
            return FALSE;
        case VK_ESCAPE:

            return FALSE;
        }
    }

    return CDialogEx::PreTranslateMessage(pMsg);
}

void CVideoDlg::ResetWindow() {
    for (int nIndex = 0; nIndex < MAX_WATCH_NUM; nIndex++) {
        m_wndStudentVideo[nIndex].DestroyWindow();
        m_wndStudentVideo[nIndex].Create(NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | SS_BITMAP, CRect(0, 0, 1, 1), this, IDC_BASEWND_VIDEO + nIndex);
        m_wndStudentVideo[nIndex].SetBackImage(IDX_VIDEO_DEFAULT_IMG, 96, 96, HRTC_LIGHT_BLACK);
        m_wndStudentVideo[nIndex].SetFaceColor(RGB(0x58, 0x58, 0x58));
        m_wndStudentVideo[nIndex].SetUserId(0);
    }
    m_wndTeacherVideo.DestroyWindow();
    m_wndTeacherVideo.Create(NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | SS_BITMAP, CRect(0, 0, 1, 1), this, IDC_BASEWND_VIDEO + 4);
    m_wndTeacherVideo.SetBackImage(IDX_VIDEO_DEFAULT_IMG, 96, 96, HRTC_LIGHT_BLACK);
    m_wndTeacherVideo.SetUserId(0);

    m_wndShareVideo.DestroyWindow();
    m_wndShareVideo.Create(NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | SS_BITMAP, CRect(0, 0, 1, 1), this, IDC_BASEWND_VIDEO + 4);
    m_wndShareVideo.SetBackImage(IDX_VIDEO_DEFAULT_IMG, 96, 96, HRTC_LIGHT_BLACK);
    m_wndShareVideo.SetUserId(0);
    m_cbxUserRole.ResetContent();
    const ClassRoleInfo*  classInfo = RTC_ENGINEMGR()->GetClassRoleInfo();
    if (classInfo->role == CLASS_ROLE_STUDENT) {
        m_cbxUserRole.AddString(L"上台");
        m_cbxUserRole.AddString(L"下台");
        m_cbxUserRole.SetCurSel(1);

        if (RTC_ENGINEMGR()->GetUserRole(classInfo->classRoomId.c_str()) == HRTC_ROLE_TYPE_PLAYER) {
            m_cbxUserRole.SetCurSel(1);
        } else if (RTC_ENGINEMGR()->GetUserRole(classInfo->classRoomId.c_str()) == HRTC_ROLE_TYPE_JOINER) {
            m_cbxUserRole.SetCurSel(0);
        } else {
        }
    }

    AdjustSizeForLayout();
}
void CVideoDlg::OnShowWindow(BOOL bShow, UINT nStatus) {
    CDialogEx::OnShowWindow(bShow, nStatus);
    const ClassRoleInfo* classRole = RTC_ENGINEMGR()->GetClassRoleInfo();
    if (classRole->role == CLASS_ROLE_TEACHER) {
        m_cbxUserRole.ShowWindow(SW_HIDE);
    } else {
        m_cbxUserRole.ShowWindow(SW_SHOW);
    }
    if (bShow && GetSafeHwnd() != NULL) {
        RebindVideoWnd();
    }
}


void CVideoDlg::OnMove(int x, int y) {
    CDialogEx::OnMove(x, y);
}


void CVideoDlg::SwitchData() {
    m_btnUserList.EnableWindow(1);
    m_btnPlayback.EnableWindow(1);
    m_btnMicrophone.EnableWindow(1);
    m_btnCamera.EnableWindow(1);
    m_btnScreenShare.EnableWindow(1);
    m_btnMicrophone.ModifyStyle(0, BS_BITMAP);
    m_btnMicrophone.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDX_BTN_MICROPHONE_ON)));
    m_btnPlayback.ModifyStyle(0, BS_BITMAP);
    m_btnPlayback.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDX_BTN_SPEAKER_ON)));

    IHRTCEngine* engine = CRtcEngineMgr::GetInstance()->GetEngine();

    engine->adjustRecordingVolume(m_bRecordingClosed ? 0 : m_dlgDevice.GetSoftInVolume());
    m_btnMicrophone.ModifyStyle(0, BS_BITMAP);
    m_btnMicrophone.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(m_bRecordingClosed ? IDX_BTN_MICROPHONE_OFF : IDX_BTN_MICROPHONE_ON)));

    engine->adjustPlaybackVolume(m_bPlaybackClosed ? 0 : m_dlgDevice.GetSoftOutVolume());
    m_btnPlayback.ModifyStyle(0, BS_BITMAP);
    m_btnPlayback.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(m_bPlaybackClosed ? IDX_BTN_SPEAKER_OFF : IDX_BTN_SPEAKER_ON)));

    m_btnCamera.ModifyStyle(0, BS_BITMAP);
    m_btnCamera.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(m_bVideoClosed ? IDX_BTN_CAMERA_OFF : IDX_BTN_CAMERA_ON)));

    m_btnScreenShare.ModifyStyle(0, BS_BITMAP);
    m_btnScreenShare.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDX_BTN_SCREEN_SHARE)));

    if (RTC_ENGINEMGR()->GetUserRole(m_roomId) == HRTC_ROLE_TYPE_PLAYER) {
        m_btnMicrophone.EnableWindow(0);
        m_btnMicrophone.ModifyStyle(0, BS_BITMAP);
        m_btnMicrophone.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDX_BTN_MICROPHONE_OFF)));
        m_btnCamera.EnableWindow(0);
        m_btnCamera.ModifyStyle(0, BS_BITMAP);
        m_btnCamera.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDX_BTN_CAMERA_OFF)));
        m_btnScreenShare.EnableWindow(0);
    }
}

void CVideoDlg::ResetData() {
    m_listWndInfo.RemoveAll();
    memset(&m_shareWndInfo, 0, sizeof(m_shareWndInfo));
    memset(&m_teacherWndInfo, 0, sizeof(m_teacherWndInfo));
    m_bScreenShared = false;
    m_bVideoClosed = false;
    m_bPlaybackClosed = false;
    m_bRecordingClosed = false;
    SwitchData();
    m_dlgUserList.ClearData();
    m_dlgUserList.ShowWindow(SW_HIDE);
    m_cbxUserRole.ResetContent();
}


void CVideoDlg::OnDestroy() {
    CDialogEx::OnDestroy();

    // TODO: Add your message handler code here
}


void CVideoDlg::OnClose() {
    // TODO: Add your message handler code here and/or call default
    OnBnClickedBtnclose();
    CDialogEx::OnClose();
}

void CVideoDlg::OnSelComChange() {
    const ClassRoleInfo*  classInfo = RTC_ENGINEMGR()->GetClassRoleInfo();
    if (classInfo->role == CLASS_ROLE_STUDENT) {
        IHRTCEngine* engine = CRtcEngineMgr::GetInstance()->GetEngine();

        CString ComSelItem(_T(""));//获取当前下拉框的值 
        m_cbxUserRole.GetLBText(m_cbxUserRole.GetCurSel(), ComSelItem);//获取CComBox下拉框当前选中的值
        if (RTC_ENGINEMGR()->GetUserRole(classInfo->classRoomId) == HRTC_ROLE_TYPE_PLAYER) {
            if (ComSelItem.Compare(L"上台") == 0) {
                m_cbxUserRole.EnableWindow(FALSE); // 防止暴力点击
                RD_LOG("VDLG", "changeUserRole joiner roomid=%s userid=%s", classInfo->classRoomId.c_str(), classInfo->userId.c_str());
                int result = engine->changeUserRole( HRTC_ROLE_TYPE_JOINER, "", 0);
            }
        } else if (RTC_ENGINEMGR()->GetUserRole(classInfo->classRoomId) == HRTC_ROLE_TYPE_JOINER) {
            if (ComSelItem.Compare(L"下台") == 0) {
                m_cbxUserRole.EnableWindow(FALSE); // 防止暴力点击
                RD_LOG("VDLG", "changeUserRole player roomid=%s userid=%s", classInfo->classRoomId.c_str(), classInfo->userId.c_str());
                int result = engine->changeUserRole(HRTC_ROLE_TYPE_PLAYER, "", 0);
            }
        } else {
        }
    }
}


void CVideoDlg::OnIdrSetdevice() {
    m_dlgDevice.ShowWindow(SW_SHOW);
    m_dlgDevice.CenterWindow();
}


void CVideoDlg::OnIdrShowinfo() {
    m_dlgMediaInfo.ShowWindow(SW_SHOW);
    m_dlgMediaInfo.CenterWindow();
}

int CVideoDlg::GetWndSize() {
    return m_listWndInfo.GetCount();
}
