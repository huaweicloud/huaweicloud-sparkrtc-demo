//
//  HWRealTimeInfoView.m
//  HwLiveDemo
//
//  Created by HW on 2020/5/19.
//  Copyright © 2020 @YanYuXin. All rights reserved.
//

#import "HWRealTimeInfoView.h"
#import "HWRealTimeInforTableViewCell.h"
#import "HWVideoSession.h"
#import "HWRtcVideoStatsInfo+Type.h"
#import "HWRtcAudioStatsInfo+Type.h"
@interface HWRealTimeInfoView ()<UITableViewDelegate,UITableViewDataSource>

@property (nonatomic, strong) UITableView *myTableView;
@property (nonatomic, strong) NSMutableArray *dataArray;
@property (nonatomic, strong) NSMutableArray *videoArray;
@property (nonatomic, strong) NSMutableArray *audioArray;
@property (nonatomic, strong) NSMutableArray *subStreamArray;
@end

@implementation HWRealTimeInfoView

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        self.alpha = 0.6;
        self.backgroundColor =[UIColor clearColor];
        [self addSubview:self.myTableView];
    }
    return self;
}

- (void)addVideoArray:(NSArray *)videoArray
{
    @synchronized (self) {
        [self.videoArray removeAllObjects];
        [self.videoArray addObjectsFromArray:videoArray];
    }
}

- (void)addSubStreamVideo:(HWRtcVideoStatsInfo *)videoStatsInfo
{
    @synchronized (self) {
        [self.subStreamArray removeAllObjects];
        [self.subStreamArray addObject:videoStatsInfo];
    }
}

- (void)addAudioArray:(NSArray *)audioArray
{
    @synchronized (self) {
        [self.audioArray removeAllObjects];
        [self.audioArray addObjectsFromArray:audioArray];
    }
}

- (void)reloadView
{
    if (self.hidden) {
        return;
    }
    @synchronized (self) {
        [self.dataArray removeAllObjects];
        NSMutableArray *allVideoArray = [NSMutableArray array];
        if (self.subStreamArray.count == 1 && self.isSubStreamAvailable) {
            HWRtcVideoStatsInfo *videoStatsInfo = [self.subStreamArray firstObject];
            [allVideoArray addObject:videoStatsInfo];
        } else
        {
            [self.subStreamArray removeAllObjects];
        }
        [allVideoArray addObjectsFromArray:self.videoArray];
        [self.dataArray addObject:allVideoArray];
        [self.dataArray addObject:self.audioArray];
        [self.myTableView reloadData];
    }

}

#pragma mark - UITableViewDelegate && UITableViewDataSource
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    if (section == 0) {
        NSArray *array = [self.dataArray objectAtIndex:0];
        return array.count;
    } else if(section == 1) {
        NSArray *array = [self.dataArray objectAtIndex:1];
        return array.count;
    }
    
    return 0;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return self.dataArray.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *identifier = @"HWRealTimeInforTableViewCell";
     
    HWRealTimeInforTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:identifier];
    if (!cell) {
        cell = [[[NSBundle mainBundle] loadNibNamed:@"HWRealTimeInforTableViewCell" owner:nil options:nil] firstObject];
    }
    cell.cellWidth = CGRectGetWidth(self.frame);
    switch (indexPath.section) {
        case 0:
        {
            NSArray *array = [self.dataArray objectAtIndex:0];
            NSInteger index = indexPath.row;
            HWRtcVideoStatsInfo *statsInfo = array[index];
            [cell reloadType:@"video"
                         uid:statsInfo.userId
                     bitRate:(int)statsInfo.bitRate
                       width:(int)statsInfo.width
                      height:(int)statsInfo.height
                   frameRate:(int)statsInfo.frameRate
                  packetLoss:(int)statsInfo.packetLoss
                       delay:(int)statsInfo.delay
                      jitter:(int)statsInfo.jitter];
        }
            break;
        case 1:
        {
            NSArray *array = [self.dataArray objectAtIndex:1];
            HWRtcAudioStatsInfo *statsInfo = array[indexPath.row];
            [cell reloadType:@"audio"
                         uid:statsInfo.userId
                     bitRate:(int)statsInfo.bitRate
                       width:-1
                      height:-1
                   frameRate:-1
                  packetLoss:(int)statsInfo.packetLoss
                       delay:(int)statsInfo.delay
                      jitter:(int)statsInfo.jitter];
        }
            break;
            
        default:
            break;
    }
    return cell;
}


