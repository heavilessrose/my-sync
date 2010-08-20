//
//  BaseProduct.h
//  BabyBear
//
//  Created by luke on 10-8-19.
//  Copyright 2010 Luke. All rights reserved.
//

#import <Foundation/Foundation.h>

#define kProduct	(@"product")
#define kPID		(@"pid")
#define kPName		(@"pname")
#define kPPrice		(@"pprice")
#define kPAllRating	(@"pall-rating")
#define kPDesc		(@"pdesc")
#define kPUrlIcon	(@"picon-url")
#define kPGallary	(@"pgallary")
#define kPUrlPhoto	(@"pphoto-url")
#define kPReviews	(@"previews")
#define kPReview	(@"preview")
#define kRName		(@"rname")
#define kRRating	(@"rrating")
#define kRDate		(@"rdate")
#define kRTitle		(@"rtitle")
#define kRCatagory	(@"rcatagory")
#define kRComment	(@"rcomment")
#define kPStores	(@"pstores")
#define kPStore		(@"pstore")
#define kSID		(@"sid")
#define kSName		(@"sname")
#define kSAddr		(@"saddr")
#define kSReserves	(@"sreserves")
#define kSLongitude	(@"slongitude")
#define kSLatitude	(@"slatitude")


@interface BaseProduct : NSObject
{
	NSString		*pid;
	NSString		*pname;
	NSString		*pprice;
	NSString		*pallRating;
	NSString		*pdesc;
	NSString		*pUrlIcon;
	NSMutableArray	*pgallary;
	//NSString		*pUrlPhoto;
	NSMutableArray	*previews;
	//NSString		*preview;
	NSMutableArray	*pstores;
	//NSString		*pstore;
	
	UIImage			*productIcon;
	BOOL			iconLoaded;
	BOOL			gallaryLoaded;
	NSString		*iconLocalPath;
}

@property (nonatomic, retain) NSString	*pid;
@property (nonatomic, retain) NSString	*pname;
@property (nonatomic, retain) NSString	*pprice;
@property (nonatomic, retain) NSString	*pallRating;
@property (nonatomic, retain) NSString	*pdesc;
@property (nonatomic, retain) NSString	*pUrlIcon;
@property (nonatomic, retain) NSMutableArray	*pgallary;
//@property (nonatomic, retain) NSString	*pUrlPhoto;
@property (nonatomic, retain) NSMutableArray	*previews;
//@property (nonatomic, retain) NSString	*preview;
@property (nonatomic, retain) NSMutableArray	*pstores;
//@property (nonatomic, retain) NSString	*pstore;

@property (nonatomic, retain) UIImage	*productIcon;
@property (nonatomic, assign) BOOL		iconLoaded;
@property (nonatomic, assign) BOOL		gallaryLoaded;
@property (nonatomic, retain) NSString	*iconLocalPath;

@end
