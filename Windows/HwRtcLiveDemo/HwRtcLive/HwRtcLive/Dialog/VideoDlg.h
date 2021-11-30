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
音视频通话窗口类
*/
#pragma once
#include "HWVideoWnd.h"
#include "HwRtcMsgDef.h"
#include "DeviceDlg.h"
#include "MediaInfoDlg.h"
// CVideoDlg 对话框
#include "UserListDlg.h"

extern int g_rtcMaxWatchNum;


class CVideoDlg : public CDialogEx {
    DECLARE_DYNAMIC(CVideoDlg)

public:
    CVideoDlg(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CVideoDlg();

    // 对话框数据
    enum { IDD = IDD_VIDEO_DIALOG };

    HWND GetLocalVideoWnd();                                // 获取本地视频窗口句柄

    void RebindVideoWnd();                                  // 重新绑定视频窗口

    void ShowControlButton(BOOL bShow = TRUE);              // 是否显示控制按钮
    void SetOwnUserId(const TCHAR * userId);                // 设置自己的用户ID
    BOOL IsTeacherUserId(const TCHAR * userId);             // 判断用户ID是否是教师
    void ShowLocalWnd();                                    // 显示本地窗口
    void SetRoomId(const string roomId);                    // 设置房间ID
    void ResetData();                                       // 重置数据
    /* 获取实际小画面正在选看的数量 */
    int  GetWatchingCount() { return m_watchingCount; }
    void TurnOn(TCHAR* userid);                             // 显示用户的视频
    void TurnOff(TCHAR* userid);                            // 不显示用户的视频
    void ResetWindow();                                     // 重置窗口
    int GetWndSize();                                       // 获取视频窗口数量
protected:
    virtual void DoDataExchange(CDataExchange* pDX);        // DDX/DDV 支持
    virtual BOOL OnInitDialog();                            // 初始化窗口
    virtual BOOL PreTranslateMessage(MSG* pMsg);            // 按键处理

    afx_msg void OnMouseMove(UINT nFlags, CPoint point);    // 鼠标移动处理
    afx_msg void OnSize(UINT nType, int cx, int cy);        // 窗口大小变化处理
    afx_msg void OnPaint();                                 // 绘制窗口
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);    // 显示窗口
    afx_msg void OnMove(int x, int y);                      // 窗口移动

    afx_msg LRESULT OnNcHitTest(CPoint point);              // 鼠标移动处理

    afx_msg void OnBnClickedBtnmin();                       // 最小化按钮处理
    afx_msg void OnBnClickedBtnclose();                     // 关闭按钮处理
    afx_msg void OnBnClickedBtnrest();                      // 最大化或还原处理

    afx_msg void OnBnClickedUserList();                     // 用户列表按钮处理
    afx_msg void OnBnClickedMore();                         // 设置按钮处理
    afx_msg void OnBnClickedBtnMicrophone();                // 麦克风按钮处理
    afx_msg void OnBnClickedBtnPlayback();                  // 扬声器按钮处理
    afx_msg void OnBnClickedBtnCamera();                    // 摄像头按钮处理
    afx_msg void OnBnClickedBtnScreenShare();               // 共享屏幕按钮处理

    afx_msg LRESULT OnUserJoined(WPARAM wParam, LPARAM lParam);                 // 远端用户加入房间处理
    afx_msg LRESULT OnJoinFailure(WPARAM wParam, LPARAM lParam);                // 加入房间失败回调处理
    afx_msg LRESULT OnJoinRoomSuccess(WPARAM wParam, LPARAM lParam);            // 成功加入房间处理
    afx_msg LRESULT OnUserOffline(WPARAM wParam, LPARAM lParam);                // 远端用户离开当前房间处理
    afx_msg LRESULT OnLeaveRoom(WPARAM wParam, LPARAM lParam);                  // 离开房间处理
    afx_msg LRESULT OnDeviceStateChanged(WPARAM wParam, LPARAM lParam);         // 设备状态发生变化处理
    afx_msg LRESULT OnDeviceVolumeChanged(WPARAM wParam, LPARAM lParam);        // 音频设备音量发生变化处理
    afx_msg LRESULT OnRemoteVideoStat(WPARAM wParam, LPARAM lParam);            // 远端视频流统计变化处理
    afx_msg LRESULT OnDefaultWatch(WPARAM wParam, LPARAM lParam);               // 默认观看处理
    afx_msg LRESULT OnUserRoleChanged(WPARAM wParam, LPARAM lParam);            // 角色切换成功处理
    afx_msg LRESULT OnShowNotice(WPARAM wParam, LPARAM lParam);                 // 显示提示信息
    afx_msg LRESULT OnDeviceSetChangedType(WPARAM wParam, LPARAM lParam);       // 设备设置改变处理
    afx_msg LRESULT OnVideoInfoChanged(WPARAM wParam, LPARAM lParam);           // 视频流统计信息变化处理
    afx_msg LRESULT OnRoleChangeFailure(WPARAM wParam, LPARAM lParam);          // 本地角色切换失败处理
    afx_msg LRESULT OnRoomRoleChangeFailure(WPARAM wParam, LPARAM lParam);      // 跨方的角色切换失败处理
    afx_msg LRESULT OnAudioInfoChanged(WPARAM wParam, LPARAM lParam);           // 音频流统计信息变化处理
    afx_msg LRESULT OnScreenSharedInfoChanged(WPARAM wParam, LPARAM lParam);    // 共享流统计信息变化处理
    afx_msg LRESULT OnScreenSharedStarted(WPARAM wParam, LPARAM lParam);        // 共享屏幕开启成功处理
    afx_msg LRESULT OnScreenSharedStoped(WPARAM wParam, LPARAM lParam);         // 共享屏幕结束处理
    afx_msg LRESULT OnScreenSharedAvailable(WPARAM wParam, LPARAM lParam);      // 远端开启共享处理
    DECLARE_MESSAGE_MAP()

protected:
    void DrawHead(CDC *pDC);                            // 绘制窗口标题栏
    void InitCtrls();                                   // 初始化控件
    void DisplayLayout();                               // 显示窗口布局
    void ShowButtonsNormal();                           // 设置按钮状态
    void AdjustButtonsDistance(int cx, int cy);         // 调整按钮间距
    void AdjustSizeForLayout(int cx = 0, int cy = 0);   // 调整窗口布局
    void SwitchData();                                  // 切换界面按钮状态


private:
    TCHAR           m_ownUserId[128];                   // 本地用户ID
    string          m_roomId;                           // 房间ID
    CBrush          m_brHead;                           // 标题栏画刷

