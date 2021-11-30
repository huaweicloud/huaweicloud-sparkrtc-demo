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
房间外设置音视频设备页面类
*/
#pragma once
#include "HWVideoWnd.h"
#include "IHRTCEngine.h"
#include "afxwin.h"
using namespace huawei::rtc;

class CDeviceDlgNew : public CDialogEx {
    DECLARE_DYNAMIC(CDeviceDlgNew)

public:
    CDeviceDlgNew(CWnd* pParent = NULL);
    virtual ~CDeviceDlgNew();

    enum { IDD = IDD_DEVICE_DIALOG_NEW };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);        // 窗口控件数据交换
    virtual BOOL OnInitDialog();                            // 初始化窗口
    virtual BOOL PreTranslateMessage(MSG* pMsg);            // 处理按键消息

    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);    // 窗口显示状态变化处理

    afx_msg void OnBnClickedBtnCancel();    // 取消按钮处理
    afx_msg void OnBnClickedBtnConfirm();   // 确定按钮处理
    afx_msg void OnBnClickedBtnApply();     // 应用按钮处理
    DECLARE_MESSAGE_MAP()

protected:
    void InitCtrls();                       // 初始化控件
    void InitCtrlsFont();                   // 初始化控件字体
    LRESULT OnNcHitTest(CPoint point);      // 鼠标移动处理

private:
    CFont           m_ftLink;               // 链接字体
    CFont           m_ftDes;                // 控件描述文字字体
    CFont           m_ftBtn;                // 按钮字体

    CComboBox       m_audioInput;           // 音频输入设备下拉框
    CComboBox       m_audioOut;             // 音频输出设备下拉框
    CComboBox       m_camera;               // 摄像头下拉框
    CMFCButton      m_btnClose;             // 关闭按钮
    CSliderCtrl     m_slideAudioInputVol;   // 音频输入音量滑块
    CSliderCtrl     m_slideAudioOutputVol;  // 音频输出音量滑块

    CMFCButton      m_btnCancel;            // 取消按钮
    CMFCButton      m_btnConfirm;           // 确认按钮
    CMFCButton      m_btnApply;             // 应用按钮
    HRTCDeviceInfo  m_videoInfo[10];        // 摄像头信息数组
    HRTCDeviceInfo  m_audioInInfo[10];      // 音频输入设备信息数组
    HRTCDeviceInfo  m_audioOutInfo[10];     // 音频输出设备信息数组

    CHWVideoWnd     m_preViewWnd;           // 预览窗口句柄
private:
    IHRTCEngine     *m_lpRtcEngine;         // 引擎指针

public:
    /* 刷新界面的设备音量 */
    void RefreshDeviceVolumeChange(HRTCDeviceType deviceType, unsigned int mute, unsigned int type);
    /* 刷新界面的音视频设备 */
    void RefreshDeviceStateChange(char* deviceId, HRTCDeviceType deviceType, HRTCDeviceState state);
};
