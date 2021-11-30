// MediaInfo.cpp : 实现文件
//
#include "stdafx.h"
#include "HwRtcLive.h"
#include "MediaInfoDlg.h"
#include "afxdialogex.h"
#include "HwRtcEngineMgr.h"
#include "StringUtil.h"
// CMediaInfoDlg 对话框

// MediaInfo 对话框

IMPLEMENT_DYNAMIC(CMediaInfoDlg, CDialogEx)

CMediaInfoDlg::CMediaInfoDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_MEDIA_INFO, pParent) {
}

CMediaInfoDlg::~CMediaInfoDlg() {
}

void CMediaInfoDlg::DoDataExchange(CDataExchange* pDX) {
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_INFO_CLOSE, m_btnClose);
    DDX_Control(pDX, IDC_VIDEOLIST, m_videoList);
    DDX_Control(pDX, IDC_SHAREDLIST, m_sharedList);
    DDX_Control(pDX, IDC_AUDIOLIST, m_audioList);
}

// MediaInfo 消息处理程序
BEGIN_MESSAGE_MAP(CMediaInfoDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_SHOWWINDOW()
    ON_WM_NCHITTEST()
    ON_BN_CLICKED(IDC_INFO_CLOSE, &CMediaInfoDlg::OnBnClickedInfoClose)
END_MESSAGE_MAP()

// CMediaInfoDlg 消息处理程序
void CMediaInfoDlg::OnPaint() {
    UpdateVideoInfo(&m_localVideoStatsList, &m_remoteVideoStatsList, FALSE);
    UpdateAudioInfo(&m_localAudioStatsList, &m_remoteAudioStatsList, FALSE);
    if (m_saveShare) {
        UpdateScreenSharedInfo(&m_shareStats, FALSE);
    }
    CPaintDC dc(this);

    DrawClient(&dc);

}

BOOL CMediaInfoDlg::OnInitDialog() {
    CDialogEx::OnInitDialog();

    InitCtrls();

    return TRUE;  // return TRUE unless you set the focus to a control
}

void CMediaInfoDlg::InitCtrls() {

    MoveWindow(0, 0, 620, 480, 1);
    CenterWindow();

    CRect ClientRect;
    GetClientRect(&ClientRect);
    CString lpString = NULL;
    SetBackgroundImage(IDX_DLG_MAIN_BAKCGROUND);
    m_btnClose.MoveWindow(ClientRect.Width() - 22, 1, 22, 22, TRUE);
    m_videoList.MoveWindow(1, 25, ClientRect.Width() - 2, ClientRect.Height() / 2 - 10, TRUE);
    m_videoList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_INFOTIP);
    lpString.LoadString(IDS_STRING_VIDEO);
    m_videoList.InsertColumn(0, lpString, 0, 100, 0);
    lpString.LoadString(IDS_STRING_RESOLUTION);
    m_videoList.InsertColumn(1, lpString, 0, 100, 1);
    lpString.LoadString(IDS_STRING_BITRATE);
    m_videoList.InsertColumn(2, lpString, 0, 100, 2);
    lpString.LoadString(IDS_STRING_FRAMERATE);
    m_videoList.InsertColumn(3, lpString, 0, 100, 3);
    lpString.LoadString(IDS_STRING_LOSS);
    m_videoList.InsertColumn(4, lpString, 0, 75, 4);
    lpString.LoadString(IDS_STRING_DELAY);
    m_videoList.InsertColumn(5, lpString, 0, 75, 5);
    lpString.LoadString(IDS_STRING_JITTER);
    m_videoList.InsertColumn(6, lpString, 0, 75, 6);

    m_sharedList.MoveWindow(1, ClientRect.Height() / 2 + 19, ClientRect.Width() - 2, 59, TRUE);
    m_sharedList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_INFOTIP);
    lpString.LoadString(IDS_STRING_SHARE);
    m_sharedList.InsertColumn(0, lpString, 0, 100, 0);
    lpString.LoadString(IDS_STRING_RESOLUTION);
    m_sharedList.InsertColumn(1, lpString, 0, 100, 1);
    lpString.LoadString(IDS_STRING_BITRATE);
    m_sharedList.InsertColumn(2, lpString, 0, 90, 2);
    lpString.LoadString(IDS_STRING_FRAMERATE);
    m_sharedList.InsertColumn(3, lpString, 0, 90, 3);
    lpString.LoadString(IDS_STRING_LOSS);
    m_sharedList.InsertColumn(4, lpString, 0, 75, 4);
    lpString.LoadString(IDS_STRING_DELAY);
    m_sharedList.InsertColumn(5, lpString, 0, 75, 5);
    lpString.LoadString(IDS_STRING_JITTER);
    m_sharedList.InsertColumn(6, lpString, 0, 75, 6);

    m_audioList.MoveWindow(1, ClientRect.Height() / 2 + 80, ClientRect.Width() - 2, ClientRect.Height() / 2 - 80, TRUE);
    m_audioList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_INFOTIP);
    lpString.LoadString(IDS_STRING_AUDIO);
    m_audioList.InsertColumn(0, lpString, 0, 150, 0);
    lpString.LoadString(IDS_STRING_BITRATE);
    m_audioList.InsertColumn(1, lpString, 0, 120, 1);
    lpString.LoadString(IDS_STRING_LOSS);
    m_audioList.InsertColumn(2, lpString, 0, 120, 2);
    lpString.LoadString(IDS_STRING_DELAY);
    m_audioList.InsertColumn(3, lpString, 0, 120, 3);
    lpString.LoadString(IDS_STRING_JITTER);
    m_audioList.InsertColumn(4, lpString, 0, 120, 4);

    GetClientRect(&ClientRect);

}

