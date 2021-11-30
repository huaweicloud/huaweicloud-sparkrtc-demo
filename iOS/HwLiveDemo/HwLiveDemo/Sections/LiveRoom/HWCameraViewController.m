//
//  HWCameraViewController.m
//  HWRtcDemo
//
//  Created by HW on 2020/2/17.
//  Copyright © 2020 @YanYuXin. All rights reserved.
//

#import "HWCameraViewController.h"
#import "HWRolesListViewController.h"
#import "HWVideoSession.h"
#import "HWReusePool.h"
#import "HWRealTimeInfoView.h"
#import "HWMonitor.h"
#import "HWRtcVideoStatsInfo+Type.h"
#import "HWRtcAudioStatsInfo+Type.h"
@interface HWCameraViewController ()<HWRtcEngineDelegate>
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

@property (nonatomic, strong) NSString *userID; /// 自己userID
@property (nonatomic, strong) NSString *userName; /// 自己用户名
@property (strong, nonatomic) HWVideoSession *localVideoSession;    /// 本地视图
@property (strong, nonatomic) HWVideoSession *subStreamVideoSession; /// 辅流视图
@property (nonatomic, strong) NSMutableArray *viewsArray;     /// 所有视图的数组
@property (nonatomic, strong) NSMutableArray *showViewsArray; /// 展示视图的数组,用于处理展示的用户管理

@property (nonatomic, strong) HWRolesListViewController *rolesListVC; /// 房间内用户界面
@property (nonatomic, strong) HWRealTimeInfoView *realTimeInfoView;   /// 实时视频信息试图
@property (nonatomic, assign) HWRtcStreamType streamType;             /// 远端视图清晰度

///重用池
@property (nonatomic, strong) HWReusePool *reusePool;            /// 重用池,用于展示视图对象的缓存使用
@property (nonatomic, assign) HWRtcVideoDisplayMode displayMode; /// 视图显示模式

@property (nonatomic, assign) int maxShowCount;  /// 视图最大显示个数. 房间模式下,最多显示4
@property (nonatomic, assign) BOOL isJoinroomSuccess;    /// 是否加入房间成功
@property (nonatomic, assign) BOOL isSubStreamAvailable; ///辅流是否可用
@property (nonatomic, assign) BOOL isFull;               /// 是否收起边框
@property (nonatomic, assign) BOOL isLocalAudioON;       ///本地音频采集是否打开
@property (nonatomic, assign) BOOL isLocalVideoON;       ///本地视频采集是否打开

@end
static NSString* localUid;

@implementation HWCameraViewController

- (HWReusePool *)reusePool {
    if (!_reusePool) {
        _reusePool = [[HWReusePool alloc] init];
    }
    return _reusePool;
}

- (NSMutableArray *)viewsArray {
    if (!_viewsArray) {
        _viewsArray = [NSMutableArray array];
    }
    return _viewsArray;
}

- (NSMutableArray *)showViewsArray {
    if (!_showViewsArray) {
        _showViewsArray = [NSMutableArray array];
    }
    return _showViewsArray;
}

- (HWRealTimeInfoView *)realTimeInfoView {
    if (!_realTimeInfoView) {
        _realTimeInfoView = [[HWRealTimeInfoView alloc] initWithFrame:CGRectMake(15, (SCREENHEIGHT-300)/2, SCREENWIDTH-30, 260)];
        _realTimeInfoView.localUserName = self.userName;
        _realTimeInfoView.center = CGPointMake(self.view.center.x, _realTimeInfoView.center.y);
        [self.monitorView addSubview:_realTimeInfoView];
        _realTimeInfoView.hidden = YES;
    }
    return _realTimeInfoView;
}

- (HWRolesListViewController *)rolesListVC {
    if (!_rolesListVC) {
        _rolesListVC = [[HWRolesListViewController alloc] initWithNibName:@"HWRolesListViewController" bundle:nil];
    }
    return _rolesListVC;
}

- (int)maxShowCount {
    return 4;
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    [self.navigationController setNavigationBarHidden:YES animated:YES];
    // 禁止侧滑
    id traget = self.navigationController.interactivePopGestureRecognizer.delegate;
    UIPanGestureRecognizer * pan = [[UIPanGestureRecognizer alloc] initWithTarget:traget action:nil];
    [self.view addGestureRecognizer:pan];
    NSLog(@"viewWillAppear");
}

- (void)viewDidLoad {
    [super viewDidLoad];
    [self configurationView];
    [self configurationData];
    [self configurationUIStatus];
    [self joinRoom];
    NSLog(@"viewDidLoad");
}

// 准备基本数据
- (void)configurationData{
    self.userName           = [HWUserDefaultManager getUserName];
    self.userID             = [HWTool getUUID];
    self.isFull             = NO;
    self.isJoinroomSuccess  = NO;
    self.isLocalAudioON     = YES;
    self.isLocalVideoON     = YES;
    self.rtcEngine.delegate = self;
    self.displayMode        = HWRtcVideoDisplayModeHidden;
    self.streamType         = [HWUserDefaultManager getHWRtcStreamType];
}

