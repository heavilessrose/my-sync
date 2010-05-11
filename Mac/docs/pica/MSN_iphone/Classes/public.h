//
//  public.h
//  MSN
//
//  Created by Jun Du on 4/13/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

@interface Public : NSObject {
}

+(void)alertMessage:(NSString *)title addMsg:(NSString *)msg;
+(BOOL) isENumberAlphabet:(unsigned char )c;

+(BOOL)isEnglishNumberAlphabet:(NSString *)string;
@end
