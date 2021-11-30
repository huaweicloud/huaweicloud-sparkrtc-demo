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
// SystemTray.h 
//
/*
自定义系统托盘图标控件类
*/
#pragma once

// wrapper class for NOTIFYICONDATA & Shell_NotifyIcon
//
class SystemTray {
public:
    SystemTray();
    ~SystemTray(void);

public:
    void Init(HWND hWnd, UINT uID);
    BOOL SetIcon(HICON hIcon);
    BOOL SetCallbackMessage(UINT uCallbackMessage);
    BOOL SetTip(const CString & tip);
    BOOL SetInfo(const CString & info);
    BOOL Disable();

private:
    BOOL ShellNotifyIcon(DWORD dwMessage);
    BOOL NotifyAdd();
    BOOL NotifyDelete();
    BOOL NotifyUpdate();

private:
    NOTIFYICONDATA nid_;

}; // class SystemTray
