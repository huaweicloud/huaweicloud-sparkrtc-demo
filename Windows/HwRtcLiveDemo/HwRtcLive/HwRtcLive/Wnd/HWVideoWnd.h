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
视频窗口类
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
    /* 设置用户ID */
    void ShowUserInfo(const TCHAR * userId);
protected:
    /* 绘制窗口 */
    afx_msg void OnPaint();
    /* 擦除窗口背景 */
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

    DECLARE_MESSAGE_MAP()

private:
    BOOL        m_bIsShowUser;  // 是否显示窗口
    COLORREF    m_colorBack;    // 窗口背景颜色

    TCHAR       m_userId[128];  // 用户ID
    CBrush      m_brushBack;    // 背景画刷
};


class CHWVideoWnd : public CStatic {
    DECLARE_DYNAMIC(CHWVideoWnd)

public:
    CHWVideoWnd();
    virtual ~CHWVideoWnd();
    /* 设置用户ID */
    void SetUserId(TCHAR* userId);
    /* 获取用户ID */
    const TCHAR* GetUserId();
    /* 获取用户昵称 */
    const TCHAR* GetUserNickName();
    /* 设置窗口颜色 */
    void SetFaceColor(COLORREF colorBack);
    /* 设置窗口背景图片 */
    BOOL SetBackImage(UINT imgId, UINT width, UINT height, COLORREF colorRef = HRTC_WHITE);

    /* 设置是否显示信息窗口 */
    void ShowVideoInfo(BOOL bShow);
    /* 获取是否显示信息窗口 */
    BOOL IsVideoInfoShowed() { return m_bIsShowInfo; };
    /* 设置是否是大窗口标志 */
    void SetBigShowFlag(BOOL bIsBig);
    /* 获取是否是大窗口标志 */
    BOOL IsBigShow() { return m_bIsBig; };
    /* 刷新窗口 */
    void RefreshWnd();

protected:
    /* 创建窗口 */
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    /* 清除窗口背景 */
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    /* 窗口大小变化处理 */
    afx_msg void OnSize(UINT nType, int cx, int cy);

    DECLARE_MESSAGE_MAP()

private:
    CImageList  m_backImages;           // 背景图片列表
    COLORREF    m_colorBack;            // 背景颜色
    CHWInfoWnd  m_windUser;             // 信息窗口，显示用户昵称
private:
    TCHAR       m_userId[128];          // 用户ID
    TCHAR       m_userName[128];        // 用户昵称
    BOOL        m_bIsShowInfo;          // 是否显示信息窗口
    BOOL        m_bIsBig;               // 是否是大窗口
};