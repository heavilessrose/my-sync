#import "MyLog.h"

void CallLog(const char* str) {
	char* path = "/var/mobile/callLog";
	//char* path = "/callLog";


	FILE *myLog = NULL;
	myLog = fopen(path,"a+");
	
	if (myLog != NULL) 
	{
		
		NSDate *time = [NSDate date];
		const char *tmp = [[time description] cStringUsingEncoding:NSASCIIStringEncoding];
		
		fputs(tmp,myLog);
		fputs("  ",myLog);
		fputs(str,myLog);
		fputs("\n",myLog);
	}
	fclose(myLog);
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