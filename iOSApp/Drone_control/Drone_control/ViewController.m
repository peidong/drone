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

@property (weak, nonatomic) IBOutlet UIButton *Sent;

@end

@implementation ViewController
{
    int forwardData;
    int backwardData;
    int leftData;
    int rightData;

    NSArray *_sliders;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    _slider1.continuous = YES;
    _slider1.value = 0.3;
    forwardData = (int)([_slider1 value] * 255);
    [_slider1 addTarget:self action:@selector(updateValue) forControlEvents:UIControlEventValueChanged];

    
    _slider2.continuous = YES;
    _slider2.value = 0.3;
    backwardData = (int)([_slider2 value] * 255);
    [_slider2 addTarget:self action:@selector(updateValue) forControlEvents:UIControlEventValueChanged];


    _slider3.continuous = YES;
    _slider3.value = 0.3;
    leftData = (int)([_slider3 value] * 255);
    [_slider3 addTarget:self action:@selector(updateValue) forControlEvents:UIControlEventValueChanged];


    _slider4.continuous = YES;
    _slider4.value = 0.3;
    rightData = (int)([_slider4 value] * 255);
    [_slider4 addTarget:self action:@selector(updateValue) forControlEvents:UIControlEventValueChanged];


  _sliders = @[_slider1, _slider2, _slider3, _slider4];
}
-(void)updateValue{
    forwardData = (int)([_slider1 value] * 255);
    backwardData = (int)([_slider2 value] * 255);
    leftData = (int)([_slider3 value] * 255);
    rightData = (int)([_slider4 value] * 255);

    _forward.text = [NSString stringWithFormat:@"forward: %d", forwardData];
    _backward.text = [NSString stringWithFormat:@"forward: %d", backwardData];
    _left.text = [NSString stringWithFormat:@"forward: %d", leftData];
    _right.text = [NSString stringWithFormat:@"forward: %d", rightData];

}


- (IBAction)HandleSent:(id)sender {
    
    NSNumber *forwardDataN = [NSNumber numberWithUnsignedInt:forwardData];
    NSNumber *backwardDataN = [NSNumber numberWithUnsignedInt:backwardData];
    NSNumber *leftDataN = [NSNumber numberWithUnsignedInt:leftData];
    NSNumber *rightDataN = [NSNumber numberWithUnsignedInt:rightData];

    AFHTTPRequestOperationManager *manager = [AFHTTPRequestOperationManager manager];
    NSDictionary *parameters = @{@"forwardData":forwardDataN, @"backwardData":backwardDataN, @"leftData":leftDataN, @"rightData":rightDataN};
    [manager POST:@"http://fryer.ee.ucla.edu/rest/api/ip_address/post/" parameters:parameters
          success:^(AFHTTPRequestOperation *operation, id responseObject) {
              NSLog(@"JSON_todata: %@", responseObject);
          } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
              NSLog(@"Error: %@", error);
          }];
    
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
 

- (void)PostToWebsite:(NSNumber *)forwardDataN backwardData:(NSNumber *) backwardDataN leftData:(NSNumber *) leftDataN rightData:(NSNumber *) rightDataN
{
    AFHTTPRequestOperationManager *manager = [AFHTTPRequestOperationManager manager];
    NSDictionary *parameters = @{@"forwardData":forwardDataN, @"backwardData":backwardDataN, @"leftData":leftDataN, @"rightData":rightDataN};
    [manager POST:@"http://fryer.ee.ucla.edu/rest/api/ip_address/post/" parameters:parameters
          success:^(AFHTTPRequestOperation *operation, id responseObject) {
              NSLog(@"JSON_todata: %@", responseObject);
          } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
              NSLog(@"Error: %@", error);
          }];
    
}
*/

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
