/*
 *  wk_sms.c
 *  ___PROJECTNAME___
 *
 *  Created by wang luke on 7/27/09.
 *  Copyright 2009 luke. All rights reserved.
 *
 */

#include "wk_sms.h"

#define LOG stdout

#define BUFSIZE 4096
char readbuf[BUFSIZE];
struct termios term, gOriginalTTYAttrs;

//#define DEBUG_ENABLED 1

#ifndef DEBUG_ENABLED
#define DEBUGLOG(x) 
#else
#define DEBUGLOG(x) x
#endif

/***********************************************\
			DEBUG
 ***********************************************/
#ifdef DEBUG_ENABLED
void HexDumpLine(unsigned char *buf, int remainder, int offset) {
	int i = 0;
	char c = 0;
	
	// Print the hex part
	fprintf(LOG, "%08x | ", offset);
	for (i = 0; i < 16; ++i) {
		if (i < remainder)
			fprintf(LOG, "%02x%s", buf[i], (i == 7) ? "  " : " ");
		else
			fprintf(LOG, "  %s", (i == 7) ? "  " : " ");
	}
	// Print the ascii part
	fprintf(LOG, " | ");
	for (i = 0; i < 16 && i < remainder; ++i) {
		c = buf[i];
		if (c >= 0x20 && c <= 0x7e)
			fprintf(LOG, "%c%s", c, (i == 7) ? " " : "");
		else
			fprintf(LOG, ".%s", (i == 7) ? " " : "");
	}
	
	fprintf(LOG, "\n");
}

void HexDump(unsigned char *buf, int size) {
	int i = 0;
	
	for (i = 0; i < size; i += 16)
		HexDumpLine(buf + i, size - i, i);
	fprintf(LOG, "%08x\n", size);
}
#endif

/***********************************************\
			SMS
 ***********************************************/
// 发送at命令
int SendCmd(int fd, void *buf, size_t size) {
	DEBUGLOG(fprintf(LOG, "Sending:\n"));
	DEBUGLOG(HexDump((unsigned char*)buf, size));
	
	int retcode=write(fd, buf, size);
	if (retcode == -1) {
		DEBUGLOG(fprintf(stderr, "Shit. %s\n", strerror(errno)));
		return -1;
	}
	return retcode;
}

int ReadRawResp(int fd) {
	int len = 0;
	struct timeval timeout;
	int nfds = fd + 1;
	fd_set readfds;
	
	FD_ZERO(&readfds);
	FD_SET(fd, &readfds);
	
	// Wait a second
	timeout.tv_sec = 0;
	timeout.tv_usec = 500000;
	
	while (select(nfds, &readfds, NULL, NULL, &timeout) > 0)
		len += read(fd, readbuf + len, BUFSIZE - len);
	
	if (len > 0) {
		DEBUGLOG(fprintf(LOG, "Read:\n"));
		DEBUGLOG(HexDump((unsigned char *)readbuf, len));
	}
	return len;
}

int stripEcho(int len) {
	char *pStart;
	
	//strip echo\r\n
	pStart=readbuf;
	
	do {
		if ((*pStart=='\r') && (*(pStart+1)=='\n'))
			break;
		pStart++;
	} while ((pStart-readbuf)<len);
	pStart++;
	pStart++;
	strcpy(readbuf, pStart);
	return strlen(readbuf);
}

int stripEndOK(int len) {
	char *pEnd;
	
	pEnd=readbuf+len-1;
	
	//strip \r\n\r\nOK\r\n
	do {
		if ((*pEnd=='O') && (*(pEnd+1)=='K'))
			break;
		pEnd--;
	} while ((readbuf+len-1-pEnd)<len);
	
	do {
		pEnd--;
		pEnd--;
	} while (((*pEnd=='\r') && (*(pEnd+1)=='\n')));
	pEnd++;
	pEnd++;
	*pEnd='\0';
	
	return strlen(readbuf);
}

int ReadResp(int fd) {
	int len;
	len=ReadRawResp(fd);
	if (len > 0) {
		DEBUGLOG(fprintf(LOG, "Read:\n"));
		DEBUGLOG(HexDump((unsigned char *)readbuf, len));
	}
	if (len>0)
		return stripEcho(len);
}

