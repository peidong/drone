//
//  FileManage.m
//  IndoorMap
//
//  Created by peidong on 14/12/9.
//  Copyright (c) 2014年 peidong. All rights reserved.
//

#import "FileManage.h"
#import "FCFileManager.h"
#import "AFNetworking.h"


@implementation FileManage

+ (void) downloadIndoorMapPointsFile{
    //    NSString *DocumentsDirectory = [FCFileManager pathForDocumentsDirectory];
    //    NSString *IndoorMapPointsFilePath = [DocumentsDirectory stringByAppendingString:@"/IndoorMapPoints.txt"];
    //
    //    if ([FCFileManager isFileItemAtPath:IndoorMapPointsFilePath]) {
    //        [FCFileManager removeItemAtPath:IndoorMapPointsFilePath];
    //    }
    //
    //    NSURLSessionConfiguration *downloadConfiguration = [NSURLSessionConfiguration defaultSessionConfiguration];
    //    AFURLSessionManager *downloadManager = [[AFURLSessionManager alloc] initWithSessionConfiguration:downloadConfiguration];
    //
    //    NSURL *downloadURL = [NSURL URLWithString:@"http://eda.ee.ucla.edu/iBeacon/IndoorMapPoints.txt"];
    //    NSURLRequest *downloadRequest = [NSURLRequest requestWithURL:downloadURL];
    //
    //    NSURLSessionDownloadTask *downloadTask = [downloadManager downloadTaskWithRequest:downloadRequest progress:nil destination:^NSURL *(NSURL *targetPath, NSURLResponse *response) {
    //        NSURL *documentsDirectoryURL = [[NSFileManager defaultManager] URLForDirectory:NSDocumentDirectory inDomain:NSUserDomainMask appropriateForURL:nil create:NO error:nil];
    //        return [documentsDirectoryURL URLByAppendingPathComponent:[response suggestedFilename]];
    //    } completionHandler:^(NSURLResponse *response, NSURL *downloadFilePathURL, NSError *error) {
    //        NSLog(@"File downloaded to: %@", downloadFilePathURL);
    //    }];
    //    [downloadTask resume];
    [self downloadFileFromStrFolderPathURL:@"http://eda.ee.ucla.edu/iBeacon/" strFileNameURL:@"IndoorMapPoints.txt"];
}

+ (void) downloadiBeaconPointsFile{
    [self downloadFileFromStrFolderPathURL:@"http://eda.ee.ucla.edu/iBeacon/" strFileNameURL:@"iBeaconPoints.txt"];
}

+ (void) downloadFileFromStrFolderPathURL:(NSString*)strFolderPathURL strFileNameURL:(NSString*)strFileNameURL{
    NSString *DocumentsDirectory = [FCFileManager pathForDocumentsDirectory];
    NSString *FilePathTemp =[DocumentsDirectory stringByAppendingString:@"/"];
    NSString *FilePath = [FilePathTemp stringByAppendingString:strFileNameURL];
    NSString *strDownloadURL = [strFolderPathURL stringByAppendingString:strFileNameURL];
    
    if ([FCFileManager isFileItemAtPath:FilePath]) {
        [FCFileManager removeItemAtPath:FilePath];
    }
    
    NSURLSessionConfiguration *downloadConfiguration = [NSURLSessionConfiguration defaultSessionConfiguration];
    AFURLSessionManager *downloadManager = [[AFURLSessionManager alloc] initWithSessionConfiguration:downloadConfiguration];
    
    NSURL *downloadURL = [NSURL URLWithString:strDownloadURL];
    NSURLRequest *downloadRequest = [NSURLRequest requestWithURL:downloadURL];
    
    NSURLSessionDownloadTask *downloadTask = [downloadManager downloadTaskWithRequest:downloadRequest progress:nil destination:^NSURL *(NSURL *targetPath, NSURLResponse *response) {
        NSURL *documentsDirectoryURL = [[NSFileManager defaultManager] URLForDirectory:NSDocumentDirectory inDomain:NSUserDomainMask appropriateForURL:nil create:NO error:nil];
        return [documentsDirectoryURL URLByAppendingPathComponent:[response suggestedFilename]];
    } completionHandler:^(NSURLResponse *response, NSURL *downloadFilePathURL, NSError *error) {
        NSLog(@"File downloaded to: %@", downloadFilePathURL);
    }];
    [downloadTask resume];
}


