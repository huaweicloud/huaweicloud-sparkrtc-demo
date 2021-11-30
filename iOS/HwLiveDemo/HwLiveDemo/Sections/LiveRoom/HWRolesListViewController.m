//
//  HWRolesListViewController.m
//  HWRtcDemo
//
//  Created by hw on 2020/2/18.
//  Copyright © 2020 @YanYuXin. All rights reserved.
//


#import "HWRolesListViewController.h"
@interface HWRolesListViewController ()<UITableViewDelegate,UITableViewDataSource>
@property (weak, nonatomic) IBOutlet NSLayoutConstraint *top;
@property (weak, nonatomic) IBOutlet UITableView *myTableView;
@property (nonatomic, strong) NSMutableArray *allDataArray;
@end

@implementation HWRolesListViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.isHiddenNav = NO;
    NSString* title = [NSString stringWithFormat:@"Members of the Live (%lu)",(unsigned long)self.viewsArray.count];
    self.navTitle = title;
    self.top.constant = self.navHeight;
}

#pragma mark - UITableViewDdelegate && UITableViewDatasource
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    NSArray *array = self.allDataArray[section];
    return array.count;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return self.allDataArray.count;
}

- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section {
    if (self.allDataArray.count >1) {
        return 30;
    } else {
        return 0.01;
    }
}

- (UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section {
    if (self.allDataArray.count > 1) {
        UIView *view = [[UIView alloc] initWithFrame:CGRectMake(0, 0, SCREENWIDTH, 30)];
        UILabel *label = [[UILabel alloc] initWithFrame:CGRectMake(IS_iPhone_X_SERIES?60:30, 7.5, 200, 15)];
        if (section == 0) {
            label.text = @"选看列表";
        } else {
            label.text = @"房间成员列表";
        }
        [view addSubview:label];
        return view;
    }
    return nil;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *identifier = @"cell";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:   identifier];
    if (!cell) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:identifier];
        cell.selectionStyle = UITableViewCellSelectionStyleNone;
    }
    if (indexPath.section==0) {
        HWVideoSession *videoSession = [self.viewsArray objectAtIndex:indexPath.row];
        if (videoSession.isPlaying) {                            
            cell.imageView.image = [UIImage imageNamed:@"role_lis_video_on"];
        } else
        {
            cell.imageView.image = [UIImage imageNamed:@"role_lis_video_off"];
        }
        cell.textLabel.text = videoSession.userName;
    } else  {
        HWVideoSession *videoSession = [self.memberArray objectAtIndex:indexPath.row];
        cell.imageView.image = [UIImage imageNamed:@"role_lis_video_off"];
        cell.textLabel.text = videoSession.userName;
    }

    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    if (indexPath.section == 1) {
        [self showMessage:@"can't choose member!"];
        return;
    }
    HWVideoSession *videoSession = [self.viewsArray objectAtIndex:indexPath.row];
    if ([videoSession.userid isEqualToString: [HWTool getUUID]]) {
        [self showMessage:@"can't choose to watch locally!"]; // 无法选看自己
        return;
    }
    if (self.isSubStreamAvailable && [videoSession.userid containsString:@"teacher_"]) {
        if (videoSession.isPlaying) {
            self.maxShowCount = 6;
        } else {
            self.maxShowCount = 7;
        }
    }
    if (self.maxShowCount <= self.showingCount) { // 选看的无法超过可以选看个数
        if(!videoSession.isPlaying){
            [self showMessage:@"show view more than limit!"];
            NSLog(@"show view more than limit!");
            return;
        }
    }
    videoSession.isPlaying = !videoSession.isPlaying; // 选看状态切换
    if (!videoSession.isPlaying) {
        [self.reusePool removeVideoView:videoSession.videoView];
        videoSession.videoView = nil;
        self.showingCount--;
        
    } else {
        videoSession.videoView = [self.reusePool dequeueReusableView];
        if (!videoSession.videoView) {
            videoSession.videoView = [[HwRtcVideoView alloc] initWithFrame:CGRectMake(0, 0, 90, 160)];
            [self.reusePool addVideoView:videoSession.videoView];
            NSLog(@"===重用池出错了===");
        }
        [videoSession.videoView setUserName:videoSession.userName];
        self.showingCount++;
    }
    BOOL result = YES;
    if (videoSession.isSubStream) { /// 判断是否是辅流
        if (videoSession.isPlaying) {
            result = [_rtcEngine startRemoteAuxiliaryStreamView:videoSession.canvas streamType:_streamType];
            if (result == 0) {
                [_rtcEngine updateRemoteAuxiliaryStreamRenderMode:videoSession.canvas.uid displayMode:HWRtcVideoDisplayModeFit mirrorMode:HWRtcVideoMirrorTypeDisable];
            }
        } else {
            result = [_rtcEngine stopRemoteAuxiliaryStreamView:videoSession.userid];
        }

    } else {
        if (videoSession.isPlaying) {
            result = [_rtcEngine startRemoteStreamView:videoSession.canvas
                                            streamType:_streamType
                                      disableAdjustRes:NO];
            if (result == 0) {
                [self.rtcEngine updateRemoteRenderMode:videoSession.canvas.uid displayMode:HWRtcVideoDisplayModeHidden mirrorMode:HWRtcVideoMirrorTypeDisable];
            }
        } else {
            result = [_rtcEngine stopRemoteStreamView:videoSession.userid];
        }
    }
    NSLog(@"|||sdk==%@ ,%@,窗口对象==%@,result = %d,videoSession =%@,窗口hidden=%d,窗口父窗口==%@,%@|||",
          _rtcEngine,
          (videoSession.isPlaying?@"设置窗口":@"取消窗口"),
          videoSession.canvas.view,
          result,
          videoSession,
          videoSession.canvas.view.hidden,
          videoSession.canvas.view.superview,
          videoSession.canvas.view.class);
    [self.myTableView reloadData];
    if (self.listViewStatusBlock) {
        self.listViewStatusBlock(videoSession);
    }
}

- (void)showMessage:(NSString *)message {
    dispatch_async(dispatch_get_main_queue(), ^{
        UIAlertController *alertc = [UIAlertController alertControllerWithTitle:@"提示:" message:message preferredStyle:UIAlertControllerStyleAlert];
        UIAlertAction *actioSure = [UIAlertAction actionWithTitle:@"确认" style:UIAlertActionStyleDefault handler:nil];
        [alertc addAction:actioSure];
        [self presentViewController:alertc animated:YES completion:nil];
    });
}

- (void)setViewsArray:(NSMutableArray *)viewsArray {
    _viewsArray = viewsArray;
    [self.allDataArray removeAllObjects];
    [self.allDataArray addObject:_viewsArray];
    if (self.memberArray.count > 0) {
        [self.allDataArray addObject:self.memberArray];
    }
    [self.myTableView reloadData];
    NSString* title = [NSString stringWithFormat:@"Members of the Live (%lu)",(unsigned long)self.viewsArray.count];
    self.navTitle = title;
}

- (void)setMemberArray:(NSMutableArray *)memberArray {
    _memberArray = memberArray;
    [self.allDataArray removeAllObjects];
    if (self.viewsArray.count > 0) {
        [self.allDataArray addObject:self.viewsArray];
    }
    [self.allDataArray addObject:_memberArray];
    [self.myTableView reloadData];
}

- (void)setStreamType:(HWRtcStreamType )streamType {
    _streamType = streamType;
    NSLog(@"%ld",(long)streamType);
}

- (NSMutableArray *)allDataArray {
    if (!_allDataArray) {
        _allDataArray = [NSMutableArray array];
    }
    return _allDataArray;
}
@end
