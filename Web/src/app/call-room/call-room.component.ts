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
import { Component, OnInit, Input, Output, EventEmitter, ChangeDetectorRef } from '@angular/core'
import { RoomSetService } from '../service/room-set.service'
import { ElementRef } from '@angular/core'
import HRTC from '../../../sdk/hrtc'
import VConsole from 'vconsole'
@Component({
  selector: 'app-call-room',
  templateUrl: './call-room.component.html',
  styleUrls: ['./call-room.component.scss'],
})

export class CallRoomComponent implements OnInit {
  // splitter pane input
  size = '100px';
  collapsed = false;
  disabledBarSize = '1px'
  constructor(
    public roomSetService: RoomSetService,
    private el: ElementRef,
    ref: ChangeDetectorRef
  ) {
    this.ref = ref
  }

  @Output() private childOuter = new EventEmitter()
  userList: any[] = []            // 用户列表数量
  curMainDisplayStream: any = null// 当前大屏展示的流
  remoteAuxStream: boolean = false // 远端辅流
  showLocal = false               // 判断本地窗口是否显示
  localStreamWapper: any = null   // 本地主流容器
  localAuxStreamWapper: any = null// 本地辅流容器
  ref: ChangeDetectorRef
  ngOnInit(): void {
    if (this.roomSetService.isMobileDevice()) {
      //@ts-ignore
      new VConsole()
      this.size = '30%'
    }
  }

