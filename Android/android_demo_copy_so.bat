@echo off<nul 3>nul
::关闭脚本语言的执行前语句
@echo off
:: create by wwx565059
echo "use param: clean to rebuild"

rem CMake
cd .\..\..\..\service\build\

::删除原有编译目录，清空所有临时文件
rmdir /S /Q .\..\build-android
rmdir /S /Q .\..\support\build-android
rmdir /S /Q .\..\component\build-android

::call .\build_android.bat -DTUP_WS_LOCALHOST=1 -DTUP_GIT_VERSION=%git_node%
call .\build_android.bat
echo errorlevel=%errorlevel%
if %errorlevel% gtr 0 goto :buildfailed 

::call .\build_android.bat -DTUP_WS_LOCALHOST=1 -DTUP_GIT_VERSION=%git_node%
rem CMake
cd .\..\build\

rmdir /S /Q .\..\build-android64
rmdir /S /Q .\..\support\build-android64
rmdir /S /Q .\..\component\build-android64

call .\build_android64.bat
echo errorlevel=%errorlevel%
if %errorlevel% gtr 0 goto :buildfailed 

::????????
set RTC_HOME_DIR=.\..\..\..\..\
set RTC_MEDIA_OUT=%RTC_HOME_DIR%platform\media\android\
set RTC_CMAKE_OUT=%RTC_HOME_DIR%service\

:packet
cd .\..\..\rtc_demo\release_demo\android_new\HwRtcLiveDemo\
del .\app\libs\HME-Audio.jar
del .\app\libs\HME-Video.jar
del .\app\libs\huawei-rtc-sdk.jar

::jar
copy "%RTC_MEDIA_OUT%HME-Audio.jar" ".\app\libs\"
copy "%RTC_MEDIA_OUT%HME-Video.jar" ".\app\libs\"
copy "%RTC_CMAKE_OUT%build-android\out\rtc-sdk.jar" ".\app\libs\"

cd .\app\libs\
jar -xvf HME-Audio.jar
jar -xvf HME-Video.jar
jar -xvf rtc-sdk.jar
del /F HME-Audio.jar
del /F HME-Video.jar
del /F rtc-sdk.jar
jar -cvfM huawei-rtc-sdk.jar ./com
rmdir /S /Q .\com
rmdir /S /Q .\META-INF
cd ..\..\

::media
del .\app\src\main\jniLibs\armeabi-v7a\*.so
copy "%RTC_MEDIA_OUT%arm32\*.so" ".\app\src\main\jniLibs\armeabi-v7a\"

::rtc
copy "%RTC_CMAKE_OUT%build-android\no_symbol\*.so" ".\app\src\main\jniLibs\armeabi-v7a\"

::other
copy "%RTC_SECUREC_OUT%android_arm\libsecurec.so" ".\app\src\main\jniLibs\armeabi-v7a\"
copy "%RTC_LWB_OUT%android\release\libwebsockets.so" ".\app\src\main\jniLibs\armeabi-v7a\"

::64
del .\app\src\main\jniLibs\arm64-v8a\*.so
copy "%RTC_MEDIA_OUT%arm64\*.so" ".\app\src\main\jniLibs\arm64-v8a\"

::rtc
copy "%RTC_CMAKE_OUT%build-android64\no_symbol\*.so" ".\app\src\main\jniLibs\arm64-v8a\"

::other
copy "%RTC_SECUREC_OUT%android_arm64-v8a\libsecurec.so" ".\app\src\main\jniLibs\arm64-v8a\"
copy "%RTC_LWB_OUT%android_64\libwebsockets.so" ".\app\src\main\jniLibs\arm64-v8a\"

goto :packet_complete

:buildfailed  
echo.  
echo.  
echo ***************************************************************************************  
echo BUILD FAILED!!  
echo ***************************************************************************************  
::失败切回原打包目录
cd .\..\..\packet\RTC_SDK
goto :end 

:packet_complete
echo.  
echo.  
echo ***************************************************************************************  
echo PACKET COMPLETE  
echo ***************************************************************************************  
goto end 

:end
exit /b %errorlevel%
