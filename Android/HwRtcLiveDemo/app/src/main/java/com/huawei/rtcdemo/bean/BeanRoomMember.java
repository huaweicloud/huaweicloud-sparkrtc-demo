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

package com.huawei.rtcdemo.bean;

/**
 * 与会者列表用户java bean
 */
public class BeanRoomMember {
    /** 房间名 */
    private String roomId;

    /** 用户名 */
    private String userId;

    /** 昵称 */
    private String nickName;

    /** 是否播放（订阅） */
    private boolean isPlaying;

    /** 是否本地用户 */
    private boolean isSelf = false;

    public String getUserId() {
        return userId;
    }

    public String getNickname() {
        return nickName;
    }

    public void setNickname(String name) {
        this.nickName = name;
    }

    public boolean getIsPlaying() {
        return isPlaying;
    }

    public void setIsPlaying(boolean isPlaying) {
        this.isPlaying = isPlaying;
    }

    public boolean isSelf() {
        return isSelf;
    }

    public void setSelf(boolean self) {
        isSelf = self;
    }

    public String getRoomId() {
        return roomId;
    }

    public void setRoomId(String roomId) {
        this.roomId = roomId;
    }

    public BeanRoomMember(String roomId, String userId, String nickname, boolean isPlaying) {
        this.roomId = roomId;
        this.userId = userId;
        this.nickName = nickname;
        this.isPlaying = isPlaying;
    }
}
