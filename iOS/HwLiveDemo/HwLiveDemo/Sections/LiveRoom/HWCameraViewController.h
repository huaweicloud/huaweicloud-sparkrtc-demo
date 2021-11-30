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
//  HWCameraViewController.h
//  HWIliveDemo
//
//  Created by HW on 2020/2/17.
//  Copyright © 2020 @YanYuXin. All rights reserved.
//

/*
会中类。该类主要功能是入会之后，展示自己和其他入会者画面。
该类中实现了切换角色、本地画面开关、麦克风开关、扬声器开关，
切换前后置摄像头、音视频实时信息。
*/

#import "HWBaseViewController.h"
#import  <HWRTCEngine/HWRtcEngine.h>

NS_ASSUME_NONNULL_BEGIN

@interface HWCameraViewController : HWBaseViewController
@property (nonatomic, strong) HWRtcEngine *rtcEngine;
@property (nonatomic, strong) NSString *roomID;
@property (nonatomic, strong) NSString *roomName;
@property (nonatomic, assign) HWRtcRole roleType;


@end

NS_ASSUME_NONNULL_END
