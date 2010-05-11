//
//  mClubParserTest.m
//  MSN
//
//  Created by Jun Du on 5/21/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//
#import "DDXML.h"


@interface mClubParserTest : GHTestCase {

}

@end


@implementation mClubParserTest

- (void)testFeedRespond
{
//	NSString *theString = @"<iq type='result' from='mclub.pica' to='7524686@pica/pica'><query xmlns='mclub:notify' action='notify' result='0'><item imid='dujunfly@gmail.com' type='m06.png' isprivate='0'><body>指一下你人生路</body></item></query></iq>";
//	NSString *theString = @"<iq type='set' from='mclub.pica'><query xmlns='mclub:notify' action='notifyreq'><item imid=\"aaa@msn.cn\" type =\"0\" isprivate=\"0\"><body>你好</body></query></iq>";
	NSString *theString = @"<iq type='result' from='mclub.pica'>\
	<query xmlns='mclub:invite' action='invite' result='0'>\
	<item imid=\"bbb@msn.cn\"/>\
	</query>\
	</iq>";
	
	NSError *error;
	
	DDXMLElement *iq = [[[DDXMLElement alloc] initWithXMLString:theString error:&error] autorelease];
	DDXMLElement *theQuery = [[iq elementsForName:@"query"] objectAtIndex:0];
	DDXMLElement *theItem = [[theQuery elementsForName:@"item"] objectAtIndex:0];
	NSString *imid = [[theItem attributeForName:@"imid"] stringValue];
	
//	MYLog(@">>>theBody:%@",[theBody stringValue]);

}

@end
