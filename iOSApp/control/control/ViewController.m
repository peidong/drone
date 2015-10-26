//
//  ViewController.m
//  control
//
//  Created by YANGYANG on 10/25/15.
//  Copyright © 2015 YANGYANG. All rights reserved.
//

#import "ViewController.h"

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

@property (weak, nonatomic) IBOutlet UIButton *Manual_auto;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
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