void CMediaInfoDlg::DrawClient(CDC *lpDC) {
    CRect	rcText;
    CRect	rcClient;
    LPCTSTR lpString = NULL;

    lpString = _T("Media Info");
    lpDC->SetTextColor(HRTC_WHITE);
    lpDC->SetBkColor(HRTC_BLUE);
    lpDC->TextOut(2, 3, lpString, _tcslen(lpString));

    GetClientRect(&rcClient);
}

void CMediaInfoDlg::OnShowWindow(BOOL bShow, UINT nStatus) {
    CDialogEx::OnShowWindow(bShow, nStatus);

}

void CMediaInfoDlg::OnBnClickedInfoClose() {
    CDialogEx::OnCancel();
}

LRESULT CMediaInfoDlg::OnNcHitTest(CPoint point) {
    LRESULT lResult = CDialogEx::OnNcHitTest(point);
    if (lResult == HTCLIENT && ::GetAsyncKeyState(MK_LBUTTON) < 0) {
        lResult = HTCAPTION;
    }

    return lResult;
}


void CMediaInfoDlg::UpdateVideoInfo(list<HRTCLocalVideoStats> *localStatsList, list<HRTCRemoteVideoStats> *remoteStatsList, bool needSave) {
    if (needSave) {
        m_localVideoStatsList.clear();
        m_remoteVideoStatsList.clear();
        for (auto status : *localStatsList) {
            m_localVideoStatsList.push_back(status);
        }
        for (auto status : *remoteStatsList) {
            m_remoteVideoStatsList.push_back(status);
        }
    }
    if (!IsWindowVisible()) {
        return;
    }
    int nListCount = m_videoList.GetItemCount();
    CString	strInfo;
    m_videoList.DeleteAllItems();
    int nIndex = 0;
    for (auto status : *localStatsList) {
        m_videoList.InsertItem(nIndex, _T("")); // 插入行
        strInfo.Format(_T("local"));
        m_videoList.SetItemText(nIndex, 0, strInfo);
        int bitrate = status.width < status.height ? status.width : status.height;
        strInfo.Format(_T("%dp"), bitrate);
        m_videoList.SetItemText(nIndex, 1, strInfo);
        strInfo.Format(_T("%d"), status.bitRate);
        m_videoList.SetItemText(nIndex, 2, strInfo);
        strInfo.Format(_T("%d"), status.frameRate);
        m_videoList.SetItemText(nIndex, 3, strInfo);
        strInfo.Format(_T("%d"), status.packetLoss);
        m_videoList.SetItemText(nIndex, 4, strInfo);
        strInfo.Format(_T("%d"), status.delay);
        m_videoList.SetItemText(nIndex, 5, strInfo);
        strInfo.Format(_T("%d"), status.jitter);
        m_videoList.SetItemText(nIndex, 6, strInfo);
        nIndex++;
    }
    for (auto status : *remoteStatsList) {
        m_videoList.InsertItem(nIndex, _T("")); // 插入行
        wchar_t *userId = StringUtility::HW_Utf8ToUnicodeW(status.userId);
        if (HRTC_USER_NAME_MAP.find(status.userId) != HRTC_USER_NAME_MAP.end()) {
            if (userId != NULL) {
                free(userId);
            }
            userId = StringUtility::HW_Utf8ToUnicodeW(HRTC_USER_NAME_MAP[status.userId].c_str());
        }

        if (userId != NULL) {
            strInfo.Format(_T("%s"), userId);
            m_videoList.SetItemText(nIndex, 0, strInfo);
            free(userId);
        }
        int bitrate = status.width < status.height ? status.width : status.height;
        strInfo.Format(_T("%dp"), bitrate);
        m_videoList.SetItemText(nIndex, 1, strInfo);
        strInfo.Format(_T("%d"), status.bitRate);
        m_videoList.SetItemText(nIndex, 2, strInfo);
        strInfo.Format(_T("%d"), status.frameRate);
        m_videoList.SetItemText(nIndex, 3, strInfo);
        strInfo.Format(_T("%d"), status.packetLoss);
        m_videoList.SetItemText(nIndex, 4, strInfo);
        strInfo.Format(_T("%d"), status.delay);
        m_videoList.SetItemText(nIndex, 5, strInfo);
        strInfo.Format(_T("%d"), status.jitter);
        m_videoList.SetItemText(nIndex, 6, strInfo);
        nIndex++;
    }

}

