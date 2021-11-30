#include "StdAfx.h"
#include "RDLog.h"
#include "HwRtcEngineMgr.h"
#include "StringUtil.h"
IHRTCEngine *CRtcEngineMgr::m_pRtcEngine = NULL;
CRtcEngineMgr *CRtcEngineMgr::m_pRtcEngineMgr = NULL;
HWEngineEventHandler CRtcEngineMgr::m_engineEventHandler;
huawei::rtc::HRTCRoleType CRtcEngineMgr::m_roleType = HRTC_ROLE_TYPE_JOINER;
map<string, huawei::rtc::HRTCRoleType> m_roomUserRole;

CRtcEngineMgr::CRtcEngineMgr() {
    m_pRtcEngineMgr = NULL;
    m_pRtcEngine = NULL;
    m_signatrue = L"";
    m_ctime = 0;
}

CRtcEngineMgr::~CRtcEngineMgr() {
    m_pRtcEngineMgr = NULL;
    m_pRtcEngine = NULL;
}

CRtcEngineMgr* CRtcEngineMgr::GetInstance(const char* appId, const char* domain) {
    if (m_pRtcEngineMgr == NULL) {
        m_pRtcEngineMgr = new CRtcEngineMgr();
    }

    if (m_pRtcEngine == NULL) {
        m_pRtcEngine = createHRtcEngine();
    }
    if (m_pRtcEngine == NULL) {
        return NULL;
    }
    if (appId == NULL) {
        return m_pRtcEngineMgr;
    }

    HRTCLogConfig logConfig;
    logConfig.level = HRTC_LOG_LEVEL_DEBUG;
    logConfig.path = RTC_LOG_PATH;
    logConfig.logSize = 10 * 1024;
    logConfig.enable = true;

    HRTCEngineConfig engineConfig;
    engineConfig.appId = appId;
    engineConfig.countryCode = "CN";
    engineConfig.enableHaTrace = true;

    HRTCEngineContext engineContext;
    engineContext.logConfig = logConfig;
    engineContext.engineConfig = engineConfig;
    engineContext.eventHandler = &m_engineEventHandler;
    int ret = m_pRtcEngine->initialize(engineContext);
    if (0 != ret) {
        return NULL;
    }
    return m_pRtcEngineMgr;
}

IHRTCEngine* CRtcEngineMgr::GetEngine() {
    return m_pRtcEngine;
}

void CRtcEngineMgr::FreeInstance() {
    if (m_pRtcEngine != NULL) {
        m_pRtcEngine->release();
        m_pRtcEngine = NULL;
    }
    CCSafeDelete(m_pRtcEngineMgr);
}

void CRtcEngineMgr::SetMsgHandlerWnd(HWND hWnd) {
    m_engineEventHandler.SetMsgReceiver(hWnd);
}

const HWND CRtcEngineMgr::GetMsgHandlerWnd() {
    return m_engineEventHandler.GetMsgReceiver();
}

BOOL CRtcEngineMgr::JoinRoom(const char* userId, const char* userName, const char* roomId, bool bOpen) {
    if (m_pRtcEngine == NULL) {
        return FALSE;
    }

    HRTCUserInfo user;
    memset(&user, 0, sizeof(HRTCUserInfo));
    user.role = m_roleType;
    user.userId = userId;
    user.userName = userName;
    HRTCJoinParam joinRoomParam;
    memset(&joinRoomParam, 0, sizeof(HRTCJoinParam));
    joinRoomParam.userId = userId;
    joinRoomParam.userName = userName;
    joinRoomParam.autoSubscribeAudio = true;
    joinRoomParam.autoSubscribeVideo = true;
    joinRoomParam.authorization = "";
    joinRoomParam.ctime = m_ctime;
    joinRoomParam.roomId = roomId;
    joinRoomParam.userRole = m_roleType;
    joinRoomParam.sfuType = HRTC_SFU_TYPE_PUBLIC_NETWORK;
    joinRoomParam.scenario = HRTC_REMOTE_AUDIO_SUBSCRIBED;
    int ret = m_pRtcEngine->joinRoom(joinRoomParam);
    RD_LOG("JoinRoom", "JoinRoom, ret = %d\n, userId=%s, username = %s, m_roleType=%d", ret, user.userId, user.userName, m_roleType);
    return ret;
}

BOOL CRtcEngineMgr::setupLocalVideo(void * hwnd) {
    if (m_pRtcEngine == NULL) {
        return FALSE;
    }
    int ret = m_pRtcEngine->setupLocalView(hwnd, HRTC_VIDEO_DISPLAY_MODE_FIT);
    RTC_LOG(L"setupLocalVideo, ret = %d\n", ret);
    return TRUE;
}

BOOL CRtcEngineMgr::leaveRoom() {
    if (m_pRtcEngine == NULL) {
        return FALSE;
    }
    int ret = m_pRtcEngine->leaveRoom();
    RTC_LOG(L"leaveRoom, ret = %d\n", ret);
    return TRUE;
}

void CRtcEngineMgr::SetUserRole(string roomid, huawei::rtc::HRTCRoleType roleType) {
    m_roomUserRole[roomid] = roleType;
}

huawei::rtc::HRTCRoleType CRtcEngineMgr::GetUserRole(string roomid) {
    if (m_roomUserRole.find(roomid) == m_roomUserRole.end()) {
        return HRTC_ROLE_TYPE_PLAYER;
    }
    return m_roomUserRole[roomid];
}

void CRtcEngineMgr::SetClassRoleInfo(ClassRoleInfo &classRole) {
    m_classRoleInfo.role = classRole.role;
    m_classRoleInfo.gradeRoomId = classRole.gradeRoomId;
    m_classRoleInfo.classRoomId = classRole.classRoomId;
    m_classRoleInfo.groupRoomId = classRole.groupRoomId;
    m_classRoleInfo.userId = classRole.userId;
    m_classRoleInfo.userName = classRole.userName;
    m_classRoleInfo.roomId = classRole.roomId;
}

void CRtcEngineMgr::SetEnterType(HwRtcEnterRoomType type) {
    m_enterType = type;
}

const ClassRoleInfo* CRtcEngineMgr::GetClassRoleInfo() {
    return &m_classRoleInfo;
}

void CRtcEngineMgr::CleanClassRoleInfo() {
    m_classRoleInfo.role = CLASS_ROLE_IDLE;
    m_classRoleInfo.gradeRoomId.clear();
    m_classRoleInfo.classRoomId.clear();
    m_classRoleInfo.groupRoomId.clear();
    m_classRoleInfo.userId.clear();
    m_classRoleInfo.userName.clear();
    m_classRoleInfo.roomId.clear();
    m_roomUserRole.erase(m_roomUserRole.begin(), m_roomUserRole.end());
}

int CRtcEngineMgr::setVideoEncParam(unsigned int totalBitRate, vector<HRTCVideoEncParam> &encoderParams) {
    if (m_pRtcEngine == NULL) {
        return FALSE;
    }
    HRTCVideoEncParam *encPs = new HRTCVideoEncParam[encoderParams.size()];
    for (unsigned int i = 0; i < encoderParams.size(); i++) {
        memcpy_s(&encPs[i], sizeof(HRTCVideoEncParam), &encoderParams[i], sizeof(HRTCVideoEncParam));
    }
    int ret = m_pRtcEngine->setVideoEncoderConfig(totalBitRate, encPs, encoderParams.size());
    CCSafeDeleteArray(encPs);
    return ret;
}

