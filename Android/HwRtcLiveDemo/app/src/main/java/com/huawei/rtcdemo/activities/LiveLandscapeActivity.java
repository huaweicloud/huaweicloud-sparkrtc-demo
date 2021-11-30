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

import android.graphics.Color;
import android.util.TypedValue;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.gyf.barlibrary.BarHide;
import com.huawei.rtc.HRTCEngine;
import com.huawei.rtc.utils.HRTCEnums;
import com.huawei.rtcdemo.R;
import com.huawei.rtcdemo.bean.BeanRoomMember;
import com.huawei.rtcdemo.utils.LogUtil;
import com.huawei.rtcdemo.utils.WindowUtil;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import static com.huawei.rtc.utils.HRTCEnums.HRTCSpeakerModel.HRTC_SPEAKER_MODE_SPEAKER;

/**
 * 横屏模式下的直播界面（横屏）
 * 展示自己和与会者画面，可切换摄像头、切换角色、开关麦克风等
 */
public class LiveLandscapeActivity extends LiveBaseActivity {
    /** 横屏模式下最多显示6个普通用户（不包含辅流和老师） */
    private static final int MAX_USER = 6;

    /** 流视图中用户名的左边距 */
    private static final int STAT_LEFT_MARGIN = 50;

    /** 用户名的字体大小 */
    private static final int STAT_TEXT_SIZE = 16;

    /** 保存已订阅用户的用户ID */
    private List<String> mUserIdList = new ArrayList<String>(MAX_USER);

    /** 保存订阅用户的用户ID和对应视图的键值对 */
    private Map<String, ViewGroup> mUserViewList = new HashMap<String, ViewGroup>(MAX_USER);

    private LinearLayout showLayout;
    private LinearLayout mChildSteamLayout;
    private RelativeLayout mSecondStreamLayout;
    private RelativeLayout mMainStreamLayout;

    @Override
    protected int getLayoutId() {
        return R.layout.activity_live_landscape;
    }

    /**
     * 点击屏幕动态显示和隐藏上下工具栏（横屏处理）
     */
    @Override
    protected void initUniqueViews() {
        ViewGroup.MarginLayoutParams params = (ViewGroup.MarginLayoutParams) bottomLayout.getLayoutParams();
        int defaultMargin = (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, 5, getResources().getDisplayMetrics());
        if (WindowUtil.isNavigationBarShow(getWindowManager())) {
            params.bottomMargin = WindowUtil.getNavigationBarHeight(this) + defaultMargin;
        } else {
            params.bottomMargin = defaultMargin;
        }
        bottomLayout.setLayoutParams(params);
        showLayout = findViewById(R.id.show_layout);
        mChildSteamLayout = findViewById(R.id.child_stream_layout);
        mSecondStreamLayout = findViewById(R.id.second_stream_layout);
        mMainStreamLayout = findViewById(R.id.main_stream_layout);
    }

    @Override
    protected void initListener() {
        showLayout.setOnClickListener(view -> {
            if (appBarLayout.getVisibility() == View.VISIBLE) {
                appBarLayout.startAnimation(mTopOutAnim);
                appBarLayout.setVisibility(View.GONE);
                mImmersionBar.hideBar(BarHide.FLAG_HIDE_BAR).init();
            } else {
                appBarLayout.setVisibility(View.VISIBLE);
                appBarLayout.startAnimation(mTopInAnim);
                mImmersionBar.hideBar(BarHide.FLAG_SHOW_BAR).init();
            }

            if (bottomLayout.getVisibility() == View.VISIBLE) {
                bottomLayout.startAnimation(mBottomOutAnim);
                bottomLayout.setVisibility(View.GONE);
            } else {
                bottomLayout.setVisibility(View.VISIBLE);
                bottomLayout.startAnimation(mBottomInAnim);
            }
        });
    }

