//
//  XmlParseOperation.m
//  BabyBear
//
//  Created by luke on 10-8-12.
//  Copyright 2010 Luke. All rights reserved.
//

#import "XmlParseOperation.h"
#import "BaseProduct.h"
#import "Review.h"
#import "Store.h"

@interface XmlParseOperation ()

@property (nonatomic, assign) id<XmlParseOperationDelegate>	delegate;
@property (nonatomic, retain) NSData				*dataToParse;
@property (nonatomic, retain) NSMutableArray		*workingArray;
@property (nonatomic, retain) BaseProduct			*workingEntry;
@property (nonatomic, retain) NSMutableString		*workingPropertyString;
@property (nonatomic, retain) NSArray				*elementsToParse;
@property (nonatomic, assign) BOOL					storingCharacterData;

@property (nonatomic, retain) Review				*aReview;
@property (nonatomic, retain) Store					*aStore;

@end

// string contants found in the xml file
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


@implementation XmlParseOperation

@synthesize delegate, dataToParse, workingEntry, workingArray, workingPropertyString, elementsToParse, storingCharacterData;
@synthesize aReview, aStore;


- (id)initWithData:(NSData *)data delegate:(id <XmlParseOperationDelegate>)theDelegate
{
    self = [super init];
    if (self != nil)
    {
        self.dataToParse = data;
        self.delegate = theDelegate;
        self.elementsToParse = [NSArray arrayWithObjects:kPID, kPName, kPPrice, kPAllRating, kPDesc, kPUrlIcon, kPGallary, kPUrlPhoto, kPReviews, 
								kPReview, kRName, kRRating, kRDate, kRTitle, kRCatagory, kRComment, kPStores, kPStore, kSID, kSName, kSAddr, 
								kSReserves, kSLongitude, kSLatitude, nil];
    }
    return self;
}

- (void)dealloc
{
	[dataToParse release];
    [workingEntry release];
    [workingPropertyString release];
    [workingArray release];
	
	[aReview release];
	[aStore release];
	
	[super dealloc];
}

// -------------------------------------------------------------------------------
//	main:
//  Given data to parse, use NSXMLParser and process all the products in xml.
// -------------------------------------------------------------------------------
- (void)main
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	self.workingArray = [NSMutableArray array];
    self.workingPropertyString = [NSMutableString string];
    
    // It's also possible to have NSXMLParser download the data, by passing it a URL, but this is not
	// desirable because it gives less control over the network, particularly in responding to
	// connection errors.
    //
    NSXMLParser *parser = [[NSXMLParser alloc] initWithData:dataToParse];
	[parser setDelegate:self];
    [parser parse];
	
	if (![self isCancelled])
    {
        // notify our AppDelegate that the parsing is complete
        [self.delegate xmlDidFinishParsing:self.workingArray];
    }
    
    self.workingArray = nil;
    self.workingPropertyString = nil;
    self.dataToParse = nil;
    
    [parser release];
	
	[pool release];
}


#pragma mark -
#pragma mark XML processing


- (void)parserDidStartDocument:(NSXMLParser *)parser
{
	NSLog(@"parserDidStartDocument");
}

- (void)parser:(NSXMLParser *)parser didStartElement:(NSString *)elementName namespaceURI:(NSString *)namespaceURI 
 qualifiedName:(NSString *)qName attributes:(NSDictionary *)attributeDict
{
    if ([elementName isEqualToString:kProduct]) {
        self.workingEntry = [[[BaseProduct alloc] init] autorelease];
    } 
	/*
	else if ([elementName isEqualToString:kPReview]) {
		if (!aReview) {
			self.aReview = [[[Review alloc] init] autorelease];
		} else {
			assert(0);
		}
	}
	else if ([elementName isEqualToString:kPStore]) {
		if (!aStore) {
			self.aStore = [[[Store alloc] init] autorelease];
		} else {
			assert(0);
		}
	}
	 */

    storingCharacterData = [elementsToParse containsObject:elementName];
}

- (void)parser:(NSXMLParser *)parser didEndElement:(NSString *)elementName
  namespaceURI:(NSString *)namespaceURI
 qualifiedName:(NSString *)qName
{
    if (self.workingEntry) {
        if (storingCharacterData) {
            NSString *trimmedString = [workingPropertyString stringByTrimmingCharactersInSet:
                                       [NSCharacterSet whitespaceAndNewlineCharacterSet]];
            [workingPropertyString setString:@""];  // clear the string for next time
            if ([elementName isEqualToString:kPID]) {
                self.workingEntry.pid = trimmedString;
            }
            else if ([elementName isEqualToString:kPName]) {        
                self.workingEntry.pname = trimmedString;
            }
            else if ([elementName isEqualToString:kPPrice]) {
                self.workingEntry.pprice = trimmedString;
            }
            else if ([elementName isEqualToString:kPAllRating]) {
                self.workingEntry.pallRating = trimmedString;
            }
            else if ([elementName isEqualToString:kPDesc]) {
                self.workingEntry.pdesc = trimmedString;
            }
            else if ([elementName isEqualToString:kPUrlIcon]) {
                self.workingEntry.pUrlIcon = trimmedString;
            }
			else if ([elementName isEqualToString:kPUrlPhoto]) {
				if ([elementName isEqualToString:kPUrlPhoto]) {
					[self.workingEntry.pgallary addObject:trimmedString];
				}
			}
			else if ([elementName isEqualToString:kPReviews]) {
				/*
				if ([elementName isEqualToString:kRTitle]) {
					[self.workingEntry.pgallary addObject:trimmedString];
				}
				 */
			}
        }
    }
}

- (void)parser:(NSXMLParser *)parser foundCharacters:(NSString *)string
{
    if (storingCharacterData) {
        [workingPropertyString appendString:string];
		NSLog(@"workingPropertyString: %@", workingPropertyString);
    }
}

- (void)parserDidEndDocument:(NSXMLParser *)parser
{
	NSLog(@"parserDidEndDocument");
	[self.workingArray addObject:self.workingEntry];  
	self.workingEntry = nil;
}

- (void)parser:(NSXMLParser *)parser parseErrorOccurred:(NSError *)parseError
{
    [delegate xmlParseErrOccurred:parseError];
}


@end
