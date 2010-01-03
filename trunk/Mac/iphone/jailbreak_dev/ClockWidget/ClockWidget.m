/*
 SmartScreen widget is a loadable bundle (plugin) with '.wdgt' extension. Principal class of bundle should be subclass of UIViewController class and conform to protocol:
 
 Bundle resources should have at least two files.
 1. Icon.png - widget icon, size is 48 x 48 pixels.
 2. PreferenceSpecifiers.plist - widget preference specifiers file.
 
 Make classes with unique names (i.e. based on your widget or company name), same goes for bundle principal class name.
 All widgets are running in same address space, so two widgets with identical class names may cause a conflict.
 Example, WidgetView - bad class name, MediaPhoneCalendarWidgetView - good class name.
 */

#import "ClockWidget.h"


@implementation ClockWidget

-(id) init
{
    self = [super init];
    
    _timeFormatter = [[NSDateFormatter alloc] init];
    _dateFormatter = [[NSDateFormatter alloc] init];
    
    return self;
}

-(void) dealloc
{
    [_timer invalidate];
    _timer = nil;

    [_timeFormatter release];
    _timeFormatter = nil;

    [_dateFormatter release];
    _dateFormatter = nil;
    
    [super dealloc];
}

-(void) loadView
{
// Create only view and controls here
    UIView* view = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 320, 70)];
    
    ivBackground = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, 320, 70)];
    [view addSubview:ivBackground];
    [ivBackground release];
    
    lTime = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, 320, 50)];
    lTime.textColor = [UIColor whiteColor];
    lTime.textAlignment = UITextAlignmentCenter;
    lTime.backgroundColor = [UIColor clearColor];
    [view addSubview:lTime];
    [lTime release];

    lDate = [[UILabel alloc] initWithFrame:CGRectMake(0, 60, 320, 20)];
    lDate.textColor = [UIColor whiteColor];
    lDate.textAlignment = UITextAlignmentCenter;
    lDate.backgroundColor = [UIColor clearColor];
    [view addSubview:lDate];
    [lDate release];
    
    self.view = view;
    [view release];
}

-(CGSize) widgetSize
{
// Return widget size in pixels. Recommended widget width is 320 pixel and height is 70, 140, 210, 280 or 350 pixels.
    return CGSizeMake(320, 70);
}

-(void) loadWidget
{
// Called on widget load. Should be used to initialize widget information and start update loop.
// Do not put time consuming code here - use NSOperation for asynchronous code execution
    
    [self loadPreferences];
    
    if([[_preferences objectForKey:@"ShowBackground"] boolValue])
        ivBackground.image = [UIImage imageWithContentsOfFile:[[NSBundle bundleForClass:[self class]] pathForResource:@"Background" ofType:@"png"]];
    
    lTime.hidden = [[_preferences objectForKey:@"TimeHidden"] boolValue];

    if(!lTime.hidden)
    {
        UIFont* font = nil;
        NSString* fontName = [_preferences objectForKey:@"TimeFontName"];
        CGFloat fontSize = [[_preferences objectForKey:@"TimeFontSize"] floatValue];

        if(fontName)
            font = [UIFont fontWithName:fontName size:fontSize];

        if(font == nil)
            font = [UIFont systemFontOfSize:[UIFont systemFontSize]];

        lTime.font = font;
        lTime.textColor = [UIColor colorWithRGB:[[_preferences objectForKey:@"TimeColor"] unsignedIntValue]];
        lTime.numberOfLines = 2;
        
        [_timeFormatter setDateFormat:[_preferences objectForKey:@"TimeFormat"]];
    }

    lDate.hidden = [[_preferences objectForKey:@"DateHidden"] boolValue];    

    if(!lDate.hidden)
    {
        UIFont* font = nil;
        NSString* fontName = [_preferences objectForKey:@"DateFontName"];
        CGFloat fontSize = [[_preferences objectForKey:@"DateFontSize"] floatValue];
    
        if(fontName)
            font = [UIFont fontWithName:fontName size:fontSize];
        
        if(font == nil)
            font = [UIFont systemFontOfSize:[UIFont systemFontSize]];
        
        lDate.font = font;
        lDate.textColor = [UIColor colorWithRGB:[[_preferences objectForKey:@"DateColor"] unsignedIntValue]];
        lDate.numberOfLines = 2;

        [_dateFormatter setDateFormat:[_preferences objectForKey:@"DateFormat"]];
    }

    [self updateControls];
    
    if(!lTime.hidden)
    {
        CGRect frame = lTime.frame;
        frame.origin.y += [[_preferences objectForKey:@"TimeOffset"] doubleValue];
        frame.size.height = [lTime.text sizeWithFont:lTime.font constrainedToSize:CGSizeMake(320, 70) lineBreakMode:UILineBreakModeWordWrap].height;
        lTime.frame = frame;
    }

    if(!lDate.hidden)
    {
        CGRect frame = lDate.frame;
        if(lTime.hidden)
            frame.origin.y = 0;
        else
            frame.origin.y = CGRectGetMaxY(lTime.frame);
        frame.origin.y += [[_preferences objectForKey:@"DateOffset"] doubleValue];
        frame.size.height = [lDate.text sizeWithFont:lDate.font constrainedToSize:CGSizeMake(320, 70) lineBreakMode:UILineBreakModeWordWrap].height;;
        lDate.frame = frame;
    }
    
    _timer = [NSTimer scheduledTimerWithTimeInterval:1 target:self selector:@selector(timeFire:) userInfo:nil repeats:YES];
}

