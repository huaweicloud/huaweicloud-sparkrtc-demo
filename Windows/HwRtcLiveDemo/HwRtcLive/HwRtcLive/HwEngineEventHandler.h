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
������ص��࣬������������ص�����IHRTCEngineEventHandler�Ļص��������ڻص����������Ϣ���������ڴ���
*/
#pragma once
#include "IHRTCEngine.h"
using namespace huawei::rtc;
using namespace std;

class HWEngineEventHandler :
    public IHRTCEngineEventHandler {
public:
    HWEngineEventHandler(void);
    ~HWEngineEventHandler(void);
    // �澯�ص�
    virtual void onWarning(int warn, const char* msg);
    // ����ص�
    virtual void onError(int error, const char* msg);
    // ���뷿��ɹ��ص�
    virtual void onJoinRoomSuccess(const char* roomId, const char* userId);
    // ���¼��뷿��ɹ��ص�
    virtual void onRejoinRoomSuccess(const char* roomId, const char* userId);
    // ���뷿��ʧ�ܻص�
    virtual void onJoinRoomFailure(int error, const char* msg);
    // �뿪����ص�
    virtual void onLeaveRoom(HRTCLeaveReason reason, const HRTCStatsInfo &statsInfo);
    // Զ���û����뵱ǰ����ص�
    virtual void onRemoteUserOnline(const char* roomId, const char* userId, const char* userName);
    // Զ���û��뿪��ǰ����ص�
    virtual void onRemoteUserOffline(const char* roomId, const char* userId, int reason);
    // ��������״̬�����仯�ص�
    virtual void onConnectionChangedNotify(HRTCConnStateTypes state, HRTCConnChangeReason reason, const char* description);
    // �豸״̬�����仯�ص�
    virtual void onDeviceStateChangedNotify(const char* deviceId, HRTCDeviceType deviceType, HRTCDeviceState deviceState);
    // ��Ƶ�豸���������仯�ص�
    virtual void onDeviceVolumeChangedNotify(HRTCDeviceType deviceType, unsigned volume, unsigned muted);
    // �û���ɫ�л��ɹ��ص�
    virtual void onUserRoleChangedNotify(const char* roomId, HRTCRoleType oldRole, HRTCRoleType newRole);
    // ��Ƶ������ص���2s����һ�λص�
    virtual void onVideoStatsNotify(HRTCLocalVideoStats *localStats, unsigned int localStatsCount, HRTCRemoteVideoStats *remoteStats, unsigned int remoteStatsCount);
    // ��Ƶ������ص���2s����һ�λص�
    virtual void onAudioStatsNotify(HRTCLocalAudioStats *localStats, unsigned int localStatsCount, HRTCRemoteAudioStats *remoteStats, unsigned int remoteStatsCount);
    // ����������ص���2s����һ�λص�
    virtual void onAuxiliaryStreamStatsNotify(HRTCLocalVideoStats *localStats, unsigned int localStatsCount, HRTCRemoteVideoStats *remoteStats, unsigned int remoteStatsCount);
    // �����������ص�
    virtual void onScreenShareStarted();
    // �������رջص�
    virtual void onScreenShareStopped(int reason);
    // Զ�˿���/ֹͣ�������ص�
    virtual void onUserAuxiliaryStreamAvailable(const char* roomId, const char* userId, bool available);
    // ���ý�����Ϣ�Ĵ��ھ��
    void SetMsgReceiver(HWND hWnd = NULL) { m_hMainWnd = hWnd; };
    // ��ȡ������Ϣ�Ĵ��ھ��
    const HWND GetMsgReceiver() { return m_hMainWnd; };
private:
    // ������Ϣ�Ĵ��ھ��
    HWND        m_hMainWnd;
};
