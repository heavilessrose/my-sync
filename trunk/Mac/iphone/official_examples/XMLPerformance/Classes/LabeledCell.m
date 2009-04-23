/*
     File: LabeledCell.m
 Abstract: Used in the DetailViewController's table, this cell type has a right justified "label" and a left justified "value".
  Version: 1.0
 
 Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple
 Inc. ("Apple") in consideration of your agreement to the following
 terms, and your use, installation, modification or redistribution of
 this Apple software constitutes acceptance of these terms.  If you do
 not agree with these terms, please do not use, install, modify or
 redistribute this Apple software.
 
 In consideration of your agreement to abide by the following terms, and
 subject to these terms, Apple grants you a personal, non-exclusive
 license, under Apple's copyrights in this original Apple software (the
 "Apple Software"), to use, reproduce, modify and redistribute the Apple
 Software, with or without modifications, in source and/or binary forms;
 provided that if you redistribute the Apple Software in its entirety and
 without modifications, you must retain this notice and the following
 text and disclaimers in all such redistributions of the Apple Software.
 Neither the name, trademarks, service marks or logos of Apple Inc. may
 be used to endorse or promote products derived from the Apple Software
 without specific prior written permission from Apple.  Except as
 expressly stated in this notice, no other rights or licenses, express or
 implied, are granted by Apple herein, including but not limited to any
 patent rights that may be infringed by your derivative works or by other
 works in which the Apple Software may be incorporated.
 
 The Apple Software is provided by Apple on an "AS IS" basis.  APPLE
 MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
 THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
 FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND
 OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
 
 IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
 OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
 MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED
 AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
 STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 
 Copyright (C) 2008 Apple Inc. All Rights Reserved.
 
*/

#import "LabeledCell.h"

@implementation LabeledCell

- (id)initWithFrame:(CGRect)frame reuseIdentifier:(NSString *)reuseIdentifier {
    self = [super initWithFrame:frame reuseIdentifier:reuseIdentifier];
    if (self != nil) {
        // Create two labels and set their fonts and colors. Positioning will be done in layoutSubviews
        label = [[UILabel alloc] initWithFrame:CGRectZero];
        label.font = [UIFont boldSystemFontOfSize:12];
        label.textColor = [UIColor darkGrayColor];
        label.textAlignment = UITextAlignmentRight;
        label.backgroundColor = [UIColor clearColor];
        [self addSubview:label];
        value = [[UILabel alloc] initWithFrame:CGRectZero];
        value.font = [UIFont boldSystemFontOfSize:14];
        value.backgroundColor = [UIColor clearColor];
        [self addSubview:value];
        self.autoresizesSubviews = YES;
    }
    return self;
}

- (void)dealloc {
    [label release];
    [value release];
    [super dealloc];
}

- (void)layoutSubviews {
    [super layoutSubviews];
    // Place the subviews appropriately.
    CGRect baseRect = CGRectInset(self.contentView.frame, 10, 10);
    CGRect rect = baseRect;
    rect.size.width = 60;
    label.frame = rect;
    rect.origin.x += 70;
    rect.size.width = baseRect.size.width - 70;
    value.frame = rect;
}

// Implement the label and value properties to pass the string data down to the UILabel objects that display them.

- (NSString *)label {
    return label.text;
}

- (void)setLabel:(NSString *)aString {
    label.text = aString;
}

- (NSString *)value {
    return value.text;
}

- (void)setValue:(NSString *)aString {
    value.text = aString;
}

@end
