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
����Ƶͨ��������
*/
#pragma once
#include "HWVideoWnd.h"
#include "HwRtcMsgDef.h"
#include "DeviceDlg.h"
#include "MediaInfoDlg.h"
// CVideoDlg �Ի���
#include "UserListDlg.h"

extern int g_rtcMaxWatchNum;


class CVideoDlg : public CDialogEx {
    DECLARE_DYNAMIC(CVideoDlg)

public:
    CVideoDlg(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~CVideoDlg();

    // �Ի�������
    enum { IDD = IDD_VIDEO_DIALOG };

    HWND GetLocalVideoWnd();                                // ��ȡ������Ƶ���ھ��

    void RebindVideoWnd();                                  // ���°���Ƶ����

    void ShowControlButton(BOOL bShow = TRUE);              // �Ƿ���ʾ���ư�ť
    void SetOwnUserId(const TCHAR * userId);                // �����Լ����û�ID
    BOOL IsTeacherUserId(const TCHAR * userId);             // �ж��û�ID�Ƿ��ǽ�ʦ
    void ShowLocalWnd();                                    // ��ʾ���ش���
    void SetRoomId(const string roomId);                    // ���÷���ID
    void ResetData();                                       // ��������
    /* ��ȡʵ��С��������ѡ�������� */
    int  GetWatchingCount() { return m_watchingCount; }
    void TurnOn(TCHAR* userid);                             // ��ʾ�û�����Ƶ
    void TurnOff(TCHAR* userid);                            // ����ʾ�û�����Ƶ
    void ResetWindow();                                     // ���ô���
    int GetWndSize();                                       // ��ȡ��Ƶ��������
protected:
    virtual void DoDataExchange(CDataExchange* pDX);        // DDX/DDV ֧��
    virtual BOOL OnInitDialog();                            // ��ʼ������
    virtual BOOL PreTranslateMessage(MSG* pMsg);            // ��������

    afx_msg void OnMouseMove(UINT nFlags, CPoint point);    // ����ƶ�����
    afx_msg void OnSize(UINT nType, int cx, int cy);        // ���ڴ�С�仯����
    afx_msg void OnPaint();                                 // ���ƴ���
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);    // ��ʾ����
    afx_msg void OnMove(int x, int y);                      // �����ƶ�

    afx_msg LRESULT OnNcHitTest(CPoint point);              // ����ƶ�����

    afx_msg void OnBnClickedBtnmin();                       // ��С����ť����
    afx_msg void OnBnClickedBtnclose();                     // �رհ�ť����
    afx_msg void OnBnClickedBtnrest();                      // ��󻯻�ԭ����

    afx_msg void OnBnClickedUserList();                     // �û��б�ť����
    afx_msg void OnBnClickedMore();                         // ���ð�ť����
    afx_msg void OnBnClickedBtnMicrophone();                // ��˷簴ť����
    afx_msg void OnBnClickedBtnPlayback();                  // ��������ť����
    afx_msg void OnBnClickedBtnCamera();                    // ����ͷ��ť����
    afx_msg void OnBnClickedBtnScreenShare();               // ������Ļ��ť����

    afx_msg LRESULT OnUserJoined(WPARAM wParam, LPARAM lParam);                 // Զ���û����뷿�䴦��
    afx_msg LRESULT OnJoinFailure(WPARAM wParam, LPARAM lParam);                // ���뷿��ʧ�ܻص�����
    afx_msg LRESULT OnJoinRoomSuccess(WPARAM wParam, LPARAM lParam);            // �ɹ����뷿�䴦��
    afx_msg LRESULT OnUserOffline(WPARAM wParam, LPARAM lParam);                // Զ���û��뿪��ǰ���䴦��
    afx_msg LRESULT OnLeaveRoom(WPARAM wParam, LPARAM lParam);                  // �뿪���䴦��
    afx_msg LRESULT OnDeviceStateChanged(WPARAM wParam, LPARAM lParam);         // �豸״̬�����仯����
    afx_msg LRESULT OnDeviceVolumeChanged(WPARAM wParam, LPARAM lParam);        // ��Ƶ�豸���������仯����
    afx_msg LRESULT OnRemoteVideoStat(WPARAM wParam, LPARAM lParam);            // Զ����Ƶ��ͳ�Ʊ仯����
    afx_msg LRESULT OnDefaultWatch(WPARAM wParam, LPARAM lParam);               // Ĭ�Ϲۿ�����
    afx_msg LRESULT OnUserRoleChanged(WPARAM wParam, LPARAM lParam);            // ��ɫ�л��ɹ�����
    afx_msg LRESULT OnShowNotice(WPARAM wParam, LPARAM lParam);                 // ��ʾ��ʾ��Ϣ
    afx_msg LRESULT OnDeviceSetChangedType(WPARAM wParam, LPARAM lParam);       // �豸���øı䴦��
    afx_msg LRESULT OnVideoInfoChanged(WPARAM wParam, LPARAM lParam);           // ��Ƶ��ͳ����Ϣ�仯����
    afx_msg LRESULT OnRoleChangeFailure(WPARAM wParam, LPARAM lParam);          // ���ؽ�ɫ�л�ʧ�ܴ���
    afx_msg LRESULT OnRoomRoleChangeFailure(WPARAM wParam, LPARAM lParam);      // �緽�Ľ�ɫ�л�ʧ�ܴ���
    afx_msg LRESULT OnAudioInfoChanged(WPARAM wParam, LPARAM lParam);           // ��Ƶ��ͳ����Ϣ�仯����
    afx_msg LRESULT OnScreenSharedInfoChanged(WPARAM wParam, LPARAM lParam);    // ������ͳ����Ϣ�仯����
    afx_msg LRESULT OnScreenSharedStarted(WPARAM wParam, LPARAM lParam);        // ������Ļ�����ɹ�����
    afx_msg LRESULT OnScreenSharedStoped(WPARAM wParam, LPARAM lParam);         // ������Ļ��������
    afx_msg LRESULT OnScreenSharedAvailable(WPARAM wParam, LPARAM lParam);      // Զ�˿���������
    DECLARE_MESSAGE_MAP()

protected:
    void DrawHead(CDC *pDC);                            // ���ƴ��ڱ�����
    void InitCtrls();                                   // ��ʼ���ؼ�
    void DisplayLayout();                               // ��ʾ���ڲ���
    void ShowButtonsNormal();                           // ���ð�ť״̬
    void AdjustButtonsDistance(int cx, int cy);         // ������ť���
    void AdjustSizeForLayout(int cx = 0, int cy = 0);   // �������ڲ���
    void SwitchData();                                  // �л����水ť״̬


private:
    TCHAR           m_ownUserId[128];                   // �����û�ID
    string          m_roomId;                           // ����ID
    CBrush          m_brHead;                           // ��������ˢ