void CMediaInfoDlg::UpdateAudioInfo(list<HRTCLocalAudioStats> *localStatsList, list<HRTCRemoteAudioStats> *remoteStatsList, bool needSave) {
    if (needSave) {
        m_localAudioStatsList.clear();
        m_remoteAudioStatsList.clear();
        for (auto status : *localStatsList) {
            m_localAudioStatsList.push_back(status);
        }
        for (auto status : *remoteStatsList) {
            m_remoteAudioStatsList.push_back(status);
        }
    }
    if (!IsWindowVisible()) {
        return;
    }
    int nListCount = m_audioList.GetItemCount();
    CString	strInfo;
    m_audioList.DeleteAllItems();
    int nIndex = 0;
    for (auto status : *localStatsList) {
        m_audioList.InsertItem(nIndex, _T("")); // 插入行
        strInfo.Format(_T("local"));
        m_audioList.SetItemText(nIndex, 0, strInfo);
        strInfo.Format(_T("%d"), status.bitRate);
        m_audioList.SetItemText(nIndex, 1, strInfo);
        strInfo.Format(_T("%d"), status.packetLoss);
        m_audioList.SetItemText(nIndex, 2, strInfo);
        strInfo.Format(_T("%d"), status.delay);
        m_audioList.SetItemText(nIndex, 3, strInfo);
        strInfo.Format(_T("%d"), status.jitter);
        m_audioList.SetItemText(nIndex, 4, strInfo);
        nIndex++;
    }
    for (auto status : *remoteStatsList) {
        m_audioList.InsertItem(nIndex, _T("")); // 插入行
        if (strlen(status.userId) == 0) {
            strInfo.Format(_T("remote"));
            m_audioList.SetItemText(nIndex, 0, strInfo);
        } else {
            wchar_t *userId = StringUtility::HW_Utf8ToUnicodeW(status.userId);
            if (HRTC_USER_NAME_MAP.find(status.userId) != HRTC_USER_NAME_MAP.end()) {
                if (userId != NULL) {
                    free(userId);
                }
                userId = StringUtility::HW_Utf8ToUnicodeW(HRTC_USER_NAME_MAP[status.userId].c_str());
            }
            if (userId != NULL) {
                strInfo.Format(_T("%s"), userId);
                m_audioList.SetItemText(nIndex, 0, strInfo);
                free(userId);
            }
        }

        strInfo.Format(_T("%d"), status.bitRate);
        m_audioList.SetItemText(nIndex, 1, strInfo);
        strInfo.Format(_T("%d"), status.packetLoss);
        m_audioList.SetItemText(nIndex, 2, strInfo);
        strInfo.Format(_T("%d"), status.delay);
        m_audioList.SetItemText(nIndex, 3, strInfo);
        strInfo.Format(_T("%d"), status.jitter);
        m_audioList.SetItemText(nIndex, 4, strInfo);
        nIndex++;
    }

}

