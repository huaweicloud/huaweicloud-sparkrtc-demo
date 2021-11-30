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
短时提示框
*/
#pragma once

#include "resource.h"
#include "afxwin.h"
// CToastDlg dialog

class CToastDlg : public CDialogEx {
    DECLARE_DYNAMIC(CToastDlg)

public:
    CToastDlg(CWnd* pParent = NULL);
    virtual ~CToastDlg();
    void ShowToast(CString text);                       // 显示短时提示信息
    enum { IDD = IDD_TOAST_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    int OnCreate(LPCREATESTRUCT lpCreateStruct);        // 创建提示对话框
    afx_msg void OnTimer(UINT_PTR nIDEvent);            // 提示信息定时器超时处理

    DECLARE_MESSAGE_MAP()
private:
    CString m_toastText;    // 提示信息
    CStatic m_ctrText;      // 提示信息显示控件
    int m_timerCount;       // 提示信息显示定时器计数
};
