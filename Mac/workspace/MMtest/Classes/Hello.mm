/*
 *  Hello.mm
 *  Untitled
 *
 *  Created by luke on 10-8-18.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Hello.h"

/* Hello.mm
 * Compile with: g++ -x objective-c++ -framework Foundation Hello.mm  -o hello
 */


@interface Greeting : NSObject {
@private
	Hello *hello;
}
- (id)init;
- (void)dealloc;
- (void)sayGreeting;
- (void)sayGreeting:(Hello*)greeting;
@end

@implementation Greeting
- (id)init {
    self = [super init];
    if (self) {
        hello = new Hello();
    }
    return self;
}
- (void)dealloc {
    delete hello;
    [super dealloc];
}
- (void)sayGreeting {
    hello->say_hello();
}
- (void)sayGreeting:(Hello*)greeting {
    greeting->say_hello();
}
@end

int main() {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
    Greeting *greeting = [[Greeting alloc] init];
    [greeting sayGreeting];                         // > Hello,  world!
	
    Hello *hello = new Hello("Bonjour, monde!");
    [greeting sayGreeting:hello];                   // > Bonjour,  monde!
	
    delete hello;
    [greeting release];
    [pool release];
    return 0;
}