    CMFCButton        m_btnMin;                         // ��С����ť
    CMFCButton        m_btnRst;                         // ��󻯰�ť
    CMFCButton        m_btnClose;                       // �رհ�ť
    CButton           m_btnUserList;                    // �û��б�ť
    CButton           m_btnMore;                        // ���ð�ť
    CButton           m_btnEndCall;                     // �Ҷϰ�ť
    CButton           m_btnMicrophone;                  // ��˷翪�ذ�ť
    CButton           m_btnPlayback;                    // ���������ذ�ť
    CButton           m_btnCamera;                      // ����ͷ���ذ�ť
    CButton           m_btnScreenShare;                 // ������Ļ��ť
    CComboBox         m_cbxUserRole;                    // �û���ɫ�л�����̨��̨��������

    CHWVideoWnd     m_wndStudentVideo[MAX_WATCH_NUM];   // ѧ����Ƶ��������
    CHWInfoWnd      m_wndStudentInfo[MAX_WATCH_NUM];    // ѧ����Ϣ��������
    CHWVideoWnd     m_wndTeacherVideo;                  // ��ʦ��Ƶ����
    CHWInfoWnd      m_wndTeacherInfo;                   // ��ʦ��Ϣ����
    CHWVideoWnd     m_wndShareVideo;                    // ������Ƶ����

    CRect           m_rcShareArea;                      // ����������
    CRect           m_rcTeacherArea;                    // ��ʦ��������

    CUserListDlg    m_dlgUserList;                      // �û��б��Ӵ���
    CDeviceDlg      m_dlgDevice;                        // �豸�Ӵ���
    CMediaInfoDlg   m_dlgMediaInfo;                     // ý������Ϣ�Ӵ���
    bool            m_bRecordingClosed;                 // ��˷��Ƿ���
    bool            m_bVideoClosed;                     // ����ͷ�Ƿ�ر�
    bool            m_bPlaybackClosed;                  // �������Ƿ�ر�
    bool            m_bScreenShared;                    // �������Ƿ���
    TCHAR           m_teacherUserId[128];               // ��ʦ�û�ID
private:	// data	
    /* ��Ƶ������Ϣ */
    typedef struct _HWVIDEO_WNDINFO {
        TCHAR   userId[128];                            // �û�ID
        BOOL    bWatch;                                 // �Ƿ�ѡ��
        BOOL    bShare;                                 // �Ƿ�����
        BOOL    bLocal;                                 // �Ƿ񱾵���Ƶ

    } HWVIDEO_WNDINFO, *PAGVIDEO_WNDINFO, *LPAGVIDEO_WNDINFO;

    CList<HWVIDEO_WNDINFO>  m_listWndInfo;              // ��Ƶ������Ϣ�б�
    HWVIDEO_WNDINFO         m_shareWndInfo;             // ������������Ϣ
    HWVIDEO_WNDINFO         m_teacherWndInfo;           // ��ʦ��Ƶ������Ϣ
    int                     m_watchingCount;            // ʵ��С��������ѡ�������������������ػ���
    BOOL                    m_bFullScreen;              // �Ƿ�ȫ��

public:
    afx_msg void OnDestroy();                           // �����ͷ�ǰ����
    afx_msg void OnClose();                             // ���ڹرմ���
    void OnSelComChange();                              // �û���ɫ������ѡ��仯����
    afx_msg void OnIdrSetdevice();                      // �����豸�˵�����
    afx_msg void OnIdrShowinfo();                       // ��ʾ��Ϣ�˵�����
};