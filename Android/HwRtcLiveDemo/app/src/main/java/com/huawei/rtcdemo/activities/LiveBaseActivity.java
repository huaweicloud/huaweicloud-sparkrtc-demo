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
import static com.huawei.rtc.utils.HRTCConstants.HRTC_SUCCESS;
import static com.huawei.rtc.utils.HRTCEnums.HRTCAudioRoute.HRTC_AUDIO_ROUTE_SPEAKER;
import static com.huawei.rtc.utils.HRTCEnums.HRTCSpeakerModel.HRTC_SPEAKER_MODE_EARPIECE;
import static com.huawei.rtc.utils.HRTCEnums.HRTCSpeakerModel.HRTC_SPEAKER_MODE_SPEAKER;
import static com.huawei.rtc.utils.HRTCEnums.HRTCStreamType.HRTC_STREAM_TYPE_LD;

import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.DividerItemDecoration;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.huawei.rtc.HRTCEngine;
import com.huawei.rtc.models.HRTCAudioFrame;
import com.huawei.rtc.models.HRTCJoinParam;
import com.huawei.rtc.models.HRTCLocalAudioStats;
import com.huawei.rtc.models.HRTCLocalVideoStats;
import com.huawei.rtc.models.HRTCRemoteAudioStats;
import com.huawei.rtc.models.HRTCRemoteVideoStats;
import com.huawei.rtc.models.HRTCStatsInfo;
import com.huawei.rtc.models.HRTCUserInfo;
import com.huawei.rtc.models.HRTCVideoFrame;
import com.huawei.rtc.utils.HRTCConstants;
import com.huawei.rtc.utils.HRTCEnums;
import com.huawei.rtcdemo.Constants;
import com.huawei.rtcdemo.R;
import com.huawei.rtcdemo.RtcApplication;
import com.huawei.rtcdemo.adapter.RoomMembersAdapter;
import com.huawei.rtcdemo.bean.BeanRoomMember;
import com.huawei.rtcdemo.interfaces.MemberEventClick;
import com.huawei.rtcdemo.interfaces.RtcEventHandler;
import com.huawei.rtcdemo.ui.MemberDialog;
import com.huawei.rtcdemo.ui.TableView;
import com.huawei.rtcdemo.utils.LogUtil;
import com.huawei.rtcdemo.utils.PrefManager;

import com.google.android.material.appbar.AppBarLayout;
import com.gyf.barlibrary.ImmersionBar;

import java.util.ArrayList;
import java.util.List;

/**
 * ????????????BaseActivity
 * ??????????????????????????????????????????????????????
 * @see LiveActivity
 * @see LiveLandscapeActivity
 */
public abstract class LiveBaseActivity extends AppCompatActivity implements MemberEventClick, RtcEventHandler {
    protected final String TAG = this.getClass().getSimpleName();

    /** Sp???????????? */
    protected SharedPreferences rtcSp;

    /** HRTCEngine */
    protected HRTCEngine mHwRtcEngine;

    /** ???????????? */
    protected int mCurrentRole;

    /** ?????????????????? */
    protected List<BeanRoomMember> roomMemberBeansList = new ArrayList<>();

    /** ??????????????????????????? */
    protected RoomMembersAdapter rtcMemberListAdapter;

    /** ?????????????????? */
    protected TextView rtcMemberTitleCountTv;

    /** ????????????????????????????????? */
    protected TextView roomListBackTv;

    /** ?????????????????? */
    protected ImageView mRoleChangeBtn;

    /** ???????????????????????????????????????????????? */
    protected ImageView mAudioRouteBtn;

    /** ???????????????????????? */
    protected ImageView mMuteAudioBtn;

    /** ???????????????????????? */
    protected ImageView mMuteVideoBtn;

    /** ????????????????????? */
    protected ImageView mSwitchCameraBtn;

    /** ??????/?????????????????????????????? */
    protected ImageView roomListBtn;

    /** ???????????????????????? */
    protected TableView mAudioQualityTableView;

    /** ???????????????????????? */
    protected TableView mVideoQualityTableView;
    protected TableView mAuxVideoQualityTableView;

    /** ?????????????????? */
    protected RelativeLayout mQualityLayout;

    /** ??????????????? */
    protected AppBarLayout appBarLayout;

    /** ??????????????? */
    protected LinearLayout bottomLayout;

    /** ?????????????????? */
    protected Animation mTopInAnim;

    /** ?????????????????? */
    protected Animation mTopOutAnim;

    /** ?????????????????? */
    protected Animation mBottomInAnim;

    /** ?????????????????? */
    protected Animation mBottomOutAnim;

