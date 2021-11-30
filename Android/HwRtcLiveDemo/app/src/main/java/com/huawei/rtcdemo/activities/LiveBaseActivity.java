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
 * 直播界面BaseActivity
 * 实现了直播基本功能，抽象类，两个子类
 * @see LiveActivity
 * @see LiveLandscapeActivity
 */
public abstract class LiveBaseActivity extends AppCompatActivity implements MemberEventClick, RtcEventHandler {
    protected final String TAG = this.getClass().getSimpleName();

    /** Sp存储对象 */
    protected SharedPreferences rtcSp;

    /** HRTCEngine */
    protected HRTCEngine mHwRtcEngine;

    /** 当前角色 */
    protected int mCurrentRole;

    /** 房间成员列表 */
    protected List<BeanRoomMember> roomMemberBeansList = new ArrayList<>();

    /** 房间成员列表适配器 */
    protected RoomMembersAdapter rtcMemberListAdapter;

    /** 成员数文本框 */
    protected TextView rtcMemberTitleCountTv;

    /** 返回文本框（点击返回） */
    protected TextView roomListBackTv;

    /** 角色切换按钮 */
    protected ImageView mRoleChangeBtn;

    /** 音频通道切换按钮（听筒、扬声器） */
    protected ImageView mAudioRouteBtn;

    /** 禁用本地音频按钮 */
    protected ImageView mMuteAudioBtn;

    /** 禁用本地视频按钮 */
    protected ImageView mMuteVideoBtn;

    /** 切换摄像头按钮 */
    protected ImageView mSwitchCameraBtn;

    /** 显示/隐藏房间成员列表按钮 */
    protected ImageView roomListBtn;

    /** 显示音频质量信息 */
    protected TableView mAudioQualityTableView;

    /** 显示视频质量信息 */
    protected TableView mVideoQualityTableView;
    protected TableView mAuxVideoQualityTableView;

    /** 质量信息布局 */
    protected RelativeLayout mQualityLayout;

    /** 顶部标题栏 */
    protected AppBarLayout appBarLayout;

    /** 底部功能栏 */
    protected LinearLayout bottomLayout;

    /** 顶部滑入动画 */
    protected Animation mTopInAnim;

    /** 顶部滑出动画 */
    protected Animation mTopOutAnim;

    /** 底部滑入动画 */
    protected Animation mBottomInAnim;

    /** 底部滑出动画 */
    protected Animation mBottomOutAnim;

    /** 沉浸式工具 */
    protected ImmersionBar mImmersionBar;

    /** 是否主动退出当前Activity */
    protected boolean mLeaveManually = false;

    /** onPause是否被调用 */
    protected boolean mOnPauseCalled = false;

    /** 是否展示调试信息 */
    protected boolean isShowInfoView = false;

    /** 订阅远端流类型 */
    private int mRemoteStreamType;

    /** 本地用户ID */
    protected String mLocalUserId;

    /** 本地用户名 */
    protected String mLocalUserName;

    /** 当前房间ID */
    protected String mRoomId;

    /** 辅流用户ID */
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
     * 引擎初始化和回调注册
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
     * 初始化数据
     */
    protected void initData() {
        mLocalUserId = getIntent().getStringExtra(Constants.KEY_USER_ID);
        mLocalUserName = getIntent().getStringExtra(Constants.KEY_USER_NAME);
        mRoomId = getIntent().getStringExtra(Constants.KEY_ROOM_ID);
        mCurrentRole = getIntent().getIntExtra(Constants.KEY_ROLE, HRTC_ROLE_TYPE_PLAYER.ordinal());
        mRemoteStreamType = rtcSp.getInt(Constants.RTC_PREF_STREAM_SELECT, HRTC_STREAM_TYPE_LD.ordinal());

        // 加入房间接口
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

        // 加入房间接口调用失败，给出对应的提示
        if (HRTC_SUCCESS != ret) {
            showToast("join room failed, input violation!");
            finish();
        }
    }

