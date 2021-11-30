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
����������࣬�ṩ�����������Ĺ����Լ����������淽���ķ�װ��
*/
#pragma once

#include "HRTCDefine.h"
#include "IHRTCEngine.h"
#include "HwEngineEventHandler.h"
#include "HwRtcMsgDef.h"
// ��־·��
#define RTC_LOG_PATH "d:\\rtc_log_1\\" 
using namespace std;

using namespace huawei::rtc;
class  CRtcEngineMgr {
public:
    // ��ȡ���������ʵ��
    static CRtcEngineMgr* GetInstance(const char* appId = NULL, const char* domain = NULL);
    // �ͷ�����
    static void FreeInstance();
    // ��ȡ����ָ��
    static IHRTCEngine* GetEngine();
    // ����SDK�ص���Ϣ�Ĵ����ھ��
    void CRtcEngineMgr::SetMsgHandlerWnd(HWND hWnd);
    // ��ȡSDK�ص���Ϣ�Ĵ����ھ��
    const HWND CRtcEngineMgr::GetMsgHandlerWnd();
    // ���뷿��
    BOOL JoinRoom(const char* userId, const char* userName, const char* roomId, bool bOpen);
    // ���ñ�����Ƶ���ھ��
    BOOL setupLocalVideo(void * hwnd);
    // �˳�����
    BOOL leaveRoom();
    // ���÷�����Ľ�ɫ
    static void SetUserRole(string roomid, huawei::rtc::HRTCRoleType roleType);
    // ��ȡ������Ľ�ɫ
    static huawei::rtc::HRTCRoleType GetUserRole(string roomid);
    // ������Ƶ�������
    int setVideoEncParam(unsigned int totalBitRate, vector<HRTCVideoEncParam> &encoderParams);
    // �����û�����Ľ�����Ϣ
    void SetClassRoleInfo(ClassRoleInfo &classRole);
    // ��ȡ�û�����Ľ�����Ϣ
    const ClassRoleInfo* GetClassRoleInfo();
    // ����û�����Ľ�����Ϣ
    void CleanClassRoleInfo();
    // ���ü��뷿������
    void SetEnterType(HwRtcEnterRoomType type);
    // ��ȡ���뷿������
    HwRtcEnterRoomType GetEnterType() { return m_enterType; };
private:
    CRtcEngineMgr();
    ~CRtcEngineMgr();
    // ����ָ��
    static IHRTCEngine* m_pRtcEngine;
    // �������ָ��
    static CRtcEngineMgr* m_pRtcEngineMgr;
    //SDK�ص��������
    static HWEngineEventHandler m_engineEventHandler;
    //���뷿��Ľ�ɫ
    static huawei::rtc::HRTCRoleType CRtcEngineMgr::m_roleType;
    // ��Ȩǩ���ַ���
    wstring m_signatrue;
    // ��Ȩʱ���
    long long m_ctime;
    // ����Ľ�����Ϣ
    ClassRoleInfo m_classRoleInfo;
    // ���뷿������
    HwRtcEnterRoomType m_enterType;
};

// ��ȡ���������ʵ��
#define RTC_ENGINEMGR() (CRtcEngineMgr::GetInstance())

