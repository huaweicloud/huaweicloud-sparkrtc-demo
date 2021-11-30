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
import static com.huawei.rtc.utils.HRTCEnums.HRTCRotationType.HRTC_ROTATION_TYPE_0;
import static com.huawei.rtc.utils.HRTCEnums.HRTCRotationType.HRTC_ROTATION_TYPE_180;
import static com.huawei.rtc.utils.HRTCEnums.HRTCRotationType.HRTC_ROTATION_TYPE_270;
import static com.huawei.rtc.utils.HRTCEnums.HRTCRotationType.HRTC_ROTATION_TYPE_90;

import android.util.Log;
import android.util.TypedValue;
import android.view.OrientationEventListener;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup;

import com.huawei.rtc.HRTCEngine;
import com.huawei.rtc.utils.HRTCEnums;
import com.huawei.rtcdemo.R;
import com.huawei.rtcdemo.bean.BeanRoomMember;
import com.huawei.rtcdemo.ui.WndArrangeMaker;
import com.huawei.rtcdemo.utils.LogUtil;
import com.huawei.rtcdemo.utils.WindowUtil;

import com.gyf.barlibrary.BarHide;

/**
 * 竖屏模式下的直播界面（竖屏）
 * 展示自己和与会者画面，可切换摄像头、切换角色、开关麦克风等
 */
public class LiveActivity extends LiveBaseActivity {

    /** 竖屏模式下订阅的最大用户（屏幕最大显示个数） */
    private static final int MAX_USER = 4;

    /** 竖屏显示流视图的自定义父布局 */
    private WndArrangeMaker mWndArrangeMaker;

    /** 处理竖屏下动态根据手机旋转方向调整辅流显示方向 */
    private OrientationEventListener mOrientationEventListener;

    /** 监听到上一次视图方向 */
    private int mLastRotation = 0;

    @Override
    protected int getLayoutId() {
        return R.layout.activity_live_room;
    }

    /**
     * 点击屏幕动态显示和隐藏上下工具栏（竖屏处理）
     */
    @Override
    protected void initUniqueViews() {
        ViewGroup.MarginLayoutParams params = (ViewGroup.MarginLayoutParams) bottomLayout.getLayoutParams();
        int defaultMargin = (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, 20, getResources().getDisplayMetrics());
        params.bottomMargin = WindowUtil.getNavigationBarHeight(this) + defaultMargin;
        bottomLayout.setLayoutParams(params);
        mWndArrangeMaker = findViewById(R.id.live_video_grid_layout);  // need first
    }

    /**
     * 根据手机旋转角度获取对应视图角度
     * @param orientation 旋转角度
     * @return 视图角度
     */
    private int getRotation(int orientation) {
        int rotation = -1;
        // 只检测是否有四个角度的改变
        if (orientation > 350 || orientation < 10) {
            // 0度：手机默认竖屏状态（home键在正下方）
            Log.d(TAG, "下");
            rotation = 0;
        } else if (orientation > 80 && orientation < 100) {
            // 90度：手机顺时针旋转90度横屏（home建在左侧）
            Log.d(TAG, "左");
            rotation = 270;
        } else if (orientation > 170 && orientation < 190) {
            // 180度：手机顺时针旋转180度竖屏（home键在上方）
            Log.d(TAG, "上");
            rotation = 180;
        } else if (orientation > 260 && orientation < 280) {
            // 270度：手机顺时针旋转270度横屏，（home键在右侧）
            Log.d(TAG, "右");
            rotation = 90;
        }

        return rotation;
    }

    /**
     * 根据角度转换为对应的枚举类型
     * @param rotation 角度
     * @return 角度类型的枚举值
     */
    private HRTCEnums.HRTCRotationType getRotationType(int rotation) {
        switch (rotation) {
            case 0:
                return HRTC_ROTATION_TYPE_0;
            case 90:
                return HRTC_ROTATION_TYPE_90;
            case 180:
                return HRTC_ROTATION_TYPE_180;
            case 270:
                return HRTC_ROTATION_TYPE_270;
        }

        return HRTC_ROTATION_TYPE_0;
    }

