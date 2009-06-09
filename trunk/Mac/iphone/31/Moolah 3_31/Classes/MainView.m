//
//  MainView.m
//  DrawTest
//
//  Created by AppsAmuck on 8/26/08.
//  Copyright Amuck LLC 2008. All rights reserved.
//

#import "MainView.h"
#define RANDOM_SEED() srandom(time(NULL))
#define RANDOM_INT(__MIN__, __MAX__) ((__MIN__) + random() % ((__MAX__+1) - (__MIN__)))
// Constant for the number of times per second (Hertz) to sample acceleration.
#define kAccelerometerFrequency     40

@implementation MainView

static int MAX_BILLS = 100;

-(void)setEmitterPosition: (CGPoint)position {
	int width = 100*2;
	int height = 44*2;
	emitterRect = CGRectMake(position.x - (width/2),  position.y-(height/2), width, height);
	useEmitterRect = YES;
}

-(void)clearEmitter {
	useEmitterRect = NO;
}

- (void)onHelpViewAnimationComplete:(NSString *)animationID finished:(NSNumber *)finished context:(void *)context {
	UIImageView *imageView = context;
	[imageView removeFromSuperview];
	helpView = nil;
}

-(void)awakeFromNib {
	
	self.backgroundColor = [UIColor blackColor];
	billCount = 0;
	billList = [[NSMutableArray alloc] init];
	
	UIImage *helpImage = [UIImage imageNamed:@"infobox.png"];
	helpView = [[UIImageView alloc] initWithImage:helpImage];
	[helpImage release];
	helpView.frame = self.frame;
	[self addSubview:helpView];
	[UIView beginAnimations:nil context:helpView];
	[UIView setAnimationDelay:5.0];
	[UIView setAnimationDuration:0.5];
	[helpView setAlpha:0.0];	
	[UIView setAnimationDelegate:self];
	[UIView setAnimationDidStopSelector:@selector(onHelpViewAnimationComplete:finished:context:)];
	[UIView commitAnimations];
	[helpView release];
	
	[[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1.0 / kAccelerometerFrequency)];
    [[UIAccelerometer sharedAccelerometer] setDelegate:self];

	[NSTimer scheduledTimerWithTimeInterval:(0.1) target:self selector:@selector(onTimer) userInfo:nil repeats:YES];
}

