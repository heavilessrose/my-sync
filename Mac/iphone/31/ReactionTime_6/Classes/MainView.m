#import "MainView.h"

@implementation MainView

@synthesize startDate;

int greenLightOn = 0;

-(void)awakeFromNib {
	
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Reaction Time: Ready to Play" message:@"Hit the gas pedal as fast as you can after the light turns green."
												   delegate:self cancelButtonTitle:@"Play" otherButtonTitles: nil];
	[alert show];
}

- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
	stopLight.image = [UIImage imageNamed:@"yellowLightSmall.png"];
	greenLightOn = 0;
	
	[NSTimer scheduledTimerWithTimeInterval:(3.0) target:self selector:@selector(onYellowLightTimer) userInfo:nil repeats:NO];
}

- (void)onYellowLightTimer
{
	stopLight.image = [UIImage imageNamed:@"redLightSmall.png"];
	
	int delay = ((int) (random() % 7) + 1);

	[NSTimer scheduledTimerWithTimeInterval:(3.0 + delay) target:self selector:@selector(onRedLightTimer) userInfo:nil repeats:NO];
}

- (void)onRedLightTimer
{
 	stopLight.image = [UIImage imageNamed:@"greenLightSmall.png"];
	
	greenLightOn = 1;
	
	self.startDate = [NSDate date];
}

- (IBAction)gasPedalPressed 
{
	double noSeconds = (double) [self.startDate timeIntervalSinceNow] * -1000;
	
	NSString *reactionTime= [[NSString alloc] initWithFormat:@"Good Job! You're reaction time is %1.0f milliseconds. Let's see if you can do better...", noSeconds];

	if(greenLightOn == 0)
		reactionTime = @"Slow down! You have to wait for the green light. Let's see if you can do better...";
	
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Reaction Time" message:reactionTime 
												   delegate:self cancelButtonTitle:@"OK" otherButtonTitles: nil];
	[alert show];	
}

@end
