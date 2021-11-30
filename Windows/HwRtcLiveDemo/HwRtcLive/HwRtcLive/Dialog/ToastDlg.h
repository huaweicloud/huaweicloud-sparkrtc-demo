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
��ʱ��ʾ��
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
    void ShowToast(CString text);                       // ��ʾ��ʱ��ʾ��Ϣ
    enum { IDD = IDD_TOAST_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    int OnCreate(LPCREATESTRUCT lpCreateStruct);        // ������ʾ�Ի���
    afx_msg void OnTimer(UINT_PTR nIDEvent);            // ��ʾ��Ϣ��ʱ����ʱ����

    DECLARE_MESSAGE_MAP()
private:
    CString m_toastText;    // ��ʾ��Ϣ
    CStatic m_ctrText;      // ��ʾ��Ϣ��ʾ�ؼ�
    int m_timerCount;       // ��ʾ��Ϣ��ʾ��ʱ������
};
