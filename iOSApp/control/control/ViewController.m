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
@property (weak, nonatomic) IBOutlet UIButton *pwm_plus;
@property (weak, nonatomic) IBOutlet UIButton *pwm_minus;
@property (weak, nonatomic) IBOutlet UIButton *pwm_daplus;
@property (weak, nonatomic) IBOutlet UIButton *pwm_daminus;
@property (weak, nonatomic) IBOutlet UIButton *start;
@property (weak, nonatomic) IBOutlet UIButton *enable;
@property (weak, nonatomic) IBOutlet UIButton *disable;


@property (weak, nonatomic) IBOutlet UITextField *pwm1;
@property (weak, nonatomic) IBOutlet UITextField *pwm2;
@property (weak, nonatomic) IBOutlet UITextField *pwm3;
@property (weak, nonatomic) IBOutlet UITextField *pwm4;
@property (weak, nonatomic) IBOutlet UITextField *mac_add;


@property (weak, nonatomic) IBOutlet UILabel *get_pwm1;
@property (weak, nonatomic) IBOutlet UILabel *get_pwm2;
@property (weak, nonatomic) IBOutlet UILabel *get_pwm3;
@property (weak, nonatomic) IBOutlet UILabel *get_pwm4;

@property (weak, nonatomic) IBOutlet UIButton *startGet;


@property (weak, nonatomic) IBOutlet UITextField *p_pitch;
@property (weak, nonatomic) IBOutlet UITextField *p_roll;
@property (weak, nonatomic) IBOutlet UITextField *p_yaw;
@property (weak, nonatomic) IBOutlet UITextField *i_pitch;
@property (weak, nonatomic) IBOutlet UITextField *i_roll;
@property (weak, nonatomic) IBOutlet UITextField *i_yaw;
@property (weak, nonatomic) IBOutlet UITextField *d_pitch;
@property (weak, nonatomic) IBOutlet UITextField *d_roll;
@property (weak, nonatomic) IBOutlet UITextField *d_yaw;

@property (weak, nonatomic) IBOutlet UIButton *send_pid;


@property (weak, nonatomic) IBOutlet UIButton *Manual_auto;
@end

@implementation ViewController{
    NSArray *_sliders;
    int control_type;
    int auto_control_command;
    int manual_control_command;
    double suspend_pwm1, suspend_pwm2, suspend_pwm3, suspend_pwm4;
    
    double d_p_pitch, d_p_roll, d_p_yaw, d_i_pitch, d_i_roll, d_i_yaw, d_d_pitch, d_d_roll, d_d_yaw;
    bool currentlyManual;
    bool startFly;
    bool startLearn;
    bool isgetting;

    NSNumber *ns_get_pwm1,*ns_get_pwm2,*ns_get_pwm3,*ns_get_pwm4;
    NSDictionary *value;
    double ns_get_pwm5;

    double d_get_pwm1, d_get_pwm2, d_get_pwm3, d_get_pwm4;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    currentlyManual = YES;
    control_type = 1;
    startFly = NO;
    startLearn = NO;
    auto_control_command =0;
    manual_control_command = 0;
    isgetting = NO;
    
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

- (IBAction)getFromServer:(id)sender {

    AFHTTPRequestOperationManager *manager = [AFHTTPRequestOperationManager manager];
    [manager GET:@"http://fryer.ee.ucla.edu/rest/api/pwm/get/" parameters:nil
         success:^(AFHTTPRequestOperation *operation, id responseObject) {
             NSLog(@"JSON: %@", responseObject);
             value = responseObject[@"data"];
             ns_get_pwm1 = value[@"pwm1"];
             ns_get_pwm2 = value[@"pwm2"];
             ns_get_pwm3 = value[@"pwm3"];
             ns_get_pwm4 = value[@"pwm4"];
             [self get_pwm1].text = [NSString stringWithFormat:@"1: %@", ns_get_pwm1];
             [self get_pwm2].text = [NSString stringWithFormat:@"2: %@", ns_get_pwm2];
             [self get_pwm3].text = [NSString stringWithFormat:@"3: %@", ns_get_pwm3];
             [self get_pwm4].text = [NSString stringWithFormat:@"4: %@", ns_get_pwm4];
         } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
             NSLog(@"Error: %@", error);
         }];
}


//setup text field

