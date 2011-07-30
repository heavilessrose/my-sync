//
//  main.m
//  SLVod
//
//  Created by luke on 11-6-12.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SCClassUtils.h"

int main(int argc, char *argv[])
{
    [SCClassUtils swizzleSelector:@selector(insertSubview:atIndex:)
                          ofClass:[UINavigationBar class]
                     withSelector:@selector(scInsertSubview:atIndex:)];
    [SCClassUtils swizzleSelector:@selector(sendSubviewToBack:)
                          ofClass:[UINavigationBar class]
                     withSelector:@selector(scSendSubviewToBack:)];
    
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, nil);
    [pool release];
    return retVal;
}
