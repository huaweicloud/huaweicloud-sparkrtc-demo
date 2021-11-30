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

package com.huawei.rtcdemo.activities;

import static com.huawei.rtc.utils.HRTCEnums.HRTCStreamType.HRTC_STREAM_TYPE_FHD;
import static com.huawei.rtc.utils.HRTCEnums.HRTCStreamType.HRTC_STREAM_TYPE_HD;
import static com.huawei.rtc.utils.HRTCEnums.HRTCStreamType.HRTC_STREAM_TYPE_LD;
import static com.huawei.rtc.utils.HRTCEnums.HRTCStreamType.HRTC_STREAM_TYPE_SD;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.text.TextUtils;
import android.widget.RadioGroup;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.AppCompatEditText;

import com.huawei.rtc.HRTCEngine;
import com.huawei.rtcdemo.BuildConfig;
import com.huawei.rtcdemo.Constants;
import com.huawei.rtcdemo.R;
import com.huawei.rtcdemo.RtcApplication;
import com.huawei.rtcdemo.ui.NewTitleBar;
import com.huawei.rtcdemo.utils.LogUtil;
import com.huawei.rtcdemo.utils.PrefManager;

import java.util.UUID;

/**
 * 设置界面
 */
public class SettingsActivity extends AppCompatActivity {
    /** 用户名输入框 */
    private AppCompatEditText mNicknameEditText;

    /** 分辨率选择组 */
    private RadioGroup rgPixels;

    /** Sp存储 */
    private SharedPreferences rtcSp;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_settings);
        rtcSp = PrefManager.getPreferences(getApplicationContext());
        initUI();
        ((RtcApplication) getApplication()).getEngine(); // 暂时保留防止奔溃
        TextView version = findViewById(R.id.hw_rtc_version);
        version.setText("V" + HRTCEngine.getVersion());
        TextView releaseTime = findViewById(R.id.hw_rtc_release_time);
        releaseTime.setText(BuildConfig.releaseTime);
    }

    /**
     * 初始化UI
     */
    private void initUI() {
        mNicknameEditText = findViewById(R.id.setting_nickname);
        NewTitleBar titleBar = findViewById(R.id.setting_title_layout);

        String userName = rtcSp.getString(Constants.RTC_PREF_USER_NAME, "");
        if (userName != null) {
            mNicknameEditText.setText(userName);
        }

        rgPixels = findViewById(R.id.rg_pixels);
        int streamType = rtcSp.getInt(Constants.RTC_PREF_STREAM_SELECT, HRTC_STREAM_TYPE_LD.ordinal());
        if (streamType == HRTC_STREAM_TYPE_LD.ordinal()) {
            rgPixels.check(R.id.setting_radio_90);
        } else if (streamType == HRTC_STREAM_TYPE_SD.ordinal()) {
            rgPixels.check(R.id.setting_radio_180);
        } else if (streamType == HRTC_STREAM_TYPE_HD.ordinal()) {
            rgPixels.check(R.id.setting_radio_360);
        } else {
            rgPixels.check(R.id.setting_radio_720);
        }

        titleBar.getIvLeft().setOnClickListener((view) -> onBackArrowPressed());
    }

    /**
     * 保存配置
     */
    private void savePreference() {
        String nickname = mNicknameEditText.getText().toString();
        if (TextUtils.isEmpty(nickname)) {
            String strToast = "名字不能为空";
            showToast(strToast);
            return;
        }

        if (nickname.length() > 8) {
            String strToast = "名字不能超过8位字符";
            showToast(strToast);
            return;
        }

        if (nickname.startsWith(" ") || nickname.endsWith(" ")) {
            String strToast = "名字前后不要使用空格";
            showToast(strToast);
            return;
        }

        String userId = rtcSp.getString(Constants.RTC_PREF_USER_ID, "");
        if (TextUtils.isEmpty(userId)) {
            userId = String.valueOf(UUID.randomUUID().hashCode());
            rtcSp.edit().putString(Constants.RTC_PREF_USER_ID,userId).apply();
        }
        rtcSp.edit().putString(Constants.RTC_PREF_USER_NAME,nickname).apply();

        int checkedRadioButtonId = rgPixels.getCheckedRadioButtonId();
        int type = HRTC_STREAM_TYPE_LD.ordinal();
        switch (checkedRadioButtonId) {
            case R.id.setting_radio_90:
                type = HRTC_STREAM_TYPE_LD.ordinal();
                break;
            case R.id.setting_radio_180:
                type = HRTC_STREAM_TYPE_SD.ordinal();
                break;
            case R.id.setting_radio_360:
                type = HRTC_STREAM_TYPE_HD.ordinal();
                break;
            case R.id.setting_radio_720:
                type = HRTC_STREAM_TYPE_FHD.ordinal();
                break;
            default:
                break;
        }
        LogUtil.d("streamType check", "streamType" + type);
        rtcSp.edit().putInt(Constants.RTC_PREF_STREAM_SELECT, type).apply();

        finish();
    }

    @Override
    public void onBackPressed() {
        onBackArrowPressed();
    }

    public void onBackArrowPressed() {
        savePreference();
    }

    private void showToast(String toastStr) {
        Toast toast = Toast.makeText(this, toastStr, Toast.LENGTH_SHORT);
        toast.show();
    }
}
