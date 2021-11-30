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
// UserListDlg.h : 头文件
/*
用户列表页面
*/
#pragma once

#include "resource.h"
#include "afxwin.h"
// CUserListDlg 对话框
class CVideoDlg;
class CUserListDlg : public CDialogEx {
    //控件变量
protected:
    CListBox        m_listUser;         // 用户列表
    CVideoDlg*      m_pVideoObject;     // 视频窗口指针
    //控件变量

// 构造
public:
    CUserListDlg(CWnd* pParent = NULL); // 标准构造函数

// 对话框数据
    enum { IDD = IDD_USERLIST_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    // 生成的消息映射函数
    virtual BOOL OnInitDialog();        // 初始化窗口
    afx_msg void OnPaint();             // 绘制窗口
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);// 擦除窗口背景
    DECLARE_MESSAGE_MAP()

public:
    void AddUser(TCHAR *userId);                    // 添加到用户列表
    void AddOwn(TCHAR *userId);                     // 添加自己到用户列表
    void DeleteUser(TCHAR *userId);                 // 删除用户信息
    void UpdateUser(TCHAR *userId);                 // 更新用户信息
    void ClearData();                               // 清空用户列表
    void RefreshListBox();                          // 更新界面上的用户列表
    void SetVideoObject(CVideoDlg * videoObject);   // 设置视频窗口指针
    bool IsExist(TCHAR *userId);                    // 检查是否已在用户列表中
    CStatic m_txtCount;
};
