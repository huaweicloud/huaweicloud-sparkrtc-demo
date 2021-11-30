# HwRtcLiveDemo

## 环境要求

- 准备集成开发环境，建议使用 Microsoft Visual Studio Code 1.43.2 或以上版本。

- 如果客户端用 Node.js 开发， 推荐安装 12.16.1 或以上版本。

- 如果客户端用 TypeScript 开发，TypeScript 的版本不低于 3.8.3。

## 运行示例程序

### 集成 SDK

然后在 [SparkRTC SDK](https://support.huaweicloud.com/dl-sdk/rtc_05_0001.html)下载 Web SDK，并解压 SDK 包，将其中的 rtc_sdk_web 文件夹解压后，将 hrtc.js 和 hrtc.d.ts 文件复制到 web 文件夹下的 sdk 目录下。

完成上述操作，下面就可以编译和运行应用程序

### 编译启动应用程序

推荐使用 Visual Studio Code 打开项目工程

- 在 Visual Studio Code 打开终端，运行`npm install`安装项目所需依赖（若 ng 命令使用不了，安装脚手架命令 npm install -g @angular/cli）
- 依赖安装完毕后，运行`ng serve`启动项目，在终端生成 localhost:端口访问链接
- 在 chrome 浏览器中访问 localhost:端口，可以看到加入房间的页面
- 在加入房间的页面填写房间号和用户名，点击“加入”按钮，即可加入房间

## 联系我们

## 代码许可
