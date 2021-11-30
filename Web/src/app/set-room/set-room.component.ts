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
import { Input, Component, OnInit, ElementRef } from '@angular/core'
import { RoomSetService } from '../service/room-set.service'
import { ModalComponent } from 'ng-devui/modal'

@Component({
  selector: 'app-set-room',
  templateUrl: './set-room.component.html',
  styleUrls: ['./set-room.component.css']
})
export class SetRoomComponent implements OnInit { // implements 表示接口实现，是typescript语法

  constructor(private elr: ElementRef, public roomSetService: RoomSetService) { }  // elementRef访问dom元素
  @Input() modalInstance: ModalComponent   // @input:定义模块输入，用来让父级组件向子组件传递内容,  modalInstance用来接收父组件传递的值
  parent: HTMLElement
  @Input() data: any

  videoResolutionTemp: any    //当前使用的视频分辨率
  videoInputTemp: any         //当前使用的视频输入
  audioOutputTemp: any        //当前使用的视频输出
  audioInputTemp: any         //当前使用的音频输出
  joinRoleTemp: any           //当前使用的用户角色

  // 设置用户角色/音视频输入输出/视频分辨率
  async setDeviceInfoTemp(): Promise<void> {
    if (!this.roomSetService.deviceInfoInited) {
      await this.roomSetService.getDeviceInfo()
    }
    this.joinRoleTemp = this.roomSetService.joinRole
    this.audioOutputTemp = this.roomSetService.audioOutput
    this.audioInputTemp = this.roomSetService.audioInput
    this.videoInputTemp = this.roomSetService.videoInput
    this.videoResolutionTemp = this.roomSetService.videoResolution
  }

  ngOnInit(): void {
    this.setDeviceInfoTemp()
  }

  // 关闭设置页面
  close(event): void {
    this.data.onClose(true)
  }

  // 设置页面确定按钮
  confirm(event): void {
    this.roomSetService.joinRole = this.joinRoleTemp
    this.roomSetService.audioOutput = this.audioOutputTemp
    this.roomSetService.audioInput = this.audioInputTemp
    this.roomSetService.videoInput = this.videoInputTemp
    this.roomSetService.videoResolution = this.videoResolutionTemp
    localStorage.setItem('domain', this.roomSetService.domain)
    localStorage.setItem('appId', this.roomSetService.appId)
    localStorage.setItem('appSecret', this.roomSetService.appSecret)
    this.data.onClose(false)
  }

  // 入会后 设置页面上传日志按钮
  logUpload(event): void {
    console.error(`reportLog begin`)
    this.roomSetService.client && this.roomSetService.client.reportLogs()
  }
}
