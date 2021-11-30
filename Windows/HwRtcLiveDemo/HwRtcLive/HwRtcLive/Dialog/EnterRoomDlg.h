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
���뷿��ҳ��
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
    void SetUserRole(HRTCRoleType type);                // �����û���ɫ
    afx_msg void OnBnClickedRadioTeacher();             // ������ʦѡ��ť����
    afx_msg void OnBnClickedRadioStudent();             // ѧ��ѡ��ť����
    enum { IDD = IDD_LOGIN_DIALOG };
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // ���ڿؼ����ݽ���
    virtual BOOL OnInitDialog();                        // ��ʼ������
    virtual BOOL PreTranslateMessage(MSG* pMsg);        // ��������Ϣ
    afx_msg void OnPaint();                             // ���ƴ���
    afx_msg void OnBnClickedBtnJoinRoom();              // ���뷿�䰴ť����
    afx_msg void OnBnClickedBtnPreSet();                // ���ð�ť����
    afx_msg void OnBnClickedBtnCreate();                // �������䰴ť����
    DECLARE_MESSAGE_MAP()

    void InitCtrls();                   //��ʼ���ؼ�
    void InitCboxClass();               // ��ʼ������������
    void SetClassRoleInfo();            // ���ý��ҽ�ɫ��Ϣ����������
    void DrawClient(CDC *lpDC);         // ���ƿͻ���
private:
    CEdit           m_ctrUserId;        // �û�ID
    CMFCButton      m_btnJoin;          // ���뷿�䰴ť
    CMFCButton      m_btnCreat;         // �������䰴ť
    CMFCButton      m_btnPreSet;        // ���ð�ť
    CFont           m_ftDesc;           // ��������
    CFont           m_ftBtn;            // ��ť����
    CPen            m_penFrame;         // ��ܻ���

    CButton         m_radioTeacher;     // ���ҽ�ɫ��ť
    CButton         m_radioStudent;     // ѧ����ɫ��ť
    CButton         m_radioJoiner;      // ˫������ɫ��ť
    CButton         m_radioPublisher;   // �����߽�ɫ��ť
    CButton         m_radioPlayer;      // ��������ɫ��ť
    CDeviceDlgNew   m_dlgDevice;        // �����������豸����
    CButton         m_encodeLD;         // ���÷ֱ���Ϊ������ť
    CButton         m_encodeSD;         // ���÷ֱ���Ϊ���尴ť
    CButton         m_encodeHD;         // ���÷ֱ���Ϊ���尴ť
    CButton         m_encodeFHD;        // ���÷ֱ���Ϊȫ���尴ť
    vector<string>  m_gradeIdList;      // �༶�б�
    CComboBox       m_cboxGrade;        // �༶�б�ؼ�
};
