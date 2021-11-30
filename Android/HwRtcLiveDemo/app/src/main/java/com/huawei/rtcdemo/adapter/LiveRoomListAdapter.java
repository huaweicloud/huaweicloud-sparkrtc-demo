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

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;
import androidx.recyclerview.widget.RecyclerView.Adapter;

import com.huawei.rtcdemo.R;

/**
 * 房间列表adapter
 */
public class LiveRoomListAdapter extends Adapter<LiveRoomListAdapter.LiveRoomHolder> {

    /** 图片资源数组 */
    private int[] defaultResIndexs = new int[]{R.drawable.img_room_item_bg_1, R.drawable.img_room_item_bg_2, R.drawable.img_room_item_bg_3, R.drawable.img_room_item_bg_4,
            R.drawable.img_room_item_bg_5, R.drawable.img_room_item_bg_6, R.drawable.img_room_item_bg_7, R.drawable.img_room_item_bg_8
    };

    /** 房间名数组 */
    private String[] mRoomIds;

    /** 点击事件监听器 */
    private OnItemClickListener mOnItemClickListener;

    public LiveRoomListAdapter(String[] roomIds) {
        mRoomIds = new String[roomIds.length];
        System.arraycopy(roomIds, 0, mRoomIds, 0, roomIds.length);
    }

    public void setOnItemClickListener (OnItemClickListener listener) {
        mOnItemClickListener = listener;
    }

    @NonNull
    @Override
    public LiveRoomHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.item_live_room_list, parent, false);
        return new LiveRoomHolder(view);
    }

    @Override
    public int getItemCount() {
        return mRoomIds.length;
    }

    @Override
    public void onBindViewHolder(@NonNull LiveRoomHolder holder, int position) {
        String roomId = mRoomIds[position];
        holder.liveImg.setImageResource(defaultResIndexs[position % defaultResIndexs.length]);
        holder.liveRoomName.setText(roomId);
        holder.itemView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (mOnItemClickListener != null) {
                    mOnItemClickListener.onItemClick(position);
                }
            }
        });
    }

    public static class LiveRoomHolder extends RecyclerView.ViewHolder {
        ImageView liveImg;
        TextView liveRoomName;

        public LiveRoomHolder(View itemView) {
            super(itemView);
            liveImg = itemView.findViewById(R.id.iv_live_img);
            liveRoomName = itemView.findViewById(R.id.tv_live_room_name);
        }
    }

    public interface OnItemClickListener {
        void onItemClick(int position);
    }
}
