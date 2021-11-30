
//
//  HRTCAudioStatsInfo+Type.m
//  HwLiveDemo
//
//  Created by HW on 2020/8/3.
//  Copyright © 2020 HW. All rights reserved.
//

#import "HWRtcAudioStatsInfo+Type.h"
#import <objc/runtime.h>
@implementation HWRtcAudioStatsInfo (Type)
static void *typeKey = "typeKey";
@dynamic type;
- (void)setType:(NSString *)type {
    objc_setAssociatedObject(self, &typeKey, type, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}

- (NSString *)type {
    return objc_getAssociatedObject(self, &typeKey);
}

@end