+ (NSMutableArray*) toNSArrayFromIndoorMapPointsFile{
    //    NSString *DocumentsDirectory = [FCFileManager pathForDocumentsDirectory];
    //    NSString *IndoorMapPointsFilePath = [DocumentsDirectory stringByAppendingString:@"/IndoorMapPoints.txt"];
    //
    //    if (![FCFileManager isFileItemAtPath:IndoorMapPointsFilePath]){
    //        [FileManage downloadIndoorMapPointsFile];
    //        //准备删掉
    //        [FCFileManager writeFileAtPath:IndoorMapPointsFilePath content:@"100,100\n200,100\n200,200\n250,200\n250,400\n100,400"];
    //        //准备删掉
    //    }
    //
    //    if ([FCFileManager isFileItemAtPath:IndoorMapPointsFilePath]) {
    //        NSString *strFromIndoorMapPointsFile = [FCFileManager readFileAtPath:IndoorMapPointsFilePath];
    //        NSArray *arrayPointsXY = [strFromIndoorMapPointsFile componentsSeparatedByString:@"\n"];
    //
    //        NSMutableArray *arrayPointsX = [NSMutableArray array];
    //        NSMutableArray *arrayPointsY = [NSMutableArray array];
    //
    //        NSMutableArray *points = [NSMutableArray array];
    //        for (int i =0 ; i<[arrayPointsXY count]; i++) {
    //            NSArray *pointsXandYinOneArray = [NSArray arrayWithArray:[arrayPointsXY[i] componentsSeparatedByString:@","]];
    //
    //            [arrayPointsX addObjectsFromArray:[pointsXandYinOneArray[0] componentsSeparatedByString:@","]];
    //
    //            [arrayPointsY addObjectsFromArray:[pointsXandYinOneArray[1] componentsSeparatedByString:@","]];
    //
    //            [points addObject:[NSValue valueWithCGPoint:CGPointMake([arrayPointsX[i] intValue], [arrayPointsY[i] intValue])]];
    //
    //        }
    //        return points;
    //    }
    //    return nil;
    return [self toNSArrayFromStrFilepathAfterDocuments:@"" strFilename:@"IndoorMapPoints.txt" strFolderPathURL:@"http://eda.ee.ucla.edu/iBeacon/" strFileNameURL:@"IndoorMapPoints.txt"];
}

+ (NSMutableArray*) toNSArrayFromiBeaconPointsFile{
    return [self toNSArrayFromStrFilepathAfterDocuments:@"" strFilename:@"iBeaconPoints.txt" strFolderPathURL:@"http://eda.ee.ucla.edu/iBeacon/" strFileNameURL:@"iBeaconPoints.txt"];
}

+ (NSMutableArray*) toNSArrayFromStrFilepathAfterDocuments:(NSString*)strFilepathAfterDocuments strFilename:(NSString*)strFilename strFolderPathURL:(NSString*)strFolderPathURL strFileNameURL:(NSString*)strFileNameURL {
    NSString *DocumentsDirectory = [FCFileManager pathForDocumentsDirectory];
    NSString *FilePath;
    if (![strFilepathAfterDocuments  isEqual: @""]) {
        NSString *FilePathTemp1 = [DocumentsDirectory stringByAppendingString:(@"/")];
        NSString *FilePathTemp2 = [FilePathTemp1 stringByAppendingString:strFilepathAfterDocuments];
        NSString *FilePathTemp = [FilePathTemp2 stringByAppendingString:@"/"];
        FilePath = [FilePathTemp stringByAppendingString:strFilename];
    }else{
        NSString *FilePathTemp = [DocumentsDirectory stringByAppendingString:(@"/")];
        FilePath = [FilePathTemp stringByAppendingString:strFilename];
    }
    
    if (![FCFileManager isFileItemAtPath:FilePath]){
        [FileManage downloadFileFromStrFolderPathURL:strFolderPathURL strFileNameURL:strFileNameURL];
        return nil;
    }
    
    if ([FCFileManager isFileItemAtPath:FilePath]) {
        NSString *strFile = [FCFileManager readFileAtPath:FilePath];
        NSArray *arrayPointsXY = [strFile componentsSeparatedByString:@"\n"];
        
        NSMutableArray *arrayPointsX = [NSMutableArray array];
        NSMutableArray *arrayPointsY = [NSMutableArray array];
        
        NSMutableArray *points = [NSMutableArray array];
        for (int i =0 ; i<[arrayPointsXY count]; i++) {
            NSArray *pointsXandYinOneArray = [NSArray arrayWithArray:[arrayPointsXY[i] componentsSeparatedByString:@","]];
            
            [arrayPointsX addObjectsFromArray:[pointsXandYinOneArray[0] componentsSeparatedByString:@","]];
            
            [arrayPointsY addObjectsFromArray:[pointsXandYinOneArray[1] componentsSeparatedByString:@","]];
            
            [points addObject:[NSValue valueWithCGPoint:CGPointMake([arrayPointsX[i] intValue], [arrayPointsY[i] intValue])]];
        }
        return points;
    }
    return nil;
}

@end
