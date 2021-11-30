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
��Ƶ������
*/
#pragma once

#define WINDOW_INFO_WIDTH 192
#define WINDOW_INFO_HEIGHT 24
#define WINDOW_INFO_Y_DELTA 4

class CHWInfoWnd : public CWnd {
    DECLARE_DYNAMIC(CHWInfoWnd)

public:
    CHWInfoWnd();
    virtual ~CHWInfoWnd();
    /* �����û�ID */
    void ShowUserInfo(const TCHAR * userId);
protected:
    /* ���ƴ��� */
    afx_msg void OnPaint();
    /* �������ڱ��� */
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

    DECLARE_MESSAGE_MAP()

private:
    BOOL        m_bIsShowUser;  // �Ƿ���ʾ����
    COLORREF    m_colorBack;    // ���ڱ�����ɫ

    TCHAR       m_userId[128];  // �û�ID
    CBrush      m_brushBack;    // ������ˢ
};


class CHWVideoWnd : public CStatic {
    DECLARE_DYNAMIC(CHWVideoWnd)

public:
    CHWVideoWnd();
    virtual ~CHWVideoWnd();
    /* �����û�ID */
    void SetUserId(TCHAR* userId);
    /* ��ȡ�û�ID */
    const TCHAR* GetUserId();
    /* ��ȡ�û��ǳ� */
    const TCHAR* GetUserNickName();
    /* ���ô�����ɫ */
    void SetFaceColor(COLORREF colorBack);
    /* ���ô��ڱ���ͼƬ */
    BOOL SetBackImage(UINT imgId, UINT width, UINT height, COLORREF colorRef = HRTC_WHITE);

    /* �����Ƿ���ʾ��Ϣ���� */
    void ShowVideoInfo(BOOL bShow);
    /* ��ȡ�Ƿ���ʾ��Ϣ���� */
    BOOL IsVideoInfoShowed() { return m_bIsShowInfo; };
    /* �����Ƿ��Ǵ󴰿ڱ�־ */
    void SetBigShowFlag(BOOL bIsBig);
    /* ��ȡ�Ƿ��Ǵ󴰿ڱ�־ */
    BOOL IsBigShow() { return m_bIsBig; };
    /* ˢ�´��� */
    void RefreshWnd();

protected:
    /* �������� */
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    /* ������ڱ��� */
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    /* ���ڴ�С�仯���� */
    afx_msg void OnSize(UINT nType, int cx, int cy);

    DECLARE_MESSAGE_MAP()

private:
    CImageList  m_backImages;           // ����ͼƬ�б�
    COLORREF    m_colorBack;            // ������ɫ
    CHWInfoWnd  m_windUser;             // ��Ϣ���ڣ���ʾ�û��ǳ�
private:
    TCHAR       m_userId[128];          // �û�ID
    TCHAR       m_userName[128];        // �û��ǳ�
    BOOL        m_bIsShowInfo;          // �Ƿ���ʾ��Ϣ����
    BOOL        m_bIsBig;               // �Ƿ��Ǵ󴰿�
};