// 准备Ui
- (void)configurationView {
    self.tabView.layer.cornerRadius         = 24;
    self.tabView.layer.masksToBounds        = YES;
    self.localVideoButton.exclusiveTouch    = YES;
    self.microphoneButton.exclusiveTouch    = YES;
    self.microphoneButton.exclusiveTouch    = YES;
    self.localAudioButton.exclusiveTouch    = YES;
    self.roleListButton.exclusiveTouch      = YES;
    self.switchCameraButton.exclusiveTouch  = YES;
    if (self.roleType == HWRtcRoleJoiner) {
        self.changeRoleTypeButton.selected = NO;
    } else if (self.roleType == HWRtcRolePlayer)  {
        self.changeRoleTypeButton.selected = YES;
    } else {
        self.changeRoleTypeButton.selected = YES;
        self.changeRoleTypeButton.enabled = NO;
    }
    
    if (IS_iPhone_X_SERIES) {
        self.topInfoTopConstraint.constant = 110;
    }
    self.labelRoomID.text = [NSString stringWithFormat:@"%@",self.roomName];

}

// 根据状态配置UI
- (void)configurationUIStatus {
    if (self.roleType == HWRtcRolePlayer) {
        self.localVideoButton.enabled   = NO;
        self.microphoneButton.enabled   = NO;
        self.localAudioButton.enabled   = NO;
        self.switchCameraButton.enabled = NO;
        self.changeRoleTypeButton.selected = YES;
    } else {
        self.localVideoButton.enabled   = YES;
        self.microphoneButton.enabled   = YES;
        self.localAudioButton.enabled   = YES;
        self.switchCameraButton.enabled = YES;
        self.changeRoleTypeButton.selected = NO;
    }
}

/// 加入房间
- (void)joinRoom {
    if (self.isJoinroomSuccess) {
        return;
    }

    int r =  [self.rtcEngine setDefaultSpeakerModel:HWRtcSpeakerModelSpeaker];//建议默认入会使用扬声器模式

    HWRtcJoinParam *joinParam = [[HWRtcJoinParam alloc]init];
    joinParam.role  = self.roleType;
    joinParam.userId = self.userID;
    joinParam.userName =  self.userName;
    joinParam.scenario = HWRtcRemoteAudioTopThree;
    joinParam.sfuType = 0;
    joinParam.roomId = self.roomID;
    joinParam.autoSubscribeAudio = NO;
    joinParam.autoSubscribeVideo = NO;
    joinParam.appId = _APP_ID_;
    joinParam.optionInfo = @"";
    int result = [self.rtcEngine joinRoom:joinParam];
    
    if (result != 0) {
        [HWTool showMessage:@"加入房间失败,请重新输入房间号"];
        [self closeRoom];
        return;
    }
    if (self.roleType != HWRtcRolePlayer) { //joiner角色的状态上,设置本地试图
        _localVideoSession = [[HWVideoSession alloc] initWithUserId:self.userID];
        HwRtcVideoView *videoView = [[HwRtcVideoView alloc] initWithFrame:self.view.bounds];
        _localVideoSession.videoView = videoView;
        _localVideoSession.userName  = self.userName;
        _localVideoSession.isPlaying = NO;//本地视图在选看列表无法选看
        [_rtcEngine setupLocalVideo:_localVideoSession.canvas
                           viewMode:self.displayMode];  /// 设置本地窗口传递给SDK
        [self.cameraView addSubview:videoView];
        [self.showViewsArray addObject:_localVideoSession];
        [self.viewsArray addObject:_localVideoSession];
        [self reloadViewWithDataArray:self.showViewsArray];
    }
    NSLog(@"joinRoom");
    int result2 = [_rtcEngine enableLocalAudioStream:YES];

}

