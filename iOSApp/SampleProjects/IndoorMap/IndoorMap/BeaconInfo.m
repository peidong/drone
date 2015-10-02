#import "BeaconInfo.h"
#import "ESTBeaconManager.h"
@interface BeaconInfo () <ESTBeaconManagerDelegate>
@property (nonatomic, assign) ESTScanType scanType;
@property (nonatomic, strong) ESTBeaconManager *beaconManager;
@property (nonatomic, strong) ESTBeaconRegion *region;
@property (nonatomic, strong) NSArray *beaconsArray;
@end
@implementation BeaconInfo

- (id)initWithScanType:(ESTScanType)scanType{
    NSLog(@"(id)initWithScanType");
    self = [super init];
    if (self)
    {
        self.scanType = scanType;
    }
    return self;
}
//入口
- (void)start
{
    NSLog(@"(void)viewDidAppear:");
    
    self.beaconManager = [[ESTBeaconManager alloc] init];
    self.beaconManager.delegate = self;
    self.beaconManager.returnAllRangedBeaconsAtOnce = YES;
    self.region = [[ESTBeaconRegion alloc] initWithProximityUUID:ESTIMOTE_PROXIMITY_UUID
                                                      identifier:@"EstimoteSampleRegion"];
    if (self.scanType == ESTScanTypeBeacon)
    {
        [self startRangingBeacons];
    }
    else
    {
        [self.beaconManager startEstimoteBeaconsDiscoveryForRegion:self.region];
    }
}

