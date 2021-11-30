//
//  HWRoomListCreateRoomView.m
//  HwLiveDemo
//
//  Created by HW on 2021/1/20.
//  Copyright © 2021 HW. All rights reserved.
//

#import "HWRoomListCreateRoomView.h"

@interface HWRoomListCreateRoomView ()<UITextFieldDelegate>
@property (weak, nonatomic) IBOutlet UITextField *roomNameTextField;
@property (weak, nonatomic) IBOutlet UIButton *createRoomButton;
@end

@implementation HWRoomListCreateRoomView
+ (instancetype)initWithXib:(CGRect)frame {
    HWRoomListCreateRoomView *view = [[NSBundle mainBundle] loadNibNamed:NSStringFromClass([self class]) owner:self options:nil].lastObject;
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
    [self modifyUI];
}

- (void)modifyUI {
    self.backgroundColor = [UIColor colorWithWhite:0.5 alpha:0.5];
    UITapGestureRecognizer *tapGR = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(closeCreateRoomView)];
    tapGR.numberOfTouchesRequired = 1;
    tapGR.numberOfTapsRequired = 1;
    [self addGestureRecognizer:tapGR];
    
    [self.createRoomButton.layer setCornerRadius: self.createRoomButton.frame.size.height/2];
    [self.createRoomButton.layer masksToBounds];
}

- (void)closeCreateRoomView{
    [UIView animateWithDuration:0.25 animations:^{
        self.frame = CGRectMake(0, SCREENHEIGHT, SCREENWIDTH, SCREENHEIGHT);
        [self endEditing:YES];
    }];
}

- (IBAction)createRoomButtonAction:(UIButton *)sender {
    
    if (self.roomNameTextField.text.length == 0) {
        [HWTool showMessage:@"请输入房间号"];
        return;
    }
    
    if (self.delegate && [self.delegate respondsToSelector:@selector(HWRoomListCreateRoomViewCreateRoomAction:)]) {
        [self.delegate HWRoomListCreateRoomViewCreateRoomAction:self.roomNameTextField.text];
        [self closeCreateRoomView];
    }
}

- (void)textFieldDidEndEditing:(UITextField *)textField {
    
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    return YES;
}


@end
