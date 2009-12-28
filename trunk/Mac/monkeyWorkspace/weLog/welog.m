//
//  welog.m
//  WeLogLib
//
//  Created by luke on 09-12-28.
//  Copyright 2009 WeTeam. All rights reserved.
//

#import "welog.h"
#import <stdio.h>
#import <stdarg.h>
#import <sys/time.h>

/*
@implementation WeLog

@end
*/
#ifdef WELOG_ENABLE

#pragma mark -
#pragma mark core
void getCurTime(char *_cur_time) {
	struct tm *tt2;
	struct timeval tstruct1;
	struct timezone tzp;
	gettimeofday(&tstruct1,&tzp);
	tt2 = localtime(&tstruct1.tv_sec);
	sprintf(_cur_time,"%04d%02d%02d %02d:%02d:%02d.%06d",
			tt2->tm_year+1900,tt2->tm_mon+1,tt2->tm_mday,tt2->tm_hour,tt2->tm_min,tt2->tm_sec,tstruct1.tv_usec);
}

void _WeLog(const char *srcFile, int lineNumber, const char *funcName, NSString *format,...) {
	va_list ap;
	
	va_start (ap, format);
	/*
	if (![format hasSuffix: @"\n"]) {
		format = [format stringByAppendingString: @"\n"];
	}
	 */
	format = [format stringByAppendingString:@"\r\n"];
	NSString *body =  [[NSString alloc] initWithFormat: format arguments: ap];
	va_end (ap);
	const char *threadName = [[[NSThread currentThread] name] UTF8String];
	NSString *srcFileName=[[NSString stringWithUTF8String:srcFile] lastPathComponent];
	char tmp[50] = {0};
	getCurTime(tmp);
#ifdef WELOG_FILE_ENABLE
	FILE *logFile = fopen(WELOG_FILE, "a+");
#endif
	if (threadName) {
#ifdef WELOG_STDERR_ENABLE
		fprintf(stderr,"%s %s/%s (%s:%d): %s",
				tmp,threadName,funcName,[srcFileName UTF8String],lineNumber,[body UTF8String]);
#endif
#ifdef WELOG_FILE_ENABLE
		fprintf(logFile,"%s %s/%s (%s:%d): %s",
				tmp,threadName,funcName,[srcFileName UTF8String],lineNumber,[body UTF8String]);
#endif
	} else {
#ifdef WELOG_STDERR_ENABLE
		fprintf(stderr,"%s %p/%s (%s:%d): %s",
				tmp,[NSThread currentThread],funcName,[srcFileName UTF8String],lineNumber,[body UTF8String]);
#endif
#ifdef WELOG_FILE_ENABLE
		fprintf(logFile,"%s %p/%s (%s:%d): %s",
				tmp,[NSThread currentThread],funcName,[srcFileName UTF8String],lineNumber,[body UTF8String]);
#endif
	}
#ifdef WELOG_FILE_ENABLE
	fclose(logFile);
#endif
	[body release];	
}


#pragma mark -
#pragma mark addtional

/* 获取文件大小, 返回负值表示失败. is_ascii=1文件名为asc编码，＝0为unicode编码 */
int Winks_fgetsize(const char *filename, unsigned int is_ascii, unsigned int *size) {
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	NSString *filePath;
	filePath = [NSString stringWithCString:filename encoding:NSASCIIStringEncoding];
	NSDictionary *attrs = [[NSFileManager defaultManager] fileAttributesAtPath:filePath traverseLink:NO];
	if(attrs != nil){
		NSNumber *fileSize;
		//if (fileSize = [attrs objectForKey:NSFileSize]) {
		if ((fileSize = [attrs objectForKey:NSFileSize]) != nil) {
			*size = [fileSize unsignedLongLongValue];
			//int psize = [fileSize intValue];
			[pool release];
			return 0;
		}else{
			[pool release];
			return 0;
		}
	}else{
		[pool release];
		return -1;
	}
}

/*删除指定的文件。成功返回0，失败返回-1。*/
int Winks_fdelete(char *file_name) {
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	NSString *filePath = [[NSString alloc] initWithCString:file_name];
	
	BOOL success = [[NSFileManager defaultManager] removeItemAtPath:filePath error:NULL];
	[filePath release];
	if(success){
		[pool release];
		return 0;
	}
	else{
		[pool release];
		return -1;
	}
}

// 控制日志当大于300k删除
void delLogFile() {
	unsigned int size = 0;
	Winks_fgetsize(WELOG_FILE, 1, &size);
	if (size >= WELOG_MAX_SIZE) {
		Winks_fdelete(WELOG_FILE);
	}
}

void HEX_DataOut(const unsigned char *fragment, unsigned short length, const unsigned char *name) {
    const unsigned char *s;
    const unsigned short data_per_line=16;
    int i, j;
    unsigned char *c1, *c2, buf[256];
	
    if (fragment==NULL)
		return;
	
    welog(@"  %s at 0x%x.",name, (unsigned int)fragment);
    welog(@"      length: %d byte%s\r\n", length, (length>0)?"s":"");
	
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
		//Winks_printf("%s\r\n",buf);
    }
}

#endif /* WELOG_ENABLE */