- (UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section {
    UIView *stackView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, CGRectGetWidth(self.frame), 40)];
    UILabel *label = [[UILabel alloc] init];
    label.text = @"type";
    [stackView addSubview:label];
    UILabel *label1 = [[UILabel alloc] init];
    label1.text = @"uid";
    [stackView addSubview:label1];
    UILabel *label2 = [[UILabel alloc] init];
    label2.text = @"码率";
    [stackView addSubview:label2];
    UILabel *label3 = [[UILabel alloc] init];
    label3.text = @"分辨率";
    [stackView addSubview:label3];
    UILabel *label4 = [[UILabel alloc] init];
    label4.text = @"帧率";
    [stackView addSubview:label4];
    UILabel *label5 = [[UILabel alloc] init];
    label5.text = @"丢包";
    [stackView addSubview:label5];
    UILabel *label6 = [[UILabel alloc] init];
    label6.text = @"延时";
    [stackView addSubview:label6];
    UILabel *label7 = [[UILabel alloc] init];
    label7.text = @"抖动";
    [stackView addSubview:label7];

    NSMutableArray *array = [NSMutableArray array];
    if (section == 0) {
        [array addObject:label];
        [array addObject:label1];
        [array addObject:label2];
        [array addObject:label3];
        [array addObject:label4];
        [array addObject:label5];
        [array addObject:label6];
        [array addObject:label7];
    } else {
        [array addObject:label];
        [array addObject:label1];
        [array addObject:label2];
        [array addObject:label5];
        [array addObject:label6];
        [array addObject:label7];
    }
    CGFloat width = CGRectGetWidth(self.frame) / array.count;
    for (int i = 0; i < array.count; i++) {
        UILabel *label = [array objectAtIndex:i];
        label.frame = CGRectMake(width * i, 0, width, 40);
        label.textAlignment = NSTextAlignmentCenter;
        label.font = [UIFont systemFontOfSize:14];
        label.layer.borderColor = [UIColor blackColor].CGColor;//边框颜色
        label.layer.borderWidth = 0.5;
    }
    return stackView;
}


- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section {
    return 40;
}

- (CGFloat)tableView:(UITableView *)tableView heightForFooterInSection:(NSInteger)section {
    return 0.1;
}
#pragma mark -setter&&getter

- (UITableView *)myTableView {
    if (!_myTableView) {
        _myTableView = [[UITableView alloc] initWithFrame:CGRectMake(0, 0, self.frame.size.width,self.frame.size.height) style:UITableViewStyleGrouped];
        _myTableView.delegate = self;
        _myTableView.dataSource = self;
        _myTableView.separatorStyle = UITableViewCellSeparatorStyleNone;
        [_myTableView setShowsVerticalScrollIndicator:NO];
        [_myTableView setShowsHorizontalScrollIndicator:NO];
    }
    return _myTableView;
}

- (NSMutableArray *)dataArray {
    if (!_dataArray) {
        _dataArray = [NSMutableArray array];
        NSArray *arrayVideo = [NSArray array];
        NSArray *arrayAudio = [NSArray array];
        [_dataArray addObject:arrayVideo];
        [_dataArray addObject:arrayAudio];
    }
    return _dataArray;
}

- (NSMutableArray *)videoArray {
    if (!_videoArray) {
        _videoArray = [NSMutableArray array];
    }
    return _videoArray;
}

- (NSMutableArray *)audioArray {
    if (!_audioArray) {
        _audioArray = [NSMutableArray array];
    }
    return _audioArray;
}

- (NSMutableArray *)subStreamArray {
    if (!_subStreamArray) {
        _subStreamArray = [NSMutableArray array];
    }
    return _subStreamArray;
}

- (void)setFrame:(CGRect)frame {
    [super setFrame:frame];
    self.myTableView.frame = CGRectMake(0, 0, self.frame.size.width,self.frame.size.height);
}

@end
