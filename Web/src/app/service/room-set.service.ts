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
import { Injectable, EventEmitter } from '@angular/core'
import { ModalService } from 'ng-devui/modal'
import { SetRoomComponent } from '../set-room/set-room.component'
import HRTC from '../../../sdk/hrtc'
import * as CryptoJS from 'crypto-js'

@Injectable({   // 通过 @Injectable() 装饰器来声明某个类可被注入实例化
  providedIn: 'root', // 将服务注册到angular依赖注入机制中，root实际是APPModule的别名。
})

export class RoomSetService {  // 把RoomSetService声明为可注入的服务,可以在别的组件中通过依赖注入的方式来使用
  // 公共参数
  domain = ''
  appId = localStorage.getItem('appId') || 'c45fc2ca33ad491fa7b8da5672240cd7'
  appSecret = localStorage.getItem('appSecret') || '79827183B3B940A29D44611DED0EF23B'

  public client: any        //客户端对象
  localStream: any          // 本地流
  remoteStreams: any = []   // 远端流
  showAux = false           // 是否显示本地辅流
  localAuxStream = null     // 本地辅流
  joinRoomStatus: boolean = false   //是否已加入房间
  joinRoles = ['Joiner', 'Player']
  joinRole = 'Joiner'   //用户角色
  preSwitchRole = ''
  roomId: string    //房间号
  userId: string    //用户Id
  videoResolution = '720p_1'  //视频分辨率
  videoResolutions = [
    '90p_1',
    '90p_2',
    '120p_1',
    '120p_2',
    '180p_1',
    '180p_2',
    '180p_3',
    '240p_1',
    '240p_2',
    '270p_1',
    '300p_1',
    '360p_1',
    '360p_2',
    '360p_3',
    '450p_1',
    '480p_1',
    '480p_2',
    '540p_1',
    '630p_1',
    '720p_1',
    '720p_2'
  ]
  videoMode: 'contain' | 'cover' | 'fill' = 'fill'  //本地流视频窗口播放模式
  audioOutputs: {   //音频输出设备
    deviceId: string;
    groupId: string;
    kind: "audioinput" | "audiooutput" | "videoinput";
    label: string;
  }[] = []
  audioInputs: {     //音频输入设备
    deviceId: string;
    groupId: string;
    kind: "audioinput" | "audiooutput" | "videoinput";
    label: string;
  }[] = []
  videoInputs: {    //视频设备
    deviceId: string;
    groupId: string;
    kind: "audioinput" | "audiooutput" | "videoinput";
    label: string;
  }[] = []
  audioOutput: any
  audioInput: any
  videoInput: any
  deviceInfoInited = false  //音视频设备信息是否获取状态
  videoUnMuted = true       //启用视频轨道状态
  audioUnMuted = true       //启用音频轨道状态
  curMainVideoUnMuted = true  //当前主流启用视频轨道状态
  curMainAudioUnMuted = true  //当前主流启用音频轨道状态
  environmentAccessInfo = {
    HA_URL: 'https://metrics-drcn.dt.hicloud.com:443',//SparkRTC接入环境
    ACCESS_URL: 'rtc.platform.dbankcloud.com/websocket'
  }
  parameter: any = HRTC.setParameter('environment', this.environmentAccessInfo) //增加全局设置

  version: string = HRTC.VERSION // 获取sdk版本号
  eventBus = new EventEmitter()

  constructor(private modalService: ModalService) { }



  //生成签名信息和时间
  getSignatureAndCtimeArray() {
    const ctimeNum = 7200000// 单位：毫秒
    const ctime1 = Math.floor((ctimeNum + new Date().getTime()) / 1000)
    const message = `${this.appId}+${this.roomId}+${this.userId}+${ctime1}`
    return [CryptoJS.HmacSHA256(message, this.appSecret).toString(), ctime1] // 使用HmacSHA256加密
  }

