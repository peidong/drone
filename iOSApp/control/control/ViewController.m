//
//  ViewController.m
//  control
//
//  Created by YANGYANG on 10/25/15.
//  Copyright © 2015 YANGYANG. All rights reserved.
//

#import "ViewController.h"
#import "AFHTTPRequestOperationManager.h"

@interface ViewController ()
@property (weak, nonatomic) IBOutlet UIButton *clockwise;
@property (weak, nonatomic) IBOutlet UIButton *unclock;
@property (weak, nonatomic) IBOutlet UIButton *forward;
@property (weak, nonatomic) IBOutlet UIButton *backward;
@property (weak, nonatomic) IBOutlet UIButton *right;
@property (weak, nonatomic) IBOutlet UIButton *left;
@property (weak, nonatomic) IBOutlet UIButton *up;
@property (weak, nonatomic) IBOutlet UIButton *down;
@property (weak, nonatomic) IBOutlet UIButton *suspend;
@property (weak, nonatomic) IBOutlet UIButton *stop;

@property (weak, nonatomic) IBOutlet UITextField *pwm1;
@property (weak, nonatomic) IBOutlet UITextField *pwm2;
@property (weak, nonatomic) IBOutlet UITextField *pwm3;
@property (weak, nonatomic) IBOutlet UITextField *pwm4;
@property (weak, nonatomic) IBOutlet UITextField *mac_add;

@property (weak, nonatomic) IBOutlet UIButton *Manual_auto;
@end

@implementation ViewController{
    NSArray *_sliders;
    int control_type;
    int auto_control_command;
    int manual_control_command;
    int suspend_pwm1, suspend_pwm2, suspend_pwm3, suspend_pwm4;
    bool currentlyManual;

}

- (void)viewDidLoad {
    [super viewDidLoad];
    currentlyManual = YES;
    control_type = 1;
    
    _pwm1.clearButtonMode = UITextFieldViewModeAlways;
    _pwm2.clearButtonMode = UITextFieldViewModeAlways;
    _pwm3.clearButtonMode = UITextFieldViewModeAlways;
    _pwm4.clearButtonMode = UITextFieldViewModeAlways;

}

- (IBAction)sendPWM:(id)sender {
    
    NSNumber *forwardDataN = [NSNumber numberWithUnsignedInt:suspend_pwm1];
    NSNumber *backwardDataN = [NSNumber numberWithUnsignedInt:suspend_pwm2];
    NSNumber *leftDataN = [NSNumber numberWithUnsignedInt:suspend_pwm3];
    NSNumber *rightDataN = [NSNumber numberWithUnsignedInt:suspend_pwm4];
    
    [self PostToWebsite1:forwardDataN backwardData:backwardDataN leftData:leftDataN rightData:rightDataN];
}



//setup text field

- (IBAction)changeByText:(id)sender {
    if ([[_pwm1 text] intValue]>=0 && [[_pwm1 text] intValue] <=100 && [[_pwm2 text] intValue]>=0 && [[_pwm2 text] intValue] <=100 &&[[_pwm3 text] intValue]>=0 && [[_pwm3 text] intValue] <=100 && [[_pwm4 text] intValue]>=0 && [[_pwm4 text] intValue] <=100) {
        
        suspend_pwm1 = [[_pwm1 text] intValue];
        suspend_pwm2 = [[_pwm2 text] intValue];
        suspend_pwm3 = [[_pwm3 text] intValue];
        suspend_pwm4 = [[_pwm4 text] intValue];
    }
}

- (IBAction)HandleStart:(id)sender {
    if (currentlyManual) {
        currentlyManual = NO;
        control_type = 2;
        [self.Manual_auto setTitle:@"Auto" forState:UIControlStateNormal];
    }else{
        currentlyManual = YES;
        control_type = 1;
        [self.Manual_auto setTitle:@"Manual" forState:UIControlStateNormal];
    }
    NSNumber *control_typeN = [NSNumber numberWithUnsignedInt:control_type];

    AFHTTPRequestOperationManager *manager = [AFHTTPRequestOperationManager manager];
    NSDictionary *parameters = @{@"control_type":control_typeN};
    [manager POST:@"http://fryer.ee.ucla.edu/rest/api/control/post/?mac_address=fc:c2:de:3d:7f:af" parameters:parameters
          success:^(AFHTTPRequestOperation *operation, id responseObject) {
              NSLog(@"JSON_todata: %@", responseObject);
          } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
              NSLog(@"Error: %@", error);
          }];
}



/*  POST data to  server
 *  send gps information to server
 *  referencing afnetwork from github
 */

- (void)PostToWebsite1:(NSNumber *)forwardDataN backwardData:(NSNumber *) backwardDataN leftData:(NSNumber *) leftDataN rightData:(NSNumber *) rightDataN
{
    AFHTTPRequestOperationManager *manager = [AFHTTPRequestOperationManager manager];
    NSDictionary *parameters = @{@"suspend_pwm1":forwardDataN, @"suspend_pwm2":backwardDataN, @"suspend_pwm3":leftDataN, @"suspend_pwm4":rightDataN};
    [manager POST:@"http://fryer.ee.ucla.edu/rest/api/control/post/?mac_address=fc:c2:de:3d:7f:af" parameters:parameters
          success:^(AFHTTPRequestOperation *operation, id responseObject) {
              NSLog(@"JSON_todata: %@", responseObject);
          } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
              NSLog(@"Error: %@", error);
          }];
    
}


- (void)PostToWebsite2:(NSNumber *)auto_control_commandN manual_control_command:(NSNumber *) manual_control_commandN
{
    AFHTTPRequestOperationManager *manager = [AFHTTPRequestOperationManager manager];
    NSDictionary *parameters = @{@"auto_control_command":auto_control_commandN, @"manual_control_command":manual_control_commandN};
    [manager POST:@"http://fryer.ee.ucla.edu/rest/api/control/post/?mac_address=fc:c2:de:3d:7f:af" parameters:parameters
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
