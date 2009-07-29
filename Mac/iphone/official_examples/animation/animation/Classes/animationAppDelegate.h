//
//  animationAppDelegate.h
//  animation
//
// Source code from: http://iPhoneDeveloperTips.com
//

#import <UIKit/UIKit.h>

@interface animationAppDelegate : NSObject <UIApplicationDelegate>
{
  UIImageView   *animatedImages;
  NSMutableArray *imageArray;
  UIWindow *window;
}

@property (nonatomic, retain) UIWindow *window;

@end

