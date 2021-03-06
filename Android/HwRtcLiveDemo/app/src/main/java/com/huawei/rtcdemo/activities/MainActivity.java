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

import static com.huawei.rtc.models.HRTCUserInfo.HRTCRoleType.HRTC_ROLE_TYPE_JOINER;
import static com.huawei.rtc.models.HRTCUserInfo.HRTCRoleType.HRTC_ROLE_TYPE_PLAYER;
import static com.huawei.rtc.utils.HRTCEnums.HRTCStreamType.HRTC_STREAM_TYPE_FHD;
import static com.huawei.rtc.utils.HRTCEnums.HRTCStreamType.HRTC_STREAM_TYPE_HD;
import static com.huawei.rtc.utils.HRTCEnums.HRTCStreamType.HRTC_STREAM_TYPE_LD;
import static com.huawei.rtc.utils.HRTCEnums.HRTCStreamType.HRTC_STREAM_TYPE_SD;
import static com.huawei.rtc.utils.HRTCEnums.HRTCVideoEncodeResolutionMode.HRTC_VIDEO_ENCODE_RESOLUTION_MODE_CONST_RATIO;

import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.Gravity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import androidx.recyclerview.widget.GridLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.huawei.rtc.HRTCEngine;
import com.huawei.rtc.models.HRTCVideoEncParam;
import com.huawei.rtc.utils.HRTCEnums;
import com.huawei.rtcdemo.Constants;
import com.huawei.rtcdemo.R;
import com.huawei.rtcdemo.RtcApplication;
import com.huawei.rtcdemo.adapter.LiveRoomListAdapter;
import com.huawei.rtcdemo.ui.LiveItemDecoration;
import com.huawei.rtcdemo.ui.NewTitleBar;
import com.huawei.rtcdemo.utils.ExceptionHandler;
import com.huawei.rtcdemo.utils.LogUtil;
import com.huawei.rtcdemo.utils.PrefManager;

import java.util.ArrayList;
import java.util.UUID;

/**
 * ???????????????
 * ????????????????????????????????????
 */