- (void)beaconManager:(ESTBeaconManager *)manager didChangeAuthorizationStatus:(CLAuthorizationStatus)status
{
    NSLog(@"(void)beaconManager:(ESTBeaconManager *)manager didChangeAuthorizationStatus");
    if (self.scanType == ESTScanTypeBeacon)
    {
        [self startRangingBeacons];
    }
}
-(void)startRangingBeacons
{
    NSLog(@"(void)startRangingBeacons");
    if ([ESTBeaconManager authorizationStatus] == kCLAuthorizationStatusNotDetermined)
    {
        NSLog(@"if ([ESTBeaconManager authorizationStatus] == kCLAuthorizationStatusNotDetermined)");
        if (floor(NSFoundationVersionNumber) <= NSFoundationVersionNumber_iOS_7_1) {
            NSLog(@"if (floor(NSFoundationVersionNumber) <= NSFoundationVersionNumber_iOS_7_1)");
            [self.beaconManager startRangingBeaconsInRegion:self.region];
        } else {
            NSLog(@"if (floor(NSFoundationVersionNumber) > NSFoundationVersionNumber_iOS_7_1)");
            [self.beaconManager requestAlwaysAuthorization];
        }
    }
    else if([ESTBeaconManager authorizationStatus] == kCLAuthorizationStatusAuthorized)
    {
        NSLog(@"else if([ESTBeaconManager authorizationStatus] == kCLAuthorizationStatusAuthorized)");
        [self.beaconManager startRangingBeaconsInRegion:self.region];
    }
    else if([ESTBeaconManager authorizationStatus] == kCLAuthorizationStatusDenied)
    {
        NSLog(@"if([ESTBeaconManager authorizationStatus] == kCLAuthorizationStatusDenied)");
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Location Access Denied"
                                                        message:@"You have denied access to location services. Change this in app settings."
                                                       delegate:nil
                                              cancelButtonTitle:@"OK"
                                              otherButtonTitles: nil];
        [alert show];
    }
    else if([ESTBeaconManager authorizationStatus] == kCLAuthorizationStatusRestricted)
    {
        NSLog(@"else if([ESTBeaconManager authorizationStatus] == kCLAuthorizationStatusRestricted)");
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Location Not Available"
                                                        message:@"You have no access to location services."
                                                       delegate:nil
                                              cancelButtonTitle:@"OK"
                                              otherButtonTitles: nil];
        [alert show];
    }
}
//出口
- (void)stop
{
    NSLog(@"(void)viewDidDisappear");
    [self.beaconManager stopRangingBeaconsInRegion:self.region];
    [self.beaconManager stopEstimoteBeaconDiscovery];
    NSLog(@"viewDidDisappear: stopRangingBeaconsInRegion");
}
- (void)beaconManager:(ESTBeaconManager *)manager didRangeBeacons:(NSArray *)beacons inRegion:(ESTBeaconRegion *)region
{
    self.beaconsArray = beacons;
    NSLog(@"beacon number:%ld",self.beaconsArray.count);
    switch (self.beaconsArray.count) {
        case 0:
            NSLog(@"No iBeacons");
            break;
        case 1:
            NSLog(@"Beacon1-> Distance: %f",[[[self.beaconsArray objectAtIndex:0] distance]floatValue]);
            NSLog(@"Beacon1-> RSSI: %lddB",[[self.beaconsArray objectAtIndex:0] rssi]);
            NSLog(@"Beacon1-> Major: %@",[[self.beaconsArray objectAtIndex:0] major]);
            break;
        case 2:
            NSLog(@"Beacon1-> Distance: %f",[[[self.beaconsArray objectAtIndex:0] distance]floatValue]);
            NSLog(@"Beacon1-> RSSI: %lddB",[[self.beaconsArray objectAtIndex:0] rssi]);
            NSLog(@"Beacon1-> Major: %@",[[self.beaconsArray objectAtIndex:0] major]);
            NSLog(@"Beacon2-> Distance: %f",[[[self.beaconsArray objectAtIndex:1] distance]floatValue]);
            NSLog(@"Beacon2-> RSSI: %lddB",[[self.beaconsArray objectAtIndex:1] rssi]);
            NSLog(@"Beacon2-> Major: %@",[[self.beaconsArray objectAtIndex:1] major]);
            break;
        case 3:
            NSLog(@"Beacon1-> Distance: %f",[[[self.beaconsArray objectAtIndex:0] distance]floatValue]);
            NSLog(@"Beacon1-> RSSI: %lddB",[[self.beaconsArray objectAtIndex:0] rssi]);
            NSLog(@"Beacon1-> Major: %@",[[self.beaconsArray objectAtIndex:0] major]);
            NSLog(@"Beacon2-> Distance: %f",[[[self.beaconsArray objectAtIndex:1] distance]floatValue]);
            NSLog(@"Beacon2-> RSSI: %lddB",[[self.beaconsArray objectAtIndex:1] rssi]);
            NSLog(@"Beacon2-> Major: %@",[[self.beaconsArray objectAtIndex:1] major]);
            NSLog(@"Beacon3-> Distance: %f",[[[self.beaconsArray objectAtIndex:2] distance]floatValue]);
            NSLog(@"Beacon3-> RSSI: %lddB",[[self.beaconsArray objectAtIndex:2] rssi]);
            NSLog(@"Beacon3-> Major: %@",[[self.beaconsArray objectAtIndex:2] major]);
            break;
        case 4:
            NSLog(@"Beacon1-> Distance: %f",[[[self.beaconsArray objectAtIndex:0] distance]floatValue]);
            NSLog(@"Beacon1-> RSSI: %lddB",[[self.beaconsArray objectAtIndex:0] rssi]);
            NSLog(@"Beacon1-> Major: %@",[[self.beaconsArray objectAtIndex:0] major]);
            NSLog(@"Beacon2-> Distance: %f",[[[self.beaconsArray objectAtIndex:1] distance]floatValue]);
            NSLog(@"Beacon2-> RSSI: %lddB",[[self.beaconsArray objectAtIndex:1] rssi]);
            NSLog(@"Beacon2-> Major: %@",[[self.beaconsArray objectAtIndex:1] major]);
            NSLog(@"Beacon3-> Distance: %f",[[[self.beaconsArray objectAtIndex:2] distance]floatValue]);
            NSLog(@"Beacon3-> RSSI: %lddB",[[self.beaconsArray objectAtIndex:2] rssi]);
            NSLog(@"Beacon3-> Major: %@",[[self.beaconsArray objectAtIndex:2] major]);
            NSLog(@"Beacon4-> Distance: %f",[[[self.beaconsArray objectAtIndex:3] distance]floatValue]);
            NSLog(@"Beacon4-> RSSI: %lddB",[[self.beaconsArray objectAtIndex:3] rssi]);
            NSLog(@"Beacon4-> Major: %@",[[self.beaconsArray objectAtIndex:3] major]);
            break;
        default:
            break;
    }
    
}
- (void)beaconManager:(ESTBeaconManager *)manager didDiscoverBeacons:(NSArray *)beacons inRegion:(ESTBeaconRegion *)region
{
    NSLog(@"(void)beaconManager:(ESTBeaconManager *)manager didDiscoverBeacons:(NSArray *)beacons inRegion:(ESTBeaconRegion *)region");
    self.beaconsArray = beacons;
}
@end
