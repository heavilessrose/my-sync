/*

===== IMPORTANT =====

This is sample code demonstrating API, technology or techniques in development.
Although this sample code has been reviewed for technical accuracy, it is not
final. Apple is supplying this information to help you plan for the adoption of
the technologies and programming interfaces described herein. This information
is subject to change, and software implemented based on this sample code should
be tested with final operating system software and final documentation. Newer
versions of this sample code may be provided with future seeds of the API or
technology. For information about updates to this and other developer
documentation, view the New & Updated sidebars in subsequent documentation
seeds.

=====================

File: HighScoresView.m
Abstract: Subclass of UIView that handles high scores.

Version: 2.0

Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple Inc.
("Apple") in consideration of your agreement to the following terms, and your
use, installation, modification or redistribution of this Apple software
constitutes acceptance of these terms.  If you do not agree with these terms,
please do not use, install, modify or redistribute this Apple software.

In consideration of your agreement to abide by the following terms, and subject
to these terms, Apple grants you a personal, non-exclusive license, under
Apple's copyrights in this original Apple software (the "Apple Software"), to
use, reproduce, modify and redistribute the Apple Software, with or without
modifications, in source and/or binary forms; provided that if you redistribute
the Apple Software in its entirety and without modifications, you must retain
this notice and the following text and disclaimers in all such redistributions
of the Apple Software.
Neither the name, trademarks, service marks or logos of Apple Inc. may be used
to endorse or promote products derived from the Apple Software without specific
prior written permission from Apple.  Except as expressly stated in this notice,
no other rights or licenses, express or implied, are granted by Apple herein,
including but not limited to any patent rights that may be infringed by your
derivative works or by other works in which the Apple Software may be
incorporated.

The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
COMBINATION WITH YOUR PRODUCTS.

IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR
DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF
CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF
APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Copyright (C) 2008 Apple Inc. All Rights Reserved.

*/

#import "HighScoresView.h"
#import "GameObjectView.h"
#import "SpaceShip.h"

#define kMaxScoreCount						7
#define kStatusFontSize						16
#define kFontName							@"Arial"

#define kScoresDefaultKey					@"scores"
#define kUserNameDefaultKey					@"name"


@implementation HighScoresView

- (void) _updateLabels
{
	NSUInteger i;

	for (i=0; i<MIN(kMaxScoreCount, [_savedScores count]); ++i) {
		[(UILabel*)[_names.subviews objectAtIndex:i] setText:[[_savedScores objectAtIndex:i] objectAtIndex:0]];
		[(UILabel*)[_scores.subviews objectAtIndex:i] setText:[NSString stringWithFormat:@"%i", [(NSNumber*)[[_savedScores objectAtIndex:i] objectAtIndex:1] unsignedIntValue]]];
	}
}

- (id)initWithFrame:(CGRect)frame {
	UIImage*				image;
	UIImageView*			imageView;
	UIView*					view;
	NSArray*				nibContents;
	//GameObject*				object;
		
	if ((self = [super initWithFrame:frame])) {
		//Once movie is over, show the "Winners don't use drugs" screen by adding a UIImage view whose content is set to Winner.png
		image = [[UIImage alloc] initWithContentsOfFile:[[NSBundle mainBundle]  pathForResource:@"Winner" ofType:@"png"]];
		imageView = [[UIImageView alloc] initWithFrame:frame];
		imageView.image = image;
		[self addSubview:imageView];

		//Release temporary resources
		[image release];
		[imageView release];

		/*_spaceShipView = [[GameObjectView alloc] initWithFrame:frame pixelFormat:kEAGLColorFormatRGB565 depthFormat:GL_DEPTH_COMPONENT24_OES preserveBackbuffer:NO];
		object = [[SpaceShip alloc] initWithType:kSpaceShipTypeDart];
		_spaceShipView.gameObject = object;
		_spaceShipView.hasReflection = YES;
		[object release];
		[self addSubview:_spaceShipView];		
		*/
		
		nibContents = [[NSBundle mainBundle] loadNibNamed:@"HighScores" owner:self options:nil];
		if ([nibContents count]>1) {
			view = [nibContents objectAtIndex:1];
			view.center = CGPointMake(frame.size.width*1./2., frame.size.height*3./2.);
			[self addSubview:view];
			
			if (![[NSUserDefaults standardUserDefaults] objectForKey:kScoresDefaultKey])
			_savedScores = [[NSArray arrayWithObjects:[NSArray arrayWithObjects:	@"Steve", [NSNumber numberWithUnsignedInt:700], nil],
													[NSArray arrayWithObjects:	@"Al", [NSNumber numberWithUnsignedInt:600], nil],
													[NSArray arrayWithObjects:	@"Eric", [NSNumber numberWithUnsignedInt:500], nil],
													[NSArray arrayWithObjects:	@"Andrea", [NSNumber numberWithUnsignedInt:400], nil],
													[NSArray arrayWithObjects:	@"Millard", [NSNumber numberWithUnsignedInt:300], nil],
													[NSArray arrayWithObjects:	@"Bill", [NSNumber numberWithUnsignedInt:200], nil],
													[NSArray arrayWithObjects:	@"Jerry", [NSNumber numberWithUnsignedInt:100], nil], nil] retain];
			else
			_savedScores = [[[NSUserDefaults standardUserDefaults] objectForKey:kScoresDefaultKey] retain];
			
			[self _updateLabels];
		}
	}
	return self;
}

