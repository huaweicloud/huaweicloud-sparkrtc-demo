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
 *     http://www.apache.org/licenses/LICENSE-2.0  *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
//
//  HWReusePool.m
//  HwLiveDemo
//
//  Created by HW on 2020/4/2.
//  Copyright © 2020 @YanYuXin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HwRtcVideoView.h"

NS_ASSUME_NONNULL_BEGIN

@interface HWReusePool : NSObject
/// 复用个数
@property (nonatomic, assign) NSUInteger count;
- (HwRtcVideoView *)dequeueReusableView;
- (void)addVideoView:(HwRtcVideoView *)videoView;
- (void)removeVideoView:(HwRtcVideoView *)view;
- (void)reset;


@end

NS_ASSUME_NONNULL_END
