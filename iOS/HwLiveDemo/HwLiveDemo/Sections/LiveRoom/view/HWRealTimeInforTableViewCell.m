//
//  HWRealTimeInforTableViewCell.m
//  HwLiveDemo
//
//  Created by HW on 2020/5/18.
//  Copyright © 2020 @YanYuXin. All rights reserved.
//

#import "HWRealTimeInforTableViewCell.h"

@interface HWRealTimeInforTableViewCell ()
@property (weak, nonatomic) IBOutlet UILabel *videoLabel;
@property (weak, nonatomic) IBOutlet UILabel *uidLabel;
@property (weak, nonatomic) IBOutlet UILabel *bitRateLabel;
@property (weak, nonatomic) IBOutlet UILabel *resolutionLabel;
@property (weak, nonatomic) IBOutlet UILabel *frameRateLabel;
@property (weak, nonatomic) IBOutlet UILabel *packetLossLabel;
@property (weak, nonatomic) IBOutlet UILabel *delayLabel;
@property (weak, nonatomic) IBOutlet UILabel *jitterLabel;

@end
@implementation HWRealTimeInforTableViewCell

- (void)awakeFromNib {
    [super awakeFromNib];
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

- (void)reloadType:(NSString *)type
               uid:(NSString *)uid
           bitRate:(int)bitRate
             width:(int)width
            height:(int)height
         frameRate:(int)frameRate
        packetLoss:(int)packetLoss
             delay:(int)delay
            jitter:(int)jitter {
    if (width == -1 && height == -1) {
        self.resolutionLabel.hidden = YES;
    }
    if (frameRate == -1) {
        self.frameRateLabel.hidden = YES;
    }
    [self reloadUIIsVideo:!self.resolutionLabel.hidden];
    self.videoLabel.text        = type;
    self.uidLabel.text          = uid;
    self.bitRateLabel.text      = [NSString stringWithFormat:@"%d",bitRate];
    self.resolutionLabel.text   = [NSString stringWithFormat:@"%dP",MIN(width, height)];
    self.frameRateLabel.text    = [NSString stringWithFormat:@"%d",frameRate];
    self.packetLossLabel.text   = [NSString stringWithFormat:@"%d",packetLoss];
    self.delayLabel.text        = [NSString stringWithFormat:@"%d",delay];
    self.jitterLabel.text       = [NSString stringWithFormat:@"%d",jitter];
}

- (void)reloadUIIsVideo:(BOOL)isVideo {
    NSMutableArray *array = [NSMutableArray array];
    if (isVideo) {
        [array addObject:self.videoLabel];
        [array addObject:self.uidLabel];
        [array addObject:self.bitRateLabel];
        [array addObject:self.resolutionLabel];
        [array addObject:self.frameRateLabel];
        [array addObject:self.packetLossLabel];
        [array addObject:self.delayLabel];
        [array addObject:self.jitterLabel];
    } else {
        [array addObject:self.videoLabel];
        [array addObject:self.uidLabel];
        [array addObject:self.bitRateLabel];
        [array addObject:self.packetLossLabel];
        [array addObject:self.delayLabel];
        [array addObject:self.jitterLabel];
    }
    CGFloat width = _cellWidth / array.count;
    for (int i = 0; i < array.count; i++) {
        UILabel *label = [array objectAtIndex:i];
        label.frame = CGRectMake(width * i, 0, width, 40);
        label.layer.borderColor = [UIColor blackColor].CGColor;//边框颜色
        label.layer.borderWidth = 0.5;
    }
}



@end
