//
//  HwRtcVideoView.m
//  HwLiveDemo
//
//  Created by HW on 2020/4/2.
//  Copyright © 2020 @YanYuXin. All rights reserved.
//

#import "HwRtcVideoView.h"

@interface HwRtcVideoView ()
@property (nonatomic, strong) UILabel *textLabel;
@end

@implementation HwRtcVideoView

- (instancetype)init {
    if (self = [super init]) {
        _textLabel = [[UILabel alloc] init];
        _textLabel.textColor  =[UIColor whiteColor];
        _textLabel.frame = CGRectMake(20, 15, 250, 40);
        _textLabel.font = [UIFont systemFontOfSize:20];
        _textLabel.numberOfLines = 0;
        [self addSubview:_textLabel];
    }
    return self;
}


- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        _textLabel = [[UILabel alloc] init];
        _textLabel.frame = CGRectMake(20, 15, 250, 40);
        _textLabel.textColor  =[UIColor whiteColor];
        _textLabel.font = [UIFont systemFontOfSize:12];
        _textLabel.numberOfLines = 0;
        [self addSubview:_textLabel];
    }
    return self;
}

- (void)setUserName:(NSString *)userName {
//    _textLabel.hidden = NO;
    _textLabel.text = userName;
}

- (void)setIsFull:(BOOL)isFull {
    _isFull = isFull;
    if (isFull) {
        [UIView animateWithDuration:0.25 animations:^{
            self.textLabel.frame = CGRectMake(20, 15, 250, 40);
        }];
    } else {
        [UIView animateWithDuration:0.25 animations:^{
            self.textLabel.frame = CGRectMake(20, 120, 250, 40);
        }];
    }
}

- (void)setIsSubStreamAvailable:(BOOL)isSubStreamAvailable {
    _isSubStreamAvailable = isSubStreamAvailable;
}

- (void)setFrame:(CGRect)frame {
    [super setFrame:frame];
    if (_isSubStreamAvailable) {
        _textLabel.frame = CGRectMake(10, 5, 100, 40);
        return;
    }
    if (CGRectGetMinY(frame)== 0 && !_isFull) {
        _textLabel.frame = CGRectMake(20, 120, 250, 40);
    } else {
        _textLabel.frame = CGRectMake(20, 15, 250, 40);
    }
}
@end