public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MainActivity";

    /** ????????????????????? */
    private static final int REQUEST_PERMISSIONS_CODE = 0X1001;

    /** ?????????????????????????????? */
    private static final int REQUEST_LIVE_ACTIVITY_CODE = 0x1002;

    /** ????????? */
    private NewTitleBar titleBar;

    /** ????????????????????? */
    private LiveRoomListAdapter adapter;

    /** SP?????? */
    private SharedPreferences rtcSp;

    /** ????????????????????????????????? */
    private boolean isLiving = false;

    /** ??????????????????????????? */
    private boolean mPermissionGranted = false;

    /** ???????????????????????????ID */
    private String[] mRoomIds = {"room0","room1","room2","room3","room4","room5","room6","room7"};

    /** ???????????? */
    private String[] mPermissions = {android.Manifest.permission.CAMERA, android.Manifest.permission.RECORD_AUDIO,
        android.Manifest.permission.WRITE_EXTERNAL_STORAGE, android.Manifest.permission.READ_EXTERNAL_STORAGE,
        android.Manifest.permission.READ_PHONE_STATE};

    /** ?????????????????? */
    private AlertDialog mAlertDialog;

    /** ?????????????????? */
    private ArrayList<HRTCVideoEncParam> mEncParamsList;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (!isTaskRoot()) {
            finish();
            return;
        }

        // ????????????
        checkPermissions();

        // ??????Sp????????????user??????
        String userInfo = PrefManager.getPreferences(getApplicationContext()).getString(Constants.RTC_PREF_USER_ID, "");
        if (TextUtils.isEmpty(userInfo)) {
            // ???????????????????????????????????????
            Intent intent = new Intent(MainActivity.this, LoginActivity.class);
            startActivity(intent);
        }

        // ????????????
        setContentView(R.layout.activity_main);

        // ?????????????????????
        initView();

        // ???????????????
        initData();

        // ????????????
        initListener();

        // ?????????sp??????
        rtcSp = PrefManager.getPreferences(getApplicationContext());

        ExceptionHandler handler = new ExceptionHandler(this);
        Thread.setDefaultUncaughtExceptionHandler(handler);
    }

    /**
     * ????????????
     */
    private void checkPermissions() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (PackageManager.PERMISSION_GRANTED != ContextCompat.checkSelfPermission(this, android.Manifest.permission.CAMERA) ||
                PackageManager.PERMISSION_GRANTED != ContextCompat.checkSelfPermission(this, android.Manifest.permission.RECORD_AUDIO) ||
                PackageManager.PERMISSION_GRANTED != ContextCompat.checkSelfPermission(this, android.Manifest.permission.WRITE_EXTERNAL_STORAGE) ||
                PackageManager.PERMISSION_GRANTED != ContextCompat.checkSelfPermission(this, android.Manifest.permission.READ_EXTERNAL_STORAGE) ||
                PackageManager.PERMISSION_GRANTED != ContextCompat.checkSelfPermission(this, android.Manifest.permission.READ_PHONE_STATE)) {
                // ???????????????????????????????????????
                requestPermissions(mPermissions, REQUEST_PERMISSIONS_CODE);
                return;
            }
        }
        mPermissionGranted = true;
    }

    /**
     * ????????????
     */
    private void initListener() {
        titleBar.getIvRight().setOnClickListener(view-> {
            Intent intent = new Intent(MainActivity.this, SettingsActivity.class);
            startActivity(intent);
        });
        adapter.setOnItemClickListener(position -> {
            if (!mPermissionGranted) {
                checkPermissions();
                return;
            }
            String roomId = mRoomIds[position];
            jumpLiveActivity(roomId, false); // here join others room
        });
    }

    /**
     * ?????????????????????
     */
    private void initView() {
        titleBar = findViewById(R.id.live_room_title);
        RecyclerView rvLiveRoom = findViewById(R.id.rv_live_room);
        adapter = new LiveRoomListAdapter(mRoomIds);
        rvLiveRoom.setLayoutManager(new GridLayoutManager(this, 2));
        rvLiveRoom.addItemDecoration(new LiveItemDecoration());
        rvLiveRoom.setAdapter(adapter);
    }

    /**
     * ?????????????????????
     * @param roomId ??????id
     * @param isCreator ?????????????????????????????????
     */
    private void jumpLiveActivity(String roomId, boolean isCreator) {
        LogUtil.i(TAG, "jumpLiveActivity roomId : " + roomId + ", isCreator : " + isCreator);
        // ??????????????????
        if (isLiving) {
            return;
        }
        // ????????????????????????????????????????????????????????????false
        isLiving = true;

        // ??????????????????
        HRTCEngine hRtcEngine = ((RtcApplication) getApplication()).getEngine();

        // ????????????????????????
        int encParamRet = hRtcEngine.setVideoEncoderConfig(Constants.DEFAULT_BITRATE, mEncParamsList);
        LogUtil.i(TAG,"jumpLiveActivity encParamRet: " + encParamRet);

        hRtcEngine.updateLocalRenderMode(HRTCEnums.HRTCVideoDisplayMode.HRTC_VIDEO_DISPLAY_MODE_HIDDEN, HRTCEnums.HRTCVideoMirrorType.HRTC_VIDEO_MIRROR_TYPE_DISABLE);

        HRTCEnums.HRTCOrientationMode direct = HRTCEnums.HRTCOrientationMode.HRTC_ORIENTATION_MODE_LANDSCAPE;
        if (isCreator) {
            direct = HRTCEnums.HRTCOrientationMode.HRTC_ORIENTATION_MODE_PORTRAIT;
        }
        // ??????????????????????????????????????????
//        hRtcEngine.setLayoutDirect(direct);

        hRtcEngine.setDefaultSpeakerModel(HRTCEnums.HRTCSpeakerModel.HRTC_SPEAKER_MODE_SPEAKER);

        if (rtcSp == null) {
            rtcSp = PrefManager.getPreferences(getApplicationContext());
        }

        String userName = rtcSp.getString(Constants.RTC_PREF_USER_NAME, "");
        String userId = rtcSp.getString(Constants.RTC_PREF_USER_ID, "");
        if (TextUtils.isEmpty(userId)) {
            // ??????????????????,?????????????????????????????????
            userId = String.valueOf(UUID.randomUUID().hashCode());
            userName = "??????";
        }

        // ????????????
        Intent intent;
        if (isCreator) {
            intent = new Intent(this, LiveActivity.class);
        } else {
            intent = new Intent(this, LiveLandscapeActivity.class);
        }

        // ?????????????????????Player??????????????????Joiner
        intent.putExtra(Constants.KEY_ROLE, isCreator ? HRTC_ROLE_TYPE_JOINER.ordinal() : HRTC_ROLE_TYPE_PLAYER.ordinal());
        intent.putExtra(Constants.KEY_ROOM_ID, roomId);
        intent.putExtra(Constants.KEY_USER_ID, userId);
        intent.putExtra(Constants.KEY_USER_NAME, userName);
        startActivityForResult(intent, REQUEST_LIVE_ACTIVITY_CODE);
    }

    /**
     * ???????????????
     */
    private void initData() {
        mEncParamsList = new ArrayList<HRTCVideoEncParam>();

        // ??????4?????????????????????????????????(???????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????)
        mEncParamsList.add(new HRTCVideoEncParam(HRTC_STREAM_TYPE_LD,160,90,24,15,270,64,false)); // ??????90p
        mEncParamsList.add(new HRTCVideoEncParam(HRTC_STREAM_TYPE_SD,320,180,24,15,600,80,false)); // ??????180p
        mEncParamsList.add(new HRTCVideoEncParam(HRTC_STREAM_TYPE_HD,640,360,24,15,1700,200,false)); // ??????360p
        mEncParamsList.add(new HRTCVideoEncParam(HRTC_STREAM_TYPE_FHD,1280,720,24,15,4000,500,false)); // ??????720p
    }

    /**
     * ??????????????????????????????
     * @param view ??????????????????
     */
    public void onCreateLiveRoomClick(View view) {
        if (!mPermissionGranted) {
            checkPermissions();
            return;
        }
        // ???????????????
        showJoinRoomDialog();
    }

    /**
     * ??????????????????????????????
     */
    private void showJoinRoomDialog() {
        if (mAlertDialog == null) {
            View view = getLayoutInflater().inflate(R.layout.pop_join_room_by_edit, null);
            mAlertDialog = new AlertDialog.Builder(this).setView(view).setCancelable(false).create();
            EditText roomIdEdit = view.findViewById(R.id.et_room_id);
            Button joinRoomText = view.findViewById(R.id.bt_join_room);
            joinRoomText.setOnClickListener(v -> {
                LogUtil.d(TAG,"joinRoomText onclick call");
                String roomId = roomIdEdit.getText().toString();
                if (TextUtils.isEmpty(roomId)) {
                    LogUtil.d(TAG,"roomIdEdit.getText() is empty");
                    Toast.makeText(this, "??????????????????", Toast.LENGTH_SHORT).show();
                    return;
                }
                jumpLiveActivity(roomId,true);
            });
            Button cancelText = view.findViewById(R.id.bt_join_cancel);
            cancelText.setOnClickListener(v -> mAlertDialog.dismiss());
        }
        mAlertDialog.show();
        TextView userNameText = mAlertDialog.findViewById(R.id.tv_user_name);
        if (userNameText != null) {
            if (rtcSp == null) {
                rtcSp = PrefManager.getPreferences(getApplicationContext());
            }
            String userName = rtcSp.getString(Constants.RTC_PREF_USER_NAME, "");
            userNameText.setText(TextUtils.isEmpty(userName) ? "??????" : userName);
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == REQUEST_LIVE_ACTIVITY_CODE) { // ???LiveActivity??????
            isLiving = false;
            if (mAlertDialog != null && mAlertDialog.isShowing()) {
                mAlertDialog.dismiss();
            }
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (grantResults.length == 0) {
            return;
        }
        if (requestCode == REQUEST_PERMISSIONS_CODE) {
            for (int i = 0;i < grantResults.length;i++) {
                if (grantResults[i] != PackageManager.PERMISSION_GRANTED) {
                    if (!ActivityCompat.shouldShowRequestPermissionRationale(this,permissions[i])) {
                        // ????????????????????????????????????????????????shouldShowRequestPermissionRationale??????false???
                        // ?????????????????????????????????????????????????????????????????????????????????????????????????????????????????????
                        new AlertDialog.Builder(this)
                            .setMessage("???????????????????????????????????????")
                            .setPositiveButton("??????", (dialog, which) -> {
                                // ???????????????????????????????????????
                                Intent intent = new Intent();
                                intent.setAction("android.settings.APPLICATION_DETAILS_SETTINGS");
                                intent.setData(Uri.fromParts("package", getPackageName(), null));
                                startActivity(intent);
                            })
                            .show();
                    }
                    return;
                }
            }
            mPermissionGranted = true;
        }
    }
}