void CloseConn(int fd) {
	if (tcdrain(fd) == -1) {
		DEBUGLOG(printf("Error waiting for drain - %s(%d).\n",
						strerror(errno), errno));
	}
	
	if (tcsetattr(fd, TCSANOW, &gOriginalTTYAttrs) == -1) {
		DEBUGLOG(printf("Error resetting tty attributes - %s(%d).\n",
						strerror(errno), errno));
	}
	
	close(fd);
}

#define kATCommandString	"AT\r"
#define kOKResponseString	"OK\r\n"
#define kNumRetries 		100

Boolean ResetConn(int fd, CALLBACK callback) {
	int numBytes; // Number of bytes read or written 
	int i, callback_count=0;
	Boolean result=false;
	
	for (i = 1; i <= 100; i++) {
		numBytes=SendCmd(fd, kATCommandString, strlen(kATCommandString));
		
		if ((numBytes==-1)||(numBytes<strlen(kATCommandString)))
			continue;
		if (!ReadResp(fd))
			continue;
		
		if (strncmp((char *)readbuf, kOKResponseString,
					strlen(kOKResponseString)) == 0) {
			result = true;
			break;
		}
		callback_count=i/10;
		callback(10+callback_count);
	}
	return result;
}

int OpenConn(CALLBACK callback) {
	// 打开串口设备
	int fd = open("/dev/tty.debug", O_RDWR | 0x20000 | O_NOCTTY);
	unsigned int handshake = TIOCM_DTR | TIOCM_RTS | TIOCM_CTS | TIOCM_DSR;
	
	if (fd == -1) {
		DEBUGLOG(fprintf(LOG, "%i(%s)\n", errno, strerror(errno)));
		exit(1);
	}
	
	// save orig attrib
	cfmakeraw(&gOriginalTTYAttrs);
	gOriginalTTYAttrs.c_cc[VMIN] = 0;
	gOriginalTTYAttrs.c_cc[VTIME] = 0;
	
	speed_t speed =115200;
	
	fcntl(fd, 4, 0);
	tcgetattr(fd, &term);
	
	cfsetspeed(&term, speed);
	cfmakeraw(&term);
	term.c_cc[VMIN] = 0;
	term.c_cc[VTIME] = 5;
	
	term.c_cflag = (term.c_cflag & ~CSIZE) | CS8;
	term.c_cflag &= ~PARENB;
	term.c_lflag &= ~ECHO;
	
	tcsetattr(fd, TCSANOW, &term);
	
	ioctl(fd, TIOCSDTR);
	ioctl(fd, TIOCCDTR);
	ioctl(fd, TIOCMSET, &handshake);
	
	//now tty has been opened
	callback(10);
	if (ResetConn(fd, callback))
		return fd;
	else {
		CloseConn(fd);
		return -1;
	}
}

// 发送短消息
Boolean sendsms(int fd, char *atcmd, char *pdu) {
	int numBytes, len, i;
	char RespString[128];
	int tries=50;
	
	DEBUGLOG(printf("sendsms() called\n"));
	for (i=0; i<tries; i++) {
		numBytes=SendCmd(fd, atcmd, strlen(atcmd));
		if ((numBytes==-1)||(numBytes<10)) {
			DEBUGLOG(printf("send atcmd sms error\n"));
			return false;
		}
		
		sprintf(RespString, "%s%c", atcmd, '>');
		len=ReadRawResp(fd);
		
		if (strncmp((char *)readbuf, RespString, strlen(RespString) )== 0)
			break;
		sleep(1);
	};
	
	numBytes=SendCmd(fd, pdu, strlen(pdu));
	if ((numBytes==-1)||(numBytes<10)) {
		DEBUGLOG(printf("send sms pdu error\n"));
		return false;
	}
	
	len=ReadRawResp(fd);
	DEBUGLOG(HexDump((unsigned char *)readbuf,len));
	char *p=strstr(readbuf, "ERROR");
	if (NULL!=p)
		return false;
	
	return true;
}

