#include "StdAfx.h"
#include "HwEngineEventHandler.h"
#include "StringUtil.h"
#include "HwRtcMsgDef.h"
#include "HwRtcEngineMgr.h"
#include "RDLog.h"
#include <iostream>
using namespace std;
#include <wchar.h>

map<string, string> HRTC_USER_NAME_MAP;
map<string, string> HRTC_USER_ROOMID_MAP;

HWEngineEventHandler::HWEngineEventHandler(void) {
    m_hMainWnd = NULL;
}

HWEngineEventHandler::~HWEngineEventHandler(void) {
    m_hMainWnd = NULL;
}

/*
*功能:警告信息处理
*参数:warn:警告码 msg:警告信息
*返回值:无
*/
void HWEngineEventHandler::onWarning(int warn, const char* msg) {
    wchar_t *message = StringUtility::HW_Utf8ToUnicodeW(msg);
    RTC_LOG(L"onWarning,warn =  %d , msg = %s\n", warn, msg);
    if (message != NULL) {
        free(message);
    }
}

/*
*功能:错误信息处理
*参数:warn:错误码 msg: 错误信息
*返回值:无
*/
void HWEngineEventHandler::onError(int error, const char* msg) {
    wchar_t *message = StringUtility::HW_Utf8ToUnicodeW(msg);
    RD_LOG("CALLBACK", "onError,err =  %d , msg = %s\n", error, msg);
    TCHAR *msgTmp = new TCHAR[HRTC_SHORT_MESSAGE_LEN];
    _tcscpy_s(msgTmp, HRTC_SHORT_MESSAGE_LEN, message);
    if (m_hMainWnd != NULL) {
        switch (error) {
            case HRTC_ERR_CODE_KICKED_OFF:
            case HRTC_ERR_CODE_RECONNECT_FAILED:
            case HRTC_ERR_CODE_SERVER_BREAK_DOWN:
            case HRTC_ERR_CODE_SIGNATURE_EXPIRED: {
                HWND appWnd = GetParent(m_hMainWnd);
                if (appWnd != NULL) {
                    ::PostMessage(appWnd, WM_SHOW_NOTICE, WPARAM(TRUE), (LPARAM)msgTmp);
                }
                if (message != NULL) {
                    free(message);
                }
                return;
            }
            case HRTC_ERR_CODE_USER_ROLE_CHANGE_FAIL:
            case HRTC_ERR_CODE_SET_USEROLE_NOT_ALLOWED: {
                ::PostMessage(m_hMainWnd, WM_USER_ROLE_CHANGE_FAIL, 0, 0);
                break;
            }
            case HRTC_ERR_CODE_SCREEN_SHARE_FAIL: {
                ::PostMessage(m_hMainWnd, WM_SCREEN_SHARED_STOPED, 0, 0);
                break;
            }
        }
        ::PostMessage(m_hMainWnd, WM_SHOW_NOTICE, WPARAM(TRUE), (LPARAM)msgTmp);
    }
    if (message != NULL) {
        free(message);
    }
}

/*
*功能:成功加入房间处理
*参数:roomId:加入的房间名 userId: 加入的用户名
*返回值:无
*/
void HWEngineEventHandler::onJoinRoomSuccess(const char* roomId, const char* userId) {
    wchar_t *room = StringUtility::HW_Utf8ToUnicodeW(roomId);
    wchar_t *user = StringUtility::HW_Utf8ToUnicodeW(userId);
    if (m_hMainWnd != NULL) {
        ::PostMessage(m_hMainWnd, WM_JOINROOM_SUCCESS, 0, 0);
        HWND appWnd = GetParent(m_hMainWnd);
        if (appWnd != NULL) {
            TCHAR *relatedID = new TCHAR[HRTC_LONG_MESSAGE_LEN];
            _tcscpy_s(relatedID, HRTC_LONG_MESSAGE_LEN, _T("Join room success!"));
            ::PostMessage(appWnd, WM_SHOW_TOAST, WPARAM(TRUE), (LPARAM)relatedID);
        }
    }
    RTC_LOG(L"onJoinRoomSuccess,roomId =  %s , msg = %s\n", room, user);
    if (room != NULL) {
        free(room);
    }
    if (user != NULL) {
        free(user);
    }
}

