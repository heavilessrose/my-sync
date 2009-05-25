//
//  FourLines.h
//  Persistence
//
//  Created by Jeff LaMarche on 7/29/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#define	kField1Key	@"Field1"
#define	kField2Key	@"Field2"
#define	kField3Key	@"Field3"
#define	kField4Key	@"Field4"

#import <UIKit/UIKit.h>


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
