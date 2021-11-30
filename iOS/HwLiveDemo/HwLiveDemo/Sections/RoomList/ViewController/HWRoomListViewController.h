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
//  HWRoomListViewController.h
//  HwLiveDemo
//
//  Created by HW on 2020/7/13.
//  Copyright © 2020 HW. All rights reserved.
//

/*
房间列表界面。里面的数值是写死的，不需要通过服务器获取，客户需要联网获取他人的房间名，
可以通过联网获取他人的房间信息，进而达到联网进入他人房间。
*/

#import "HWBaseViewController.h"

NS_ASSUME_NONNULL_BEGIN

@interface HWRoomListViewController : HWBaseViewController
@end

NS_ASSUME_NONNULL_END
