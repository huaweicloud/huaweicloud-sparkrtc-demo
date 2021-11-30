//
//  AppDelegate.m
//  HwLiveDemo
//
//  Created by HW on 2020/7/13.
//  Copyright © 2020 HW. All rights reserved.
//

#import "AppDelegate.h"
#import "HWMonitor.h"
@interface AppDelegate ()

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    #ifdef DEBUG
    #else
    [HWMonitor redirectNSlogToDocumentFolder];
    [HWCrashMonitor startExceptionCollectLogs];
    #endif
    
    return YES;
}

- (UIInterfaceOrientationMask)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(nullable UIWindow *)window {
    if (_allowRotation == YES) {
        return UIInterfaceOrientationMaskLandscapeRight;
    } else {
        return (UIInterfaceOrientationMaskPortrait);
    }
}

@end
