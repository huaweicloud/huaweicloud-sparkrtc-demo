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
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
import { BrowserModule } from '@angular/platform-browser';//浏览器中运行的应用都需要引入的模块
import { NgModule } from '@angular/core';   //每个模块都需要引入的核心库
import { DevUIModule } from 'ng-devui';
import { AppRoutingModule } from './app-routing.module';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { AppComponent } from './app.component';   //自定义组件
import { CallRoomComponent } from './call-room/call-room.component';
import { JoinRoomComponent } from './join-room/join-room.component';
import { FormsModule } from '@angular/forms';
import { SetRoomComponent } from './set-room/set-room.component';
import { APP_BASE_HREF } from '@angular/common';

@NgModule({
  declarations: [
    AppComponent,
    CallRoomComponent,
    JoinRoomComponent,
    SetRoomComponent
  ],
  imports: [    //当前项目依赖的模块
    FormsModule,
    AppRoutingModule,
    BrowserModule,
    BrowserAnimationsModule,
    DevUIModule
  ],
  providers: [{ provide: APP_BASE_HREF, useValue: '' }],
  bootstrap: [AppComponent]   //默认启动组件
})
export class AppModule { }