- (IBAction)changeByText:(id)sender {
    if ([[_pwm1 text] doubleValue]>=0 && [[_pwm1 text] doubleValue] <=100 && [[_pwm2 text] doubleValue]>=0 && [[_pwm2 text] doubleValue] <=100 &&[[_pwm3 text] doubleValue]>=0 && [[_pwm3 text] doubleValue] <=100 && [[_pwm4 text] doubleValue]>=0 && [[_pwm4 text] doubleValue] <=100) {
        
        suspend_pwm1 = [[_pwm1 text] doubleValue];
        suspend_pwm2 = [[_pwm2 text] doubleValue];
        suspend_pwm3 = [[_pwm3 text] doubleValue];
        suspend_pwm4 = [[_pwm4 text] doubleValue];
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

- (IBAction)startfly:(id)sender {
    if (startFly) {
        startFly = NO;
        auto_control_command =0;
        [self.Manual_auto setTitle:@"startFly" forState:UIControlStateNormal];
    }else{
        startFly = YES;
        auto_control_command =1;
        [self.Manual_auto setTitle:@"Fly_Starting" forState:UIControlStateNormal];
    }
    [self updateValue];
}


- (IBAction)learn:(id)sender {
    if (startLearn) {
        startLearn = NO;
        auto_control_command =0;
        [self.Manual_auto setTitle:@"startLearn" forState:UIControlStateNormal];
    }else{
        startLearn = YES;
        auto_control_command =2;
        [self.Manual_auto setTitle:@"Learn_Starting" forState:UIControlStateNormal];
    }
    [self updateValue];
}


- (IBAction)suspeng_1:(id)sender {
    manual_control_command = 1;
    [self updateValue];
}

- (IBAction)up_2:(id)sender {
    manual_control_command = 2;
    [self updateValue];

}

- (IBAction)down_3:(id)sender {
    manual_control_command = 3;
    [self updateValue];

}

- (IBAction)forward_4:(id)sender {
    manual_control_command = 4;
    [self updateValue];

}
- (IBAction)backward_5:(id)sender {
    manual_control_command = 5;
    [self updateValue];

}
- (IBAction)left_6:(id)sender {
    manual_control_command = 6;
    [self updateValue];

}
- (IBAction)right7:(id)sender {
    manual_control_command = 7;
    [self updateValue];

}
- (IBAction)clock_8:(id)sender {
    manual_control_command = 8;
    [self updateValue];

}
- (IBAction)unclock_9:(id)sender {
    manual_control_command = 9;
    [self updateValue];

}
- (IBAction)stop_10:(id)sender {
    manual_control_command = 10;
    [self updateValue];

}
- (IBAction)pwm_plus_11:(id)sender {
    manual_control_command = 11;
    [self updateValue];

}
- (IBAction)pwm_minus_12:(id)sender {
    manual_control_command = 12;
    [self updateValue];

}

- (IBAction)pwm_daplus_13:(id)sender {
    manual_control_command = 13;
    [self updateValue];

}
- (IBAction)pwm_daminus_14:(id)sender {
    manual_control_command = 14;
    [self updateValue];

}
- (IBAction)start:(id)sender {
    manual_control_command = 0;
    [self updateValue];
}
- (IBAction)enable:(id)sender {
    manual_control_command = 16;
    [self updateValue];
}
- (IBAction)disable:(id)sender {
    manual_control_command = 17;
    [self updateValue];
    
}


- (IBAction)updatePid:(id)sender {
    d_d_pitch = [[_d_pitch text] doubleValue];
    d_d_roll = [[_d_roll text] doubleValue];
    d_d_yaw = [[_d_yaw text] doubleValue];
    
    d_i_pitch = [[_i_pitch text] doubleValue];
    d_i_roll = [[_i_roll text] doubleValue];
    d_i_yaw = [[_i_yaw text] doubleValue];
    
    d_p_pitch = [[_p_pitch text] doubleValue];
    d_p_roll = [[_p_roll text] doubleValue];
    d_p_yaw = [[_p_yaw text] doubleValue];
}


- (IBAction)sendpid:(id)sender {
    
    NSNumber *ns_d_pitch = [NSNumber numberWithDouble:d_d_pitch];
    NSNumber *ns_d_roll = [NSNumber numberWithDouble:d_d_roll];
    NSNumber *ns_d_yaw = [NSNumber numberWithDouble:d_d_yaw];
    NSNumber *ns_i_pitch = [NSNumber numberWithDouble:d_i_pitch];
    NSNumber *ns_i_roll = [NSNumber numberWithDouble:d_i_roll];
    NSNumber *ns_i_yaw = [NSNumber numberWithDouble:d_i_yaw];
    NSNumber *ns_p_pitch = [NSNumber numberWithDouble:d_p_pitch];
    NSNumber *ns_p_roll = [NSNumber numberWithDouble:d_p_roll];
    NSNumber *ns_p_yaw = [NSNumber numberWithDouble:d_p_yaw];
    
    AFHTTPRequestOperationManager *manager = [AFHTTPRequestOperationManager manager];
    NSDictionary *parameters = @{@"kp_pitch":ns_p_pitch, @"ki_pitch":ns_i_pitch, @"kd_pitch":ns_d_pitch, @"kp_roll":ns_p_roll, @"ki_roll":ns_i_roll, @"kd_roll":ns_d_roll, @"kp_yaw":ns_p_yaw, @"ki_yaw":ns_i_yaw, @"kd_yaw":ns_d_yaw};
    [manager POST:@"http://fryer.ee.ucla.edu/rest/api/pid_tuning/post/" parameters:parameters
          success:^(AFHTTPRequestOperation *operation, id responseObject) {
              NSLog(@"JSON_todata: %@", responseObject);
          } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
              NSLog(@"Error: %@", error);
          }];
    
}


-(void)updateValue{
    
    NSNumber *auto_control_commandN = [NSNumber numberWithUnsignedInt:auto_control_command];
    NSNumber *manual_control_commandN = [NSNumber numberWithUnsignedInt:manual_control_command];
    [self PostToWebsite2:auto_control_commandN manual_control_command:manual_control_commandN];
    NSLog(@"Update Value");
    
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
