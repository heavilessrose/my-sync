//
//  XmlParseOperation.h
//  BabyBear
//
//  Created by luke on 10-8-12.
//  Copyright 2010 Luke. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol XmlParseOperationDelegate

@optional
- (void)xmlWillParse;
- (void)xmlParsing:(int)aState withErr:(NSError *)aErr;
- (void)xmlDidFinishParsing:(NSDictionary *)aProductList;
- (void)xmlParseErrOccurred:(NSError *)aErr;

@end


@class BaseProduct, Review, Store;
@interface XmlParseOperation : NSOperation <NSXMLParserDelegate> {
	id<XmlParseOperationDelegate>	delegate;
    NSData					*dataToParse;
    
    //NSMutableDictionary		*workingDictionary;
    BaseProduct				*workingProduct;
    NSMutableString			*workingPropertyString;
    NSArray					*elementsToParse;
    BOOL					storingCharacterData;
	
	Review					*aReview;
	Store					*aStore;
}


- (id)initWithData:(NSData *)data delegate:(id <XmlParseOperationDelegate>)theDelegate;

@end