    /**
     * 初始化视图
     * @return 布局id
     */
    protected abstract int getLayoutId();

    /**
     * 初始化视图组件
     */
    protected abstract void initUniqueViews();

    /**
     * 视图监听事件处理
     */
    protected abstract void initListener();

    /**
     * 本地角色为joiner的视图处理
     */
    protected abstract void startJoin();

    /**
     * 本地角色为publisher的视图处理
     */
    protected abstract void startPublish();

    /**
     * 取消订阅远端用户
     * @param userId 用户id
     */
    protected abstract void removeRemoteUser(String userId);

    /**
     * 删除本地用户
     */
    protected abstract void removeLocalUser();

    /**
     * 订阅远端用户
     * @param userId 用户id
     * @param nickname 用户名
     */
    protected abstract void renderRemoteUser(String userId, String nickname);

    /**
     * 重新添加本地用户
     */
    protected abstract void reRenderLocalUser();

    /**
     * 用户加入回调中不同的处理
     * @param roomId 房间id
     * @param userId 用户id
     * @param nickname 用户名
     */
    protected abstract void onUserJoinedOtherHandler(String roomId, String userId, String nickname);

    /**
     * 刷新已订阅用户的用户名(针对同一个用户ID加入房间)
     * @param userId 用户id
     * @param nickname 用户名
     */
    protected abstract void refreshDisplayName(String userId, String nickname);