  // 生成用户信息
  generateUserInfo() {
    const signatrueAndCtimeArray: Array<any> = this.getSignatureAndCtimeArray()
    return {
      userId: this.userId,
      userName: this.userId,
      signature: signatrueAndCtimeArray[0],
      ctime: signatrueAndCtimeArray[1],
      role: this.joinRole === 'Joiner' ? 0 : 2
    }
  }

  // 打印设备错误信息
  printDeviceError(error): void {
    console.error('getDeviceInfo failed, code: ', error.code)
  }

  // 获取摄像头麦克风等设备信息
  getDeviceInfo(): Promise<any[]> {
    this.deviceInfoInited = true
    return Promise.all([
      HRTC.getMicrophones().then((deviceInfo) => this.getAudioInput(deviceInfo)).catch(this.printDeviceError),
      HRTC.getCameras().then((deviceInfo) => this.getVideoInput(deviceInfo)).catch(this.printDeviceError),
      HRTC.getSpeakers().then((deviceInfo) => this.getAudioOutput(deviceInfo)).catch(this.printDeviceError)
    ])
  }

  // 获取摄像头麦克风等设备ID
  getDeviceId(type: string, deviceId: string): string {
    if (deviceId !== 'default') {
      return deviceId
    }
    let defaultDeviceId = 'default'
    if (type === 'microPhone') {
      const defaultDevice = this.audioInputs.find(audioInput => audioInput.deviceId === 'default')
      if (defaultDevice) {
        const matchedDevice = this.audioInputs.find(
          audioInput => defaultDevice.label !== audioInput.label && defaultDevice.label.endsWith(audioInput.label)
        )
        if (matchedDevice) {
          defaultDeviceId = matchedDevice.deviceId
        }
      }
    } else if (type === 'video') {
      const defaultDevice = this.videoInputs.find(videoInput => videoInput.deviceId === 'default')
      if (defaultDevice) {
        const matchedDevice = this.videoInputs.find(
          videoInput => defaultDevice.label !== videoInput.label && defaultDevice.label.endsWith(videoInput.label)
        )
        if (matchedDevice) {
          defaultDeviceId = matchedDevice.deviceId
        }
      }
    } else if (type === 'speaker') {
      const defaultDevice = this.audioOutputs.find(audioOutput => audioOutput.deviceId === 'default')
      if (defaultDevice) {
        const matchedDevice = this.audioOutputs.find(
          audioOutput => defaultDevice.label !== audioOutput.label && defaultDevice.label.endsWith(audioOutput.label)
        )
        if (matchedDevice) {
          defaultDeviceId = matchedDevice.deviceId
        }
      }
    }
    return defaultDeviceId
  }

  // 获取音视频输入输出
  getAudioInput(deviceInfo: MediaDeviceInfo[]): void {
    this.audioInputs = [];
    for (let item of deviceInfo) {
      this.audioInputs.push({
        deviceId: item.deviceId,
        groupId: item.groupId,
        kind: item.kind,
        label: item.label || item.deviceId
      });
    }
    this.audioInput = deviceInfo[0];
    console.info('audioInput: ', this.audioInput)
  }

  getVideoInput(deviceInfo: MediaDeviceInfo[]): void {
    this.videoInputs = [];
    for (let item of deviceInfo) {
      this.videoInputs.push({
        deviceId: item.deviceId,
        groupId: item.groupId,
        kind: item.kind,
        label: item.label || item.deviceId
      });
    }
    this.videoInput = deviceInfo[0];
    console.info('videoInput: ', this.videoInput)
  }

  getAudioOutput(deviceInfo: MediaDeviceInfo[]): void {
    this.audioOutputs = [];
    for (let item of deviceInfo) {
      this.audioOutputs.push({
        deviceId: item.deviceId,
        groupId: item.groupId,
        kind: item.kind,
        label: item.label || item.deviceId
      });
    }
    this.audioOutput = deviceInfo[0];
    console.info('audioOutput: ', this.audioOutput)
  }

