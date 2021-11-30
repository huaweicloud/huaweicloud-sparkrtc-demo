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
//  UIColor+HWColor.h
//  HW
//
//  Created by HW on 2019/7/22.
//  Copyright © 2019年 HW. All rights reserved.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface UIColor (HWColor)

/// 从十六进制字符串获取颜色，
/// @param color 支持@“#123456”、 @“0X123456”、 @“123456”三种格式
+ (UIColor *)colorWithString:(NSString *)color;

/// 从十六进制字符串获取颜色，
/// @param color :支持@“#123456”、 @“0X123456”、 @“123456”三种格式
/// @param alpha <#alpha description#>
+ (UIColor *)colorWithString:(NSString *)color alpha:(CGFloat)alpha;

@end

NS_ASSUME_NONNULL_END
