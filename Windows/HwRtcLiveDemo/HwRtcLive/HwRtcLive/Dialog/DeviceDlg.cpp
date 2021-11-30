// DeviceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HwRtcLive.h"
#include "DeviceDlg.h"
#include "afxdialogex.h"
#include "HwRtcEngineMgr.h"
#include "HwRtcMsgDef.h"
#include "StringUtil.h"
#include "IHRTCEngine.h"
// CDeviceDlg 对话框
IMPLEMENT_DYNAMIC(CDeviceDlg, CDialogEx)

CDeviceDlg::CDeviceDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CDeviceDlg::IDD, pParent) {

}

CDeviceDlg::~CDeviceDlg() {
}

void CDeviceDlg::DoDataExchange(CDataExchange* pDX) {
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CLOSE, m_btnClose);
    DDX_Control(pDX, IDC_BTNCANCEL_DEVICE, m_btnCancel);
    DDX_Control(pDX, IDC_BTN_CONFIRM_DEVICE, m_btnConfirm);
    DDX_Control(pDX, IDC_BTN_APPLY_DEVICE, m_btnApply);
    DDX_Control(pDX, IDC_SLDAOUT_DEVICE, m_slideAudioOutputVol);
    DDX_Control(pDX, IDC_SLDAIN_DEVICE, m_slideAudioInputVol);

    DDX_Control(pDX, IDC_BIGTYPE, m_cbxBigStream);
    DDX_Control(pDX, IDC_SLIDAIN_S, m_slideAInSVol);
    DDX_Control(pDX, IDC_SLIDAOUT_S, m_slideAOutSVol);
    DDX_Control(pDX, IDC_COMBO_AUDIO_DEVICE_IN, m_audioInput);
    DDX_Control(pDX, IDC_COMBO_AUDIO_DEVICE_OUT, m_audioOut);
    DDX_Control(pDX, IDC_COMBO_CAMERA_DEVICE_IN, m_camera);
}


