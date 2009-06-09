#import "MainView.h"

#define RANDOM_SEED() srandom(time(NULL))
#define RANDOM_INT(__MIN__, __MAX__) ((__MIN__) + random() % ((__MAX__+1) - (__MIN__)))

@implementation MainView

@synthesize phoneticPassword;
@synthesize passwordLength;
@synthesize emailPasswordButton;
@synthesize emailAddress;

- (id)initWithFrame:(CGRect)frame {
	if (self = [super initWithFrame:frame]) {
		
	}
	return self;
}

- (IBAction)setPasssword {
	
	[UIView beginAnimations:nil context:NULL];
	[UIView setAnimationDuration:1];
	[password setAlpha:0.0];
	[phoneticPassword setAlpha:0.0];
	[emailPasswordButton setAlpha:0.0];
	[UIView commitAnimations];
	
	[UIView beginAnimations:nil context:NULL];
	[UIView setAnimationDuration:1];
	[password setAlpha:0.85];
	if(showPhonetics.on) {[phoneticPassword setAlpha:0.85];}
	[emailPasswordButton setAlpha:0.85];
	[UIView commitAnimations];
	
	
	NSArray *keys = [NSArray arrayWithObjects:
					@"a", @"b", @"c", @"d", @"e",
					@"f", @"g", @"h", @"i", @"j",
					@"k", @"l", @"m", @"n", @"o",
					@"p", @"q", @"r", @"s",	@"t",
					@"u", @"v", @"w", @"x",	@"y", @"z",

					@"A", @"B", @"C", @"D", @"E",
					@"F", @"G", @"H", @"I", @"J",
					@"K", @"L", @"M", @"N", @"O",
					@"P", @"Q", @"R", @"S", @"T",
					@"U", @"V", @"W", @"X", @"Y", @"Z",

					@"0", @"1", @"2", @"3", @"4", 
					@"5", @"6", @"7", @"8", @"9",

					@"~", @"!", @"?", @"#", @"%", 
					@"^", @"&", @"*", @"(", @")",

					 nil];
	
	NSArray *objects = [NSArray arrayWithObjects:
					@"alpha", @"bravo", @"charlie", @"delta", @"echo",
					@"foxtrot", @"golf", @"hotel", @"india", @"juliet",
					@"kilo", @"lima", @"mike", @"november", @"oscar",
					@"papa", @"quebec", @"romeo", @"sierra", @"tango",
					@"uniform", @"victor", @"whiskey", @"xray", @"yankee", @"zulu",
						
					@"ALPHA", @"BRAVO", @"CHARLIE", @"DELTA", @"ECHO",
					@"FOXTROT", @"GOLF", @"HOTEL", @"INDIA", @"JULIET",
					@"KILO", @"LIMA", @"MIKE", @"NOVEMBER", @"OSCAR",
					@"PAPA", @"QUEBEC", @"ROMEO", @"SIERRA", @"TANGO",
					@"UNIFORM", @"VICTOR", @"WHISKEY", @"XRAY", @"YANKEE", @"ZULU",
						
					@"Zero", @"One", @"Two", @"Three", @"Four", 
					@"Five", @"Six", @"Seven", @"Eight", @"Nine",
						
					@"Tilda", @"Exclamation Point", @"Question Mark", @"Pound Sign", @"Percent Sign", 
					@"Carrot Sign", @"Ampersand", @"Asterick", @"Left Parenthesis", @"Right Parenthesis",
						
					nil];

	NSDictionary *dictionary = [NSDictionary dictionaryWithObjects:objects forKeys:keys];
	
	NSInteger iPassswordLength = [passwordLength.text intValue];
	BOOL bShowPhonetics = showPhonetics.on;
	BOOL bIncludeLowerCase = includeLowerCase.on;
	BOOL bIncludeUpperCase = includeUpperCase.on;
	BOOL bIncludeNumbers = includeNumbers.on;
	BOOL bIncludePunctuation = includePunctuation.on;
	
	NSString* passwordText = @"";
	NSString* phoneticText = @"";

	NSString* lowercaseChars = @"abcdefghijklmnopqrstuvwxyz";
	NSString* uppercaseChars = @"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	NSString* numbersChars = @"0123456789";
	NSString* punctuationChars = @"~!?#%^&*()";
	
	RANDOM_SEED();

	if(!bIncludeLowerCase && !bIncludeUpperCase && !bIncludeNumbers && !bIncludePunctuation)
	{
		UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"PasswordGen" message:@"No password character set selected."
													   delegate:self cancelButtonTitle:@"OK" otherButtonTitles: nil];
		[alert show];	
		[alert release];

		return;
	}
	
	NSString* passwordChars = @"";
	
	if(bIncludeLowerCase)
		passwordChars = [NSString stringWithFormat:@"%@%@", passwordChars, lowercaseChars];
	
	if(bIncludeUpperCase)
		passwordChars = [NSString stringWithFormat:@"%@%@", passwordChars, uppercaseChars];
	
	if(bIncludeNumbers)
		passwordChars = [NSString stringWithFormat:@"%@%@", passwordChars, numbersChars];
	
	if(bIncludePunctuation)
		passwordChars = [NSString stringWithFormat:@"%@%@", passwordChars, punctuationChars];
	
	for(NSInteger i=0; i<iPassswordLength; i++) {
		
		int index = RANDOM_INT(0, [passwordChars length]-1);

		NSRange range = NSMakeRange(index, 1);
		NSString *passwordChar = [passwordChars substringWithRange:range];	
		
		passwordText = [NSString stringWithFormat:@"%@%@", passwordText, passwordChar];
		
		NSLog(passwordChar);
		
		if(i>0)
			phoneticText = [NSString stringWithFormat:@"%@, %@", phoneticText, [dictionary valueForKey:passwordChar]];
		else
			phoneticText = [dictionary valueForKey:passwordChar];
	}
	
	password.text = @"";
	password.text = passwordText;
	
	phoneticPassword.text = @"";
	
	if(bShowPhonetics)
		phoneticPassword.text = phoneticText;
}

-(IBAction) resignResponder {
	[password resignFirstResponder];
	[passwordLength resignFirstResponder];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
	if (textField == password) {
		[password resignFirstResponder];
	}
	if (textField == passwordLength) {
		[passwordLength resignFirstResponder];
	}
	return YES;
}

-(IBAction) emailPassword {
	
	NSString *urlString = @"mailto:?subject=Password%20Generator&body=";
	urlString = [urlString stringByAppendingString:@"Password:%20"];
	urlString = [urlString stringByAppendingString:password.text];
	if(showPhonetics.on) {
		urlString = [urlString stringByAppendingString:@"%20Phonetic:%20"];
		urlString = [urlString stringByAppendingString:[phoneticPassword.text stringByReplacingOccurrencesOfString:@" " withString:@"%20"]];
	}
	
	NSURL* mailURL = [NSURL URLWithString: urlString]; 
	[[UIApplication sharedApplication] openURL: mailURL]; 
}


@end
