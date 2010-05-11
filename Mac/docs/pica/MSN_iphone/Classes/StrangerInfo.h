//
//  Stranger.h
//  MSN
//
//  Created by  apple on 08-10-20.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface StrangerInfo : NSObject 
{
	NSString *imid;
	NSString *nickname;
	NSString *realname;
	NSString *group;
	NSString *sex;
	NSString *age;
	NSString *province;
	NSString *desc;
}

@property (nonatomic,retain)NSString *imid;
@property (nonatomic,retain)NSString *nickname;
@property (nonatomic,retain)NSString *realname;
@property (nonatomic,retain)NSString *sex;
@property (nonatomic,retain)NSString *age;
@property (nonatomic,retain)NSString *province;
@property (nonatomic,retain)NSString *desc;
@property (nonatomic,retain)NSString *group;
@end
