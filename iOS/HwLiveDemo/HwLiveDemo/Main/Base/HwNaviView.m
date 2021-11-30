//
//  HwNaviView.m
//  HwLiveDemo
//
//  Created by HW on 2019/4/3.
//  Copyright © 2019年 YanYuXin. All rights reserved.
//

#import "HwNaviView.h"
#import "UIColor+HWColor.h"
#import "Masonry.h"

@interface HwNaviView()

//左侧按钮
@property (nonatomic, strong)UIButton *leftBtn;

//中间标题
@property (nonatomic, strong) UILabel *titleLab;

@end
@implementation HwNaviView

- (instancetype)init {
    if (self = [super init]) {
        [self initView];
    }
    return self;
}

- (instancetype)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        [self initView];
    }
    return self;
}

- (void)initView {
    //背景颜色设置为透明
    self.backgroundColor = [UIColor clearColor];
    //创建左侧按钮
    [self addSubview:self.leftBtn];
    CGFloat leftbtnY = IS_iPhone_X_SERIES ? 44.0f: 27.0f;
    [self.leftBtn mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.equalTo(self.mas_top).offset(leftbtnY);
        make.left.equalTo(self.mas_left).offset(10);
        make.width.mas_equalTo(@21);
        make.height.mas_equalTo(@21);
    }];
    //创建中间标题
    [self addSubview:self.titleLab];
    CGFloat midLabelY = IS_iPhone_X_SERIES ? 44.0f: 27.0f;
    [self.titleLab mas_makeConstraints:^(MASConstraintMaker *make) {
        make.centerX.equalTo(self.mas_centerX).offset(0);
        make.top.equalTo(self.mas_top).offset(midLabelY);
        make.left.equalTo(self.mas_left).offset(40);
        make.right.equalTo(self.mas_right).offset(-40);
        make.height.mas_equalTo(@18);
    }];
    
}

#pragma mark action

- (void)nav_leftClickAction:(UIButton *)sender {
    if (self.delegate && [self.delegate respondsToSelector:@selector(navigationView:clickLeftBtn:)]) {
        [self.delegate navigationView:self clickLeftBtn:sender];
    }
    
    if (self.leftTapBlock) {
        self.leftTapBlock();
    }
}

- (void)setTitleLabStr:(NSString *)str {
    self.titleLab.text = str;
}

#pragma mark getter

- (UIButton *)leftBtn {
    if (_leftBtn == nil) {
        _leftBtn = [UIButton buttonWithType:UIButtonTypeCustom];
        [_leftBtn setImage:[UIImage imageNamed:@"icon_back"] forState:UIControlStateNormal];
        [_leftBtn addTarget:self action:@selector(nav_leftClickAction:) forControlEvents:UIControlEventTouchUpInside];
    }
    return _leftBtn;
}

- (UILabel *)titleLab {
    if (_titleLab == nil) {
        _titleLab = [[UILabel alloc]init];
        _titleLab.font = [UIFont systemFontOfSize:18.0];
        _titleLab.textColor = [UIColor colorWithString:@"#0080bd"];
        _titleLab.textAlignment = NSTextAlignmentCenter;
    }
    return _titleLab;
}
/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

@end