/*
*功能:重新成功加入房间处理
*参数:roomId:加入的房间名 userId: 加入的用户名
*返回值:无
*/
void HWEngineEventHandler::onRejoinRoomSuccess(const char* roomId, const char* userId) {
    RD_LOG("CALLBACK", "onReJoinRoomSuccess,roomId =  %s , userId = %s\n", roomId, userId);
    if (m_hMainWnd != NULL) {
        HWND appWnd = GetParent(m_hMainWnd);
        if (appWnd != NULL) {
            TCHAR *relatedID = new TCHAR[200];
            _tcscpy_s(relatedID,200 ,_T("reconnect to server successfully!"));
            ::PostMessage(appWnd, WM_SHOW_TOAST, WPARAM(TRUE), (LPARAM)relatedID);
        }
    }
}

/*
*功能:加入房间失败处理
*参数:roomId:加入的房间名 userId: 加入的用户名
*返回值:无
*/
void HWEngineEventHandler::onJoinRoomFailure(int error, const char* msg) {
    if (msg == nullptr) {
        RD_LOG("onJoinRoomFailure", "error=%d , msg=null\n", error);
        return;
    }
    wchar_t *wmsg = wmsg = StringUtility::HW_Utf8ToUnicodeW(msg);
    RD_LOG("onJoinRoomFailure", "error=%d , msg=%s\n", error, msg);
    if (m_hMainWnd != NULL) {
        HWND appWnd = GetParent(m_hMainWnd);
        if (appWnd != NULL) {
            TCHAR *tMsg = new TCHAR[HRTC_SHORT_MESSAGE_LEN];
            _tcscpy_s(tMsg, HRTC_SHORT_MESSAGE_LEN, wmsg);
            ::PostMessage(appWnd, WM_SHOW_TOAST, WPARAM(TRUE), (LPARAM)tMsg);
        }
        ::PostMessage(m_hMainWnd, WM_JOINROOM_FAIL, NULL, NULL);
    }
    if (wmsg != NULL) {
        free(wmsg);
    }
}

/*
*功能:离开房间处理
*参数:reason:离开房间的原因 statsInfo: 离开房间状态信息
*返回值:无
*/
void HWEngineEventHandler::onLeaveRoom(HRTCLeaveReason reason, const HRTCStatsInfo &statsInfo) {
    if (m_hMainWnd != NULL) {
        ::PostMessage(m_hMainWnd, WM_LEAVE_ROOM, 0, 0);
    }
    HRTC_USER_NAME_MAP.erase(HRTC_USER_NAME_MAP.begin(), HRTC_USER_NAME_MAP.end());
    HRTC_USER_ROOMID_MAP.erase(HRTC_USER_ROOMID_MAP.begin(), HRTC_USER_ROOMID_MAP.end());
    RD_LOG("HWEngineEventHandler", "onLeaveRoom reason %d, statsInfo mildlyFrozenCounts=%lld, severelyFrozenCounts=%lld, totalMildlyFrozenTime=%lld, totalSeverelyFrozenTime=%lld, totalActiveTime=%lld",
        reason, statsInfo.mildlyFrozenCounts, statsInfo.severelyFrozenCounts, statsInfo.totalMildlyFrozenTime, statsInfo.totalSeverelyFrozenTime, statsInfo.totalActiveTime);
}

