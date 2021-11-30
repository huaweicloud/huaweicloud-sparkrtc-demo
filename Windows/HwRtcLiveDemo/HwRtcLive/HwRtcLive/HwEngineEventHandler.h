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
主引擎回调类，重载了主引擎回调基类IHRTCEngineEventHandler的回调方法，在回调方法里把消息发给主窗口处理。
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
    // 告警回调
    virtual void onWarning(int warn, const char* msg);
    // 错误回调
    virtual void onError(int error, const char* msg);
    // 加入房间成功回调
    virtual void onJoinRoomSuccess(const char* roomId, const char* userId);
    // 重新加入房间成功回调
    virtual void onRejoinRoomSuccess(const char* roomId, const char* userId);
    // 加入房间失败回调
    virtual void onJoinRoomFailure(int error, const char* msg);
    // 离开房间回调
    virtual void onLeaveRoom(HRTCLeaveReason reason, const HRTCStatsInfo &statsInfo);
    // 远端用户加入当前房间回调
    virtual void onRemoteUserOnline(const char* roomId, const char* userId, const char* userName);
    // 远端用户离开当前房间回调
    virtual void onRemoteUserOffline(const char* roomId, const char* userId, int reason);
    // 网络连接状态发生变化回调
    virtual void onConnectionChangedNotify(HRTCConnStateTypes state, HRTCConnChangeReason reason, const char* description);
    // 设备状态发生变化回调
    virtual void onDeviceStateChangedNotify(const char* deviceId, HRTCDeviceType deviceType, HRTCDeviceState deviceState);
    // 音频设备音量发生变化回调
    virtual void onDeviceVolumeChangedNotify(HRTCDeviceType deviceType, unsigned volume, unsigned muted);
    // 用户角色切换成功回调
    virtual void onUserRoleChangedNotify(const char* roomId, HRTCRoleType oldRole, HRTCRoleType newRole);
    // 视频流详情回调，2s触发一次回调
    virtual void onVideoStatsNotify(HRTCLocalVideoStats *localStats, unsigned int localStatsCount, HRTCRemoteVideoStats *remoteStats, unsigned int remoteStatsCount);
    // 音频流详情回调，2s触发一次回调
    virtual void onAudioStatsNotify(HRTCLocalAudioStats *localStats, unsigned int localStatsCount, HRTCRemoteAudioStats *remoteStats, unsigned int remoteStatsCount);
    // 共享流详情回调，2s触发一次回调
    virtual void onAuxiliaryStreamStatsNotify(HRTCLocalVideoStats *localStats, unsigned int localStatsCount, HRTCRemoteVideoStats *remoteStats, unsigned int remoteStatsCount);
    // 共享流开启回调
    virtual void onScreenShareStarted();
    // 共享流关闭回调
    virtual void onScreenShareStopped(int reason);
    // 远端开启/停止共享流回调
    virtual void onUserAuxiliaryStreamAvailable(const char* roomId, const char* userId, bool available);
    // 设置接收消息的窗口句柄
    void SetMsgReceiver(HWND hWnd = NULL) { m_hMainWnd = hWnd; };
    // 获取接收消息的窗口句柄
    const HWND GetMsgReceiver() { return m_hMainWnd; };
private:
    // 接收消息的窗口句柄
    HWND        m_hMainWnd;
};
