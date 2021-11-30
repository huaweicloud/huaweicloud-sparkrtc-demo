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
import { Component, OnInit } from '@angular/core'
import { RoomSetService } from '../service/room-set.service'
import HRTC from '../../../sdk/hrtc'

@Component({
  selector: 'app-join-room',
  templateUrl: './join-room.component.html',
  styleUrls: ['./join-room.component.css'],
})

export class JoinRoomComponent implements OnInit {
  constructor(
    public roomSetService: RoomSetService
  ) { }

  ngOnInit(): void { this.roomSetService.getDeviceInfo() }

  // 子组件往父组件传的值：当收到msg === LEAVE时 做离开房间动作
  getChildData(msg: string): void {
    if (msg === 'LEAVE') {
      this.roomSetService.joinRoomStatus = false
    }
  }

  // 设置按钮
  joinSet(): void {
    this.roomSetService.joinSet()
  }

  // 创建CLient对象
  createClient(): void {
    const config = {
      appId: this.roomSetService.appId,
      domain: this.roomSetService.domain
    }
    this.roomSetService.client = HRTC.createClient(config)
  }

  // 加入房间
  async joinRoom(): Promise<void> {
    try {
      if (this.roomSetService.joinRoomStatus) {
        console.log('already join room')
        return
      }
      if (
        (this.roomSetService.roomId || '').length < 1 || (this.roomSetService.roomId || '').length > 64
        || (!/^[a-zA-Z0-9]+$/g.test(this.roomSetService.roomId)
          || (this.roomSetService.userId || '').length < 1) || (this.roomSetService.roomId || '').length > 64
        || !/^[a-zA-Z0-9_-]+$/g.test(this.roomSetService.userId)) {
        console.error('make sure roomId and userId')
        this.roomSetService.joinRoomStatus = false
        return
      } else {
        this.createClient()
        this.roomSetService.joinRoomStatus = true
        this.roomSetService.preSwitchRole = this.roomSetService.joinRole
        console.log('join room ok')
        if (!this.roomSetService.deviceInfoInited) {
          await this.roomSetService.getDeviceInfo()
        }
        // 同步 进入房间前后 摄像头麦克风状态
        this.roomSetService.curMainVideoUnMuted = this.roomSetService.videoUnMuted
        this.roomSetService.curMainAudioUnMuted = this.roomSetService.audioUnMuted
      }
    } catch (error) {
      console.error('join room fail')
    }
  }
}

