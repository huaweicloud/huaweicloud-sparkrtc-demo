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
媒体流信息页面
*/
#pragma once
#include "HwRtcMsgDef.h"
#include "IHRTCEngine.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "afxwin.h"
using namespace huawei::rtc;
// MediaInfo 对话框

class CMediaInfoDlg : public CDialogEx {
    DECLARE_DYNAMIC(CMediaInfoDlg)

public:
    CMediaInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CMediaInfoDlg();
    afx_msg void OnBnClickedInfoClose();    // 关闭窗口按钮处理
    /* 更新视频流信息 */
    void UpdateVideoInfo(list<HRTCLocalVideoStats> *localStatsList, list<HRTCRemoteVideoStats> *remoteStatsList, bool needSave);
    /* 更新音频流信息 */
    void UpdateAudioInfo(list<HRTCLocalAudioStats> *localStatsList, list<HRTCRemoteAudioStats> *remoteStatsList, bool needSave);
    void UpdateScreenSharedInfo(ScreenShareStats *stats, bool needSave);    // 更新共享流信息
    void SetShareVisible(bool isVisible);   // 设置是否显示共享流信息
    void ClearData();                       // 清空页面信息
    enum { IDD = IDD_MEDIA_INFO };


protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL OnInitDialog();

    afx_msg void OnPaint();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    DECLARE_MESSAGE_MAP()

protected:
    void InitCtrls();
    void DrawClient(CDC *lpDC);
    LRESULT OnNcHitTest(CPoint point);

private:
    CMFCButton m_btnClose;  // 关闭按钮

    CListCtrl m_videoList;  // 视频流统计信息列表控件

    CListCtrl m_audioList;  // 音频流统计信息列表控件
    CListCtrl m_sharedList; // 共享流统计信息列表控件
    list<HRTCLocalVideoStats> m_localVideoStatsList;    // 本地视频统计信息列表
    list<HRTCRemoteVideoStats> m_remoteVideoStatsList;  // 远端视频统计信息列表
    list<HRTCLocalAudioStats> m_localAudioStatsList;    // 本地音频统计信息列表
    list<HRTCRemoteAudioStats> m_remoteAudioStatsList;  // 远端音频统计信息列表
    ScreenShareStats m_shareStats;  // 共享流统计信息
    bool m_saveShare = FALSE;       // 是否保存共享流统计信息
    bool m_isShareVisible = FALSE;  // 是否显示共享流统计信息
};