- (void)onTimer
{
	
	
	UIImageView *dollarView;
	UIImage *dollarImage;
	
	// for effect... make it harder to get some of the other bill index's
	// if it does not pass criteria reset back to 1
	
	// double tap upside down for 5 Dollar bill
	if(billIndex > 2) {
		if (deviceTilt.y > 0.5f)
			billIndex = 5;
		else
			billIndex = 1;
	}
	else if(billIndex > 1) {
		if (deviceTilt.x > 0.5f) 
			billIndex = 3;
		else if (deviceTilt.x < -0.5f)
			billIndex = 4;
		else
			billIndex = 2;
			
	}
	 
	
	switch (billIndex) {
		case 2:
			dollarImage = [UIImage imageNamed:@"dol2.png"];
			break;
		case 3:
			dollarImage = [UIImage imageNamed:@"5dol.jpg"];
			break;
		case 4:
			dollarImage = [UIImage imageNamed:@"10dol.jpg"];
			break;
		case 5:
			dollarImage = [UIImage imageNamed:@"20dol.jpg"];
			break;			
		default:
			dollarImage = [UIImage imageNamed:@"dollar.png"];
			break;
	}
	
	// for effect only draw one 2 dollar bill by resetting billIndex
	if (billIndex == 2)
		billIndex = 1;
	
	dollarView = [[UIImageView alloc] initWithImage:dollarImage];
	int scale = RANDOM_INT(1, 5);
	int width = 100*scale;
	int height = 44*scale;
	if(useEmitterRect) {
		dollarView.frame = emitterRect;
	//} else if (deviceTilt.x > 0.5f || deviceTilt.y > 0.5f || deviceTilt.x < -0.5f || deviceTilt.y < -0.5f) {
	} else if (deviceTilt.y > 0.5f) {
		dollarView.frame = CGRectMake(160 - (width/2), 490, width, height);
	} else if (deviceTilt.y < -0.5f) {
		dollarView.frame = CGRectMake(160 - (width/2), -(height+10), width, height);
	} else if (deviceTilt.x > 0.5f) {
		dollarView.frame = CGRectMake(-(width+10), 240-(height/2), width, height);
	} else if (deviceTilt.x < -0.5f) {
		dollarView.frame = CGRectMake(330, 240-(height/2), width, height);
	} else {
		switch (RANDOM_INT(0, 3)) {
			case 0:
				dollarView.frame = CGRectMake(-300, -300, 100*scale, 44*scale);
				break;
			case 1:
				dollarView.frame = CGRectMake(470, -150, 100*scale, 44*scale);
				break;
			case 3:
				dollarView.frame = CGRectMake(470, 630, 100*scale, 44*scale);
				break;			
			default:
				dollarView.frame = CGRectMake(-150, 630, 100*scale, 44*scale);
				break;
		}
	}
	
	[self addSubview:dollarView];
	if(helpView)
		[self bringSubviewToFront:helpView];
	
	[UIView beginAnimations:nil context:dollarView];
	[UIView setAnimationDuration:2];


	dollarView.frame = CGRectMake(RANDOM_INT(-50, 370), RANDOM_INT(-25, 665), 100, 44);
		
	int radians = RANDOM_INT(0, 360) * 0.0174532925;		
	[dollarView setTransform:CGAffineTransformMakeRotation(radians)];
	
	[UIView setAnimationDelegate:self];
	[UIView commitAnimations];
	
	billCount++;
	[billList addObject:dollarView];
	if([billList count] >= MAX_BILLS) {
		UIImageView* oldBill = (UIImageView*)[billList objectAtIndex:0];
		[billList removeObjectAtIndex:0];
		
		[UIView beginAnimations:nil context:oldBill];
		
		[UIView setAnimationDuration:2];
		[oldBill setAlpha:0.0];
		
		[UIView setAnimationDelegate:self];
		[UIView setAnimationDidStopSelector:@selector(goPutThemBack:finished:context:)];
		[UIView commitAnimations];
	}
	
}

- (void)goPutThemBack:(NSString *)animationID finished:(NSNumber *)finished context:(void *)context {
	
	UIImageView *dollarView2 = context;
	
	[dollarView2 removeFromSuperview];
}

- (id)initWithFrame:(CGRect)frame {
	if (self = [super initWithFrame:frame]) {
		useEmitterRect = NO;
	}
	return self;
}

- (void)drawRect:(CGRect)rect {
}

+(UIImage *)rotateImage:(UIImage *)image {
	
	UIImageView *imageView = [[UIImageView alloc] init];
	
	UIImage *imageCopy = [[UIImage alloc] initWithCGImage:image.CGImage];

	imageView.transform = CGAffineTransformMakeRotation(1.57079633);
	
	imageView.image = imageCopy;
	return (imageView.image);
}

- (void)dealloc {
	[billList release];
	[super dealloc];
}


#pragma mark -
#pragma mark === Tilt handling  ===
#pragma mark
// UIAccelerometerDelegate method, called when the device accelerates.
- (void)accelerometer:(UIAccelerometer *)accelerometer didAccelerate:(UIAcceleration *)acceleration {
    // Update the accelerometer graph view
	deviceTilt.x = acceleration.x;
	deviceTilt.y = acceleration.y;
}

#pragma mark -
#pragma mark === Touch handling  ===
#pragma mark

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event  {
	for (UITouch *touch in touches){
		int newBillIndex = touch.tapCount;
		if(newBillIndex > billIndex)
			billIndex = newBillIndex;
		[self setEmitterPosition: [touch locationInView:self]];
		//only care about the first touch so break camp
		return;
	}
	if([touches count] == 5)
		billIndex == 5;
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
	for (UITouch *touch in touches){
		[self setEmitterPosition: [touch locationInView:self]];
		//only care about the first touch so break camp
		return;
	}
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
	[self clearEmitter];
}

@end