/// 更新用户画面视图
- (void)reloadViewWithDataArray:(NSMutableArray *)dataArray {
    CGFloat offset = 0;
    CGFloat width = 0;
    CGFloat height = 0;
    width = SCREENWIDTH;
    height = SCREENHEIGHT;
    if (dataArray.count==1) {
        [UIView animateWithDuration:0.25 animations:^{
            HWVideoSession *videoSession = [dataArray objectAtIndex:0];
            videoSession.videoView.frame = CGRectMake(0, offset, width, height);
            [videoSession.videoView setUserName:videoSession.userName];
        }];
    } else if(dataArray.count==2) {
        [UIView animateWithDuration:0.25 animations:^{
            HWVideoSession *videoSession = [dataArray objectAtIndex:0];
            videoSession.videoView.frame = CGRectMake(0, offset, width, height/2);
            [videoSession.videoView setUserName:videoSession.userName];
            
            HWVideoSession *videoSession1 = [dataArray objectAtIndex:1];
            videoSession1.videoView.frame = CGRectMake(0, offset+height/2, width, height/2);
            [videoSession1.videoView setUserName:videoSession1.userName];
        }];
    } else if(dataArray.count==3) {
        [UIView animateWithDuration:0.25 animations:^{
            HWVideoSession *videoSession = [dataArray objectAtIndex:0];
            videoSession.videoView.frame = CGRectMake(0, offset, width, height/2);
            [videoSession.videoView setUserName:videoSession.userName];
            
            HWVideoSession *videoSession1 = [dataArray objectAtIndex:1];
            videoSession1.videoView.frame = CGRectMake(0, offset+height/2,  width/2, height/2);
            [videoSession1.videoView setUserName:videoSession1.userName];
            
            HWVideoSession *videoSession2 = [dataArray objectAtIndex:2];
            videoSession2.videoView.frame = CGRectMake(width/2, offset+height/2, width/2, height/2);
            [videoSession2.videoView setUserName:videoSession2.userName];
        }];
    } else if(dataArray.count==4) {
        [UIView animateWithDuration:0.25 animations:^{
            HWVideoSession *videoSession = [dataArray objectAtIndex:0];
            videoSession.videoView.frame = CGRectMake(0, offset, width/2, height/2);
            [videoSession.videoView setUserName:videoSession.userName];
            
            HWVideoSession *videoSession1 = [dataArray objectAtIndex:1];
            videoSession1.videoView.frame = CGRectMake(width/2, offset,  width/2, height/2);
            [videoSession1.videoView setUserName:videoSession1.userName];
            
            HWVideoSession *videoSession2 = [dataArray objectAtIndex:2];
            videoSession2.videoView.frame = CGRectMake(0, offset+height/2, width/2, height/2);
            [videoSession2.videoView setUserName:videoSession2.userName];
            
            HWVideoSession *videoSession3 = [dataArray objectAtIndex:3];
            videoSession3.videoView.frame = CGRectMake(width/2, offset+height/2, width/2, height/2);
            [videoSession3.videoView setUserName:videoSession3.userName];
        }];
        for (HWVideoSession *videoSession in dataArray) {
            videoSession.videoView.isSubStreamAvailable = NO;
        }
    }
    NSLog(@"|||更新视图的数组内容:%@|||",dataArray);
}

/// 更新用户userName位置
- (void)reloadVideoViewUserName {
    if (self.showViewsArray.count == 0 || _isSubStreamAvailable) {
        return;
    }
    if (self.showViewsArray.count < 4) {
        HWVideoSession *videoSession = [self.showViewsArray objectAtIndex:0];
        videoSession.videoView.isFull = self.isFull;
    } else {
        HWVideoSession *videoSession = [self.showViewsArray objectAtIndex:0];
        videoSession.videoView.isFull = self.isFull;
        HWVideoSession *videoSession1 = [self.showViewsArray objectAtIndex:1];
        videoSession1.videoView.isFull = self.isFull;
    }
}

- (void)observeApplicationWillEnterForeground:(NSNotification *)notification {
    if ([self isKindOfClass:[HWCameraViewController class]]) {
        if (_isLocalVideoON && self.localVideoButton.enabled) { /// 根据状态返回前台,开启摄像头
            [self.rtcEngine enableLocalVideo:YES];
            NSLog(@"cameraVC,observeApplicationWillEnterForeground-localVideo:on");
        }
        if (_isLocalAudioON && self.localAudioButton.enabled) { /// 根据状态返回前台,开启麦克风
            [self.rtcEngine enableLocalAudioStream:YES];
            NSLog(@"cameraVC,observeApplicationWillEnterForeground-localAudio:on");
        }
    }
}

- (void)observeApplicationDidEnterBackground:(NSNotification *)notification {
    if ([self isKindOfClass:[HWCameraViewController class]]) {
        if (_isLocalVideoON && self.localVideoButton.enabled ) { ///  正常情况下返回后台,建议关闭摄像头
            [self.rtcEngine enableLocalVideo:NO];
            NSLog(@"cameraVC,observeApplicationDidEnterBackground-localVideo:off");
        }
        if (_isLocalAudioON && self.localAudioButton.enabled ) { ///  正常情况下返回后台,建议关闭麦克风
            [self.rtcEngine enableLocalAudioStream:NO];
            NSLog(@"cameraVC,observeApplicationDidEnterBackground-localAudio:off");
        }
    }
}

- (NSString *)getUserName:(NSString *)uuid {
    NSString *userName = @"";
    for (HWVideoSession *videoSession in self.viewsArray) {
        if ([videoSession.userid isEqualToString:uuid]) {
            userName = videoSession.userName;
            break;
        }
    }
    return userName;
}

