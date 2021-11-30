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
// HwRtcLive.h : main header file for the PROJECT_NAME application
//
/*
主程序类
*/
#pragma once

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "HwCatchDump.h"
#include "RDLOG.h"
using namespace HwDumpCatch;
// CHwRtcLiveApp:
// See HwRtcLive.cpp for the implementation of this class


class CHwRtcLiveApp : public CWinApp {
public:
    CHwRtcLiveApp();

    // Overrides
public:
    bool m_bLogout;                             // 程序重启标志
    virtual BOOL InitInstance();                // 程序初始化
    GdiplusStartupInput gdiplusStartupInput;    // GDI+启动参数
    ULONG_PTR           gdiplusToken;           // GDI+被初始化后在应用程序中的GDI+标识
    DECLARE_MESSAGE_MAP()
    virtual int ExitInstance();                 // 程序退出处理
    bool StartProcess(LPCTSTR strPath, LPCTSTR strCommand, int nShowType); // 重启程序
private:
    CHwCrashDumpCatch m_dumpCatch;              // 异常捕获
};

extern CHwRtcLiveApp theApp;                    // 主程序对象