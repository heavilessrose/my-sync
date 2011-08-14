//
//  UIImage+NSCoding.m
//  SLVod
//
//  Created by luke on 11-8-14.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "UIImage+NSCoding.h"


@implementation UIImage (NSCoding)

- (id)initWithCoder:(NSCoder *)decoder {
    NSData *pngData = [decoder decodeObjectForKey:@"PNGRepresentation"];
    [self autorelease];
    self = [[UIImage alloc] initWithData:pngData];
    return self;
}

- (void)encodeWithCoder:(NSCoder *)encoder {
    NSData *png = UIImagePNGRepresentation(self);
    [encoder encodeObject:png forKey:@"PNGRepresentation"];
}

@end