/*
*功能:用户加入处理
*参数:roomId:加入的房间名 userId: 加入的用户名 nickName:昵称
*返回值:无
*/
void HWEngineEventHandler::onRemoteUserOnline(const char* roomId, const char* userId, const char* userName) {
    string uid(userId);
    string uname(userName);
    HRTC_USER_NAME_MAP[uid] = uname;
    const ClassRoleInfo* classRole = RTC_ENGINEMGR()->GetClassRoleInfo();
    if (HRTC_USER_ROOMID_MAP.find(uid) == HRTC_USER_ROOMID_MAP.end()
        || classRole->roomId.compare(roomId) != 0) {
        HRTC_USER_ROOMID_MAP[uid] = roomId;
    } else {
        RD_LOG("onRemoteUserOnline", "onRemoteUserOnline exit, userId=%s, exitroomid=%s, roomId=%s", userId, HRTC_USER_ROOMID_MAP[uid].c_str(), roomId);
        return;
    }
    wchar_t *room = StringUtility::HW_Utf8ToUnicodeW(roomId);
    wchar_t *user = StringUtility::HW_Utf8ToUnicodeW(userId);
    wchar_t *name = StringUtility::HW_Utf8ToUnicodeW(userName);
    UserJoinInfo* userInfo = new UserJoinInfo();
    _tcscpy_s(userInfo->roomId, HRTCConstant::HRTC_MAX_ROOMID_LEN, room);
    _tcscpy_s(userInfo->userId, HRTCConstant::HRTC_MAX_USERID_LEN, user);
    _tcscpy_s(userInfo->nickname, HRTCConstant::HRTC_MAX_USERNAME_LEN, ((name != nullptr) ? name : _T("")));
    if (m_hMainWnd != NULL) {
        ::PostMessage(m_hMainWnd, WM_USERJOIN, (WPARAM)userInfo, 0);
    }
    RD_LOG("onRemoteUserOnline", "onRemoteUserOnline,roomId =  %s , userId = %s", roomId, userId);
    if (room != NULL) {
        free(room);
    }
    if (user != NULL) {
        free(user);
    }
    if (name != NULL) {
        free(name);
    }
}

/*
*功能:用户离线处理
*参数:roomId:离线的房间名 userId: 离线的用户名 reason:离线原因
*返回值:无
*/
void HWEngineEventHandler::onRemoteUserOffline(const char* roomId, const char* userId, int reason) {
    wchar_t *room = StringUtility::HW_Utf8ToUnicodeW(roomId);
    wchar_t *user = StringUtility::HW_Utf8ToUnicodeW(userId);
    string uid(userId);
    map<string, string>::iterator iter = HRTC_USER_ROOMID_MAP.find(uid);
    if (iter == HRTC_USER_ROOMID_MAP.end()
        || HRTC_USER_ROOMID_MAP[uid].compare(roomId) != 0) {
        RD_LOG("onRemoteUserOffline", "onRemoteUserOffline not exit, userId=%s, exitroomid=%s, roomId=%s", userId, HRTC_USER_ROOMID_MAP[uid].c_str(), roomId);
        if (room != NULL) {
            free(room);
        }
        if (user != NULL) {
            free(user);
        }
        return;
    }
    HRTC_USER_ROOMID_MAP.erase(iter);

    iter = HRTC_USER_NAME_MAP.find(uid);
    if (iter != HRTC_USER_NAME_MAP.end()) {
        HRTC_USER_NAME_MAP.erase(iter);
    }
    UserOfflineInfo* userInfo = new UserOfflineInfo();
    _tcscpy_s(userInfo->roomId, HRTCConstant::HRTC_MAX_ROOMID_LEN, room);
    _tcscpy_s(userInfo->userId, HRTCConstant::HRTC_MAX_USERID_LEN, user);
    userInfo->reason = reason;
    if (m_hMainWnd != NULL) {
        ::PostMessage(m_hMainWnd, WM_USEROFFLINE, (WPARAM)userInfo, 0);
    }
    RD_LOG("onRemoteUserOffline", "onRemoteUserOffline,roomId =  %s , userId = %s, reason = %d\n", roomId, userId, reason);
    if (room != NULL) {
        free(room);
    }
    if (user != NULL) {
        free(user);
    }
}

/*
*功能:网络连接状态处理
*参数:state:网络状态 reason: 网络更改原因 description:描述信息
*返回值:无
*/
void HWEngineEventHandler::onConnectionChangedNotify(HRTCConnStateTypes state, HRTCConnChangeReason reason, const char* description) {
    if (m_hMainWnd == NULL) {
        return;
    }
    wchar_t *desc = StringUtility::HW_Utf8ToUnicodeW(description);
    RTC_LOG(L"onConnectionChangedNotify,connType =  %d , reason = %d, description = %s\n", state, reason, desc);
    if (reason != HRTC_CONN_CHANGED_LEAVE_ROOM && reason != HRTC_CONN_CHANGED_CONNECTING) {
        HWND appWnd = GetParent(m_hMainWnd);
        if (appWnd != NULL) {
            TCHAR *relatedID = new TCHAR[128];
            _tcscpy_s(relatedID, 128, desc);
            ::PostMessage(appWnd, WM_SHOW_TOAST, WPARAM(TRUE), (LPARAM)relatedID);
        }
    }
    if (desc != NULL) {
        free(desc);
    }
}

