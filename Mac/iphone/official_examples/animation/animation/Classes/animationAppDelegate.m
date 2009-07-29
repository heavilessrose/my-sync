//
//  animationAppDelegate.m
//  animation
//
// Source code from: http://iPhoneDeveloperTips.com
//

#import "animationAppDelegate.h"

#define IMAGE_COUNT       36
#define IMAGE_WIDTH       240
#define IMAGE_HEIGHT      180
#define STATUS_BAR_HEIGHT 20
#define SCREEN_HEIGHT     460
#define SCREEN_WIDTH      320

@implementation animationAppDelegate

@synthesize window;

- (void)stopAnimation
{
  [animatedImages stopAnimating];
}

- (void)applicationDidFinishLaunching:(UIApplication *)application
{
	window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
  
  // Array to hold jpg images
  imageArray = [[NSMutableArray alloc] initWithCapacity:IMAGE_COUNT];

  // Build array of images, cycling through image names
  for (int i = 0; i < IMAGE_COUNT; i++)
    [imageArray addObject:[UIImage imageNamed:[NSString stringWithFormat:@"Frame_%d.jpg", i]]];
    
  // Animated images - centered on screen
  animatedImages = [[UIImageView alloc] 
     initWithFrame:CGRectMake(
        (SCREEN_WIDTH / 2) - (IMAGE_WIDTH / 2), 
        (SCREEN_HEIGHT / 2) - (IMAGE_HEIGHT / 2) + STATUS_BAR_HEIGHT,
        IMAGE_WIDTH, IMAGE_HEIGHT)];
  animatedImages.animationImages = [NSArray arrayWithArray:imageArray];

  // One cycle through all the images takes 1.5 seconds
  animatedImages.animationDuration = 1.0;
  
  // Repeat forever
  animatedImages.animationRepeatCount = -1;
  
  // Add subview and make window visible
  [window addSubview:animatedImages];
  [window makeKeyAndVisible];

  // Start it up
  animatedImages.startAnimating;

  // Wait 5 seconds, then stop animation
	[self performSelector:@selector(stopAnimation) withObject:nil afterDelay:5.0];

}

- (void)dealloc 
{
  [imageArray release];
  [animatedImages release];
  [window release];
  [super dealloc];
}

@end
