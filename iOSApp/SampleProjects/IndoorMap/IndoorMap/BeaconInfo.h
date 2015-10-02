#import <UIKit/UIKit.h>
#import "ESTBeacon.h"

typedef enum : int
{
    ESTScanTypeBluetooth,
    ESTScanTypeBeacon
    
} ESTScanType;


@interface BeaconInfo : UIView
- (id)initWithScanType:(ESTScanType)scanType;
- (void)start;
@end
