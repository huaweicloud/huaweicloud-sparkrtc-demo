//
//  HWSubStreamViewController.m
//  HwLiveDemo
//
//  Created by weshow on 2020/7/29.
//  Copyright © 2020 HW. All rights reserved.
//

#import "HWSubStreamViewController.h"
#import "UIViewController+Orientation.h"
#import "HWRolesListViewController.h"
#import "HWRealTimeInfoView.h"
@interface HWSubStreamViewController ()<HWRtcEngineDelegate>
@property (weak, nonatomic) IBOutlet UILabel *labelRoomID;
@property (weak, nonatomic) IBOutlet UIView *cameraView;
@property (weak, nonatomic) IBOutlet UIView *monitorView;
@property (weak, nonatomic) IBOutlet UIView *viewInfo;
@property (weak, nonatomic) IBOutlet NSLayoutConstraint *topInfoTopConstraint;
@property (weak, nonatomic) IBOutlet NSLayoutConstraint *tabViewBottonConstraint;
@property (weak, nonatomic) IBOutlet UIButton *changeRoleTypeButton;
@property (weak, nonatomic) IBOutlet UIView *tabView;
@property (weak, nonatomic) IBOutlet UIButton *localVideoButton;
@property (weak, nonatomic) IBOutlet UIButton *microphoneButton;
@property (weak, nonatomic) IBOutlet UIButton *localAudioButton;
@property (weak, nonatomic) IBOutlet UIButton *roleListButton;
@property (weak, nonatomic) IBOutlet UIButton *switchCameraButton;
@property (weak, nonatomic) IBOutlet UIButton *leaveRoomButton;

@property (nonatomic, strong) NSString *userID;
@property (nonatomic, strong) NSString *userName;
@property (strong, nonatomic) HWVideoSession *localVideoSession;
@property (strong, nonatomic) HWVideoSession *subStreamVideoSession;
@property (nonatomic, strong) NSMutableArray *viewsArray;
@property (nonatomic, strong) NSMutableArray *showViewsArray;
@property (nonatomic, strong) NSMutableDictionary *heartBodyDict;
@property (nonatomic, strong) UIActivityIndicatorView * activityIndicator;

@property (nonatomic, strong) HWRolesListViewController *rolesListVC;
@property (nonatomic, strong) HWRealTimeInfoView *realTimeInfoView;
@property (nonatomic, assign) HWRtcStreamType streamType;

///重用池
@property (nonatomic, strong) HWReusePool *reusePool;
@property (nonatomic, assign) HWRtcStreamType defaultStreamType;
@property (nonatomic, assign) HWRtcVideoDisplayMode displayMode;

@property (nonatomic, assign) int maxShowCount;
@property (nonatomic, assign) BOOL isJoinroomSuccess;
@property (nonatomic, assign) BOOL isSubStreamAvailable; ///辅流是否可用
@property (nonatomic, assign) BOOL isFull;
@property (nonatomic, assign) BOOL isMute;
@end

@implementation HWSubStreamViewController
@dynamic reusePool;
@dynamic viewsArray;
@dynamic showViewsArray;
@dynamic realTimeInfoView;

- (HWRolesListViewController *)rolesListVC
{
    if (!_rolesListVC) {
        _rolesListVC = [[HWRolesListViewController alloc] initWithNibName:@"HWRolesListViewController" bundle:nil];
        _rolesListVC.isSubStreamAvailable = YES;
    }
    return _rolesListVC;
}

- (int)maxShowCount {
    return 7;
}

- (void)viewDidLoad {
    [super viewDidLoad];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self setNewOrientation:YES];
    self.realTimeInfoView.frame = CGRectMake(80, (SCREENHEIGHT-300)/2, SCREENWIDTH-200, 300);
}

- (void)viewDidDisappear:(BOOL)animated
{
    [super viewDidDisappear:animated];
    if (!self.navigationController.viewControllers) {
        [self setNewOrientation:NO];
    }
}

