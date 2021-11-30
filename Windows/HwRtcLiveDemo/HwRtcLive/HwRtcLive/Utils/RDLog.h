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
/********************************************************************
* 文 件 名:    RDLog.h
* 文件描述:    HW Rtc Demo 日志收集
********************************************************************/

#pragma once

#ifdef    __cplusplus
extern "C" {
#endif

    /* 写日志接口 */
    void RDLog(const char *tag,
        const char *file,
        int line,
        const char *fmt, ...);
    /* 写日志宏定义*/
#define RD_LOG(tag, fmt, ...)        RDLog(tag, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
    /* 打开本地文件日志通道 */
    void RDOpenLog();
    /* 关闭本地文件日志通道 */
    void RDCloseLog();

#ifdef    __cplusplus
}

#endif // __cplusplus


