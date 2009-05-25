#import "AppController.h"
#import "PreferenceController.h"

@implementation AppController

+ (void)initialize
{
	NSMutableDictionary *defaultValues = [NSMutableDictionary dictionary];
	NSData *colorAsData = [NSKeyedArchiver archivedDataWithRootObject:
						   [NSColor yellowColor]];
	[defaultValues setObject:colorAsData forKey:BNRTableBgColorKey];
	[defaultValues setObject:[NSNumber numberWithBool:YES]
					  forKey:BNREmptyDocKey];
	[[NSUserDefaults standardUserDefaults] registerDefaults:defaultValues];
	NSLog(@"registered %@", defaultValues);
}

- (IBAction)showPreferencePanel:(id)sender
{
	// Is preferenceController nil?
	if (!preferenceController) {
		preferenceController = [[PreferenceController alloc] init];
	}
	NSLog(@"Showing %@", preferenceController);
	[preferenceController showWindow:self];
}

- (BOOL)applicationShouldOpenUntitledFile:(NSApplication *)sender
{
	NSLog(@"applicationShouldOpenUntitledFile:");
	return [[NSUserDefaults standardUserDefaults] boolForKey:BNREmptyDocKey];
	
}


@end
