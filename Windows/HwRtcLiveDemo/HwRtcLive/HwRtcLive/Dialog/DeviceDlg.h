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
��������������Ƶ�豸ҳ����
*/
#pragma once
#include "HWVideoWnd.h"
#include "IHRTCEngine.h"
#include "afxwin.h"
using namespace huawei::rtc;


class CDeviceDlg : public CDialogEx {
    DECLARE_DYNAMIC(CDeviceDlg)

public:
    CDeviceDlg(CWnd* pParent = NULL);
    virtual ~CDeviceDlg();

    enum { IDD = IDD_DEVICE_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);        // ���ڿؼ����ݽ���
    virtual BOOL OnInitDialog();                            // ��ʼ������
    virtual BOOL PreTranslateMessage(MSG* pMsg);            // ��������Ϣ

    afx_msg void OnPaint();                                 // ���ƴ���
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);    // ������ʾ״̬�仯����


    afx_msg void OnBnClickedBtnCancel();    // ȡ����ť����
    afx_msg void OnBnClickedBtnConfirm();   // ȷ����ť����
    afx_msg void OnBnClickedBtnApply();     // Ӧ�ð�ť����
    DECLARE_MESSAGE_MAP()

protected:
    void InitCtrls();                       // ��ʼ���ؼ�
    void DrawClient(CDC *lpDC);             // ���ƿͻ���
    LRESULT OnNcHitTest(CPoint point);      // ����ƶ�����

private:
    CFont           m_ftLink;               // ��������
    CFont           m_ftDes;                // �ؼ�������������
    CFont           m_ftBtn;                // ��ť����

    CPen            m_penFrame;             // ��ܻ���
    CComboBox       m_audioInput;           // ��Ƶ�����豸������
    CComboBox       m_audioOut;             // ��Ƶ����豸������
    CComboBox       m_camera;               // ����ͷ������
    CMFCButton      m_btnClose;             // �رհ�ť
    CSliderCtrl     m_slideAudioInputVol;   // ��Ƶ������������
    CSliderCtrl     m_slideAudioOutputVol;  // ��Ƶ�����������

    CSliderCtrl     m_slideAInSVol;         // ��Ƶ��������������
    CSliderCtrl     m_slideAOutSVol;        // ��Ƶ��������������
    CMFCButton      m_btnCancel;            // ȡ����ť
    CMFCButton      m_btnConfirm;           // ȷ�ϰ�ť
    CMFCButton      m_btnApply;             // Ӧ�ð�ť
    HRTCDeviceInfo  m_videoInfo[10];        // ����ͷ��Ϣ����
    HRTCDeviceInfo  m_audioInInfo[10];      // ��Ƶ�����豸��Ϣ����
    HRTCDeviceInfo  m_audioOutInfo[10];     // ��Ƶ����豸��Ϣ����
    CComboBox       m_cbxBigStream;         // �����ֱ���������
    HRTCStreamType  m_bigStreamType;        // ��������������
    int             m_softInVolume;         // ��Ƶ����������
    int             m_softOutVolume;        // ��Ƶ���������
    BOOL            m_bRemoteStreamAdaptive;// Զ�����Ƿ�����Ӧ
private:
    IHRTCEngine     *m_lpRtcEngine;         // ����ָ��

public:
    /* ˢ�½�����豸���� */
    void RefreshDeviceVolumeChange(HRTCDeviceType deviceType, unsigned int mute, unsigned int type);
    /* ˢ�½��������Ƶ�豸 */
    void RefreshDeviceStateChange(const char* deviceId, HRTCDeviceType deviceType, HRTCDeviceState state);
    /* ��ȡ�������������� */
    HRTCStreamType GetBigStreamType();
    /* ��ȡ��Ƶ���������� */
    int GetSoftInVolume() { return m_softInVolume; };
    /* ��ȡ��Ƶ��������� */
    int GetSoftOutVolume() { return m_softOutVolume; };
    /* ��ȡԶ�����Ƿ�����Ӧ */
    bool isRemoteStreamAdaptive();
};
