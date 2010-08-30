/*
 *  Hello.h
 *  Untitled
 *
 *  Created by luke on 10-8-18.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#import <Foundation/Foundation.h>

class Hello;


class Hello {
private:
	id greeting_text;  // holds an NSString
public:
	Hello() {
		greeting_text = @"Hello, world!";
	}
	Hello(const char* initial_greeting_text) {
		greeting_text = [[NSString alloc] initWithUTF8String:initial_greeting_text];
	}
	void say_hello() {
		printf("%s\n", [greeting_text UTF8String]);
	}
};