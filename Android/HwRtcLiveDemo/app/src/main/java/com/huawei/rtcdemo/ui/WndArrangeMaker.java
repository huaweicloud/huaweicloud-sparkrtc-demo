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

package com.huawei.rtcdemo.ui;

import android.content.Context;
import android.graphics.Color;
import android.util.AttributeSet;
import android.util.Log;
import android.util.TypedValue;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.huawei.rtcdemo.R;
import com.huawei.rtcdemo.utils.LogUtil;
import com.huawei.rtcdemo.utils.WindowUtil;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * 竖屏模式下流视图处理
 */
public class WndArrangeMaker extends RelativeLayout {
    private static final String TAG = "WndArrangeMaker";

    /** 竖屏模式下订阅的最大用户（屏幕最大显示个数） */
    private static final int MAX_USER = 4;

    /** 流视图中用户名的左边距 */
    private static final int STAT_LEFT_MARGIN = 50;

    /** 顶部工具栏隐藏状态下流视图中用户名的顶部边距 */
    private static final int STAT_TOP_MARGIN_DEFAULT = 30;

    /** 顶部工具栏显示状态下流视图中用户名的顶部边距 */
    private static int STAT_TOP_MARGIN_MOVE;

    /** 流视图中用户名的字体大小 */
    private static final int STAT_TEXT_SIZE = 16;

    /** 当前流视图中用户名是否处于顶部工具栏隐藏状态下显示的位置 */
    private boolean mIsMoveUp = false;

    /** 保存已订阅用户的用户ID */
    private List<String> mUserIdList = new ArrayList<String>(MAX_USER);

    /** 保存订阅用户的用户ID和对应视图的键值对 */
    private Map<String, ViewGroup> mUserViewList = new HashMap<String, ViewGroup>(MAX_USER);

    public WndArrangeMaker(Context context) {
        super(context);
        init(context);
    }

    public WndArrangeMaker(Context context, AttributeSet attrs) {
        super(context, attrs);
        init(context);
    }

    public WndArrangeMaker(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init(context);
    }

