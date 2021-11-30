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
ý������Ϣҳ��
*/
#pragma once
#include "HwRtcMsgDef.h"
#include "IHRTCEngine.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "afxwin.h"
using namespace huawei::rtc;
// MediaInfo �Ի���

class CMediaInfoDlg : public CDialogEx {
    DECLARE_DYNAMIC(CMediaInfoDlg)

public:
    CMediaInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~CMediaInfoDlg();
    afx_msg void OnBnClickedInfoClose();    // �رմ��ڰ�ť����
    /* ������Ƶ����Ϣ */
    void UpdateVideoInfo(list<HRTCLocalVideoStats> *localStatsList, list<HRTCRemoteVideoStats> *remoteStatsList, bool needSave);
    /* ������Ƶ����Ϣ */
    void UpdateAudioInfo(list<HRTCLocalAudioStats> *localStatsList, list<HRTCRemoteAudioStats> *remoteStatsList, bool needSave);
    void UpdateScreenSharedInfo(ScreenShareStats *stats, bool needSave);    // ���¹�������Ϣ
    void SetShareVisible(bool isVisible);   // �����Ƿ���ʾ��������Ϣ
    void ClearData();                       // ���ҳ����Ϣ
    enum { IDD = IDD_MEDIA_INFO };


protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    virtual BOOL OnInitDialog();

    afx_msg void OnPaint();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    DECLARE_MESSAGE_MAP()

protected:
    void InitCtrls();
    void DrawClient(CDC *lpDC);
    LRESULT OnNcHitTest(CPoint point);

private:
    CMFCButton m_btnClose;  // �رհ�ť

    CListCtrl m_videoList;  // ��Ƶ��ͳ����Ϣ�б�ؼ�

    CListCtrl m_audioList;  // ��Ƶ��ͳ����Ϣ�б�ؼ�
    CListCtrl m_sharedList; // ������ͳ����Ϣ�б�ؼ�
    list<HRTCLocalVideoStats> m_localVideoStatsList;    // ������Ƶͳ����Ϣ�б�
    list<HRTCRemoteVideoStats> m_remoteVideoStatsList;  // Զ����Ƶͳ����Ϣ�б�
    list<HRTCLocalAudioStats> m_localAudioStatsList;    // ������Ƶͳ����Ϣ�б�
    list<HRTCRemoteAudioStats> m_remoteAudioStatsList;  // Զ����Ƶͳ����Ϣ�б�
    ScreenShareStats m_shareStats;  // ������ͳ����Ϣ
    bool m_saveShare = FALSE;       // �Ƿ񱣴湲����ͳ����Ϣ
    bool m_isShareVisible = FALSE;  // �Ƿ���ʾ������ͳ����Ϣ
};