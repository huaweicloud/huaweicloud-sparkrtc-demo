::jar包打成一个
cd .\HwRtcLiveDemo\app\libs\
del /F huawei-rtc-sdk.jar
jar -xvf HME-Audio.jar
jar -xvf HME-Video.jar
jar -xvf rtc-sdk.jar
del /F HME-Audio.jar
del /F HME-Video.jar
del /F rtc-sdk.jar
jar -cvfM huawei-rtc-sdk.jar ./com
rmdir /S /Q .\com
rmdir /S /Q .\META-INF