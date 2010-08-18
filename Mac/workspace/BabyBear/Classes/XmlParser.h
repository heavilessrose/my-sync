//
//  XmlParser.h
//  BabyBear
//
//  Created by luke on 10-8-12.
//  Copyright 2010 Luke. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol XmlParserDelegate

@optional
- (void)xmlWillParse;
- (void)xmlParsing:(int)state withErr:(NSError *)err;
- (void)xmlParseFinished:(int)state withErr:(NSError *)err;

@end



@interface XmlParser : NSObject {
	id<XmlParserDelegate> delegate;
}

@property (nonatomic, assign) id<XmlParserDelegate> delegate;
@end