    private void init(Context context) {
        setBackgroundResource(R.color.gray_light);
        int systemStatusBarHeight = WindowUtil.getSystemStatusBarHeight(context);
        float titleHeight = TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, 48, getResources().getDisplayMetrics());
        STAT_TOP_MARGIN_MOVE = (int) (systemStatusBarHeight + titleHeight + STAT_TOP_MARGIN_DEFAULT);
    }

    /**
     * 添加对应用户视频流到布局
     * @param userId 用户ID
     * @param nickname 用户名
     * @param surface 流视图
     */
    public void addUserVideoSurface(String userId, String nickname, SurfaceView surface) {
        LogUtil.i(TAG, "addUserVideoSurface surface : " + userId + ", userName: " + nickname);
        if (surface == null) {
            LogUtil.e(TAG, "addUserVideoSurface surface == null " + userId);
            return;
        }

        LogUtil.i(TAG, "addUserVideoSurface surface != null " + userId);

        if (mUserViewList.size() >= MAX_USER) {
            LogUtil.i(TAG, "addUserVideoSurface mUserViewList>=MAX_USER " + userId);
            if (mUserViewList.containsKey(userId)) {
                mUserViewList.put(userId, createVideoView(surface, nickname));
            } else {
                LogUtil.e(TAG, "maximum view reached:" + mUserViewList.size());
                return;
            }
        } else {
            if (!mUserIdList.contains(userId)) {
                LogUtil.i(TAG, "addUserVideoSurface mUserIdList userId: " + userId);
                mUserIdList.add(userId);
            }
            mUserViewList.put(userId, createVideoView(surface, nickname));
        }
        requestGridLayout();
    }

    /**
     * 重新添加本地用户的视图
     * @param userId 用户ID
     * @param nickname 用户名
     * @param surface 流视图
     * @return 返回超过最大个数被取消订阅的用户ID
     */
    public String reAddLocalUserVideoSurface(String userId, String nickname, SurfaceView surface) {
        String removeUserId = null;
        if (surface == null) {
            return null;
        }

        if (mUserViewList.size() >= MAX_USER) {
            if (mUserViewList.containsKey(userId)) {
                mUserViewList.put(userId, createVideoView(surface, nickname));
            } else {
                removeUserId = mUserIdList.get(mUserIdList.size() - 1);
                mUserViewList.remove(mUserIdList.get(mUserIdList.size() - 1));
                mUserIdList.remove(mUserIdList.get(mUserIdList.size() - 1));

                mUserIdList.add(0, userId);
                mUserViewList.put(userId, createVideoView(surface, nickname));
            }
        } else {
            if (!mUserIdList.contains(userId)) {
                LogUtil.i(TAG, "addUserVideoSurface mUserIdList userId: " + userId);
                mUserIdList.add(0, userId);
            }
            mUserViewList.put(userId, createVideoView(surface, nickname));
        }
        requestGridLayout();
        return removeUserId;
    }

    /**
     * 封装视频流视图和用户名到一个布局
     * @param surface 视频流视图
     * @param userLabel 用户名
     * @return 包含视频流视图和用户名的父布局
     */
    private ViewGroup createVideoView(SurfaceView surface, String userLabel) {
        RelativeLayout layout = new RelativeLayout(getContext());

        layout.setId(surface.hashCode());

        LayoutParams videoLayoutParams = new LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
        layout.addView(surface, videoLayoutParams);

        TextView text = new TextView(getContext());
        text.setId(layout.hashCode());
        LayoutParams textParams = new LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        textParams.addRule(RelativeLayout.ALIGN_PARENT_TOP, RelativeLayout.TRUE);
        textParams.leftMargin = STAT_LEFT_MARGIN;
        textParams.topMargin = STAT_TOP_MARGIN_DEFAULT;
        text.setTextColor(Color.DKGRAY);
        text.setTextSize(STAT_TEXT_SIZE);
        text.setText(userLabel);

        layout.addView(text, textParams);
        return layout;
    }

    /**
     * 根据用户ID刷新流视图中显示的用户名
     * @param userId 用户ID
     * @param name 用户名
     */
    public void refreshUserName(String userId, String name) {
        LogUtil.d(TAG, "refreshUserName : " + name);
        if (mUserViewList.containsKey(userId)) {
            LogUtil.d(TAG, "refreshUserName list contains user");
            ViewGroup v = mUserViewList.get(userId);
            if (v != null) {
                LogUtil.d(TAG, "refreshUserName got textView, and refresh name.");
                TextView txt = v.findViewById(v.hashCode());
                txt.setText(name);
            }
        }
    }

    /**
     * 根据用户ID删除对应流视图
     * @param userId 用户ID
     */
    public void removeUserVideo(String userId) {
        LogUtil.i(TAG, "removeUserVideo userId:" + userId);
        if (mUserIdList.contains(userId)) {
            mUserIdList.remove(userId);
            mUserViewList.remove(userId);
        }
        requestGridLayout();
    }

    /**
     * 刷新布局
     */
    private void requestGridLayout() {
        removeAllViews();
        layout(mUserIdList.size());
    }

    /**
     * 动态添加布局
     * @param size 已订阅人数
     */
    private void layout(int size) {
        LogUtil.i(TAG, "layout size:" + size);
        LayoutParams[] params = getParams(size);
        for (int i = 0; i < size; i++) {
            LogUtil.i(TAG, "userId:" + mUserIdList.get(i));
            addView(mUserViewList.get(mUserIdList.get(i)), params[i]);
        }
    }

    /**
     * 动态处理不同订阅人数下的布局位置显示
     * @param size 已订阅人数
     * @return 获取对应的布局显示规则
     */
    private LayoutParams[] getParams(int size) {
        int width = getMeasuredWidth();
        int height = getMeasuredHeight();

        LayoutParams[] array = new LayoutParams[size];

        for (int i = 0; i < size; i++) {
            if (i == 0) {
                array[0] = new LayoutParams(
                        LayoutParams.MATCH_PARENT,
                        LayoutParams.MATCH_PARENT);
                array[0].addRule(RelativeLayout.ALIGN_PARENT_TOP, RelativeLayout.TRUE);
                array[0].addRule(RelativeLayout.ALIGN_PARENT_LEFT, RelativeLayout.TRUE);

                View text = mUserViewList.get(mUserIdList.get(0)).getChildAt(1);
                LayoutParams layoutParams = (RelativeLayout.LayoutParams) text.getLayoutParams();
                if (mIsMoveUp) {
                    layoutParams.topMargin = STAT_TOP_MARGIN_DEFAULT;
                } else {
                    layoutParams.topMargin = STAT_TOP_MARGIN_MOVE;
                }

                text.setLayoutParams(layoutParams);
            } else if (i == 1) {
                array[1] = new LayoutParams(width, height / 2);
                array[0].height = array[1].height;
                array[1].width = width; // 解决部分手机多人连麦时出现黑边框问题
                array[1].addRule(RelativeLayout.BELOW, mUserViewList.get(mUserIdList.get(0)).getId());
                array[1].addRule(RelativeLayout.ALIGN_PARENT_LEFT, RelativeLayout.TRUE);
                array[1].addRule(RelativeLayout.ALIGN_PARENT_LEFT, RelativeLayout.TRUE);

                View text = mUserViewList.get(mUserIdList.get(1)).getChildAt(1);
                LayoutParams layoutParams = (RelativeLayout.LayoutParams) text.getLayoutParams();
                layoutParams.topMargin = STAT_TOP_MARGIN_DEFAULT;
                text.setLayoutParams(layoutParams);
            } else if (i == 2) {
                array[i] = new LayoutParams(width / 2, height / 2);
                array[i - 1].width = array[i].width;
                array[i].addRule(RelativeLayout.RIGHT_OF, mUserViewList.get(mUserIdList.get(i - 1)).getId());
                array[i].addRule(RelativeLayout.ALIGN_TOP, mUserViewList.get(mUserIdList.get(i - 1)).getId());

                View text = mUserViewList.get(mUserIdList.get(i)).getChildAt(1);
                LayoutParams layoutParams = (RelativeLayout.LayoutParams) text.getLayoutParams();
                layoutParams.topMargin = STAT_TOP_MARGIN_DEFAULT;
                text.setLayoutParams(layoutParams);
            } else if (i == 3) {
                array[i] = new LayoutParams(width / 2, height / 2);
                array[0].width = width / 2;
                array[1].addRule(RelativeLayout.BELOW, 0);
                array[1].addRule(RelativeLayout.ALIGN_PARENT_LEFT, 0);
                array[1].addRule(RelativeLayout.RIGHT_OF, mUserViewList.get(mUserIdList.get(0)).getId());
                array[1].addRule(RelativeLayout.ALIGN_PARENT_TOP, RelativeLayout.TRUE);
                array[2].addRule(RelativeLayout.ALIGN_PARENT_LEFT, RelativeLayout.TRUE);
                array[2].addRule(RelativeLayout.RIGHT_OF, 0);
                array[2].addRule(RelativeLayout.ALIGN_TOP, 0);
                array[2].addRule(RelativeLayout.BELOW, mUserViewList.get(mUserIdList.get(0)).getId());
                array[3].addRule(RelativeLayout.BELOW, mUserViewList.get(mUserIdList.get(1)).getId());
                array[3].addRule(RelativeLayout.RIGHT_OF, mUserViewList.get(mUserIdList.get(2)).getId());

                View text = mUserViewList.get(mUserIdList.get(1)).getChildAt(1);
                LayoutParams layoutParams = (RelativeLayout.LayoutParams) text.getLayoutParams();
                if (mIsMoveUp) {
                    layoutParams.topMargin = STAT_TOP_MARGIN_DEFAULT;
                } else {
                    layoutParams.topMargin = STAT_TOP_MARGIN_MOVE;
                }
                text.setLayoutParams(layoutParams);
            }
        }
        return array;
    }

    /**
     * 竖屏模式下顶部流视图的文字根据顶部工具栏显示动态上移
     */
    public void moveNameUp() {
        mIsMoveUp = true;
        switch (mUserIdList.size()) {
            case 1:
            case 2:
            case 3:
                View text = mUserViewList.get(mUserIdList.get(0)).getChildAt(1);
                LayoutParams layoutParams = (RelativeLayout.LayoutParams) text.getLayoutParams();
                layoutParams.topMargin = STAT_TOP_MARGIN_DEFAULT;
                text.setLayoutParams(layoutParams);
                break;
            case 4:
                View text1 = mUserViewList.get(mUserIdList.get(0)).getChildAt(1);
                LayoutParams layoutParams1 = (RelativeLayout.LayoutParams) text1.getLayoutParams();
                layoutParams1.topMargin = STAT_TOP_MARGIN_DEFAULT;
                text1.setLayoutParams(layoutParams1);
                View text2 = mUserViewList.get(mUserIdList.get(1)).getChildAt(1);
                LayoutParams layoutParams2 = (RelativeLayout.LayoutParams) text2.getLayoutParams();
                layoutParams2.topMargin = STAT_TOP_MARGIN_DEFAULT;
                text2.setLayoutParams(layoutParams2);
                break;
            default:
                break;
        }

    }

    /**
     * 竖屏模式下顶部流视图的文字根据顶部工具栏显示动态下移
     */
    public void moveNameDown() {
        mIsMoveUp = false;
        switch (mUserIdList.size()) {
            case 1:
            case 2:
            case 3:
                View text = mUserViewList.get(mUserIdList.get(0)).getChildAt(1);
                LayoutParams layoutParams = (RelativeLayout.LayoutParams) text.getLayoutParams();
                layoutParams.topMargin = STAT_TOP_MARGIN_MOVE;
                text.setLayoutParams(layoutParams);
                break;
            case 4:
                View text1 = mUserViewList.get(mUserIdList.get(0)).getChildAt(1);
                LayoutParams layoutParams1 = (RelativeLayout.LayoutParams) text1.getLayoutParams();
                layoutParams1.topMargin = STAT_TOP_MARGIN_MOVE;
                text1.setLayoutParams(layoutParams1);
                View text2 = mUserViewList.get(mUserIdList.get(1)).getChildAt(1);
                LayoutParams layoutParams2 = (RelativeLayout.LayoutParams) text2.getLayoutParams();
                layoutParams2.topMargin = STAT_TOP_MARGIN_MOVE;
                text2.setLayoutParams(layoutParams2);
                break;
            default:
                break;
        }
    }

    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        clearAllVideo();
    }

    private void clearAllVideo() {
        removeAllViews();
        mUserViewList.clear();
        mUserIdList.clear();
    }
}