BEGIN_MESSAGE_MAP(CDeviceDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_SHOWWINDOW()
    ON_WM_NCHITTEST()
    ON_BN_CLICKED(IDC_BTNCANCEL_DEVICE, &CDeviceDlg::OnBnClickedBtnCancel)
    ON_BN_CLICKED(IDC_BTN_CONFIRM_DEVICE, &CDeviceDlg::OnBnClickedBtnConfirm)
    ON_BN_CLICKED(IDC_BTN_APPLY_DEVICE, &CDeviceDlg::OnBnClickedBtnApply)
    ON_BN_CLICKED(IDC_CLOSE, &CDeviceDlg::OnBnClickedBtnCancel)
END_MESSAGE_MAP()


// CDeviceDlg 消息处理程序
void CDeviceDlg::OnPaint() {
    CPaintDC dc(this);

    DrawClient(&dc);
}

BOOL CDeviceDlg::OnInitDialog() {
    CDialogEx::OnInitDialog();
    CRect ClientRect;
    GetClientRect(&ClientRect);
    m_lpRtcEngine = RTC_ENGINEMGR()->GetEngine();

    m_ftDes.CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    m_ftLink.CreateFont(17, 0, 0, 0, FW_NORMAL, FALSE, TRUE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    m_ftBtn.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    m_penFrame.CreatePen(PS_SOLID, 1, HRTC_GRAY);
    SetBackgroundImage(IDX_DLG_MAIN_BAKCGROUND);
    m_btnClose.MoveWindow(ClientRect.Width() + 22, 1, 22, 22, TRUE);
    InitCtrls();

    return TRUE;  // return TRUE unless you set the focus to a control
}

void CDeviceDlg::InitCtrls() {
    CRect ClientRect;

    MoveWindow(0, 0, 512, 620, 1);
    CenterWindow();

    GetClientRect(&ClientRect);

    m_audioInput.MoveWindow(160, 98, 210, 25, TRUE);
    m_audioOut.MoveWindow(160, 238, 210, 25, TRUE);
    m_camera.MoveWindow(160, 388, 210, 25, TRUE);
    m_slideAudioInputVol.MoveWindow(155, 140, 240, 25, TRUE);
    m_slideAudioOutputVol.MoveWindow(155, 285, 240, 25, TRUE);
    m_slideAInSVol.MoveWindow(155, 165, 240, 25, TRUE);
    m_slideAOutSVol.MoveWindow(155, 310, 240, 25, TRUE);
    m_cbxBigStream.MoveWindow(160, ClientRect.Height() - 180, 65, 30, TRUE);
    m_cbxBigStream.InsertString(HRTC_STREAM_TYPE_LD, L"90p");
    m_cbxBigStream.InsertString(HRTC_STREAM_TYPE_SD, L"180p");
    m_cbxBigStream.InsertString(HRTC_STREAM_TYPE_HD, L"360p");
    m_cbxBigStream.InsertString(HRTC_STREAM_TYPE_FHD, L"720p");
    m_bigStreamType = HRTC_STREAM_TYPE_LD;
    m_cbxBigStream.SetCurSel(m_bigStreamType);

    m_bRemoteStreamAdaptive = TRUE;
    m_btnApply.MoveWindow(66, ClientRect.Height() - 88, 100, 36, TRUE);
    m_btnConfirm.MoveWindow(196, ClientRect.Height() - 88, 100, 36, TRUE);
    m_btnCancel.MoveWindow(326, ClientRect.Height() - 88, 100, 36, TRUE);

    m_audioInput.SetFont(&m_ftDes);
    m_audioOut.SetFont(&m_ftDes);
    m_camera.SetFont(&m_ftDes);
    m_btnCancel.SetFont(&m_ftBtn);
    m_btnCancel.SetWindowText(_T("Cancel"));
    m_btnConfirm.SetFont(&m_ftBtn);
    m_btnConfirm.SetWindowText(_T("Confirm"));
    m_btnApply.SetFont(&m_ftBtn);
    m_btnApply.SetWindowText(_T("Apply"));

    m_slideAudioInputVol.SetRange(0, 100);
    m_slideAudioOutputVol.SetRange(0, 100);
    m_slideAInSVol.SetRange(0, 100);
    m_slideAOutSVol.SetRange(0, 100);

    m_softInVolume = 10;
    m_softOutVolume = 10;
}

void CDeviceDlg::DrawClient(CDC *lpDC) {
    CRect	rcText;
    CRect	rcClient;
    CString lpString = NULL;

    GetClientRect(&rcClient);

    CFont* defFont = lpDC->SelectObject(&m_ftDes);
    lpDC->SetBkColor(HRTC_WHITE);
    lpDC->SetTextColor(HRTC_GRAY);

    lpDC->SelectObject(&m_penFrame);
    rcText.SetRect(rcClient.Width() / 2 - 190, 95, rcClient.Width() / 2 + 130, 125);
    lpDC->RoundRect(&rcText, CPoint(32, 32));
    lpString.LoadString(IDS_STRING_AUDIO_INPUT);
    lpDC->TextOut(80, 102, lpString, _tcslen(lpString));

    rcText.SetRect(rcClient.Width() / 2 - 190, 235, rcClient.Width() / 2 + 130, 265);
    lpDC->RoundRect(&rcText, CPoint(32, 32));
    lpString.LoadString(IDS_STRING_AUDIO_OUTPUT);
    lpDC->TextOut(80, 242, lpString, _tcslen(lpString));

    rcText.SetRect(rcClient.Width() / 2 - 190, 385, rcClient.Width() / 2 + 130, 415);
    lpDC->RoundRect(&rcText, CPoint(32, 32));
    lpString.LoadString(IDS_STRING_CAMERA);
    lpDC->TextOut(80, 392, lpString, _tcslen(lpString));

    lpString.LoadString(IDS_STRING_RECORDING_DEVICE);
    lpDC->SetTextColor(HRTC_BLUE);
    lpDC->TextOut(66, 70, lpString, _tcslen(lpString));
    lpString.LoadString(IDS_STRING_PLAYBACK_DEVICE);
    lpDC->TextOut(66, 210, lpString, _tcslen(lpString));
    lpString.LoadString(IDS_STRING_VIDEO_DEVICE);
    lpDC->TextOut(66, 360, lpString, _tcslen(lpString));
    lpString.LoadString(IDS_STRING_VOLUME);
    lpDC->SetTextColor(RGB(0x00, 0x00, 0x00));
    lpDC->TextOut(66, 145, lpString, _tcslen(lpString));
    lpDC->TextOut(66, 285, lpString, _tcslen(lpString));
    m_btnApply.ShowWindow(TRUE);
    lpString.LoadString(IDS_STRING_SOFT_VOLUME);
    lpDC->SetTextColor(RGB(0x00, 0x00, 0x00));
    lpDC->TextOut(66, 170, lpString, _tcslen(lpString));
    lpDC->TextOut(66, 310, lpString, _tcslen(lpString));
    lpDC->SetTextColor(HRTC_BLUE);
    lpString.LoadString(IDS_STRING_DOWNSTREAM_RESOLUTION);
    lpDC->SetTextColor(HRTC_BLUE);
    lpDC->TextOut(66, rcClient.Height() - 180, lpString, _tcslen(lpString));
    lpDC->SelectObject(defFont);
}

LRESULT CDeviceDlg::OnNcHitTest(CPoint point) {
    LRESULT lResult = CDialogEx::OnNcHitTest(point);
    if (lResult == HTCLIENT && ::GetAsyncKeyState(MK_LBUTTON) < 0) {
        lResult = HTCAPTION;
    }
    return lResult;
}

void CDeviceDlg::OnShowWindow(BOOL bShow, UINT nStatus) {
    CDialogEx::OnShowWindow(bShow, nStatus);
    if (!bShow) {
        return;
    }

    CString strDeviceName;
    CString strDeviceID;
    char strCurID[512] = { 0 };
    IHRTCEngine* engine = CRtcEngineMgr::GetInstance()->GetEngine();
    IHRTCAudioDeviceManager*  audio = engine->getAudioDeviceManager();
    unsigned int count = 10;
    unsigned int playVolume, recordVolume;
    audio->getPlaybackDeviceVolume(&playVolume);
    audio->getRecordDeviceVolume(&recordVolume);
    m_slideAudioOutputVol.SetPos(playVolume);
    m_slideAudioInputVol.SetPos(recordVolume);

    m_slideAInSVol.SetPos(m_softInVolume);
    m_slideAOutSVol.SetPos(m_softOutVolume);

    m_audioOut.ResetContent();
    audio->getCurrentPlaybackDevice(strCurID);
    audio->getPlaybackDevices(m_audioOutInfo, &count);
    for (UINT nIndex = 0; nIndex < count; nIndex++) {
        TCHAR* deviceName = StringUtility::HW_Utf8ToUnicodeW(m_audioOutInfo[nIndex].deviceName);

        m_audioOut.InsertString(nIndex, deviceName);

        if (strncmp(strCurID, m_audioOutInfo[nIndex].deviceId, strlen(m_audioOutInfo[nIndex].deviceId)) == 0) {
            m_audioOut.SetCurSel(nIndex);
        }
        if (deviceName != NULL) {
            free(deviceName);
        }
    }
    count = 10;
    memset(strCurID, 0, 512);
    audio->getCurrentRecordDevice(strCurID);
    audio->getRecordDevices(m_audioInInfo, &count);
    m_audioInput.ResetContent();

    for (UINT nIndex = 0; nIndex < count; nIndex++) {
        TCHAR* deviceName = StringUtility::HW_Utf8ToUnicodeW(m_audioInInfo[nIndex].deviceName);
        m_audioInput.InsertString(nIndex, deviceName);

        if (strncmp(strCurID, m_audioInInfo[nIndex].deviceId, strlen(m_audioInInfo[nIndex].deviceId)) == 0) {
            m_audioInput.SetCurSel(nIndex);
        }
        if (deviceName != NULL) {
            free(deviceName);
        }
    }

    count = 10;
    memset(strCurID, 0, 512);
    IHRTCVideoDeviceManager*  video = engine->getVideoDeviceManager();
    video->getCurrentVideoDevice(strCurID);
    m_camera.ResetContent();
    video->getVideoDevices(m_videoInfo, &count);
    for (UINT nIndex = 0; nIndex < count; nIndex++) {
        TCHAR* deviceName = StringUtility::HW_Utf8ToUnicodeW(m_videoInfo[nIndex].deviceName);
        m_camera.InsertString(nIndex, deviceName);

        if (strncmp(strCurID, m_videoInfo[nIndex].deviceId, strlen(m_videoInfo[nIndex].deviceId)) == 0) {
            m_camera.SetCurSel(nIndex);
        }
        if (deviceName != NULL) {
            free(deviceName);
        }
    }

    m_cbxBigStream.SetCurSel(m_bigStreamType);
}

BOOL CDeviceDlg::PreTranslateMessage(MSG* pMsg) {
    if (pMsg->message == WM_KEYDOWN) {
        switch (pMsg->wParam) {
        case VK_ESCAPE: {
            OnBnClickedBtnCancel();
            return FALSE;
        }
        case VK_RETURN: {
            OnBnClickedBtnConfirm();
            return FALSE;
        }
        }
    }

    return CDialogEx::PreTranslateMessage(pMsg);
}

void CDeviceDlg::OnBnClickedBtnCancel() {
    CDialogEx::OnCancel();
}

void CDeviceDlg::OnBnClickedBtnConfirm() {
    OnBnClickedBtnApply();
    CDialogEx::OnOK();
}

void CDeviceDlg::OnBnClickedBtnApply() {
    unsigned int changeType = 0;
    int		nCurSel = 0;
    CString strDeviceName;
    CString strDeviceID;
    IHRTCEngine* engine = CRtcEngineMgr::GetInstance()->GetEngine();
    IHRTCAudioDeviceManager*  audio = engine->getAudioDeviceManager();
    int position1 = m_slideAudioOutputVol.GetPos();
    int position2 = m_slideAudioInputVol.GetPos();
    audio->setCurrentPlaybackDeviceVolume(position1);
    audio->setRecordDeviceVolume(position2);
    if (m_softInVolume != m_slideAInSVol.GetPos() || m_softOutVolume != m_slideAOutSVol.GetPos()) {
        m_softInVolume = m_slideAInSVol.GetPos();
        m_softOutVolume = m_slideAOutSVol.GetPos();
        changeType |= DEVICE_CHANGE_SOFT_VOLUME;
    }

    nCurSel = m_audioInput.GetCurSel();
    if (nCurSel != -1) {
        audio->setRecordDevice(m_audioInInfo[nCurSel].deviceId);
    }

    nCurSel = m_audioOut.GetCurSel();
    if (nCurSel != -1) {
        audio->setCurrentPlaybackDevice(m_audioOutInfo[nCurSel].deviceId);
    }

    IHRTCVideoDeviceManager*  video = engine->getVideoDeviceManager();
    nCurSel = m_camera.GetCurSel();
    char strCurID[512] = { 0 };
    video->getCurrentVideoDevice(strCurID);
    if (nCurSel != -1 && strncmp(strCurID, m_videoInfo[nCurSel].deviceId, strlen(m_videoInfo[nCurSel].deviceId)) != 0) {
        changeType |= DEVICE_CHANGE_CAMERA;
        video->setVideoDevice(m_videoInfo[nCurSel].deviceId);
    }
    int bigStype = m_bigStreamType;
    BOOL remoteAdaptive = m_bRemoteStreamAdaptive;

    m_bigStreamType = (HRTCStreamType)m_cbxBigStream.GetCurSel();
    if (m_bigStreamType != bigStype) {
        changeType |= DEVICE_CHANGE_STREAM_TYPE;
    }
    if (changeType > 0) {
        GetParent()->PostMessage(WM_CHANGE_DEVICE_TYPE, changeType, 0);
    }
}

bool CDeviceDlg::isRemoteStreamAdaptive() {
    return m_bRemoteStreamAdaptive == TRUE;
}

HRTCStreamType CDeviceDlg::GetBigStreamType() {
    return m_bigStreamType;
}

void CDeviceDlg::RefreshDeviceVolumeChange(HRTCDeviceType deviceType, unsigned int mute, unsigned int volume) {
    IHRTCEngine* engine = CRtcEngineMgr::GetInstance()->GetEngine();
    IHRTCAudioDeviceManager*  audio = engine->getAudioDeviceManager();
    if (HRTC_DEVTYPE_AUDIO_PLAYBACK == deviceType) {
        m_slideAudioOutputVol.SetPos(volume);
    } else if (HRTC_DEVTYPE_AUDIO_RECORDING == deviceType) {
        m_slideAudioInputVol.SetPos(volume);
    }

}

void CDeviceDlg::RefreshDeviceStateChange(const char* deviceId, HRTCDeviceType deviceType, HRTCDeviceState state) {
    if (state == HRTC_DEVICE_STATE_ACTIVE) {
        IHRTCEngine* engine = CRtcEngineMgr::GetInstance()->GetEngine();
        if (deviceType == HRTC_DEVTYPE_VIDEO_CAPTURE) {
            IHRTCVideoDeviceManager*  video = engine->getVideoDeviceManager();
            char curDeviceId[HRTCConstant::HRTC_MAX_DEVICE_ID_LEN];
            video->getCurrentVideoDevice(curDeviceId);
            if (strlen(curDeviceId) == 0) {
                video->setVideoDevice(deviceId);
            }
        } else if (deviceType == HRTC_DEVTYPE_AUDIO_PLAYBACK) {
            IHRTCAudioDeviceManager*  audio = engine->getAudioDeviceManager();
            unsigned int count = 10;
            audio->getPlaybackDevices(m_audioInInfo, &count);
            if (count == 1) {
                audio->setCurrentPlaybackDevice(deviceId);
            }
        } else if (deviceType == HRTC_DEVTYPE_AUDIO_RECORDING) {
            IHRTCAudioDeviceManager*  audio = engine->getAudioDeviceManager();
            unsigned int count = 10;
            audio->getRecordDevices(m_audioInInfo, &count);
            if (count == 1) {
                audio->setRecordDevice(deviceId);
            }
        } else {

        }
    }
}

