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
// HwRtcLiveDlg.h : header file
//
/*
��������
*/
#pragma once
#include "EnterRoomDlg.h"
#include "TypeEnterDlg.h"
#include "VideoDlg.h"
#include "ToastDlg.h"
#include "Systray.h"
#include "CreateDlg.h"
// CHwRtcLiveDlg dialog
class CRtcEngineMgr;
class CHwRtcLiveDlg : public CDialogEx {
    // Construction
public:
    CHwRtcLiveDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data

    enum { IDD = IDD_HWRTCLIVE_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
    HICON m_hIcon;
    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    // ���뷿��
    afx_msg LRESULT OnJoinRoom(WPARAM wParam, LPARAM lParam);
    // �˳�����
    afx_msg LRESULT OnLeaveRoom(WPARAM wParam, LPARAM lParam);
    // �����¼����
    afx_msg LRESULT OnEnterLogin(WPARAM wParam, LPARAM lParam);
    // ���봴������ҳ��
    afx_msg LRESULT OnCreateRoom(WPARAM wParam, LPARAM lParam);
    //�ص���¼ҳ��
    afx_msg LRESULT OnReturn(WPARAM wParam, LPARAM lParam);
    // ��ʾ��ʱ��ʾ��Ϣ
    afx_msg LRESULT OnShowToast(WPARAM wParam, LPARAM lParam);
    // ��ʾ��ʾ��Ϣ
    afx_msg LRESULT OnShowNotice(WPARAM wParam, LPARAM lParam);
    // �޸��û���ɫ
    afx_msg LRESULT OnChangeRoleRadio(WPARAM wParam, LPARAM lParam);
    // ϵͳ���̲���֪ͨ
    afx_msg LRESULT OnSysTrayNotify(WPARAM wParam, LPARAM lParam);
    // ��С����ť����
    afx_msg void OnBnClickedBtnmin();
    // �رհ�ť����
    afx_msg void OnBnClickedBtnclose();
    // �����϶�����
    afx_msg LRESULT OnNcHitTest(CPoint point);
    DECLARE_MESSAGE_MAP()
    // ����ϵͳ���̲˵�
    void CreateLoginMenu();
    // �ϴ���־
    void onMenuUploadLog();
protected:
    // ��ʼ���ؼ�
    void InitCtrls();
    // ���ƿͻ���
    void DrawClient(CDC *lpDC);
    // ��ʼ���Ӵ���
    void InitChildDialog();
    // ��������Ϣ
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    // ���ñ������
    void SetEncParam();
private:
    CMFCButton       m_btnMin;           // ��С����ť
    CButton       m_btnClose;         // �رհ�ť
    CButton       m_btnSetting;       // ���ð�ť
    CEnterRoomDlg   m_dlgEnterRoom;     // ���뷿�䴰��
    CCreateDlg      m_dlgCreate;        // �������䴰��
    CTypeEnterDlg   m_dlgTypeEnter;     // ����ģʽѡ�񴰿�
    CVideoDlg       m_dlgVideo;         // ����Ƶͨ������
    CToastDlg       m_toastWnd;         // ��ʱ֪ͨ����
    CRtcEngineMgr*   m_rtcEngineMgr;    // ���������ָ��
    CFont       m_ftTitle;              // ���ڱ�������
    CFont       m_ftLink;               // ����
    CFont       m_ftVer;                // �汾������
    SystemTray  m_sysTray;              // ϵͳ����
    HICON       m_trayIcon;             // ϵͳ����ͼ��
public:
    // �������ٴ���
    afx_msg void OnDestroy();
};
