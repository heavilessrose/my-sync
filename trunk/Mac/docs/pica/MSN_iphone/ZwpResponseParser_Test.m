//
//  ZwpResponseParser_Test.m
//  MSN
//
//  Created by Jun Du on 3/25/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "ZwpResponseParser.h"
#import "MSNAppDelegate.h"
#import "public.h"


@interface ZwpParserTest : GHTestCase
{
	ZwpResponseParser *zwpParser;
	NSData *testData;
}
@end

@implementation ZwpParserTest

- (id) init
{
	self = [super init];
	if (self != nil) 
	{
		zwpParser = [[ZwpResponseParser alloc] initWithType:0];
	}
	return self;
}

- (void)dealloc 
{
	[zwpParser release];
	[super dealloc];
}

#pragma mark GHTestCase protocols.
- (void)setUp 
{

}

- (void)tearDown 
{

}


#pragma mark -

-(void)testUtility
{
	NSString *string1 = @"sd234ksdf.sdfisdf";
	NSString *string2 = @"33你好sdf";
	if ([Public isEnglishNumberAlphabet:string1])
		MYLog(@">>>string1");
	if ([Public isEnglishNumberAlphabet:string2])
		MYLog(@">>>string2");
}

- (void)testZwpPage0
{
	NSString *datafilePath = [[NSBundle mainBundle] pathForResource:@"zwpLog_03252009" ofType:nil];
	testData = [NSData dataWithContentsOfFile:datafilePath];
	[zwpParser setData:testData];
	NSError *error = nil;
	[zwpParser praseResponseData:&error];
	MYLog(@">>>[error]:%@",error);
	GHAssertNotNil([zwpParser htmlString],@"This variable should be something not nil.");
	MYLog(@">>>[UT]: htmlString:\n%@",[zwpParser htmlString]);
}

- (void)testgetImageString
{
	GHAssertTrue(([[zwpParser htmlString] rangeOfString:@"<img src='jV1ViGye6Uwa50yl87fu3tbSEBAS3t+SEtMwEtA=.gif' alt=''>"].location != NSNotFound),\
				 @"<UT> Image string doesn't appear!");
}

- (void)testgetTextBoxString
{
	GHAssertTrue(([[zwpParser htmlString] rangeOfString:@"<form"].location != NSNotFound), @"<UT> gettextbox string not appear");
}


- (void)testgetFormElementLocation
{
	NSUInteger index = 0;
	NSRange testRange;
	
	NSString *testString = @"<br/><a href='/msnportal/redirectHotWord.do?id=5'>彩票</a><a href='/msnportal/redirectHotWord.do?id=6'>笑话</a><a href='/msnportal/redirectHotWord.do?id=7'>奇闻</a><a href='/msnportal/redirectHotWord.do?id=8'>星座</a><br/><form action='http://wap.live.com/search/search.aspx?d=allloc&a=results&mid=3888&lc=zh-CN' method='GET'>hahahaha<input type='text' name='q' value='' />";
	testRange = [testString rangeOfString:@"hahahaha"];
	index = [zwpParser getFormElementLocation:testString];
	GHAssertTrue((index == testRange.location),@"No </form tag , has <form tag, should return the location of '>' of form tag");
	
	NSString *testString1 = @"><br/><br/><a href='http://pp.3g.cn/MSN/IndexComm.aspx'>社区</a>|<a href='http://news2.3g.cn/msn.aspx?wid=7202'>新闻</a>|<a href='http://sports.3g.cn/index.aspx?wid=7202'>体育</a>|<a href='http://book.3g.cn/msn/index.aspx'>书城</a>|<a href='http://ent.3g.cn/index.aspx?wid=7202'>娱乐</a>|<a href='http://qinggan.3g.cn/index.aspx?wid=7202'>情感</a>|<a href='http://caijing.3g.cn/index.aspx?wid=7202'>财经</a>|<a href='http://g.pica.com:8085/0_msn/index.wml'>游戏</a>";
	GHAssertTrue((NSNotFound == [zwpParser getFormElementLocation:testString1]), @"No </form tag, no <form tag, shoud return NSNotFound");
	
	NSString *testString2 = @"<br/><form action='http://wap.live.com/search/search.aspx?d=allloc&a=results&mid=3888&lc=zh-CN' method='GET'><input type='text' name='q' value='' /><input type='submit' value='搜索' /></form><br/><br/>【<a href='http://mob.3g.cn/MSNIndex/count.aspx?ty=1&amp;linkid=0&amp;lintyid=0&amp;chid=159&amp;url=http://news.3g.cn/china.aspx!wid<>7202'>国内</a>";
	GHAssertTrue((NSNotFound == [zwpParser getFormElementLocation:testString2]), @"has </form tag, no <form tag, should return NSNotFound");
	
	index = 0;
	NSString *testString3 = @"<input type='submit' value='搜索' /></form><br/><br/><br/><form action='http://wap.live.com/search/search.aspx?d=allloc&a=results&mid=3888&lc=zh-CN' method='GET'>hahahaha<input type='text' name='q' value='' />";
	testRange = [testString3 rangeOfString:@"hahahaha"];
	index = [zwpParser getFormElementLocation:testString3];
	MYLog(@">>>[testString3]:%@",testString3);
	MYLog(@">>>[testRange.location]:%d,[index]:%d",testRange.location,index);
	GHAssertTrue((index == testRange.location),@"has </form tag , has <form tag, should return the location of '>' of form tag");

}

- (void)testconvertSubmitdataToDict
{
    NSString *submitString = @"username=abc&nickname=hello&item1=$input_filed&item2=hahahaha";
	NSDictionary *testDict = [[zwpParser convertSubmitdataToDict:submitString] retain];
	NSDictionary *alreadyDict  = [testDict objectForKey:@"kAlreadyElement"];
	NSDictionary *needProcDict = [testDict objectForKey:@"kNeedProcElement"];

	GHAssertTrue(([alreadyDict count] == 3),@"this test should return 3 key-value pairs. ACT:%d",[alreadyDict count]);
	GHAssertTrue(([needProcDict count] == 1),@"this test should return 1 key-value pairs.");
	GHAssertEqualStrings([needProcDict objectForKey:@"item1"],@"input_filed",@"the items should be equal here");
	
	GHAssertTrue([testDict isKindOfClass:[NSDictionary class]],@"Should be an instance of NSDictionary");
	[testDict release];
}

- (void)testhrefParser
{
	NSString *href1 = @"http://qinggan.3g.cn/NewsContent.aspx?sid=&wid=7202&nid=23951";
	NSMutableString *href = [href1 mutableCopy];
	NSRange indexRange = [href rangeOfString:@"?"];
	NSString *tagsString = [href substringFromIndex:indexRange.location + 1];
	[href deleteCharactersInRange:NSMakeRange(indexRange.location, [href length]-indexRange.location)];
	
	GHAssertTrue([href length] == 37,@"Deleted wrong index.");
	
	NSDictionary *testDict = [[zwpParser convertSubmitdataToDict:tagsString] retain];
	NSDictionary *alreadyDict  = [testDict objectForKey:@"kAlreadyElement"];
//	NSDictionary *needProcDict = [testDict objectForKey:@"kNeedProcElement"];
	
	GHAssertTrue(([alreadyDict count] == 3), @"Should be 3");
}



@end