// 得到短信中心号码
char* getSMSCNumber(int fd) {
	int numBytes, len;
	
	char *pStart, *pEnd;
	DEBUGLOG(printf("getSMSCNumber()\n"));
	
	numBytes=SendCmd(fd, "AT+CSCA?\r", 10);
	
	if ((numBytes==-1)||(numBytes<10)) {
		DEBUGLOG(printf("send at+csca error\n"));
		return NULL;
	}
	
	len=ReadResp(fd);
	if (len<=0) {
		DEBUGLOG(fprintf(stderr, "failed to get response\n"));
		return NULL;
	}
	stripEndOK(strlen(readbuf));
	//now in readbuf, like   +CSCA: "8613800519500",145
	//next is striped  unused,got smsc number
	pStart=readbuf;
	
	do {
		if (*pStart=='\"')
			break;
		pStart++;
	} while ((pStart-readbuf)<len);
	pStart++;
	pEnd=pStart;
	
	do {
		if (*pEnd=='\"')
			break;
		pEnd++;
	} while ((pEnd-readbuf)<len);
	*pEnd='\0';
	if (*pStart=='+')
		pStart++; //strip first +
	strcpy(readbuf, pStart);
	return (char *)readbuf;
}

void NOOP_CALLBACK(int process) {
	DEBUGLOG(printf("NOOP_CALLBACK - Progress %d\n",process));
}

#define MAX_PHONE_NUMBER_LENGTH 512
#define MAX_SMS_LENGTH 8192

int sendmessage(const char *aphone, const char *asms, char *code, CALLBACK callback)
{
	char *_sms_text, *_receiver_number;
	size_t _sms_text_len, _receiver_number_len;
	
	int fd, phonecount, smscount, i, j, callback_count, callback_step;
	char esmsc[128], ephone[128], esms[512/*12+70*4+1*/];
	char *pdu, *atcmd;
	int result = 0;
	
	if (callback == NULL) {
		callback = NOOP_CALLBACK;
	}
	// Check phone/number and copy to local buffer
	if (aphone == NULL || asms == NULL || code == NULL) {
		return -1;
	}
	_receiver_number_len = strlen(aphone);
	if (_receiver_number_len > MAX_PHONE_NUMBER_LENGTH) {
		_receiver_number_len = MAX_PHONE_NUMBER_LENGTH;
	}
	_receiver_number = malloc(_receiver_number_len + 1);
	snprintf(_receiver_number, _receiver_number_len + 1, "%s", aphone);
	
	_sms_text_len = strlen(asms);
	if (_sms_text_len > MAX_SMS_LENGTH) {
		_sms_text_len = MAX_SMS_LENGTH;
	}
	_sms_text = malloc(_sms_text_len + 1);
	snprintf(_sms_text, _sms_text_len + 1, "%s", asms);
	
	DEBUGLOG(printf("%s - %s\n", _receiver_number, _sms_text));
	//计算要发送几次
	smscount=CalcSMSSendCount(_sms_text);
	if (smscount==0) {
		result = -1;
		goto __EXIT__;
	};
	// 群发的号码数
	phonecount=SplitePhoneNumber(_receiver_number);
	// 
	callback_step=80/(smscount*phonecount);
	
	DEBUGLOG(printf("%d numbers, sms length %d\n", phonecount, smscount));
	// 打开串口连接
	fd=OpenConn(callback);
	if (callback) {
		callback(20);
	}
	if (fd ==-1) {
		DEBUGLOG(printf("initconn error!\n"));
		result = -1;
		goto __EXIT__;
	}
	// 编码短信中心号码
	encodingSMSC(getSMSCNumber(fd), esmsc);
	if (code) {
		//FIXME: NOT thread safe!
		strcpy(code, getCountryCode());
	}
	
	DEBUGLOG(printf("encodingSMSC() success\n"));
	for (j=0; j<phonecount; j++) {
		encodingPhone(j, ephone);
		DEBUGLOG(printf("encodingPhone() success\n"));
		//分多次发送
		for (i=0; i<smscount; i++) {
			encodingSMS(i, esms,smscount,i+1);
			pdu=encodingPDU(esmsc, ephone, esms,smscount,i+1);
			callback_count=20+(i+j+1)*callback_step;
			callback(callback_count);
			atcmd=encodingATCMD(pdu);
			if (!sendsms(fd, atcmd, pdu)) {
				DEBUGLOG(printf("sendsms() failed\n"));
				result = -1;
				goto __EXIT__;
			}
		}
		DEBUGLOG(printf("sendsms() done\n"));
	}
	if (callback) {
		callback(100);
	}	
	DEBUGLOG(printf("Send Success! result : %d\n", result));
	
__EXIT__:
	DEBUGLOG(printf("sendmessage() Cleaning up\n"));
	// Clean up and exit
	free(_receiver_number);
	_receiver_number = 0;
	free(_sms_text);
	_sms_text = 0;
	FreePhoneBuf();
	if (fd) {
		CloseConn(fd);
		fd = -1;
	}
	return result;
}

