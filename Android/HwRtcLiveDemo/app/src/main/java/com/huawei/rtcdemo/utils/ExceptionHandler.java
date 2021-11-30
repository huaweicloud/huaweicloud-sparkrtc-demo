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

package com.huawei.rtcdemo.utils;

import android.content.Context;
import android.os.Environment;
import android.os.Process;
import android.util.Log;

import java.io.BufferedWriter;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.lang.Thread.UncaughtExceptionHandler;
import java.nio.charset.Charset;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Locale;

/**
 * 异常处理类
 */
public class ExceptionHandler implements UncaughtExceptionHandler {
    private static final String TAG = "ExceptionHandler";

    private UncaughtExceptionHandler mDefaultHandler = null;

    // Device properties
    private String brand;

    private String model;

    private String board;

    private String revision;

    private String os_ver;

    public ExceptionHandler(Context ctx) {
        brand = android.os.Build.BRAND;
        model = android.os.Build.MODEL;
        board = android.os.Build.BOARD;
        revision = android.os.Build.VERSION.INCREMENTAL;
        os_ver = android.os.Build.VERSION.RELEASE;

        // Store the default exception handler
        mDefaultHandler = Thread.getDefaultUncaughtExceptionHandler();
    }

    @Override
    public void uncaughtException(Thread thread, Throwable ex) {
        try {
            long uptime = getSystemTime();

            StringBuffer crashReport = new StringBuffer();

            // [DEVICE]
            crashReport.append(getDeviceInfo(uptime));

            // [APPLICATION]
            crashReport.append(getApplicationInfo(uptime));

            // [EXCEPTION]
            crashReport.append("[EXCEPTION]").append(System.lineSeparator());

            String name = ex.getMessage();
            if (null != name) {
                crashReport.append("Name: ").append(name).append(System.lineSeparator());
            }

            crashReport.append("Reason: ").append(ex.toString()).append(System.lineSeparator());

            crashReport.append("Process ID: ").append(Process.myPid()).append(System.lineSeparator());

            crashReport.append("Thread: ").append(Process.myTid()).append(" (").append(thread.getName()).append(")").append(System.lineSeparator());

            StackTraceElement[] stackTrace = ex.getStackTrace();
            if (stackTrace.length > 0) {
                for (int i = 1; i <= stackTrace.length; i++) {
                    crashReport.append("Call Stack " + i + ": ").append(stackTrace[i - 1].toString()).append(System.lineSeparator());
                }
            }

            // Try to find the root cause
            Throwable rootCause = ex;
            while (rootCause.getCause() != null) {
                rootCause = rootCause.getCause();
            }

            if (rootCause != ex) {
                // We got the root cause which is different from the original exception
                stackTrace = rootCause.getStackTrace();

                if (stackTrace.length > 0) {
                    crashReport.append("Caused by: ").append(rootCause.toString()).append(System.lineSeparator());

                    for (int i = 1; i <= stackTrace.length; i++) {
                        crashReport.append("Call Stack " + i + ": ").append(stackTrace[i - 1].toString());
                    }
                }
            }
            crashReport.append(System.lineSeparator());
            saveToFile(crashReport.toString());
        } finally {
            if (mDefaultHandler != null) {
                Log.i("DmpCrashReporter", "End of crash reporting. System default handler will do the rest work.");
                mDefaultHandler.uncaughtException(thread, ex);
            }
        }
    }

    private String getDeviceInfo(long uptime) {
        StringBuilder builder = new StringBuilder();
        builder.append("[DEVICE]").append(System.lineSeparator());
        builder.append("Brand: ").append(brand).append(System.lineSeparator());
        builder.append("Model: ").append(model).append(System.lineSeparator());
        builder.append("Board: ").append(board).append(System.lineSeparator());
        builder.append("Revision: ").append(revision).append(System.lineSeparator());
        builder.append("Android Version: ").append(os_ver).append(System.lineSeparator());

        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS", Locale.US);
        String currTime = sdf.format(Calendar.getInstance().getTime());
        builder.append("System Time :").append(currTime).append(System.lineSeparator());

        builder.append("Boot Up Seconds: ").append(uptime / 1000).append(".").append(uptime % 1000).append(System.lineSeparator());
        builder.append(System.lineSeparator());
        return builder.toString();
    }

    private String getApplicationInfo(long uptime) {
        StringBuilder builder = new StringBuilder();
        builder.append("[APPLICATION]").append(System.lineSeparator());
        builder.append(System.lineSeparator());

        return builder.toString();
    }

    private long getSystemTime() {
        long time = System.currentTimeMillis();
        return time;
    }

    // 保存文件到sd卡，rtcLog目录下，这个目录调用logUpload，整个目录会被上传到后台，用于分析
    public void saveToFile(String content) {
        BufferedWriter out = null;

        Log.i(TAG, "saveToFile: " + content);
        // 获取SD卡状态
        String state = Environment.getExternalStorageState();
        // 判断SD卡是否就绪
        if (!state.equals(Environment.MEDIA_MOUNTED)) {
            return;
        }
        String filePath = LogUtil.getLogPath();
        try {
            out = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(filePath + "/demoCrash.txt", true), Charset.defaultCharset()));
            out.newLine();
            out.write(content);

        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (out != null) {
                try {
                    out.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