  // 切换本地视频分辨率
  setVideoProfile(): void {
    try {
      if (this.joinRole !== 'Player') {
        this.localStream?.setVideoProfile(this.videoResolution)
        console.log('set video profile success ', this.videoResolution)
      }
    } catch (error) {
      console.error('set video profile fail ', this.videoResolution)
    }
  }

  // 发布本地主流
  async publishStream(): Promise<void> {
    if (this.client && this.localStream) {
      try {
        await this.client.publish(this.localStream)
        console.log('publish local stream success')
      } catch (error) {
        console.error('publish local stream fail', error)
      }
    }
  }

  // 取消发布本地主流
  async unpublishStream(): Promise<void> {
    if (this.client && this.localStream) {
      try {
        await this.client.unpublish(this.localStream)
        console.log(`unpublish local stream success`)
      } catch (error) {
        console.error('unpublish local stream fail', error)
      }
    }
  }

  // 停止预览本地辅流
  stopLocalAuxStream(): void {
    this.localAuxStream?.close()
    this.showAux = false
    this.localAuxStream = null
  }

  // 切换本地用户角色
  async switchUserRole(): Promise<void> {
    try {
      const role = this.joinRole === 'Joiner' ? 0 : 2
      const signatureAndCtimeArray: Array<any> = this.getSignatureAndCtimeArray()
      const authorization = {
        signature: signatureAndCtimeArray[0],
        ctime: signatureAndCtimeArray[1]
      }
      await this.client.switchRole(role, authorization)
      console.log('set user role ok' + this.joinRole)
      if (this.joinRole === 'Joiner') {
        await this.publishStream()
      }
      if (this.joinRole === 'Player' && this.localAuxStream) {
        this.stopLocalAuxStream()
      }
    } catch (error) {
      console.error('set user role fail' + error)
    }
  }

  // 设置按钮的ui组件
  joinSet(): void {
    const results = this.modalService.open({
      id: 'modal-modal',
      width: this.isMobileDevice() ? '90%' : '500px',
      backdropCloseable: false,
      showAnimate: true,
      component: SetRoomComponent,
      onClose: () => {
        console.log('on modal closed.')
      },
      data: {
        content: 'Error: This is an error message, please take a look.',
        onClose: (isCancel) => {
          if (!isCancel) {
            this.applySettings()
          }
          results.modalInstance.hide()
        },
      },
    })
  }

  // 设备系统是否为移动端
  isMobileDevice(): boolean {
    const MAP_EXP = {
      Android: /(android)\s+([\d.]+)/,
      Iphone: /(iphone\sos)\s([\d_]+)/
    }
    const ua = navigator.userAgent.toLowerCase()
    let result = false
    for (const key in MAP_EXP) {
      if (Object.prototype.hasOwnProperty.call(MAP_EXP, key)) {
        const uaMatch = ua.match(MAP_EXP[key])
        if (uaMatch) {
          result = true
          break
        }
      }
    }
    return result
  }

  // 设置页面
  async applySettings(): Promise<void> {
    try {
      if (this.remoteStreams) {
        this.remoteStreams.forEach((remote) => {
          remote.stream.setAudioOutput(this.getDeviceId('speaker', this.audioOutput.deviceId))
        })
      }
      const switchActions = []
      if (this.localStream && this.audioInput) {
        switchActions.push(this.localStream?.switchDevice('audio', this.getDeviceId('microPhone', this.audioInput.deviceId)))
      }
      if (this.localStream && this.videoInput) {
        switchActions.push(this.localStream?.switchDevice('video', this.getDeviceId('video', this.videoInput.deviceId)))
      }
      if (switchActions.length > 0) {
        await Promise.all(switchActions)
        this.setVideoProfile()
        if (this.preSwitchRole !== this.joinRole) {
          this.preSwitchRole = this.joinRole
          this.switchUserRole()
        }
        if (!this.curMainAudioUnMuted) {
          this.localStream?.muteAudio()
        }
        if (!this.curMainVideoUnMuted) {
          this.localStream?.muteVideo()
        }
      }
    } catch (error) {
      console.error(error)
    }
  }

}
