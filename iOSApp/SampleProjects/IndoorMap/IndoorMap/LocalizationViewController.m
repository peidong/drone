#import "LocalizationViewController.h"
#import "Canvas.h"
#import "FileManage.h"
#import "FCFileManager.h"
#import <CoreMotion/CoreMotion.h>


@interface LocalizationViewController () <UIAlertViewDelegate>

@end

@implementation LocalizationViewController


- (void)viewDidAppear:(BOOL)animated{
    [super viewDidAppear:animated];
    //    [self.navigationController.navigationBar setTranslucent:NO];
    
    NSOperationQueue *theQueue = [[NSOperationQueue alloc] init];
    
    _accelerometerData = [[CMAccelerometerData alloc] init];
    _motionManager = [[CMMotionManager alloc] init];
    _magnetometerData = [[CMMagnetometerData alloc]init];
    
    _motionManager.accelerometerUpdateInterval=0.5;
    [_motionManager startAccelerometerUpdatesToQueue:theQueue withHandler:^(CMAccelerometerData *accelerometerData, NSError *error) {
        
        _accelerometerData = _motionManager.accelerometerData;
        
        double x = _accelerometerData.acceleration.x;
        double y = _accelerometerData.acceleration.y;
        double z = _accelerometerData.acceleration.z;
        
//        NSLog(@"Accelerometer-------X: %.02f, Y: %.02f, z:%.02f", x, y, z);
    }];
    
    
    _motionManager.gyroUpdateInterval=0.5;
    [_motionManager startGyroUpdatesToQueue:theQueue withHandler:^(CMGyroData *gyroData, NSError *error) {
        _rotationRate = _motionManager.gyroData.rotationRate;
        double x = _rotationRate.x;
        double y = _rotationRate.y;
        double z = _rotationRate.z;

//        NSLog(@"Gyroscope----------------X: %.02f, Y: %.02f, Z: %.02f",x,y,z);
    }];
    
    _motionManager.magnetometerUpdateInterval=0.5;
    [_motionManager startMagnetometerUpdatesToQueue:theQueue withHandler:^(CMMagnetometerData *magnetometerData, NSError *error) {
        
        _magnetometerData = _motionManager.magnetometerData;
        
        double x = _magnetometerData.magneticField.x;
        double y = _magnetometerData.magneticField.y;
        double z = _magnetometerData.magneticField.z;
        
//        self.iBeaconInfoTextView.text = [NSString stringWithFormat:@"x=%.f\ny=%.f\nz=%.f",x,y,z];
        /**
         (y>0) heading = 90 - atan2(x,y)*180 / pi
         (y<0) heading = 270 - atan2(x,y)*180 / pi
         (y=0, x<0) heading = 180.0
         (y=0, x>0) heading = 0.0
         */

//        NSLog(@"Magnetometer-------X: %.f, Y: %.f, z:%.f", x, y, z);
//        if (y<=90 &&y>=60) {
//            if (x>=-400) {
//                NSLog(@"West");
//            }
//            if (x<=-400) {
//                NSLog(@"East");
//            }
//        }
//        if (y>=100 && y<=115) {
//            NSLog(@"North");
//        }
    }];

    
//iBeacon
    self.scanType =ESTScanTypeBeacon;
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


- (void)viewDidDisappear:(BOOL)animated{
    [super viewDidDisappear:animated];
    [FileManage downloadIndoorMapPointsFile];
    [FileManage downloadiBeaconPointsFile];

    //iBeacon
    [self.beaconManager stopRangingBeaconsInRegion:self.region];
    [self.beaconManager stopEstimoteBeaconDiscovery];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    self.numberOfBeacons = 0;
    self.beaconsDictionary = [[NSMutableDictionary alloc]initWithCapacity:4];
    self.beaconsViewDictionary = [[NSMutableDictionary alloc]init];
    self.beaconsPositionDictionaryX = [[NSMutableDictionary alloc]init];
    self.beaconsPositionDictionaryY = [[NSMutableDictionary alloc]init];
    self.whichBeaconIsLongPressed = [[NSString alloc]init];
    
    //    [self.canvasDraw removeFromSuperview];
    //添加画布类
    Canvas *canvas = [[Canvas alloc] initWithFrame:self.RoomPlanView.bounds];
    //    self.canvasDraw = canvas;
    [self.RoomPlanView addSubview:canvas];

    

    
    /**
     add beacon view

     
     */
    
    NSMutableArray *pointsBeaconArray = [FileManage toNSArrayFromiBeaconPointsFile];
    for (int i=0; i < pointsBeaconArray.count; i++) {
        NSValue *pointBeaconTemp = [pointsBeaconArray objectAtIndex:i];
        CGPoint pointBeacon = [pointBeaconTemp CGPointValue];
        UIView *beaconView = [[UIView alloc] initWithFrame:CGRectMake(pointBeacon.x-0.5*BEACON_X,pointBeacon.y-0.5*BEACON_Y,BEACON_X,BEACON_Y)];
        
        NSString *beacon_image = [[NSString alloc]init];
        switch (i) {
            case 0:
                beacon_image = @"beacon_blue";
                [self.beaconsPositionDictionaryX setValue:[NSNumber numberWithFloat:pointBeacon.x] forKey:@"blue1"];
                [self.beaconsPositionDictionaryY setValue:[NSNumber numberWithFloat:pointBeacon.y] forKey:@"blue1"];
                break;
            case 1:
                beacon_image = @"beacon_blue";
                [self.beaconsPositionDictionaryX setValue:[NSNumber numberWithFloat:pointBeacon.x] forKey:@"blue2"];
                [self.beaconsPositionDictionaryY setValue:[NSNumber numberWithFloat:pointBeacon.y] forKey:@"blue2"];
                break;
            case 2:
                beacon_image = @"beacon_green";
                [self.beaconsPositionDictionaryX setValue:[NSNumber numberWithFloat:pointBeacon.x] forKey:@"green"];
                [self.beaconsPositionDictionaryY setValue:[NSNumber numberWithFloat:pointBeacon.y] forKey:@"green"];
                break;
            case 3:
                beacon_image = @"beacon_purple";
                [self.beaconsPositionDictionaryX setValue:[NSNumber numberWithFloat:pointBeacon.x] forKey:@"purple"];
                [self.beaconsPositionDictionaryY setValue:[NSNumber numberWithFloat:pointBeacon.y] forKey:@"purple"];
                break;
            default:
                break;
        }
        UIImage *imageBeacon = [UIImage imageNamed:beacon_image];
        UIImageView *imageBeaconView = [[UIImageView alloc] initWithImage:imageBeacon];
        
        //specify the frame of the imageView in the superview , here it will fill the superview
        imageBeaconView.frame = beaconView.bounds;
        
        // add the imageview to the superview
        [beaconView addSubview:imageBeaconView];
        
        //add the view to the main view
        
        [self.RoomPlanView addSubview:beaconView];
        
        unsigned long int numberOfBeacons = self.beaconsViewDictionary.count+1;
        self.numberOfBeacons++;
        [self.beaconsViewDictionary setValue:beaconView forKey:[NSString stringWithFormat:@"%lu",numberOfBeacons]];
    }
    
    /**
     add location view
     */
    CGFloat xWidth =15;
    CGFloat yWidth =15;
    UIView *locationView = [[UIView alloc] initWithFrame:CGRectMake(100-0.5*xWidth,100-0.5*yWidth,xWidth,yWidth)];
    UIImage *image = [UIImage imageNamed:@"dotImage"];
    UIImageView *imageView = [[UIImageView alloc] initWithImage:image];
    //specify the frame of the imageView in the superview , here it will fill the superview
    imageView.frame = locationView.bounds;
    
    // add the imageview to the superview
    [locationView addSubview:imageView];
    
    //add the view to the main view
    
    [self.RoomPlanView addSubview:locationView];
    self.locationView = locationView;
}




- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


/**
 Scale and rotation transforms are applied relative to the layer's anchor point this method moves a gesture recognizer's view's anchor point between the user's fingers.
 */
- (void)adjustAnchorPointForGestureRecognizer:(UIGestureRecognizer *)gestureRecognizer
{
    if (gestureRecognizer.state == UIGestureRecognizerStateBegan) {
        UIView *piece = gestureRecognizer.view;
        CGPoint locationInView = [gestureRecognizer locationInView:piece];
        CGPoint locationInSuperview = [gestureRecognizer locationInView:piece.superview];
        
        piece.layer.anchorPoint = CGPointMake(locationInView.x / piece.bounds.size.width, locationInView.y / piece.bounds.size.height);
        piece.center = locationInSuperview;
    }
}

- (IBAction)pinchGesture:(UIPinchGestureRecognizer *)gestureRecognizer {
    [self adjustAnchorPointForGestureRecognizer:gestureRecognizer];
    
    if ([gestureRecognizer state] == UIGestureRecognizerStateBegan || [gestureRecognizer state] == UIGestureRecognizerStateChanged) {
        NSLog(@"Pinch: %f",gestureRecognizer.scale);
        [gestureRecognizer view].transform = CGAffineTransformScale([[gestureRecognizer view] transform], [gestureRecognizer scale], [gestureRecognizer scale]);
        [gestureRecognizer setScale:1];
    }
}

- (IBAction)panGesture:(UIPanGestureRecognizer *)gestureRecognizer {
    UIView *piece = [gestureRecognizer view];
    
    [self adjustAnchorPointForGestureRecognizer:gestureRecognizer];
    
    if ([gestureRecognizer state] == UIGestureRecognizerStateBegan || [gestureRecognizer state] == UIGestureRecognizerStateChanged) {
        CGPoint translation = [gestureRecognizer translationInView:[piece superview]];
        
        [piece setCenter:CGPointMake([piece center].x + translation.x, [piece center].y + translation.y)];
        [gestureRecognizer setTranslation:CGPointZero inView:[piece superview]];
    }
}

- (IBAction)rotationGesture:(UIRotationGestureRecognizer *)gestureRecognizer {
    [self adjustAnchorPointForGestureRecognizer:gestureRecognizer];
    
    if ([gestureRecognizer state] == UIGestureRecognizerStateBegan || [gestureRecognizer state] == UIGestureRecognizerStateChanged) {
        [gestureRecognizer view].transform = CGAffineTransformRotate([[gestureRecognizer view] transform], [gestureRecognizer rotation]);
        [gestureRecognizer setRotation:0];
    }
}

- (IBAction)longPressGesture:(UILongPressGestureRecognizer *)gestureRecognizer {
    if ([gestureRecognizer state] == UIGestureRecognizerStateBegan) {
        
        [self becomeFirstResponder];
        self.pieceForLongPress = [gestureRecognizer view];
        self.longPressPoint = [gestureRecognizer locationInView:gestureRecognizer.view];
        
        if (self.longPressPoint.x > 165 && self.longPressPoint.x <185 && self.longPressPoint.y > 90 && self.longPressPoint.y <110) {
            self.whichBeaconIsLongPressed = @"blue1";
            NSLog(@"blue1");
        }

        if (self.longPressPoint.x > 240 && self.longPressPoint.x <260 && self.longPressPoint.y > 240 && self.longPressPoint.y <260) {
            NSLog(@"blue2");
            self.whichBeaconIsLongPressed = @"blue2";
        }
        
        if (self.longPressPoint.x > 165 && self.longPressPoint.x <185 && self.longPressPoint.y > 390 && self.longPressPoint.y <410) {
            NSLog(@"green");
            self.whichBeaconIsLongPressed = @"green";
        }
        
        if (self.longPressPoint.x > 90 && self.longPressPoint.x <110 && self.longPressPoint.y > 240 && self.longPressPoint.y <260) {
            NSLog(@"purple");
            self.whichBeaconIsLongPressed = @"purple";
        }
        
        UIAlertView *alertView= [[UIAlertView alloc]initWithTitle:@"iBeacon Info" message:[NSString stringWithFormat:@"Name:%@\nUUID:%@\nMajor:%d\nMinor:%d\nRSSI:%lddB\nDistance:%.02f",self.whichBeaconIsLongPressed,[[[self.beaconsDictionary objectForKey:self.whichBeaconIsLongPressed]proximityUUID]UUIDString],[[[self.beaconsDictionary objectForKey:self.whichBeaconIsLongPressed]major]intValue],[[[self.beaconsDictionary objectForKey:self.whichBeaconIsLongPressed]minor]intValue],[[self.beaconsDictionary objectForKey:self.whichBeaconIsLongPressed]rssi],[[[self.beaconsDictionary objectForKey:self.whichBeaconIsLongPressed]distance]floatValue]] delegate:self cancelButtonTitle:@"Cancel" otherButtonTitles:@"Send", nil];
        [alertView show];
        /*
         Set up the menu.
         */
//        NSString *menuItemTitle = NSLocalizedString(@"Put iBeacon", @"Put iBeacon here");
//        UIMenuItem *resetMenuItem = [[UIMenuItem alloc] initWithTitle:menuItemTitle action:@selector(putBeacon:)];
//        
//        UIMenuController *menuController = [UIMenuController sharedMenuController];
//        [menuController setMenuItems:@[resetMenuItem]];
//        
//        CGPoint location = [gestureRecognizer locationInView:[gestureRecognizer view]];
//        CGRect menuLocation = CGRectMake(location.x, location.y, 0, 0);
//        [menuController setTargetRect:menuLocation inView:[gestureRecognizer view]];
//        
//        [menuController setMenuVisible:YES animated:YES];
    }
}

-(void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex{
    if (buttonIndex == 0) return;
    NSLog(@"Name:%@\nUUID:%@\nMajor:%d\nMinor:%d\nRSSI:%lddB\nDistance:%.02f",self.whichBeaconIsLongPressed,[[[self.beaconsDictionary objectForKey:self.whichBeaconIsLongPressed]proximityUUID]UUIDString],[[[self.beaconsDictionary objectForKey:self.whichBeaconIsLongPressed]major]intValue],[[[self.beaconsDictionary objectForKey:self.whichBeaconIsLongPressed]minor]intValue],[[self.beaconsDictionary objectForKey:self.whichBeaconIsLongPressed]rssi],[[[self.beaconsDictionary objectForKey:self.whichBeaconIsLongPressed]distance]floatValue]);
}

- (void)putBeacon:(UIMenuController *)controller
{
    NSLog(@"Put Beacon");
    
    CGFloat xWidth =25;
    CGFloat yWidth =37.5;
    UIView *beaconView = [[UIView alloc] initWithFrame:CGRectMake(self.longPressPoint.x-0.5*xWidth,self.longPressPoint.y-0.5*yWidth,xWidth,yWidth)];
    UIImage *image = [UIImage imageNamed:@"beacon_linearnie"];
    UIImageView *imageView = [[UIImageView alloc] initWithImage:image];
    
    //specify the frame of the imageView in the superview , here it will fill the superview
    imageView.frame = beaconView.bounds;
    
    // add the imageview to the superview
    [beaconView addSubview:imageView];
    
    //add the view to the main view
    
    [self.RoomPlanView addSubview:beaconView];
    
    
    NSString *iBeaconPointX = [NSString stringWithFormat:@"%.f", round(self.longPressPoint.x-0.5*xWidth)];
    NSString *iBeaconPointY = [NSString stringWithFormat:@"%.f", round(self.longPressPoint.y-0.5*yWidth)];
    NSString *iBeaconPointTemp = [iBeaconPointX stringByAppendingString:@","];
    NSString *iBeaconPoint = [iBeaconPointTemp stringByAppendingString:iBeaconPointY];
    [FCFileManager writeFileAtDocumentPathWithFileName:@"iBeaconPoints.txt" Content:iBeaconPoint append:true newline:true];
    
    beaconView.tag = (self.numberOfBeacons + 1);
    switch (self.numberOfBeacons) {
        case 0:
            self.beaconView1 = beaconView;
            break;
        case 1:
            self.beaconView2 = beaconView;
            break;
        case 2:
            self.beaconView3 = beaconView;
            break;
        case 3:
            self.beaconView4 = beaconView;
            break;
        case 4:
            self.beaconView5 = beaconView;
            break;
        case 5:
            self.beaconView6 = beaconView;
            break;
        case 6:
            self.beaconView7 = beaconView;
            break;
        case 7:
            self.beaconView8 = beaconView;
            break;
        case 8:
            self.beaconView9 = beaconView;
            break;
        case 9:
            self.beaconView10 = beaconView;
            break;
        case 10:
            self.beaconView11 = beaconView;
            break;
        case 11:
            self.beaconView12 = beaconView;
            break;
        case 12:
            self.beaconView13 = beaconView;
            break;
        case 13:
            self.beaconView14 = beaconView;
            break;
        case 14:
            self.beaconView15 = beaconView;
            break;
        case 15:
            self.beaconView16 = beaconView;
            break;
        case 16:
            self.beaconView17 = beaconView;
            break;
        case 17:
            self.beaconView18 = beaconView;
            break;
        case 18:
            self.beaconView19 = beaconView;
            break;
        case 19:
            self.beaconView20 = beaconView;
            break;
        default:
            break;
    }
    self.numberOfBeacons++;
    [self.beaconsViewDictionary setValue:beaconView forKey:[NSString stringWithFormat:@"%d",self.numberOfBeacons]];
    NSLog(@"%@",self.beaconsViewDictionary);
}

/**
 Animate back to the default anchor point and transform.
 */
- (void)resetPiece:(UIMenuController *)controller
{
    NSLog(@"resetPiece");
    UIView *pieceForLongPress = self.pieceForLongPress;
    
    CGPoint centerPoint = CGPointMake(CGRectGetMidX(pieceForLongPress.bounds), CGRectGetMidY(pieceForLongPress.bounds));
    CGPoint locationInSuperview = [pieceForLongPress convertPoint:centerPoint toView:[pieceForLongPress superview]];
    
    [[pieceForLongPress layer] setAnchorPoint:CGPointMake(0.5, 0.5)];
    [pieceForLongPress setCenter:locationInSuperview];
    
    [UIView beginAnimations:nil context:nil];
    [pieceForLongPress setTransform:CGAffineTransformIdentity];
    [UIView commitAnimations];
}

- (IBAction)ResetMap:(UIButton *)sender {
    NSLog(@"Reset Map");
    self.pieceForLongPress = self.RoomPlanView;
    UIView *pieceForLongPress = self.pieceForLongPress;
    
    CGPoint centerPoint = CGPointMake(CGRectGetMidX(pieceForLongPress.bounds), CGRectGetMidY(pieceForLongPress.bounds));
    CGPoint locationInSuperview = [pieceForLongPress convertPoint:centerPoint toView:[pieceForLongPress superview]];
    
    [[pieceForLongPress layer] setAnchorPoint:CGPointMake(0.5, 0.5)];
    [pieceForLongPress setCenter:locationInSuperview];
    
    [UIView beginAnimations:nil context:nil];
    [pieceForLongPress setTransform:CGAffineTransformIdentity];
    [UIView commitAnimations];
}

- (IBAction)Test:(UIButton *)sender {
    NSLog(@"Test");
//    [UIView beginAnimations:nil context:nil];
//    CGRect tempframe = self.locationView.frame;
//    tempframe.origin.y += 10;
//    tempframe.origin.x += 10;
//    self.locationView.frame = tempframe;
    
    
//    UIView *tempView = [self.beaconsViewDictionary objectForKey:@"1"];
//    CGRect tempframe = tempView.frame;
//    tempframe.origin.y += 10;
//    tempframe.origin.x += 10;
//    tempView.frame = tempframe;
//    [self.beaconsViewDictionary setValue:tempView forKey:@"1"];
//    
//    [UIView commitAnimations];

//    NSMutableDictionary *beaconsDictionaryTemp = [[NSMutableDictionary alloc]initWithDictionary:self.beaconsDictionary];
//    int startPointX = (([[[beaconsDictionaryTemp objectForKey:@"blue1"]distance]floatValue]-DISTANCE_ERROR)/DISTANCE_PER_PIXEL)+[[self.beaconsPositionDictionaryX objectForKey:@"blue1"]floatValue];
//    int startPointY = (([[[beaconsDictionaryTemp objectForKey:@"blue1"]distance]floatValue]-DISTANCE_ERROR)/DISTANCE_PER_PIXEL)+[[self.beaconsPositionDictionaryY objectForKey:@"blue1"]floatValue];
//    int endPointX = (([[[beaconsDictionaryTemp objectForKey:@"blue1"]distance]floatValue]+DISTANCE_ERROR)/DISTANCE_PER_PIXEL)+[[self.beaconsPositionDictionaryX objectForKey:@"blue1"]floatValue];
//    int endPointY = (([[[beaconsDictionaryTemp objectForKey:@"blue1"]distance]floatValue]+DISTANCE_ERROR)/DISTANCE_PER_PIXEL)+[[self.beaconsPositionDictionaryY objectForKey:@"blue1"]floatValue];
//    NSLog(@"%d,%d,%d,%d",startPointX,startPointY,endPointX,endPointY);
//    
//    for (int x=startPointX; (x<=self.RoomPlanView.frame.size.width && x<=endPointX); x++) {
//        for (int y=0; (y<=self.RoomPlanView.frame.size.height && y<=endPointY); y++) {
//
//            
//            float distanceBluePixel1 = sqrt(pow((x-[[self.beaconsPositionDictionaryX objectForKey:@"blue1"]floatValue]), 2)+pow((y-[[self.beaconsPositionDictionaryX objectForKey:@"blue1"]floatValue]), 2));
//            float distanceBluePixel2 = sqrt(pow((x-[[self.beaconsPositionDictionaryX objectForKey:@"blue2"]floatValue]), 2)+pow((y-[[self.beaconsPositionDictionaryX objectForKey:@"blue2"]floatValue]), 2));
//            float distanceGreenPixel = sqrt(pow((x-[[self.beaconsPositionDictionaryX objectForKey:@"green"]floatValue]), 2)+pow((y-[[self.beaconsPositionDictionaryX objectForKey:@"green"]floatValue]), 2));
//            float distancePurplePixel = sqrt(pow((x-[[self.beaconsPositionDictionaryX objectForKey:@"purple"]floatValue]), 2)+pow((y-[[self.beaconsPositionDictionaryX objectForKey:@"purple"]floatValue]), 2));
//            
//            float distanceBlue1 = distanceBluePixel1 * DISTANCE_PER_PIXEL;
//            float distanceBlue2 = distanceBluePixel2 * DISTANCE_PER_PIXEL;
//            float distanceGreen = distanceGreenPixel * DISTANCE_PER_PIXEL;
//            float distancePurple = distancePurplePixel * DISTANCE_PER_PIXEL;
//            
//            NSLog(@"x=%d, y=%d, distanceBlue1=%f",x,y,distanceBlue1);
//            NSLog(@"x=%d, y=%d, distanceBlue2=%f",x,y,distanceBlue2);
//            NSLog(@"x=%d, y=%d, distanceGreen=%f",x,y,distanceGreen);
//            NSLog(@"x=%d, y=%d, distancePurple=%f",x,y,distancePurple);
//            
//            if (distanceBlue1<([[[beaconsDictionaryTemp objectForKey:@"blue1"]distance]floatValue]+DISTANCE_ERROR)) {
//                NSLog(@"距离1");
//                if (distanceBlue2<([[[beaconsDictionaryTemp objectForKey:@"blue2"]distance]floatValue]+DISTANCE_ERROR)){
//                    NSLog(@"距离2");
//                    if (distanceGreen<([[[beaconsDictionaryTemp objectForKey:@"green"]distance]floatValue]+DISTANCE_ERROR)){
//                        NSLog(@"距离3");
//                        if (distancePurple<([[[beaconsDictionaryTemp objectForKey:@"purple"]distance]floatValue]+DISTANCE_ERROR)){
//                            NSLog(@"x=%d, y=%d, distancePurple=%f",x,y,distancePurple);
//                            [UIView beginAnimations:nil context:nil];
//                            CGRect tempframe = self.locationView.frame;
//                            tempframe.origin.y = y;
//                            tempframe.origin.x = x;
//                            self.locationView.frame = tempframe;
//                            [UIView commitAnimations];
//                        }
//                    }
//                }
//            }
//            
//            
//        }
//    }
}


// UIMenuController requires that we can become first responder or it won't display
- (BOOL)canBecomeFirstResponder
{
    return YES;
}

- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer
{
    // If the gesture recognizers's view isn't one of our pieces, don't allow simultaneous recognition.
    if (gestureRecognizer.view != self.RoomPlanView) {
        return NO;
    }
    
    // If the gesture recognizers are on different views, don't allow simultaneous recognition.
    if (gestureRecognizer.view != otherGestureRecognizer.view) {
        return NO;
    }
    
    // If either of the gesture recognizers is the long press, don't allow simultaneous recognition.
    if ([gestureRecognizer isKindOfClass:[UILongPressGestureRecognizer class]] || [otherGestureRecognizer isKindOfClass:[UILongPressGestureRecognizer class]]) {
        return NO;
    }
    return YES;
}

- (id)initWithScanType:(ESTScanType)scanType{
    NSLog(@"(id)initWithScanType");
    self = [super init];
    if (self)
    {
        self.scanType = scanType;
    }
    return self;
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

- (void)beaconManager:(ESTBeaconManager *)manager didRangeBeacons:(NSArray *)beacons inRegion:(ESTBeaconRegion *)region
{
    self.beaconsArray = beacons;
//    NSLog(@"beacon number:%ld",self.beaconsArray.count);
    if (self.beaconsArray.count > 0) {
        for (int i=0; i < self.beaconsArray.count; i++) {
            switch ([[[self.beaconsArray objectAtIndex:i] major]intValue]) {
                case 55555:
                    [self.beaconsDictionary setValue:[self.beaconsArray objectAtIndex:i] forKey:@"blue1"];
                    break;
                case 23697:
                    [self.beaconsDictionary setValue:[self.beaconsArray objectAtIndex:i] forKey:@"blue2"];
                    break;
                case 55950:
                    [self.beaconsDictionary setValue:[self.beaconsArray objectAtIndex:i] forKey:@"green"];
                    break;
                case 51634:
                    [self.beaconsDictionary setValue:[self.beaconsArray objectAtIndex:i] forKey:@"purple"];
                    break;
                default:
                    break;
            }
        }
    }
    
    self.iBeaconInfoTextView.text = [NSString stringWithFormat:@"blue1's distance: %.02f\nblue2's distance: %.02f\ngreen's distance: %.02f\npurple's distance: %.02f",[[[self.beaconsDictionary objectForKey:@"blue1"]distance]floatValue],[[[self.beaconsDictionary objectForKey:@"blue2"]distance]floatValue],[[[self.beaconsDictionary objectForKey:@"green"]distance]floatValue],[[[self.beaconsDictionary objectForKey:@"purple"]distance]floatValue]];
//    NSLog(@"blue1's distance: %f",[[[self.beaconsDictionary objectForKey:@"blue1"]distance]floatValue]);
//    NSLog(@"blue2's distance: %f",[[[self.beaconsDictionary objectForKey:@"blue2"]distance]floatValue]);
//    NSLog(@"green's distance: %f",[[[self.beaconsDictionary objectForKey:@"green"]distance]floatValue]);
//    NSLog(@"purple's distance: %f",[[[self.beaconsDictionary objectForKey:@"purple"]distance]floatValue]);
    
    

    CGRect tempframe = self.locationView.frame;
    if ([[[self.beaconsDictionary objectForKey:@"blue1"]distance]floatValue] < 0.5 && [[[self.beaconsDictionary objectForKey:@"blue1"]distance]floatValue] > 0) {
        tempframe.origin.y = [[self.beaconsPositionDictionaryY objectForKey:@"blue1"]floatValue]+([[[self.beaconsDictionary objectForKey:@"blue1"]distance]floatValue]/DISTANCE_PER_PIXEL);
        tempframe.origin.x = [[self.beaconsPositionDictionaryX objectForKey:@"blue1"]floatValue];
    }else if ([[[self.beaconsDictionary objectForKey:@"blue2"]distance]floatValue] < 0.5 && [[[self.beaconsDictionary objectForKey:@"blue2"]distance]floatValue] > 0){
        tempframe.origin.y = [[self.beaconsPositionDictionaryY objectForKey:@"blue2"]floatValue];
        tempframe.origin.x = [[self.beaconsPositionDictionaryX objectForKey:@"blue2"]floatValue]-([[[self.beaconsDictionary objectForKey:@"blue2"]distance]floatValue]/DISTANCE_PER_PIXEL);
    }else if ([[[self.beaconsDictionary objectForKey:@"green"]distance]floatValue] < 0.5 && [[[self.beaconsDictionary objectForKey:@"green"]distance]floatValue] > 0){
        tempframe.origin.y = [[self.beaconsPositionDictionaryY objectForKey:@"green"]floatValue]-([[[self.beaconsDictionary objectForKey:@"green"]distance]floatValue]/DISTANCE_PER_PIXEL);
        tempframe.origin.x = [[self.beaconsPositionDictionaryX objectForKey:@"green"]floatValue];
    }else if ([[[self.beaconsDictionary objectForKey:@"purple"]distance]floatValue] < 0.5 && [[[self.beaconsDictionary objectForKey:@"purple"]distance]floatValue] > 0){
        tempframe.origin.y = [[self.beaconsPositionDictionaryY objectForKey:@"purple"]floatValue];
        tempframe.origin.x = [[self.beaconsPositionDictionaryX objectForKey:@"purple"]floatValue]+([[[self.beaconsDictionary objectForKey:@"purple"]distance]floatValue]/DISTANCE_PER_PIXEL);
    }else {
        
        if ([[[self.beaconsDictionary objectForKey:@"blue1"]distance]floatValue] > 0 && [[[self.beaconsDictionary objectForKey:@"green"]distance]floatValue] > 0) {
            if ([[[self.beaconsDictionary objectForKey:@"blue1"]distance]floatValue] < [[[self.beaconsDictionary objectForKey:@"green"]distance]floatValue]) {
                tempframe.origin.y = [[self.beaconsPositionDictionaryY objectForKey:@"blue1"]floatValue]+([[[self.beaconsDictionary objectForKey:@"blue1"]distance]floatValue]/DISTANCE_PER_PIXEL);
                if (tempframe.origin.y > 250){
                    tempframe.origin.y =245;
                }
            }else if ([[[self.beaconsDictionary objectForKey:@"blue1"]distance]floatValue] > [[[self.beaconsDictionary objectForKey:@"green"]distance]floatValue]){
                tempframe.origin.y = [[self.beaconsPositionDictionaryY objectForKey:@"green"]floatValue]-([[[self.beaconsDictionary objectForKey:@"green"]distance]floatValue]/DISTANCE_PER_PIXEL);
                if (tempframe.origin.y < 250){
                    tempframe.origin.y =255;
                }
            }
        }
        
        if ([[[self.beaconsDictionary objectForKey:@"blue2"]distance]floatValue] > 0 && [[[self.beaconsDictionary objectForKey:@"purple"]distance]floatValue] > 0) {
            if ([[[self.beaconsDictionary objectForKey:@"purple"]distance]floatValue] < [[[self.beaconsDictionary objectForKey:@"blue2"]distance]floatValue]) {
                tempframe.origin.x = [[self.beaconsPositionDictionaryX objectForKey:@"purple"]floatValue]+([[[self.beaconsDictionary objectForKey:@"purple"]distance]floatValue]/DISTANCE_PER_PIXEL);
                if (tempframe.origin.x > 175) {
                    tempframe.origin.x = 170;
                }
            }else if ([[[self.beaconsDictionary objectForKey:@"purple"]distance]floatValue] > [[[self.beaconsDictionary objectForKey:@"blue2"]distance]floatValue]){
                tempframe.origin.x = [[self.beaconsPositionDictionaryX objectForKey:@"blue2"]floatValue]-([[[self.beaconsDictionary objectForKey:@"blue2"]distance]floatValue]/DISTANCE_PER_PIXEL);
                if (tempframe.origin.x < 175) {
                    tempframe.origin.x = 180;
                }
            }
        }
    }
    
    if (tempframe.origin.x >= 250) {
        tempframe.origin.x = 245;
    }
    if (tempframe.origin.x < 100) {
        tempframe.origin.x = 105;
    }
    if (tempframe.origin.y >= 400) {
        tempframe.origin.y = 395;
    }
    if (tempframe.origin.y < 100) {
        tempframe.origin.y = 100;
    }
    
    
    [UIView beginAnimations:nil context:nil];    
    self.locationView.frame = tempframe;
    [UIView commitAnimations];


//    for (int x=0; x<=self.RoomPlanView.frame.size.width; x++) {
//        for (int y=0; y<=self.RoomPlanView.frame.size.height; y++) {
//    
//    
//            float distanceBluePixel1 = sqrt(pow((x-[[self.beaconsPositionDictionaryX objectForKey:@"blue1"]floatValue]), 2)+pow((y-[[self.beaconsPositionDictionaryX objectForKey:@"blue1"]floatValue]), 2));
//            float distanceBluePixel2 = sqrt(pow((x-[[self.beaconsPositionDictionaryX objectForKey:@"blue2"]floatValue]), 2)+pow((y-[[self.beaconsPositionDictionaryX objectForKey:@"blue2"]floatValue]), 2));
//            float distanceGreenPixel = sqrt(pow((x-[[self.beaconsPositionDictionaryX objectForKey:@"green"]floatValue]), 2)+pow((y-[[self.beaconsPositionDictionaryX objectForKey:@"green"]floatValue]), 2));
//            float distancePurplePixel = sqrt(pow((x-[[self.beaconsPositionDictionaryX objectForKey:@"purple"]floatValue]), 2)+pow((y-[[self.beaconsPositionDictionaryX objectForKey:@"purple"]floatValue]), 2));
//            
//            float distanceBlue1 = distanceBluePixel1 * DISTANCE_PER_PIXEL;
//            float distanceBlue2 = distanceBluePixel2 * DISTANCE_PER_PIXEL;
//            float distanceGreen = distanceGreenPixel * DISTANCE_PER_PIXEL;
//            float distancePurple = distancePurplePixel * DISTANCE_PER_PIXEL;
//
//            if (distanceBlue1<([[[self.beaconsDictionary objectForKey:@"blue1"]distance]floatValue]+DISTANCE_ERROR)) {
////                NSLog(@"x=%d, y=%d, distanceBlue1=%f",x,y,distanceBlue1);
//                if (distanceBlue2<([[[self.beaconsDictionary objectForKey:@"blue2"]distance]floatValue]+DISTANCE_ERROR)){
////                    NSLog(@"x=%d, y=%d, distanceBlue2=%f",x,y,distanceBlue2);
//                    if (distanceGreen<([[[self.beaconsDictionary objectForKey:@"green"]distance]floatValue]+DISTANCE_ERROR)){
////                        NSLog(@"x=%d, y=%d, distanceGreen=%f",x,y,distanceGreen);
//                        if (distancePurple<([[[self.beaconsDictionary objectForKey:@"purple"]distance]floatValue]+DISTANCE_ERROR)){
//                            NSLog(@"x=%d, y=%d, distancePurple=%f",x,y,distancePurple);
//                        }
//                    }
//                }
//            }
//    
//    
//        }
//    }
//
//    switch (self.beaconsArray.count) {
//        case 0:
//            NSLog(@"No iBeacons");
//            break;
//        case 1:
//            NSLog(@"Beacon1-> Distance: %f",[[[self.beaconsArray objectAtIndex:0] distance]floatValue]);
//            NSLog(@"Beacon1-> RSSI: %lddB",[[self.beaconsArray objectAtIndex:0] rssi]);
//            NSLog(@"Beacon1-> Major: %@",[[self.beaconsArray objectAtIndex:0] major]);
//            NSLog(@"Beacon1-> Minor: %@",[[self.beaconsArray objectAtIndex:0] minor]);
//            NSLog(@"Beacon1-> UUID: %@",[[self.beaconsArray objectAtIndex:0] proximityUUID]);
//            NSLog(@"Beacon1-> Mac Address: %@",[[self.beaconsArray objectAtIndex:0] macAddress]);
//            NSLog(@"Beacon1-> RSSI value at 1m: %@",[[self.beaconsArray objectAtIndex:0] measuredPower]);            break;
//        case 2:
//            NSLog(@"Beacon1-> Distance: %f",[[[self.beaconsArray objectAtIndex:0] distance]floatValue]);
//            NSLog(@"Beacon1-> RSSI: %lddB",[[self.beaconsArray objectAtIndex:0] rssi]);
//            NSLog(@"Beacon1-> Major: %@",[[self.beaconsArray objectAtIndex:0] major]);
//            NSLog(@"Beacon2-> Distance: %f",[[[self.beaconsArray objectAtIndex:1] distance]floatValue]);
//            NSLog(@"Beacon2-> RSSI: %lddB",[[self.beaconsArray objectAtIndex:1] rssi]);
//            NSLog(@"Beacon2-> Major: %@",[[self.beaconsArray objectAtIndex:1] major]);
//            break;
//        case 3:
//            NSLog(@"Beacon1-> Distance: %f",[[[self.beaconsArray objectAtIndex:0] distance]floatValue]);
//            NSLog(@"Beacon1-> RSSI: %lddB",[[self.beaconsArray objectAtIndex:0] rssi]);
//            NSLog(@"Beacon1-> Major: %@",[[self.beaconsArray objectAtIndex:0] major]);
//            NSLog(@"Beacon2-> Distance: %f",[[[self.beaconsArray objectAtIndex:1] distance]floatValue]);
//            NSLog(@"Beacon2-> RSSI: %lddB",[[self.beaconsArray objectAtIndex:1] rssi]);
//            NSLog(@"Beacon2-> Major: %@",[[self.beaconsArray objectAtIndex:1] major]);
//            NSLog(@"Beacon3-> Distance: %f",[[[self.beaconsArray objectAtIndex:2] distance]floatValue]);
//            NSLog(@"Beacon3-> RSSI: %lddB",[[self.beaconsArray objectAtIndex:2] rssi]);
//            NSLog(@"Beacon3-> Major: %@",[[self.beaconsArray objectAtIndex:2] major]);
//            break;
//        case 4:
//            NSLog(@"Beacon1-> Distance: %f",[[[self.beaconsArray objectAtIndex:0] distance]floatValue]);
//            NSLog(@"Beacon1-> RSSI: %lddB",[[self.beaconsArray objectAtIndex:0] rssi]);
//            NSLog(@"Beacon1-> Major: %@",[[self.beaconsArray objectAtIndex:0] major]);
//            NSLog(@"Beacon1-> Minor: %@",[[self.beaconsArray objectAtIndex:0] minor]);
//            NSLog(@"Beacon1-> UUID: %@",[[self.beaconsArray objectAtIndex:0] proximityUUID]);
//            NSLog(@"Beacon1-> Mac Address: %@",[[self.beaconsArray objectAtIndex:0] macAddress]);
//            NSLog(@"Beacon1-> RSSI value at 1m: %@",[[self.beaconsArray objectAtIndex:0] measuredPower]);
//            NSLog(@"Beacon2-> Distance: %f",[[[self.beaconsArray objectAtIndex:1] distance]floatValue]);
//            NSLog(@"Beacon2-> RSSI: %lddB",[[self.beaconsArray objectAtIndex:1] rssi]);
//            NSLog(@"Beacon2-> Major: %@",[[self.beaconsArray objectAtIndex:1] major]);
//            NSLog(@"Beacon3-> Distance: %f",[[[self.beaconsArray objectAtIndex:2] distance]floatValue]);
//            NSLog(@"Beacon3-> RSSI: %lddB",[[self.beaconsArray objectAtIndex:2] rssi]);
//            NSLog(@"Beacon3-> Major: %@",[[self.beaconsArray objectAtIndex:2] major]);
//            NSLog(@"Beacon4-> Distance: %f",[[[self.beaconsArray objectAtIndex:3] distance]floatValue]);
//            NSLog(@"Beacon4-> RSSI: %lddB",[[self.beaconsArray objectAtIndex:3] rssi]);
//            NSLog(@"Beacon4-> Major: %@",[[self.beaconsArray objectAtIndex:3] major]);
//            break;
//        default:
//            break;
//    }
    
}
- (void)beaconManager:(ESTBeaconManager *)manager didDiscoverBeacons:(NSArray *)beacons inRegion:(ESTBeaconRegion *)region
{
    NSLog(@"(void)beaconManager:(ESTBeaconManager *)manager didDiscoverBeacons:(NSArray *)beacons inRegion:(ESTBeaconRegion *)region");
    self.beaconsArray = beacons;
}


@end
