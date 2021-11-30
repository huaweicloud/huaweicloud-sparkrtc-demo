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

package com.huawei.rtcdemo;

import android.app.Application;

import com.huawei.rtc.HRTCEngine;
import com.huawei.rtc.models.HRTCEngineConfig;
import com.huawei.rtcdemo.adapter.RtcEventCallbackAdapter;
import com.huawei.rtcdemo.interfaces.RtcEventHandler;
import com.huawei.rtcdemo.utils.LogUtil;

/**
 * 应用Application
 * 初始化HRTCEngine
 */
public class RtcApplication extends Application {

    /** RTC 引擎 */
    private HRTCEngine mHwRtcEngine;

    /** RTC 引擎回调事件 */
    private RtcEventCallbackAdapter mHwHandler = new RtcEventCallbackAdapter();

    /** 域名 */
    String serverAddr;

    /** AppId */
    String appId;

    public RtcApplication() {
        super();
    }

    @Override
    public void onCreate() {
        super.onCreate();
        LogUtil.setContext(getApplicationContext());
    }

    @Override
    public void onTerminate() {
        super.onTerminate();
        HRTCEngine.destroy();
    }

    /**
     * 获取RTC引擎
     * @return HRTCEngine
     */
    public HRTCEngine getEngine() {
        if (mHwRtcEngine == null) {
            serverAddr = Constants.RTC_DEFAULT_SERVER_ADDR;
            appId = Constants.RTC_DEFAULT_APP_ID;

            HRTCEngineConfig config = new HRTCEngineConfig();
            config.setAppId(appId);
            config.setContext(this);
            config.setDomain(serverAddr);
            config.setCountryCode("");
            config.setLogEnable(true);
            config.setLogLevel(HRTCEngineConfig.HRTCLogLevel.HRTC_LOG_LEVEL_DEBUG);
            config.setLogPath(LogUtil.getLogPath());
            config.setLogSize(1024 * 10);

            mHwRtcEngine = HRTCEngine.create(config, mHwHandler);
        }

        return mHwRtcEngine;
    }

    /**
     * 注册回调事件
     * @param handler handler
     */
    public void registerEventHandler(RtcEventHandler handler) {
        mHwHandler.addHandler(handler);
    }

    /**
     * 移除回调事件
     * @param handler handler
     */
    public void removeEventHandler(RtcEventHandler handler) {
        mHwHandler.removeHandler(handler);
    }
}