- (void) addName:(NSString*)name score:(NSUInteger)score
{
	NSMutableArray*			array;
	NSInteger				i;
	
	if (score > [[[_savedScores objectAtIndex:[_savedScores count]-1] objectAtIndex:1] unsignedIntValue]) {
		array = [NSMutableArray arrayWithArray:_savedScores];
		[array removeObjectAtIndex:[_savedScores count]-1];
	
		if (!name)
		name = [[NSUserDefaults standardUserDefaults] stringForKey:kUserNameDefaultKey];
		if(!name) {
			[[NSUserDefaults standardUserDefaults] setObject:@"" forKey:kUserNameDefaultKey];
			name = @"";
		}

		for (i=[array count]-1; i>=-1; --i) {
			if ((i==-1) || (score < [[[array objectAtIndex:i] objectAtIndex:1]  unsignedIntValue])) {
				[array insertObject:[NSArray arrayWithObjects:name, [NSNumber numberWithUnsignedInt:score], nil] atIndex:i+1];
				_rank = i+2;
				break;
			}
		}
		[_savedScores release];
		_savedScores = [array retain];
		
		[[NSUserDefaults standardUserDefaults] setObject:_savedScores forKey:kScoresDefaultKey];

		[self _updateLabels];
	}
}

- (void)textFieldDidEndEditing:(UITextField*)textField {
	[[NSUserDefaults standardUserDefaults] setObject:[textField text] forKey:kUserNameDefaultKey];
	
	[(NSMutableArray*)_savedScores replaceObjectAtIndex:_rank-1 withObject:[NSArray arrayWithObjects:[textField text], [[_savedScores objectAtIndex:_rank-1] objectAtIndex:1], nil]];
	
	[[NSUserDefaults standardUserDefaults] setObject:_savedScores forKey:kScoresDefaultKey];

	[self _updateLabels];	
}

- (BOOL)textFieldShouldReturn:(UITextField*)textField {
	[textField resignFirstResponder];
	
	return YES;
}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
	_rank = 0;
}

- (void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	UITextField*			textField;
	UIAlertView*			alertView;
	
	[UIView beginAnimations:nil context:NULL];
	((UIView*)[[self subviews] objectAtIndex:1]).center = CGPointMake(self.frame.size.height*1./2., self.frame.size.width*1./2.);
	[UIView commitAnimations];
	
	//Update user name if needed
	if (_rank) {	
		textField = [[UITextField alloc] initWithFrame:CGRectMake(0., 0., 130, 25)];
		textField.borderStyle = UITextBorderStyleBezel;
		[textField setDelegate:self];
		[textField setBackgroundColor:[UIColor colorWithWhite:1.0 alpha:.5]];
		[textField setTextColor:[UIColor blackColor]];
		[textField setFont:[UIFont fontWithName:kFontName size:kStatusFontSize]];
		[textField setPlaceholder:@"Tap to edit"];
		[textField setText:[[NSUserDefaults standardUserDefaults] stringForKey:kUserNameDefaultKey]];

		alertView = [[UIAlertView alloc] initWithTitle:@"Congratulations!" message:nil delegate:self cancelButtonTitle:nil otherButtonTitles:@"OK", nil];
		[alertView setDelegate:self];
		[alertView setMessage:[NSString stringWithFormat:@"TouchFighter                                 \n entered the High Scores\n at rank %i!", _rank]];
		textField.center = CGPointMake(200., 55.);
		[alertView addSubview:textField];
		[textField release];
		[alertView show];
		[alertView release];
	}	
}

- (void)dealloc 
{
	[_savedScores release];
	[_spaceShipView release];
	[super dealloc];
}


@end