/// 点击界面 收起上下两块视图
- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    _isFull = !_isFull;
    if (_isFull) {
        [UIView animateWithDuration:0.25 animations:^{
            self.topInfoTopConstraint.constant = 0;
            self.tabViewBottonConstraint.constant = - 110;
            self.changeRoleTypeButton.hidden = YES;
        }];
    } else {
        [UIView animateWithDuration:0.25 animations:^{
            self.topInfoTopConstraint.constant = 110;
            self.tabViewBottonConstraint.constant = 20;
            self.changeRoleTypeButton.hidden = NO;
        }];
    }
    [self reloadVideoViewUserName];
    NSLog(@"===>isFull = %d,%f",_isFull,self.tabViewBottonConstraint.constant);
}

/// 离开房间
- (void)closeRoom {
    [self.navigationController popViewControllerAnimated:YES];
    NSLog(@"=== closeRoom");
}

#pragma mark - HwRtcEngineDelegate
/// 加入房间成功
- (void)onJoinSuccess:(NSString*)roomId
               userId:(NSString*)userId{
    localUid = userId;
    dispatch_async(dispatch_get_main_queue(),
    ^{
        self.isJoinroomSuccess = YES;
        //保持常亮
        [UIApplication sharedApplication].idleTimerDisabled = YES;
    });
    NSLog(@"onJoinSuccess %@",userId);
}

/// 重新加入房间成功
- (void)onRejoinRoomSuccess:(NSString *)roomid
                     userid:(NSString *)userid {
    dispatch_async(dispatch_get_main_queue(), ^{
        [HWTool showMessage:@"重新加入房间成功"];
    });
}

/// 离开房间
- (void)onLeaveRoom:(HWRtcLeaveReason)leaveReason
          statsInfo:(HWRtcStatsInfo *)statsInfo{
    if (self.isJoinroomSuccess) {
        self.isJoinroomSuccess = NO;
        [self performSelectorOnMainThread:@selector(closeRoom) withObject:nil waitUntilDone:NO];
        self.rtcEngine.delegate = nil;
    }
}

/// 用户加入房间
-(void)onRemoteUserOnline:(NSString*)roomId userId:(NSString *)userId userName:(NSString *)userName {
    
    dispatch_async(dispatch_get_main_queue(),
                   ^{
        /// 自己的uid无需处理
        if([userId isEqualToString:localUid] || self.roleType == HWRtcRolePlayer){
            return;
        }
        HWVideoSession *videoSession = [[HWVideoSession alloc] initWithUserId:userId];
        videoSession.userName  = userName;
        videoSession.userName  = userName;
        NSLog(@"加入房间 roomId = %@,userId = %@, userName = %@",roomId,userId,userName);
        if (self.showViewsArray.count < self.maxShowCount) { /// 只有展示视图的个数在4之内,用户进来才会处理
            HwRtcVideoView *videoView = [self.reusePool dequeueReusableView];
            if (!videoView) {
                videoView = [[HwRtcVideoView alloc] initWithFrame:CGRectMake(0, 0, 90, 160)];
                [self.reusePool addVideoView:videoView];
            }
            [videoView setUserName:userName];
            videoSession.videoView = videoView;
            [videoSession.videoView setUserName:userName];
            videoSession.isPlaying = YES;
            [self.cameraView addSubview:videoView];
            int result = [self.rtcEngine startRemoteStreamView:videoSession.canvas
                                                    streamType:self.streamType
                                              disableAdjustRes:NO]; // 设置远端视图
            if (result == 0) {
                [self.rtcEngine updateRemoteRenderMode:userId displayMode:self.displayMode mirrorMode:HWRtcVideoMirrorTypeAuto];
            }
            NSLog(@"|||加入房间  %@,窗口对象==%@,result = %d,videoSession =%@,窗口hidden=%d,窗口父窗口==%@,%@|||",
                  (videoSession.isPlaying?@"设置窗口":@"取消窗口"),
                  videoSession.canvas.view,
                  result,
                  videoSession,
                  videoSession.canvas.view.hidden,
                  videoSession.canvas.view.superview,
                  videoSession.canvas.view.class);
            [self.showViewsArray addObject:videoSession];
            [self reloadViewWithDataArray:self.showViewsArray];
            
        }
        [self.viewsArray addObject:videoSession];
        if(self.rolesListVC!=nil){ ///  更新用户列表视图数据
            self.rolesListVC.showingCount = self.showViewsArray.count;
            self.rolesListVC.viewsArray = self.viewsArray;
        }
    });
}

