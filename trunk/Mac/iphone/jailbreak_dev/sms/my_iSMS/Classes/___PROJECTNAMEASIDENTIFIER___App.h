//
//  ___PROJECTNAMEASIDENTIFIER___App.h
//  ___PROJECTNAME___
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include "device.h"

// include for the sdk compiler and open toolchain headers
#ifndef UIKIT_UIFont_UIColor_H
#define UIKIT_UIFont_UIColor_H
typedef float CGFloat;
#import <UIKit/UIFont.h>
#import <UIKit/UIColor.h>
#endif


@interface ___PROJECTNAMEASIDENTIFIER___App : UIApplication {
    UIWindow *window;
    UIView *mainView;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) UIView *mainView;

-(BOOL)sendMessage:(NSString*) msg to:(NSString*)receiver;
@end
