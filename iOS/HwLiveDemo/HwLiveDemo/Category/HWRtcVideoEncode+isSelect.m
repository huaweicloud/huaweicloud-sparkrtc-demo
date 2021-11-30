//
//  HWRtcVideoEncode+isSelect.m
//  HwLiveDemo
//
//  Created by HW on 2020/9/9.
//  Copyright © 2020 @YanYuXin. All rights reserved.
//

#import "HWRtcVideoEncode+isSelect.h"
#import "objc/runtime.h"

@implementation HWRtcVideoEncode (isSelect)
static char charKey;
static char maxBitRateKey;
@dynamic isSelect;
@dynamic maxBitRate;
- (NSValue *)isSelect {
     return objc_getAssociatedObject(self, &charKey);
}

- (void)setIsSelect:(NSValue *)isSelect {
    objc_setAssociatedObject(self, &charKey, isSelect, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}

- (NSString *)maxBitRate {
    return objc_getAssociatedObject(self, &maxBitRateKey);
}

- (void)setMaxBitRate:(NSString *)maxBitRate {
    objc_setAssociatedObject(self, &maxBitRateKey, maxBitRate, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}

- (void)encodeWithCoder:(NSCoder *)coder {
    [coder encodeObject:self.isSelect forKey:@"isSelect"];
    [coder encodeInteger:self.streamFlag forKey:@"streamFlag"];
    [coder encodeInteger:self.width forKey:@"width"];
    [coder encodeInteger:self.height forKey:@"height"];
    [coder encodeInteger:self.frameRate forKey:@"frameRate"];
    [coder encodeInteger:self.minFrameRate forKey:@"minFrameRate"];
    [coder encodeInteger:self.bitrate forKey:@"bitrate"];
    [coder encodeInteger:self.minBitrate forKey:@"minBitrate"];
    [coder encodeBool:self.disableAdjustRes forKey:@"disableAdjustRes"];
}

- (nullable instancetype)initWithCoder:(nonnull NSCoder *)coder {
    if (self = [super init]) {
        self.isSelect = [coder decodeObjectForKey:@"isSelect"];
        self.streamFlag = [coder decodeIntForKey:@"streamFlag"];
        self.width = [coder decodeIntForKey:@"width"];
        self.height = [coder decodeIntForKey:@"height"];
        self.frameRate = [coder decodeIntForKey:@"frameRate"];
        self.minFrameRate = [coder decodeIntForKey:@"minFrameRate"];
        self.bitrate = [coder decodeIntForKey:@"bitrate"];
        self.minBitrate = [coder decodeIntForKey:@"minBitrate"];
        self.disableAdjustRes = [coder decodeBoolForKey:@"disableAdjustRes"];
    }
    return self;
}


@end
