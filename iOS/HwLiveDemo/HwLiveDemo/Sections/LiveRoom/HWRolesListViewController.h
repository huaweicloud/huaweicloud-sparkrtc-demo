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
//  HWRolesListViewController.h
//  HWIliveDemo
//
//  Created by HW on 2020/2/18.
//  Copyright © 2020 @YanYuXin. All rights reserved.
//

/*
房间用户列表。会记录在进入该房间以join角色进入的所有用户。可以选择选看某个用户。
*/

#import "HWBaseViewController.h"
#import  <HWRTCEngine/HWRtcEngine.h>
#import "HWVideoSession.h"
#import "HWReusePool.h"
typedef void(^HWRolesListViewStatusBlock)(HWVideoSession * _Nullable videoSession);

NS_ASSUME_NONNULL_BEGIN


@interface HWRolesListViewController : HWBaseViewController

@property (nonatomic, strong) NSMutableArray *viewsArray;
@property (nonatomic, strong) NSMutableArray *memberArray;
/// 当前显示个数
@property (nonatomic, assign) NSInteger showingCount;
///
@property (nonatomic, assign) NSInteger maxShowCount;
@property (nonatomic, copy) HWRolesListViewStatusBlock listViewStatusBlock;
@property (nonatomic, strong) HWRtcEngine *rtcEngine;
///重用池
@property (nonatomic, strong) HWReusePool *reusePool;

@property (nonatomic, assign) HWRtcStreamType streamType;
@property (nonatomic, assign) BOOL isSubStreamAvailable;

@end

NS_ASSUME_NONNULL_END
