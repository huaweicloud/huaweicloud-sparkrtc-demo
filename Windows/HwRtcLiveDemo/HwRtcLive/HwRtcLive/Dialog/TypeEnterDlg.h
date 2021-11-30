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
大班课模式选择页面
*/
#pragma once

#include "IHRTCEngine.h"

// CTypeEnterDlg 对话框

class CTypeEnterDlg : public CDialogEx {
    DECLARE_DYNAMIC(CTypeEnterDlg)

public:
    CTypeEnterDlg(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CTypeEnterDlg();


    enum { IDD = IDD_TYPE_DIALOG };


protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL OnInitDialog();
    DECLARE_MESSAGE_MAP()
    void InitCtrls();
    afx_msg void OnPaint();
    void DrawClient(CDC *lpDC);
private:
    CMFCButton      m_btn1VN;       // 1VN大班课按钮
    CFont           m_ftDesc;       // 描述字体
    CFont           m_ftBtn;        // 按钮字体
    CPen            m_penFrame;     // 框架画笔
public:
    afx_msg void OnBnClickedBtnLogin1VN();
};