/// 用户离会 视图不需要多余设置,SDK自动断开和视图链接
- (void)onRemoteUserOffline:(NSString *)roomId userId:(NSString *)userId reason:(NSInteger)reason
 {
    if(self.roleType == HWRtcRolePlayer){
        return;
    }
    dispatch_async(dispatch_get_main_queue(),
    ^{
        NSLog(@"离开房间:rooid=%@,userid=%@,reason=%ld\n,viewsArray=%@",roomId,userId,(long)reason,self.viewsArray);
        HWVideoSession *offlineVideoSession;
        for (HWVideoSession *videoSession in self.viewsArray) {
            if ([userId isEqualToString:videoSession.userid] && !videoSession.isSubStream) {
                offlineVideoSession = videoSession;
                break;
            }
        }
        if (offlineVideoSession) {
            [self.viewsArray removeObject:offlineVideoSession];
            [self.reusePool removeVideoView:offlineVideoSession.videoView];
            BOOL isLineShowView = NO;
            if (self.showViewsArray.count > 0 && self.maxShowCount>0) {
                for (HWVideoSession *videoSession in self.showViewsArray) {
                    if ([videoSession.userid isEqualToString:userId] && !videoSession.isSubStream) {
                        isLineShowView = YES;
                        break;
                    }
                }
            }
            if (isLineShowView) {
                [self.showViewsArray removeObject:offlineVideoSession];
                [self reloadViewWithDataArray:self.showViewsArray];
            }
            
        }
        if(self.rolesListVC!=nil){
            self.rolesListVC.showingCount = self.showViewsArray.count;
            self.rolesListVC.viewsArray = self.viewsArray;
        }
    });
}

/// 角色切换回调
/// demo只在Joiner 和 player 之间的切换.
-(void)onUserRoleChangedNotify:(HWRtcRole)oldRole newRole:(HWRtcRole)newRole {
    dispatch_async(dispatch_get_main_queue(), ^{
        NSLog(@"|||切换角色");
        if (self.roleType == HWRtcRolePlayer) {
            [self.showViewsArray removeObject:self.localVideoSession];
            [self.viewsArray removeObject:self.localVideoSession];
            self.localVideoSession.videoView.hidden = YES;
            self.localVideoSession.videoView = nil;
            self.localVideoSession.canvas.view = nil;
            [self.rtcEngine setupLocalVideo:self.localVideoSession.canvas viewMode:self.displayMode]; /// 用户主动取消选看,需要设置空视图给SDK
            NSLog(@"|||切换角色为player");
        } else  {
            ///如果选看四个了.就抛弃最后一个选看,把本地的显示出来
            if (self.showViewsArray.count == self.maxShowCount) {
                HWVideoSession *lastVideoSession = [self.showViewsArray lastObject];
                [self.reusePool removeVideoView:lastVideoSession.videoView];
                lastVideoSession.videoView = nil;
                lastVideoSession.isPlaying = NO;
                [self.rtcEngine stopRemoteStreamView:lastVideoSession.userid]; /// 用户主动取消选看,需要设置空视图给SDK
                [self.showViewsArray removeObject:lastVideoSession];
                self.rolesListVC.viewsArray = self.viewsArray; ///刷新视图
            }
            
            if (!self.localVideoSession) {
                self.localVideoSession = [[HWVideoSession alloc] initWithUserId:self.userID];
                self.localVideoSession.userName  = self.userName;
                self.localVideoSession.userName = self.userName;
            }
            if (!self.localVideoSession.videoView) {
                self.localVideoSession.videoView = [[HwRtcVideoView alloc] initWithFrame:self.view.bounds];
                [self.reusePool addVideoView:self.localVideoSession.videoView];
                [self.cameraView addSubview:self.localVideoSession.videoView];
            }
            self.localVideoSession.videoView.hidden = NO;
            self.localVideoSession.isPlaying = YES;
            int result = [self.rtcEngine setupLocalVideo:self.localVideoSession.canvas
                                                 viewMode:self.displayMode]; /// 重新选看本地视图
            [self.showViewsArray insertObject:self.localVideoSession atIndex:0];
            [self.viewsArray insertObject:self.localVideoSession atIndex:0];
            NSLog(@"|||切换角色wei joiner,SDK对象==%@,窗口对象==%@,result = %d,username =%@,窗口hidden=%d,窗口父窗口==%@,%@|||",
                  self.rtcEngine,
                  self.localVideoSession.canvas.view,
                  result,
                  self.localVideoSession.userName,
                  self.localVideoSession.canvas.view.hidden,
                  self.localVideoSession.canvas.view.superview,
                  self.localVideoSession.canvas.view.class);
        }
        [self reloadViewWithDataArray:self.showViewsArray];
        [self configurationUIStatus];
        self.changeRoleTypeButton.enabled = YES;
    });
}

