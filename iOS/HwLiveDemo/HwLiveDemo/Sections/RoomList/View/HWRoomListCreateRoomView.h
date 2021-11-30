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
//  HWRoomListCreateRoomView.h
//  HwLiveDemo
//
//  Created by HW on 2021/1/20.
//  Copyright © 2021 HW. All rights reserved.
//

/*
主动创建房间试图。该试图可以创建房间，以join的角色加入输入房间名的房间里。
该房间名只可以是英文和数值，不可以输入中文和符号，否则入会会失败。
*/

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@protocol HWRoomListCreateRoomViewDelegate  <NSObject>

@optional
- (void)HWRoomListCreateRoomViewCreateRoomAction:(NSString *)roomName;

@end

@interface HWRoomListCreateRoomView : UIView

@property (nonatomic, weak) id <HWRoomListCreateRoomViewDelegate> delegate;

+(instancetype)initWithXib:(CGRect)frame;
@end

NS_ASSUME_NONNULL_END
