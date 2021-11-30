//
//  HWVideoSession.m
//  HwLiveDemo
//
//  Created by HW on 2020/4/2.
//  Copyright © 2020 @YanYuXin. All rights reserved.
//

#import "HWVideoSession.h"
@implementation HWVideoSession
- (instancetype)initWithUserId:(nonnull NSString *)userId {
    if (self = [super init]) {
        self.canvas = [[HWRtcVideoCanvas alloc] init];
        self.canvas.uid = userId;
        _userid = userId;
        _isPlaying = NO;
        _isSubStream = NO;
    }
    return self;
}

- (void)setVideoView:(HwRtcVideoView *)videoView {
    _videoView = videoView;
    self.canvas.view = (HWRtcView *)_videoView;
}

- (void)setIsPlaying:(BOOL)isPlaying {
    _isPlaying = isPlaying;
    self.canvas.view = (HWRtcView *)_videoView;
}

- (NSString *)description {
    NSString *str = @"";
    str= [NSString stringWithFormat:@"videoView = %@,canvas = %@,userName = %@,userID = %@,isPlaying = %d,isSubStream = %d,|| frame = (%.0f,%.0f,%.0f,%.0f)",_videoView,_canvas,_userName,_canvas.uid,_isPlaying,_isSubStream,CGRectGetMinX(_videoView.frame),CGRectGetMinY(_videoView.frame),CGRectGetWidth(_videoView.frame),CGRectGetHeight(_videoView.frame)];
    return str;
    
}

@end