    /**
     * 用户列表开启远端订阅
     * @param userId 用户ID
     * @param userName 用户名
     * @return 开启订阅是否成功
     */
    @Override
    public boolean openVideo(String userId, String userName) {
        if (!isTeacher(userId) && getNumberOfPlaying() >= MAX_USER) {
            showToast("open " + userName + " view failed, current displayed view reaches maximum!");
            return false;
        }
        showToast("opened " + userName + " view");
        if (isTeacher(userId)) {
            addTeacherSurface(userId, userName);
        } else {
            renderRemoteUser(userId, userName);
        }
        changePlayState(userId);
        return true;
    }

    /**
     * 本地角色为joiner时的视图处理（横屏处理）
     */
    @Override
    protected void startJoin() {
        LogUtil.i(TAG, "startJoin!");
        SurfaceView surface = prepareRtcVideo(mLocalUserId, true);
        addUserVideoSurface(mLocalUserId, mLocalUserName, surface);
        roomListBtn.setActivated(true);
    }

    /**
     * 本地角色为publisher时的视图处理（横屏处理）
     */
    @Override
    protected void startPublish() {
        LogUtil.i(TAG, "startPublish !");
        SurfaceView surface = prepareRtcVideo(mLocalUserId, true);
        addUserVideoSurface(mLocalUserId, mLocalUserName, surface);
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
        addUserVideoSurface(userId, nickname, surface);
    }

    /**
     * 添加老师视图
     * @param userId 用户ID
     * @param nickname 用户名
     */
    public void addTeacherSurface(String userId, String nickname) {
        LogUtil.i(TAG, "addTeacherSurface userId:" + userId);
        SurfaceView surface = prepareRtcVideo(userId, false);
        RelativeLayout.LayoutParams videoLayoutParams = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
        mMainStreamLayout.addView(surface, videoLayoutParams);

        TextView text = new TextView(this);
        text.setId(mMainStreamLayout.hashCode());
        text.setTextColor(Color.DKGRAY);
        text.setTextSize(STAT_TEXT_SIZE);
        text.setText(nickname);
        RelativeLayout.LayoutParams textParams = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        textParams.leftMargin = STAT_LEFT_MARGIN;
        mMainStreamLayout.addView(text, textParams);
    }

    /**
     * 订阅辅流
     * @param userId 用户ID
     */
    public void renderRemoteAuxView(String userId) {
        LogUtil.i(TAG, "renderRemoteAuxView userId:" + userId);
        SurfaceView surface = mHwRtcEngine.createRenderer(getApplicationContext());
        mHwRtcEngine.startRemoteAuxiliaryStreamView(userId, surface);
        mHwRtcEngine.updateRemoteAuxiliaryStreamRenderMode(userId, HRTCEnums.HRTCVideoDisplayMode.HRTC_VIDEO_DISPLAY_MODE_FIT, HRTCEnums.HRTCVideoMirrorType.HRTC_VIDEO_MIRROR_TYPE_DISABLE);

        addSecondStreamSurface(surface);
    }

    /**
     * 取消订阅辅流
     * @param userId 用户ID
     */
    private void removeRemoteAuxView(String userId) {
        LogUtil.i(TAG, "removeRemoteAuxView userId:" + userId);
        mHwRtcEngine.stopRemoteAuxiliaryStreamView(userId);
        mSecondStreamLayout.removeAllViews();
    }

    /**
     * 取消订阅远端用户
     * @param userId 用户ID
     */
    @Override
    protected void removeRemoteUser(String userId) {
        LogUtil.i(TAG, "removeRemoteUser userId:" + userId);
        removeRtcVideo(userId, false);
        if (isTeacher(userId)) {
            mMainStreamLayout.removeAllViews();
        } else {
            removeUserVideo(userId);
        }
    }

    /**
     * 重新添加本地用户
     */
    @Override
    protected void reRenderLocalUser() {
        LogUtil.i(TAG, "reRenderLocalUser userId:" + mLocalUserId);
        SurfaceView surface = prepareRtcVideo(mLocalUserId, true);
        String removeUserId = reAddLocalUserVideoSurface(mLocalUserId, mLocalUserName, surface);
        if (removeUserId != null) {
            changePlayState(removeUserId);
            removeRtcVideo(removeUserId, false);
        }
    }

