//
//  HWRoomListViewController.m
//  HwLiveDemo
//
//  Created by HW on 2020/7/13.
//  Copyright © 2020 HW. All rights reserved.
//

#import "HWRoomListViewController.h"
#import <HWRTCEngine/HWRtcEngine.h>
#import "HWLoginView.h"
#import "HWLiveRoomModel.h"
#import "HWRoomListSettingViewController.h"
#import "HWCameraViewController.h"
#import "HWSubStreamViewController.h"
#import "HWRoomListCreateRoomView.h"
#import "HWRoomListCollectionViewCell.h"
@interface HWRoomListViewController ()<HWRoomListCreateRoomViewDelegate,UICollectionViewDelegate,UICollectionViewDataSource,HWLoginViewDelegate>
@property (nonatomic, strong) HWRtcEngine *rtcEngine;
@property (nonatomic, strong) UICollectionView *myCollectionView;
@property (nonatomic, strong) HWRoomListCreateRoomView *createRoomView;
@property (nonatomic, strong) UIButton *settingButton;
@property (nonatomic, strong) UIButton *startLiveButton;
@property (nonatomic, strong) NSMutableArray *dataArray;
@property (nonatomic, assign) int page;
@end

@implementation HWRoomListViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.isHiddenNav = NO;
    self.isHiddenBak = YES;
    self.navTitle = @"视频互动";
    self.page = 0;
    if (![HWUserDefaultManager isTemLogin]) {
        [self pushLoginView];
    } else {
        [self createUI];
        [self getRoomListData];
    }
    [self instancetRtcEngine];
}

/// 初始化SDK
- (void)instancetRtcEngine {
    _rtcEngine = [HWRtcEngine sharedEngine];
//    _rtcEngine.delegate = self;
    if (![[NSFileManager defaultManager] fileExistsAtPath:HWLOGPATCH]) {
        [[NSFileManager defaultManager] createDirectoryAtPath:HWLOGPATCH withIntermediateDirectories:YES attributes:nil error:nil];
    }
    
    HWRtcEngineConfig * cfg = [[HWRtcEngineConfig alloc]init];
    cfg.appId = _APP_ID_;
    cfg.domain = _DOMAIN_;
    cfg.countryCode = @"CN";
    cfg.enableHaTrace = YES;
    cfg.logLevel = HWRtcLogLevelDebug;
    cfg.logPath = HWLOGPATCH; //设置日志路径
    cfg.enableLog = YES;
    cfg.logSize = 10*1024;
    cfg.muteAudioRoute = NO;
  int re =  [_rtcEngine initWithConfig:cfg];
    
    [_rtcEngine setEnableRtcStats:YES]; // 默认开启打点.关闭打点无法定位
//    [_rtcEngine setRemoteAudioTopNVoice:3]; // 设置远端音频最大的N个人的音频流
    [_rtcEngine enableHowlingDetect:YES]; // 开启啸叫检测
//    [self.rtcEngine setVideoEncParam:[self getVideoEncParam] totalBitRate:4096];//设置上行流编码能力

}

