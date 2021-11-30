//
//  HWReusePool.m
//  HwLiveDemo
//
//  Created by HW on 2020/4/2.
//  Copyright © 2020 @YanYuXin. All rights reserved.
//

#import "HWReusePool.h"

@interface HWReusePool ()
// 等待使用的队列
@property (nonatomic, strong) NSMutableSet *waitUsedQueue;
// 使用中的队列
@property (nonatomic, strong) NSMutableSet *usingQueue;

@end

@implementation HWReusePool
- (instancetype)init {
    self = [super init];
    if (self) {
        _waitUsedQueue = [NSMutableSet set];
        _usingQueue = [NSMutableSet set];
    }
    return self;
}

- (HwRtcVideoView *)dequeueReusableView {
    HwRtcVideoView *view = [_waitUsedQueue anyObject];
    if (view == nil) {
        return nil;
    } else {
        view.hidden = NO;
        [_waitUsedQueue removeObject:view];
        [_usingQueue addObject:view];
        return view;
    }
}

- (void)addVideoView:(HwRtcVideoView *)videoView {
    if (!videoView) {
        return;
    }
    
    [_usingQueue addObject:videoView];
    
}

- (void)removeVideoView:(HwRtcVideoView *)view {
    if (!view) {
        return;
    }
    view.hidden = YES;
    [_usingQueue removeObject:view];
    [_waitUsedQueue addObject:view];
}

- (void)reset {
    UIView *view = nil;
    while ((view = [_usingQueue anyObject])) {
        [_usingQueue removeObject:view];
        [_waitUsedQueue addObject:view];
    }
}

- (NSUInteger)count {
    return _usingQueue.count + _waitUsedQueue.count;
}

@end
