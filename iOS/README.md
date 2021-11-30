# HwRtcLiveDemo

## 环境要求

- 准备Xcode即成开发。
- 准备iOS 8.0及以上的iPhone真机。
- 支持的终端CPU架构：armv7，armv7s和arm64。

## 运行示例程序

### 创建账号获取AppId

在打开项目工程前，您首先需要做的是获取App ID和域名：

1. [注册](https://id1.cloud.huawei.com/UnifiedIDMPortal/portal/userRegister/regbyphone.html?themeName=red&access_type=offline&clientID=103493351&loginChannel=88000000&loginUrl=https%3A%2F%2Fauth.huaweicloud.com%2Fauthui%2Flogin.html%3Fservice%3Dhttps%253A%252F%252Fsupport.huaweicloud.com%252Fcsdk-rtc%252Frtc_05_0124.html%23&service=https%3A%2F%2Fauth.huaweicloud.com%2Fauthui%2FcasLogin%3Fservice%3Dhttps%253A%252F%252Fsupport.huaweicloud.com%252Fcsdk-rtc%252Frtc_05_0124.html&countryCode=cn&scope=https%3A%2F%2Fwww.huawei.com%2Fauth%2Faccount%2Funified.profile+https%3A%2F%2Fwww.huawei.com%2Fauth%2Faccount%2Frisk.idstate&reqClientType=88&state=ae89034592bf47d3b7b07a2df2c7074b&lang=zh-cn)华为账号，并[登录](https://auth.huaweicloud.com/authui/login.html?locale=zh-cn&service=https%3A%2F%2Fsupport.huaweicloud.com%2Fcsdk-rtc%2Frtc_05_0124.html#/login)

2. 在实时音视频控制台[创建应用](https://support.huaweicloud.com/usermanual-rtc/rtc_04_0001.html)，并获取App ID和域名

3. 应用创建完成后，请将应用的名称和ID附在工单中[提交审核](https://console.huaweicloud.com/ticket/?#/ticketindex/business?productTypeId=ffb4ebf5fb094bc6aef0129c276ce42e)，以便应用生效

4. 将获取到的App ID和域名分别填入项目工程中的`PrefixHeader.pch`中

   ```/// _APP_ID_ 和 _DOMAIN_ 从华为云获取
   #define _APP_ID_ @""
   #define _DOMAIN_ @""
   ```

### 集成SDK
然后在 [SparkRTC SDK](https://support.huaweicloud.com/dl-sdk/rtc_05_0001.html)下载 iOS SDK，并解压 SDK 包，将其中的文件根据下面
对应目录将SDK拷贝到对应项目工程目录

在对应目录将SDK拷贝到对应项目工程目录

| SDK目录              | 项目工程目录            |
| -------------------- | ----------------------- |
| `RTC.bundle`  | `/HwLiveDemo/HwLiveDemo/Vendor`            |
| `hwffmpeg.framework`  | `HwLiveDemo/HwLiveDemo/Vendor` |
| `HWRTCEngine.framework`  | `HwLiveDemo/HwLiveDemo/Vendor` |

拷贝完成上述文件后，您还需要配置一下你的开发证书。

完成上述操作，下面就可以进行编译和运行应用程序到你的手机上。

### 编译启动应用程序

- 打开项目工程
- 连接手机，确保手机开启调试模式，并且Xcode出现可识别的对应设备标识。
- 点击运行

## 联系我们

## 代码许可