/// 四路流编码参数设置
- (NSArray <HWRtcVideoEncode *>*)getVideoEncParam {
    HWRtcVideoEncode *videoEncodeLD = [[HWRtcVideoEncode alloc] init];
    videoEncodeLD.streamFlag = HWRtcStreamTypeLD;
    videoEncodeLD.width = 160;
    videoEncodeLD.height = 90;
    videoEncodeLD.frameRate = 24;
    videoEncodeLD.bitrate = 270;
    videoEncodeLD.minBitrate = 64;
    videoEncodeLD.minFrameRate = 15;
    videoEncodeLD.disableAdjustRes = YES;
    HWRtcVideoEncode *videoEncodeSD = [[HWRtcVideoEncode alloc] init];
    videoEncodeSD.streamFlag = HWRtcStreamTypeSD;
    videoEncodeSD.width = 320;
    videoEncodeSD.height = 180;
    videoEncodeSD.frameRate = 24;
    videoEncodeSD.bitrate = 600;
    videoEncodeSD.minBitrate = 80;
    videoEncodeSD.minFrameRate = 15;
    videoEncodeSD.disableAdjustRes = YES;
    HWRtcVideoEncode *videoEncodeHD = [[HWRtcVideoEncode alloc] init];
    videoEncodeHD.streamFlag = HWRtcStreamTypeHD;
    videoEncodeHD.width = 640;
    videoEncodeHD.height = 360;
    videoEncodeHD.frameRate = 24;
    videoEncodeHD.bitrate = 1700;
    videoEncodeHD.minBitrate = 200;
    videoEncodeHD.minFrameRate = 24;
    videoEncodeHD.disableAdjustRes = YES;
    HWRtcVideoEncode *videoEncodeFHD = [[HWRtcVideoEncode alloc] init];
    videoEncodeFHD.streamFlag = HWRtcStreamTypeFHD;
    videoEncodeFHD.width = 1280;
    videoEncodeFHD.height = 720;
    videoEncodeFHD.frameRate = 24;
    videoEncodeFHD.bitrate = 4000;
    videoEncodeFHD.minBitrate = 500;
    videoEncodeFHD.minFrameRate = 24;
    videoEncodeFHD.disableAdjustRes = YES;
    return @[videoEncodeSD,videoEncodeLD,videoEncodeHD,videoEncodeFHD];
}

- (void)pushLoginView {
    HWLoginView *loginView = [HWLoginView initWithXib:CGRectMake(0, 0, SCREENWIDTH, SCREENHEIGHT)];
    loginView.delegate = self;
    [self.view addSubview:loginView];
}

//准备UI
- (void)createUI {
    [self.view addSubview:self.myCollectionView];
    [self.view addSubview:self.startLiveButton];
    [self.weNavigationView addSubview:self.settingButton];
}

//准备房间数据，该数值为固定值
- (void)getRoomListData {
    /// 创建8个房间.
    for (int i = 0; i < 8; i++) {
        HWLiveRoomModel *model = [[HWLiveRoomModel alloc] init];
        model.roomId = [NSString stringWithFormat:@"room%d",i];
        model.roomName = [NSString stringWithFormat:@"room%d",i];
        model.imageName = [NSString stringWithFormat:@"roomList_%d",i];
        [self.dataArray addObject:model];
    }
    [self.myCollectionView reloadData];
}

// 跳转设置页
- (void)openSettingViewControllerAction:(UIButton *)but {
    HWRoomListSettingViewController *roomListSvc = [[HWRoomListSettingViewController alloc] init];
    roomListSvc.rtcEngine = self.rtcEngine;
    [self.navigationController pushViewController:roomListSvc animated:YES];
}

/// 创建房间
- (void)startLiveAction:(UIButton *)but {
    self.createRoomView.frame = CGRectMake(0, SCREENHEIGHT, SCREENWIDTH, SCREENHEIGHT);
    [UIView animateWithDuration:0.25 animations:^{    
        self.createRoomView.frame = CGRectMake(0, 0, SCREENWIDTH, SCREENHEIGHT);
    }];
    return;
}

#pragma mark - HWLoginViewDelegate
- (void)LoginView:(HWLoginView *)LoginView userName:(nonnull NSString *)userName {
    [HWUserDefaultManager setUserName:userName];
    [UIView animateWithDuration:0.5 animations:^{
        LoginView.frame = CGRectMake(0, SCREENHEIGHT, SCREENWIDTH, SCREENHEIGHT);
    } completion:^(BOOL finished) {
        [LoginView removeFromSuperview];
    }];
    [self createUI];
    [self getRoomListData];
}

