# HwRtcLiveDemo

## 环境要求

- 准备集成开发环境，建议使用Microsoft Visual Studio 2015或以上版本。

- 支持Windows 7或以上版本的Windows设备。

- 支持的平台：x86 release。

## 运行示例程序

### 创建账号获取AppId和域名

在打开项目工程前，您首先需要做的是获取App ID和域名：

1. [注册](https://id1.cloud.huawei.com/UnifiedIDMPortal/portal/userRegister/regbyphone.html?themeName=red&access_type=offline&clientID=103493351&loginChannel=88000000&loginUrl=https%3A%2F%2Fauth.huaweicloud.com%2Fauthui%2Flogin.html%3Fservice%3Dhttps%253A%252F%252Fsupport.huaweicloud.com%252Fcsdk-rtc%252Frtc_05_0124.html%23&service=https%3A%2F%2Fauth.huaweicloud.com%2Fauthui%2FcasLogin%3Fservice%3Dhttps%253A%252F%252Fsupport.huaweicloud.com%252Fcsdk-rtc%252Frtc_05_0124.html&countryCode=cn&scope=https%3A%2F%2Fwww.huawei.com%2Fauth%2Faccount%2Funified.profile+https%3A%2F%2Fwww.huawei.com%2Fauth%2Faccount%2Frisk.idstate&reqClientType=88&state=ae89034592bf47d3b7b07a2df2c7074b&lang=zh-cn)华为账号，并[登录](https://auth.huaweicloud.com/authui/login.html?locale=zh-cn&service=https%3A%2F%2Fsupport.huaweicloud.com%2Fcsdk-rtc%2Frtc_05_0124.html#/login)

2. 在实时音视频控制台[创建应用](https://support.huaweicloud.com/usermanual-rtc/rtc_04_0001.html)，并获取App ID和域名

3. 应用创建完成后，请将应用的名称和ID附在工单中[提交审核](https://console.huaweicloud.com/ticket/?#/ticketindex/business?productTypeId=ffb4ebf5fb094bc6aef0129c276ce42e)，以便应用生效

4. 将获取到的App ID和域名分别填入项目工程中的`HwRtcLiveDlg.cpp`中

   ```c++
   // 需要向华为云申请并填写正确的APPID
   const char* _APP_ID_ = "";
   const char* _DOMAIN_ = "";
   ```

### 集成SDK

然后在 [SparkRTC SDK](https://support.huaweicloud.com/dl-sdk/rtc_05_0001.html)下载Windows SDK，并解压SDK包，将其中的rtc_sdk_windows文件夹复制到与HwRtcLiveDemo文件夹放于同一目录下。

- HwRtcLiveDemo
- rtc_sdk_windows

完成上述操作，下面就可以编译和运行应用程序

### 编译启动应用程序

推荐使用`Visual Studio`打开项目工程

- 打开项目工程HwRtcLive.sln
- 选择Release和x86编译平台
- 点击运行，编译并运行程序

## 联系我们

## 代码许可