    CMFCButton        m_btnMin;                         // 最小化按钮
    CMFCButton        m_btnRst;                         // 最大化按钮
    CMFCButton        m_btnClose;                       // 关闭按钮
    CButton           m_btnUserList;                    // 用户列表按钮
    CButton           m_btnMore;                        // 设置按钮
    CButton           m_btnEndCall;                     // 挂断按钮
    CButton           m_btnMicrophone;                  // 麦克风开关按钮
    CButton           m_btnPlayback;                    // 扬声器开关按钮
    CButton           m_btnCamera;                      // 摄像头开关按钮
    CButton           m_btnScreenShare;                 // 共享屏幕按钮
    CComboBox         m_cbxUserRole;                    // 用户角色切换（上台下台）下拉框

    CHWVideoWnd     m_wndStudentVideo[MAX_WATCH_NUM];   // 学生视频窗口数组
    CHWInfoWnd      m_wndStudentInfo[MAX_WATCH_NUM];    // 学生信息窗口数组
    CHWVideoWnd     m_wndTeacherVideo;                  // 教师视频窗口
    CHWInfoWnd      m_wndTeacherInfo;                   // 教师信息窗口
    CHWVideoWnd     m_wndShareVideo;                    // 共享视频窗口

    CRect           m_rcShareArea;                      // 共享窗口区域
    CRect           m_rcTeacherArea;                    // 教师窗口区域

    CUserListDlg    m_dlgUserList;                      // 用户列表子窗口
    CDeviceDlg      m_dlgDevice;                        // 设备子窗口
    CMediaInfoDlg   m_dlgMediaInfo;                     // 媒体流信息子窗口
    bool            m_bRecordingClosed;                 // 麦克风是否静音
    bool            m_bVideoClosed;                     // 摄像头是否关闭
    bool            m_bPlaybackClosed;                  // 扬声器是否关闭
    bool            m_bScreenShared;                    // 共享流是否开启
    TCHAR           m_teacherUserId[128];               // 教师用户ID
private:	// data	
    /* 视频窗口信息 */
    typedef struct _HWVIDEO_WNDINFO {
        TCHAR   userId[128];                            // 用户ID
        BOOL    bWatch;                                 // 是否选看
        BOOL    bShare;                                 // 是否共享流
        BOOL    bLocal;                                 // 是否本地视频

    } HWVIDEO_WNDINFO, *PAGVIDEO_WNDINFO, *LPAGVIDEO_WNDINFO;

    CList<HWVIDEO_WNDINFO>  m_listWndInfo;              // 视频窗口信息列表
    HWVIDEO_WNDINFO         m_shareWndInfo;             // 共享流窗口信息
    HWVIDEO_WNDINFO         m_teacherWndInfo;           // 教师视频窗口信息
    int                     m_watchingCount;            // 实际小画面正在选看的数量，不包括本地画面
    BOOL                    m_bFullScreen;              // 是否全屏

public:
    afx_msg void OnDestroy();                           // 窗口释放前处理
    afx_msg void OnClose();                             // 窗口关闭处理
    void OnSelComChange();                              // 用户角色下拉框选择变化处理
    afx_msg void OnIdrSetdevice();                      // 设置设备菜单处理
    afx_msg void OnIdrShowinfo();                       // 显示信息菜单处理
};