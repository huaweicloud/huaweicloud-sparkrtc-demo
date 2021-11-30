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

package com.huawei.rtcdemo.interfaces;

import com.huawei.rtc.models.HRTCAudioFrame;
import com.huawei.rtc.models.HRTCLocalAudioStats;
import com.huawei.rtc.models.HRTCLocalVideoStats;
import com.huawei.rtc.models.HRTCRemoteAudioStats;
import com.huawei.rtc.models.HRTCRemoteVideoStats;
import com.huawei.rtc.models.HRTCStatsInfo;
import com.huawei.rtc.models.HRTCUserInfo;
import com.huawei.rtc.models.HRTCVideoFrame;
import com.huawei.rtc.utils.HRTCEnums;

import java.util.List;

/**
 * rtc sdk 回调接口
 */
public interface RtcEventHandler {
    /**
     * 告警
     * @param var1 错误码
     * @param var2 描述
     */
    void onWarning(int var1, String var2);

    /**
     * 错误
     * @param var1 错误码
     * @param var2 描述
     */
    void onError(int var1, String var2);

    /**
     * 加入房间成功
     * @param var1 房间id
     * @param var2 用户id
     */
    void onJoinRoomSuccess(String var1, String var2);

    /**
     * 加入房间失败
     * @param var1 错误码
     * @param var2 描述
     */
    void onJoinRoomFailure(int var1, String var2);

    /**
     * 离开房间
     * @param reason 离开原因
     * @param var3 会议期间的统计信息
     */
    void onLeaveRoom(HRTCEnums.HRTCLeaveReason reason, HRTCStatsInfo var3);

    /**
     * 远端用户加入房间
     * @param var1 房间ID
     * @param var2 用户ID
     * @param var3 用户名称
     */
    void onUserJoined(String var1, String var2, String var3);

    /**
     * 远端用户离开房间
     * @param var1 房间ID
     * @param var2 用户ID
     * @param var3 预留字段
     */
    void onUserOffline(String var1, String var2, int var3);

    /**
     * 连接状态发生改变
     * @param var1 网络连接状态
     * @param var2 网络连接状态原因
     * @param var3 描述
     */
    void onConnectionStateChange(HRTCEnums.HRTCConnStateTypes var1, HRTCEnums.HRTCConnChangeReason var2, String var3);

    /**
     * 用户角色发生变化
     * @param var2 改变前的角色
     * @param var3 改变后的角色
     */
    void onUserRoleChanged(HRTCUserInfo.HRTCRoleType var2, HRTCUserInfo.HRTCRoleType var3);

    /**
     * 音频模式发生变化
     * @param var1 音频路由
     */
    void onAudioRouteChanged(HRTCEnums.HRTCAudioRoute var1);

    /**
     * 视频流详情，2s触发一次回调
     * @param var1 本地视频发流详情参数
     * @param var2 远端视频收流详情参数
     */
    void onVideoStats(List<HRTCLocalVideoStats> var1, List<HRTCRemoteVideoStats> var2);

    /**
     * 音频流详情，2s触发一次回调
     * @param var1 本地音频发流详情
     * @param var2 远端音频收流详情
     */
    void onAudioStats(List<HRTCLocalAudioStats> var1, List<HRTCRemoteAudioStats> var2);

    /**
     * 辅流加入房间
     * @param var1 房间ID
     * @param var2 用户ID
     * @param var3 true为辅流开始推送，false为辅流停止推送
     */
    void onUserSubStreamAvailable(String var1, String var2, boolean var3);

    /**
     * 辅流详情，2s触发一次回调
     * @param var1 本地辅流的发流详情
     * @param var2 远端辅流的收流详情
     */
    void onSubStreamStats(List<HRTCLocalVideoStats> var1, List<HRTCRemoteVideoStats> var2);

    /**
     * 回调远端的视频原始数据流
     * @param roomId 房间ID
     * @param userId 用户ID
     * @param frame 视频数据
     */
    void onRenderVideoFrame(String roomId, String userId, HRTCVideoFrame frame);

    /**
     * 回调远端的音频原始数据流
     * @param frame 音频数据
     */
    void onPlaybackAudioFrame(HRTCAudioFrame frame);
}