    /**
     * 初始化公共基础组件
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

        // 发送接收的  码率、丢包、延迟、抖动
        // 收发的 用户、分辨率、帧率、码率、丢包、延迟、抖动就行
        mAudioQualityTableView.clearTableContentList()
            .setTableHeader("类型", "码率", "丢包", "延迟", "抖动")
            .refreshTableData();
        mVideoQualityTableView.clearTableContentList()
            .setTableHeader("类型", "用户", "码率", "分 辨 率", "帧率", "丢包", "延迟", "抖动")
            .refreshTableData();
        mAuxVideoQualityTableView.clearTableContentList()
            .setTableHeader("类型", "用户", "码率", "分 辨 率", "帧率", "丢包", "延迟", "抖动")
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
            .barColor(R.color.main_background) // 设置颜色
            .autoStatusBarDarkModeEnable(true, 0.2f) // 自动状态栏字体变色
            .autoNavigationBarDarkModeEnable(true, 0.2f) // 自动导航栏字体变色
            .fullScreen(true); // 是否全屏
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
        mLeaveManually = true;  // 调用finish退出页面时，为可控退出，onPause不关闭视频
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
        // 视频自渲染回调
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
                .setTableHeader("类型", "用户", "码率", "分 辨 率", "帧率", "丢包", "延迟", "抖动");
        for (int i = 0; i < localStats.size(); i++) {
            mVideoQualityTableView.addTableContent("视频发送",
                    mLocalUserName,
                    String.valueOf(localStats.get(i).getBitRate()),
                    Math.min(localStats.get(i).getWidth(), localStats.get(i).getHeight()) + "p",
                    String.valueOf(localStats.get(i).getFrameRate()),
                    String.valueOf(localStats.get(i).getPacketLoss()),
                    String.valueOf(localStats.get(i).getDelay()),
                    String.valueOf(localStats.get(i).getJitter()));
        }

        for (int i = 0; i < remoteStats.size(); i++) {
            mVideoQualityTableView.addTableContent("视频接收", getUserName(remoteStats.get(i).getUserId()),
                    String.valueOf(remoteStats.get(i).getBitRate()),
                    Math.min(remoteStats.get(i).getWidth(), remoteStats.get(i).getHeight()) + "p",
                    String.valueOf(remoteStats.get(i).getFrameRate()),
                    String.valueOf(remoteStats.get(i).getPacketLoss()),
                    String.valueOf(remoteStats.get(i).getDelay()),
                    String.valueOf(remoteStats.get(i).getJitter()));
        }
        runOnUiThread(() -> mVideoQualityTableView.refreshTableData());
    }

    // 发送接收的  码率、丢包、延迟、抖动
    // 收发的 分辨率、帧率、码率、丢包、延迟、抖动就行
    @SuppressLint("ResourceType")
    @Override
    public void onAudioStats(List<HRTCLocalAudioStats> list, List<HRTCRemoteAudioStats> list1) {
        mAudioQualityTableView.clearTableContentList()
                .setTableHeader("类型", "码率", "丢包", "延迟", "抖动");
        for (int i = 0; i < list.size(); i++) {
            mAudioQualityTableView.addTableContent("音频发送",
                    String.valueOf(list.get(i).getBitRate()),
                    String.valueOf(list.get(i).getPacketLoss()),
                    String.valueOf(list.get(i).getDelay()),
                    String.valueOf(list.get(i).getJitter()));
        }
        for (int i = 0; i < list1.size(); i++) {
            mAudioQualityTableView.addTableContent("音频接收",
                    String.valueOf(list1.get(i).getBitRate()),
                    String.valueOf(list1.get(i).getPacketLoss()),
                    String.valueOf(list1.get(i).getDelay()),
                    String.valueOf(list1.get(i).getJitter()));
        }

        runOnUiThread(() -> mAudioQualityTableView.refreshTableData());
    }

    @Override
    public void onSubStreamStats(List<HRTCLocalVideoStats> localStats, List<HRTCRemoteVideoStats> remoteStats) {
        mAuxVideoQualityTableView.clearTableContentList().setTableHeader("类型", "用户", "码率", " 分 辨 率 ", "帧率", "丢包", "延迟", "抖动");
        for (HRTCRemoteVideoStats remoteState : remoteStats) {
            mAuxVideoQualityTableView.addTableContent("共享屏幕", getUserName(remoteState.getUserId()),
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
                // 下麦切player
                removeLocalUser();
                removeRoomMember(mLocalUserId);
            } else  {
                // 上麦切joiner
                reRenderLocalUser();
                addLocalToRoomMember(mLocalUserId);
            }
            LogUtil.i(TAG, "onUserRoleChanged userId:" + mLocalUserId);
            refreshAdapter();
            mRoleChangeBtn.setActivated(isPlayer); // Active为false表示下麦
            mMuteAudioBtn.setEnabled(!isPlayer);
            mMuteVideoBtn.setEnabled(!isPlayer);
            mSwitchCameraBtn.setEnabled(!isPlayer);
            mSwitchCameraBtn.setActivated(!isPlayer);
        });
    }

    @Override
    public void onUserJoined(String roomId, String userId, String name) {
        LogUtil.i(TAG, "onUserJoined roomId:" + roomId + ", userId:" + userId + ", nickname:" + name);
        // 角色为publisher不处理
        runOnUiThread(() -> {
            if(userId.equals(mLocalUserId)) {
                // 是自己的场景，添加到列表中，自己添加在第一位
                addLocalToRoomMember(mLocalUserId);
                LogUtil.i(TAG, "onUserJoined mLocalUserId add:" + userId);
                refreshAdapter();
                return;
            }

            final String nickname = TextUtils.isEmpty(name) ? userId : name;
            BeanRoomMember member = getRoomMember(userId);
            if (member != null) {
                LogUtil.i(TAG, "onUserJoined userId exist: " + userId);

                // 用户加入时，检查是否已加入，是否名称相同
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
        // 角色为publisher不处理
        runOnUiThread(() -> {
            if (userId.equals(mLocalUserId)) {
                // 是自己，从列表中删除
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
        showToast("房间已断开，请重新加入房间.");
        finish();
    }

    @Override
    public void onAudioRouteChanged(HRTCEnums.HRTCAudioRoute audioRoute) {
        runOnUiThread(() -> {
            LogUtil.i(TAG, "onAudioRouteChanged: " + audioRoute);
            if (audioRoute == HRTC_AUDIO_ROUTE_SPEAKER) { // 扬声器状态
                mAudioRouteBtn.setActivated(true);
            } else { // 其他状态（蓝牙、听筒、耳麦）
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
     * 取消订阅远端用户
     * @param userId 用户id
     * @param userName 用户名
     */
    @Override
    public void closeVideo(String userId, String userName) {
        removeRemoteUser(userId);
        changePlayState(userId);
        showToast("closed " + userName + " view");
    }

