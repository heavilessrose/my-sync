//
//  Foo.h
//  RandomApp
//
//  Created by luke on 4/16/09.
//  Copyright 2009 luke. All rights reserved.
//

#import <Cocoa/Cocoa.h>


/*!
    @class       Foo 
    @superclass  NSObject { IBOutlet NSTextField *textField; }
    @abstract    <#(brief description)#>
    @discussion  <#(comprehensive description)#>
*/
@interface Foo : NSObject {
	IBOutlet NSTextField *textField;
}

/*!
    @method     seed:
    @abstract   <#(brief description)#>
    @discussion <#(comprehensive description)#>
    @param      sender <#(description)#>
    @result     <#(description)#>
*/
- (IBAction)seed:(id)sender;

/*!
    @method     generate:
    @abstract   <#(brief description)#>
    @discussion <#(comprehensive description)#>
    @param      sender <#(description)#>
    @result     <#(description)#>
*/
- (IBAction)generate:(id)sender;

@end