void CMediaInfoDlg::UpdateScreenSharedInfo(ScreenShareStats *stats, bool needSave) {
    if (needSave) {
        m_shareStats.localStats.assign(stats->localStats.begin(), stats->localStats.end());
        m_shareStats.remoteStats.assign(stats->remoteStats.begin(), stats->remoteStats.end());
        m_saveShare = TRUE;
    }
    if (!IsWindowVisible()) {
        return;
    }
    RD_LOG("mediaInfo", "m_isShareVisible=%d", m_isShareVisible);
    if (!m_isShareVisible) {
        m_sharedList.DeleteAllItems();
        return;
    }
    int nListCount = m_videoList.GetItemCount();
    CString	strInfo;
    int nIndex = 0;
    if (stats->localStats.size() > 0) {
        for (auto status : stats->localStats) {
            m_sharedList.DeleteAllItems();
            m_sharedList.InsertItem(nIndex, _T("")); // 插入行
            strInfo.Format(_T("local"));
            m_sharedList.SetItemText(nIndex, 0, strInfo);
            int bitrate = status.width < status.height ? status.width : status.height;
            strInfo.Format(_T("%dp"), bitrate);
            m_sharedList.SetItemText(nIndex, 1, strInfo);
            strInfo.Format(_T("%d"), status.bitRate);
            m_sharedList.SetItemText(nIndex, 2, strInfo);
            strInfo.Format(_T("%d"), status.frameRate);
            m_sharedList.SetItemText(nIndex, 3, strInfo);
            strInfo.Format(_T("%d"), status.packetLoss);
            m_sharedList.SetItemText(nIndex, 4, strInfo);
            strInfo.Format(_T("%d"), status.delay);
            m_sharedList.SetItemText(nIndex, 5, strInfo);
            strInfo.Format(_T("%d"), status.jitter);
            m_sharedList.SetItemText(nIndex, 6, strInfo);
        }
    } else {
        for (auto status : stats->remoteStats) {
            m_sharedList.DeleteAllItems();
            m_sharedList.InsertItem(nIndex, _T("")); // 插入行
            wchar_t *userId = StringUtility::HW_Utf8ToUnicodeW(status.userId);
            if (HRTC_USER_NAME_MAP.find(status.userId) != HRTC_USER_NAME_MAP.end()) {
                if (userId != NULL) {
                    free(userId);
                }
                userId = StringUtility::HW_Utf8ToUnicodeW(HRTC_USER_NAME_MAP[status.userId].c_str());
            }
            if (userId != NULL) {
                strInfo.Format(_T("%s"), userId);
                m_sharedList.SetItemText(nIndex, 0, strInfo);
                if (userId != NULL) {
                    free(userId);
                }
            }
            int bitrate = status.width < status.height ? status.width : status.height;
            strInfo.Format(_T("%dp"), bitrate);
            m_sharedList.SetItemText(nIndex, 1, strInfo);
            strInfo.Format(_T("%d"), status.bitRate);
            m_sharedList.SetItemText(nIndex, 2, strInfo);
            strInfo.Format(_T("%d"), status.frameRate);
            m_sharedList.SetItemText(nIndex, 3, strInfo);
            strInfo.Format(_T("%d"), status.packetLoss);
            m_sharedList.SetItemText(nIndex, 4, strInfo);
            strInfo.Format(_T("%d"), status.delay);
            m_sharedList.SetItemText(nIndex, 5, strInfo);
            strInfo.Format(_T("%d"), status.jitter);
            m_sharedList.SetItemText(nIndex, 6, strInfo);
        }
    }
}

void CMediaInfoDlg::SetShareVisible(bool isVisible) {
    m_isShareVisible = isVisible;
    RD_LOG("CHQ", "SetShareVisible=%d", m_isShareVisible);
    if (!m_isShareVisible) {
        m_sharedList.DeleteAllItems();
    }
}

void CMediaInfoDlg::ClearData() {
    m_localVideoStatsList.clear();
    m_remoteVideoStatsList.clear();
    m_localAudioStatsList.clear();
    m_remoteAudioStatsList.clear();
    m_shareStats.localStats.clear();
    m_shareStats.remoteStats.clear();
    m_sharedList.DeleteAllItems();
    m_audioList.DeleteAllItems();
    m_videoList.DeleteAllItems();
    m_saveShare = FALSE;
    m_isShareVisible = FALSE;
}