  ngAfterContentInit(): void {
    // 事件监听
    // 远端用户进入房间事件
    this.roomSetService.client.on('peer-join', (user: any) => {
      console.log(`${user.userId} joinroom`)
      const userInfo = {
        userId: user.userId,
        userName: user.userId,
        ratio: '720p'
      }
      this.userList.push(userInfo)
    })

    // 远端用户退出房间事件
    this.roomSetService.client.on('peer-leave', (userId: string) => {
      this.userList = this.userList.filter((item) => item.userId !== userId)
      console.log(`${JSON.stringify(userId)} leaveroom`)
    })

    // 远端流添加事件
    this.roomSetService.client.on('stream-added', (event: any) => {
      const remoteStream = {
        userId: event.stream.getUserId(),
        isShow: false,
        stream: event.stream,
        muteAudio: false,
        muteVideo: false,
        type: event.stream.getType(),
        showCurMain: false
      }
      this.roomSetService.remoteStreams.push(remoteStream)
      this.ref.markForCheck()
      this.ref.detectChanges()
      this.subscribe(remoteStream.stream, remoteStream.type)
      console.log('######stream-added', event?.stream.getUserId(), event?.stream.id_)
    })

    // 远端流订阅事件：
    this.roomSetService.client.on('stream-subscribed', (event: any) => {
      console.log('######stream-subscribed', event.stream.getUserId())
      const user = this.roomSetService.remoteStreams.find(
        (stream) => stream.userId === event.stream.getUserId() && stream.type === event.stream.getType()
      )
      event.stream.play(`c0-remote${event.stream.getUserId() + event.stream.getType()}`, { muted: false })
      user.isShow = true
      event.stream.getType() === 'auxiliary' && (this.remoteAuxStream = true)
      this.roomSetService.remoteStreams.forEach((remote) => {
        remote.stream.setAudioOutput(this.roomSetService.audioOutput)
      })
    })

    // 远端流移除事件
    this.roomSetService.client.on('stream-removed', (evt: any) => {
      console.log('######stream-removed', evt.stream.getUserId(), evt.stream.getType())
      evt.stream.close()
      this.roomSetService.remoteStreams = this.roomSetService.remoteStreams.filter(
        (stream) => stream.stream.getUserId() !== evt.stream.getUserId() || stream.type !== evt.stream.getType()
      )
      this.ref.markForCheck()
      this.ref.detectChanges()
      evt.stream.getType() === 'auxiliary' && (this.remoteAuxStream = false)
      if (this.curMainDisplayStream.stream.getId() !== evt.stream.getId()) {
        return
      }
      this.curMainDisplayStream = this.localStreamWapper
      this.handleSwitchView(this.localStreamWapper)
    })

    // 远端流更新事件
    this.roomSetService.client.on('stream-updated', (evt: any) => {
      console.log('stream-updated', evt.stream.getUserId())
      const Stream = this.roomSetService.remoteStreams.find((stream) => stream.stream.getUserId() === evt.stream.getUserId())
      Stream.stream = evt.stream
    })

    // 客户端出现错误事件
    this.roomSetService.client.on('error', (error) => {
      this.roomSetService.joinRoomStatus === false
      this.roomSetService.remoteStreams = []
      this.userList = []
      this.roomSetService.audioUnMuted = true
      this.roomSetService.videoUnMuted = true
      const err = '异常：' + error.errorMsg
      console.error(err)
    })

    // client连接状态变更事件
    this.roomSetService.client.on('connection-state-changed', (info: any) => {
      console.log(`connection-state-changed prevState ${info.prevState}`)
      console.log(`connection-state-changed curState ${info.curState}`)
      console.log(`connection-state-changed：${info.curState}`)
    })

    // 用户被踢下线事件，当用户以相同的userId在其他client加入房间后，被踢的client会收到该事件通知
    this.roomSetService.client.on('client-banned', (userInfo) => {
      console.log(`client-banned:${userInfo.userId}`)
      this.leaveRoom()
    })

    // 本地摄像头设备变更事件，当插、拔本地摄像头时触发
    this.roomSetService.client.on('camera-changed', (deviceInfo) => {
      console.log(`camera-changed: ${deviceInfo.deviceId}  ${deviceInfo.state}`)
      HRTC.getCameras()
        .then((res) => {
          return (this.roomSetService.videoInputs = res)
        })
      this.roomSetService.getDeviceInfo()
      try {
        if (this.roomSetService.videoInput.deviceId === deviceInfo.deviceId && deviceInfo.state === 'REMOVE') {
          if (this.roomSetService.videoInputs.length === 0) {
            this.roomSetService.videoInput = null
            this.localPreviewStop()
            console.log('localPreviewStop')
          } else {
            if (this.showLocal) {
              this.roomSetService.videoInput = this.roomSetService.videoInputs.find(
                (input) => input.deviceId !== deviceInfo.deviceId)
              this.roomSetService.localStream.switchDevice('video', this.roomSetService.videoInput.deviceId)
              console.log('switchDevice', this.roomSetService.videoInput.deviceId)
            }
          }
        } else if (deviceInfo.state === 'ADD') {
          if (this.showLocal) {
            this.roomSetService.localStream.switchDevice('video', deviceInfo.deviceId)
          }
        }
      } catch (error) {
        console.log(error)
      }
    })

    // 本地录音设备变更事件，当本地录音设备变更时触发
    this.roomSetService.client.on('recording-device-changed', (deviceInfo) => {
      console.log(`recording-device-changed: ${deviceInfo.deviceId}  ${deviceInfo.state}`)
      HRTC.getMicrophones()
        .then((res) => {
          console.log(res)
          return (this.roomSetService.audioInputs = res)
        })
      this.roomSetService.getDeviceInfo()
      try {
        if (deviceInfo.state === 'REMOVE') {
          if (this.roomSetService.audioInputs.length === 0) {
            this.roomSetService.audioInputs = null
          } else {
            this.roomSetService.audioInput = this.roomSetService.audioInputs.find(
              (input) => input.deviceId !== deviceInfo.deviceId && input.groupId !== deviceInfo.groupId
            )
            this.roomSetService.localStream.switchDevice('audio', this.roomSetService.audioInput.deviceId)
          }
        } else if (deviceInfo.state === 'ADD') {
          this.roomSetService.localStream.switchDevice('audio', deviceInfo.deviceId)
          console.log("deviceInfo add")
        }
      } catch (error) {
        console.log(error)
      }
    })

    // 本地播放设备变更事件，当本地播放设备变更时触发
    this.roomSetService.client.on('playback-device-changed', (deviceInfo) => {
      console.log(`playback-device-changed: ${deviceInfo.deviceId}  ${deviceInfo.state}`)
      if (deviceInfo.state === 'ADD') {
        HRTC.getSpeakers()
          .then((res) => {
            return (this.roomSetService.audioOutputs = res)
          })
        try {
          this.roomSetService.audioOutput = deviceInfo.deviceId
        }
        catch (err) {
          console.log(err)
        }
      } else {
        HRTC.getSpeakers()
          .then((res) => {
            return (this.roomSetService.audioOutputs = res)
          })
        try {
          this.roomSetService.audioOutput = this.roomSetService.audioOutputs[0].deviceId
        }
        catch (err) {
          console.log(err)
        }
      }
    })

    this.joinRoomInit()
  }

