//
//  ViewController.m
//  Drone_onair
//
//  Created by YANGYANG on 10/3/15.
//  Copyright © 2015 YANGYANG. All rights reserved.
//

#import "ViewController.h"
#import <CoreLocation/CoreLocation.h>
#import "AFHTTPRequestOperationManager.h"

@interface ViewController () <CLLocationManagerDelegate>
@property (strong, nonatomic) IBOutlet UILabel *latitudeLabel;
@property (strong, nonatomic) IBOutlet UILabel *longitudeLabel;
@property (strong, nonatomic) IBOutlet UILabel *accuracyLabel;
@property (strong, nonatomic) IBOutlet UILabel *timestampLabel;
@property (strong, nonatomic) IBOutlet UIButton *trackingButton;
@property (strong, nonatomic) IBOutlet UILabel *accuracyLevelLabel;
@property (strong, nonatomic) IBOutlet UILabel *sessionIDLabel;

@end

@implementation ViewController
{
    CLLocationManager *locationManager;
    CLLocation *previousLocation;
    double totalDistanceInMeters;
    bool currentlyTracking;
    bool firstTimeGettingPosition;
    NSUUID *guid;
    NSDate *lastWebsiteUpdateTime;
    int timeIntervalInSeconds;
    bool increasedAccuracy;

}

- (void)viewDidLoad {
    [super viewDidLoad];
    currentlyTracking = NO;
    timeIntervalInSeconds = 60; // change this to the time interval you want
    
    BOOL appIDIsSet = [[NSUserDefaults standardUserDefaults] boolForKey:@"appIDIsSet"];
    if (!appIDIsSet) {
        [[NSUserDefaults standardUserDefaults] setBool:YES forKey:@"appIDIsSet"];
        [[NSUserDefaults standardUserDefaults] setObject:[[NSUUID UUID] UUIDString] forKey:@"appID"];
        [[NSUserDefaults standardUserDefaults] synchronize];
    }

}

- (void)startTracking
{
    NSLog(@"start tracking");
    
    locationManager = [[CLLocationManager alloc] init];
    locationManager.desiredAccuracy = kCLLocationAccuracyNearestTenMeters;
    locationManager.distanceFilter = 0; // meters
    //locationManager.pausesLocationUpdatesAutomatically = NO; // YES is default
    locationManager.activityType = CLActivityTypeAutomotiveNavigation;
    locationManager.delegate = self;
    
    guid = [NSUUID UUID];
    totalDistanceInMeters = 0;
    increasedAccuracy = YES;
    firstTimeGettingPosition = YES;
    lastWebsiteUpdateTime = [NSDate date]; // new timestamp
    [self updateAccuracyLevel:@"high"];
    [self sessionIDLabel].text = [guid UUIDString];
    if ([locationManager respondsToSelector:@selector(requestWhenInUseAuthorization)]) {
        [locationManager requestWhenInUseAuthorization];
    }
    
    [locationManager startUpdatingLocation];
}

- (void)stopTracking
{
    NSLog(@"stop tracking");
    
    [self sessionIDLabel].text = @"phoneNumber:";
    [locationManager stopUpdatingLocation];
    locationManager = nil;
}

- (IBAction)handleTrackingButton:(id)sender {
        if (currentlyTracking) {
            [self stopTracking];
            currentlyTracking = NO;
            [self.trackingButton setTitle:@"start tracking" forState:UIControlStateNormal];
        } else {
            [self startTracking];
            currentlyTracking = YES;
            [self.trackingButton setTitle:@"stop tracking" forState:UIControlStateNormal];
        }
}

- (void)reduceTrackingAccuracy
{
    locationManager.desiredAccuracy = kCLLocationAccuracyThreeKilometers;
    locationManager.distanceFilter = 5;
    increasedAccuracy = NO;
    [self updateAccuracyLevel:@"low"];
}

- (void)increaseTrackingAccuracy
{
    locationManager.desiredAccuracy = kCLLocationAccuracyNearestTenMeters;
    locationManager.distanceFilter = 0;
    increasedAccuracy = YES;
    [self updateAccuracyLevel:@"high"];
}

