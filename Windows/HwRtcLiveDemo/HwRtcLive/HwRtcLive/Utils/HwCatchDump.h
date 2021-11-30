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
异常捕获类
*/
#pragma once
#include "windows.h"
using namespace std;
#include <string>
namespace HwDumpCatch {
    /* 写日志*/
    void Hwlog(const std::wstring& log_msg);

    class CHwCrashDumpCatch {
    public:
        CHwCrashDumpCatch();
        ~CHwCrashDumpCatch();
    private:
        /* 设置未处理异常过滤指针 */
        static LPTOP_LEVEL_EXCEPTION_FILTER WINAPI HwSetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter);
        /* 异常信息写入dump文件 */
        static BOOL HwReleaseDumpFile(const std::wstring& strPath, EXCEPTION_POINTERS *pException);
        /* 异常信息处理 */
        static LONG WINAPI UnhandledExceptionFilterEx(struct _EXCEPTION_POINTERS *pException);
        /* 抛参数错误异常 */
        static void HwPureCallHandler(void);
        /* 参数错误处理 */
        static void HwInvalidParameterHandler(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, uintptr_t pReserved);
        /* 添加异常处理函数 */
        BOOL HwAddExceptionHandle();
        /* 移除异常处理函数 */
        BOOL HwRmExceptionHandle();
        /* 预置未处理异常的处理函数 */
        BOOL HwPreSetUnhandledExceptionFilter();
        /* 设置参数异常处理函数 */
        void HwSetInvalidHandle();
        /* 取消参数异常处理函数 */
        void HwUnSetInvalidHandle();
    private:
        LPTOP_LEVEL_EXCEPTION_FILTER m_preFilter;   // 异常过滤函数指针
        _invalid_parameter_handler m_preIph;        // 参数错误异常处理函数
        _purecall_handler m_prePch;                 // 错误处理函数指针
    };
};

