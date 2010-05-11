//
//  API.h
//  MSN
//
//  Created by  apple on 08-10-30.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

//#import <UIKit/UIKit.h>

BOOL isEmail(NSString* theString);
UIImage* makeImage(UIImage* backgroundImage, UIImage*subImage, CGRect subImageRect);
UIImage* makeGreyImage(UIImage*theImage);
UIImage *makeHeadPhotoImage(UIImage *theImage);
NSComparisonResult compareStrings(NSString *string1, NSString *string2);
NSString *getStatus(NSString *theString);
NSString *getStatusString(NSString *theString,BOOL displayOffline);
BOOL isValidEmailCharacter(NSString* theCharacters);
UInt16 char16(int i);
UInt32 char32(int i);

extern NSString*	audioClipPath(NSString* clipName);

extern BOOL	isBuddyImid(NSString* anImid);
extern NSString*	stringAPSDuration(NSInteger anIndex);
//extern NSInteger	indexAPSDuration(NSString* aString);

extern NSMutableArray* apsDurationStrings();
extern NSInteger durationFrom(NSInteger anIndex);
extern NSString* convertString(NSString* aString);