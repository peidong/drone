//
//  ViewController.m
//  Drone_control
//
//  Created by YANGYANG on 10/6/15.
//  Copyright © 2015 YANGYANG. All rights reserved.
//

#import "ViewController.h"
#import "AFHTTPRequestOperationManager.h"

@interface ViewController ()
@property (weak, nonatomic) IBOutlet UISlider *slider1;
@property (weak, nonatomic) IBOutlet UISlider *slider2;
@property (weak, nonatomic) IBOutlet UISlider *slider3;
@property (weak, nonatomic) IBOutlet UISlider *slider4;

@property (weak, nonatomic) IBOutlet UILabel *forward;
@property (weak, nonatomic) IBOutlet UILabel *backward;
@property (weak, nonatomic) IBOutlet UILabel *left;
@property (weak, nonatomic) IBOutlet UILabel *right;

@property (weak, nonatomic) IBOutlet UIStepper *stepper1;
@property (weak, nonatomic) IBOutlet UIStepper *stepper2;
@property (weak, nonatomic) IBOutlet UIStepper *stepper3;
@property (weak, nonatomic) IBOutlet UIStepper *stepper4;
@property (weak, nonatomic) IBOutlet UIStepper *stepper_total;

@property (weak, nonatomic) IBOutlet UITextField *text1;
@property (weak, nonatomic) IBOutlet UITextField *text2;
@property (weak, nonatomic) IBOutlet UITextField *text3;
@property (weak, nonatomic) IBOutlet UITextField *text4;



@property (weak, nonatomic) IBOutlet UIButton *startButton;

@end

@implementation ViewController
{
    int forwardData;
    int backwardData;
    int leftData;
    int rightData;
    bool currentlyStart;
    int total;
    
    NSArray *_sliders;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    currentlyStart = NO;
    
    // Do any additional setup after loading the view, typically from a nib.
    _slider1.continuous = NO;
    _slider1.value = 0.3;
    forwardData = (int)([_slider1 value] * 100);
    [_slider1 addTarget:self action:@selector(updateValue) forControlEvents:UIControlEventValueChanged];

    
    _slider2.continuous = NO;
    _slider2.value = 0.3;
    backwardData = (int)([_slider2 value] * 100);
    [_slider2 addTarget:self action:@selector(updateValue) forControlEvents:UIControlEventValueChanged];


    _slider3.continuous = NO;
    _slider3.value = 0.3;
    leftData = (int)([_slider3 value] * 100);
    [_slider3 addTarget:self action:@selector(updateValue) forControlEvents:UIControlEventValueChanged];


    _slider4.continuous = NO;
    _slider4.value = 0.3;
    rightData = (int)([_slider4 value] * 100);
    [_slider4 addTarget:self action:@selector(updateValue) forControlEvents:UIControlEventValueChanged];
    
    _stepper1.value = 30;
    _stepper2.value = 30;
    _stepper3.value = 30;
    _stepper4.value = 30;
    total = 30;
    _stepper_total.value = 30;
    
    _text1.clearButtonMode = UITextFieldViewModeAlways;
    _text2.clearButtonMode = UITextFieldViewModeAlways;
    _text3.clearButtonMode = UITextFieldViewModeAlways;
    _text4.clearButtonMode = UITextFieldViewModeAlways;


  _sliders = @[_slider1, _slider2, _slider3, _slider4];
}
-(void)updateValue{
    
    forwardData = (int)(_slider1.value * 100);
    backwardData = (int)([_slider2 value] * 100);
    leftData = (int)([_slider3 value] * 100);
    rightData = (int)([_slider4 value] * 100);
    _stepper1.value = (int)([_slider1 value] * 100);
    _stepper2.value = (int)([_slider2 value] * 100);
    _stepper3.value = (int)([_slider3 value] * 100);
    _stepper4.value = (int)([_slider4 value] * 100);
    total = (int)([_stepper_total value]);
    
    _forward.text = [NSString stringWithFormat:@"a: %d", forwardData];
    _backward.text = [NSString stringWithFormat:@"b: %d", backwardData];
    _left.text = [NSString stringWithFormat:@"c: %d", leftData];
    _right.text = [NSString stringWithFormat:@"d: %d", rightData];
    
    _text1.text = [NSString stringWithFormat:@"%d", forwardData];
    _text2.text = [NSString stringWithFormat:@"%d", backwardData];
    _text3.text = [NSString stringWithFormat:@"%d", leftData];
    _text4.text = [NSString stringWithFormat:@"%d", rightData];
    
    if (currentlyStart) {
        NSNumber *forwardDataN = [NSNumber numberWithUnsignedInt:forwardData];
        NSNumber *backwardDataN = [NSNumber numberWithUnsignedInt:backwardData];
        NSNumber *leftDataN = [NSNumber numberWithUnsignedInt:leftData];
        NSNumber *rightDataN = [NSNumber numberWithUnsignedInt:rightData];
        
        [self PostToWebsite:forwardDataN backwardData:backwardDataN leftData:leftDataN rightData:rightDataN];
    }
    NSLog(@"Update Value");
    
}

