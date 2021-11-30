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
// StringUtil.h : �ַ����������ͷ�ļ�
//
#pragma once
#include<iostream>
using namespace std;
//#include <string>
//#include <wstring>
#define RTC_LOG StringUtility::MyPrint
class StringUtility {
public:
    /* �����ַ���ת��UTF8�ַ��� */
    static char *HW_LocaleToUtf8(const char *strSrc);
    /* UNICODE�ַ���ת��UTF8�ַ��� */
    static char *HW_UnicodeToUtf8(const wchar_t *strSrc);
    /* �����ַ���ת��UNICODE�ַ��� */
    static wchar_t *HW_LocaleToUnicodeW(const char *strSrc);
    /* UTF8�ַ���ת�ɱ����ַ��� */
    static char *HW_Utf8ToLocale(const char *strSrc);
    /* UTF8�ַ���ת��UNICODE�ַ��� */
    static wchar_t *HW_Utf8ToUnicodeW(const char *strSrc);
    /* UNICODE�ַ���ת�ɱ����ַ��� */
    static char * HW_UnicodeToLocale(const wchar_t *strSrc);
    /* ��ӡ��־��debug������� */
    static void MyPrint(wchar_t * format, ...);
};
