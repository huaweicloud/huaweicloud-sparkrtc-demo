//
//  HWTemporaryLoginView.m
//  HwLiveDemo
//
//  Created by HW on 2020/7/16.
//  Copyright © 2020 HW. All rights reserved.
//

#import "HWLoginView.h"
#import "UITextField+Expand.h"
@interface HWLoginView ()<UITextFieldDelegate>
@property (weak, nonatomic) IBOutlet UIButton *loginButton;
@property (weak, nonatomic) IBOutlet UITextField *userNameTextField;

@end

@implementation HWLoginView

+(instancetype)initWithXib:(CGRect)frame {
    HWLoginView *view = [[NSBundle mainBundle] loadNibNamed:NSStringFromClass([self class]) owner:self options:nil].lastObject;
    view.frame = frame;
    [view awakeFromNib];
    return view;
}

//1、正在准备初始化 -- loadNibName 之后会调用
- (instancetype)initWithCoder:(NSCoder *)aDecoder {
    self = [super initWithCoder:aDecoder];
    if (self) {
       
    }
    return self;
}

//2、初始化完毕`（若想初始化时做点事情，最好在这个方法里面实现）`
- (void)awakeFromNib {
    [super awakeFromNib];

    self.userNameTextField.layer.borderColor = [UIColor whiteColor].CGColor;
    self.userNameTextField.layer.borderWidth = 1;
    self.userNameTextField.attributedPlaceholder = [[NSAttributedString alloc] initWithString:@"设置用户名" attributes:@{NSForegroundColorAttributeName: [UIColor colorWithWhite:1 alpha:0.5]}];
    self.userNameTextField.layer.cornerRadius = 20;
    self.userNameTextField.layer.masksToBounds = YES;
    self.userNameTextField.delegate = self;
    self.loginButton.layer.cornerRadius = 20;
    self.loginButton.layer.masksToBounds = YES;
    
    [self.userNameTextField lengthLimit:^{
           if (self.userNameTextField.text.length > 8) {
               self.userNameTextField.text = [self.userNameTextField.text substringToIndex:8];
           }
       }];

}

- (IBAction)userTextFieldAction:(UITextField *)sender {
    
}

- (IBAction)loginAction:(UIButton *)sender {
    if (self.userNameTextField.text.length == 0) {
        [HWTool showMessage:@"请输入用户名"];
        return;
    }     
    if (self.delegate && [self.delegate respondsToSelector:@selector(LoginView:userName:)]) {
        [self.delegate LoginView:self userName:self.userNameTextField.text];
    }
}

#pragma mark - UITextFieldDelegate
- (BOOL)textFieldShouldBeginEditing:(UITextField *)textField {
    NSLog(@"textFieldShouldBeginEditing");
    if ([UIScreen mainScreen].bounds.size.height < 700) {
        [UIView animateWithDuration:0.25 animations:^{
            self.frame = CGRectMake(0, -120, SCREENWIDTH, SCREENHEIGHT);
        }];
    }
    return YES;
}

- (void)textFieldDidEndEditing:(UITextField *)textField {
    self.frame = CGRectMake(0, 0, SCREENWIDTH, SCREENHEIGHT);
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    self.frame = CGRectMake(0, 0, SCREENWIDTH, SCREENHEIGHT);
    [self endEditing:YES];
}

@end