#pragma mark - UICollectionViewDelegate & dataSource
- (NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView {
    return 1;
}

- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section {
    return self.dataArray.count;
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath {
    HWRoomListCollectionViewCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:@"cell" forIndexPath:indexPath];
    HWLiveRoomModel *model = [self.dataArray objectAtIndex:indexPath.row];;
    [cell configLiveModel:model];
    return cell;
}

-  (void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath {
    HWLiveRoomModel *model = [self.dataArray objectAtIndex:indexPath.row];
    HWCameraViewController *cameraVC;
    if ([model.roomName containsString:@"room"]) {
        cameraVC = [[HWSubStreamViewController alloc] initWithNibName:@"HWSubStreamViewController" bundle:nil];
    } else {
        
        cameraVC = [[HWCameraViewController alloc] init];
    }
    cameraVC.rtcEngine = self.rtcEngine;
    cameraVC.roomID = model.roomId;
    cameraVC.roomName = model.roomName; //roomName
    cameraVC.roleType = HWRtcRolePlayer;
    [self.navigationController pushViewController:cameraVC animated:YES];
}

#pragma mark - HWRoomListCreateRoomViewDelegate
- (void)HWRoomListCreateRoomViewCreateRoomAction:(NSString *)roomName {
    HWCameraViewController *cameraVC = [[HWCameraViewController alloc] init];
    cameraVC.rtcEngine = self.rtcEngine;
    cameraVC.roomName = roomName;
    cameraVC.roomID = roomName;
    cameraVC.roleType = HWRtcRoleJoiner;
    [self.navigationController pushViewController:cameraVC animated:YES];
}

#pragma get && set
- (NSMutableArray *)dataArray {
    if (!_dataArray) {
        _dataArray = [NSMutableArray array];
    }
    return _dataArray;
}

- (UICollectionView *)myCollectionView {
    if (!_myCollectionView) {
        UICollectionViewFlowLayout *layout = [[UICollectionViewFlowLayout alloc] init];
        layout.scrollDirection = UICollectionViewScrollDirectionVertical;
        layout.minimumLineSpacing = 15.f;
        layout.minimumInteritemSpacing = 15.f;
        layout.sectionInset = UIEdgeInsetsMake(15.f, 15.f, 30.f, 15.f);
        CGFloat itemW = (SCREENWIDTH - 45.f) / 2 ;
        layout.itemSize = CGSizeMake(itemW, itemW);
        _myCollectionView = [[UICollectionView alloc] initWithFrame:CGRectMake(0, self.navHeight, SCREENWIDTH, SCREENHEIGHT - (114 + (IS_iPhone_X_SERIES?kGetSafeAreaTopHeight:0)) + 50) collectionViewLayout:layout];
        _myCollectionView.backgroundColor = [UIColor whiteColor];
        _myCollectionView.dataSource = self;
        _myCollectionView.delegate = self;
        [_myCollectionView registerClass:HWRoomListCollectionViewCell.class forCellWithReuseIdentifier:@"cell"];
    }
    return _myCollectionView;
}

- (UIButton *)settingButton {
    if (!_settingButton) {
        _settingButton = [UIButton buttonWithType:UIButtonTypeCustom];
        _settingButton.frame = CGRectMake((SCREENWIDTH - 70), 10, 30, 30);
        [_settingButton setImage:[UIImage imageNamed:@"btn_set"] forState:UIControlStateNormal];
        [_settingButton addTarget:self action:@selector(openSettingViewControllerAction:) forControlEvents:UIControlEventTouchDown];
    }
    return _settingButton;
}

- (UIButton *)startLiveButton {
    if (!_startLiveButton) {
        _startLiveButton = [UIButton buttonWithType:UIButtonTypeCustom];
        _startLiveButton.frame = CGRectMake((SCREENWIDTH - 240)/2, SCREENHEIGHT - 100, 240, 44);
        _startLiveButton.backgroundColor = [UIColor colorWithString:@"#32dbf0"];
        _startLiveButton.layer.cornerRadius = 22;
        _startLiveButton.layer.masksToBounds = YES;
        [_startLiveButton setTitle:@"创建房间" forState:UIControlStateNormal];
        [_startLiveButton addTarget:self action:@selector(startLiveAction:) forControlEvents:UIControlEventTouchDown];
        _startLiveButton.exclusiveTouch = YES;
        _startLiveButton.selected = NO;
    }
    return _startLiveButton;
}

- (HWRoomListCreateRoomView *)createRoomView {
    if (!_createRoomView) {
        _createRoomView = [HWRoomListCreateRoomView initWithXib:CGRectMake(0, 0, SCREENWIDTH, SCREENHEIGHT)];
        _createRoomView.delegate = self;
        [self.view addSubview:_createRoomView];
    }
    return _createRoomView;
}

@end
