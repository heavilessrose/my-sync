/*
 *  MyLog.m
 *  
 *
 *  Created by michael.wu on 7/2/09
 *  Copyright 2009 Winks. All rights reserved.
 *
 */
#import "MyLog.h"
#import <Foundation/Foundation.h>
#import <sys/time.h>
#import <sys/stat.h>
#import <pthread.h>



//如果日志大于80k，则删除
void Winks_clear_log() {
#if 1	
	struct stat st;
	Winks_printf("Winks_clear_log");
	if(stat(WINKS_CALLLOG_PATH,&st) == 0)
	{
		Winks_printf("Winks_clear_log1");
		if(st.st_size>804800)
		{
			//int ret = system("rm -f /var/mobile/callLog");
			Winks_printf("Winks_clear_log:size=%d",st.st_size);
		}
		Winks_printf("Winks_clear_log2");
	}
	Winks_printf("end Winks_clear_log");
#endif	
}


void getCurTime(char *_cur_time)
{
	struct tm *tt2;
	struct timeval tstruct1;
	struct timezone tzp;
	gettimeofday(&tstruct1,&tzp);
	tt2 = localtime(&tstruct1.tv_sec);
	sprintf(_cur_time,"%04d%02d%02d %02d:%02d:%02d.%06d",
			tt2->tm_year+1900,tt2->tm_mon+1,tt2->tm_mday,tt2->tm_hour,tt2->tm_min,tt2->tm_sec,tstruct1.tv_usec);
}

void Winks_printfErr(const char* fmt, ... )
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

extern int SpringBoardpid;
extern int Mobilepid;

void CallLog(const char* str) {
	
	//char* path = "/var/mobile/callLog";
	//char* path = "/callLog";
	
	FILE *myLog = NULL;
	int thepid = (int)getpid();
	if(thepid == SpringBoardpid)
	{
		char path[] = "/var/mobile/callLogSpr.txt";
		myLog = fopen(path,"a+");
	}
	else if(thepid == Mobilepid)
	{
		char path[] = "/var/mobile/callLogMobile.txt";
		myLog = fopen(path,"a+");
	}
	else
	{
		char path[] = "/var/mobile/callLogOther.txt";
		myLog = fopen(path,"a+");
	}
	
	
	
	if (myLog != NULL) 
	{		
		//		NSDate *time = [NSDate date];
		//		const char *tmp = [[time description] cStringUsingEncoding:NSASCIIStringEncoding];
		
		char tmp[50] = {0};
		getCurTime((char *)tmp);
		
		
		char  buf[18];
		memset( buf, 0x0, sizeof( buf ) );
		snprintf(buf,sizeof(buf),"%d",thepid );
		fputs("[PID:",myLog);
		fputs(buf,myLog);
		fputs("]",myLog);
		
		pthread_t tid = pthread_self();
		memset( buf, 0x0, sizeof( buf ) );
		snprintf(buf,sizeof(buf),"%d", (unsigned int)tid);
		fputs("[TID:",myLog);
		fputs(buf,myLog);
		fputs("]",myLog);
		
		fputs(tmp,myLog);
		fputs("  ",myLog);
		fputs(str,myLog);
		fputs("\n",myLog);
	}
	fclose(myLog);
	
}

#ifdef SHOW_WINKS_LOG
void Winks_HEX_DataOut(const unsigned char *fragment, unsigned short length, const unsigned char *name)
{
#if 0
    const unsigned char *s;
    const unsigned short data_per_line=16;
    int i, j;
    unsigned char *c1, *c2, buf[256];
	
    if (fragment==NULL)
		return;
	
    Winks_printf("  %s at 0x%x.",name, (unsigned int)fragment);
    Winks_printf("      length: %d byte%s\r\n", length, (length>0)?"s":"");
	
    s=fragment;
    j=length;
    while (j>0) {
		memset(buf, ' ',256);
		/* c1+=sprintf(c1=buf, "    data: "); */
		memcpy(buf, "    data: ", 11);
		c1=(buf+10);
		c2=c1+(3*data_per_line)+1;
		for (i=((j>data_per_line)?data_per_line:j); i>0; i--, j--) {
			*c1=(*s>>4); *(c1)+=(*c1<0x0a)?'0':('a'-0x0a); c1++;
			*c1=(*s&0x0f); *(c1)+=(*c1<0x0a)?'0':('a'-0x0a); c1++;
			*c1++=' ';
			if (isprint(*s))
				*c2++=*s;
			else
				*c2++='.';
			s++;
		}
		*c2=0;
		Winks_printf("%s\r\n",buf);
    }
#endif
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


void CallLogInt(int myInt) {
	char* path = "/var/mobile/callLog";
	//char* path = "/callLog";
	char  buf[18];
	memset( buf, 0x0, sizeof( buf ) );
	snprintf(buf,sizeof(buf),"%d",myInt );
	
	FILE *myLog = NULL;
	myLog = fopen(path,"a+");
	
	if (myLog != NULL) 
	{
		
		NSDate *time = [NSDate date];
		const char *tmp = [[time description] cStringUsingEncoding:NSASCIIStringEncoding];
		
		fputs(tmp,myLog);
		fputs("  ",myLog);
		fputs(buf,myLog);
		fputs("\n",myLog);
	}
	fclose(myLog);
}

#if 0
//panqj add test
static void Winks_Notifycallback(CFNotificationCenterRef center, void *observer, CFStringRef name, const void *object, CFDictionaryRef userInfo) {
   Winks_printf("Winks_Notifycallback:pid=%d",(int)getpid());
	Winks_printf("Winks_Notifycallback:Notification intercepted: %s\n", [name UTF8String]);
    if (userInfo)
	{
		NSDictionary *info = userInfo;
		int dcount = CFDictionaryGetCount(userInfo);
		id keys = [userInfo allKeys];
		int i = 0;
		for(i=0; i< dcount; i++)
		{
			id key = [keys objectAtIndex:i];
			Winks_printf("User Info %d: key:%s,value:%s",i,[[key description] cString],[[[info objectForKey:key] description] cString]);
		}
		//CFShow(userInfo);
	}
    return;
}

int Winks_AddCoreTelephonyNotify(void)
{
	printf("Watching for Core Telephony notifications\n");
	
	Winks_printf("Winks_AddCoreTelephonyNotify:pid=%d",(int)getpid());
	id ct = CTTelephonyCenterGetDefault();
	CTTelephonyCenterAddObserver(
								 ct,
								 NULL,
								 Winks_Notifycallback,
								 kCTCallStatusChangeNotification,
								 NULL,
								 CFNotificationSuspensionBehaviorDeliverImmediately);
	//CTTelephonyCenterAddObserver(server, self, smsCallBack,kCTSMSMessageReceivedNotification,NULL,4);
}
//panqj add end
#endif

#endif
