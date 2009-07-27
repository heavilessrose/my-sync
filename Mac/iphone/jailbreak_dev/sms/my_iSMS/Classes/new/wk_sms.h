/*
 *  wk_sms.h
 *  ___PROJECTNAME___
 *
 *  Created by wang luke on 7/27/09.
 *  Copyright 2009 luke. All rights reserved.
 *
 */

#ifndef wk_sms
#define wk_sms

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <paths.h>
#include <termios.h>
#include <sysexits.h>
#include <sys/param.h>
#include <sys/select.h>
#include <sys/time.h>

#include <MacTypes.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/serial/IOSerialKeys.h>
#include <IOKit/IOBSD.h>

#include "ConvertUTF.h"

#ifdef __cplusplus
extern "C" {
#endif

// SMS
	typedef void (* CALLBACK) (int);
	int OpenConn();
	void CloseConn(int fd);
	Boolean ResetConn(int fd, CALLBACK callback);
	int ReadResp(int fd);
	int SendCmd(int fd, void *buf, size_t size);
	int sendmessage(const char *phone,const char *sms,char *code,CALLBACK callback);

	char * getSMSCNumber(int fd);
	Boolean sendsms(int fd,char *atcmd,char *pdu);
	
// DEBUG
#ifdef DEBUG_ENABLED
	void HexDump(unsigned char *buf, int size);
	void HexDumpLine(unsigned char *buf, int remainder, int offset);
#endif
	
// ENCODE
	char* encodingSMSC(char *smsc,char *encodedsmsc);
	char* encodingPhone(int index,char *encodedphone);
	int CalcSMSSendCount(char *sms);
	char* encodingSMS(int index,char *encodedSMS,int msgCount,int msgIdx);
	char* encodingPDU(char *encodedsmsc,char * encodedphone,char *encodedsms,int msgCount,int msgIdx);
	char* encodingATCMD(char *pdu);
	int SplitePhoneNumber(char *phone);
	char* getCountryCode();
	void FreePhoneBuf();
	//only for debug
	/*char * CalcCountryCode(char *smsc);
	 char * ProcessPhoneNumberWithCountryCode(char *buf);
	 char * getCountryCode();
	 char * setCountryCode(char *);
	 char *getUSMS();*/
	
// UNICODE_HELPER
	char * utf8_to_utf16(char *s,char *d);
#ifdef __cplusplus
}
#endif
#endif


