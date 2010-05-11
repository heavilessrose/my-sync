//
//  ZwpResponseParser.h
//  MSN
//
//  Created by  apple on 09-3-20.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ZwpResponseParser : NSObject 
{
	NSMutableData *parserData;
	NSMutableString *htmlString;
	UInt64 parserdLength;
	int formId;
	int requestFrom;
}

- (id)initWithType:(int)theType;
- (NSUInteger)getFormElementLocation:(NSString *)targetString;
- (NSDictionary *)convertSubmitdataToDict:(NSString *) submitString;
- (void)setData:(NSData *)theData;
- (void)setZwpHeaders:(NSDictionary*)theHeaders;
- (NSString*)praseResponseData:(NSError**)theError;
- (BOOL)getTag;
- (NSNumber*)getPageStyleString;
- (NSNumber*)getFontStyleString;
- (NSNumber*)getColorStyleString;
- (NSNumber*)getAlignmentStyleString;
- (NSNumber*)getSizeStyleString;
- (NSNumber*)getOffsetStyleString;
- (NSNumber*)getResDataString;
- (NSNumber*)getResponseEndString;
- (NSNumber*)getTitleString;
- (NSNumber*)getBRString;
- (NSNumber*)getHRString;
- (NSNumber*)getMarqueeString;
- (NSNumber*)getTextString;
- (NSNumber*)getLinkString;
- (NSNumber*)getLocalAnchorString;
- (NSNumber*)getImageString;
- (NSNumber*)getAutoRefreshString;
- (NSNumber*)getFileString;
- (NSNumber*)getTextBoxString;
- (NSNumber*)getSingleSelectionString;
- (NSNumber*)getMultiSelectionString;
- (NSNumber*)getHiddenString;
- (NSNumber*)getFileBrowserString;
- (NSNumber*)getRecordString;
- (NSNumber*)getCameraString;
- (NSNumber*)getValueResetButtonString;
- (NSNumber*)getResetButtonString;
- (NSNumber*)getSubmitButtonString;
- (NSNumber*)getCheckboxString;
- (NSNumber*)getExtCommandString;
@property (nonatomic, retain) NSMutableData *parserData;
@property (nonatomic, retain) NSMutableString *htmlString;
@property int requestFrom;
@end