- (void)locationManager:(CLLocationManager *)manager didUpdateLocations:(NSArray *)locations
{
    CLLocation *location = [locations lastObject];
    
    NSTimeInterval secondsSinceLastWebsiteUpdate = fabs([lastWebsiteUpdateTime timeIntervalSinceNow]);
    if (firstTimeGettingPosition || (secondsSinceLastWebsiteUpdate > timeIntervalInSeconds)) { // currently one minute
        
        if (location.horizontalAccuracy < 500.0 && location.coordinate.latitude != 0 && location.coordinate.longitude != 0) {
            
            if (increasedAccuracy) {
                [self reduceTrackingAccuracy];
            }
            
            if (firstTimeGettingPosition) {
                firstTimeGettingPosition = NO;
            } else {
                CLLocationDistance distance = [location distanceFromLocation:previousLocation];
                totalDistanceInMeters += distance;
            }
            
            previousLocation = location;
            
            
            // This is for reference from github
           
            NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
           
            [dateFormatter setDateFormat:@"yyyy-MM-dd%20HH:mm:ss"]; // mysql format
            NSString *latitude = [NSString stringWithFormat:@"%f", location.coordinate.latitude];
            NSString *longitude = [NSString stringWithFormat:@"%f", location.coordinate.longitude];
            
            
             //note that the guid is created in startTracking method above
            [self GetWebsiteData:latitude longitude:longitude];
            [self PostToWebsite:latitude longitude:longitude];
            
            
            lastWebsiteUpdateTime = [NSDate date]; // new timestamp
            
        } else if (!increasedAccuracy) {
            [self increaseTrackingAccuracy];
        }
    }
    
    if ([[UIApplication sharedApplication] applicationState] == UIApplicationStateActive) {
        [self updateUIWithLocationData:location];
    }
}

- (void)updateAccuracyLevel:(NSString *)accuracyLevel
{
    [self accuracyLevelLabel].text= [NSString stringWithFormat:@"accuracy level: %@", accuracyLevel];
}

- (void)updateUIWithLocationData:(CLLocation *)location
{
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"dd/MM/yyyy HH:mm"];
    [self timestampLabel].text = [NSString stringWithFormat:@"timestamp: %@",[dateFormatter stringFromDate:location.timestamp]];
    [self latitudeLabel].text = [NSString stringWithFormat:@"latitude: %f", location.coordinate.latitude];
    [self longitudeLabel].text = [NSString stringWithFormat:@"longitude: %f", location.coordinate.longitude];
    [self accuracyLabel].text= [NSString stringWithFormat:@"accuracy: %dm", (int)location.horizontalAccuracy];
}

- (void)locationManager:(CLLocationManager *)manager didFailWithError:(NSError *)error
{
    NSLog(@"locationManager error: %@", [error description]);
}



/*  send data to json server
 *  Get Data ip_address from web to ensurance connecting
 *  referencing afnetwork from github
 */

- (void)GetWebsiteData:(NSString *)latitude longitude:(NSString *)longitude
{
    AFHTTPRequestOperationManager *manager = [AFHTTPRequestOperationManager manager];
    [manager GET:@"http://fryer.ee.ucla.edu/rest/api/ip_address/get/" parameters:nil
     success:^(AFHTTPRequestOperation *operation, id responseObject) {
        NSLog(@"JSON: %@", responseObject);
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        NSLog(@"Error: %@", error);
    }];
}


/*  POST data to  server
 *  send gps information to server
 *  referencing afnetwork from github
 */

- (void)PostToWebsite:(NSString *)latitude longitude:(NSString *)longitude
{
    // use the websmithing defaultUploadWebsite for testing, change the *phoneNumber* form variable to something you
    // know and then check your location with your browser here: https://www.websmithing.com/gpstracker/displaymap.php
    /*
     NSString *defaultUploadWebsite = @"http://example.com/resources.json";
     
     AFHTTPRequestOperationManager *manager = [AFHTTPRequestOperationManager manager];
     manager.responseSerializer = [AFHTTPResponseSerializer serializer];
     
     NSDictionary *parameters = @{@"latitude": latitude,
     @"longitude": longitude,
     };
     
     [manager GET:defaultUploadWebsite parameters:parameters
     success:^(AFHTTPRequestOperation *operation, id responseObject) {
     NSString *response = [[NSString alloc] initWithData:responseObject encoding:NSUTF8StringEncoding];
     NSLog(@"Response: %@", response);
     }
     failure:^(AFHTTPRequestOperation *operation, NSError *error) {
     NSLog(@"AFHTTPRequestOperation Error: %@", [error description]);
     }];
    */
    
    AFHTTPRequestOperationManager *manager = [AFHTTPRequestOperationManager manager];
    NSDictionary *parameters = @{@"ip_address":latitude, @"network_name":longitude};
    [manager POST:@"http://fryer.ee.ucla.edu/rest/api/ip_address/post/" parameters:parameters
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