    /** ??????????????? */
    protected ImmersionBar mImmersionBar;

    /** ????????????????????????Activity */
    protected boolean mLeaveManually = false;

    /** onPause??????????????? */
    protected boolean mOnPauseCalled = false;

    /** ???????????????????????? */
    protected boolean isShowInfoView = false;

    /** ????????????????????? */
    private int mRemoteStreamType;

    /** ????????????ID */
    protected String mLocalUserId;

    /** ??????????????? */
    protected String mLocalUserName;

    /** ????????????ID */
    protected String mRoomId;

    /** ????????????ID */
    protected String mAuxUserId = "";

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        initRoot();
        setContentView(getLayoutId());
        initData();
        initCommonView();
        initUniqueViews();
        initListener();
        if (mCurrentRole == HRTC_ROLE_TYPE_JOINER.ordinal()) {
            startJoin();
        } else if (mCurrentRole == HRTC_ROLE_TYPE_PLAYER.ordinal()) {
            startPlay();
        } else {
            startPublish();
        }
    }

    protected void showToast(final String msg) {
        runOnUiThread(() -> {
            Toast.makeText(LiveBaseActivity.this, msg, Toast.LENGTH_SHORT).show();
        });
    }

    /**
     * ??????????????????????????????
     */
    protected void initRoot() {
        rtcSp = PrefManager.getPreferences(getApplicationContext());
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        mHwRtcEngine = ((RtcApplication) getApplication()).getEngine();
        LogUtil.i(TAG, "registerEventHandler: before!");
        ((RtcApplication) getApplication()).registerEventHandler(this);
        LogUtil.i(TAG, "registerEventHandler: after!");
    }

    /**
     * ???????????????
     */
    protected void initData() {
        mLocalUserId = getIntent().getStringExtra(Constants.KEY_USER_ID);
        mLocalUserName = getIntent().getStringExtra(Constants.KEY_USER_NAME);
        mRoomId = getIntent().getStringExtra(Constants.KEY_ROOM_ID);
        mCurrentRole = getIntent().getIntExtra(Constants.KEY_ROLE, HRTC_ROLE_TYPE_PLAYER.ordinal());
        mRemoteStreamType = rtcSp.getInt(Constants.RTC_PREF_STREAM_SELECT, HRTC_STREAM_TYPE_LD.ordinal());

        // ??????????????????
        HRTCJoinParam param = new HRTCJoinParam();
        param.setUserId(mLocalUserId);
        param.setUserName(mLocalUserName);
        param.setRole(HRTCJoinParam.HRTCRoleType.values()[mCurrentRole]);
        param.setRoomId(mRoomId);
        param.setOptionalInfo("");
        param.setSfuType(0);
        param.setAutoSubscribeAudio(true);
        param.setAutoSubscribeVideo(true);

        int ret = mHwRtcEngine.joinRoom(param);

        // ??????????????????????????????????????????????????????
        if (HRTC_SUCCESS != ret) {
            showToast("join room failed, input violation!");
            finish();
        }
    }

    /**
     * ???????????????
     * @return ??????id
     */
    protected abstract int getLayoutId();

    /**
     * ?????????????????????
     */
    protected abstract void initUniqueViews();

    /**
     * ????????????????????????
     */
    protected abstract void initListener();

    /**
     * ???????????????joiner???????????????
     */
    protected abstract void startJoin();

    /**
     * ???????????????publisher???????????????
     */
    protected abstract void startPublish();

    /**
     * ????????????????????????
     * @param userId ??????id
     */
    protected abstract void removeRemoteUser(String userId);

    /**
     * ??????????????????
     */
    protected abstract void removeLocalUser();

    /**
     * ??????????????????
     * @param userId ??????id
     * @param nickname ?????????
     */
    protected abstract void renderRemoteUser(String userId, String nickname);

    /**
     * ????????????????????????
     */
    protected abstract void reRenderLocalUser();

    /**
     * ????????????????????????????????????
     * @param roomId ??????id
     * @param userId ??????id
     * @param nickname ?????????
     */
    protected abstract void onUserJoinedOtherHandler(String roomId, String userId, String nickname);

    /**
     * ?????????????????????????????????(?????????????????????ID????????????)
     * @param userId ??????id
     * @param nickname ?????????
     */
    protected abstract void refreshDisplayName(String userId, String nickname);

    /**
     * ???????????????????????????
     */
    protected void initCommonView() {
        TextView roomNameText = findViewById(R.id.live_room_name);
        String roomName = "room: " + mRoomId;
        roomNameText.setText(roomName);
        roomNameText.setSelected(true);

        TextView userNameText = findViewById(R.id.live_user_name);
        String userName = "user: " + mLocalUserName;
        userNameText.setText(userName);
        userNameText.setSelected(true);

        boolean isPlayer = (mCurrentRole == HRTC_ROLE_TYPE_PLAYER.ordinal());

        mQualityLayout = findViewById(R.id.quality_rl);
        mAudioQualityTableView = findViewById(R.id.audio_quality_table);
        mVideoQualityTableView = findViewById(R.id.video_quality_table);
        mAuxVideoQualityTableView = findViewById(R.id.aux_video_quality_table);

        // ???????????????  ?????????????????????????????????
        // ????????? ?????????????????????????????????????????????????????????????????????
        mAudioQualityTableView.clearTableContentList()
            .setTableHeader("??????", "??????", "??????", "??????", "??????")
            .refreshTableData();
        mVideoQualityTableView.clearTableContentList()
            .setTableHeader("??????", "??????", "??????", "??? ??? ???", "??????", "??????", "??????", "??????")
            .refreshTableData();
        mAuxVideoQualityTableView.clearTableContentList()
            .setTableHeader("??????", "??????", "??????", "??? ??? ???", "??????", "??????", "??????", "??????")
            .refreshTableData();
        mSwitchCameraBtn = findViewById(R.id.live_btn_switch_camera);
        mSwitchCameraBtn.setActivated(!isPlayer);

        mRoleChangeBtn = findViewById(R.id.live_btn_role_change);
        mRoleChangeBtn.setActivated(isPlayer);

        mMuteVideoBtn = findViewById(R.id.live_btn_mute_video);
        mMuteAudioBtn = findViewById(R.id.live_btn_mute_audio);
        mMuteAudioBtn.setActivated(true);
        mMuteVideoBtn.setActivated(true);

        mAudioRouteBtn = findViewById(R.id.live_btn_audio_route);
        mAudioRouteBtn.setActivated(true);
//        mHwRtcEngine.setRemoteAudioTopNVoice(3);

        roomListBtn = findViewById(R.id.live_btn_room_list);

        mTopInAnim = AnimationUtils.loadAnimation(this, R.anim.slide_top_in);
        mTopOutAnim = AnimationUtils.loadAnimation(this, R.anim.slide_top_out);
        mBottomInAnim = AnimationUtils.loadAnimation(this, R.anim.slide_bottom_in);
        mBottomOutAnim = AnimationUtils.loadAnimation(this, R.anim.slide_bottom_out);
        mImmersionBar = ImmersionBar.with(this)
            .barColor(R.color.main_background) // ????????????
            .autoStatusBarDarkModeEnable(true, 0.2f) // ???????????????????????????
            .autoNavigationBarDarkModeEnable(true, 0.2f) // ???????????????????????????
            .fullScreen(true); // ????????????
        mImmersionBar.init();
        appBarLayout = findViewById(R.id.live_room_appbar_layout);
        bottomLayout = findViewById(R.id.bottom_total_Layout);
    }

    @Override
    protected void onDestroy() {
        LogUtil.i(TAG, "onDestroy!");
        if (mImmersionBar != null) {
            mImmersionBar.destroy();
        }
        if (!mLeaveManually) {
            mHwRtcEngine.leaveRoom();
        }
        super.onDestroy();
    }

    @Override
    public void finish() {
        mLeaveManually = true;  // ??????finish????????????????????????????????????onPause???????????????
        super.finish();
        mHwRtcEngine.leaveRoom();
        ((RtcApplication) getApplication()).removeEventHandler(this);
    }

    @Override
    public void onWarning(int i, String s) {
    }

    @Override
    public void onError(int errCode, String msg) {
        LogUtil.i(TAG, "onError code:" + errCode + ", msg:" + msg);
        showToast("error code:" + errCode + System.getProperty("line.separator") + "msg:" + msg);
        runOnUiThread(() -> {
            if (errCode == HRTCConstants.HRTCErrorCode.HRTC_ERR_CODE_JOIN_ROOM_STATUS_BUSY || errCode == HRTCConstants.HRTCErrorCode.HRTC_ERR_CODE_KICKED_OFF) {
                LogUtil.i(TAG, "call leave room");
                finish();
            }
            if (HRTCConstants.HRTCErrorCode.HRTC_ERR_CODE_USER_ROLE_CHANGE_FAIL == errCode) {
                mRoleChangeBtn.setEnabled(true);
            }
        });
    }

    @Override
    public void onLeaveRoom(HRTCEnums.HRTCLeaveReason reason, HRTCStatsInfo statsInfo) {
        LogUtil.i(TAG, " onLeaveRoom, reason:" + reason);
        if (!mLeaveManually) {
            runOnUiThread(this::finish);
        }
    }

    @Override
    public void onJoinRoomSuccess(String roomId, String userId) {
        LogUtil.i(TAG, "onJoinRoomSuccess roomId:" + roomId + ", userId:" + userId + ", userName: " + mLocalUserName);
    }

    @Override
    public void onJoinRoomFailure(int i, String s) {
        showToast("onJoinRoomFailure[" + i + "]: " + s);
        LogUtil.e(TAG, "onJoinRoomFailure[" + i + "]: " + s);
        finish();
    }

    @Override
    public void onRenderVideoFrame(String roomId, String userId, HRTCVideoFrame frame) {
        // ?????????????????????
        LogUtil.i(TAG, "onRenderVideoFrame demo " + "   " + roomId + "   " + userId + "   " + frame);
    }

    @Override
    public void onPlaybackAudioFrame(HRTCAudioFrame frame) {
        LogUtil.d(TAG, "onPlaybackAudioFrame: demo " + frame.getFrameType() + " " + frame.getBytesPerSample() + " " + frame.getChannels() + " " + frame.getSampleRate() + " " + frame.getSamplesPerSec());
    }

    @SuppressLint("ResourceType")
    @Override
    public void onVideoStats(List<HRTCLocalVideoStats> localStats, List<HRTCRemoteVideoStats> remoteStats) {
        LogUtil.i(TAG, "onVideoStats localStats.size" + localStats.size() + " remoteStats.size()" + remoteStats.size());
        mVideoQualityTableView.clearTableContentList()
                .setTableHeader("??????", "??????", "??????", "??? ??? ???", "??????", "??????", "??????", "??????");
        for (int i = 0; i < localStats.size(); i++) {
            mVideoQualityTableView.addTableContent("????????????",
                    mLocalUserName,
                    String.valueOf(localStats.get(i).getBitRate()),
                    Math.min(localStats.get(i).getWidth(), localStats.get(i).getHeight()) + "p",
                    String.valueOf(localStats.get(i).getFrameRate()),
                    String.valueOf(localStats.get(i).getPacketLoss()),
                    String.valueOf(localStats.get(i).getDelay()),
                    String.valueOf(localStats.get(i).getJitter()));
        }

        for (int i = 0; i < remoteStats.size(); i++) {
            mVideoQualityTableView.addTableContent("????????????", getUserName(remoteStats.get(i).getUserId()),
                    String.valueOf(remoteStats.get(i).getBitRate()),
                    Math.min(remoteStats.get(i).getWidth(), remoteStats.get(i).getHeight()) + "p",
                    String.valueOf(remoteStats.get(i).getFrameRate()),
                    String.valueOf(remoteStats.get(i).getPacketLoss()),
                    String.valueOf(remoteStats.get(i).getDelay()),
                    String.valueOf(remoteStats.get(i).getJitter()));
        }
        runOnUiThread(() -> mVideoQualityTableView.refreshTableData());
    }

    // ???????????????  ?????????????????????????????????
    // ????????? ????????????????????????????????????????????????????????????
    @SuppressLint("ResourceType")
    @Override
    public void onAudioStats(List<HRTCLocalAudioStats> list, List<HRTCRemoteAudioStats> list1) {
        mAudioQualityTableView.clearTableContentList()
                .setTableHeader("??????", "??????", "??????", "??????", "??????");
        for (int i = 0; i < list.size(); i++) {
            mAudioQualityTableView.addTableContent("????????????",
                    String.valueOf(list.get(i).getBitRate()),
                    String.valueOf(list.get(i).getPacketLoss()),
                    String.valueOf(list.get(i).getDelay()),
                    String.valueOf(list.get(i).getJitter()));
        }
        for (int i = 0; i < list1.size(); i++) {
            mAudioQualityTableView.addTableContent("????????????",
                    String.valueOf(list1.get(i).getBitRate()),
                    String.valueOf(list1.get(i).getPacketLoss()),
                    String.valueOf(list1.get(i).getDelay()),
                    String.valueOf(list1.get(i).getJitter()));
        }

        runOnUiThread(() -> mAudioQualityTableView.refreshTableData());
    }

    @Override
    public void onSubStreamStats(List<HRTCLocalVideoStats> localStats, List<HRTCRemoteVideoStats> remoteStats) {
        mAuxVideoQualityTableView.clearTableContentList().setTableHeader("??????", "??????", "??????", " ??? ??? ??? ", "??????", "??????", "??????", "??????");
        for (HRTCRemoteVideoStats remoteState : remoteStats) {
            mAuxVideoQualityTableView.addTableContent("????????????", getUserName(remoteState.getUserId()),
                    String.valueOf(remoteState.getBitRate()),
                    Math.min(remoteState.getWidth(), remoteState.getHeight()) + "p",
                    String.valueOf(remoteState.getFrameRate()),
                    String.valueOf(remoteState.getPacketLoss()),
                    String.valueOf(remoteState.getDelay()),
                    String.valueOf(remoteState.getJitter()));
        }

        runOnUiThread(() -> mAuxVideoQualityTableView.refreshTableData());
    }

    @Override
    public void onUserRoleChanged(HRTCUserInfo.HRTCRoleType oldRole, HRTCUserInfo.HRTCRoleType newRole) {
        LogUtil.i(TAG, "onUserRoleChanged roleType: " + oldRole + "roleType1:" + newRole);
        mCurrentRole = newRole.ordinal();
        runOnUiThread(() -> {
            mRoleChangeBtn.setEnabled(true);
            boolean isPlayer = oldRole == HRTC_ROLE_TYPE_JOINER && newRole == HRTC_ROLE_TYPE_PLAYER;
            if (isPlayer) {
                // ?????????player
                removeLocalUser();
                removeRoomMember(mLocalUserId);
            } else  {
                // ?????????joiner
                reRenderLocalUser();
                addLocalToRoomMember(mLocalUserId);
            }
            LogUtil.i(TAG, "onUserRoleChanged userId:" + mLocalUserId);
            refreshAdapter();
            mRoleChangeBtn.setActivated(isPlayer); // Active???false????????????
            mMuteAudioBtn.setEnabled(!isPlayer);
            mMuteVideoBtn.setEnabled(!isPlayer);
            mSwitchCameraBtn.setEnabled(!isPlayer);
            mSwitchCameraBtn.setActivated(!isPlayer);
        });
    }

    @Override
    public void onUserJoined(String roomId, String userId, String name) {
        LogUtil.i(TAG, "onUserJoined roomId:" + roomId + ", userId:" + userId + ", nickname:" + name);
        // ?????????publisher?????????
        runOnUiThread(() -> {
            if(userId.equals(mLocalUserId)) {
                // ??????????????????????????????????????????????????????????????????
                addLocalToRoomMember(mLocalUserId);
                LogUtil.i(TAG, "onUserJoined mLocalUserId add:" + userId);
                refreshAdapter();
                return;
            }

            final String nickname = TextUtils.isEmpty(name) ? userId : name;
            BeanRoomMember member = getRoomMember(userId);
            if (member != null) {
                LogUtil.i(TAG, "onUserJoined userId exist: " + userId);

                // ????????????????????????????????????????????????????????????
                if (!member.getNickname().equals(nickname)) {
                    LogUtil.i(TAG, "onUserJoined name need refresh, old name: " + member.getNickname());
                    refreshRoomMemberName(member, nickname);
                }

                return;
            }
            showToast(nickname + " has joined room");
            onUserJoinedOtherHandler(roomId, userId, nickname);
        });
    }

    @Override
    public void onUserOffline(String roomId, String userId, int reason) {
        LogUtil.i(TAG, "onUserOffline roomId:" + roomId + ", userId:" + userId + ", reason:" + reason);
        // ?????????publisher?????????
        runOnUiThread(() -> {
            if (userId.equals(mLocalUserId)) {
                // ??????????????????????????????
                removeRoomMember(userId);
                LogUtil.i(TAG, "onUserOffline mLocalUserId:" + userId);
                refreshAdapter();
            }

            BeanRoomMember user = getRoomMember(userId);
            if (user != null) {
                removeRoomMember(userId);
                if (user.getIsPlaying()){
                    removeRemoteUser(userId);
                }
                refreshAdapter();
                showToast(user.getNickname() + " leaves room!");
            }
        });
    }

    @Override
    protected void onRestoreInstanceState(@NonNull Bundle savedInstanceState) {
        super.onRestoreInstanceState(savedInstanceState);
        showToast("???????????????????????????????????????.");
        finish();
    }

    @Override
    public void onAudioRouteChanged(HRTCEnums.HRTCAudioRoute audioRoute) {
        runOnUiThread(() -> {
            LogUtil.i(TAG, "onAudioRouteChanged: " + audioRoute);
            if (audioRoute == HRTC_AUDIO_ROUTE_SPEAKER) { // ???????????????
                mAudioRouteBtn.setActivated(true);
            } else { // ??????????????????????????????????????????
                mAudioRouteBtn.setActivated(false);
            }
        });
    }

    @Override
    public void onConnectionStateChange(HRTCEnums.HRTCConnStateTypes var1, HRTCEnums.HRTCConnChangeReason var2, String var3) {
        LogUtil.i(TAG, "onConnectionStateChange var1" + var1 + "var2:" + var2 + "var3:" + var3);
        final HRTCEnums.HRTCConnStateTypes state = var1;
        showToast(var3);
        runOnUiThread(() -> {
            if (HRTCEnums.HRTCConnStateTypes.HRTC_CONN_FAILED == state) {
                finish();
            }
        });
    }

    /**
     * ????????????????????????
     * @param userId ??????id
     * @param userName ?????????
     */
    @Override
    public void closeVideo(String userId, String userName) {
        removeRemoteUser(userId);
        changePlayState(userId);
        showToast("closed " + userName + " view");
    }

    /**
     * ?????????????????????
     */
    @Override
    public void onBackPressed() {
        LogUtil.i(TAG, "onBackPressed !");
        showNormalMoreButtonDialog();
    }

    /**
     * activity?????????????????????
     */
    @Override
    public void onPause() {
        super.onPause();
        LogUtil.i(TAG, "onPause!");
        if (!mLeaveManually) {
            if (mMuteVideoBtn.isActivated()) {
                mHwRtcEngine.enableLocalVideo(false); // ???????????????????????????
            }

            if (mMuteAudioBtn.isActivated()) {
                mHwRtcEngine.enableLocalAudioStream(false); // ???????????????????????????
            }
            mOnPauseCalled = true;
        }
    }

    /**
     * activity?????????????????????
     */
    @Override
    public void onResume() {
        super.onResume();
        LogUtil.i(TAG, "onResume!");
        if (mOnPauseCalled) {
            if (mMuteVideoBtn.isActivated()) {
                mHwRtcEngine.enableLocalVideo(true); // ???????????????????????????
            }

            if (mMuteAudioBtn.isActivated()) {
                mHwRtcEngine.enableLocalAudioStream(true); // ???????????????????????????
            }
            mOnPauseCalled = false;
        }
    }

    /**
     * ????????????????????????????????????
     * @param view ????????????????????????
     */
    public void getRoomMemberPopup(View view) {
        MemberDialog dialog = new MemberDialog(this);
        dialog.show();
        RecyclerView roomMemberView = dialog.findViewById(R.id.recycler_member);
        roomListBackTv = dialog.findViewById(R.id.tv_room_list_back);
        rtcMemberTitleCountTv = dialog.findViewById(R.id.tv_members);
        String memberText = "(" + roomMemberBeansList.size() + ")";
        rtcMemberTitleCountTv.setText(memberText);
        roomListBackTv.setOnClickListener(v -> dialog.dismiss());

        getRoomMemberListWithThread(roomMemberView);
    }

    /**
     * ?????????????????????
     * @param rtcMemberListView ?????????????????????
     */
    protected void getRoomMemberListWithThread(RecyclerView rtcMemberListView) {
        // ???????????????????????????????????????UI???
        // ?????????????????????????????????
        LinearLayoutManager manager = new LinearLayoutManager(LiveBaseActivity.this);
        // ?????????????????????
        rtcMemberListView.setLayoutManager(manager);
        LogUtil.i(TAG, "roomMemberBeansList.size(): " + roomMemberBeansList.size());
        // ??????adapter
        rtcMemberListAdapter = new RoomMembersAdapter(LiveBaseActivity.this, roomMemberBeansList);
        rtcMemberListView.setAdapter(rtcMemberListAdapter);
        // ???????????????
        rtcMemberListView.addItemDecoration(new DividerItemDecoration(LiveBaseActivity.this, RecyclerView.VERTICAL)); // support ??????????????? 25??????????????????
    }

    /**
     * ??????????????????????????????
     * @param view ??????????????????
     */
    public void onShowInfo(View view) {
        LogUtil.i(TAG, "onShowInfo!");
        showInfo();
    }

    /**
     * ??????????????????????????????
     * @param view ??????????????????
     */
    public void onChangeRole(View view) {
        LogUtil.i(TAG, "Base onChangeRole! roomId: " + mRoomId);
        if (mCurrentRole == HRTC_ROLE_TYPE_PLAYER.ordinal()) {
            mHwRtcEngine.changeUserRole(HRTC_ROLE_TYPE_JOINER,null,0);
        } else if (mCurrentRole == HRTC_ROLE_TYPE_JOINER.ordinal()) {
            mHwRtcEngine.changeUserRole(HRTC_ROLE_TYPE_PLAYER,null,0);
        }
        mRoleChangeBtn.setEnabled(false);
    }

    /**
     * ???????????????????????????????????????
     * @param view ???????????????????????????????????????
     */
    public void onAudioRouteClicked(View view) {
        LogUtil.i(TAG, "onAudioRouteClicked !");
        if (view.isActivated()) {
            mHwRtcEngine.setSpeakerModel(HRTC_SPEAKER_MODE_EARPIECE);
        } else {
            mHwRtcEngine.setSpeakerModel(HRTC_SPEAKER_MODE_SPEAKER);
        }
        view.setActivated(!view.isActivated());
    }

    /**
     * ???????????????????????????
     * @param view ????????????
     */
    public void onHangupClicked(View view) {
        LogUtil.i(TAG, "onHangupClicked!");
        finish();
    }

    /**
     * ????????????????????????????????????
     * @param view ?????????????????????
     */
    public void onSwitchCameraClicked(View view) {
        LogUtil.i(TAG, "onSwitchCameraClicked !");
        mHwRtcEngine.switchCamera();
    }

    /**
     * ???????????????????????????????????????
     * @param view ????????????????????????
     */
    public void onMuteAudioClicked(View view) {
        LogUtil.i(TAG, "onMuteAudioClicked !");
        int ret = mHwRtcEngine.enableLocalAudioStream(!view.isActivated());
        if (ret == 0) {
            view.setActivated(!view.isActivated());
        } else {
            showToast("OnMuteAudioClicked failed: CODE:" + ret);
            LogUtil.i(TAG, "OnMuteAudioClicked failed: CODE:" + ret);
        }

    }

    /**
     * ???????????????????????????????????????
     * @param view ????????????????????????
     */
    public void onMuteVideoClicked(View view) {
        LogUtil.i(TAG, "onMuteVideoClicked !");
        int ret = mHwRtcEngine.enableLocalVideo(!view.isActivated());
        if (ret == 0) {
            view.setActivated(!view.isActivated());
        } else {
            showToast("onMuteVideoClicked failed: CODE:" + ret);
            LogUtil.i(TAG, "onMuteVideoClicked failed: CODE:" + ret);
        }
    }

    /**
     * ??????????????????
     */
    protected void showInfo() {
        LogUtil.i(TAG, "onEnableInfo!");
        if (isShowInfoView) {
            mQualityLayout.setVisibility(View.INVISIBLE);
        } else {
            mQualityLayout.setVisibility(View.VISIBLE);
            mAuxVideoQualityTableView.refreshTableData();
            mVideoQualityTableView.refreshTableData();
        }

        isShowInfoView = !isShowInfoView;
    }

    /**
     * ??????????????????
     */
    protected void refreshAdapter() {
        LogUtil.i(TAG, "refreshAdapter !");
        if (rtcMemberListAdapter != null) {
            String memberText = "(" + roomMemberBeansList.size() + ")";
            rtcMemberTitleCountTv.setText(memberText);
            rtcMemberListAdapter.notifyDataSetChanged();
        }
    }

    /**
     * ???????????????joiner??????????????????
     */
    protected void startPlay() {
        LogUtil.i(TAG, "startPlay !");
        mMuteAudioBtn.setEnabled(false);
        mMuteVideoBtn.setEnabled(false);
        mSwitchCameraBtn.setEnabled(false);
        roomListBtn.setActivated(true);
    }

    /**
     * ????????????????????????????????????
     * @param userId ??????ID
     * @param local ?????????????????????
     * @return ???????????????????????????
     */
    protected SurfaceView prepareRtcVideo(String userId, boolean local) {
        LogUtil.i(TAG, "prepareRtcVideo userId:" + userId + "local:" + local);
        SurfaceView surface = mHwRtcEngine.createRenderer(getApplicationContext());
        if (local) {
            mHwRtcEngine.setupLocalView(surface);
        } else {
            // ????????????????????????????????????
            int ret = mHwRtcEngine.startRemoteStreamView(userId, surface, HRTCEnums.HRTCStreamType.values()[mRemoteStreamType], false);
            if (ret == 0) {
                mHwRtcEngine.updateRemoteRenderMode(userId, HRTCEnums.HRTCVideoDisplayMode.HRTC_VIDEO_DISPLAY_MODE_HIDDEN, HRTCEnums.HRTCVideoMirrorType.HRTC_VIDEO_MIRROR_TYPE_DISABLE);
            }
        }
        return surface;
    }

    /**
     * ????????????????????????
     * @param userId ??????ID
     * @param local ?????????????????????
     */
    protected void removeRtcVideo(String userId, boolean local) {
        LogUtil.i(TAG, "removeRtcVideo userId:" + userId + "local:" + local);
        if (local) {
            mHwRtcEngine.setupLocalView(null);
        } else {
            mHwRtcEngine.stopRemoteStreamView(userId);
        }
    }

    /**
     * ????????????????????????????????????
     * @param userId ??????ID
     */
    protected void removeRoomMember(String userId) {
        LogUtil.i(TAG, "removeRoomMember userId:" + userId);
        for (int i = 0; i < roomMemberBeansList.size(); i++) {
            LogUtil.i(TAG, "roomMemberBeansList.get(i): " + roomMemberBeansList.get(i) + " userId:" + roomMemberBeansList
                .get(i).getUserId());
            if (roomMemberBeansList.get(i).getUserId().equals(userId)) {
                roomMemberBeansList.remove(i);
                return;
            }
        }
    }

    /**
     * ????????????????????????????????????
     * @param userId ????????????ID
     */
    protected void addLocalToRoomMember(String userId) {
        LogUtil.i(TAG, "addLocalToRoomMember userId:" + userId);
        for (int i = 0; i < roomMemberBeansList.size(); i++) {
            if (roomMemberBeansList.get(i).getUserId().equals(userId)) {
                return;
            }
        }
        BeanRoomMember self = new BeanRoomMember(mRoomId, userId, mLocalUserName, true);
        self.setSelf(true);
        roomMemberBeansList.add(0, self); // ????????????????????????
    }

    /**
     * ????????????ID???????????????java Bean??????
     * @param userId ??????ID
     * @return BeanRoomMember
     */
    protected BeanRoomMember getRoomMember(String userId) {
        LogUtil.i(TAG, "getRoomMember userId:" + userId);
        for (BeanRoomMember member : roomMemberBeansList) {
            if (member.getUserId().equals(userId)) {
                return member;
            }
        }
        return null;
    }

    /**
     * ????????????????????????
     */
    protected void showNormalMoreButtonDialog() {
        AlertDialog.Builder normalMoreButtonDialog = new AlertDialog.Builder(this);
        normalMoreButtonDialog.setTitle("leave room");
        normalMoreButtonDialog.setIcon(R.drawable.img_logo);
        normalMoreButtonDialog.setMessage("confirm to leave room?");

        // ????????????
        normalMoreButtonDialog.setPositiveButton("confirm",
                (dialog, which) -> {
                    dialog.dismiss();
                    finish();
                });
        normalMoreButtonDialog.setNegativeButton("cancel",
                (dialog, which) -> dialog.dismiss());

        normalMoreButtonDialog.create().show();
    }

    /**
     * ?????????????????????????????????
     * @param userId ??????ID
     */
    protected void changePlayState(String userId) {
        LogUtil.i(TAG, "changePlayState userId:" + userId);
        for (int i = 0; i < roomMemberBeansList.size(); i++) {
            if (roomMemberBeansList.get(i).getUserId().equals(userId)) {
                // play????????????
                roomMemberBeansList.get(i).setIsPlaying(!roomMemberBeansList.get(i).getIsPlaying());
            }
        }
    }

    /**
     *  ????????????????????????
     * @param userId ??????ID
     * @param isWithAux ??????????????????
     */
    protected void changeAuxState(String userId, boolean isWithAux) {
        LogUtil.i(TAG, "changeAuxState userId:" + userId + "isWithAux:" + isWithAux);
        if (isWithAux) {
            mAuxUserId = userId;
        } else {
            mAuxUserId = "";
        }
    }

    /**
     * ??????????????????
     * @return ????????????????????????
     */
    protected boolean hasAux() {
        LogUtil.i(TAG, "hasAux");
        return !mAuxUserId.isEmpty();
    }

    /**
     * ????????????ID???????????????
     * @param userId ??????ID
     * @return ?????????
     */
    protected String getUserName(String userId) {
        if (roomMemberBeansList != null) {
            for (BeanRoomMember member : roomMemberBeansList) {
                if (member.getUserId().equals(userId)) {
                    return member.getNickname();
                }
            }
        }
        return userId;
    }

    /**
     * ??????????????????????????????
     * @param member ????????????
     * @param nickname ?????????
     */
    protected void refreshRoomMemberName(BeanRoomMember member, String nickname) {
        // ??????id???????????????????????????
        member.setNickname(nickname);
        removeRoomMember(member.getUserId());
        roomMemberBeansList.add(member);
        refreshDisplayName(member.getUserId(), nickname);
        refreshAdapter();
    }
}