/// 辅流回调
- (void)onUserAuxiliaryStreamAvailable:(NSString *)roomId userId:(NSString *)userId isAvailable:(BOOL)isAvailable {
    dispatch_async(dispatch_get_main_queue(), ^{
        ///切换角色按钮
        self.changeRoleTypeButton.enabled = !isAvailable;
        self.roleListButton.enabled = !isAvailable;
        self.isSubStreamAvailable = isAvailable;
        self.realTimeInfoView.isSubStreamAvailable = isAvailable;
        if (self.roleType == HWRtcRolePlayer) {
            self.switchCameraButton.enabled = NO;
            self.microphoneButton.enabled = NO;
            self.localVideoButton.enabled = NO;
        } else {
            self.switchCameraButton.enabled = !isAvailable;
            self.microphoneButton.enabled = !isAvailable;
            self.localVideoButton.enabled = !isAvailable;
        }
        if (isAvailable) {
           /// 如果辅流进来 只保持一个辅流全屏。
            self.maxShowCount = 0;
            self.localVideoSession.videoView.hidden = YES; /// 隐藏本地窗口
            [self.rtcEngine setupLocalVideo:self.localVideoSession.canvas
                                   viewMode:self.displayMode];
            [self.showViewsArray removeObject:self.localVideoSession];
            for (HWVideoSession *videoSession in self.showViewsArray) {
                videoSession.isPlaying = NO;
                [self.reusePool removeVideoView:videoSession.videoView];
                videoSession.videoView = nil;
                [self.rtcEngine stopRemoteStreamView:videoSession.userid]; /// 用户主动取消选看,需要设置空视图给SDK
            }
            [self.showViewsArray removeAllObjects];
            
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
            }
            [self.subStreamVideoSession.videoView setUserName:self.subStreamVideoSession.userName];
            self.subStreamVideoSession.videoView.hidden = NO;
            
            int result = [self.rtcEngine startRemoteAuxiliaryStreamView:self.subStreamVideoSession.canvas streamType:self.streamType];

            if (!result) {
                [self.rtcEngine updateRemoteAuxiliaryStreamRenderMode:self.subStreamVideoSession.canvas.uid displayMode:HWRtcVideoDisplayModeFit mirrorMode:HWRtcVideoMirrorTypeDisable];
            }
            [self.showViewsArray addObject:self.subStreamVideoSession];
            NSLog(@"辅流result = %d",result);
            
        } else {
            self.subStreamVideoSession.isPlaying = NO;
            self.subStreamVideoSession.videoView.hidden = YES;
            [self.showViewsArray removeAllObjects];
            NSLog(@"取消辅流");
            self.maxShowCount = 4;
            
            if (self.roleType != HWRtcRolePlayer) {
                /// 重新选看回去
                /// 本地
                if (!self.localVideoSession.videoView) {
                    self.localVideoSession.videoView = [[HwRtcVideoView alloc] initWithFrame:CGRectMake(0, 0, 90, 160)];
                    [self.cameraView addSubview:self.localVideoSession.videoView];
                }
                self.localVideoSession.videoView.hidden = NO;
                [self.rtcEngine setupLocalVideo:self.localVideoSession.canvas viewMode:self.displayMode];
                [self.showViewsArray addObject:self.localVideoSession];
            }
            for (HWVideoSession *videoSession in self.viewsArray) {
                if (self.showViewsArray.count >= self.maxShowCount) {
                    break;
                }
                if ([videoSession.userid isEqualToString:self.userID]) {
                    continue;
                }
                videoSession.videoView = [self.reusePool dequeueReusableView];
                if (!videoSession.videoView) {
                    videoSession.videoView = [[HwRtcVideoView alloc] initWithFrame:CGRectMake(0, 0, 90, 160)];
                    [self.reusePool addVideoView:videoSession.videoView];
                    [self.cameraView addSubview:videoSession.videoView];
                }
                int result = [self.rtcEngine startRemoteStreamView:videoSession.canvas
                                                        streamType:self.streamType
                                                  disableAdjustRes:NO];
                if (result == 0) {
                    [self.rtcEngine updateRemoteRenderMode:userId displayMode:self.displayMode mirrorMode:HWRtcVideoMirrorTypeAuto];

                }
                videoSession.isPlaying = YES;
                [videoSession.videoView setUserName:videoSession.userName];
                [self.showViewsArray addObject:videoSession];
            }
        }
        self.rolesListVC.viewsArray = self.viewsArray;
        self.rolesListVC.showingCount = self.showViewsArray.count;
        self.rolesListVC.maxShowCount = self.maxShowCount;
        [self reloadViewWithDataArray:self.showViewsArray];
    });
}

/// 引擎状态
- (void)onConnectionChangedNotify:(HWRtcConnStateType)state
                           reason:(HWRtcConnChangeReason)reason
                      description:(NSString *)description {
    dispatch_async(dispatch_get_main_queue(), ^{
        NSLog(@"onConnStateChange description = %@",description);
        if (state == HWRtcConnStateTypeConnected || state ==  HWRtcConnStateTypeConnecting){
            return ;
        }
        [HWTool showMessage:description];
    });
}

