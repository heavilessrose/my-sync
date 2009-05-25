#import "BallView.h"


@implementation BallView
@synthesize image;
@dynamic currentPoint;
@synthesize previousPoint;
@synthesize acceleration;
@synthesize ballXVelocity;
@synthesize ballYVelocity;

- (id)initWithCoder:(NSCoder *)coder {
	
	if (self = [super initWithCoder:coder]) {
		self.image = [UIImage imageNamed:@"ball.png"];
		self.currentPoint = CGPointMake((self.bounds.size.width / 2) + (image.size.width / 2), (self.bounds.size.height / 2) + (image.size.height / 2));
		
		ballXVelocity = 0.0f;
		ballYVelocity = 0.0f;
	}
	return self;
}

- (id)initWithFrame:(CGRect)frame {
	if (self = [super initWithFrame:frame]) {
		// Initialization code
	}
	return self;
}


- (void)drawRect:(CGRect)rect {
	
	[image drawAtPoint:currentPoint];
	
	
}

- (CGPoint)currentPoint {
	
	return currentPoint;
}
- (void)draw {
	static NSDate *lastDrawTime;
	
	if (lastDrawTime != nil) {
		NSTimeInterval secondsSinceLastDraw = -([lastDrawTime timeIntervalSinceNow]);
		
		ballYVelocity = ballYVelocity + -(acceleration.y * secondsSinceLastDraw);
		ballXVelocity = ballXVelocity + acceleration.x * secondsSinceLastDraw;
		
		CGFloat xAcceleration = secondsSinceLastDraw * ballXVelocity * kVelocityMultiplier;
		CGFloat yAcceleration = secondsSinceLastDraw * ballYVelocity * kVelocityMultiplier;
		
		self.currentPoint = CGPointMake(self.currentPoint.x + xAcceleration, self.currentPoint.y +yAcceleration);
	}
	// Update last time with current time
	[lastDrawTime release];
	lastDrawTime = [[NSDate alloc] init];
	
}
- (void)setCurrentPoint:(CGPoint)newPoint {
	previousPoint = currentPoint;
	currentPoint = newPoint;
	
	if (currentPoint.x < 0) {
		currentPoint.x = 0;
		ballXVelocity = 0;
	}
	if (currentPoint.y < 0){
		currentPoint.y = 0;
		ballYVelocity = 0;
	} 
	
	if (currentPoint.x > self.bounds.size.width - image.size.width) {
		currentPoint.x = self.bounds.size.width  - image.size.width; 
		ballXVelocity = 0;
	}
	
	if (currentPoint.y > self.bounds.size.height - image.size.height) {
		currentPoint.y = self.bounds.size.height - image.size.height;
		ballYVelocity = 0;
	}
	
	CGRect currentImageRect = CGRectMake(currentPoint.x, currentPoint.y, currentPoint.x + image.size.width, currentPoint.y + image.size.height);
	CGRect previousImageRect = CGRectMake(previousPoint.x, previousPoint.y, previousPoint.x + image.size.width,  currentPoint.y + image.size.height);
	[self setNeedsDisplayInRect:CGRectUnion(currentImageRect, previousImageRect)];
	
}
- (void)dealloc {
	[image release];
	[acceleration release];
	[super dealloc];
}

@end