    /**
     * 删除本地用户
     */
    @Override
    protected void removeLocalUser() {
        LogUtil.i(TAG, "removeLocalUser userId:" + mLocalUserId);
        removeRtcVideo(mLocalUserId, true);
        removeUserVideo(mLocalUserId);
    }

    /**
     * 用户加入回调中不同的处理（横屏处理）
     * @param roomId 房间ID
     * @param userId 用户ID
     * @param nickname 用户名
     */
    @Override
    protected void onUserJoinedOtherHandler(String roomId, String userId, String nickname) {
        boolean playing = getNumberOfPlaying() < MAX_USER || isTeacher(userId);
        roomMemberBeansList.add(new BeanRoomMember(roomId, userId, nickname, playing)); // 添加房间成员信息
        LogUtil.i(TAG, "onUserJoined roomMemberBeansList refresh adapter, userId:" + userId);
        refreshAdapter();

        if (isTeacher(userId)) {
            addTeacherSurface(userId, nickname);
            return;
        }

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
        for (int i = 0; i < roomMemberBeansList.size(); i++) {
            if (isTeacher(roomMemberBeansList.get(i).getUserId())) {
                continue;
            }
            if (roomMemberBeansList.get(i).getIsPlaying()) {
                num++;
            }
        }
        LogUtil.i(TAG, "getNumberOfPlaying num:" + num);
        return num;
    }

    /**
     * 刷新已订阅用户的用户名 (针对同一个用户ID加入房间)
     * @param userId 用户ID
     * @param name 用户名
     */
    @Override
    protected void refreshDisplayName(String userId, String name) {
        LogUtil.d(TAG, "refreshDisplayName, user id:" + userId + ", user new name:" + name);
        if (mUserViewList.containsKey(userId)) {
            ViewGroup v = mUserViewList.get(userId);
            if (v != null) {
                TextView t = v.findViewById(v.hashCode());
                t.setText(name);
            }
            return;
        }
        if (isTeacher(userId)) {
            TextView t = mMainStreamLayout.findViewById(mMainStreamLayout.hashCode());
            if (t != null) {
                t.setText(name);
            }
        }
    }

    /**
     * 判断用户是否是老师
     * @param userId 用户ID
     * @return 是否是老师
     */
    public boolean isTeacher(String userId) {
        return userId.startsWith("teacher_");
    }

    @Override
    public void onUserSubStreamAvailable(String roomId, final String userId, final boolean available) {
        if ((available && hasAux()) || (!available && !hasAux())) {
            return;
        }
        runOnUiThread(() -> {
            if (available) {
                LogUtil.i(TAG, "onUserSubStreamAvailable add userId: " + userId);
                changeAuxState(userId, true);
                mAuxVideoQualityTableView.setVisibility(View.VISIBLE);
                renderRemoteAuxView(userId);
            } else {
                LogUtil.i(TAG, "onUserSubStreamAvailable remove userId: " + userId);
                mAuxVideoQualityTableView.setVisibility(View.GONE);
                changeAuxState(userId, false);
                removeRemoteAuxView(userId);
            }
            refreshAdapter();
        });
    }

    /**
     * 添加辅流视图
     * @param surface 辅流视图
     */
    private void addSecondStreamSurface(SurfaceView surface) {
        RelativeLayout.LayoutParams videoLayoutParams = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
        mSecondStreamLayout.addView(surface, videoLayoutParams);
    }