/// 加入房间失败
- (void)onJoinRoomFailure:(int)errorCode
                 errorMsg:(NSString*)errorMsg {
    dispatch_async(dispatch_get_main_queue(), ^{
        NSString* message = @"加入房间失败";
        self.isJoinroomSuccess = NO;
        [HWTool showMessage:message];
        [self closeRoom];
        //关闭常亮
        [UIApplication sharedApplication].idleTimerDisabled = YES;
        NSLog(@"onJoinRoomFailure%@",[NSString stringWithFormat:@"errorCode = %d \nerrorMsg = %@",errorCode,errorMsg]);
    });
}

/// 引擎error
- (void)onError:(int)errorCode  errorMsg:(NSString*)errorMsg {
    if (self.roleType == HWRtcRolePlayer) {
        return;
    }
    dispatch_async(dispatch_get_main_queue(), ^{
        //切换失败
        if (errorCode == HWRtcErrorCodeUserRoleChangeFail) {
            if (self.roleType == HWRtcRoleJoiner) {
                self.roleType =  HWRtcRolePlayer;
            } else
            {
                self.roleType = HWRtcRoleJoiner;
            }
            self.changeRoleTypeButton.selected = !self.changeRoleTypeButton.selected;
            self.changeRoleTypeButton.enabled = YES;
            [HWTool showMessage:errorMsg];
            NSLog(@"切换角色失败");
        }
        NSLog(@"%@",[NSString stringWithFormat:@"errorCode = %d \nerrorMsg = %@",errorCode,errorMsg]);
    });
}

/// 日志上传状态
- (void)onLogUploadResult:(int)result{}

/// 日志上传进度
- (void)onLogUploadProgress:(int)progress{}

// 视频信息
- (void)onVideoStatsNotify:(NSArray<HWRtcVideoStatsInfo *> *)videoStatsArray remoteVideoInfo:(NSArray<HWRtcVideoStatsInfo *> *)remoteVideoStatsInfos {
    dispatch_async(dispatch_get_main_queue(), ^{
        NSMutableArray *array = [NSMutableArray array];
        for (HWRtcVideoStatsInfo *videoStatsInfo in videoStatsArray) {
            videoStatsInfo.userId = self.userName;
            videoStatsInfo.type = @"视频发送";
            [array addObject:videoStatsInfo];
        }
        NSMutableArray *dataArray = [NSMutableArray array];
        for (HWRtcVideoStatsInfo *videoStatsInfo in remoteVideoStatsInfos) {
            videoStatsInfo.type = @"视频接收";
            if (videoStatsInfo.userId && videoStatsInfo.userId.length > 0) {
                videoStatsInfo.userId = [self getUserName:videoStatsInfo.userId];
                [dataArray addObject:videoStatsInfo];
            }
        }
        [array addObjectsFromArray:dataArray];
        [self.realTimeInfoView addVideoArray:array];
        [self.realTimeInfoView reloadView];
        NSLog(@"<><><>remoteVideoStatsInfos.count = %ld",remoteVideoStatsInfos.count);
        NSLog(@"回调视频信息==onLocalVideoInfo");
    });
}

// 辅流视频信息
- (void)onAuxiliaryStreamStatsNotify:(NSArray<HWRtcVideoStatsInfo *> *)subStreamStatsArray
                     remoteVideoInfo:(NSArray<HWRtcVideoStatsInfo *> *)remoteVideoStatsInfos {
    dispatch_async(dispatch_get_main_queue(), ^{
        if (subStreamStatsArray.count > 0) {
            HWRtcVideoStatsInfo *videoStatsInfo = [subStreamStatsArray firstObject];
            videoStatsInfo.type = @"屏幕共享";
            [self.realTimeInfoView addSubStreamVideo:videoStatsInfo];
        } else if (remoteVideoStatsInfos.count > 0) {
            HWRtcVideoStatsInfo *videoStatsInfo = [remoteVideoStatsInfos firstObject];
            videoStatsInfo.type = @"屏幕共享";
            [self.realTimeInfoView addSubStreamVideo:[remoteVideoStatsInfos firstObject]];
        }
        [self.realTimeInfoView reloadView];
        NSLog(@"回调视频信息==onSubStreamInfoWithLocalVideoInfo");
    });
}

// 音频信息
- (void)onAudioStatsNotify:(NSArray<HWRtcAudioStatsInfo *> *)audioStatsArray remoteAudioInfo:(NSArray<HWRtcAudioStatsInfo *> *)remoteAudioStatsInfos {
    dispatch_async(dispatch_get_main_queue(), ^{
        NSMutableArray *array = [NSMutableArray array];
        for (HWRtcAudioStatsInfo *audioStatsInfo in audioStatsArray) {
            audioStatsInfo.userId = self.userName;
            audioStatsInfo.type = @"音频发送";
            [array addObject:audioStatsInfo];
        }
        for (HWRtcAudioStatsInfo *audioStatsInfo in remoteAudioStatsInfos) {
            audioStatsInfo.userId =@"remote";
            audioStatsInfo.type = @"音频接收";
        }
        [array addObjectsFromArray:remoteAudioStatsInfos];
        [self.realTimeInfoView addAudioArray:array];
        [self.realTimeInfoView reloadView];
        NSLog(@"回调音频信息==onLocalAudioInfo");
    });
}

