//
//  HWBaseViewController.m
//  HW
//
//  Created by HW on 2019/7/22.
//  Copyright © 2019年 HW. All rights reserved.
//

#import "HWBaseViewController.h"
//16进制颜色值
#define UIColorFromRGB(rgbValue) [UIColor colorWithRed:((float)((rgbValue & 0xFF0000) >> 16))/255.0 green:((float)((rgbValue & 0xFF00) >> 8))/255.0 blue:((float)(rgbValue & 0xFF))/255.0 alpha:1.0]

@interface HWBaseViewController ()<UINavigationControllerDelegate>
/// <#Description#>
@property (nonatomic, strong) UIButton *backButton;

/// <#Description#>
@property (nonatomic, strong) UILabel *labelTitle;
@end

@implementation HWBaseViewController

- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
//    //禁止掉侧滑手势
//    if([self.navigationController respondsToSelector:@selector(interactivePopGestureRecognizer)])
//    {
//        self.navigationController.interactivePopGestureRecognizer.enabled = NO;
//    }
}

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = UIColorBaseBackGroud;
    self.navigationController.delegate = self;
    self.navigationController.interactivePopGestureRecognizer.delegate = (id)self;
//    self.navigationController.interactivePopGestureRecognizer.enabled = YES;
    [self createNavUI];
    [self addNotificationObserver];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    [self.navigationController setNavigationBarHidden:YES animated:animated];
}

- (void)viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear:animated];
}


- (void)createNavUI {
    [self.view addSubview:self.weNavigationView];
    [self.weNavigationView addSubview:self.backButton];
    [self.weNavigationView addSubview:self.labelTitle];
}

- (void)addNotificationObserver {
    // app生命后台
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(observeApplicationDidEnterBackground:) name:UIApplicationWillResignActiveNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(observeApplicationWillEnterForeground:) name:UIApplicationDidBecomeActiveNotification object:nil];
}

- (void)observeApplicationWillEnterForeground:(NSNotification *)notification {
    
}

- (void)observeApplicationDidEnterBackground:(NSNotification *)notification {
    
}

- (void)navigationController:(UINavigationController *)navigationController
      willShowViewController:(UIViewController *)viewController
                    animated:(BOOL)animated {
    UIBarButtonItem *itemleft = [[UIBarButtonItem alloc] initWithImage:[UIImage imageNamed:@"icon_back_normal"] style:UIBarButtonItemStylePlain target:self action:@selector(popAction:)];
    itemleft.tintColor = [UIColor whiteColor];
    viewController.navigationItem.leftBarButtonItem = itemleft;
}

- (void)popAction:(UIBarButtonItem *)barButtonItem {
    [self.navigationController popViewControllerAnimated:YES];
}


- (void)popViewControl:(UIButton *)but {
    [self.navigationController popViewControllerAnimated:YES];
}

- (void)dealloc {
    NSLog(@"😆😆😆%@--->delloc 释放了",NSStringFromClass([self class]));
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

#pragma mark - 导航栏 UI
- (UIView *)weNavigationView {
    if (!_weNavigationView) {
        float height = 0;
        ///iOS 11以上 ：状态栏高度为20，增加了SafeArea安全区域来判断状态栏高度
        ///iOS 11以下 ：状态栏高度为20，一般判断时直接判断状态栏高度为20
        if (@available(iOS 11.0, *)) {
            height = 12;
        } else {
            height = 32;
        }
        _weNavigationView = [[UIView alloc] initWithFrame:CGRectMake(0, kGetSafeAreaTopHeight + height, SCREENWIDTH, 48)];
        _weNavigationView.backgroundColor = [UIColor clearColor];
        _weNavigationView.hidden = YES;
        
    }
    return _weNavigationView;
}

- (UIButton *)backButton {
    if (!_backButton) {
        _backButton = [UIButton buttonWithType:UIButtonTypeCustom];
        _backButton.frame = CGRectMake(10, 10, 24, 24);
        [_backButton setBackgroundImage:[UIImage imageNamed:@"nav_back"] forState:UIControlStateNormal];
        [_backButton addTarget:self action:@selector(popViewControl:) forControlEvents:UIControlEventTouchUpInside];
        _backButton.exclusiveTouch = YES;
    }
    return _backButton;
}

- (UILabel *)labelTitle {
    if (!_labelTitle) {
        _labelTitle = [[UILabel alloc] initWithFrame:CGRectMake(0, 13, SCREENWIDTH, 22)];
        _labelTitle.textAlignment = NSTextAlignmentCenter;
        _labelTitle.textColor = UIColorText;
        if (@available(iOS 8.2, *)) {
            _labelTitle.font = [UIFont systemFontOfSize:16 weight:UIFontWeightMedium];
        } else {
            _labelTitle.font = [UIFont systemFontOfSize:16];
        }
    }
    return _labelTitle;
}

- (CGFloat)navHeight {
    return (CGRectGetHeight(self.weNavigationView.frame) + CGRectGetMinY(self.weNavigationView.frame));
}

- (void)setNavTitle:(NSString *)navTitle {
    _navTitle = navTitle;
    self.labelTitle.text = navTitle;
}

- (void)setIsHiddenNav:(BOOL)isHiddenNav {
    _isHiddenNav = isHiddenNav;
    self.weNavigationView.hidden = isHiddenNav;
}

- (void)setIsHiddenBak:(BOOL)isHiddenBak {
    _isHiddenBak = isHiddenBak;
    self.backButton.hidden = isHiddenBak;
}

- (void)setBackImage:(UIImage *)backImage {
    _backImage = backImage;
    [self.backButton setBackgroundImage:backImage forState:UIControlStateNormal];
}


@end
