//
//  PolynomialView.m
//  Polynomials
//
//  Created by Aaron Hillegass on 11/27/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "PolynomialView.h"
#import "Polynomial.h"

@implementation PolynomialView

- (id)initWithFrame:(NSRect)frame {
    [super initWithFrame:frame];
    polynomials = [[NSMutableArray alloc] init];
    return self;
}

- (IBAction)createNewPolynomial:(id)sender
{
    Polynomial *p = [[Polynomial alloc] init];
    [polynomials addObject:p];
    [p release];
    [self setNeedsDisplay:YES];
}
- (IBAction)deleteRandomPolynomial:(id)sender
{
    if ([polynomials count] == 0) {
        NSBeep();
        return;
    }
    int i = random() % [polynomials count];
    [polynomials removeObjectAtIndex:i];
    [self setNeedsDisplay:YES];
}


- (void)drawRect:(NSRect)rect 
{
    NSRect bounds = [self bounds];
    [[NSColor whiteColor] set];
    [NSBezierPath fillRect:bounds];
    
    CGContextRef ctx = [[NSGraphicsContext currentContext] graphicsPort];
    CGRect cgBounds = *(CGRect *)&bounds;    
    for (Polynomial *p in polynomials) {
        [p drawInRect:cgBounds
          inContext:ctx];
    }
}

@end