/***********************************************\
			ENCODE
 ***********************************************/

// 倒置
char *InvertString(char *invertStr) {
	int Len;
	int i;
	char t;
	
	Len=strlen(invertStr);
	
	if (Len%2) {
		sprintf(invertStr, "%sF", invertStr);
		Len++;
	}
	
	for (i=0; i<Len; i+=2) {
		t=invertStr[i];
		invertStr[i]=invertStr[i+1];
		invertStr[i+1]=t;
	}
	
	return invertStr;
}

char gUnicodeSMS[65535], gPDU[65535], atcmd[128], tPDU[65535];
char *phonebuf[1000], phonecount=0;
char gCode[4];
int p_len;
Boolean isChinaUser, isInternationNumber;

void getcode(char *phone, char *retcode) {
	char buf[128];
	char code[4];
	memset(code, 0, 4);
	strcpy(buf, phone);
	
	char *p=buf;
	switch (*p) {
		case '1':
			strcpy(code, "1");
			break;
		case '2':
			p++;
			switch (*p) {
				case '0':
					strcpy(code, "20");
					break;
				case '7':
					strcpy(code, "27");
					break;
				default:
					p+=2;
					*p=0;
					strcpy(code, buf);
			}
			break;
		case '3':
			p++;
			switch (*p) {
				case '5':
				case '7':
				case '8':
					p+=2;
					*p=0;
					strcpy(code, buf);
					break;
				default:
					*++p=0;
					strcpy(code, buf);
			}
			break;
		case '4':
			p++;
			if (*p=='2') {
				p+=2;
				*p=0;
				strcpy(code, buf);
			} else {
				*++p=0;
				strcpy(code, buf);
			}
			;
			break;
			
		case '5':
			p++;
			if (*p=='9') {
				p+=2;
				*p=0;
				strcpy(code, buf);
			} else {
				*++p=0;
				strcpy(code, buf);
			}
			;
			
			break;
			
		case '6':
			p++;
			switch (*p) {
				case '7':
				case '8':
				case '9':
					p+=2;
					*p=0;
					strcpy(code, buf);
					break;
				default:
					*++p=0;
					strcpy(code, buf);
					break;
			}
			break;
			
		case '7':
			
			strcpy(code, "7");
			break;
		case '8':
			p++;
			switch (*p) {
				case '1':
				case '2':
				case '4':
				case '6':
					*++p=0;
					strcpy(code, buf);
					break;
				default:
					p+=2;
					*p=0;
					strcpy(code, buf);
					break;
			}
			break;
			
		case '9':
			p++;
			switch (*p) {
				case '6':
				case '7':
				case '9':
					p+=2;
					*p=0;
					strcpy(code, buf);
					break;
				default:
					*++p=0;
					strcpy(code, buf);
					break;
			}
			break;
		default:
			break;
			
	}
	strcpy(retcode, code);
}

