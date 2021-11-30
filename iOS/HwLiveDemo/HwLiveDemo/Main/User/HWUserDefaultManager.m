//
//  HWUserDefaultManager.m
//  HwLiveDemo
//
//  Created by HW on 2021/2/3.
//  Copyright © 2021 HW. All rights reserved.
//

#import "HWUserDefaultManager.h"
#define KHWUSERNAME @"userName"
#define KHWRtcStreamType @"streamType"

@implementation HWUserDefaultManager
+ (BOOL)isTemLogin {
    BOOL isLogin = NO;
    NSString *userName =  [HWUserDefaultManager getUserName];
    if (userName && userName.length > 0) {
        isLogin = YES;
    }
    return isLogin;
}

+ (void)setUserName:(NSString *)userName {
    [[NSUserDefaults standardUserDefaults] setValue:userName forKey:KHWUSERNAME];
}

+ (NSString *)getUserName {
    NSString *userName = [[NSUserDefaults standardUserDefaults] valueForKey:KHWUSERNAME];
    if (userName && userName.length > 0) {
        return userName;
    }
    return @"";
}

+ (void)setHWRtcStreamType:(HWRtcStreamType)type {
    NSNumber * numtype = [NSNumber numberWithShort:type];
    [[NSUserDefaults standardUserDefaults] setValue:numtype forKey:KHWRtcStreamType];
}

+ (HWRtcStreamType)getHWRtcStreamType {
    NSNumber *numtype = [[NSUserDefaults standardUserDefaults] valueForKey:KHWRtcStreamType];
    if (numtype ==nil) {
        return HWRtcStreamTypeLD;
    }
    HWRtcStreamType type = [numtype shortValue];
    return type;
}

@end
