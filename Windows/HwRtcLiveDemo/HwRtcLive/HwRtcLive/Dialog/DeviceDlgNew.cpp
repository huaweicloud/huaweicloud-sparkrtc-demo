// DeviceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HwRtcLive.h"
#include "DeviceDlgNew.h"
#include "afxdialogex.h"
#include "HwRtcEngineMgr.h"
#include "HwRtcMsgDef.h"
#include "StringUtil.h"
#include "IHRTCEngine.h"

// CDeviceDlgNew 对话框
IMPLEMENT_DYNAMIC(CDeviceDlgNew, CDialogEx)

struct WindowPosData {
    int wndHnd;
    int x;
    int y;
    int width;
    int height;
};

WindowPosData g_wndPos[] = {
    { IDC_BTN_APPLY_DEVICE,         116, 518, 75,  23 },
    { IDC_SLDAIN_DEVICE,            149, 121,  297, 24 },
    { IDC_SLDAOUT_DEVICE,           149, 249, 297, 24 },
    { IDC_BTNCANCEL_DEVICE,         216, 518, 75,  23 },
    { IDC_BTN_CONFIRM_DEVICE,       317, 518, 75,  23 },
    { IDC_COMBO_INPUT_AUDIO_DEVICE, 149, 54,  297, 21 },
    { IDC_COMBO_OUT_AUDIO_DEVICE,   149, 185, 297, 21 },
    { IDC_COMBO_VIDEO_DEVICE,       125, 323, 318, 21 },
    { IDC_PREVIEW_WINDOW,           152, 362, 204, 128},
    { IDC_STATIC_AUDIO_INPUT,       47,  57,  96,  13 },
    { IDC_STATIC_VOLUME_INPUT,      63,  124,  39,  13 },
    { IDC_STATIC_AUDIO_OUTPUT,      47,  188, 83,  13 },
    { IDC_STATIC_VOLUME_OUT,        63,  252, 39,  13 },
    { IDC_STATIC_VIDEO,             47,  325, 66,  13 }
};


CDeviceDlgNew::CDeviceDlgNew(CWnd* pParent /*=NULL*/)
    : CDialogEx(CDeviceDlgNew::IDD, pParent) {
}

CDeviceDlgNew::~CDeviceDlgNew() {
}

void CDeviceDlgNew::DoDataExchange(CDataExchange* pDX) {
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CLOSE, m_btnClose);
    DDX_Control(pDX, IDC_BTNCANCEL_DEVICE, m_btnCancel);
    DDX_Control(pDX, IDC_BTN_CONFIRM_DEVICE, m_btnConfirm);
    DDX_Control(pDX, IDC_BTN_APPLY_DEVICE, m_btnApply);
    DDX_Control(pDX, IDC_SLDAOUT_DEVICE, m_slideAudioOutputVol);
    DDX_Control(pDX, IDC_SLDAIN_DEVICE, m_slideAudioInputVol);
    DDX_Control(pDX, IDC_COMBO_INPUT_AUDIO_DEVICE, m_audioInput);
    DDX_Control(pDX, IDC_COMBO_OUT_AUDIO_DEVICE, m_audioOut);
    DDX_Control(pDX, IDC_COMBO_VIDEO_DEVICE, m_camera);
    DDX_Control(pDX, IDC_PREVIEW_WINDOW, m_preViewWnd);
}

BEGIN_MESSAGE_MAP(CDeviceDlgNew, CDialogEx)
    ON_WM_SHOWWINDOW()
    ON_WM_NCHITTEST()
    ON_BN_CLICKED(IDC_BTNCANCEL_DEVICE, &CDeviceDlgNew::OnBnClickedBtnCancel)
    ON_BN_CLICKED(IDC_BTN_CONFIRM_DEVICE, &CDeviceDlgNew::OnBnClickedBtnConfirm)
    ON_BN_CLICKED(IDC_BTN_APPLY_DEVICE, &CDeviceDlgNew::OnBnClickedBtnApply)
    ON_BN_CLICKED(IDC_CLOSE, &CDeviceDlgNew::OnBnClickedBtnCancel)
END_MESSAGE_MAP()

BOOL CDeviceDlgNew::OnInitDialog() {
    CDialogEx::OnInitDialog();
    CRect ClientRect;
    GetClientRect(&ClientRect);

    m_ftDes.CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    InitCtrlsFont();
    m_lpRtcEngine = RTC_ENGINEMGR()->GetEngine();

    SetBackgroundImage(IDX_DLG_MAIN_BAKCGROUND);
    m_btnClose.MoveWindow(ClientRect.Width() - 22, 1, 22, 22, TRUE);
    InitCtrls();
    return TRUE;
}

