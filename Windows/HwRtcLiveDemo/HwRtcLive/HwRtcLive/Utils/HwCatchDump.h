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
�쳣������
*/
#pragma once
#include "windows.h"
using namespace std;
#include <string>
namespace HwDumpCatch {
    /* д��־*/
    void Hwlog(const std::wstring& log_msg);

    class CHwCrashDumpCatch {
    public:
        CHwCrashDumpCatch();
        ~CHwCrashDumpCatch();
    private:
        /* ����δ�����쳣����ָ�� */
        static LPTOP_LEVEL_EXCEPTION_FILTER WINAPI HwSetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter);
        /* �쳣��Ϣд��dump�ļ� */
        static BOOL HwReleaseDumpFile(const std::wstring& strPath, EXCEPTION_POINTERS *pException);
        /* �쳣��Ϣ���� */
        static LONG WINAPI UnhandledExceptionFilterEx(struct _EXCEPTION_POINTERS *pException);
        /* �ײ��������쳣 */
        static void HwPureCallHandler(void);
        /* ���������� */
        static void HwInvalidParameterHandler(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, uintptr_t pReserved);
        /* ����쳣������ */
        BOOL HwAddExceptionHandle();
        /* �Ƴ��쳣������ */
        BOOL HwRmExceptionHandle();
        /* Ԥ��δ�����쳣�Ĵ����� */
        BOOL HwPreSetUnhandledExceptionFilter();
        /* ���ò����쳣������ */
        void HwSetInvalidHandle();
        /* ȡ�������쳣������ */
        void HwUnSetInvalidHandle();
    private:
        LPTOP_LEVEL_EXCEPTION_FILTER m_preFilter;   // �쳣���˺���ָ��
        _invalid_parameter_handler m_preIph;        // ���������쳣������
        _purecall_handler m_prePch;                 // ��������ָ��
    };
};

