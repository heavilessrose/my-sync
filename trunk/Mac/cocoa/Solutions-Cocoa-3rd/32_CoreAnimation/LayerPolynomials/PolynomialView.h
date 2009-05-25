//
//  PolynomialView.h
//  Polynomials
//
//  Created by Aaron Hillegass on 11/27/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface PolynomialView : NSView {
    NSMutableArray *polynomials;
    BOOL blasted;
}
- (NSPoint)randomOffViewPosition;
- (IBAction)createNewPolynomial:(id)sender;
- (IBAction)deleteRandomPolynomial:(id)sender;
- (IBAction)blastem:(id)sender;

@end
