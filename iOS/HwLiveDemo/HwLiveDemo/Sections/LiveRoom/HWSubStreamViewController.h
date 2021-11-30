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
//  HWSubStreamViewController.h
//  HwLiveDemo
//
//  Created by weshow on 2020/7/29.
//  Copyright © 2020 HW. All rights reserved.
//

#import "HWCameraViewController.h"

NS_ASSUME_NONNULL_BEGIN

/*
    HWSubStreamViewController 继承于 HWCameraViewController.
主要的UI功能和SDK的加入房间逻辑是一致.只是视图的更新和辅流回调的处理不一致.
 */

@interface HWSubStreamViewController : HWCameraViewController

@end

NS_ASSUME_NONNULL_END
