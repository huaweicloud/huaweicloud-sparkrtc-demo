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
加入房间页面
*/
#pragma once
#include "HwRtcMsgDef.h"
#include "afxwin.h"
#include "afxlistctrl.h"
#include "HRTCDefine.h"
#include "..\Dialog\DeviceDlgNew.h"
#include<vector>
#include "afxcmn.h"
using namespace std;
using namespace huawei::rtc;

class CEnterRoomDlg : public CDialogEx {
    DECLARE_DYNAMIC(CEnterRoomDlg)

public:
    CEnterRoomDlg(CWnd* pParent = NULL);
    virtual ~CEnterRoomDlg();
    void SetUserRole(HRTCRoleType type);                // 设置用户角色
    afx_msg void OnBnClickedRadioTeacher();             // 主讲老师选择按钮处理
    afx_msg void OnBnClickedRadioStudent();             // 学生选择按钮处理
    enum { IDD = IDD_LOGIN_DIALOG };
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // 窗口控件数据交换
    virtual BOOL OnInitDialog();                        // 初始化窗口
    virtual BOOL PreTranslateMessage(MSG* pMsg);        // 处理按键消息
    afx_msg void OnPaint();                             // 绘制窗口
    afx_msg void OnBnClickedBtnJoinRoom();              // 加入房间按钮处理
    afx_msg void OnBnClickedBtnPreSet();                // 设置按钮处理
    afx_msg void OnBnClickedBtnCreate();                // 创建房间按钮处理
    DECLARE_MESSAGE_MAP()

    void InitCtrls();                   //初始化控件
    void InitCboxClass();               // 初始化教室下拉框
    void SetClassRoleInfo();            // 设置教室角色信息到管理类中
    void DrawClient(CDC *lpDC);         // 绘制客户区
private:
    CEdit           m_ctrUserId;        // 用户ID
    CMFCButton      m_btnJoin;          // 加入房间按钮
    CMFCButton      m_btnCreat;         // 创建房间按钮
    CMFCButton      m_btnPreSet;        // 设置按钮
    CFont           m_ftDesc;           // 描述字体
    CFont           m_ftBtn;            // 按钮字体
    CPen            m_penFrame;         // 框架画笔

    CButton         m_radioTeacher;     // 教室角色按钮
    CButton         m_radioStudent;     // 学生角色按钮
    CButton         m_radioJoiner;      // 双向流角色按钮
    CButton         m_radioPublisher;   // 发布者角色按钮
    CButton         m_radioPlayer;      // 接收流角色按钮
    CDeviceDlgNew   m_dlgDevice;        // 房间内设置设备窗口
    CButton         m_encodeLD;         // 设置分辨率为流畅按钮
    CButton         m_encodeSD;         // 设置分辨率为标清按钮
    CButton         m_encodeHD;         // 设置分辨率为高清按钮
    CButton         m_encodeFHD;        // 设置分辨率为全高清按钮
    vector<string>  m_gradeIdList;      // 班级列表
    CComboBox       m_cboxGrade;        // 班级列表控件
};
