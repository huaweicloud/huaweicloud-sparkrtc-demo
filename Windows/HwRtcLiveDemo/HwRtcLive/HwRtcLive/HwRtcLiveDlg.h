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
主窗口类
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
    // 加入房间
    afx_msg LRESULT OnJoinRoom(WPARAM wParam, LPARAM lParam);
    // 退出房间
    afx_msg LRESULT OnLeaveRoom(WPARAM wParam, LPARAM lParam);
    // 进入登录界面
    afx_msg LRESULT OnEnterLogin(WPARAM wParam, LPARAM lParam);
    // 进入创建房间页面
    afx_msg LRESULT OnCreateRoom(WPARAM wParam, LPARAM lParam);
    //回到登录页面
    afx_msg LRESULT OnReturn(WPARAM wParam, LPARAM lParam);
    // 显示短时提示消息
    afx_msg LRESULT OnShowToast(WPARAM wParam, LPARAM lParam);
    // 显示提示信息
    afx_msg LRESULT OnShowNotice(WPARAM wParam, LPARAM lParam);
    // 修改用户角色
    afx_msg LRESULT OnChangeRoleRadio(WPARAM wParam, LPARAM lParam);
    // 系统托盘操作通知
    afx_msg LRESULT OnSysTrayNotify(WPARAM wParam, LPARAM lParam);
    // 最小化按钮处理
    afx_msg void OnBnClickedBtnmin();
    // 关闭按钮处理
    afx_msg void OnBnClickedBtnclose();
    // 窗口拖动处理
    afx_msg LRESULT OnNcHitTest(CPoint point);
    DECLARE_MESSAGE_MAP()
    // 创建系统托盘菜单
    void CreateLoginMenu();
    // 上传日志
    void onMenuUploadLog();
protected:
    // 初始化控件
    void InitCtrls();
    // 绘制客户区
    void DrawClient(CDC *lpDC);
    // 初始化子窗口
    void InitChildDialog();
    // 处理按键消息
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    // 设置编码参数
    void SetEncParam();
private:
    CMFCButton       m_btnMin;           // 最小化按钮
    CButton       m_btnClose;         // 关闭按钮
    CButton       m_btnSetting;       // 设置按钮
    CEnterRoomDlg   m_dlgEnterRoom;     // 加入房间窗口
    CCreateDlg      m_dlgCreate;        // 创建房间窗口
    CTypeEnterDlg   m_dlgTypeEnter;     // 大班课模式选择窗口
    CVideoDlg       m_dlgVideo;         // 音视频通话窗口
    CToastDlg       m_toastWnd;         // 短时通知窗口
    CRtcEngineMgr*   m_rtcEngineMgr;    // 引擎管理类指针
    CFont       m_ftTitle;              // 窗口标题字体
    CFont       m_ftLink;               // 废弃
    CFont       m_ftVer;                // 版本号字体
    SystemTray  m_sysTray;              // 系统托盘
    HICON       m_trayIcon;             // 系统托盘图标
public:
    // 窗口销毁处理
    afx_msg void OnDestroy();
};
