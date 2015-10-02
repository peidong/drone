//
//  DistanceBeaconViewController.h
//  Examples
//
//  Created by peidong on 14/12/10.
//  Copyright (c) 2014年 com.estimote. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ESTBeacon.h"

typedef enum : int
{
    ESTScanTypeBluetooth,
    ESTScanTypeBeacon
    
} ESTScanType;


@interface DistanceBeaconViewController : UIViewController
- (id)initWithScanType:(ESTScanType)scanType;
@end
