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
创建房间页面类
*/
#pragma once
#pragma once
#include "afxwin.h"
#include "afxlistctrl.h"
#include "HRTCDefine.h"
#include "HwRtcLive.h"
#include <vector>
using namespace std;
using namespace huawei::rtc;

// CCreateDlg 对话框

class CCreateDlg : public CDialogEx {
    DECLARE_DYNAMIC(CCreateDlg)

public:
    CCreateDlg(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CCreateDlg();
    enum { IDD = IDD_CREATE_ROOM_DIALOG };
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL OnInitDialog();            // 初始化窗口
    afx_msg void OnBnClickedBtnJoinRoom();  // 加入房间
    afx_msg void OnBnClickedBtnReturn();    // 返回登录界面
    void InitCtrls();           // 初始化控件
    void SetClassRoleInfo();    // 设置用户加入的教室信息
    afx_msg void OnPaint();     // 绘制窗口
    void DrawClient(CDC *lpDC); // 绘制客户区
    DECLARE_MESSAGE_MAP()
private:
    CFont           m_ftDesc;       // 描述字体
    CFont           m_ftBtn;        // 按钮字体
    CPen            m_penFrame;     // 框架画笔
    CFont           m_ftText;       // 文字字体
    CEdit           m_ctrRoomId;    // 房间ID输入框
    CEdit           m_ctrUserId;    // 用户ID输入框
    CMFCButton      m_btnJoin;      // 加入按钮
    CMFCButton      m_btnReturn;    // 返回按钮
};