  // 加入房间 并开启预览本地流
  async joinRoomInit(): Promise<void> {
    try {
      await this.roomSetService.client.join(this.roomSetService.roomId, this.roomSetService.generateUserInfo())
      await this.startLocalPreview()
      console.log('join romm success')
    } catch (error) {
      console.error(error)
    }
  }

  // 创建并初始化本地流：
  async initializeLocalStream(): Promise<any> {
    if (this.showLocal) {
      console.log('localstream is palying')
      return
    }
    const co = {
      audio: true,
      video: true,
      screen: false,
      mirror: false,
      microphoneId: this.roomSetService.audioInput.deviceId,
      cameraId: this.roomSetService.videoInput.deviceId
    }
    const localSt: any = HRTC.createStream(co)
    localSt.setVideoProfile(this.roomSetService.videoResolution)
    await localSt.initialize()
    return localSt
  }

  // 预览本地流：
  async localPreview(elementId: string): Promise<void> {
    console.log('start localPreview')
    const localSt = await this.initializeLocalStream()
    this.roomSetService.localStream = localSt
    if (this.roomSetService.localStream) {
      await this.roomSetService.localStream.play(`main-display`, { muted: true })
    }
    if (!this.roomSetService.audioUnMuted) {
      this.roomSetService.localStream.muteAudio()
    }
    if (!this.roomSetService.videoUnMuted) {
      this.roomSetService.localStream.muteVideo()
    }
    this.showLocal = true
  }

  // 开启预览本地流
  async startLocalPreview(): Promise<void> {
    await this.localPreview(`main-display`)
    this.localStreamWapper = {
      userId: 'local',
      stream: this.roomSetService.localStream,
      muteAudio: !this.roomSetService.audioUnMuted,
      muteVideo: !this.roomSetService.audioUnMuted,
      type: 'main',
      showCurMain: true  //本地流开启预览后，默认在大屏展示
    }
    this.curMainDisplayStream = this.localStreamWapper
    if (this.roomSetService.joinRole !== 'Player') {
      this.roomSetService.publishStream()
    }
    console.log('publishStream success!')
  }

  // 停止预览本地流：
  localPreviewStop(): void {
    this.roomSetService.localStream.close()
    this.showLocal = false
  }

  // 创建并预览本地辅流：
  async startLocalAuxStream(): Promise<void> {
    if (this.roomSetService.showAux) {
      console.log('The localAuxStream is being played')
      return
    } else if (this.remoteAuxStream) {
      console.log('RemoteAuxStream already exists.')
      return
    }
    const co = {
      screen: true,
      screenAudio: true
    }
    const localSt: any = HRTC.createStream(co)
    localSt.setVideoProfile('720p_1')
    try {
      await localSt.initialize()
      this.roomSetService.localAuxStream = localSt // 本地辅流
      localSt.bindScreenAudio2RelatedStream(this.roomSetService.localStream, false)
      localSt.play(`c0-main-aux`)
      this.roomSetService.showAux = true
      localSt.on('screen-sharing-stopped', () => {
        this.roomSetService.stopLocalAuxStream()
      })
      this.localAuxStreamWapper = {
        userId: 'local',
        stream: this.roomSetService.localAuxStream,
        muteAudio: false,
        muteVideo: false,
        type: 'auxiliary'
      }
      console.log(`play localstream success, info: ${JSON.stringify(co)}`)
    } catch (error) {
      console.error('local preview fail', error)
    }
  }

  // 发布本地辅流：
  async pubAuxStream(): Promise<void> {
    await this.startLocalAuxStream()
    if (this.roomSetService.client && this.roomSetService.localAuxStream) {
      try {
        await this.roomSetService.client.publish(this.roomSetService.localAuxStream)
        console.log(`publish localAuxStream success`)
      } catch (error) {
        console.error('publish localAuxStream fail', error)
      }
    }
  }

