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
通知提示框
*/
#pragma once
#include "afxwin.h"


// CNoticeDlg 对话框

class CNoticeDlg : public CDialogEx {
    DECLARE_DYNAMIC(CNoticeDlg)

public:
    CNoticeDlg(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CNoticeDlg();
    void MessageBox(CString sMsg);      // 设置提示信息
    // 对话框数据
    enum { IDD = IDD_NOTICE_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    afx_msg void OnBnClickedCancel();   // 取消按钮处理
    BOOL OnInitDialog();                // 初始化窗口
    DECLARE_MESSAGE_MAP()
private:
    CStatic m_noticeMsg;                // 提示信息显示控件
    CString m_msg;                      // 提示信息
};
