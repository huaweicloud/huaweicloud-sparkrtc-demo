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
日志单实例类
*/
#pragma once
#include <winnt.h>

#ifdef __cplusplus

// 单体声明
#define DECLARE_SINGLETON(T)  \
private: \
    static T * volatile singleton_instance; \
    virtual ~T(); \
public: \
    static T * GetInstance();

#pragma warning (disable:4722)

// 单体实现
#define IMPLEMENT_SINGLETON(T) \
T* volatile T::singleton_instance = NULL; \
T* T::GetInstance() \
{ \
    if (singleton_instance) \
    { \
        return singleton_instance; \
    } \
    T *mine = new (T); \
    T *curr = (T *)InterlockedCompareExchangePointer((void **)&singleton_instance, mine, nullptr); \
    if (curr != NULL) \
    { \
        delete mine; \
        mine = curr; \
    } \
    return (T*)mine; \
}\
\
T::~T()\
{\
}

// 单体取用
#define SINGLETON(T) T::GetInstance()
#endif // __cplusplus