    /**
     * 返回键事件处理
     */
    @Override
    public void onBackPressed() {
        LogUtil.i(TAG, "onBackPressed !");
        showNormalMoreButtonDialog();
    }

    /**
     * activity停止与前台交互
     */
    @Override
    public void onPause() {
        super.onPause();
        LogUtil.i(TAG, "onPause!");
        if (!mLeaveManually) {
            if (mMuteVideoBtn.isActivated()) {
                mHwRtcEngine.enableLocalVideo(false); // 切后台主动关闭视频
            }

            if (mMuteAudioBtn.isActivated()) {
                mHwRtcEngine.enableLocalAudioStream(false); // 切后台主动关闭音频
            }
            mOnPauseCalled = true;
        }
    }

    /**
     * activity开启与前台交互
     */
    @Override
    public void onResume() {
        super.onResume();
        LogUtil.i(TAG, "onResume!");
        if (mOnPauseCalled) {
            if (mMuteVideoBtn.isActivated()) {
                mHwRtcEngine.enableLocalVideo(true); // 切后台主动开启视频
            }

            if (mMuteAudioBtn.isActivated()) {
                mHwRtcEngine.enableLocalAudioStream(true); // 切后台主动开启音频
            }
            mOnPauseCalled = false;
        }
    }

    /**
     * 与会成员列表按钮点击事件
     * @param view 与会成员列表按钮
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
     * 与会列表初始化
     * @param rtcMemberListView 用户列表父布局
     */
    protected void getRoomMemberListWithThread(RecyclerView rtcMemberListView) {
        // 此时已在主线程中，可以更新UI了
        // 定义一个线性布局管理器
        LinearLayoutManager manager = new LinearLayoutManager(LiveBaseActivity.this);
        // 设置布局管理器
        rtcMemberListView.setLayoutManager(manager);
        LogUtil.i(TAG, "roomMemberBeansList.size(): " + roomMemberBeansList.size());
        // 设置adapter
        rtcMemberListAdapter = new RoomMembersAdapter(LiveBaseActivity.this, roomMemberBeansList);
        rtcMemberListView.setAdapter(rtcMemberListAdapter);
        // 增加分割线
        rtcMemberListView.addItemDecoration(new DividerItemDecoration(LiveBaseActivity.this, RecyclerView.VERTICAL)); // support 包的版本是 25或以上时使用
    }

    /**
     * 调试信息按钮点击事件
     * @param view 调试信息按钮
     */
    public void onShowInfo(View view) {
        LogUtil.i(TAG, "onShowInfo!");
        showInfo();
    }

    /**
     * 切换角色按钮点击事件
     * @param view 切换角色按钮
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
     * 扬声器听筒切换按钮点击事件
     * @param view 扬声器听筒切换按钮点击事件
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
     * 挂断按钮的点击事件
     * @param view 挂断按钮
     */
    public void onHangupClicked(View view) {
        LogUtil.i(TAG, "onHangupClicked!");
        finish();
    }

    /**
     * 切换摄像头按钮的点击事件
     * @param view 切换摄像头按钮
     */
    public void onSwitchCameraClicked(View view) {
        LogUtil.i(TAG, "onSwitchCameraClicked !");
        mHwRtcEngine.switchCamera();
    }

