#import "SendEmailViewController.h"

@implementation SendEmailViewController

@synthesize toEmailField;
@synthesize emailBody;

- (BOOL) showInView: (UIView*) parentView {
	_parentView = parentView;
	[_parentView addSubview:self];
}

- (IBAction)cancelClicked {
	[_parentView removeFromSuperview:self];
}

- (IBAction)sendEmailClicked {
    [_parentView removeFromSuperview:self];
}

- (void)dealloc {
	[emailBody release];
	[toEmailField release];
	[super dealloc];
}

@end