// 编码短信中心号码
char * encodingSMSC(char *smsc, char *encodedsmsc) {
	char buf[128], buf2[128];
	
	/*短信中心号码*/
	//1、将短信息中心号码去掉+号，看看长度是否为偶数，如果不是，最后添加F 
	//	即 addr = "+8613800200500" 
	//  => addr = "8613800200500F" 
	//2、将奇数位和偶数位交换。 
	//  => addr = "683108020005F0" 
	//3、将短信息中心号码前面加上字符91，91是国际化的意思 
	//  => addr = "91683108020005F0" 
	//4、算出 addr 长度，结果除2，格式化成2位的16进制字符串，16 / 2 = 8 => "08" 
	//=> addr = "0891683108020005F0" 
	char *p;
	if (strlen(smsc)>0) {
		p=smsc;
		if (*p=='+')
			p++;
		strcpy(buf2, p);
		if ((buf2[0]=='8') && (buf2[1]=='6'))
			isChinaUser=true;
		else
			isChinaUser=false;
	}
	getcode(p, gCode);
	sprintf(buf, "91%s", InvertString(buf2));
	sprintf(encodedsmsc, "%02X%s", (unsigned int)(strlen(buf)/2), buf);
}

char * ProcessPhoneNumberWithCountryCode(char *buf) {
	//目前实现的功能是 如果是中国大陆的用户，检查是不是“86”开头的，如果是，删除86
	char tbuf[128];
	
	char *p=buf;
	//memset(tbuf,0,128);
	if (isChinaUser && (!isInternationNumber)) {
		if ((*p=='8') && (*(p+1)=='6')) {
			strcpy(tbuf, p+2);
			strcpy(buf, tbuf);
		}
	}
	return buf;
}

int SplitePhoneNumber(char *phone) {
	int i, count=0;
	char *p, *prev, *pstring;
	char buf[4096];
	
	memset(phonebuf, 0, sizeof(phonebuf));
	strcpy(buf, phone);
	p=buf;
	prev=buf;
	
	for (i=0; i<=strlen(phone); i++) {
		
		if ((*p==',')||(*p==0)) {
			pstring=(char *)malloc(p-prev+1);
			*p=(char)0;
			strcpy(pstring, prev);
			prev=p+1;
			phonebuf[count]=pstring;
			count++;
		}
		p++;
	}
	
	return count;
}

void FreePhoneBuf() {
	int i;
	for (i=0; i<phonecount; i++) {
		free(phonebuf[i]);
	}
}

// 编码电话号码
char * encodingPhone(int index, char *encodedphone) {
	char buf[128], *pp, *pb, *phone;
	int i, len;
	/*对方手机号码*/
	//1、将手机号码去掉+号，看看长度是否为偶数，如果不是，最后添加F 
	//即 phone = "+8613602433649" 
	//=> phone = "8613602433649F" 
	//2、将手机号码奇数位和偶数位交换。 
	//=> phone = "683106423346F9" 
	
	phone=phonebuf[index];
	pp=phone;
	pb=buf;
	len = strlen(phone);
	if (len>128)
		return NULL;
	memset(buf, 0, sizeof(buf));
	//查查看是不是国际电话号码
	if (*pp=='+')
		isInternationNumber=true;
	else
		isInternationNumber=false;
	
	//去除非数字，比如电话号码里有空格或者‘-’之类的 包括开头的那个+
	for (i=0; i<len; i++) {
		if ((*pp>='0') && (*pp<='9'))
			*pb++=*pp;
		*pp++;
	}
	//如果是国内的用户，手机号码包含86，删除之
	ProcessPhoneNumberWithCountryCode(buf);
	p_len=strlen(buf);
	sprintf(encodedphone, "%s", InvertString(buf));
}

// 短信分割数
int CalcSMSSendCount(char *sms) {
	
	memset(gUnicodeSMS, 0, sizeof(gUnicodeSMS));
	utf8_to_utf16(sms, gUnicodeSMS);
	// 短信的最大长度是70个字符,每个字符被格式化成4个字节
	return (int)strlen(gUnicodeSMS)/(70*4)+1;
}

/**
 * Enhanced by Shawn to support large text
 */
