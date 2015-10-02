//
//  FileManage.h
//  IndoorMap
//
//  Created by peidong on 14/12/9.
//  Copyright (c) 2014年 peidong. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface FileManage : UIView

+ (void) downloadIndoorMapPointsFile;
+ (void) downloadFileFromStrFolderPathURL:(NSString*)strFolderPathURL strFileNameURL:(NSString*)strFileNameURL;
+ (NSMutableArray*) toNSArrayFromIndoorMapPointsFile;
+ (NSMutableArray*) toNSArrayFromStrFilepathAfterDocuments:(NSString*)strFilepathAfterDocuments strFilename:(NSString*)strFilename strFolderPathURL:(NSString*)strFolderPathURL strFileNameURL:(NSString*)strFileNameURL;
+ (NSMutableArray*) toNSArrayFromiBeaconPointsFile;
+ (void) downloadiBeaconPointsFile;
@end
