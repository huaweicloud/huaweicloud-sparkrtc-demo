//
//  HWMonitor.m
//  HwLiveDemo
//
//  Created by HW on 2019/4/11.
//  Copyright © 2019年 @YanYuXin. All rights reserved.
//

#import "HWMonitor.h"
#import <UIKit/UIKit.h>
#include <libkern/OSAtomic.h>
#include <execinfo.h>
#import <CommonCrypto/CommonDigest.h>
#import <CommonCrypto/CommonHMAC.h>

NSString * const YunUncaughtExceptionHandlerSignalExceptionName = @"YunUncaughtExceptionHandlerSignalExceptionName";
NSString * const YunUncaughtExceptionHandlerSignalKey = @"YunUncaughtExceptionHandlerSignalKey";
NSString * const YunUncaughtExceptionHandlerAddressesKey = @"YunUncaughtExceptionHandlerAddressesKey";

volatile int32_t YunUncaughtExceptionCount = 0;
const int32_t YunUncaughtExceptionMaximum = 10;

const NSInteger YunUncaughtExceptionHandlerSkipAddressCount = 4;
const NSInteger YunUncaughtExceptionHandlerReportAddressCount = 5;
static HWMonitor *instance = nil;


@implementation HWMonitor

+ (instancetype)sharedInstance {
    return [[[self class] alloc] init];
}

- (instancetype)init {
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [super init];
    });
    
    return instance;
}

+ (instancetype)allocWithZone:(struct _NSZone *)zone {
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [super allocWithZone:zone];
    });
    return instance;
}

+ (void)redirectNSlogToDocumentFolder {
    NSString *logFilePath = HWLOGPATCH;
    NSFileManager *defaultManager = [NSFileManager defaultManager];
    if (![defaultManager fileExistsAtPath:HWLOGPATCH]) {
        [defaultManager createDirectoryAtPath:logFilePath withIntermediateDirectories:YES attributes:nil error:nil];
    }
    NSString *fileName = @"LOG-HwLiveDemo.log";
    logFilePath = [logFilePath stringByAppendingPathComponent:fileName];
        
    if ([defaultManager fileExistsAtPath:logFilePath]) {
        float fileSize = [[defaultManager attributesOfItemAtPath:logFilePath error:nil] fileSize]/(1024.0*1021.0);
        // 删除超过50M的文件
        if (fileSize > 50) {
            [defaultManager removeItemAtPath:logFilePath error:nil];
        }
    }
    
    NSLog(@"%@",logFilePath);
    
    // 将log输入到文件
    freopen([logFilePath cStringUsingEncoding:NSASCIIStringEncoding], "a+", stdout);
    
    freopen([logFilePath cStringUsingEncoding:NSASCIIStringEncoding], "a+", stderr);
}
                                                        
@end


@implementation HWCrashMonitor
+ (void)startExceptionCollectLogs {
    YunInstallUncaughtExceptionHandler();
}

//begain
void YunInstallUncaughtExceptionHandler(void) {
    NSSetUncaughtExceptionHandler(&HandleException);
    //:调用abort函数生成的信号
    signal(SIGABRT, SignalHandler);
    //执行了非法指令. 通常是因为可执行文件本身出现错误, 或者视图执行数据段. 堆栈溢出时也有可能产生这个信号。
    signal(SIGILL, SignalHandler);
    //视图访问未分配给自己的内存, 或视图往没有写权限的内存地址写数据.
    signal(SIGSEGV, SignalHandler);
    //发生致命的算术运算错误时发出. 不仅包括浮点运算错误, 还包括溢出及除数为0等其它所有的算术的错误。
    signal(SIGFPE, SignalHandler);
    //非法地址, 包括内存地址对齐(alignment)出错。比如访问一个四个字长的整数, 但其地址不是4的倍数。它与SIGSEGV的区别在于后者是由于对合法存储地址的非法访问触发的(如访问不属于自己存储空间或只读存储空间)。
    signal(SIGBUS, SignalHandler);
    /*管道破裂。这个信号通常在进程间通信产生，
     比如采用FIFO(管道)通信的两个进程，读管道没打开或者意外终止就往管道写，
     写进程会收到SIGPIPE信号。此外用Socket通信的两个进程，
     写进程在写Socket的时候，读进程已经终止*/
    signal(SIGPIPE, SignalHandler);
}

