# HwRtcLiveDemo

## 环境要求

- 准备Android Studio或者Eclipse集成开发环境，推荐使用Android Studio 3.3.2及以上。

- 准备Android运行环境：API 21、Android 5.0以上设备。

- 支持的终端CPU架构：armeabi-v7a、arm64-v8a。

## 运行示例程序

### 创建账号获取AppId

在打开项目工程前，您首先需要做的是获取App ID和域名：

1. [注册](https://id1.cloud.huawei.com/UnifiedIDMPortal/portal/userRegister/regbyphone.html?themeName=red&access_type=offline&clientID=103493351&loginChannel=88000000&loginUrl=https%3A%2F%2Fauth.huaweicloud.com%2Fauthui%2Flogin.html%3Fservice%3Dhttps%253A%252F%252Fsupport.huaweicloud.com%252Fcsdk-rtc%252Frtc_05_0124.html%23&service=https%3A%2F%2Fauth.huaweicloud.com%2Fauthui%2FcasLogin%3Fservice%3Dhttps%253A%252F%252Fsupport.huaweicloud.com%252Fcsdk-rtc%252Frtc_05_0124.html&countryCode=cn&scope=https%3A%2F%2Fwww.huawei.com%2Fauth%2Faccount%2Funified.profile+https%3A%2F%2Fwww.huawei.com%2Fauth%2Faccount%2Frisk.idstate&reqClientType=88&state=ae89034592bf47d3b7b07a2df2c7074b&lang=zh-cn)华为账号，并[登录](https://auth.huaweicloud.com/authui/login.html?locale=zh-cn&service=https%3A%2F%2Fsupport.huaweicloud.com%2Fcsdk-rtc%2Frtc_05_0124.html#/login)

2. 在实时音视频控制台[创建应用](https://support.huaweicloud.com/usermanual-rtc/rtc_04_0001.html)，并获取App ID和域名

3. 应用创建完成后，请将应用的名称和ID附在工单中[提交审核](https://console.huaweicloud.com/ticket/?#/ticketindex/business?productTypeId=ffb4ebf5fb094bc6aef0129c276ce42e)，以便应用生效

4. 将获取到的App ID和域名分别填入项目工程中的`com.huawei.rtcdemo.Constants类`中

   ```java
   /**
    * 需要向华为云申请并填写正确的appid和server_addr
    */
   public static final String RTC_DEFAULT_SERVER_ADDR = "";
   public static final String RTC_DEFAULT_APP_ID = "";
   ```

### 集成SDK
然后在 [SparkRTC SDK](https://support.huaweicloud.com/dl-sdk/rtc_05_0001.html)下载 Android SDK，并解压 SDK 包，将其中的文件根据下面
对应目录将SDK拷贝到对应项目工程目录

在对应目录将SDK拷贝到对应项目工程目录

| SDK目录              | 项目工程目录            |
| -------------------- | ----------------------- |
| `.aar` 文件          | `/app/libs/`            |
| `arm64-v8a` 文件夹   | `/app/src/main/jniLibs` |
| `armeabi-v7a` 文件夹 | `/app/src/main/jniLibs` |

拷贝完成上述文件后，您还需要修改`app`目录下的`build.gradle`文件

```groovy
android {
    ...
    sourceSets {
    	main {
        	jniLibs.srcDirs = ['src/main/jniLibs']
    	}
	}
    ...
    dependencies {
    	implementation fileTree(dir: 'libs', include: ['*.jar', '*.aar'])
        ...
    }
}
```

完成上述操作，下面就可以进行编译和运行应用程序

### 编译启动应用程序

推荐使用`Android Studio`打开项目工程

- 打开项目工程
- 连接手机，确保手机开启调试模式，并且Android Studio出现可识别的对应设备标识
- 点击运行

## 联系我们

## 代码许可