    @Override
    protected void initListener() {
        mWndArrangeMaker.setOnClickListener(view -> {
            if (appBarLayout.getVisibility() == View.VISIBLE) {
                appBarLayout.startAnimation(mTopOutAnim);
                appBarLayout.setVisibility(View.GONE);
                mImmersionBar.hideBar(BarHide.FLAG_HIDE_BAR).init();
                mWndArrangeMaker.moveNameUp();
            } else {
                appBarLayout.setVisibility(View.VISIBLE);
                appBarLayout.startAnimation(mTopInAnim);
                mImmersionBar.hideBar(BarHide.FLAG_SHOW_BAR).init();
                mWndArrangeMaker.moveNameDown();
            }

            if (bottomLayout.getVisibility() == View.VISIBLE) {
                bottomLayout.startAnimation(mBottomOutAnim);
                bottomLayout.setVisibility(View.GONE);
            } else {
                bottomLayout.setVisibility(View.VISIBLE);
                bottomLayout.startAnimation(mBottomInAnim);
            }
        });

        mOrientationEventListener = new OrientationEventListener(this) {
            @Override
            public void onOrientationChanged(int orientation) {
                LogUtil.i(TAG, "onOrientationChanged: " + orientation);
                if (orientation == OrientationEventListener.ORIENTATION_UNKNOWN) {
                    // 手机平放时，检测不到有效的角度，使用0角度模式
                    orientation = 0;
                }

                // 只检测是否有四个角度的改变
                int rotation = getRotation(orientation);
                if (rotation != -1 && rotation != mLastRotation) {
                    mLastRotation = rotation;
                    mHwRtcEngine.setRemoteAuxiliaryStreamViewRotation(getAuxUserId(), getRotationType(rotation));
                }
            }
        };
    }

    /**
     * 本地角色为joiner时的视图处理（竖屏处理）
     */
    @Override
    protected void startJoin() {
        LogUtil.i(TAG, "startJoin!");
        SurfaceView surface = prepareRtcVideo(mLocalUserId, true);
        mWndArrangeMaker.addUserVideoSurface(mLocalUserId, mLocalUserName, surface);
        roomListBtn.setActivated(true);
    }

    /**
     * 本地角色为publisher时的视图处理（竖屏处理）
     */
    @Override
    protected void startPublish() {
        LogUtil.i(TAG, "startPublish !");
        SurfaceView surface = prepareRtcVideo(mLocalUserId, true);
        mWndArrangeMaker.addUserVideoSurface(mLocalUserId, mLocalUserName, surface);
        mRoleChangeBtn.setVisibility(View.INVISIBLE);  // Enable改为Invisible
        mAudioRouteBtn.setEnabled(false);
        roomListBtn.setEnabled(false);
    }

    /**
     * 订阅远端用户
     * @param userId 用户ID
     * @param nickname 用户名
     */
    @Override
    public void renderRemoteUser(String userId, String nickname) {
        LogUtil.i(TAG, "renderRemoteUser userId:" + userId);
        SurfaceView surface = prepareRtcVideo(userId, false);
        mWndArrangeMaker.addUserVideoSurface(userId, nickname, surface);
    }

    /**
     * 订阅辅流
     * @param userId 用户ID
     * @param userName 用户名
     */
    public void renderRemoteAuxView(String userId, String userName) {
        LogUtil.i(TAG, "renderRemoteAuxView userId:" + userId);
        SurfaceView surface = mHwRtcEngine.createRenderer(getApplicationContext());
        mHwRtcEngine.startRemoteAuxiliaryStreamView(userId, surface);
        mHwRtcEngine.updateRemoteAuxiliaryStreamRenderMode(userId, HRTCEnums.HRTCVideoDisplayMode.HRTC_VIDEO_DISPLAY_MODE_FIT, HRTCEnums.HRTCVideoMirrorType.HRTC_VIDEO_MIRROR_TYPE_DISABLE);

        mWndArrangeMaker.addUserVideoSurface("AUX_" + userId, "AUX_" + userName, surface);
    }

    /**
     * 取消订阅所有远端用户
     */
    public void unSelectAllRemoteUsers() {
        LogUtil.i(TAG, "unSelectAllRemoteUsers !");
        for (BeanRoomMember beanRoomMember : roomMemberBeansList) {
            String userId = beanRoomMember.getUserId();
            if (userId.equals(mLocalUserId)) {
                // is self, do nothing
                continue;
            }

            if (beanRoomMember.getIsPlaying()) {
                removeRtcVideo(userId, false);
                mWndArrangeMaker.removeUserVideo(userId);
                beanRoomMember.setIsPlaying(false);
            }
        }
    }

    /**
     * 重新订阅所有加入与会列表中的远端用户
     */
    public void reRenderAllRemoteUsers() {
        LogUtil.i(TAG, "reRenderAllRemoteUsers !");
        for (BeanRoomMember beanRoomMember : roomMemberBeansList) {
            if (getNumberOfPlaying() >= MAX_USER) {
                break;
            }

            String userId = beanRoomMember.getUserId();
            if (userId.equals(mLocalUserId)) {
                // is self, do nothing
                continue;
            }

            renderRemoteUser(userId, beanRoomMember.getNickname());
            beanRoomMember.setIsPlaying(true);
        }
    }

    /**
     * 取消订阅辅流
     * @param userId 用户ID
     */
    private void removeRemoteAuxView(String userId) {
        LogUtil.i(TAG, "removeRemoteAuxView userId:" + userId);
        mHwRtcEngine.stopRemoteAuxiliaryStreamView(userId);
        mWndArrangeMaker.removeUserVideo("AUX_" + userId);
    }