- (void)reloadViewWithDataArray:(NSMutableArray *)dataArray {
    CGFloat offset = 0;
    CGFloat offsetBottom = 0;
    CGFloat width = 0;
    CGFloat height = 0;
    width = SCREENWIDTH;
    height = SCREENHEIGHT;
    if (IS_iPhone_X_SERIES) {
        offset = 33;
        offsetBottom = 32;
    }

    /// 辅流视图UI设置
    self.subStreamVideoSession.videoView.frame = CGRectMake(offset + 60, 0, SCREENWIDTH - 160 - offset - offsetBottom, SCREENHEIGHT-100);
    self.subStreamVideoSession.videoView.isSubStreamAvailable = self.isSubStreamAvailable;
    self.subStreamVideoSession.videoView.hidden = !self.isSubStreamAvailable;
    BOOL isHasTeacher = NO;
    
    NSMutableArray *remoteVideoArray = [NSMutableArray array];
    for (int i = 0; i < dataArray.count; i++) {
        HWVideoSession *videoSession = [dataArray objectAtIndex:i];
        videoSession.videoView.isSubStreamAvailable = YES;
        if ([videoSession.userid containsString:@"teacher_"]) { /// 携带teacher_为老师
            videoSession.videoView.frame = CGRectMake(SCREENWIDTH - 100 - offsetBottom, 0, 100, 100);
            [videoSession.videoView setUserName:videoSession.userName];
            isHasTeacher = YES;
        } else {
            if (remoteVideoArray.count <= 6) { //有可能没teacher的.学生就选看7个了
                [remoteVideoArray addObject:videoSession];
            }
        }
    }
    if (!isHasTeacher && dataArray.count > 6) {
        HWVideoSession *lastVideoSession = [self.showViewsArray lastObject];
        [self.reusePool removeVideoView:lastVideoSession.videoView];
        lastVideoSession.videoView = nil;
        lastVideoSession.isPlaying = NO;
        [self.rtcEngine stopRemoteStreamView:lastVideoSession.userid];
        [self.showViewsArray removeObject:lastVideoSession];
        self.rolesListVC.viewsArray = self.viewsArray; ///刷新视图
    }
    for (int i = 0; i < remoteVideoArray.count; i++) { // 学生视图的UI布局
        HWVideoSession *videoSession = [remoteVideoArray objectAtIndex:i];
        videoSession.videoView.frame = CGRectMake(offset + (SCREENWIDTH-offset-offsetBottom)/6 * i, SCREENHEIGHT-100, (SCREENWIDTH-offset-offsetBottom)/6,100);
        [videoSession.videoView setUserName:videoSession.userName];
    }
    NSLog(@"|||更新视图的数组内容:%@|||",dataArray);
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    _isFull = !_isFull;
    NSLog(@"===>isFull = %d,%f",_isFull,self.tabViewBottonConstraint.constant);
    if (_isFull) {
        [UIView animateWithDuration:0.25 animations:^{
            self.topInfoTopConstraint.constant = 0;
            self.tabViewBottonConstraint.constant = - 60;
            self.changeRoleTypeButton.hidden = YES;
        }];
    } else {
        [UIView animateWithDuration:0.25 animations:^{
            self.topInfoTopConstraint.constant = 60;
            self.tabViewBottonConstraint.constant = 20;
            self.changeRoleTypeButton.hidden = NO;
        }];
    }
}


#pragma mark - HwRtcEngineDelegate
/// 辅流回调
- (void)onUserAuxiliaryStreamAvailable:(NSString *)roomId userId:(NSString *)userId isAvailable:(BOOL)isAvailable 
 {
    dispatch_async(dispatch_get_main_queue(), ^{

        self.isSubStreamAvailable = isAvailable;
        self.realTimeInfoView.isSubStreamAvailable = isAvailable;
        if (isAvailable) {
           /// 如果辅流进来 只保持一个辅流全屏。
            self.maxShowCount = 7;
            if (!self.subStreamVideoSession) {
                self.subStreamVideoSession = [[HWVideoSession alloc] initWithUserId:userId];
                self.subStreamVideoSession.isSubStream = YES;
            } else {
                self.subStreamVideoSession.userid = userId;
                self.subStreamVideoSession.canvas.uid = userId;
            }
            self.subStreamVideoSession.isPlaying = YES;
            self.subStreamVideoSession.userName  = [NSString stringWithFormat:@"辅流-%@",userId];
            if (!self.subStreamVideoSession.videoView) {
                self.subStreamVideoSession.videoView = [[HwRtcVideoView alloc] initWithFrame:self.view.bounds];
                [self.cameraView addSubview:self.subStreamVideoSession.videoView];
            } else {            
                self.subStreamVideoSession.videoView.hidden = NO;
            }
            [self.subStreamVideoSession.videoView setUserName:self.subStreamVideoSession.userName];
//            int result = [self.rtcEngine startRemoteSubStreamView:self.subStreamVideoSession.canvas streamType:self.streamType];
//            if (result) {
//                [self.rtcEngine setRemoteSubStreamViewDisplayMode:HWRtcVideoDisplayModeFit userid:self.subStreamVideoSession.userid];
//            }
            int result = [self.rtcEngine startRemoteAuxiliaryStreamView:self.subStreamVideoSession.canvas streamType:self.streamType];
            if (!result) {
                [self.rtcEngine updateRemoteAuxiliaryStreamRenderMode:self.subStreamVideoSession.canvas.uid displayMode:HWRtcVideoDisplayModeFit mirrorMode:HWRtcVideoMirrorTypeDisable];
            }
            NSLog(@"辅流result = %d",result);
            
        } else {
            self.subStreamVideoSession.isPlaying = NO;
            [self.rtcEngine stopRemoteStreamView:self.subStreamVideoSession.userid];
//            [self.rtcEngine stopRemoteSubStreamView:self.subStreamVideoSession.userid];
            self.subStreamVideoSession.videoView.hidden = YES;
            NSLog(@"取消辅流");
        }
        self.rolesListVC.viewsArray = self.viewsArray;
        self.rolesListVC.showingCount = self.showViewsArray.count;
        self.rolesListVC.maxShowCount = self.maxShowCount;
        [self reloadViewWithDataArray:self.showViewsArray];
    });
}

@end