/*
*功能:设备状态改变处理
*参数:deviceId:设备号 deviceType: 设备类型 deviceState:设备状态
*返回值:无
*/
void HWEngineEventHandler::onDeviceStateChangedNotify(const char* deviceId, HRTCDeviceType deviceType, HRTCDeviceState deviceState) {
    DeviceInfos * info = new DeviceInfos();
    info->deviceType = deviceType;
    info->deviceState = deviceState;
    errno_t err = strcpy_s(info->deviceId, deviceId);
    if (err != 0) {
        RTC_LOG(L"onDeviceStateChangedNotify,device id copy err = %d\n", err);
    }
    if (m_hMainWnd != NULL) {
        ::PostMessage(m_hMainWnd, WM_DEVICE_STATE_CHANGE, (WPARAM)info, 0);
    }
}

/*
*功能:设备音量改变处理
*参数:deviceType: 设备类型 volume:音量大小 muted:订阅状态
*返回值:无
*/
void HWEngineEventHandler::onDeviceVolumeChangedNotify(HRTCDeviceType deviceType, unsigned int volume, unsigned int muted) {
    DeviceInfos * info = new DeviceInfos();
    info->deviceType = deviceType;
    info->muted = muted;
    info->volume = volume;
    if (m_hMainWnd != NULL) {
        ::PostMessage(m_hMainWnd, WM_DEVICE_VOLUME_CHANGE, (WPARAM)info, 0);
    }
}

/*
*功能:角色切换处理
*参数:roomId: 房间号 oldRole:老角色 newRole:新角色
*返回值:无
*/
void HWEngineEventHandler::onUserRoleChangedNotify(const char* roomId, HRTCRoleType oldRole, HRTCRoleType newRole) {
    RoleChangeInfo *roleInfo = new RoleChangeInfo();
    roleInfo->roomId = roomId;
    roleInfo->oldRole = oldRole;
    roleInfo->newRole = newRole;
    if (m_hMainWnd != NULL) {
        ::PostMessage(m_hMainWnd, WM_USER_ROLE_CHANGED, (WPARAM)roleInfo, (LPARAM)0);
    }
}

/*
*功能:视频状态上报处理
*参数:localStats:本地状态 localStatsCount:本地状态数量 remoteStats:远端状态 remoteStatsCount:远端状态数量
*返回值:无
*/
void HWEngineEventHandler::onVideoStatsNotify(HRTCLocalVideoStats *localStats, unsigned int localStatsCount, HRTCRemoteVideoStats *remoteStats, unsigned int remoteStatsCount) {
    list<HRTCLocalVideoStats> *encodeTmp = new list<HRTCLocalVideoStats>();
    list<HRTCRemoteVideoStats> *decodeTmp = new list<HRTCRemoteVideoStats>();
    RD_LOG("EVENT", "onVideoStatsNotify local size = %d, remote size = %d", localStatsCount, remoteStatsCount);
    for (unsigned int i = 0; i < localStatsCount; i++) {
        HRTCLocalVideoStats stats;
        memcpy_s(&stats, sizeof(stats), &localStats[i], sizeof(HRTCLocalVideoStats));
        encodeTmp->push_back(stats);
    }
    for (unsigned int i = 0; i < remoteStatsCount; i++) {
        HRTCRemoteVideoStats stats;
        memcpy_s(&stats, sizeof(stats), &remoteStats[i], sizeof(HRTCRemoteVideoStats));
        decodeTmp->push_back(stats);
    }
    if (m_hMainWnd != NULL) {
        ::PostMessage(m_hMainWnd, WM_VIDEO_INFO_CHANGED, (WPARAM)encodeTmp, (LPARAM)decodeTmp);
    }
}