/// 音频路由切换回调
- (void)onAudioRouteChange:(HWRtcAudioRoute)audioRoute {
    dispatch_async(dispatch_get_main_queue(), ^{
        switch (audioRoute) {
            case HWRtcAudioRouteSpeaker:
                self.localAudioButton.selected = NO;
                break;
            case HWRtcAudioRouteReceiver:
                self.localAudioButton.selected = YES;
                break;
            default:
                self.localAudioButton.selected = YES;
                break;
        }
        NSLog(@"onAudioRouteChange audioRoute = %ld",audioRoute);
    });
}

// 签名过期
- (void)onAuthorizationExpired {
    NSLog(@"onSignatureExpired");
}

#pragma mark - XIB
// 切换角色
- (IBAction)switchRole:(UIButton *)sender {
    sender.selected  = !sender.selected;

    if (self.roleType == HWRtcRoleJoiner) {
        self.roleType =  HWRtcRolePlayer;
    } else {
        self.roleType = HWRtcRoleJoiner;
    }
    [self.rtcEngine changeUserRole:self.roleType signature:NULL ctime:0];
    sender.enabled = NO;
    NSLog(@"switchRole");
}

// 切换前后置摄像头
- (IBAction)switchCamerAction:(UIButton *)sender {
    [NSObject cancelPreviousPerformRequestsWithTarget:self selector:@selector(buttonClickedAction:) object:sender];
    [self performSelector:@selector(buttonClickedAction:) withObject:sender afterDelay:0.5]; ///防止快速点击
}

- (void)buttonClickedAction:(UIButton *)button {
    button.selected = !button.selected;
    [self.rtcEngine switchCamera];
    NSLog(@"switchCamera");
}

// 摄像头
- (IBAction)recordVideo:(UIButton *)sender {
    sender.selected = !sender.selected;
    _isLocalVideoON = !sender.selected;
    [_rtcEngine enableLocalVideo:!sender.selected];
    NSLog(@"recordVideo");
}

// 麦克风
- (IBAction)recordVoicer:(UIButton *)sender {
    sender.selected = !sender.selected;
    _isLocalAudioON = !sender.selected;
    [_rtcEngine enableLocalAudioStream:!sender.selected];
    NSLog(@"recordVoicer");
}

// 关闭房间
- (IBAction)leaveRoom:(UIButton *)sender {
    sender.selected = !sender.selected;
    if (self.isJoinroomSuccess) {
        [_rtcEngine leaveRoom];
    } else { /// 加入失败直接退出房间
        [_rtcEngine leaveRoom];
        [self closeRoom];
    }
    NSLog(@"leaveRoom,%@",self.navigationController.viewControllers);
}

// 扬声器
- (IBAction)setSpeakerModel:(UIButton *)sender {
    if (IS_iPad) {
        [HWTool showMessage:@"iPad无听筒模式"];
        return;
    }
    NSLog(@"%d",sender.selected);
    int result = [_rtcEngine setSpeakerModel:sender.selected?HWRtcSpeakerModelSpeaker:HWRtcSpeakerModelEarpiece];
    NSLog(@"setSpeakerModel %d",result);
}

// 用户列表
- (IBAction)openRolesListAction:(UIButton *)sender {
    self.rolesListVC.viewsArray = self.viewsArray;
    self.rolesListVC.rtcEngine  = self.rtcEngine;
    self.rolesListVC.reusePool = self.reusePool;
    self.rolesListVC.maxShowCount = self.maxShowCount;
    self.rolesListVC.showingCount = self.showViewsArray.count;
    self.rolesListVC.streamType = self.streamType;

    __weak typeof(self) weakSelf = self;
    self.rolesListVC.listViewStatusBlock = ^(HWVideoSession * _Nullable videoSession) { /// 用户状态发生改变
        __strong typeof(weakSelf) strongSelf = weakSelf;
        if (videoSession.isPlaying) {
            [strongSelf.showViewsArray addObject:videoSession];
        } else {
            [strongSelf.showViewsArray removeObject:videoSession];
        }
        [strongSelf reloadViewWithDataArray:strongSelf.showViewsArray];
    };
    [self.navigationController pushViewController:self.rolesListVC animated:YES];
    NSLog(@"openRolesListAction");
}

// 视频信息
- (IBAction)showInfo:(UIButton *)sender {
    sender.selected = !sender.selected;
    self.realTimeInfoView.hidden = !self.realTimeInfoView.hidden;
    if (!self.realTimeInfoView.hidden) {    
        [self.realTimeInfoView reloadView];
    }
    NSLog(@"showInfo");
}

@end
