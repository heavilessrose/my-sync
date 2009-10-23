/*
 *  Utils.h
 *  netTest
 *
 *  Created by wang luke on 09-10-23.
 *  Copyright 2009 luke. All rights reserved.
 *
 */

#import <stdio.h>
#import <stdlib.h>
#import <string.h>
#import <sys/types.h>
#import <sys/socket.h>
#import <errno.h>
#import <unistd.h>
#import <netinet/in.h>
#import <limits.h>
#import <netdb.h>
#import <arpa/inet.h>
#import <ctype.h>

#import <stdarg.h>

void CallLog(const char* str);
void Winks_printf(const char* fmt, ... );
