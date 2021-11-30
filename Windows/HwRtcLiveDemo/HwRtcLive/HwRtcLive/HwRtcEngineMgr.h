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
主引擎管理类，提供对主引擎对象的管理，以及部分主引擎方法的封装。
*/
#pragma once

#include "HRTCDefine.h"
#include "IHRTCEngine.h"
#include "HwEngineEventHandler.h"
#include "HwRtcMsgDef.h"
// 日志路径
#define RTC_LOG_PATH "d:\\rtc_log_1\\" 
using namespace std;

using namespace huawei::rtc;
class  CRtcEngineMgr {
public:
    // 获取引擎管理类实例
    static CRtcEngineMgr* GetInstance(const char* appId = NULL, const char* domain = NULL);
    // 释放引擎
    static void FreeInstance();
    // 获取引擎指针
    static IHRTCEngine* GetEngine();
    // 设置SDK回调消息的处理窗口句柄
    void CRtcEngineMgr::SetMsgHandlerWnd(HWND hWnd);
    // 获取SDK回调消息的处理窗口句柄
    const HWND CRtcEngineMgr::GetMsgHandlerWnd();
    // 加入房间
    BOOL JoinRoom(const char* userId, const char* userName, const char* roomId, bool bOpen);
    // 设置本地视频窗口句柄
    BOOL setupLocalVideo(void * hwnd);
    // 退出房间
    BOOL leaveRoom();
    // 设置房间里的角色
    static void SetUserRole(string roomid, huawei::rtc::HRTCRoleType roleType);
    // 获取房间里的角色
    static huawei::rtc::HRTCRoleType GetUserRole(string roomid);
    // 设置视频编码参数
    int setVideoEncParam(unsigned int totalBitRate, vector<HRTCVideoEncParam> &encoderParams);
    // 设置用户加入的教室信息
    void SetClassRoleInfo(ClassRoleInfo &classRole);
    // 获取用户加入的教室信息
    const ClassRoleInfo* GetClassRoleInfo();
    // 清空用户加入的教室信息
    void CleanClassRoleInfo();
    // 设置加入房间类型
    void SetEnterType(HwRtcEnterRoomType type);
    // 获取加入房间类型
    HwRtcEnterRoomType GetEnterType() { return m_enterType; };
private:
    CRtcEngineMgr();
    ~CRtcEngineMgr();
    // 引擎指针
    static IHRTCEngine* m_pRtcEngine;
    // 引擎管理指针
    static CRtcEngineMgr* m_pRtcEngineMgr;
    //SDK回调处理对象
    static HWEngineEventHandler m_engineEventHandler;
    //加入房间的角色
    static huawei::rtc::HRTCRoleType CRtcEngineMgr::m_roleType;
    // 鉴权签名字符串
    wstring m_signatrue;
    // 鉴权时间戳
    long long m_ctime;
    // 加入的教室信息
    ClassRoleInfo m_classRoleInfo;
    // 加入房间类型
    HwRtcEnterRoomType m_enterType;
};

// 获取引擎管理类实例
#define RTC_ENGINEMGR() (CRtcEngineMgr::GetInstance())

