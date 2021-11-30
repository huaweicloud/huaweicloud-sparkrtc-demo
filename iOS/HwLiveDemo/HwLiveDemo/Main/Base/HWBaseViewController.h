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
//  HWBaseViewController.h
//  HW
//
//  Created by HW on 2019/7/22.
//  Copyright © 2019年 HW. All rights reserved.
//

#import <UIKit/UIKit.h>
NS_ASSUME_NONNULL_BEGIN

@interface HWBaseViewController : UIViewController
/// 导航栏
@property (nonatomic, strong) UIView *weNavigationView;

/// 返回按钮图片
@property (nonatomic, strong) UIImage *backImage;

/// 标题
@property (nonatomic, strong) NSString *navTitle;

/// 是否隐藏自定义导航栏
@property (nonatomic, assign) BOOL isHiddenNav;

/// 是否隐藏自定义左返回事件
@property (nonatomic, assign) BOOL isHiddenBak;

@property (nonatomic, readonly) CGFloat navHeight;

/**
 返回App页面
 
 @param notification 通知
 */
- (void)observeApplicationWillEnterForeground:(NSNotification *)notification;

/**
 返回后台
 
 @param notification 通知
 */
- (void)observeApplicationDidEnterBackground:(NSNotification *)notification;

- (void)popViewControl:(UIButton *)but;

@end

NS_ASSUME_NONNULL_END
