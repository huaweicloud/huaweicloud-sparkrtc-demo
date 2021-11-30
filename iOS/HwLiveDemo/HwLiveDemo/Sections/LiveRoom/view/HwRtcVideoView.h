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
//  HwRtcVideoView.h
//  HwLiveDemo
//
//  Created by HW on 2020/4/2.
//  Copyright © 2020 @YanYuXin. All rights reserved.
//

/*会中用户展示视图。*/

#import  <HWRTCEngine/HWRtcObject.h>

NS_ASSUME_NONNULL_BEGIN


@interface HwRtcVideoView : HWRtcView
@property (nonatomic, assign) BOOL isFull;
@property (nonatomic, assign) BOOL isSubStreamAvailable;
- (void)setUserName:(NSString *)userName;
@end

NS_ASSUME_NONNULL_END
