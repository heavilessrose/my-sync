/*
 *  Utils.c
 *  netTest
 *
 *  Created by wang luke on 09-10-23.
 *  Copyright 2009 luke. All rights reserved.
 *
 */




#import "Utils.h"
void CallLog(const char* str) {
	//    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	char* path = "/var/mobile/netLog";
	//char *path = "/Users/luke/netLog";
	
	FILE *myLog = NULL;
	myLog = fopen(path,"a+");
	
	if (myLog != NULL) 
	{		
		//		NSDate *time = [NSDate date];
		//		const char *tmp = [[time description] cStringUsingEncoding:NSASCIIStringEncoding];
		//		
		//		fputs(tmp,myLog);
		fputs("  ",myLog);
		fputs(str,myLog);
		fputs("\r\n",myLog);
		//
		printf("[APP] %s\r\n",str);
	}
	fclose(myLog);
	//	[pool release];
}

void Winks_printf(const char* fmt, ... )
{
    const int MAX_DBG_STR = 1024;
	
    int written = 0;
    char buffer[MAX_DBG_STR];
	memset(buffer,0,MAX_DBG_STR);
	
    va_list va;
    va_start( va, fmt );
    written = vsprintf( &buffer[0], fmt, va);
    va_end(va);
	
	if(strlen(buffer))
		CallLog(buffer);
}

