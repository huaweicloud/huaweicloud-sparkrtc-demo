//
//  HWTool.m
//  HW
//
//  Created by HW on 2019/7/26.
//  Copyright © 2019年 HW. All rights reserved.
//

#import "HWTool.h"
@implementation HWTool

+ (void)showMessage:(NSString *)string {
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.2 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        __block UIAlertController *alertController = [UIAlertController alertControllerWithTitle:@"提示：" message:string preferredStyle:UIAlertControllerStyleAlert];
        UIApplication *application = [UIApplication sharedApplication];
        [application.keyWindow.rootViewController presentViewController:alertController animated:YES completion:nil];
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1.5 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            [alertController dismissViewControllerAnimated:YES completion:nil];
        });
    });
}

/**  卸载应用重新安装后会不一致*/
+ (NSString *)getUUID {
    NSString * strUUID = [UIDevice currentDevice].identifierForVendor.UUIDString;
    //RoomID: 字符串， 64个字节以内。 26 个小写英文字母 a~z； 26 个大写英文字母 A~Z； 10 个数字 0~9；
    //为了满足拼接roomID（roomID中不允许使用‘-’字符）
    strUUID = [strUUID stringByReplacingOccurrencesOfString:@"-" withString:@""];
    return strUUID;
}

+ (CALayer *)getGradientLayerWithFrame:(CGRect)frame
                            frameColor:(UIColor *)frameColor
                               toColot:(UIColor *)toColor {
    CAGradientLayer *gradient = [CAGradientLayer layer];
    gradient.frame = frame;
    gradient.colors = @[(id)frameColor.CGColor,(id)toColor.CGColor];
    gradient.startPoint = CGPointMake(0, 1.0);
    gradient.endPoint = CGPointMake(0, 0.0);
    gradient.locations = @[@0,@1];
    return gradient;
}

@end
