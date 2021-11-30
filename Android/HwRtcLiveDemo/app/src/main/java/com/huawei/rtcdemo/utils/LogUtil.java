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
import android.util.Log;

import java.io.File;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.nio.charset.Charset;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

/**
 * Log使用类
 */
public class LogUtil {
    private static final int VERBOSE = 0;
    private static final int DEBUG = 1;
    private static final int INFO = 2;
    private static final int WARNING = 3;
    private static final int ERROR = 4;
    private static final int NO_LOG = 5;
    private static int LOG_LEVEL = VERBOSE;
    private static boolean logFileEnable = true;

    private static Context context;

    public static void setContext(Context context) {
        LogUtil.context = context;
    }

    public static String getLogPath() {
        if (context != null) {
            File path = context.getExternalFilesDir("rtcLog");
            return path.getAbsolutePath();
        }
        return null;
    }

    // verbose
    public static void v(String tag, String msg) {
        if (VERBOSE < LOG_LEVEL) {
            return;
        }
        if (msg == null) {
            return;
        }
        Log.v(tag, msg);
        write("VERBOSE", getPrefixName(), msg);
    }

    public static void v(String msg) {
        if (DEBUG < LOG_LEVEL) {
            return;
        }
        if (msg == null) {
            return;
        }
        v(getPrefixName(), msg);
    }

    // debug
    public static void d(String tag, String msg) {
        if (DEBUG < LOG_LEVEL) {
            return;
        }
        if (msg == null) {
            return;
        }
        Log.d(tag, msg);
        write("DEBUG", getPrefixName(), msg);
    }

    public static void d(String msg) {
        if (DEBUG < LOG_LEVEL) {
            return;
        }
        if (msg == null) {
            return;
        }
        d(getPrefixName(), msg);
    }

    // info
    public static void i(String tag, String msg) {
        if (INFO < LOG_LEVEL) {
            return;
        }
        if (msg == null) {
            return;
        }
        Log.i(tag, msg);
        write("INFO", getPrefixName(), msg);
    }

    public static void i(String msg) {
        if (DEBUG < LOG_LEVEL) {
            return;
        }
        if (msg == null) {
            return;
        }
        i(getPrefixName(), msg);
    }

    // warning
    public static void w(String tag, String msg) {
        if (WARNING < LOG_LEVEL) {
            return;
        }
        if (msg == null) {
            return;
        }
        Log.w(tag, msg);
        write("WARNING", getPrefixName(), msg);
    }

    public static void w(String msg) {
        if (DEBUG < LOG_LEVEL) {
            return;
        }
        if (msg == null) {
            return;
        }
        w(getPrefixName(), msg);
    }

    public static void w(String tag, String msg, Throwable tr) {
        if (WARNING < LOG_LEVEL) {
            return;
        }
        if (msg == null) {
            return;
        }
        Log.w(tag, msg, tr);
        write("WARNING", getPrefixName(), msg);
    }

    // error
    public static void e(String tag, String msg) {
        if (ERROR < LOG_LEVEL) {
            return;
        }
        if (msg == null) {
            return;
        }
        Log.e(tag, msg);
        write("ERROR", getPrefixName(), msg);
    }

    public static void e(String msg) {
        if (DEBUG < LOG_LEVEL) {
            return;
        }
        if (msg == null) {
            return;
        }
        e(getPrefixName(), msg);
    }

    /**
     * 写到文件中的log的前缀，如果因为混淆之类的原因而取不到，就返回"[ minify ]"
     *
     * @return prefix
     */
    private static String getPrefixName() {
        StackTraceElement[] sts = Thread.currentThread().getStackTrace();
        try {
            for (StackTraceElement st : sts) {
                if (st.isNativeMethod()) {
                    continue;
                }
                if (st.getClassName().equals(Thread.class.getName())) {
                    continue;
                }
                if (st.getClassName().equals(LogUtil.class.getName())) {
                    continue;
                }
                if (st.getFileName() != null) {
                    return "[ " + Thread.currentThread().getName() +
                            ": " + st.getFileName() + ":" + st.getLineNumber() +
                            " " + st.getMethodName() + " ]";
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "[ minify ]";
    }

    /**
     * 追加文件：使用FileWriter
     *
     * @param level   等级
     * @param prefix  前缀
     * @param content 内容
     */
    private static void write(String level, String prefix, String content) {
        if (!logFileEnable) {
            return;
        }
        OutputStreamWriter writer = null;
        try {
            String appLogPath = getLogPath() + "/DemoLogInfo.log";
            // 打开一个写文件器，构造函数中的第二个参数true表示以追加形式写文件
            writer = new OutputStreamWriter(new FileOutputStream(appLogPath, true), Charset.defaultCharset());
            SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss:SSS", Locale.getDefault());
            String time = sdf.format(new Date());
            writer.write(time + ": " + level + "/" + prefix + ": " + content + System.lineSeparator());
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (writer != null) {
                try {
                    writer.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
