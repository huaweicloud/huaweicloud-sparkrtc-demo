
//
//  UIColor+HWColor.m
//  HW
//
//  Created by HW on 2019/7/22.
//  Copyright © 2019年 HW. All rights reserved.
//

#import "UIColor+HWColor.h"

@implementation UIColor (HWColor)
+ (UIColor *)colorWithString:(NSString *)color alpha:(CGFloat)alpha {
    //删除字符串中的空格
    NSString *string = [[color stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]] uppercaseString];
    // 字符串长度必定大于等于6。
    if ([string length] < 6) {
        return [UIColor clearColor];
    }
    //如果是0x开头的，那么截取字符串，字符串从索引为2的位置开始，一直到末尾
    if ([string hasPrefix:@"0X"]) {
        string = [string substringFromIndex:2];
    }
    //如果是#开头的，那么截取字符串，字符串从索引为1的位置开始，一直到末尾
    if ([string hasPrefix:@"#"]) {
        string = [string substringFromIndex:1];
    }
    if ([string length] != 6) {
        return [UIColor clearColor];
    }
    // 取到r b a 的值
    NSRange range;
    range.location = 0;
    range.length = 2;
    //r
    NSString *rStr = [string substringWithRange:range];
    //g
    range.location = 2;
    NSString *gStr = [string substringWithRange:range];
    //b
    range.location = 4;
    NSString *bStr = [string substringWithRange:range];
    
    unsigned int r, g, b;
    [[NSScanner scannerWithString:rStr] scanHexInt:&r];
    [[NSScanner scannerWithString:gStr] scanHexInt:&g];
    [[NSScanner scannerWithString:bStr] scanHexInt:&b];
    return [UIColor colorWithRed:((float)r / 255.0f) green:((float)g / 255.0f) blue:((float)b / 255.0f) alpha:alpha];
}

//默认alpha值为1
+ (UIColor *)colorWithString:(NSString *)color {
    return [self colorWithString:color alpha:1.0f];
}

@end