void CDeviceDlgNew::InitCtrlsFont() {
    for (int i = 0; i < 14; i++) {
        WindowPosData data = g_wndPos[i];
        GetDlgItem(data.wndHnd)->SetFont(&m_ftDes);
        GetDlgItem(data.wndHnd)->MoveWindow(data.x, data.y, data.width, data.height, FALSE);
    }
}

void CDeviceDlgNew::InitCtrls() {
    CRect ClientRect;

    MoveWindow(0, 0, 512, 600, 1);
    CenterWindow();

    GetClientRect(&ClientRect);

    m_btnApply.MoveWindow(66, ClientRect.Height() - 68, 100, 36, TRUE);
    m_btnConfirm.MoveWindow(196, ClientRect.Height() - 68, 100, 36, TRUE);
    m_btnCancel.MoveWindow(326, ClientRect.Height() - 68, 100, 36, TRUE);

    m_preViewWnd.SetBackImage(IDX_VIDEO_DEFAULT_IMG, 96, 96, HRTC_LIGHT_BLACK);
    m_preViewWnd.SetFaceColor(RGB(0x58, 0x58, 0x58));

    m_btnCancel.SetFont(&m_ftBtn);
    m_btnCancel.SetWindowText(_T("Cancel"));

    m_btnConfirm.SetFont(&m_ftBtn);
    m_btnConfirm.SetWindowText(_T("Confirm"));

    m_btnApply.SetFont(&m_ftBtn);
    m_btnApply.SetWindowText(_T("Apply"));

    m_slideAudioInputVol.SetRange(0, 100);
    m_slideAudioOutputVol.SetRange(0, 100);

}

LRESULT CDeviceDlgNew::OnNcHitTest(CPoint point) {
    LRESULT lResult = CDialogEx::OnNcHitTest(point);
    if (lResult == HTCLIENT && ::GetAsyncKeyState(MK_LBUTTON) < 0) {
        lResult = HTCAPTION;
    }

    return lResult;
}

void CDeviceDlgNew::OnShowWindow(BOOL bShow, UINT nStatus) {
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

    m_audioOut.ResetContent();
    audio->getCurrentPlaybackDevice(strCurID);
    audio->getPlaybackDevices(m_audioOutInfo, &count);
    for (UINT nIndex = 0; nIndex < count; nIndex++) {
        TCHAR* deviceName = StringUtility::HW_Utf8ToUnicodeW(m_audioOutInfo[nIndex].deviceName);

        m_audioOut.InsertString(nIndex, deviceName);

        if (strncmp(strCurID, m_audioOutInfo[nIndex].deviceId,strlen(m_audioOutInfo[nIndex].deviceId)) == 0) {
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
    engine->stopLocalPreview();
    RTC_ENGINEMGR()->setupLocalVideo(m_preViewWnd);
    engine->startLocalPreview();
}

BOOL CDeviceDlgNew::PreTranslateMessage(MSG* pMsg) {
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

void CDeviceDlgNew::OnBnClickedBtnCancel() {
    RD_LOG("OnBnClickedBtnCancel", "stopLocalPreview");
    RTC_ENGINEMGR()->GetEngine()->stopLocalPreview();

    CDialogEx::OnCancel();
}

void CDeviceDlgNew::OnBnClickedBtnConfirm() {
    OnBnClickedBtnApply();
    RD_LOG("OnBnClickedBtnConfirm", "stopLocalPreview");
    RTC_ENGINEMGR()->GetEngine()->stopLocalPreview();

    CDialogEx::OnOK();
}

void CDeviceDlgNew::OnBnClickedBtnApply() {
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
}

void CDeviceDlgNew::RefreshDeviceVolumeChange(HRTCDeviceType deviceType, unsigned int mute, unsigned int volume) {
    IHRTCEngine* engine = CRtcEngineMgr::GetInstance()->GetEngine();
    IHRTCAudioDeviceManager*  audio = engine->getAudioDeviceManager();
    if (HRTC_DEVTYPE_AUDIO_PLAYBACK == deviceType) {
        m_slideAudioOutputVol.SetPos(volume);
    } else if (HRTC_DEVTYPE_AUDIO_RECORDING == deviceType) {
        m_slideAudioInputVol.SetPos(volume);
    }
}

void CDeviceDlgNew::RefreshDeviceStateChange(char* deviceId, HRTCDeviceType deviceType, HRTCDeviceState state) {

}

