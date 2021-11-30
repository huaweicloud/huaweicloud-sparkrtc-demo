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
//  HWRealTimeInfoView.h
//  HwLiveDemo
//
//  Created by HW on 2020/5/19.
//  Copyright © 2020 @YanYuXin. All rights reserved.
//

/*
视频实时信息视图。
会展示音视频的uid、码率、分辨率、帧率、丢包、延时、抖动这些关键信息。每次SDK隔2秒上报一次。
*/

#import <UIKit/UIKit.h>
#import <HWRTCEngine/HWRTCEngine.h>
NS_ASSUME_NONNULL_BEGIN

@interface HWRealTimeInfoView : UIView
@property (nonatomic, assign) BOOL isSubStreamAvailable;
@property (nonatomic, strong) NSString *localUserName;

- (void)addVideoArray:(NSArray *)videoArray;
- (void)addSubStreamVideo:(HWRtcVideoStatsInfo *)videoStatsInfo;
- (void)addAudioArray:(NSArray *)audioArray;
- (void)reloadView;
@end

NS_ASSUME_NONNULL_END
