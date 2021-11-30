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
房间内设置音视频设备页面类
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
    virtual void DoDataExchange(CDataExchange* pDX);        // 窗口控件数据交换
    virtual BOOL OnInitDialog();                            // 初始化窗口
    virtual BOOL PreTranslateMessage(MSG* pMsg);            // 处理按键消息

    afx_msg void OnPaint();                                 // 绘制窗口
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);    // 窗口显示状态变化处理


    afx_msg void OnBnClickedBtnCancel();    // 取消按钮处理
    afx_msg void OnBnClickedBtnConfirm();   // 确定按钮处理
    afx_msg void OnBnClickedBtnApply();     // 应用按钮处理
    DECLARE_MESSAGE_MAP()

protected:
    void InitCtrls();                       // 初始化控件
    void DrawClient(CDC *lpDC);             // 绘制客户区
    LRESULT OnNcHitTest(CPoint point);      // 鼠标移动处理

private:
    CFont           m_ftLink;               // 链接字体
    CFont           m_ftDes;                // 控件描述文字字体
    CFont           m_ftBtn;                // 按钮字体

    CPen            m_penFrame;             // 框架画笔
    CComboBox       m_audioInput;           // 音频输入设备下拉框
    CComboBox       m_audioOut;             // 音频输出设备下拉框
    CComboBox       m_camera;               // 摄像头下拉框
    CMFCButton      m_btnClose;             // 关闭按钮
    CSliderCtrl     m_slideAudioInputVol;   // 音频输入音量滑块
    CSliderCtrl     m_slideAudioOutputVol;  // 音频输出音量滑块

    CSliderCtrl     m_slideAInSVol;         // 音频输入软音量滑块
    CSliderCtrl     m_slideAOutSVol;        // 音频输入软音量滑块
    CMFCButton      m_btnCancel;            // 取消按钮
    CMFCButton      m_btnConfirm;           // 确认按钮
    CMFCButton      m_btnApply;             // 应用按钮
    HRTCDeviceInfo  m_videoInfo[10];        // 摄像头信息数组
    HRTCDeviceInfo  m_audioInInfo[10];      // 音频输入设备信息数组
    HRTCDeviceInfo  m_audioOutInfo[10];     // 音频输出设备信息数组
    CComboBox       m_cbxBigStream;         // 大流分辨率下拉框
    HRTCStreamType  m_bigStreamType;        // 大流清晰度类型
    int             m_softInVolume;         // 音频输入软音量
    int             m_softOutVolume;        // 音频输出软音量
    BOOL            m_bRemoteStreamAdaptive;// 远端流是否自适应
private:
    IHRTCEngine     *m_lpRtcEngine;         // 引擎指针

public:
    /* 刷新界面的设备音量 */
    void RefreshDeviceVolumeChange(HRTCDeviceType deviceType, unsigned int mute, unsigned int type);
    /* 刷新界面的音视频设备 */
    void RefreshDeviceStateChange(const char* deviceId, HRTCDeviceType deviceType, HRTCDeviceState state);
    /* 获取大流清晰度类型 */
    HRTCStreamType GetBigStreamType();
    /* 获取音频输入软音量 */
    int GetSoftInVolume() { return m_softInVolume; };
    /* 获取音频输出软音量 */
    int GetSoftOutVolume() { return m_softOutVolume; };
    /* 获取远端流是否自适应 */
    bool isRemoteStreamAdaptive();
};
