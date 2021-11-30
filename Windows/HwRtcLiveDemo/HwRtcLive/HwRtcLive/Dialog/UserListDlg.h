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
// UserListDlg.h : ͷ�ļ�
/*
�û��б�ҳ��
*/
#pragma once

#include "resource.h"
#include "afxwin.h"
// CUserListDlg �Ի���
class CVideoDlg;
class CUserListDlg : public CDialogEx {
    //�ؼ�����
protected:
    CListBox        m_listUser;         // �û��б�
    CVideoDlg*      m_pVideoObject;     // ��Ƶ����ָ��
    //�ؼ�����

// ����
public:
    CUserListDlg(CWnd* pParent = NULL); // ��׼���캯��

// �Ի�������
    enum { IDD = IDD_USERLIST_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    // ���ɵ���Ϣӳ�亯��
    virtual BOOL OnInitDialog();        // ��ʼ������
    afx_msg void OnPaint();             // ���ƴ���
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);// �������ڱ���
    DECLARE_MESSAGE_MAP()

public:
    void AddUser(TCHAR *userId);                    // ��ӵ��û��б�
    void AddOwn(TCHAR *userId);                     // ����Լ����û��б�
    void DeleteUser(TCHAR *userId);                 // ɾ���û���Ϣ
    void UpdateUser(TCHAR *userId);                 // �����û���Ϣ
    void ClearData();                               // ����û��б�
    void RefreshListBox();                          // ���½����ϵ��û��б�
    void SetVideoObject(CVideoDlg * videoObject);   // ������Ƶ����ָ��
    bool IsExist(TCHAR *userId);                    // ����Ƿ������û��б���
    CStatic m_txtCount;
};
