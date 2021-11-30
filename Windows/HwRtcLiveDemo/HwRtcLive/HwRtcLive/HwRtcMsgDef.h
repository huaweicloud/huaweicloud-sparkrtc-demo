/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
*
* Licensed to the Apache Software Foundation (ASF) under one
* or more contributor license agreements.  See the NOTICE file
* distributed with this work for additional information
* regarding copyright ownership.  The ASF licenses this file
* to you under the Apache License, Version 2.0 (the
* "License"); you may not use this file except in compliance
* with the License.  You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing,
* software distributed under the License is distributed on an
* "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
* KIND, either express or implied.  See the License for the
* specific language governing permissions and limitations
* under the License.
*/
/*
��Ϣ����
*/
#pragma once
#include "HRTCDefine.h"
#include <map> 
#include <string> 
#include<list>
#include <vector>
#define WM_JOINROOM WM_USER+100
#define WM_LEAVEROOM WM_USER+101
#define WM_USERJOIN WM_USER+102
#define WM_USEROFFLINE WM_USER+103
#define WM_SHOW_USERLIST WM_USER+104
#define WM_LIST_ITEM WM_USER+105
#define WM_AUDIO_VOLUME_INDICATION WM_USER+106
#define WM_DEVICE_VOLUME_CHANGE WM_USER+107
#define WM_DEVICE_STATE_CHANGE WM_USER+108
#define WM_SHOW_TOAST WM_USER+109
#define WM_SHOW_NOTICE WM_USER+110
#define WM_DEFAULTWATCH WM_USER+112
#define WM_LEAVE_ROOM WM_USER+113
#define WM_NOTIFY_MAIN_TRAY_ICON WM_USER+114
#define WM_USER_ROLE_CHANGED WM_USER+115
#define WM_CHANGE_ROLE_RADIO WM_USER+116
#define WM_JOINROOM_FAIL WM_USER+117
#define WM_USER_ROLE_CHANGE_FAIL WM_USER+118
#define WM_CHANGE_DEVICE_TYPE WM_USER+119
#define WM_SCREEN_SHARED_STARTED WM_USER+120
#define WM_SCREEN_SHARED_STOPED WM_USER+121
#define WM_SCREEN_SHARED_AVAILABLE WM_USER+122
#define WM_AUDIO_INFO_CHANGED WM_USER+123
#define WM_VIDEO_INFO_CHANGED WM_USER+124
#define WM_SCREEN_SHARED_INFO_CHANGED WM_USER+125
#define WM_JOINROOM_SUCCESS WM_USER+126
#define WM_USER_ROOM_ROLE_CHANGE_FAIL WM_USER+127
#define WM_USER_ENTER_LOGIN WM_USER+128
#define WM_USER_CREATE_ROOM WM_USER+129
#define WM_USER_RETURN WM_USER+130
#define MAX_WATCH_NUM 6
#define HW_OK 0



using namespace huawei::rtc;
using namespace std;

extern map<string, string> HRTC_USER_NAME_MAP;

/*������û���Ϣ*/
class UserJoinInfo {
public:
    TCHAR userId[HRTCConstant::HRTC_MAX_USERID_LEN];
    TCHAR nickname[HRTCConstant::HRTC_MAX_USERNAME_LEN];
    TCHAR roomId[HRTCConstant::HRTC_MAX_ROOMID_LEN];
};

/*���ߵ��û���Ϣ*/
class UserOfflineInfo {
public:
    TCHAR userId[HRTCConstant::HRTC_MAX_USERID_LEN];
    TCHAR roomId[HRTCConstant::HRTC_MAX_ROOMID_LEN];
    int reason;
};

/*ѡ�����û���Ϣ*/
class UserWatchInfo {
public:
    TCHAR userId[HRTCConstant::HRTC_MAX_USERID_LEN];
    bool bWatch;
};

/*��������״̬��Ϣ*/
class ConnectionStateInfo {
public:
    HRTCConnStateTypes types;
    HRTCConnChangeReason reason;
    TCHAR descprition[128];
};

/*�豸��Ϣ*/
class DeviceInfos {
public:
    HRTCDeviceType deviceType;
    unsigned volume;
    unsigned muted;
    char deviceId[HRTCConstant::HRTC_MAX_DEVICE_ID_LEN];
    HRTCDeviceState deviceState;
};

/*Ĭ��ѡ���û���Ϣ*/
class DefaultWatchInfo {
public:
    TCHAR roomId[HRTCConstant::HRTC_MAX_ROOMID_LEN];
    TCHAR userId[HRTCConstant::HRTC_MAX_USERID_LEN];
    HRTCStreamType streamType;
};

/*��Ļ������û���Ϣ*/
class ScreenShareInfo {
public:
    TCHAR roomId[HRTCConstant::HRTC_MAX_ROOMID_LEN];
    TCHAR userId[HRTCConstant::HRTC_MAX_USERID_LEN];
    int available;
};

/*��Ļ�����״̬*/
class ScreenShareStats {
public:
    list<HRTCLocalVideoStats> localStats;
    list<HRTCRemoteVideoStats> remoteStats;
};

/*��ɫ�л�����Ϣ*/
class RoleChangeInfo {
public:
    string roomId;
    HRTCRoleType oldRole;
    HRTCRoleType newRole;
};

/*�豸�л�������*/
enum HwRtcDeviceChangeType {
    DEVICE_CHANGE_SOFT_VOLUME = 1,
    DEVICE_CHANGE_CAMERA = 2,
    DEVICE_CHANGE_STREAM_TYPE = 4,
};

/*�û���ɫ*/
enum ClassRole {
    CLASS_ROLE_TEACHER = 0,
    CLASS_ROLE_TUTOR = 1,
    CLASS_ROLE_STUDENT = 2,
    CLASS_ROLE_IDLE
};

/*�û�����Ľ�����Ϣ*/
class ClassRoleInfo {
public:
    ClassRole role = CLASS_ROLE_IDLE;
    string gradeRoomId;
    string classRoomId;
    string groupRoomId;
    string userId;
    string userName;
    string roomId;
};

/*���뷿������*/
enum HwRtcEnterRoomType {
    ENTER_ROOM_1VN,
    ENTER_ROOM_CLASS
};