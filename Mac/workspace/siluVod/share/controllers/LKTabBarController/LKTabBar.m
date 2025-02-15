//
//  LKTabBar.m
//  siluVod
//
//  Created by luke on 11-5-15.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "LKTabBar.h"

#define LKTabBar_GLOW_IMAGE_TAG             2394858
#define LKTabBar_TAB_ARROW_IMAGE_TAG        2394859
#define LKTabBar_TAB_ARROW_MOVE_DURATION    0.3f


@interface LKTabBar ()
- (CGFloat)horizontalLocationFor:(NSUInteger)tabIndex;
- (void)addTabBarArrowAtIndex:(NSUInteger)itemIndex;
- (UIButton *)buttonAtIndex:(NSUInteger)itemIndex width:(CGFloat)width;
- (UIImage *)tabBarImage:(UIImage *)startImage size:(CGSize)targetSize backgroundImage:(UIImage *)backgroundImage;
- (UIImage *)blackFilledImageWithWhiteBackgroundUsing:(UIImage *)startImage;
- (UIImage *)tabBarBackgroundImageWithSize:(CGSize)targetSize backgroundImage:(UIImage *)backgroundImage;
@end

@implementation LKTabBar
@synthesize buttons, delegate;

- (void)dealloc
{
    [buttons release];
    [super dealloc];
    MLog(@"");
}

- (id)initWithItemCount:(NSUInteger)itemCount itemSize:(CGSize)itemSize tag:(NSInteger)objectTag delegate:(id <LKTabBarDelegate>)customTabBarDelegate
{
    if ((self = [super init]))
    {
        // The tag allows callers with multiple controls to distinguish between them
        self.tag = objectTag;
        
        // Set the delegate
        self.delegate = customTabBarDelegate;
        
        // Add the background image
        UIImage *backgroundImage = [delegate backgroundImage];
        UIImageView *backgroundImageView = [[[UIImageView alloc] initWithImage:backgroundImage] autorelease];
        backgroundImageView.frame = CGRectMake(0, 0, backgroundImage.size.width, backgroundImage.size.height);
        [self addSubview:backgroundImageView];
        
        // Adjust our width based on the number of items & the width of each item
        self.frame = CGRectMake(0, 0, itemSize.width * itemCount, itemSize.height);
        
        // Initalize the array we use to store our buttons
        self.buttons = [NSMutableArray arrayWithCapacity:itemCount];
        
        // horizontalOffset tracks the proper x value as we add buttons as subviews
        CGFloat horizontalOffset = 0;
        
        // Iterate through each item
        for (NSUInteger i = 0 ; i < itemCount ; i++)
        {
            // Create a button
            UIButton *button = [self buttonAtIndex:i width:self.frame.size.width/itemCount];
            
            // Register for touch events
            [button addTarget:self action:@selector(touchDownAction:) forControlEvents:UIControlEventTouchDown];
            [button addTarget:self action:@selector(touchUpInsideAction:) forControlEvents:UIControlEventTouchUpInside];
            [button addTarget:self action:@selector(otherTouchesAction:) forControlEvents:UIControlEventTouchUpOutside];
            [button addTarget:self action:@selector(otherTouchesAction:) forControlEvents:UIControlEventTouchDragOutside];
            [button addTarget:self action:@selector(otherTouchesAction:) forControlEvents:UIControlEventTouchDragInside];
            
            // Set the button's x offset
            button.frame = CGRectMake(horizontalOffset, 0.0, button.frame.size.width, button.frame.size.height);
            
            // Add the button as our subview
            [self addSubview:button];
            
            // Add the button to our buttons array
            [buttons addObject:button];
            
            // Advance the horizontal offset
            horizontalOffset = horizontalOffset + itemSize.width;
        }
    }
    return self;
}


#pragma mark -

#pragma mark button
// Create a button at the provided index
- (UIButton *)buttonAtIndex:(NSUInteger)itemIndex width:(CGFloat)width
{
    // Create a new button with the right dimensions
    UIButton *button = [UIButton buttonWithType:UIButtonTypeCustom];
    button.frame = CGRectMake(0.0, 0.0, width, self.frame.size.height);
    
    // Ask the delegate for the button's image
    UIImage *rawButtonImage = [delegate imageFor:self atIndex:itemIndex];
    // Create the normal state image by converting the image's background to gray
    UIImage *buttonImage = [self tabBarImage:rawButtonImage size:button.frame.size backgroundImage:nil];
    // And create the pressed state image by converting the image's background to the background image we get from the delegate
    UIImage *buttonPressedImage = [self tabBarImage:rawButtonImage size:button.frame.size backgroundImage:[delegate selectedItemBackgroundImage]];
    
    // Set the gray & blue images as the button states
    [button setImage:buttonImage forState:UIControlStateNormal];
    [button setImage:buttonPressedImage forState:UIControlStateHighlighted];
    [button setImage:buttonPressedImage forState:UIControlStateSelected];
    
    // Ask the delegate for the highlighted/selected state image & set it as the selected background state
    [button setBackgroundImage:[delegate selectedItemImage] forState:UIControlStateHighlighted];
    [button setBackgroundImage:[delegate selectedItemImage] forState:UIControlStateSelected];
    
    button.adjustsImageWhenHighlighted = NO;
    
    return button;
}

