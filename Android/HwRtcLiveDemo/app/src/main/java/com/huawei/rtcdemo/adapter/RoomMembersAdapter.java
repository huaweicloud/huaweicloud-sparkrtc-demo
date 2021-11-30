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

package com.huawei.rtcdemo.adapter;

import android.annotation.SuppressLint;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import com.huawei.rtcdemo.R;
import com.huawei.rtcdemo.bean.BeanRoomMember;
import com.huawei.rtcdemo.interfaces.MemberEventClick;

import java.util.List;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

/**
 * 与会者列表adapter
 */
public class RoomMembersAdapter extends RecyclerView.Adapter {

    /** 成员事件监听器（开启、关闭视频） */
    private MemberEventClick mEventClick;

    /** 成员列表 */
    private List<BeanRoomMember> mEntityList;

    public RoomMembersAdapter(MemberEventClick eventClick, List<BeanRoomMember> entityList) {
        this.mEventClick = eventClick;
        this.mEntityList = entityList;
    }

    @NonNull
    @Override
    public RecyclerView.ViewHolder onCreateViewHolder(@NonNull ViewGroup viewGroup, int viewType) {
        final View view = LayoutInflater.from(viewGroup.getContext()).inflate(R.layout.item_recycler_room_member, viewGroup, false);
        return new RoomMemberRecyclerHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull RecyclerView.ViewHolder holder, int position) {
        BeanRoomMember entity = mEntityList.get(position);
        RoomMemberRecyclerHolder viewHolder = ((RoomMemberRecyclerHolder) holder);

        viewHolder.userNameTv.setText(entity.getNickname());
        viewHolder.userId = entity.getUserId();
        if (entity.isSelf()) {
            viewHolder.videoImg.setVisibility(View.INVISIBLE);
        } else {
            viewHolder.videoImg.setVisibility(View.VISIBLE);
            viewHolder.videoImg.setImageResource(entity.getIsPlaying() ? R.drawable.btn_video_on : R.drawable.btn_video_off);
            viewHolder.videoImg.setTag(entity.getIsPlaying() ? "select" : "unSelect");
            viewHolder.videoImg.setOnClickListener(new View.OnClickListener() {
                @SuppressLint("ResourceType")
                @Override
                public void onClick(View v) {
                    if (viewHolder.videoImg.getTag().equals("unSelect")) {
                        if (mEventClick.openVideo(viewHolder.userId, viewHolder.userNameTv.getText().toString())) {
                            viewHolder.videoImg.setTag("select");
                            viewHolder.videoImg.setImageResource(R.drawable.btn_video_on);
                        }
                    } else {
                        mEventClick.closeVideo(viewHolder.userId, viewHolder.userNameTv.getText().toString());
                        viewHolder.videoImg.setTag("unSelect");
                        viewHolder.videoImg.setImageResource(R.drawable.btn_video_off);
                    }
                }
            });
        }
    }

    @Override
    public int getItemCount() {
        return mEntityList.size();
    }

    private static class RoomMemberRecyclerHolder extends RecyclerView.ViewHolder {
        private TextView userNameTv;
        private ImageView videoImg;
        private String userId;

        public RoomMemberRecyclerHolder(View itemView) {
            super(itemView);
            userNameTv = itemView.findViewById(R.id.item_user_id);
            videoImg = itemView.findViewById(R.id.img_video);
        }
    }
}