  // 取消发布本地辅流：
  async unPubAuxStream(): Promise<void> {
    if (this.roomSetService.localAuxStream && this.roomSetService.client) {
      try {
        await this.roomSetService.client.unpublish(this.roomSetService.localAuxStream)
        console.log(`unpublish localAuxStream success`)
      } catch (error) {
        console.error('unpublish localAuxStream fail', error)
      }
    }
    this.roomSetService.stopLocalAuxStream()
  }

  // 发布辅流状态切换
  async toggleSubStream(): Promise<void> {
    if (this.roomSetService.joinRole === 'Player') {
      console.log('Player not have the permission to send streams ')
      return
    }
    if (this.roomSetService.showAux) {
      await this.unPubAuxStream()
      this.handleSwitchView(this.localStreamWapper)
    } else {
      this.pubAuxStream()
    }
  }

  // 获取远端辅流信息
  getUserByStreamId(remoteStream: any): any {
    if (remoteStream) {
      const user = this.userList.filter((item) => item.userId === remoteStream.userId)[0]
      if (user) {
        return remoteStream.type === 'auxiliary' ? user.userName + '的屏幕共享' : user.userName
      }
    }
  }

  // 订阅远端流
  async subscribe(remoteStream, type): Promise<void> {
    try {
      const stream = this.roomSetService.remoteStreams.find((Stream) => Stream.userId === remoteStream.getUserId() && Stream.type === type)
      if (!stream.isShow) {
        await this.roomSetService.client.subscribe(remoteStream, { video: true, audio: true, autoAdjustResolution: 1 })
        console.log(`######subscribe ${remoteStream.getUserId()} success`)
        stream.isShow = true
      }
    } catch (error) {
      console.error('subscribe failed', error)
    }
  }

  // 取消订阅远端流
  async unsubscribe(remoteStream): Promise<void> {
    if (this.roomSetService.joinRoomStatus === false) {
      console.error('Not joined in room')
      return
    }
    try {
      const stream = this.roomSetService.remoteStreams.find((Stream) => Stream.userId === remoteStream.getUserId())
      if (stream.isShow) {
        await this.roomSetService.client.unsubscribe(remoteStream)
        console.log('unsubscribe success')
        stream.isShow = false
      }
    } catch (error) {
      console.error('unsubscribe failed, ', error)
    }
  }

  // 离开房间
  async leaveRoom(): Promise<void> {
    try {
      if (this.roomSetService.localStream) {
        this.localPreviewStop()
      }
      if (this.roomSetService.localAuxStream) {
        this.roomSetService.stopLocalAuxStream()
      }
      await this.roomSetService.client.leave()
    } catch (error) {
      console.error('leave room occur error: ' + error)
    }
    this.roomSetService.joinRoomStatus === false
    this.roomSetService.remoteStreams = []
    this.userList = []
    this.roomSetService.localStream = null
    this.roomSetService.videoUnMuted = true
    this.roomSetService.audioUnMuted = true
    this.roomSetService.client = null
    this.childOuter.emit('LEAVE')
    console.log('leave room succeed')
  }

  // 是否禁用音频轨道：
  muteAudio(streamWapper): void {
    if (streamWapper.userId !== 'local') {
      this.roomSetService.remoteStreams.forEach(remoteSteam => {
        if (remoteSteam.userId === streamWapper.userId && remoteSteam.type === streamWapper.type) {
          if (!streamWapper.muteAudio) {
            remoteSteam.stream.muteAudio()
          } else {
            remoteSteam.stream.unmuteAudio()
          }
          streamWapper.muteAudio = !streamWapper.muteAudio
          remoteSteam.muteAudio = streamWapper.muteAudio
        }
      })
    } else {
      if (!streamWapper.muteAudio) {
        streamWapper.stream.muteAudio()
        if (streamWapper.type === 'main') {
          this.roomSetService.curMainAudioUnMuted = false
        }
      } else {
        streamWapper.stream.unmuteAudio()
        if (streamWapper.type === 'main') {
          this.roomSetService.curMainAudioUnMuted = true
        }
      }
      streamWapper.muteAudio = !streamWapper.muteAudio
    }
    this.ref.markForCheck()
    this.ref.detectChanges()
  }