char *encodingSMS(int index, char *encodedSMS,int msgCount,int msgIdx) {
	char *pStart;
	char buf[70*4+1];
	memset(buf, 0, sizeof(buf));
	/*短信内容*/
	// 1、转字符串转换为Unicode代码，例如“工作愉快！”的unicode代码为 5DE54F5C61095FEBFF01， 
	// msg 长度除2，保留两位16进制数，即 5DE54F5C61095FEBFF01 = 20 / 2 => "0A"，再加上 msg 
	// msg = "0A5DE54F5C61095FEBFF01" 
	
	if(msgCount > 1){
		// need 12 bytes for the header
		pStart=gUnicodeSMS+index*(70*4 - 12);
		memcpy(buf, pStart, 70*4 - 12);
		// Add the header
		/*
		 05  - 协议头的长度 
		 00  - 标志这是个分拆短信 
		 03  - 分拆数据元素的长度 
		 39  - 唯一标志（用于把两条短信合并） 
		 02  - 一共两条 
		 01  - 这是第一条
		 */
		unsigned int msgLen = (unsigned int)((12/*header*/ + strlen(buf))/2)/*msg length*/;
#ifdef DEBUG_ENABLED
		printf("packet length %d\n",msgLen);
#endif		
		sprintf(encodedSMS,"%02X05000339%02X%02X%s",msgLen, msgCount,msgIdx,buf);
	}else{
		pStart=gUnicodeSMS+index*70*4;
		memcpy(buf, pStart, 70*4);
		sprintf(encodedSMS, "%02X%s", (unsigned int)(strlen(buf)/2), buf);	
	}
	return encodedSMS;
}

// 编码pdu
char *encodingPDU(char *encodedsmsc, char * encodedphone, char *encodedsms, int msgCount, int msgIdx) {
	
	memset(gPDU, 0, sizeof(gPDU));
	
	//手机号码前加上字符串 xx 00 0D 91（xx:通常为11,如果msgCount > 1则为51. 00：固定，0D：手机号码的长度，不算＋号，十六进制表示，91：发送到国际为91，发送到国内为81）
	// 即 phone = "11000D91" + phone 
	//=> 11000D91683106423346F9
	
	//2、手机号码后加上 000800 和刚才的短信息内容，000800也写死就可以了 
	// Shawn Note: here 00 08 00 means:
	//   00:协议标识 (TP-PID), 是普通 GSM 类型，点到点方式
	//   08:用户信息编码方式 (TP-DCS) , 7-bit 编码（ 08 ： UCS2 编码） 
	//   00:有效期 (TP-VP), 短信的有效时间 
	//3. 紧接着短信息长度 
	//4. 添加报头，如果是多个短信息的话。
	//5. 内容
	//6. ctrl+z and cr
	//即 phone = phone + "000800" + msg 
	//即 11000D91683106423346F9 + 000800 + 0A5DE54F5C61095FEBFF01 
	//=> phone = 11000D91683106423346F90008000A5DE54F5C61095FEBFF01 
	
	//3、phone 长度除以2，格式化成2位的十进制数
	// 即 11000D91683106423346F90008000A5DE54F5C61095FEBFF01 => 50位 / 2 => 25 
	//最后面加ctrl+z和回车
	char* __buf = (char*)gPDU;
	if(msgCount > 1){
		__buf[0] = '5';
		__buf[1] = '1';
	}else{
		//TODO: 处理需要feedback的情况，第5bit=1
		if(false/*need feedback*/){
			__buf[0] = '3';
			__buf[1] = '1';			
		}else{
			__buf[0] = '1';
			__buf[1] = '1';
		}
	}
	__buf+=2; // Move 2 bytes forward for next contents
	
	// Now finish the left part
	if (isInternationNumber){
		sprintf(__buf, "00%02X91%s000800%s%c%c", p_len, encodedphone,
				encodedsms, 26, 13);
	}else{
		sprintf(__buf, "00%02X81%s000800%s%c%c", p_len, encodedphone,
				encodedsms, 26, 13);
	}
	
	/*发送短信命令*/
	//发送短信的命令"AT+CMGS=(短信长度/2)回车"
	//gPDU的长度是去掉ctrl+z和回车的长度
	sprintf(atcmd, "AT+CMGS=%ld\r", (strlen(gPDU)-2)/2);
	
	//把短信中心号码也加在短信内容上
	sprintf(tPDU, "%s%s", encodedsmsc, gPDU);
	strcpy(gPDU, tPDU);
	
	/*send message*/
	/*
	 所以要发送的内容为 
	 AT+CMGF=0 <回车> 
	 OK 
	 AT+CMGS=25<回车> 
	 > addr+phone <Ctrl+Z发送> 
	 */
	return gPDU;
}