/*
*功能:音频状态上报处理
*参数:localStats:本地状态 localStatsCount:本地状态数量 remoteStats:远端状态 remoteStatsCount:远端状态数量
*返回值:无
*/
void HWEngineEventHandler::onAudioStatsNotify(HRTCLocalAudioStats *localStats, unsigned int localStatsCount, HRTCRemoteAudioStats *remoteStats, unsigned int remoteStatsCount) {
    list<HRTCLocalAudioStats> *encodeTmp = new list<HRTCLocalAudioStats>();
    list<HRTCRemoteAudioStats> *decodeTmp = new list<HRTCRemoteAudioStats>();
    RD_LOG("EVENT", "onAudioStatsNotify local size = %d, remote size = %d", localStatsCount, remoteStatsCount);
    for (unsigned int i = 0; i < localStatsCount; i++) {
        HRTCLocalAudioStats stats;
        memcpy_s(&stats, sizeof(stats), &localStats[i], sizeof(HRTCLocalAudioStats));
        encodeTmp->push_back(stats);
    }
    for (unsigned int i = 0; i < remoteStatsCount; i++) {
        HRTCRemoteAudioStats stats;
        memcpy_s(&stats, sizeof(stats), &remoteStats[i], sizeof(HRTCRemoteAudioStats));
        decodeTmp->push_back(stats);
    }
    if (m_hMainWnd != NULL) {
        ::PostMessage(m_hMainWnd, WM_AUDIO_INFO_CHANGED, (WPARAM)encodeTmp, (LPARAM)decodeTmp);
    }

}

/*
*功能:辅流状态上报处理
*参数:localStats:本地状态 localStatsCount:本地状态数量 remoteStats:远端状态 remoteStatsCount:远端状态数量
*返回值:无
*/
void HWEngineEventHandler::onAuxiliaryStreamStatsNotify(HRTCLocalVideoStats *localStats, unsigned int localStatsCount, HRTCRemoteVideoStats *remoteStats, unsigned int remoteStatsCount) {
    ScreenShareStats *shareStats = new ScreenShareStats();
    for (unsigned int i = 0; i < localStatsCount; i++) {
        HRTCLocalVideoStats stats;
        memcpy_s(&stats, sizeof(stats), &localStats[i], sizeof(HRTCLocalVideoStats));
        shareStats->localStats.push_back(stats);
    }
    for (unsigned int i = 0; i < remoteStatsCount; i++) {
        HRTCRemoteVideoStats stats;
        memcpy_s(&stats, sizeof(stats), &remoteStats[i], sizeof(HRTCRemoteVideoStats));
        shareStats->remoteStats.push_back(stats);
    }

    if (m_hMainWnd != NULL) {
        ::PostMessage(m_hMainWnd, WM_SCREEN_SHARED_INFO_CHANGED, (WPARAM)shareStats, 0);
    }
}

/*
*功能:开始共享处理
*参数:无
*返回值:无
*/
void HWEngineEventHandler::onScreenShareStarted() {
    if (m_hMainWnd != NULL) {
        ::PostMessage(m_hMainWnd, WM_SCREEN_SHARED_STARTED, 0, 0);
    }
}

/*
*功能:停止共享处理
*参数:reason:停止共享原因
*返回值:无
*/
void HWEngineEventHandler::onScreenShareStopped(int reason) {
    if (m_hMainWnd != NULL) {
        ::PostMessage(m_hMainWnd, WM_SCREEN_SHARED_STOPED, 0, 0);
    }
}

/*
*功能:用户辅流处理
*参数:roomId:共享的房间号 userId:共享的用户号 available:是否共享
*返回值:无
*/
void HWEngineEventHandler::onUserAuxiliaryStreamAvailable(const char* roomId, const char* userId, bool available) {
    wchar_t *room = StringUtility::HW_Utf8ToUnicodeW(roomId);
    wchar_t *user = StringUtility::HW_Utf8ToUnicodeW(userId);
    ScreenShareInfo* shareInfo = new ScreenShareInfo();
    _tcscpy_s(shareInfo->roomId, HRTCConstant::HRTC_MAX_ROOMID_LEN, room);
    _tcscpy_s(shareInfo->userId, HRTCConstant::HRTC_MAX_USERID_LEN, user);
    shareInfo->available = available;

    RD_LOG("onUserAuxStreamAvailable", " share roomId =%s , userId = %s, available= %d\n", roomId, userId, shareInfo->available);
    if (m_hMainWnd != NULL) {
        ::PostMessage(m_hMainWnd, WM_SCREEN_SHARED_AVAILABLE, (WPARAM)shareInfo, 0);
    }
    if (room != NULL) {
        free(room);
    }
    if (user != NULL) {
        free(user);
    }
}