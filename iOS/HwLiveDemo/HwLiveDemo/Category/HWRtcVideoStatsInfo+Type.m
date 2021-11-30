//
//  HRTCVideoStatsInfo+Type.m
//  HwLiveDemo
//
//  Created by HW on 2020/8/3.
//  Copyright © 2020 HW. All rights reserved.
//

#import "HWRtcVideoStatsInfo+Type.h"
#import "objc/runtime.h"
@implementation HWRtcVideoStatsInfo (Type)
static char charKey;
@dynamic type;
- (void)setType:(NSString *)type {
    objc_setAssociatedObject(self, &charKey, type, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}

- (NSString *)getType {
    return objc_getAssociatedObject(self, &charKey);
}

- (NSString *)type {
    return objc_getAssociatedObject(self, &charKey);
}

@end
