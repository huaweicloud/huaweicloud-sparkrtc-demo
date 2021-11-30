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

import android.app.Activity;
import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.drawable.Drawable;
import android.text.TextUtils;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.constraintlayout.widget.ConstraintLayout;

import com.huawei.rtcdemo.R;

/**
 * 新标题栏
 */
public class NewTitleBar extends ConstraintLayout {
    /** 标题 */
    private TextView mTvTitle;

    /** 左边文字 */
    private TextView mTvLeft;

    /** 左边图标 */
    private ImageView mIvLeft;

    private ImageView mIvLeft2;

    /** 右边文字 */
    private TextView mTvRight;

    /** 右边第二图标 */
    private ImageView mIvRight2;

    /** 右边图标 */
    private ImageView mIvRight;


    public NewTitleBar(Context context) {
        super(context);
        init(context, null);
    }

    public NewTitleBar(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        init(context, attrs);
    }

    public NewTitleBar(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init(context, attrs);
    }

    private void init(Context context, AttributeSet attrs) {
        // 不要轻易修改它的布局，如果你要改变它的背景等，请在xml中定义它的地方设置。
        LayoutInflater.from(context).inflate(R.layout.layout_new_title_bar, this, true);
        mTvTitle = findViewById(R.id.tv_title);
        mIvLeft2 = findViewById(R.id.iv_left_2);
        mTvLeft = findViewById(R.id.tv_left);
        mTvRight = findViewById(R.id.tv_right);
        mIvRight = findViewById(R.id.iv_right);
        mIvRight2 = findViewById(R.id.iv_right_2);
        mIvLeft = findViewById(R.id.iv_back);
        if (isInEditMode()) {
            return;
        }
        // 点击返回。
        mIvLeft.setOnClickListener(v -> {
            if (v.getContext() instanceof Activity) {
                ((Activity) v.getContext()).onBackPressed();
            }
        });
        if (attrs != null) {
            TypedArray typedArray = context.obtainStyledAttributes(attrs, R.styleable.NewTitleBar);

            // 是否展示返回图标。
            mIvLeft.setVisibility(typedArray.getBoolean(R.styleable.NewTitleBar_display_as_home, false)
                    ? GONE : VISIBLE);

            String rightText = typedArray.getString(R.styleable.NewTitleBar_right_text);
            setRightText(rightText);
            int rightTextColor = typedArray.getColor(R.styleable.NewTitleBar_right_text_color, -1);
            if (rightTextColor != -1) {
                mTvRight.setTextColor(rightTextColor);
            }


            String leftText = typedArray.getString(R.styleable.NewTitleBar_left_text);
            setLeftText(leftText);
            int leftTextColor = typedArray.getColor(R.styleable.NewTitleBar_left_text_color, -1);
            if (leftTextColor != -1) {
                mTvLeft.setTextColor(leftTextColor);
            }

            String centerText = typedArray.getString(R.styleable.NewTitleBar_center_text);
            setTitle(centerText);
            int centerTextColor = typedArray.getColor(R.styleable.NewTitleBar_center_color, -1);
            if (centerTextColor != -1) {
                mTvTitle.setTextColor(centerTextColor);
            }
            int bgColor = typedArray.getColor(R.styleable.NewTitleBar_title_bg_color, -1);
            setBackgroundColor(bgColor != -1 ? bgColor : getResources().getColor(R.color.black));

            Drawable drawable = typedArray.getDrawable(R.styleable.NewTitleBar_right_icon);
            if (drawable != null) {
                mIvRight.setImageDrawable(drawable);
                mIvRight.setVisibility(VISIBLE);
            }

            Drawable drawable2 = typedArray.getDrawable(R.styleable.NewTitleBar_right2_icon);
            if (drawable2 != null) {
                mIvRight2.setImageDrawable(drawable2);
                mIvRight2.setVisibility(VISIBLE);
            }

            Drawable left2 = typedArray.getDrawable(R.styleable.NewTitleBar_left2_icon);
            if (left2 != null) {
                mIvLeft2.setImageDrawable(left2);
                mIvLeft2.setVisibility(VISIBLE);
            }

            Drawable leftDrawable = typedArray.getDrawable(R.styleable.NewTitleBar_left_icon);
            if (leftDrawable != null) {
                mIvLeft.setImageDrawable(leftDrawable);
            }
            typedArray.recycle();
        }
    }


    public void setTitle(int resId) {
        if (mTvTitle != null) {
            mTvTitle.setText(resId);
        }
    }

    /**
     * 设置标题
     * @param title 标题
     */
    public void setTitle(CharSequence title) {
        if (mTvTitle != null) {
            mTvTitle.setText(TextUtils.isEmpty(title) ? "" : title);
        }
    }

    public void setRightClickListener(OnClickListener clickListener) {
        mTvRight.setOnClickListener(clickListener);
    }

    public void setRightTextClickListner(OnClickListener clickListener) {
        mTvRight.setOnClickListener(clickListener);
    }

    /**
     * 设置右边图片点击事件。
     *
     * @param clickListener clickListener
     */
    public void setRightIconClickListener(OnClickListener clickListener) {
        mIvRight.setOnClickListener(clickListener);
    }

    /**
     * 设置右边2图片点击事件。
     *
     * @param clickListener
     */

    public void setRight2IconClickListener(OnClickListener clickListener) {
        mIvRight2.setOnClickListener(clickListener);
    }

    /**
     * 设置左边2图片点击事件。
     *
     * @param clickListener
     */

    public void setLeft2IconClickListener(OnClickListener clickListener) {
        mIvLeft2.setOnClickListener(clickListener);
    }

    public void setDisplayAsHome() {
        findViewById(R.id.iv_back).setVisibility(GONE);
    }

    public void setRightText(String rightText) {
        mTvRight.setText(rightText);
    }

    public void setLeftClickListener(OnClickListener onClickListener) {
        mTvLeft.setOnClickListener(onClickListener);
    }

    public TextView getTvRight() {
        return mTvRight;
    }

    public void setLeftText(String leftText) {
        mTvLeft.setText(leftText);
    }

    public ImageView getIvRight() {
        return mIvRight;
    }

    public ImageView getIvLeft() {
        return mIvLeft;
    }

    public TextView getTvTitle() {
        return mTvTitle;
    }

    public TextView getTvLeft() {
        return mTvLeft;
    }

    public ImageView getmIvRight2() {
        return mIvRight2;
    }

    public ImageView getmIvLeft2() {
        return mIvLeft2;
    }
}
