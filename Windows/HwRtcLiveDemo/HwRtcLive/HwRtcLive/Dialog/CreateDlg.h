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
��������ҳ����
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

// CCreateDlg �Ի���

class CCreateDlg : public CDialogEx {
    DECLARE_DYNAMIC(CCreateDlg)

public:
    CCreateDlg(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~CCreateDlg();
    enum { IDD = IDD_CREATE_ROOM_DIALOG };
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    virtual BOOL OnInitDialog();            // ��ʼ������
    afx_msg void OnBnClickedBtnJoinRoom();  // ���뷿��
    afx_msg void OnBnClickedBtnReturn();    // ���ص�¼����
    void InitCtrls();           // ��ʼ���ؼ�
    void SetClassRoleInfo();    // �����û�����Ľ�����Ϣ
    afx_msg void OnPaint();     // ���ƴ���
    void DrawClient(CDC *lpDC); // ���ƿͻ���
    DECLARE_MESSAGE_MAP()
private:
    CFont           m_ftDesc;       // ��������
    CFont           m_ftBtn;        // ��ť����
    CPen            m_penFrame;     // ��ܻ���
    CFont           m_ftText;       // ��������
    CEdit           m_ctrRoomId;    // ����ID�����
    CEdit           m_ctrUserId;    // �û�ID�����
    CMFCButton      m_btnJoin;      // ���밴ť
    CMFCButton      m_btnReturn;    // ���ذ�ť
};
