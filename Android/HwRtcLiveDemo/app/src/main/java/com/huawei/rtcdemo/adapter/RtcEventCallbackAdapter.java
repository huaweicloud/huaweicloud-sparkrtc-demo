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

package com.huawei.rtcdemo.adapter;

import com.huawei.rtc.IHRTCEngineEventHandler;
import com.huawei.rtc.models.HRTCAudioFrame;
import com.huawei.rtc.models.HRTCConnectInfo;
import com.huawei.rtc.models.HRTCLocalAudioStats;
import com.huawei.rtc.models.HRTCLocalVideoStats;
import com.huawei.rtc.models.HRTCRemoteAudioStats;
import com.huawei.rtc.models.HRTCRemoteVideoStats;
import com.huawei.rtc.models.HRTCStatsInfo;
import com.huawei.rtc.models.HRTCUserInfo;
import com.huawei.rtc.models.HRTCVideoFrame;
import com.huawei.rtc.utils.HRTCEnums;
import com.huawei.rtcdemo.interfaces.RtcEventHandler;
import com.huawei.rtcdemo.utils.LogUtil;

import java.util.ArrayList;
import java.util.List;

/**
 * rtc sdk 回调实现
 */
public class RtcEventCallbackAdapter extends IHRTCEngineEventHandler {
    private static final String TAG = "RtcEventHandler";
    private ArrayList<RtcEventHandler> mHandler = new ArrayList<>();

    @Override
    public void onWarning(int code, String msg) {
        LogUtil.d(TAG, "Handler onWarning code : " + code + ", msg : " + msg);
        for (RtcEventHandler handler : mHandler) {
            handler.onWarning(code, msg);
        }
    }

    @Override
    public void onError(int code, String msg) {
        LogUtil.d(TAG, "Handler onError code : " + code + ", msg : " + msg);
        for (RtcEventHandler handler : mHandler) {
            handler.onError(code, msg);
        }
    }

    @Override
    public void onJoinRoomSuccess(String roomId, String userId) {
        LogUtil.d(TAG, "Handler onJoinRoomSuccess roomId: " + roomId + ", userId = " + userId);
        for (RtcEventHandler handler : mHandler) {
            handler.onJoinRoomSuccess(roomId, userId);
        }
    }

    @Override
    public void onJoinRoomFailure(int code, String msg) {
        LogUtil.d(TAG, "Handler onJoinRoomFailure code : " + code + ", msg : " + msg);
        for (RtcEventHandler handler : mHandler) {
            handler.onJoinRoomFailure(code, msg);
        }
    }

    @Override
    public void onLeaveRoom(HRTCEnums.HRTCLeaveReason reason, HRTCStatsInfo statsInfo) {
        LogUtil.d(TAG, "Handler onLeaveRoom reason : " + reason);
        for (RtcEventHandler handler : mHandler) {
            handler.onLeaveRoom(reason, statsInfo);
        }
    }

    @Override
    public void onRemoteUserOnline(String roomId, String userId, String userName) {
        LogUtil.d(TAG, "Handler onRemoteUserOnline roomId:" + roomId + " userId:" + userId + " username:" + userName);
        for (RtcEventHandler handler : mHandler) {
            handler.onUserJoined(roomId, userId, userName);
        }
    }

    @Override
    public void onRemoteUserOffline(String roomId, String userId, int reason) {
        LogUtil.d(TAG, "Handler onRemoteUserOffline roomId:" + roomId + " userId:" + userId + " reason:" + reason);
        for (RtcEventHandler handler : mHandler) {
            handler.onUserOffline(roomId, userId, reason);
        }
    }

    @Override
    public void onFirstRemoteVideoDecoded(String roomId, String userId, int width, int height) {
        LogUtil.d(TAG, "Handler onFirstRemoteVideoDecoded roomId : " + roomId + ", userId : " + userId);
    }

    @Override
    public void onConnectionChangedNotify(HRTCEnums.HRTCConnStateTypes connStateTypes, HRTCEnums.HRTCConnChangeReason connChangeReason, String description) {
        LogUtil.d(TAG, "Handler onConnectionChangedNotify: " + description);
        for (RtcEventHandler handler : mHandler) {
            handler.onConnectionStateChange(connStateTypes, connChangeReason, description);
        }
    }

