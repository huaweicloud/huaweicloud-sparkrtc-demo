//
//  UIViewController+Orientation.m
//  HwLiveDemo
//
//  Created by HW on 2020/7/26.
//  Copyright © 2020 HW. All rights reserved.
//

#import "UIViewController+Orientation.h"

#import <objc/runtime.h>
#import "AppDelegate.h"

#define HWAppDelegate ((AppDelegate *)[UIApplication sharedApplication].delegate)

//定义常量 必须是C语言字符串
static char *FullScreenAllowRotationKey = "FullScreenAllowRotationKey";

@implementation UIViewController (Orientation)
void swizzleMethod(Class class, SEL originalSelector, SEL swizzledSelector){
    //该方法可能不存在于类中，但存在于其父类中
    Method originalMethod = class_getInstanceMethod(class, originalSelector);
    Method swizzledMethod = class_getInstanceMethod(class, swizzledSelector);

    //如果原始方法已经存在，class_addMethod将失败
    BOOL didAddMethod = class_addMethod(class, originalSelector, method_getImplementation(swizzledMethod), method_getTypeEncoding(swizzledMethod));

    //该方法不存在，我们只添加了一个
    if (didAddMethod) {
        class_replaceMethod(class, swizzledSelector, method_getImplementation(originalMethod), method_getTypeEncoding(originalMethod));
    } else {
        method_exchangeImplementations(originalMethod, swizzledMethod);
    }
}

/**
是否允许横屏

@return bool YES 允许 NO 不允许
*/
- (BOOL)shouldAutorotate1 {
    BOOL flag = objc_getAssociatedObject(self, FullScreenAllowRotationKey);
    return flag;
}

/**
屏幕方向

@return 屏幕方向
*/
- (UIInterfaceOrientation)supportedInterfaceOrientations1 {
    BOOL flag = objc_getAssociatedObject(self, FullScreenAllowRotationKey);
    if (flag) {
        return UIInterfaceOrientationLandscapeLeft;
    } else  {
        return UIInterfaceOrientationPortrait;
    }
}

- (UIInterfaceOrientation)preferredInterfaceOrientationForPresentation1 {
    BOOL flag = objc_getAssociatedObject(self, FullScreenAllowRotationKey);
    if (flag) {
        return UIInterfaceOrientationLandscapeLeft;
    } else {
        return UIInterfaceOrientationPortrait;
    }
}

/**
 强制横屏方法
 
 @param fullscreen 屏幕方向
 */
- (void)setNewOrientation:(BOOL)fullscreen {
    HWAppDelegate.allowRotation = fullscreen;
     objc_setAssociatedObject(self, FullScreenAllowRotationKey,[NSNumber numberWithBool:fullscreen], OBJC_ASSOCIATION_ASSIGN);
    
    swizzleMethod([self class], @selector(shouldAutorotate), @selector(shouldAutorotate1));
    swizzleMethod([self class], @selector(supportedInterfaceOrientations), @selector(supportedInterfaceOrientations1));
    swizzleMethod([self class], @selector(preferredInterfaceOrientationForPresentation), @selector(preferredInterfaceOrientationForPresentation1));
    
    @autoreleasepool {
        if (fullscreen) {
            NSNumber *resetOrientationTarget = [NSNumber numberWithInt:UIInterfaceOrientationUnknown];
            [[UIDevice currentDevice] setValue:resetOrientationTarget forKey:@"orientation"];
            NSNumber *orientationTarget = [NSNumber numberWithInt:UIInterfaceOrientationLandscapeLeft];
            [[UIDevice currentDevice] setValue:orientationTarget forKey:@"orientation"];
        } else {
            NSNumber *resetOrientationTarget = [NSNumber numberWithInt:UIInterfaceOrientationUnknown];
            [[UIDevice currentDevice] setValue:resetOrientationTarget forKey:@"orientation"];
            NSNumber *orientationTarget = [NSNumber numberWithInt:UIInterfaceOrientationPortrait];
            [[UIDevice currentDevice] setValue:orientationTarget forKey:@"orientation"];
        }
    }
}

@end