//setup text field

- (IBAction)changeByText:(id)sender {
    if ([[_text1 text] intValue]>=0 && [[_text1 text] intValue] <=100 && [[_text2 text] intValue]>=0 && [[_text2 text] intValue] <=100 &&[[_text3 text] intValue]>=0 && [[_text3 text] intValue] <=100 && [[_text4 text] intValue]>=0 && [[_text4 text] intValue] <=100) {
        
    _slider1.value = [[_text1 text] intValue]*0.01;
    _slider2.value = [[_text2 text] intValue]*0.01;
        
    _slider3.value = [[_text3 text] intValue]*0.01;
    _slider4.value = [[_text4 text] intValue]*0.01;
    NSLog(@"Edit did end");
    [self updateValue];
    }
}


- (IBAction)stepper_total:(id)sender {
    if (total - _stepper_total.value  < 0) {
        _slider1.value = _slider1.value + 0.01;
        _slider2.value = _slider2.value + 0.01;
        _slider3.value = _slider3.value + 0.01;
        _slider4.value = _slider4.value + 0.01;
    }else{
        _slider1.value = _slider1.value - 0.01;
        _slider2.value = _slider2.value - 0.01;
        _slider3.value = _slider3.value - 0.01;
        _slider4.value = _slider4.value - 0.01;
    }
    
    [self updateValue];

}

//setup stepper action
- (IBAction)stepper:(id)sender {
    if (_slider1.value - _stepper1.value * 0.01 < 0) {
        _slider1.value = _slider1.value + 0.01;
    }else{
        _slider1.value = _slider1.value - 0.01;
    }
    [self updateValue];
}
- (IBAction)stepper2:(id)sender {
    if (_slider2.value - _stepper2.value * 0.01 < 0) {
        _slider2.value = _slider2.value + 0.01;
    }else{
        _slider2.value = _slider2.value - 0.01;
    }
    [self updateValue];
}
- (IBAction)stepper3:(id)sender {
    if (_slider3.value - _stepper3.value * 0.01 < 0) {
        _slider3.value = _slider3.value + 0.01;
    }else{
        _slider3.value = _slider3.value - 0.01;
    }
    [self updateValue];
}

- (IBAction)stepper4:(id)sender {
    if (_slider4.value - _stepper4.value * 0.01 < 0) {
        _slider4.value = _slider4.value + 0.01;
    }else{
        _slider4.value = _slider4.value - 0.01;
    }
    [self updateValue];
}

- (IBAction)HandleStart:(id)sender {
    if (currentlyStart) {
        currentlyStart = NO;
        //            [self.trackingButton setTitle:@"start tracking" forState:UIControlStateNormal];
        [self.startButton setTitle:@"start sending" forState:UIControlStateNormal];
    }else{
        currentlyStart = YES;
        [self.startButton setTitle:@"stop sending" forState:UIControlStateNormal];
        [self updateValue];

    }
    
}


- (IBAction)MoveMinimum:(UIButton *)sender {
    for (UISlider *slider in _sliders) {
        [self animateSlider:slider toValue:slider.minimumValue];
    }
    [self updateValue];

}

- (IBAction)MoveMaximum:(UIButton *)sender {
    for (UISlider *slider in _sliders) {
        [self animateSlider:slider toValue:slider.maximumValue];
    }
    [self updateValue];

}

- (IBAction)MoveMedium:(UIButton *)sender {
    for (UISlider *slider in _sliders) {
        [self animateSlider:slider toValue:(slider.maximumValue/2)];
    }
    [self updateValue];

}

- (void)animateSlider:(UISlider*)slider toValue:(float)value
{
    if (NSFoundationVersionNumber > NSFoundationVersionNumber_iOS_6_1) {
        [UIView animateWithDuration:0.5 animations:^{
            [slider setValue:value animated:YES];
        }];
    }
    else {
        [slider setValue:value animated:YES];
    }
}

/*  POST data to  server
 *  send gps information to server
 *  referencing afnetwork from github
 */

- (void)PostToWebsite:(NSNumber *)forwardDataN backwardData:(NSNumber *) backwardDataN leftData:(NSNumber *) leftDataN rightData:(NSNumber *) rightDataN
{
    AFHTTPRequestOperationManager *manager = [AFHTTPRequestOperationManager manager];
    NSDictionary *parameters = @{@"pwm1":forwardDataN, @"pwm2":backwardDataN, @"pwm3":leftDataN, @"pwm4":rightDataN};
    [manager POST:@"http://fryer.ee.ucla.edu/rest/api/pwm/post/" parameters:parameters
          success:^(AFHTTPRequestOperation *operation, id responseObject) {
              NSLog(@"JSON_todata: %@", responseObject);
          } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
              NSLog(@"Error: %@", error);
          }];
    
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
