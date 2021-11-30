//
//  HWRoomListSettingViewController.m
//  HwLiveDemo
//
//  Created by hw on 2020/7/17.
//  Copyright © 2020 HW. All rights reserved.
//

#import "HWRoomListSettingViewController.h"
#import "UITextField+Expand.h"
#import "HWRtcVideoEncode+isSelect.h"
@interface HWRoomListSettingViewController ()<HWRtcEngineDelegate>
@property (weak, nonatomic) IBOutlet UILabel *visionLabel;
@property (weak, nonatomic) IBOutlet UILabel *biuldLabel;
@property (weak, nonatomic) IBOutlet UITextField *nickNameTF;
@property (weak, nonatomic) IBOutlet NSLayoutConstraint *scrollViewTop;

@property (weak, nonatomic) IBOutlet UIButton *downStreamResource90Button;
@property (weak, nonatomic) IBOutlet UIButton *downStreamResource180Button;
@property (weak, nonatomic) IBOutlet UIButton *downStreamResource360Button;
@property (weak, nonatomic) IBOutlet UIButton *downStreamResource720Button;

@property (nonatomic, assign) BOOL isNeedInstrance;

@end
@implementation HWRoomListSettingViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.isHiddenNav = NO;
    NSString* title = @"设置";
    self.navTitle = title;
    self.scrollViewTop.constant = self.navHeight;
    self.nickNameTF.text =   [HWUserDefaultManager getUserName];
    __weak typeof(self) weakSelf = self;
    [self.nickNameTF lengthLimit:^{
        if (weakSelf.nickNameTF.text.length > 8) {
            weakSelf.nickNameTF.text = [weakSelf.nickNameTF.text substringToIndex:8];
        }
    }];
    [self setVersion];
    [self reloadDownStreamUI];
}

- (void)setVersion{
    NSString *visionStr = [HWRtcEngine getVersion];;
    self.visionLabel.text = [NSString stringWithFormat:@"版本号：%@",visionStr];
    NSString *buildDate = [NSString stringWithFormat:@"%s %s",__DATE__, __TIME__];
    self.biuldLabel.text = [NSString stringWithFormat:@"编译时间：%@",buildDate];
}

- (void)reloadDownStreamUI {
    switch ([HWUserDefaultManager getHWRtcStreamType]) {
        case HWRtcStreamTypeLD:
        {
            self.downStreamResource90Button.selected = YES;
            self.downStreamResource180Button.selected = NO;
            self.downStreamResource360Button.selected = NO;
            self.downStreamResource720Button.selected = NO;
        }
            break;
        case HWRtcStreamTypeSD:
        {
            self.downStreamResource90Button.selected = NO;
            self.downStreamResource180Button.selected = YES;
            self.downStreamResource360Button.selected = NO;
            self.downStreamResource720Button.selected = NO;
        }
            break;
        case HWRtcStreamTypeHD:
        {
            self.downStreamResource90Button.selected = NO;
            self.downStreamResource180Button.selected = NO;
            self.downStreamResource360Button.selected = YES;
            self.downStreamResource720Button.selected = NO;
        }
            break;
        case HWRtcStreamTypeFHD:
        {
            self.downStreamResource90Button.selected = NO;
            self.downStreamResource180Button.selected = NO;
            self.downStreamResource360Button.selected = NO;
            self.downStreamResource720Button.selected = YES;
        }
            break;
            
        default:
            break;
    }
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    if (_nickNameTF.text.length>0) {
        NSString *temp = [_nickNameTF.text stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
        NSString *text = [temp stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
        [HWUserDefaultManager setUserName:text];
    }
}

- (IBAction)touch:(id)sender {
    [self.view endEditing:YES];
}

- (IBAction)downStreamResource90Action:(UIButton *)sender {
    if (!sender.selected) {
        self.downStreamResource90Button.selected = YES;
        self.downStreamResource180Button.selected = NO;
        self.downStreamResource360Button.selected = NO;
        self.downStreamResource720Button.selected = NO;
        [HWUserDefaultManager setHWRtcStreamType:HWRtcStreamTypeLD];
    }
}

- (IBAction)downStreamResource180Action:(UIButton *)sender {
    if (!sender.selected) {
        self.downStreamResource90Button.selected = NO;
        self.downStreamResource180Button.selected = YES;
        self.downStreamResource360Button.selected = NO;
        self.downStreamResource720Button.selected = NO;
        [HWUserDefaultManager setHWRtcStreamType:HWRtcStreamTypeSD];
    }
}

- (IBAction)downStreamResource360Action:(UIButton *)sender {
    if (!sender.selected) {
        self.downStreamResource90Button.selected = NO;
        self.downStreamResource180Button.selected = NO;
        self.downStreamResource360Button.selected = YES;
        self.downStreamResource720Button.selected = NO;
        [HWUserDefaultManager setHWRtcStreamType:HWRtcStreamTypeHD];
    }
}

- (IBAction)downStreamResource720Action:(UIButton *)sender {
    if (!sender.selected) {
        self.downStreamResource90Button.selected = NO;
        self.downStreamResource180Button.selected = NO;
        self.downStreamResource360Button.selected = NO;
        self.downStreamResource720Button.selected = YES;
        [HWUserDefaultManager setHWRtcStreamType:HWRtcStreamTypeFHD];
    }
}

#pragma mark- UIScrollViewDelegate
- (void)scrollViewWillBeginDragging:(UIScrollView *)scrollView {
    [self.view endEditing:YES];
}

@end
