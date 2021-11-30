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

import android.content.SharedPreferences;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.Gravity;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import com.huawei.rtcdemo.Constants;
import com.huawei.rtcdemo.R;
import com.huawei.rtcdemo.utils.PrefManager;

import java.util.UUID;

/**
 * 登录界面
 */
public class LoginActivity extends AppCompatActivity {

    /** 用户名编辑框 */
    private EditText userNameEdit;

    /** Sp存储对象 */
    private SharedPreferences rtcSp;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);
        userNameEdit = findViewById(R.id.et_user_name);
        rtcSp = PrefManager.getPreferences(getApplicationContext());
    }

    /**
     * 登录按钮点击事件
     * @param view 登录按钮
     */
    public void onLoginClicked(View view) {
        String name = userNameEdit.getText().toString();
        if (TextUtils.isEmpty(name)) {
            String strToast = "名字不能为空";
            showToast(strToast);
            return;
        }

        if (name.length() > 8) {
            String strToast = "名字不能超过8位字符";
            showToast(strToast);
            return;
        }

        if (name.startsWith(" ") || name.endsWith(" ")) {
            String strToast = "名字前后不要使用空格";
            showToast(strToast);
            return;
        }

        String userId = String.valueOf(UUID.randomUUID().hashCode());

        rtcSp.edit().putString(Constants.RTC_PREF_USER_ID,userId).apply();
        rtcSp.edit().putString(Constants.RTC_PREF_USER_NAME,name).apply();
        finish();
    }

    private void showToast(String toastStr) {
        Toast toast = Toast.makeText(this, toastStr, Toast.LENGTH_SHORT);
        toast.setGravity(Gravity.BOTTOM, 0, getWindowManager().getDefaultDisplay().getHeight() / 7);
        toast.show();
    }
}