    /**
     * 禁用本地音频按钮的点击事件
     * @param view 禁用本地音频按钮
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
     * 禁用本地视频按钮的点击事件
     * @param view 禁用本地视频按钮
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
     * 开启调试信息
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
     * 刷新列表数据
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
     * 本地角色为joiner时的视图处理
     */
    protected void startPlay() {
        LogUtil.i(TAG, "startPlay !");
        mMuteAudioBtn.setEnabled(false);
        mMuteVideoBtn.setEnabled(false);
        mSwitchCameraBtn.setEnabled(false);
        roomListBtn.setActivated(true);
    }

    /**
     * 订阅对应用户并获取流视图
     * @param userId 用户ID
     * @param local 是否是本地用户
     * @return 返回对应用户流视图
     */
    protected SurfaceView prepareRtcVideo(String userId, boolean local) {
        LogUtil.i(TAG, "prepareRtcVideo userId:" + userId + "local:" + local);
        SurfaceView surface = mHwRtcEngine.createRenderer(getApplicationContext());
        if (local) {
            mHwRtcEngine.setupLocalView(surface);
        } else {
            // 下行分辨率自适应默认打开
            int ret = mHwRtcEngine.startRemoteStreamView(userId, surface, HRTCEnums.HRTCStreamType.values()[mRemoteStreamType], false);
            if (ret == 0) {
                mHwRtcEngine.updateRemoteRenderMode(userId, HRTCEnums.HRTCVideoDisplayMode.HRTC_VIDEO_DISPLAY_MODE_HIDDEN, HRTCEnums.HRTCVideoMirrorType.HRTC_VIDEO_MIRROR_TYPE_DISABLE);
            }
        }
        return surface;
    }

    /**
     * 取消订阅对应用户
     * @param userId 用户ID
     * @param local 是否是本地用户
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
     * 从与会列表中删除对应用户
     * @param userId 用户ID
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
     * 添加本地用户到与会者列表
     * @param userId 本地用户ID
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
        roomMemberBeansList.add(0, self); // 添加房间成员信息
    }

    /**
     * 根据用户ID获取对应的java Bean对象
     * @param userId 用户ID
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
     * 退出房间确认弹框
     */
    protected void showNormalMoreButtonDialog() {
        AlertDialog.Builder normalMoreButtonDialog = new AlertDialog.Builder(this);
        normalMoreButtonDialog.setTitle("leave room");
        normalMoreButtonDialog.setIcon(R.drawable.img_logo);
        normalMoreButtonDialog.setMessage("confirm to leave room?");

        // 设置按钮
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
     * 切换对应用户的订阅状态
     * @param userId 用户ID
     */
    protected void changePlayState(String userId) {
        LogUtil.i(TAG, "changePlayState userId:" + userId);
        for (int i = 0; i < roomMemberBeansList.size(); i++) {
            if (roomMemberBeansList.get(i).getUserId().equals(userId)) {
                // play状态取反
                roomMemberBeansList.get(i).setIsPlaying(!roomMemberBeansList.get(i).getIsPlaying());
            }
        }
    }

    /**
     *  切换辅流订阅状态
     * @param userId 用户ID
     * @param isWithAux 是否订阅辅流
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
     * 辅流共享判断
     * @return 是否有辅流在共享
     */
    protected boolean hasAux() {
        LogUtil.i(TAG, "hasAux");
        return !mAuxUserId.isEmpty();
    }

    /**
     * 根据用户ID获取用户名
     * @param userId 用户ID
     * @return 用户名
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
     * 刷新对应用户的用户名
     * @param member 用户对象
     * @param nickname 用户名
     */
    protected void refreshRoomMemberName(BeanRoomMember member, String nickname) {
        // 根据id刷新列表中的用户名
        member.setNickname(nickname);
        removeRoomMember(member.getUserId());
        roomMemberBeansList.add(member);
        refreshDisplayName(member.getUserId(), nickname);
        refreshAdapter();
    }
}