- (void)dimAllButtonsExcept:(UIButton *)selectedButton
{
    for (UIButton *button in buttons)
    {
        if (button == selectedButton)
        {
            button.selected = YES;
            button.highlighted = button.selected ? NO : YES;
            
            UIImageView *tabBarArrow = (UIImageView *)[self viewWithTag:LKTabBar_TAB_ARROW_IMAGE_TAG];
            NSUInteger selectedIndex = [buttons indexOfObjectIdenticalTo:button];
            if (tabBarArrow)
            {
                [UIView beginAnimations:nil context:nil];
                [UIView setAnimationDuration:LKTabBar_TAB_ARROW_MOVE_DURATION];
                CGRect frame = tabBarArrow.frame;
                frame.origin.x = [self horizontalLocationFor:selectedIndex];
                tabBarArrow.frame = frame;
                [UIView commitAnimations];
            }
            else
            {
                [self addTabBarArrowAtIndex:selectedIndex];
            }
        }
        else
        {
            button.selected = NO;
            button.highlighted = NO;
        }
    }
}

- (void)touchDownAction:(UIButton *)button
{
    [self dimAllButtonsExcept:button];
    
    if (delegate && [delegate respondsToSelector:@selector(touchDownAtItemAtIndex:)])
    {
        [delegate touchDownAtItemAtIndex:[buttons indexOfObject:button]];
    }
}

- (void)touchUpInsideAction:(UIButton *)button
{
    [self dimAllButtonsExcept:button];
    
    if ([delegate respondsToSelector:@selector(touchUpInsideItemAtIndex:)])
    {
        [delegate touchUpInsideItemAtIndex:[buttons indexOfObject:button]];
    }
}

- (void)otherTouchesAction:(UIButton *)button
{
    [self dimAllButtonsExcept:button];
}

- (void)selectItemAtIndex:(NSInteger)index
{
    // Get the right button to select
    UIButton *button = [buttons objectAtIndex:index];
    
    [self dimAllButtonsExcept:button];
}

#pragma mark glow
// Add a glow at the bottom of the specified item
- (void)glowItemAtIndex:(NSInteger)index
{
    // Get the right button. We'll use to calculate where to put the glow
    UIButton *button = [buttons objectAtIndex:index];
    
    // Ask the delegate for the glow image
    UIImage *glowImage = [delegate glowImage];
    
    // Create the image view that will hold the glow image
    UIImageView *glowImageView = [[[UIImageView alloc] initWithImage:glowImage] autorelease];
    
    // Center the glow image at the center of the button horizontally and at the bottom of the button vertically
    glowImageView.frame = CGRectMake(button.frame.size.width/2.0 - glowImage.size.width/2.0, button.frame.origin.y + button.frame.size.height - glowImage.size.height, glowImage.size.width, glowImage.size.height);
    
    // Set the glow image view's tag so we can find it later when we want to remove the glow
    glowImageView.tag = LKTabBar_GLOW_IMAGE_TAG;
    
    // Add the glow image view to the button
    [button addSubview:glowImageView];
}

// Remove the glow at the bottom of the specified item
- (void)removeGlowAtIndex:(NSInteger)index
{
    // Find the right button
    UIButton *button = [buttons objectAtIndex:index];
    // Find the glow image view
    UIImageView *glowImageView = (UIImageView *)[button viewWithTag:LKTabBar_GLOW_IMAGE_TAG];
    // Remove it from the button
    [glowImageView removeFromSuperview];
}


#pragma mark arrow

- (CGFloat)horizontalLocationFor:(NSUInteger)tabIndex
{
    UIImageView *tabBarArrow = (UIImageView *)[self viewWithTag:LKTabBar_TAB_ARROW_IMAGE_TAG];
    
    // A single tab item's width is the entire width of the tab bar divided by number of items
    CGFloat tabItemWidth = self.frame.size.width / buttons.count;
    // A half width is tabItemWidth divided by 2 minus half the width of the arrow
    CGFloat halfTabItemWidth = (tabItemWidth / 2.0) - (tabBarArrow.frame.size.width / 2.0);
    
    // The horizontal location is the index times the width plus a half width
    return (tabIndex * tabItemWidth) + halfTabItemWidth;
}

