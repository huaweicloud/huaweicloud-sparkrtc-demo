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
//  HWTool.h
//  HW
//
//  Created by HW on 2019/7/26.
//  Copyright © 2019年 HW. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
NS_ASSUME_NONNULL_BEGIN

@interface HWTool : NSObject
///  弹窗提示信息。1.5秒之后自动关闭弹窗。
+ (void)showMessage:(NSString *)string;
+ (NSString *)getUUID;
/// 获取时间戳 单位到毫秒
+ (CALayer *)getGradientLayerWithFrame:(CGRect)frame
                            frameColor:(UIColor *)frameColor
                               toColot:(UIColor *)toColor;
@end

NS_ASSUME_NONNULL_END
