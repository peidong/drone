

#import <UIKit/UIKit.h>
#import <CoreMotion/CoreMotion.h>



#import "ESTBeacon.h"
#import "ESTBeaconManager.h"

#define DISTANCE_ERROR 1
#define BEACON_X 20
#define BEACON_Y 30
#define DISTANCE_PER_PIXEL 0.023

typedef enum : int
{
    ESTScanTypeBluetooth,
    ESTScanTypeBeacon
    
} ESTScanType;

@interface LocalizationViewController : UIViewController

@property CMAccelerometerData *accelerometerData;
@property CMMotionManager *motionManager;
@property(readonly, nonatomic) CMRotationRate rotationRate;
@property(readonly, nonatomic) CMMagneticField magneticField;
@property(readonly, nonatomic) CMMagnetometerData *magnetometerData;

@property CGFloat pinchScale;
@property CGPoint longPressPoint;
@property CGPoint positionPoint;
@property (strong, nonatomic) UIView *locationView;//location point
@property (strong, nonatomic) IBOutlet UIView *RoomPlanView;

@property (nonatomic, weak) UIView *pieceForLongPress;
@property (strong, nonatomic) UIView *canvasDraw;

@property int numberOfBeacons;
@property (readwrite) NSMutableDictionary *beaconsViewDictionary;
@property (strong, nonatomic) UIView *beaconView1;
@property (strong, nonatomic) UIView *beaconView2;
@property (strong, nonatomic) UIView *beaconView3;
@property (strong, nonatomic) UIView *beaconView4;
@property (strong, nonatomic) UIView *beaconView5;
@property (strong, nonatomic) UIView *beaconView6;
@property (strong, nonatomic) UIView *beaconView7;
@property (strong, nonatomic) UIView *beaconView8;
@property (strong, nonatomic) UIView *beaconView9;
@property (strong, nonatomic) UIView *beaconView10;
@property (strong, nonatomic) UIView *beaconView11;
@property (strong, nonatomic) UIView *beaconView12;
@property (strong, nonatomic) UIView *beaconView13;
@property (strong, nonatomic) UIView *beaconView14;
@property (strong, nonatomic) UIView *beaconView15;
@property (strong, nonatomic) UIView *beaconView16;
@property (strong, nonatomic) UIView *beaconView17;
@property (strong, nonatomic) UIView *beaconView18;
@property (strong, nonatomic) UIView *beaconView19;
@property (strong, nonatomic) UIView *beaconView20;

@property (nonatomic, strong) NSMutableDictionary *beaconsDictionary;
@property (nonatomic, strong) NSMutableDictionary *beaconsPositionDictionaryX;
@property (nonatomic, strong) NSMutableDictionary *beaconsPositionDictionaryY;

- (IBAction)pinchGesture:(UIPinchGestureRecognizer *)gestureRecognizer;
- (IBAction)panGesture:(UIPanGestureRecognizer *)gestureRecognizer;
- (IBAction)rotationGesture:(UIRotationGestureRecognizer *)gestureRecognizer;
- (IBAction)longPressGesture:(UILongPressGestureRecognizer *)gestureRecognizer;
- (IBAction)ResetMap:(UIButton *)sender;
- (IBAction)Test:(UIButton *)sender;


////////////////iBeacon
@property (nonatomic, assign) ESTScanType scanType;
@property (nonatomic, strong) ESTBeaconManager *beaconManager;
@property (nonatomic, strong) ESTBeaconRegion *region;
@property (nonatomic, strong) NSArray *beaconsArray;

/**
 iBeacon: blue1 ice1 Major:55555 Minor:47056
 iBeacon: blue2 ice2 Major:23697 Minor:49133
 iBeacon: green mint Major:55950 Minor:21162
 iBeacon: purple blueberry Major:51634 Minor:14360
 */
@property (strong, nonatomic) IBOutlet UITextView *iBeaconInfoTextView;

@property NSString *whichBeaconIsLongPressed;


@end

