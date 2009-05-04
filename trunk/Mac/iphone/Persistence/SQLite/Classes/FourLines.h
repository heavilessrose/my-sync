//
//  FourLines.h
//  archive
//
//  Created by wang luke on 5/4/09.
//  Copyright 2009 luke. All rights reserved.
//

#define kField1key @"Field1"
#define kField2key @"Field2"
#define kField3key @"Field3"
#define kField4key @"Field4"

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
// 一个简单的符合NSCoding和NSCopying的数据模型类 （即一个可归档的数据对象）
@interface FourLines : NSObject <NSCoding, NSCopying> {
	NSString *field1;
	NSString *field2;
	NSString *field3;
	NSString *field4;
}

@property (nonatomic, retain) NSString *field1;
@property (nonatomic, retain) NSString *field2;
@property (nonatomic, retain) NSString *field3;
@property (nonatomic, retain) NSString *field4;

@end
