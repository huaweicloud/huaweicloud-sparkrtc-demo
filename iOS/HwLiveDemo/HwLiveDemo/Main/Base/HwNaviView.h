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
//  HwNaviView.h
//  HwLiveDemo
//
//  Created by HW on 2019/4/3.
//  Copyright © 2019年 YanYuXin. All rights reserved.
//

#import <UIKit/UIKit.h>

@class HwNaviView;

@protocol UINavigationViewDelegate<NSObject>

@optional

/**
 点击左边按钮回调
 
 @param HwNaviView HwNaviView description
 @param sender sender description
 */
- (void)navigationView:(HwNaviView *)HwNaviView clickLeftBtn:(UIButton *)sender;

@end

NS_ASSUME_NONNULL_BEGIN

@interface HwNaviView : UIView

/** 代理 */
@property (nonatomic ,weak) id<UINavigationViewDelegate>delegate;

/** 左边按钮点击回调 */
@property (copy, nonatomic) dispatch_block_t leftTapBlock;

/** 设置导航栏标题 */
- (void)setTitleLabStr:(NSString *)str;

@end

NS_ASSUME_NONNULL_END