    /**
     * 添加普通视频流
     * @param userId 用户ID
     * @param nickname 用户名
     * @param surface 视图
     */
    private void addUserVideoSurface(String userId, String nickname, SurfaceView surface) {
        ViewGroup videoView = createVideoView(surface, nickname);
        if (mUserViewList.size() >= MAX_USER) {
            LogUtil.i(TAG, "addUserVideoSurface mUserViewList>=MAX_USER " + userId);
            if (mUserViewList.containsKey(userId)) {
                mUserViewList.put(userId, videoView);
                int index = 0;
                for (int i = 0;i < mUserIdList.size();i++) {
                    if (mUserIdList.get(i).equals(userId)) {
                        index = i;
                        break;
                    }
                }
                LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.MATCH_PARENT, 1);
                mChildSteamLayout.addView(videoView,index,layoutParams);
                refreshRootView();
            } else {
                LogUtil.e(TAG, "maximum view reached:" + mUserViewList.size());
            }
        } else {
            if (!mUserIdList.contains(userId)) {
                LogUtil.i(TAG, "addUserVideoSurface mUserIdList userId: " + userId);
                mUserIdList.add(userId);
            }
            mUserViewList.put(userId, videoView);
            LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.MATCH_PARENT, 1);
            mChildSteamLayout.addView(videoView,layoutParams);
            refreshRootView();
        }
    }

    /**
     * 封装视频流视图和用户名到一个布局
     * @param surface 视频流视图
     * @param userLabel 用户名
     * @return 包含视频流视图和用户名的父布局
     */
    private ViewGroup createVideoView(SurfaceView surface, String userLabel) {
        RelativeLayout layout = new RelativeLayout(this);
        layout.setId(surface.hashCode());

        RelativeLayout.LayoutParams videoLayoutParams = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
        layout.addView(surface, videoLayoutParams);

        TextView text = new TextView(this);
        text.setId(layout.hashCode());
        RelativeLayout.LayoutParams textParams = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        textParams.leftMargin = STAT_LEFT_MARGIN;
        text.setTextColor(Color.DKGRAY);
        text.setTextSize(STAT_TEXT_SIZE);
        text.setText(userLabel);

        layout.addView(text, textParams);
        return layout;
    }

    /**
     * 删除用户视图
     * @param userId 用户ID
     */
    public void removeUserVideo(String userId) {
        LogUtil.i(TAG, "removeUserVideo userId:" + userId);
        if (mUserIdList.contains(userId)) {
            mUserIdList.remove(userId);
            ViewGroup removeView = mUserViewList.remove(userId);
            mChildSteamLayout.removeView(removeView);
            refreshRootView();
        }
    }

    /**
     * 重新添加本地用户的视图
     * @param userId 用户ID
     * @param nickname 用户名
     * @param surface 流视图
     * @return 返回超过最大个数被取消订阅的用户ID
     */
    private String reAddLocalUserVideoSurface(String userId, String nickname, SurfaceView surface) {
        String removeUserId = null;
        if (surface == null) {
            return null;
        }
        if (mUserViewList.size() >= MAX_USER) {
            if (!mUserViewList.containsKey(userId)) {
                removeUserId = mUserIdList.get(mUserIdList.size() - 1);
                ViewGroup removeView = mUserViewList.remove(mUserIdList.get(mUserIdList.size() - 1));
                mUserIdList.remove(mUserIdList.get(mUserIdList.size() - 1));
                mChildSteamLayout.removeView(removeView);

                mUserIdList.add(0, userId);
            }
        } else {
            if (!mUserIdList.contains(userId)) {
                LogUtil.i(TAG, "reAddLocalUserVideoSurface mUserIdList userId: " + userId);
                mUserIdList.add(0, userId);
            }
        }
        ViewGroup videoView = createVideoView(surface, nickname);
        mUserViewList.put(userId, videoView);
        LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.MATCH_PARENT, 1);
        mChildSteamLayout.addView(videoView,0,layoutParams);
        refreshRootView();
        return removeUserId;
    }

    /**
     * 刷新布局
     */
    private void refreshRootView() {
        mChildSteamLayout.getRootView().requestLayout();
        mChildSteamLayout.getRootView().invalidate();
    }
}