void HandleException(NSException *exception) {
    int32_t exceptionCount = OSAtomicIncrement32(&YunUncaughtExceptionCount);
    if (exceptionCount > YunUncaughtExceptionMaximum) {
        return;
    }
    
    NSArray *callStack = [HWCrashMonitor backtrace];
    NSMutableArray *callStackMutable = [NSMutableArray arrayWithArray:callStack];
    NSMutableDictionary *userInfo =
        [NSMutableDictionary dictionaryWithDictionary:[exception userInfo]];
    [userInfo
        setObject:callStackMutable
        forKey:YunUncaughtExceptionHandlerAddressesKey];
    
    [[[HWMonitor alloc] init] performSelectorOnMainThread:@selector(handleException:)
                                               withObject:[NSException exceptionWithName:[exception name]
                                                                                  reason:[exception reason]
                                                                                userInfo:userInfo]
                                            waitUntilDone:YES];
}

void SignalHandler(int signal) {
    int32_t exceptionCount = OSAtomicIncrement32(&YunUncaughtExceptionCount);
    if (exceptionCount > YunUncaughtExceptionMaximum)
    {
        return;
    }
    
    NSMutableDictionary *userInfo =
        [NSMutableDictionary
            dictionaryWithObject:[NSNumber numberWithInt:signal]
            forKey:YunUncaughtExceptionHandlerSignalKey];

    NSArray *callStack = [HWCrashMonitor backtrace];
    NSMutableArray *callStackMutable = [NSMutableArray arrayWithArray:callStack];
    [userInfo
        setObject:callStackMutable
        forKey:YunUncaughtExceptionHandlerAddressesKey];
    
    [[[HWCrashMonitor alloc] init]
        performSelectorOnMainThread:@selector(handleException:)
        withObject:
            [NSException
                exceptionWithName:YunUncaughtExceptionHandlerSignalExceptionName
                reason:
                    [NSString stringWithFormat:
                        NSLocalizedString(@"Signal %d was raised.", nil),
                        signal]
                userInfo:
                    [NSDictionary
                        dictionaryWithObject:[NSNumber numberWithInt:signal]
                        forKey:YunUncaughtExceptionHandlerSignalKey]]
        waitUntilDone:YES];
}

void uncaughtExceptionHandler(NSException *exception) {

    NSArray *arr = [exception callStackSymbols];
    NSString *reason = [exception reason];
    NSString *name = [exception name];
    //可以根据这个路径去找到保存在本地的异常报告
    NSLog(@"arr = %@ ---- | --- nreason = %@ --- | --- name = %@",arr,reason,name);
}

+ (NSArray *)backtrace {
     void* callstack[128];
     int frames = backtrace(callstack, 128);
     char **strs = backtrace_symbols(callstack, frames);
     
     int i;
     NSMutableArray *backtrace = [NSMutableArray arrayWithCapacity:frames];
     for (
         i = YunUncaughtExceptionHandlerSkipAddressCount;
         i < YunUncaughtExceptionHandlerSkipAddressCount +
            YunUncaughtExceptionHandlerReportAddressCount;
        i++)
     {
         [backtrace addObject:[NSString stringWithUTF8String:strs[i]]];
     }
     free(strs);
     
     return backtrace;
}

- (void)validateAndSaveCriticalApplicationData{}

//错误日志开始发送到服务器 (此处就打印输出了)
- (void)handleException:(NSException *)exception {
    [self validateAndSaveCriticalApplicationData];
    NSString *errorStr = [NSString stringWithFormat:NSLocalizedString(
                                                                      @"error info details follow:%@%@", nil),
                          [exception reason],
                          [[exception userInfo] objectForKey:YunUncaughtExceptionHandlerAddressesKey]]
    ;
    
    if(errorStr) {
        NSLog(@"|||error:%@|||",errorStr);
    }
    NSLog(@"|||error:%s, %d|||", __FUNCTION__, __LINE__);
}

@end