char *getCountryCode() {
	return gCode;
}
char *encodingATCMD(char *pdu) {
	return atcmd;
}


/***********************************************\
		UNICODE_HELPER
 ***********************************************/
void char2buf(char *sBuf,char *dBuf);
UTF16 * UTF8toUTF16(char * s, int * count);

char * utf8_to_utf16(char *s,char *d)
{
	char *inBuf;
	UTF16 *ret=NULL;
	int i,count;
	unsigned char c1,c2;
	
	unsigned char *p=NULL,*psmsBuffer=NULL;
	char tBuf[3];
	
	//char2buf(s,inBuf);
	inBuf=s;
	
	ret= UTF8toUTF16(inBuf,&count);
	if (0!=count){
		psmsBuffer=(unsigned char *)d;
		p=psmsBuffer;
        for (i=0;i<count-1;i++){
			c1=*(ret+i)>>8;
			c2=*(ret+i);
			sprintf(tBuf,"%.2X",c1);
			*psmsBuffer=tBuf[0];*psmsBuffer++;
			*psmsBuffer=tBuf[1];*psmsBuffer++;
			sprintf(tBuf,"%.2X",c2);
			*psmsBuffer=tBuf[0];*psmsBuffer++;
			*psmsBuffer=tBuf[1];*psmsBuffer++;
		}
		*psmsBuffer=0x0;
	}
	free (ret);
	return (char *) p;
}

void char2buf(char *sBuf,char *dBuf)
{
	int i;
	char buf[3]={0x1,0x2,0x0};
	char *p,*pend;
	p=sBuf;
	int len=strlen(sBuf)/2;
	for (i=0;i<len;i++){
        buf[0]=*p;p++;
        buf[1]=*p;p++;
        dBuf[i]=strtol(buf,&pend,16);
	}
	dBuf[len]=0x0;
}


/*
 * Detect UTF8String Encoding Length and Count needed Bytes;
 */
int UTF8StringCount(char * pUTF8EncodingString)
{
	ConversionResult ErrorCode;
	unsigned int bytes;
	int slen;
	UTF8 *source_start, *source_end;
	
	slen = strlen(pUTF8EncodingString);
	source_start = (UTF8 *)pUTF8EncodingString;
	source_end = (UTF8 *)(pUTF8EncodingString + slen + 1);              /* include NUL terminator */
	
	
	if (NULL != pUTF8EncodingString) {
		ErrorCode = ConvertUTF8toUTF16 ((const UTF8 **)&source_start,source_end,NULL,NULL,strictConversion,&bytes);		
		if (ErrorCode == conversionOK) 
			return bytes;
	}
	return 0;
}


/*
 * Get a UTF16 * representation of a UTF8 format char *
 * The representation is a converted copy, so the result needs to be freed
 * char * s == NULL is handled properly
 * count*sizeof(UTF16) is bytes count for result;
 * Does not handle byte arrays, only null-terminated strings.
 */
UTF16 * UTF8toUTF16(char * s, int * count)
{
    UTF16 * buf=NULL;
	
    if (NULL!=s) {
        unsigned int widechrs,bytes; 
        int slen;
        ConversionResult ret;
        UTF8 *source_start, *source_end;
        UTF16 *target_start, *target_end;
		
        slen = strlen(s);
		
        bytes = UTF8StringCount(s);
        widechrs = bytes / sizeof(UTF16);
		*count = widechrs;
        
		buf=(UTF16 *)malloc((widechrs+1)*sizeof(UTF16));
		memset(buf,0,(widechrs+1)*sizeof(UTF16));
		
        if (widechrs != 0) {
            source_start = (UTF8 *)s;
            source_end = (UTF8 *)(s + slen + 1);              /* include NUL terminator */
			target_start = (UTF16 *)buf;
            target_end = (UTF16 *)(buf + widechrs + 1);
			
            
            ret = ConvertUTF8toUTF16(
									 (const UTF8 **)&source_start, source_end,
									 &target_start, target_end, strictConversion, &bytes);
            if (ret != conversionOK) {
                printf("UNICODE_ERROR: second call to ConvertUTF8toUTF16 failed (%d)", ret);
            }
        }
    }
    return buf;
}