- (void)addTabBarArrowAtIndex:(NSUInteger)itemIndex
{
    UIImage *tabBarArrowImage = [delegate tabBarArrowImage];
    UIImageView *tabBarArrow = [[[UIImageView alloc] initWithImage:tabBarArrowImage] autorelease];
    tabBarArrow.tag = LKTabBar_TAB_ARROW_IMAGE_TAG;
    // To get the vertical location we go up by the height of arrow and then come back down 2 pixels so the arrow is slightly on top of the tab bar.
    CGFloat verticalLocation = -tabBarArrowImage.size.height + 2;
    tabBarArrow.frame = CGRectMake([self horizontalLocationFor:itemIndex], verticalLocation, tabBarArrowImage.size.width, tabBarArrowImage.size.height);
    
    [self addSubview:tabBarArrow];
}

#pragma mark button

// Create a tab bar image
-(UIImage *)tabBarImage:(UIImage *)startImage size:(CGSize)targetSize backgroundImage:(UIImage *)backgroundImageSource
{
    // The background is either the passed in background image (for the blue selected state) or gray (for the non-selected state)
    UIImage *backgroundImage = [self tabBarBackgroundImageWithSize:startImage.size backgroundImage:backgroundImageSource];
    
    // Convert the passed in image to a white backround image with a black fill
    UIImage *bwImage = [self blackFilledImageWithWhiteBackgroundUsing:startImage];
    
    // Create an image mask
    CGImageRef imageMask = CGImageMaskCreate(CGImageGetWidth(bwImage.CGImage),
                                             CGImageGetHeight(bwImage.CGImage),
                                             CGImageGetBitsPerComponent(bwImage.CGImage),
                                             CGImageGetBitsPerPixel(bwImage.CGImage),
                                             CGImageGetBytesPerRow(bwImage.CGImage),
                                             CGImageGetDataProvider(bwImage.CGImage), NULL, YES);
    
    // Using the mask create a new image
    CGImageRef tabBarImageRef = CGImageCreateWithMask(backgroundImage.CGImage, imageMask);
    
    UIImage *tabBarImage = [UIImage imageWithCGImage:tabBarImageRef scale:startImage.scale orientation:startImage.imageOrientation];
    
    // Cleanup
    CGImageRelease(imageMask);
    CGImageRelease(tabBarImageRef);
    
    // Create a new context with the right size
    UIGraphicsBeginImageContextWithOptions(targetSize, NO, 0.0);
    
    // Draw the new tab bar image at the center
    [tabBarImage drawInRect:CGRectMake((targetSize.width/2.0) - (startImage.size.width/2.0), (targetSize.height/2.0) - (startImage.size.height/2.0), startImage.size.width, startImage.size.height)];
    
    // Generate a new image
    UIImage *resultImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    return resultImage;
}

// Convert the image's fill color to black and background to white
-(UIImage *)blackFilledImageWithWhiteBackgroundUsing:(UIImage *)startImage
{
    // Create the proper sized rect
    CGRect imageRect = CGRectMake(0, 0, CGImageGetWidth(startImage.CGImage), CGImageGetHeight(startImage.CGImage));
    
    // Create a new bitmap context
    CGContextRef context = CGBitmapContextCreate(NULL, imageRect.size.width, imageRect.size.height, 8, 0, CGImageGetColorSpace(startImage.CGImage), kCGImageAlphaPremultipliedLast);
    
    CGContextSetRGBFillColor(context, 1, 1, 1, 1);
    CGContextFillRect(context, imageRect);
    
    // Use the passed in image as a clipping mask
    CGContextClipToMask(context, imageRect, startImage.CGImage);
    // Set the fill color to black: R:0 G:0 B:0 alpha:1
    CGContextSetRGBFillColor(context, 0, 0, 0, 1);
    // Fill with black
    CGContextFillRect(context, imageRect);
    
    // Generate a new image
    CGImageRef newCGImage = CGBitmapContextCreateImage(context);
    UIImage *newImage = [UIImage imageWithCGImage:newCGImage scale:startImage.scale orientation:startImage.imageOrientation];
    
    // Cleanup
    CGContextRelease(context);
    CGImageRelease(newCGImage);
    
    return newImage;
}

-(UIImage *)tabBarBackgroundImageWithSize:(CGSize)targetSize backgroundImage:(UIImage *)backgroundImage
{
    // The background is either the passed in background image (for the blue selected state) or gray (for the non-selected state)
    UIGraphicsBeginImageContextWithOptions(targetSize, NO, 0.0);
    if (backgroundImage)
    {
        // Draw the background image centered
        [backgroundImage drawInRect:CGRectMake((targetSize.width - CGImageGetWidth(backgroundImage.CGImage)) / 2, (targetSize.height - CGImageGetHeight(backgroundImage.CGImage)) / 2, CGImageGetWidth(backgroundImage.CGImage), CGImageGetHeight(backgroundImage.CGImage))];
    }
    else
    {
        [[UIColor lightGrayColor] set];
        UIRectFill(CGRectMake(0, 0, targetSize.width, targetSize.height));
    }
    
    UIImage *finalBackgroundImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    return finalBackgroundImage;
}

@end