    /**
     * 获取共享辅流的用户ID
     * @return 共享辅流的用户ID
     */
    private String getAuxUserId() {
        LogUtil.i(TAG, "getAuxUserId!");
        return mAuxUserId;
    }

    /**
     * 取消订阅远端用户
     * @param userId 用户ID
     */
    @Override
    protected void removeRemoteUser(String userId) {
        LogUtil.i(TAG, "removeRemoteUser userId:" + userId);
        removeRtcVideo(userId, false);
        mWndArrangeMaker.removeUserVideo(userId);
    }

    /**
     * 重新添加本地用户
     */
    @Override
    protected void reRenderLocalUser() {
        LogUtil.i(TAG, "reRenderLocalUser userId:" + mLocalUserId);
        SurfaceView surface = prepareRtcVideo(mLocalUserId, true);
        String removeUserId = mWndArrangeMaker.reAddLocalUserVideoSurface(mLocalUserId, mLocalUserName, surface);
        if (removeUserId != null) {
            changePlayState(removeUserId);
        }
    }

    /**
     * 删除本地用户
     */
    @Override
    protected void removeLocalUser() {
        LogUtil.e(TAG, "removeLocalUser userId:" + mLocalUserId);
        removeRtcVideo(mLocalUserId, true);
        mWndArrangeMaker.removeUserVideo(mLocalUserId);
    }


    @Override
    protected void onUserJoinedOtherHandler(String roomId, String userId, String nickname) {
        boolean playing = getNumberOfPlaying() < MAX_USER && !hasAux();
        roomMemberBeansList.add(new BeanRoomMember(roomId, userId, nickname, playing)); // 添加房间成员信息
        LogUtil.i(TAG, "onUserJoined roomMemberBeansList refresh adapter userId:" + userId);
        refreshAdapter();
        if (playing) {
            LogUtil.i(TAG, "onUserJoined IsHaveAux NO userId:" + userId);
            renderRemoteUser(userId, nickname);
        }
    }

    /**
     * 获取已订阅的个数
     * @return 已订阅的个数
     */
    public int getNumberOfPlaying() {
        int num = 0;
        for (BeanRoomMember beanRoomMember : roomMemberBeansList) {
            if (beanRoomMember.getIsPlaying()) {
                num++;
            }
        }
        LogUtil.i(TAG, "getNumberOfPlaying num:" + num);
        return num;
    }

    /**
     * 刷新已订阅用户的用户名(针对同一个用户ID加入房间)
     * @param userId 用户ID
     * @param name 用户名
     */
    @Override
    protected void refreshDisplayName(String userId, String name) {
        mWndArrangeMaker.refreshUserName(userId, name);
    }

    /**
     * 用户列表开启远端订阅
     * @param userId 用户ID
     * @param userName 用户名
     * @return 开启订阅是否成功
     */
    @Override
    public boolean openVideo(String userId, String userName) {
        if (getNumberOfPlaying() >= MAX_USER) {
            showToast("open " + userName + " view failed, current displayed view reaches maximum!");
            return false;
        }

        if (hasAux()) {
            showToast("open " + userName + " view failed, sub stream is sharing");
            return false;
        }

        showToast("already opened " + userName + " view");
        renderRemoteUser(userId, userName);
        changePlayState(userId);
        return true;
    }

    @Override
    public void onUserSubStreamAvailable(String roomId, final String userId, final boolean available) {
        if ((available && hasAux()) || (!available && !hasAux())) {
            return;
        }
        runOnUiThread(() -> {
            if (available) {
                if (mOrientationEventListener != null) {
                    mOrientationEventListener.enable();
                }
                LogUtil.i(TAG, "onUserSubStreamAvailable add userId: " + userId);

                if (getRoomMember(userId) == null) {
                    BeanRoomMember member = new BeanRoomMember(roomId, userId, userId, false);
                    roomMemberBeansList.add(member);
                }

                changeAuxState(userId, true);
                mAuxVideoQualityTableView.setVisibility(View.VISIBLE);
                mRoleChangeBtn.setVisibility(View.INVISIBLE);
                removeLocalUser();
                unSelectAllRemoteUsers();
                renderRemoteAuxView(userId, userId);
            } else {
                if (mOrientationEventListener != null) {
                    mOrientationEventListener.disable();
                }
                LogUtil.i(TAG, "onUserSubStreamAvailable remove userId: " + userId);

                mRoleChangeBtn.setVisibility(View.VISIBLE);
                mAuxVideoQualityTableView.setVisibility(View.GONE);
                changeAuxState(userId, false);
                removeRemoteAuxView(userId);
                if (mCurrentRole == HRTC_ROLE_TYPE_JOINER.ordinal()) {
                    reRenderLocalUser();
                }
                LogUtil.i(TAG, "reRenderAllRemoteUsers -1");
                reRenderAllRemoteUsers();
            }
            refreshAdapter();
        });
    }

    @Override
    public void finish() {
        super.finish();
        if (mOrientationEventListener != null) {
            mOrientationEventListener.disable();
        }
    }
}