-(void) unloadWidget
{
// Called on widget unload. Should be used to release widget information and stop update loop.
// Do not put time consuming code here - use NSOperation for asynchronous code execution
    
    [_timer invalidate];
    _timer = nil;

// Save widget settings back to disk
    [self savePreferences];
}

-(void) pauseWidget
{
// Pause information updates to save battery power
    [_timer invalidate];
    _timer = nil;
}

-(void) resumeWidget
{
// Resume information updates    
// Don not put time consuming code here - use NSOperation for asynchronous code execution
    [self updateControls];
    
    _timer = [NSTimer scheduledTimerWithTimeInterval:1 target:self selector:@selector(timeFire:) userInfo:nil repeats:YES];
}

-(void) timeFire:(NSTimer*) timer
{
    [self updateControls];
}

-(void) loadPreferences
{
    // Use Preferences.plist inside your widget bundle to keep widget settings
    // This file is also used by SmartScreen Preferences tool
    NSString* preferencesPath = [[[NSBundle bundleForClass:[self class]] resourcePath] stringByAppendingPathComponent:@"Preferences.plist"];
    
    _preferences = [NSMutableDictionary dictionaryWithContentsOfFile:preferencesPath];
    if(_preferences == nil)
        _preferences = [NSMutableDictionary dictionary];
    
    // Use PreferenceSpecifiers.plist inside your widget bundle to keep default widget settings
    // This file is also used by SmartScreen Preferences tool
    NSString* preferenceSpecifiersPath = [[NSBundle bundleForClass:[self class]] pathForResource:@"PreferenceSpecifiers" ofType:@"plist"];
    NSArray* preferenceSpecifiers = [NSArray arrayWithContentsOfFile:preferenceSpecifiersPath];
    for(NSDictionary* specifier in preferenceSpecifiers)
    {
        NSString* key = [specifier objectForKey:@"Key"];
        NSString* value = [specifier objectForKey:@"DefaultValue"];
        
        if(key && value)
        {
            // Apply default values for missing settings
            if([_preferences objectForKey:key] == nil)
                [_preferences setObject:value forKey:key];
        }
    }

    [_preferences retain];
}

-(void) savePreferences
{
    NSString* preferencesPath = [[[NSBundle bundleForClass:[self class]] resourcePath] stringByAppendingPathComponent:@"Preferences.plist"];
    [_preferences writeToFile:preferencesPath atomically:YES];
    [_preferences release];
    _preferences = nil;
}

-(void) updateControls
{
    NSDate* date = [NSDate date];

    if(!lTime.hidden)
        lTime.text = [_timeFormatter stringFromDate:date];
    if(!lDate.hidden)
        lDate.text = [_dateFormatter stringFromDate:date];
}

@end

@implementation UIColor (RGBA)

+(UIColor*) colorWithRGB:(unsigned int) rgb
{
    return [UIColor colorWithRed:(float)(rgb >> 16) / 255 green:(float)((rgb >> 8) & 0xFF) / 255 blue:(float)(rgb & 0xFF) / 255 alpha:1.0];
}

@end