    @Override
    public void onLogUploadResult(int i) {
        LogUtil.d(TAG, "Handler onLogUploadResult i: " + i);
    }

    @Override
    public void onLogUploadProgress(int i) {
        LogUtil.i(TAG, "Handler onLogUploadProgress i : " + i);
    }

    @Override
    public void onUserRoleChangedNotify(HRTCUserInfo.HRTCRoleType oldRole, HRTCUserInfo.HRTCRoleType newRole) {
        LogUtil.i(TAG, "Handler onUserRoleChangedNotify oldRole : " + oldRole + " , newRole : " + newRole);
        for (RtcEventHandler handler : mHandler) {
            handler.onUserRoleChanged(oldRole, newRole);
        }
    }

    @Override
    public void onAudioRouteStateChangedNotify(HRTCEnums.HRTCAudioRoute route) {
        LogUtil.i(TAG, "Handler onAudioRouteStateChangedNotify audioRoute : " + route);
        for (RtcEventHandler handler : mHandler) {
            handler.onAudioRouteChanged(route);
        }
    }

    @Override
    public void onVideoStatsNotify(List<HRTCLocalVideoStats> localStats, List<HRTCRemoteVideoStats> remoteStats) {
        LogUtil.i(TAG, "Handler onVideoStatsNotify ");
        for (RtcEventHandler handler : mHandler) {
            handler.onVideoStats(localStats, remoteStats);
        }
    }

    @Override
    public void onAudioStatsNotify(List<HRTCLocalAudioStats> localStats, List<HRTCRemoteAudioStats> remoteStats) {
        LogUtil.i(TAG, "Handler onAudioStatsNotify ");
        for (RtcEventHandler handler : mHandler) {
            handler.onAudioStats(localStats, remoteStats);
        }
    }

    @Override
    public void onUserAuxiliaryStreamAvailable(String roomId, String userId, boolean available) {
        LogUtil.d(TAG, "Handler onUserAuxiliaryStreamAvailable userId : " + userId);
        for (RtcEventHandler handler : mHandler) {
            handler.onUserSubStreamAvailable(roomId, userId, available);
        }
    }

    @Override
    public void onRenderExternalVideoFrame(String roomId, HRTCEnums.HRTCMediaDirection direction, String userId, HRTCVideoFrame videoFrame) {
        LogUtil.d(TAG, "Handler onRenderExternalVideoFrame s : " + roomId);
        for (RtcEventHandler handler : mHandler) {
            handler.onRenderVideoFrame(roomId, userId, videoFrame);
        }
    }

    @Override
    public void onPlaybackExternalAudioFrame(String roomId, HRTCEnums.HRTCMediaDirection direction, HRTCAudioFrame audioFrame) {
        LogUtil.d(TAG, "Handler onPlaybackExternalAudioFrame s : " + audioFrame.getChannels());
        for (RtcEventHandler handler : mHandler) {
            handler.onPlaybackAudioFrame(audioFrame);
        }
    }

    @Override
    public void onAuthorizationExpired() {
        LogUtil.d(TAG, "Handler onAuthorizationExpired ");
    }

    @Override
    public void onAuxiliaryStreamStatsNotify(List<HRTCLocalVideoStats> localStats, List<HRTCRemoteVideoStats> remoteStats) {
        LogUtil.d(TAG, "Handler onAuxiliaryStreamStatsNotify ");
        for (RtcEventHandler handler : mHandler) {
            handler.onSubStreamStats(localStats, remoteStats);
        }
    }

    @Override
    public void onHowlingUpDetected(int result) {
        LogUtil.d(TAG, "Handler onHowlingUpDetected result : " + result);
    }

    @Override
    public void onHowlingDownDetected(int result) {
        LogUtil.d(TAG, "Handler onHowlingDownDetected result : " + result);
    }

    public void addHandler(RtcEventHandler handler) {
        LogUtil.i(TAG, "addHandler! ");
        mHandler.add(handler);
    }

    public void removeHandler(RtcEventHandler handler) {
        LogUtil.i(TAG, "removeHandler! ");
        mHandler.remove(handler);
    }
}
