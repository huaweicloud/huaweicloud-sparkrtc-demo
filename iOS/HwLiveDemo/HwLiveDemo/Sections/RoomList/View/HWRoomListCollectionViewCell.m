//
//  HWRoomListCollectionViewCell.m
//  HW
//
//  Created by HW on 2019/7/23.
//  Copyright © 2019年 HW. All rights reserved.
//

#import "HWRoomListCollectionViewCell.h"
@interface HWRoomListCollectionViewCell ()
@property (nonatomic, strong) UIImageView *bgImageView;
/// 遮阴
@property (nonatomic, strong) UIView *shadeView;
@property (nonatomic, strong) UILabel *roomNameLabel;
@end

@implementation HWRoomListCollectionViewCell

- (instancetype)init {
    self = [super init];
    if (self) {
        [self createUI];
    }
    return self;
}

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        [self createUI];
        self.backgroundColor = [UIColor colorWithString:@"#434B5C"];
        self.layer.cornerRadius = 8;
        self.layer.masksToBounds = YES;
    }
    return self;
}

- (void)createUI {
    _bgImageView = [[UIImageView alloc] init];
    _bgImageView.frame = CGRectMake(0, 0, CGRectGetWidth(self.frame), CGRectGetWidth(self.frame));
    _bgImageView.clipsToBounds = YES;
    _bgImageView.contentMode = UIViewContentModeScaleAspectFill;
    _bgImageView.userInteractionEnabled = YES;
    [self addSubview:_bgImageView];
    CGFloat height = CGRectGetWidth(self.frame) == ((SCREENWIDTH - 48)/3)?26.9:40;
    _shadeView = [[UIView alloc] initWithFrame:CGRectMake(0, CGRectGetHeight(self.frame) - height, CGRectGetWidth(self.frame), height)];
    [_shadeView.layer addSublayer:[HWTool getGradientLayerWithFrame:CGRectMake(0, 0, CGRectGetWidth(self.frame), height) frameColor:[UIColor colorWithString:@"#000000" alpha:1.0] toColot:[UIColor colorWithString:@"#000000" alpha:0.0]]];
    [self addSubview:_shadeView];
    
    _roomNameLabel = [[UILabel alloc] init];
    _roomNameLabel.textAlignment=NSTextAlignmentLeft;
    _roomNameLabel.textColor = [UIColor colorWithString:@"#FFFFFF"];
    _roomNameLabel.text = @"0000000";
    _roomNameLabel.alpha = 0.8;
    _roomNameLabel.numberOfLines = 0;
    _roomNameLabel.font = [UIFont systemFontOfSize:12];
    [self addSubview:self.roomNameLabel];

    [_roomNameLabel mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.equalTo(self.mas_left).mas_offset(15);
        make.right.equalTo(self.mas_right).mas_offset(-70);
        make.bottom.equalTo(self.mas_bottom).mas_offset(-8);
        make.height.mas_offset(30);
    }];
}

- (void)configLiveModel:(HWLiveRoomModel *)model {
    self.bgImageView.image = [UIImage imageNamed:model.imageName];
    self.roomNameLabel.text = model.roomName;
}

@end
