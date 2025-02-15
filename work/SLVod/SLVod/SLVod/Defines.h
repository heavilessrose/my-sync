//
//  Defines.h
//  siluVod
//
//  Created by luke on 11-6-1.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "SLVodAppDelegate.h"

#define globalApp ((SLVodAppDelegate *)[UIApplication sharedApplication].delegate)

#define SLImgeLoadedNotif (@"SLImgeLoadedNotif")

// Color
#define RGBCOLOR(r,g,b) [UIColor colorWithRed:(r)/255.0 green:(g)/255.0 blue:(b)/255.0 alpha:1]
#define RGBACOLOR(r,g,b,a) [UIColor colorWithRed:(r)/255.0 green:(g)/255.0 blue:(b)/255.0 alpha:(a)]

// Font
#define Font_Big			([UIFont systemFontOfSize:18.0f])
#define Font_Normal			([UIFont systemFontOfSize:16.0f])
#define Font_Small			([UIFont systemFontOfSize:14.0f])
#define Font_Tiny			([UIFont systemFontOfSize:12.0f])
#define Font_Huge_bold		([UIFont boldSystemFontOfSize:20.0f])
#define Font_Big_bold		([UIFont boldSystemFontOfSize:18.0f])
#define Font_Normal_bold	([UIFont boldSystemFontOfSize:16.0f])
#define Font_Small_bold		([UIFont boldSystemFontOfSize:14.0f])
#define Font_Tiny_bold		([UIFont boldSystemFontOfSize:12.0f])

// URL
#define SL_PAGESIZE         (5)
#define SL_BASE_HOST        ([NSURL URLWithString:@"http://i.siluhd.com"])
#define SL_HOT              (@"ipadgetnew.asp?page=%d")
#define SL_CATE_LIST        (@"ipad_get_typelist.asp?page=%d")
#define SL_CATE_MOVS        (@"ipadsearch.asp?searchtype=Filmtype&page=%d&keyword=%@")
#define SL_SEARCH           (@"ipadsearch.asp?searchtype=search&page=%d&keyword=%@")
#define SL_LOGIN            (@"ipadlogin.asp?userid=%@&password=%@")
#define SL_REG              (@"ipadreguser.asp?userid=%@&password=%@&myemail=%@&UDID=%@")

// LOG
#define LK_DEBUG
#define LK_MEM_DEBUG

#ifdef LK_DEBUG 
#define NSLog(...) NSLog(__VA_ARGS__)
#else
#define NSLog(...) 
#endif

#ifdef LK_DEBUG
#define DLog(...) NSLog(@"%s %@", __PRETTY_FUNCTION__, [NSString stringWithFormat:__VA_ARGS__])
#define ALog(...) [[NSAssertionHandler currentHandler] handleFailureInFunction:[NSString stringWithCString:__PRETTY_FUNCTION__ encoding:NSUTF8StringEncoding] file:[NSString stringWithCString:__FILE__ encoding:NSUTF8StringEncoding] lineNumber:__LINE__ description:__VA_ARGS__]
#define DLOG DLog(@"");
#else
#define DLog(...) do { } while (0)
#ifndef NS_BLOCK_ASSERTIONS
#define NS_BLOCK_ASSERTIONS
#endif
#define ALog(...) NSLog(@"%s %@", __PRETTY_FUNCTION__, [NSString stringWithFormat:__VA_ARGS__])
#endif

#ifdef LK_MEM_DEBUG
#define MLog(...) NSLog(@"%s %@", __PRETTY_FUNCTION__, [NSString stringWithFormat:__VA_ARGS__])
#else
#define MLog(...) do {} while (0)
#endif


#define ZAssert(condition, ...) do { if (!(condition)) { ALog(__VA_ARGS__); }} while(0)

