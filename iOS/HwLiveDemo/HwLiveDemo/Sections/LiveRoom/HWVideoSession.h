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
//  HWVideoSession.m
//  HwLiveDemo
//
//  Created by HW on 2020/4/2.
//  Copyright © 2020 @YanYuXin. All rights reserved.
//

/*
用户model。记录每个用户的信息
*/
#import <Foundation/Foundation.h>
#import "HwRtcVideoView.h"
NS_ASSUME_NONNULL_BEGIN


@interface HWVideoSession : NSObject
@property (nonatomic, strong) NSString *userName;
@property (nonatomic, strong) NSString *userid;
@property (nonatomic, strong) NSString *roomid;
@property (nonatomic, strong) HwRtcVideoView * __nullable videoView;
@property (nonatomic, strong) HWRtcVideoCanvas *canvas;
@property (nonatomic, assign) BOOL isPlaying;
@property (nonatomic, assign) BOOL isSubStream;//是否为辅流

- (instancetype)initWithUserId:(nonnull NSString *)userId;


@end

NS_ASSUME_NONNULL_END
