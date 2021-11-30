// HWVideoWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "HwRtcLive.h"
#include "HWVideoWnd.h"
#include "HWRtcMsgDef.h"
#include "StringUtil.h"

IMPLEMENT_DYNAMIC(CHWInfoWnd, CWnd)


CHWInfoWnd::CHWInfoWnd()
    : m_bIsShowUser(FALSE)
{
    m_brushBack.CreateSolidBrush(RGB(0x70, 0x70, 0x70));
}

CHWInfoWnd::~CHWInfoWnd() {
    m_brushBack.DeleteObject();
}

BEGIN_MESSAGE_MAP(CHWInfoWnd, CWnd)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


void CHWInfoWnd::ShowUserInfo(const TCHAR * userId) {
    _tcscpy_s(m_userId, HRTCConstant::HRTC_MAX_USERID_LEN, userId);
    ShowWindow(SW_SHOW);
    m_bIsShowUser = TRUE;
    Invalidate(TRUE);
    UpdateWindow();
}

void CHWInfoWnd::OnPaint() {
    CPaintDC dcPaint(this);
    CRect	rect;
    CString strUser;

    dcPaint.SetBkMode(TRANSPARENT);
    dcPaint.SetTextColor(HRTC_WHITE);

    if (m_bIsShowUser) {
        GetClientRect(&rect);
        rect.top = rect.top + WINDOW_INFO_Y_DELTA;
        strUser.Format(_T("%s"), m_userId);
        dcPaint.DrawText(strUser, &rect, DT_VCENTER | DT_CENTER | DT_NOPREFIX);
    }
}

BOOL CHWInfoWnd::OnEraseBkgnd(CDC* pDC) {
    CRect rcClient;
    GetClientRect(&rcClient);
    pDC->FillRect(&rcClient, &m_brushBack);

    return TRUE;
}

// CHWVideoWnd
IMPLEMENT_DYNAMIC(CHWVideoWnd, CWnd)

CHWVideoWnd::CHWVideoWnd() :
    m_colorBack(RGB(0x58, 0x58, 0x58))
    , m_bIsShowInfo(FALSE)
    , m_bIsBig(FALSE) {

}

CHWVideoWnd::~CHWVideoWnd() {
    m_backImages.DeleteImageList();
}

BEGIN_MESSAGE_MAP(CHWVideoWnd, CWnd)
    ON_WM_ERASEBKGND()
    ON_WM_CREATE()
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_WM_PAINT()
END_MESSAGE_MAP()

// CHWVideoWnd 消息处理程序
BOOL CHWVideoWnd::OnEraseBkgnd(CDC* pDC) {

    return TRUE;
}

void CHWVideoWnd::SetUserId(TCHAR* userId) {
    memset(m_userId, 0, 128);
    if (userId == NULL) {
        return;
    } else {
        _tcscpy_s(m_userId, HRTCConstant::HRTC_MAX_USERID_LEN, userId);
    }
    char *userIdC = StringUtility::HW_UnicodeToUtf8(userId);
    string uid(userIdC);
    if (HRTC_USER_NAME_MAP.find(uid) != HRTC_USER_NAME_MAP.end()) {
        TCHAR * userName = StringUtility::HW_Utf8ToUnicodeW(HRTC_USER_NAME_MAP[uid].c_str());
        if (userName != NULL) {
            _tcscpy_s(m_userName, HRTCConstant::HRTC_MAX_USERID_LEN, userName);
            free(userName);
        }
    } else {
        _tcscpy_s(m_userName, HRTCConstant::HRTC_MAX_USERID_LEN, userId);
    }
    if (userIdC != NULL) {
        free(userIdC);
    }
}

const TCHAR* CHWVideoWnd::GetUserId() {
    return m_userId;
}

const TCHAR* CHWVideoWnd::GetUserNickName() {
    return m_userName;
}

BOOL CHWVideoWnd::SetBackImage(UINT imgId, UINT width, UINT height, COLORREF colorRef) {
    CBitmap backBitmap;

    if (!backBitmap.LoadBitmap(imgId)) {
        return FALSE;
    }

    m_backImages.DeleteImageList();
    m_backImages.Create(width, height, ILC_COLOR24 | ILC_MASK, 1, 1);
    m_backImages.Add(&backBitmap, colorRef);
    backBitmap.DeleteObject();
    Invalidate(TRUE);
    return TRUE;
}

void CHWVideoWnd::SetFaceColor(COLORREF colorBack) {
    m_colorBack = colorBack;

    Invalidate(TRUE);
}

int CHWVideoWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
    if (CWnd::OnCreate(lpCreateStruct) == -1) {
        return -1;
    }
    return 0;
}

void CHWVideoWnd::ShowVideoInfo(BOOL bShow) {
    m_bIsShowInfo = bShow;
    Invalidate(TRUE);

}

void CHWVideoWnd::SetBigShowFlag(BOOL bIsBig) {
    CRect	rcClient;

    m_bIsBig = bIsBig;
    GetClientRect(&rcClient);

    int px = (rcClient.Width() - WINDOW_INFO_WIDTH) / 2;
    int py = rcClient.Height() - WINDOW_INFO_HEIGHT;

    if (m_windUser.GetSafeHwnd() != NULL) {
        if (m_bIsBig) {
            py = py - WINDOW_INFO_Y_DELTA;
        }
        m_windUser.MoveWindow(px, py, WINDOW_INFO_WIDTH, WINDOW_INFO_HEIGHT);
    }
};

void CHWVideoWnd::OnSize(UINT nType, int cx, int cy) {
    CWnd::OnSize(nType, cx, cy);

    int px = (cx - WINDOW_INFO_WIDTH) / 2;
    int py = cy - WINDOW_INFO_HEIGHT;
    if (m_windUser.GetSafeHwnd() != NULL) {
        if (m_bIsBig) {
            py = py - WINDOW_INFO_Y_DELTA;
        }

        m_windUser.MoveWindow(px, py, WINDOW_INFO_WIDTH, WINDOW_INFO_HEIGHT);
    }
}

void CHWVideoWnd::RefreshWnd() {
    SetBitmap(NULL);
}