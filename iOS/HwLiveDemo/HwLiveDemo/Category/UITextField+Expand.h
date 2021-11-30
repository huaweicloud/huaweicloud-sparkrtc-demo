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
//  NSObject+UITextField_Expand.h
//  HwLiveDemo
//
//  Created by hw on 2020/7/21.
//  Copyright © 2020 HW. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef void(^LimitBlock)(void);

NS_ASSUME_NONNULL_BEGIN

@interface UITextField (Expand)

@property (nonatomic , copy)LimitBlock limitBlock;

- (void)lengthLimit:(void (^)(void))limit;

@end

NS_ASSUME_NONNULL_END