  // 是否禁用视频轨道：
  muteVideo(streamWapper): void {
    if (streamWapper.userId !== 'local') {
      this.roomSetService.remoteStreams.forEach(remoteSteam => {
        if (remoteSteam.userId === streamWapper.userId && remoteSteam.type === streamWapper.type) {
          if (!streamWapper.muteVideo) {
            remoteSteam.stream.muteVideo()
          } else {
            remoteSteam.stream.unmuteVideo()
          }
          streamWapper.muteVideo = !streamWapper.muteVideo
          remoteSteam.muteVideo = streamWapper.muteVideo
        }
      })
    } else {
      if (!streamWapper.muteVideo) {
        streamWapper.stream.muteVideo()
      } else {
        streamWapper.stream.unmuteVideo()
      }
      this.roomSetService.curMainVideoUnMuted = streamWapper.type !== 'main'
      streamWapper.muteVideo = !streamWapper.muteVideo
    }
    this.ref.markForCheck()
    this.ref.detectChanges()
  }

  /**
   * 点击左侧小屏事件
   * @param stream 流对象 本地流/本地辅流时为''
   * @param userId 'local'/userId
   * @param type 'main'/'auxiliary'
   */

  async handleSwitchView(streamWapper): Promise<void> {
    // 大屏流切换为小屏展示
    await this.curMainDisplayStream.stream.stop()

    const resolutionId = this.curMainDisplayStream.stream.getStreamInfo().videoProfiles[0].resolutionId
    const thisId = (this.curMainDisplayStream.stream.getVideoHRTCTrackByTrackId && this.curMainDisplayStream.stream.getVideoHRTCTrackByTrackId(resolutionId)?.getTrackId()) || resolutionId
    const hiddenElement = document.getElementById("main-display").querySelector(`[id='player_${thisId}']`) as HTMLElement
    if (hiddenElement) {
      hiddenElement.style.display = 'none'
    }
    if (this.curMainDisplayStream.userId === 'local' && this.curMainDisplayStream.type === 'main') {
      await this.roomSetService.localStream?.play('c0-main')
      this.localStreamWapper.showCurMain = false
    } else if (this.curMainDisplayStream.userId === 'local' && this.curMainDisplayStream.type === 'auxiliary') {
      await this.roomSetService.localAuxStream?.play('c0-main-aux')
      this.localAuxStreamWapper.showCurMain = false
    } else { // remoteStream -- main/auxiliary
      await this.curMainDisplayStream.stream?.play(`c0-remote${this.curMainDisplayStream.userId}${this.curMainDisplayStream.type}`)
      this.curMainDisplayStream.showCurMain = false
    }
    // 清理辅流
    this.roomSetService.localAuxStream && !this.roomSetService.showAux && this.roomSetService.stopLocalAuxStream()

    // 点击的小屏流切换为大屏展示
    streamWapper.showCurMain = true
    if (streamWapper.userId === 'local' && streamWapper.type === 'main') {
      await this.roomSetService.localStream?.stop()
      await this.roomSetService.localStream?.play('main-display')
      this.localStreamWapper = streamWapper
      this.curMainDisplayStream = this.localStreamWapper
    } else if (streamWapper.userId === 'local' && streamWapper.type === 'auxiliary') {
      const removeMianAuxStream = document.getElementById('c0-main-aux').querySelector('div') as HTMLElement
      removeMianAuxStream.innerHTML = ''
      await this.roomSetService.localAuxStream?.stop()
      await this.roomSetService.localAuxStream?.play('main-display', { objectFit: this.roomSetService.videoMode })
      this.localAuxStreamWapper = streamWapper
      this.curMainDisplayStream = this.localAuxStreamWapper
    } else { // remoteStream -- main/auxiliary
      const removeRemoteStream = document.getElementById('c0-remote' + streamWapper.userId + streamWapper.type).querySelector('div') as HTMLElement
      removeRemoteStream.innerHTML = ''
      await streamWapper.stream?.stop()
      await streamWapper.stream?.play('main-display')
      this.curMainDisplayStream = streamWapper
    }
    this.ref.markForCheck()
    this.ref.detectChanges()
  }
  // 设置按钮
  joinSet(): void {
    this.roomSetService.joinSet()
  }
}
