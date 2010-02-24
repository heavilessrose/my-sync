#import "wk_engin.h"

#import <Foundation/Foundation.h>
#import "MyLog.h"
#import "wk_xml.h"
#import "wk_template_api.h"
#import "wk_iphoneccshw.h"

extern int Winks_xml_parse( Winks_XmlMem_s* pdata, Winks_XmlInt_s** ppxml );
extern int Winks_xml_free( Winks_XmlInt_s* pxml );

/*
 Winks_UserGetCCShow_e,
 Winks_UserStopCCShow_e,
 Winks_log_cc_show_e,
 Winks_if_show_cc_e
 */

/*更改unicode字节序
 unictr unicode编码的字符串
 unilen byte字节数
 */
extern int client_sockfd;
extern Winks_CCSW_Global_s gCCSW;
void Winks_ChangeUniSeq(const unsigned char *unictr,int unilen) {
	if (unilen <=0 || unilen%2 != 0)
		return;
	
	unsigned char p;
	for (int i = 0;i< unilen/2;i++) {
		memset(&p,0,1);
		memcpy(&p,unictr+i*2,1);
		memcpy((void *)(unictr+i*2),(void *)(unictr+i*2+1),1);
		memcpy((void *)(unictr+i*2+1),&p,1);
	}
		
}
/* retBuff -- 返回的内存缓冲区 
 retLen -- 输入内存缓冲区大小，输出实际使用的内存缓冲区大小
 IsDft -- 是否获取默认的配置
 */
int Winks_UserGetCCShowSetup(char * retBuff, int * retLen, int IsDft) {
	return WINKS_RETURN_FAILURE;
}

/*获取通话号码的彩像*/
int Winks_UserGetCCShow(char * phonenumber, int dir, WinksStrgFile_CCShow_s *pCCShow) {
	Winks_printf("into Winks_UserGetCCShow phonenumber=%s",phonenumber);
	int ret = 0;
	//清零
	memset(pCCShow,0,sizeof(WinksStrgFile_CCShow_s));
	
#ifndef WINKS_LONG_SOCKET
	if (Winks_InitSocket() != WINKS_RETURN_SUCCESS)
		return WINKS_RETURN_FAILURE;
#endif
	
	int processid = WINKS_PROCESSID;
	int numLen = strlen(phonenumber) + 1;
	int totallen = WINKS_INT_LENGTH * 9 + numLen;
	
	int funid = Winks_UserGetCCShow_e;
	int funlen = totallen - WINKS_INT_LENGTH * 4;
	
	char *msgdata = NULL;
	char *tmpdata = NULL;	
	
	msgdata = malloc(totallen);
	if(msgdata)
	{
		memset(msgdata, 0, totallen);
		tmpdata = msgdata;
		memcpy(tmpdata, &processid, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		ret = totallen - 8;
		memcpy(tmpdata, &ret, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, &funid, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, &funlen, WINKS_INT_LENGTH);
		
		//第一参数
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		ret = 0;
		memcpy(tmpdata, &ret, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, &numLen, WINKS_INT_LENGTH);		
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, phonenumber, numLen);
		//第二参数
		tmpdata = tmpdata + numLen;
		ret = 1;		
		memcpy(tmpdata, &ret, WINKS_INT_LENGTH);		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		
		ret = WINKS_INT_LENGTH;		
		memcpy(tmpdata, &ret, WINKS_INT_LENGTH);		
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, &dir, ret);		
		
		ret = send(client_sockfd,msgdata,totallen,0);
		
		if(msgdata)
		{
			free(msgdata);
			msgdata = NULL;
		}
		
		if (ret <= 0) 
		{
			Winks_printfErr("Winks_Write:error=%s  the ret=%d",strerror(errno),ret);
#ifndef WINKS_LONG_SOCKET	
			Winks_CloseSocket();
#endif
			return WINKS_RETURN_FAILURE;
		}		
	}
	else
	{
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}
#if 0
	//输入进程号
	//Winks_printf("ready for write data");
	Winks_Write(&processid,WINKS_INT_LENGTH);
	Winks_Write(&totallen,WINKS_INT_LENGTH);
	//Winks_printf("Winks_UserGetCCShow:totallen=%d",totallen);
	//输入函数号
	Winks_Write(&funid,WINKS_INT_LENGTH);
	//Winks_printf("Winks_UserGetCCShow:funid=%d",funid);
	Winks_Write(&funlen,WINKS_INT_LENGTH);
	
	//输入第一个参数
	int parameterId = 0;
	int parameterLen = strlen(phonenumber);
	Winks_Write(&parameterId,WINKS_INT_LENGTH);
	Winks_Write(&parameterLen,WINKS_INT_LENGTH);
	Winks_Write(phonenumber,parameterLen);
	
	//输入第二个参数
	parameterId = 1;
	parameterLen = WINKS_INT_LENGTH;
	Winks_Write(&parameterId,WINKS_INT_LENGTH);
	Winks_Write(&parameterLen,WINKS_INT_LENGTH);
	Winks_Write(&dir,parameterLen);
	
	//flush数据
	ret = Winks_Write_flush();
	
	if(ret <= 0 )
	{
		Winks_printfErr("ERROR: Winks_Write_flush the server socket retrun error=%s",strerror(errno));
		return WINKS_RETURN_FAILURE;
	}
#endif
	//读数据到缓冲
	if (Winks_Read_flush() < 0) {
		Winks_printfErr("ERROR: the server socket retrun error=%s",strerror(errno));
		Winks_Close_Read_flush();
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}
	
	//校验返回结果
	//Winks_printf("Winks_UserGetCCShow:ready for read data");
	int errcode = 0;
	Winks_Read(&errcode,WINKS_INT_LENGTH);
	if (errcode < 0) {
		Winks_printfErr("ERROR: the server socket retrun errcode=%d",errcode);
		Winks_Close_Read_flush();
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}
	
	//获取返回值总长度
	int reslen = 0;
	//Winks_Lseek(WINKS_INT_LENGTH,SEEK_CUR);
	Winks_Read(&reslen,WINKS_INT_LENGTH);
	//Winks_printf("Winks_UserGetCCShow:the result length=%d",reslen);

	//获取返回值
	int res = 0;
	Winks_Lseek(WINKS_INT_LENGTH*2,SEEK_CUR);
	Winks_Read(&res,WINKS_INT_LENGTH);
	if (res < 0) {
		Winks_printfErr("Winks_UserGetCCShow:return the result res=%d",res);
		Winks_Close_Read_flush();
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}
	

	
	//获取pCCShow值
	int tmplen = 0;
	Winks_Lseek(WINKS_INT_LENGTH*2,SEEK_CUR);
	//获取RetStatus
	Winks_Lseek(WINKS_INT_LENGTH*2,SEEK_CUR);
	Winks_Read(&(pCCShow->RetStatus),WINKS_INT_LENGTH);
	//pCCShow->RetStatus = 2;
	//Winks_printf("Winks_UserGetCCShow:pCCShow->RetStatus=%d",pCCShow->RetStatus);
	//获取WK_NO
	Winks_Lseek(WINKS_INT_LENGTH,SEEK_CUR);
	Winks_Read(&tmplen,WINKS_INT_LENGTH);
	//Winks_printf("Winks_UserGetCCShow:pCCShow->WK_NO_tmplen=%d",tmplen);
	Winks_Read(pCCShow->WK_NO,tmplen);
	//Winks_printf("Winks_UserGetCCShow:pCCShow->WK_NO=%s",pCCShow->WK_NO);
	//获取Name
	Winks_Lseek(WINKS_INT_LENGTH,SEEK_CUR);
	Winks_Read(&tmplen,WINKS_INT_LENGTH);
	//Winks_printf("Winks_UserGetCCShow:pCCShow->Name_tmplen=%d",tmplen);
	Winks_Read(pCCShow->Name,tmplen);
	//Winks_printf("Winks_UserGetCCShow:pCCShow->Name=%s",pCCShow->Name);
	//获取WK_ID
	Winks_Lseek(WINKS_INT_LENGTH,SEEK_CUR);
	Winks_Read(&tmplen,WINKS_INT_LENGTH);
	//Winks_printf("Winks_UserGetCCShow:pCCShow->WK_ID_tmplen=%d",tmplen);
	Winks_Read(pCCShow->WK_ID,tmplen);
	//Winks_printf("Winks_UserGetCCShow:pCCShow->WK_ID=%s",pCCShow->WK_ID);
	//获取CC_Idx
	Winks_Lseek(WINKS_INT_LENGTH*2,SEEK_CUR);
	Winks_Read(&(pCCShow->CC_Idx),sizeof(short));	
	//Winks_printf("Winks_UserGetCCShow:pCCShow->CC_Idx=%d",pCCShow->CC_Idx);
	//获取img_type
	Winks_Lseek(WINKS_INT_LENGTH*2,SEEK_CUR);
	Winks_Read(&(pCCShow->img_type),sizeof(char));
	////////
	//pCCShow->img_type = WINKS_FILE_EXT_jpg;
	
	//Winks_printf("Winks_UserGetCCShow:pCCShow->img_type=%d",pCCShow->img_type);//
	//获取img_cnt
	Winks_Lseek(WINKS_INT_LENGTH*2,SEEK_CUR);
	Winks_Read(&(pCCShow->img_cnt),sizeof(char));
	//Winks_printf("Winks_UserGetCCShow:pCCShow->img_cnt=%d",pCCShow->img_cnt);//
	//获取GreetLen
	Winks_Lseek(WINKS_INT_LENGTH*2,SEEK_CUR);
	Winks_Read(&(pCCShow->GreetLen),sizeof(char));	
	//Winks_printf("Winks_UserGetCCShow:pCCShow->GreetLen=%d",pCCShow->GreetLen);//
	//获取SaveType
	Winks_Lseek(WINKS_INT_LENGTH*2,SEEK_CUR);
	Winks_Read(&(pCCShow->SaveType),sizeof(char));
	//Winks_printf("Winks_UserGetCCShow:pCCShow->SaveType=%d",pCCShow->SaveType);//
	//获取pCCImage
	Winks_Lseek(WINKS_INT_LENGTH,SEEK_CUR);
	Winks_Read(&tmplen,WINKS_INT_LENGTH);
	//Winks_printf("Winks_UserGetCCShow:pCCShow->pCCImage_tmplen=%d",tmplen);
	//should alloc memery
	//pCCShow->pCCImage = alloc(tmplen);
	//Winks_Read(pCCShow->pCCImage,tmplen);	
	//Winks_printf("Winks_UserGetCCShow:pCCShow->pCCImage=%s",pCCShow->pCCImage);
	
	//获取CCImageSize
	//Winks_Lseek(WINKS_INT_LENGTH*2,SEEK_CUR);
	Winks_Lseek(WINKS_INT_LENGTH,SEEK_CUR);
	Winks_Read(&tmplen,WINKS_INT_LENGTH);
	//Winks_printf("Winks_UserGetCCShow:pCCShow->CCImageSize_tmplen=%d",tmplen);
	//Winks_Read(&(pCCShow->CCImageSize),sizeof(long));
	//Winks_printf("Winks_UserGetCCShow:pCCShow->CCImageSize=%ld",pCCShow->CCImageSize);
	
	//获取CCImgPath
	Winks_Lseek(WINKS_INT_LENGTH,SEEK_CUR);
	Winks_Read(&tmplen,WINKS_INT_LENGTH);
	//Winks_printf("Winks_UserGetCCShow:pCCShow->CCImgPath_tmplen=%d",tmplen);
	Winks_Read(*(pCCShow->CCImgPath),tmplen);
	////////
//	memset(*(pCCShow->CCImgPath),0,tmplen);
//	char *test = "/var/mobile/winkstest.jpg";
//	memcpy(*(pCCShow->CCImgPath), test, strlen(test));
	
	//Winks_printf("Winks_UserGetCCShow:pCCShow->CCImgPath=%s",*(pCCShow->CCImgPath));
	//获取GreetStr
	Winks_Lseek(WINKS_INT_LENGTH,SEEK_CUR);
	Winks_Read(&tmplen,WINKS_INT_LENGTH);
	//Winks_printf("Winks_UserGetCCShow:tmplen=%d,pCCShow->GreetStr_tmplen=%d",tmplen,pCCShow->GreetLen);
	Winks_Read(pCCShow->GreetStr,tmplen);
	//Winks_HEX_DataOut(pCCShow->GreetStr,tmplen,"[Winks_UserGetCCShow1:GreetStr]");
	Winks_ChangeUniSeq((unsigned char *)(pCCShow->GreetStr),tmplen);
	//Winks_printf("Winks_UserGetCCShow2:pCCShow=0x%x",pCCShow);
	//Winks_HEX_DataOut(pCCShow->GreetStr,tmplen,"Winks_UserGetCCShow2:GreetStr");
	

	
	//NSString *nsgreet = [[NSString alloc] initWithBytes:(void *)pCCShow->GreetStr length:winks_strlen_ucs2_type((char *)pCCShow->GreetStr) encoding:NSUnicodeStringEncoding];
//	Winks_printf("FirstUpdNumberController:: greet = %s", [nsgreet UTF8String]);
	
//	NSString *nsstr = [NSString stringWithCharacters:(unichar*)pCCShow->GreetStr length:pCCShow->GreetLen];
//	Winks_HEX_DataOut(pCCShow->GreetStr,tmplen,"Winks_UserGetCCShow:");
//	Winks_printf("Winks_UserGetCCShow:pCCShow->GreetStr=%s",pCCShow->GreetStr);
//	Winks_printf("Winks_UserGetCCShow:pCCShow->GreetStr=%s",[nsstr UTF8String]);
	
	Winks_Close_Read_flush();
#ifndef WINKS_LONG_SOCKET	
	Winks_CloseSocket();
#endif
	Winks_printf("out Winks_UserGetCCShow the res=%d",res);
	return res;//WINKS_RETURN_SUCCESS;//res;
}

/*停止展示彩像*/
int Winks_UserStopCCShow(void) {
	Winks_printf("into Winks_UserStopCCShow");
	int ret = 0;
#ifndef WINKS_LONG_SOCKET
	if (Winks_InitSocket() != WINKS_RETURN_SUCCESS)
		return WINKS_RETURN_FAILURE;
#endif
	int processid = WINKS_PROCESSID;
	int totallen = WINKS_INT_LENGTH * 4;
	
	int funid = Winks_UserStopCCShow_e;
	int funlen = 0;
	char *msgdata = NULL;
	char *tmpdata = NULL;	
	
	msgdata = malloc(totallen);
	if(msgdata)
	{
		memset(msgdata, 0, totallen);
		tmpdata = msgdata;
		memcpy(tmpdata, &processid, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		ret = totallen - 8;
		memcpy(tmpdata, &ret, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, &funid, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, &funlen, WINKS_INT_LENGTH);
		
		ret = send(client_sockfd,msgdata,totallen,0);
		
		if(msgdata)
		{
			free(msgdata);
			msgdata = NULL;
		}
		
		if (ret <= 0) 
		{
			Winks_printfErr("Winks_Write:error=%s  the ret=%d",strerror(errno),ret);
#ifndef WINKS_LONG_SOCKET	
			Winks_CloseSocket();
#endif
			return WINKS_RETURN_FAILURE;
		}		
	}
	else
	{
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}
	
#if 0
	//输入进程号
	//Winks_printf("ready for write data");
	Winks_Write(&processid,WINKS_INT_LENGTH);
	Winks_Write(&totallen,WINKS_INT_LENGTH);
	//输入函数号
	Winks_Write(&funid,WINKS_INT_LENGTH);
	//Winks_printf("Winks_UserStopCCShow:funid=%d",funid);
	Winks_Write(&funlen,WINKS_INT_LENGTH);
	
	//flush数据
	ret = Winks_Write_flush();
	if(ret <= 0)
	{
		Winks_printfErr("ERROR: Winks_Write_flush the server socket retrun error=%s",strerror(errno));
		return WINKS_RETURN_FAILURE;
	}
#endif	
	
	//读数据到缓冲
	if (Winks_Read_flush() < 0) {
		Winks_printfErr("ERROR: the server socket retrun error=%s",strerror(errno));
		Winks_Close_Read_flush();
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}
	
	//校验返回结果
	//Winks_printf("Winks_UserStopCCShow:ready for read data");
	int errcode = 0;
	Winks_Read(&errcode,WINKS_INT_LENGTH);
	if (errcode < 0) {
		Winks_printfErr("ERROR: the server socket retrun errcode=%d",errcode);
		Winks_Close_Read_flush();
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}
	
	int reslen = 0;
	//获取返回值总长度
	//Winks_Lseek(WINKS_INT_LENGTH,SEEK_CUR);
	Winks_Read(&reslen,WINKS_INT_LENGTH);
	//Winks_printf("Winks_UserStopCCShow:the result length=%d",reslen);
	int res = 0;
	//获取返回值
	Winks_Lseek(WINKS_INT_LENGTH*2,SEEK_CUR);
	Winks_Read(&res,WINKS_INT_LENGTH);
	
	Winks_Close_Read_flush();
	
#ifndef WINKS_LONG_SOCKET	
	Winks_CloseSocket();
#endif
	Winks_printf("out Winks_UserStopCCShow the res=%d",res);
	return res;
}

/* 记录此次彩像展示情况。返回负值表示失败 */
int Winks_log_cc_show(char * CC_ID, unsigned long time, unsigned char interrupted) {
	Winks_printf("into Winks_log_cc_show");
#ifndef WINKS_LONG_SOCKET
	if (Winks_InitSocket() != WINKS_RETURN_SUCCESS)
		return WINKS_RETURN_FAILURE;
#endif
	int processid = WINKS_PROCESSID;
	int cc_idLen = strlen(CC_ID) + 1;
	int totallen = WINKS_INT_LENGTH * 10 + cc_idLen  + sizeof(long) + 1; //sizeof(char);
	
	int funid = Winks_log_cc_show_e;
	int funlen = totallen - WINKS_INT_LENGTH *4;
	
	char *msgdata = NULL;
	char *tmpdata = NULL;
	int ret = 0;
	
	msgdata = malloc(totallen);
	if(msgdata)
	{
		memset(msgdata, 0, totallen);
		tmpdata = msgdata;
		memcpy(tmpdata, &processid, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		ret = totallen - 8;
		memcpy(tmpdata, &ret, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, &funid, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, &funlen, WINKS_INT_LENGTH);
		
		//第一参数
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		ret = 0;
		memcpy(tmpdata, &ret, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, &cc_idLen, WINKS_INT_LENGTH);		
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, CC_ID, cc_idLen);
		//第二参数
		tmpdata = tmpdata + cc_idLen;
		ret = 1;		
		memcpy(tmpdata, &ret, WINKS_INT_LENGTH);		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		
		ret = sizeof(long);		
		memcpy(tmpdata, &ret, WINKS_INT_LENGTH);		
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, &time, ret);
		//第三参数
		tmpdata = tmpdata + ret;
		ret = 2;		
		memcpy(tmpdata, &ret, WINKS_INT_LENGTH);		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		
		ret = 1;		
		memcpy(tmpdata, &ret, WINKS_INT_LENGTH);		
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, &interrupted, ret);
		
		ret = send(client_sockfd,msgdata,totallen,0);
		
		if(msgdata)
		{
			free(msgdata);
			msgdata = NULL;
		}
		
		if (ret <= 0) {
			Winks_printf("Winks_Write:error=%s  the ret=%d",strerror(errno),ret);
#ifndef WINKS_LONG_SOCKET	
			Winks_CloseSocket();
#endif
			WINKS_RETURN_FAILURE;
		}		
	}
	else
	{
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}
	
#if 0	
	//输入进程号
	//Winks_printf("ready for write data");
	Winks_Write(&processid,WINKS_INT_LENGTH);
	Winks_Write(&totallen,WINKS_INT_LENGTH);
	//输入函数号
	Winks_Write(&funid,WINKS_INT_LENGTH);
	//Winks_printf("Winks_log_cc_show:funid=%d",funid);
	Winks_Write(&funlen,WINKS_INT_LENGTH);
	
	//输入第一个参数
	int parameterId = 0;
	int parameterLen = strlen(CC_ID);
	Winks_Write(&parameterId,WINKS_INT_LENGTH);
	Winks_Write(&parameterLen,WINKS_INT_LENGTH);
	Winks_Write(CC_ID,parameterLen);
	
	//输入第二个参数
	parameterId = 1;
	parameterLen = sizeof(long);
	Winks_Write(&parameterId,WINKS_INT_LENGTH);
	Winks_Write(&parameterLen,WINKS_INT_LENGTH);
	Winks_Write(&time,parameterLen);
	
	//输入第三个参数
	parameterId = 1;
	parameterLen = sizeof(char);
	Winks_Write(&parameterId,WINKS_INT_LENGTH);
	Winks_Write(&parameterLen,WINKS_INT_LENGTH);
	Winks_Write(&interrupted,parameterLen);	
	
	//flush数据
	if(0 >= Winks_Write_flush())
	{
		Winks_printfErr("ERROR: Winks_Write_flush the server socket retrun error=%s",strerror(errno));
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}
#endif	
	
	//读数据到缓冲
	if (Winks_Read_flush() < 0) {
		Winks_printfErr("ERROR: the server socket retrun error=%s",strerror(errno));
		Winks_Close_Read_flush();
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}
	
	//校验返回结果
	//Winks_printf("Winks_log_cc_show:ready for read data");
	int errcode = 0;
	Winks_Read(&errcode,WINKS_INT_LENGTH);
	if (errcode < 0) {
		Winks_printfErr("ERROR: the server socket retrun errcode=%d",errcode);
		Winks_Close_Read_flush();
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}
	
	int reslen = 0;
	//获取返回值总长度
	//Winks_Lseek(WINKS_INT_LENGTH,SEEK_CUR);
	Winks_Read(&reslen,WINKS_INT_LENGTH);
	//Winks_printf("Winks_log_cc_show:the result length=%d",reslen);
	int res = 0;
	//获取返回值
	Winks_Lseek(WINKS_INT_LENGTH*2,SEEK_CUR);
	Winks_Read(&res,WINKS_INT_LENGTH);
	
	Winks_Close_Read_flush();
	
#ifndef WINKS_LONG_SOCKET	
	Winks_CloseSocket();
#endif
	Winks_printf("out Winks_log_cc_show the res=%d",res);
	return res;
}

/*获取是否展示彩像标志. 1:展示,0:不展示*/
int Winks_if_show_cc(void) {

	Winks_printf("into Winks_if_show_cc");
#ifndef WINKS_LONG_SOCKET
	if (Winks_InitSocket() != WINKS_RETURN_SUCCESS)
		return WINKS_RETURN_FAILURE;
#endif
	int processid = WINKS_PROCESSID;
	int totallen = WINKS_INT_LENGTH * 4;
	Winks_printf("Winks_if_show_cc:totallen=%d",totallen);	
	int funid = Winks_if_show_cc_e;
	int funlen = 0;
	char *msgdata = NULL;
	char *tmpdata = NULL;
	int ret = 0;
	
	msgdata = malloc(totallen);
	if(msgdata)
	{
		memset(msgdata, 0, totallen);
		tmpdata = msgdata;
		memcpy(tmpdata, &processid, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		ret =totallen - 8;
		memcpy(tmpdata, &ret, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, &funid, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, &funlen, WINKS_INT_LENGTH);
		
		ret = send(client_sockfd,msgdata,totallen,0);
		
		if(msgdata)
		{
			free(msgdata);
			msgdata = NULL;
		}
		
		if (ret <= 0) {
			Winks_printfErr("Winks_Write:error=%s  the ret=%d",strerror(errno),ret);
#ifndef WINKS_LONG_SOCKET	
			Winks_CloseSocket();
#endif
			return WINKS_RETURN_FAILURE;
		}		
	}
	else
	{
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}
	
#if 0	
	//输入进程号
	Winks_printf("write the data");
	Winks_Write(&processid,WINKS_INT_LENGTH);
	Winks_Write(&totallen,WINKS_INT_LENGTH);
	//输入函数号
	Winks_Write(&funid,WINKS_INT_LENGTH);
	Winks_printf("Winks_if_show_cc:funid=%d",funid);	
	Winks_Write(&funlen,WINKS_INT_LENGTH);
	
	//flush数据
	
	if(Winks_Write_flush() <= 0 )
	{
		Winks_printfErr("ERROR: the server socket  send  retrun error=%s",strerror(errno));
		return WINKS_RETURN_FAILURE;
	}
#endif
	
	Winks_printf("Winks_if_show_cc:Winks_Write_flush finished");
	//读数据到缓冲
	if (Winks_Read_flush() < 0) {
		Winks_printfErr("ERROR: the server socket retrun error=%s",strerror(errno));
		Winks_Close_Read_flush();
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}
	
	//校验返回结果
	int errcode = 0;
	Winks_printf("Winks_if_show_cc:ready for read data");
	Winks_Read(&errcode,WINKS_INT_LENGTH);
	if (errcode < 0) {
		Winks_printfErr("ERROR: the server socket retrun errcode=%d",errcode);
		Winks_Close_Read_flush();
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}
	
	int reslen = 0;
	//获取返回值总长度
	//Winks_Lseek(WINKS_INT_LENGTH,SEEK_CUR);
	Winks_Read(&reslen,WINKS_INT_LENGTH);
	//Winks_printf("Winks_if_show_cc:the result length=%d",reslen);
	
	int res = 0;
	//获取返回值
	Winks_Lseek(WINKS_INT_LENGTH*2,SEEK_CUR);
//	Winks_Read(&res,WINKS_INT_LENGTH);
//	Winks_printf("Winks_if_show_cc:res T=%d",res);
//	Winks_Read(&res,WINKS_INT_LENGTH);
//	Winks_printf("Winks_if_show_cc:res L=%d",res);
	Winks_Read(&res,WINKS_INT_LENGTH);
	//Winks_printf("Winks_if_show_cc:res V=%d",res);
	
#ifndef WINKS_LONG_SOCKET	
	Winks_CloseSocket();
#endif
	Winks_Close_Read_flush();
	
	Winks_printf("out Winks_if_show_cc the res=%d",res);
	return res;
}

/*获取是否展示片头标志。1: 播放, 其它: 不播*/
int Winks_if_show_titleCC(void) 
{
	Winks_printf("into Winks_if_show_titleCC");
#ifndef WINKS_LONG_SOCKET
	if (Winks_InitSocket() != WINKS_RETURN_SUCCESS)
		return WINKS_RETURN_FAILURE;
#endif
	int processid = WINKS_PROCESSID;
	int totallen = WINKS_INT_LENGTH * 4;
	
	int funid = Winks_if_show_titleCC_e;
	int funlen = 0;
	char *msgdata = NULL;
	char *tmpdata = NULL;
	int ret = 0;
	
	msgdata = malloc(totallen);
	if(msgdata)
	{
		memset(msgdata, 0, totallen);
		tmpdata = msgdata;
		memcpy(tmpdata, &processid, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		ret = totallen -8;
		memcpy(tmpdata, &ret, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, &funid, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, &funlen, WINKS_INT_LENGTH);
		
		ret = send(client_sockfd,msgdata,totallen,0);
		
		if(msgdata)
		{
			free(msgdata);
			msgdata = NULL;
		}
		
		if (ret <= 0) 
		{
			Winks_printfErr("Winks_Write:error=%s  the ret=%d",strerror(errno),ret);
#ifndef WINKS_LONG_SOCKET	
			Winks_CloseSocket();
#endif
			return WINKS_RETURN_FAILURE;
		}
		
		
		
	}
	else
	{
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}
#if 0	
	//输入进程号
	//Winks_printf("ready for write data");
	Winks_Write(&processid,WINKS_INT_LENGTH);
	Winks_Write(&totallen,WINKS_INT_LENGTH);
	//输入函数号
	Winks_Write(&funid,WINKS_INT_LENGTH);
	//Winks_printf("Winks_if_show_titleCC:funid=%d",funid);
	Winks_Write(&funlen,WINKS_INT_LENGTH);
	
	//flush数据
	if(Winks_Write_flush() <= 0)
	{
		Winks_printfErr("ERROR: Winks_Write_flush the server socket retrun error=%s",strerror(errno));
		return WINKS_RETURN_FAILURE;
	}
#endif
	
	//读数据到缓冲
	if (Winks_Read_flush() < 0) {
		Winks_printfErr("ERROR: the server socket retrun error=%s",strerror(errno));
		Winks_Close_Read_flush();
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}
	
	//校验返回结果
	//Winks_printf("Winks_if_show_titleCC:ready for read data");
	int errcode = 0;
	Winks_Read(&errcode,WINKS_INT_LENGTH);
	if (errcode < 0) {
		Winks_printfErr("ERROR: the server socket retrun errcode=%d",errcode);
		Winks_Close_Read_flush();
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}
	
	int reslen = 0;
	//获取返回值总长度
	//Winks_Lseek(WINKS_INT_LENGTH,SEEK_CUR);
	Winks_Read(&reslen,WINKS_INT_LENGTH);
	//Winks_printf("Winks_if_show_titleCC: the result length=%d",reslen);
	int res = 0;
	//获取返回值
	Winks_Lseek(WINKS_INT_LENGTH*2,SEEK_CUR);
//	Winks_Read(&res,WINKS_INT_LENGTH);
//	Winks_printf("Winks_if_show_titleCC:res T=%d",res);
//	Winks_Read(&res,WINKS_INT_LENGTH);
//	Winks_printf("Winks_if_show_titleCC:res L=%d",res);
	Winks_Read(&res,WINKS_INT_LENGTH);
	//Winks_printf("Winks_if_show_titleCC:res V=%d",res);
	
#ifndef WINKS_LONG_SOCKET	
	Winks_CloseSocket();
#endif
	Winks_Close_Read_flush();
	Winks_printf("out Winks_if_show_titleCC the res=%d",res);
	return res;
	//return 1;
}


/*向引擎报告电话事件*/
int Winks_post_call_event (WinksCallEvent_e event,char* number) {
	Winks_printf("into Winks_post_call_event");
	int ret = 0;
	char *msgdata = NULL;
	char *tmpdata = NULL;
#ifndef WINKS_LONG_SOCKET
	if (Winks_InitSocket() != WINKS_RETURN_SUCCESS)
		return WINKS_RETURN_FAILURE;
#endif
	int processid = WINKS_PROCESSID;
	int totallen = WINKS_INT_LENGTH * 4;
	int numberLen =  0;
	if (number != NULL) 
	{
		numberLen = strlen(number) +1;
		totallen = totallen + numberLen;
	}
	else
	{
		return WINKS_RETURN_FAILURE;
	}
	
	msgdata = malloc(totallen);
	if(msgdata)
	{
		memset(msgdata, 0, totallen);
		tmpdata = msgdata;
		memcpy(tmpdata, &processid, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		ret = totallen - 8;
		memcpy(tmpdata, &ret, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, &event, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, &numberLen, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, number, numberLen);
		
		ret = send(client_sockfd,msgdata,totallen,0);
		if (ret <= 0) {
			Winks_printfErr("Winks_Write:error=%s  the ret=%d",strerror(errno),ret);
		}
		
		if(msgdata)
		{
			free(msgdata);
			msgdata = NULL;
		}
		
	}
	else
	{
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}
//	Winks_printf("Winks_post_call_event:numlen=%d,num=%s",strlen(number),number);
//	Winks_printf("Winks_post_call_event:totallen=%d",totallen);
#if 0	
	int funlen = 0;
	//输入进程号
	//Winks_printf("ready for write data");
	Winks_Write(&processid,WINKS_INT_LENGTH);
	Winks_Write(&totallen,WINKS_INT_LENGTH);
	
	//输入事件号
	Winks_Write(&event,WINKS_INT_LENGTH);
	//Winks_printf("Winks_post_call_event:event=%d",event);
	
	funlen = strlen(number);
		//输入手机号长度
	Winks_Write(&funlen,WINKS_INT_LENGTH);
	
	//输入手机号
	Winks_Write(number,funlen);
	
	
	//flush数据
	if(Winks_Write_flush() <= 0)
	{
		Winks_printfErr("ERROR: Winks_Write_flush the server socket retrun error=%s",strerror(errno));
		return WINKS_RETURN_FAILURE;
	}
#endif
	
	int funid = 0;//函数号
	
	int tmp = recv(client_sockfd,&funid,(size_t)sizeof(int),0);
	if (tmp < 0) {
		Winks_printfErr("out Winks_Read_flush:read error");
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}
	
#ifndef WINKS_LONG_SOCKET	
	Winks_CloseSocket();
#endif
	Winks_printf("out Winks_post_call_event");
	return 0;
}

/* 获取设置项的值
 * function: Winks_GetSettingValue
 * parameters:
 *   Winks_SettingType_e type: 设置项类别
 *   unsigned long *value: (out)设置项值
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_GetSettingValue(Winks_SettingType_e type, unsigned long *value) {
	Winks_printf("into Winks_GetSettingValue type=%d",type);
	
#ifndef WINKS_LONG_SOCKET
	if (Winks_InitSocket() != WINKS_RETURN_SUCCESS)
		return WINKS_RETURN_FAILURE;
#endif
	
	int processid = WINKS_PROCESSID;
	int totallen = WINKS_INT_LENGTH * 7;
	//Winks_printf("Winks_GetSettingValue:totallen=%d",totallen);
	int funid = Winks_GetSettingValue_e;
	int funlen = WINKS_INT_LENGTH * 3;
	char *msgdata = NULL;
	char *tmpdata = NULL;
	int ret = 0;
	
	msgdata = malloc(totallen);
	if(msgdata)
	{
		memset(msgdata, 0, totallen);
		tmpdata = msgdata;
		memcpy(tmpdata, &processid, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		ret = totallen - 8;
		memcpy(tmpdata, &ret, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, &funid, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, &funlen, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, &ret, WINKS_INT_LENGTH);
		
		ret = WINKS_INT_LENGTH;
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, &ret, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, &type, WINKS_INT_LENGTH);
		
		ret = send(client_sockfd,msgdata,totallen,0);
		
		if(msgdata)
		{
			free(msgdata);
			msgdata = NULL;
		}
		
		if (ret <= 0) 
		{
			Winks_printfErr("Winks_Write:error=%s  the ret=%d",strerror(errno),ret);
#ifndef WINKS_LONG_SOCKET	
			Winks_CloseSocket();
#endif
			return WINKS_RETURN_FAILURE;
		}		
	}
	else
	{
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return	WINKS_RETURN_FAILURE;
	}
#if 0
	//输入进程号
	//Winks_printf("ready for write data");
	Winks_Write(&processid,WINKS_INT_LENGTH);
	Winks_Write(&totallen,WINKS_INT_LENGTH);
	//Winks_printf("into Winks_GetSettingVa0");
	//输入函数号
	Winks_Write(&funid,WINKS_INT_LENGTH);
	//Winks_printf("Winks_GetSettingValue:funid=%d",funid);
	Winks_Write(&funlen,WINKS_INT_LENGTH);
	
	//输入第一个参数
	int parameterId = 0;
	int parameterLen = WINKS_INT_LENGTH;
	Winks_Write(&parameterId,WINKS_INT_LENGTH);
	Winks_Write(&parameterLen,WINKS_INT_LENGTH);
	//Winks_printf("Winks_GetSettingValue type=%d",type);
	Winks_Write(&type,parameterLen);
	
	//flush数据
	if(Winks_Write_flush() <= 0)
	{
		Winks_printfErr("ERROR: Winks_Write_flush the server socket retrun error=%s",strerror(errno));
		return WINKS_RETURN_FAILURE;
	}
#endif
	
	//读数据到缓冲
	if (Winks_Read_flush() < 0) {
		Winks_printfErr("ERROR: the server socket retrun error=%s",strerror(errno));
		Winks_Close_Read_flush();
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}
	
	//校验返回结果
	//Winks_printf("Winks_GetSettingValue:ready for read data");
	int errcode = 0;
	Winks_Read(&errcode,WINKS_INT_LENGTH);
	if (errcode < 0) {
		Winks_printfErr("ERROR: the server socket retrun errcode=%d",errcode);
		Winks_Close_Read_flush();
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}
	
	int reslen = 0;
	//获取返回值总长度
	//Winks_Lseek(WINKS_INT_LENGTH,SEEK_CUR);
	Winks_Read(&reslen,WINKS_INT_LENGTH);
	//Winks_printf("Winks_GetSettingValue:the result length=%d",reslen);
	int res = 0;
	//获取返回值
	Winks_Lseek(WINKS_INT_LENGTH*2,SEEK_CUR);
	Winks_Read(&res,WINKS_INT_LENGTH);
	if (res < 0) {
		Winks_printfErr("Winks_GetSettingValue:return the result res=%d",res);
		Winks_Close_Read_flush();
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}
	//获取value
	Winks_Lseek(WINKS_INT_LENGTH*2,SEEK_CUR);
	
	Winks_Read(value,sizeof(long));
	
#ifndef WINKS_LONG_SOCKET	
	Winks_CloseSocket();
#endif
	
	Winks_Close_Read_flush();
	Winks_printf("out Winks_GetSettingValue the res=%d,value=%d",res,*value);
	return res;
	//return 3;
	//return WINKS_RETURN_SUCCESS;
}

/*查询本地用户激活状态。
 
 state 5:引擎初始化失败
 state 4:引擎初始化中
 state 3:禁用用户
 state 2:退订用户
 state 1:已激活用户
 state 0:未激活用户
 */
int Winks_UserGetState(int *state) {
	Winks_printf("into Winks_UserGetState");
#ifndef WINKS_LONG_SOCKET
	if (Winks_InitSocket() != WINKS_RETURN_SUCCESS)
		return WINKS_RETURN_FAILURE;
#endif
	
	int processid = WINKS_PROCESSID;
	int totallen = WINKS_INT_LENGTH * 4;
	//Winks_printf("Winks_UserGetState:totallen=%d",totallen);
	int funid = Winks_UserGetState_e;
	int funlen = 0;
	
	char *msgdata = NULL;
	char *tmpdata = NULL;
	int ret = 0;
	
	msgdata = malloc(totallen);
	if(msgdata)
	{
		memset(msgdata, 0, totallen);
		tmpdata = msgdata;
		memcpy(tmpdata, &processid, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		ret = totallen - 8;
		memcpy(tmpdata, &ret, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, &funid, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, &funlen, WINKS_INT_LENGTH);
		
		ret = send(client_sockfd,msgdata,totallen,0);
		
		if(msgdata)
		{
			free(msgdata);
			msgdata = NULL;
		}
		
		if (ret <= 0) 
		{
			Winks_printfErr("Winks_Write:error=%s  the ret=%d",strerror(errno),ret);
#ifndef WINKS_LONG_SOCKET	
			Winks_CloseSocket();
#endif
			return WINKS_RETURN_FAILURE;
		}		
	}
	else
	{
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}
	
#if 0	
	//输入进程号
	//Winks_printf("ready for write data");
	Winks_Write(&processid,WINKS_INT_LENGTH);
	Winks_Write(&totallen,WINKS_INT_LENGTH);
	//输入函数号
	Winks_Write(&funid,WINKS_INT_LENGTH);
	//Winks_printf("Winks_UserGetState:funid=%d",funid);
	Winks_Write(&funlen,WINKS_INT_LENGTH);
	
	//flush数据
	if(Winks_Write_flush() <= 0)
	{
		Winks_printfErr("ERROR: Winks_Write_flush the server socket retrun error=%s",strerror(errno));
		return WINKS_RETURN_FAILURE;
	}
#endif
	
	//读数据到缓冲
	if (Winks_Read_flush() < 0) {
		Winks_printfErr("ERROR: the server socket retrun error=%s",strerror(errno));
		Winks_Close_Read_flush();
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}
	
//	char tmp[200] = {0};
//	Winks_Read(tmp,200);
//	Winks_HEX_DataOut(tmp,32,"Winks_UserGetState:");
#if 1
	//校验返回结果
	//Winks_printf("Winks_UserGetState:ready for read data");
	int errcode = 0;
	Winks_Read(&errcode,WINKS_INT_LENGTH);
	if (errcode < 0) {
		Winks_printfErr("ERROR: the server socket retrun errcode=%d",errcode);
		Winks_Close_Read_flush();
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}
	
	int reslen = 0;
	//获取返回值总长度
	//Winks_Lseek(WINKS_INT_LENGTH,SEEK_CUR);
	Winks_Read(&reslen,WINKS_INT_LENGTH);
	//Winks_printf("Winks_UserGetState:the result length=%d",reslen);
	int res = 0;
	//获取返回值
	Winks_Lseek(WINKS_INT_LENGTH*2,SEEK_CUR);
	Winks_Read(&res,WINKS_INT_LENGTH);
	if (res < 0) {
		Winks_printfErr("Winks_UserGetState:return the result res=%d",res);
		Winks_Close_Read_flush();
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}
	
	//获取state
	Winks_Lseek(WINKS_INT_LENGTH*2,SEEK_CUR);
	Winks_Read(state,WINKS_INT_LENGTH);
	
	Winks_Close_Read_flush();
	//Winks_printf("Winks_UserGetState the state=%d",*state);
#ifndef WINKS_LONG_SOCKET	
	Winks_CloseSocket();
#endif
	Winks_printf("out Winks_UserGetState the res=%d,state=%d",res,*state);
	return res;
#endif
}

//panqj add
/*存储电话状态 1:通话中 0:挂断，*/
void Winks_save_Callstatus () 
{
		
	FILE *fp = NULL;
	fp = fopen(WINKS_CALLSTATUS_FILE_PATH,"wb");
	
	if (fp != NULL) 
	{
		fclose(fp);
		//写缓存
		//sync();
	} else {
		Winks_printfErr("Winks_save_Callstatus:error fp=%x,errno=%s",fp,strerror(errno));
	}
	
}

void Winks_delete_Callstatus()
{	
	system("rm -f "WINKS_CALLSTATUS_FILE_PATH);
	Winks_printf("rm the file /var/mobile/winks_CallStatus !ret =%d",ret);
}

/*获取电话状态 1:通话中 0:挂断,多路电话时使用*/
int Winks_get_Callstatus () 
{
	Winks_printf("into Winks_get_Callstatus");
	int state = -1;
	FILE *fp = NULL;
	//int ret = 0;
	
	fp = fopen(WINKS_CALLSTATUS_FILE_PATH,"rb");
	Winks_printf("Winks_get_Callstatus fopen=%x",fp);
	if (fp != NULL) 
	{		
		state = 1;
		fclose(fp);
		//return state;
	} 
	else 
	{
		Winks_printfErr("Winks_get_Callstatus:error fp=%d,errno=%s",fp,strerror(errno));
	}
	
	
	Winks_printf("Winks_get_Callstatus:the multcallstate=%d",state);
	return state;
}


/*存储电话状态 1:通话中 0:闲置中，防止多路电话*/
void Winks_save_multiCallstate (int state) {
	//Winks_printf("into Winks_save_callstate:state=%d",state);
	

	//int ret = system("rm -f "WINKS_CALLSTATE_FILE_PATH);
	//Winks_printf("rm the file /var/mobile/winks_call_state !ret =%d",ret);
	
	FILE *fp = NULL;
	fp = fopen(WINKS_CALLSTATE_FILE_PATH,"wb");
	
	if (fp != NULL) 
	{
		fwrite(&state,sizeof(int),1,fp);
		//Winks_printf("Winks_save_multiCallstate:state=%d",state);
		fclose(fp);
		//写缓存
		sync();
	} else {
		Winks_printfErr("Winks_save_multiCallstate:error fp=%x,errno=%s",fp,strerror(errno));
	}
	
}

/*获取电话状态 1:通话中 0:闲置中,多路电话时使用*/
int Winks_get_multiCallstate () {
	Winks_printf("into Winks_get_multiCallstate");
	int state = -1;
	FILE *fp = NULL;
	int ret = -2;
	
	fp = fopen(WINKS_CALLSTATE_FILE_PATH,"rb");
	Winks_printf("Winks_get_multiCallstate fopen=%x",fp);
	if (fp != NULL) 
	{		
		ret = fread(&state,sizeof(int),1,fp);
		//Winks_printf("Winks_get_multiCallstate:state=%d ,read file return value=%d",state,ret);
		fclose(fp);
		//return state;
	} 
	else 
	{
		//Winks_printfErr("Winks_get_multiCallstate:error fp=%d,errno=%s",fp,strerror(errno));
	}
	
	
	Winks_printf("Winks_get_multiCallstate:the multcallstate=%d",state);
	return state;
}

char* Winks_save_globelNumber(const char *num)
{
	if (num != NULL && strlen(num) < MAX_PHONENUMBER_LENGTH) 
	{
		char* tmp = malloc(strlen(num)+1);
		if(tmp)
		{
			memset(tmp,0,strlen(num)+1);
			char *p = tmp;
			for (int i=0;i< strlen(num);i++) 
			{
				if (*(num+i) != ' ' && *(num+i) != ')' && *(num+i) != '(' && *(num+i) != '-') 
				{
					memcpy(p,num+i,1);
					p++;
				}
			}
		}
		
		return tmp;		
	}
	return NULL;
}

#if 0
/*保存去电或来电号码*/
void Winks_save_number (const char *num) 
{
	Winks_printf("into Winks_save_number the num=%s",num);
#if 1
	int len = 0;
	if(num)
	{
		
		int len = strlen(num);
		Winks_printf("out Winks_save_number1 len=%d",len);
		if (num != NULL && len < MAX_PHONENUMBER_LENGTH) 
		{
			FILE *fp = NULL;
			fp = fopen(WINKS_STORE_FILE_PATH,"wb");
			
			if (fp != NULL) 
			{
				int fd;
				fd = fileno( fp );
				//fcntl(fd, F_SETLKW, file_lock(F_WRLCK, SEEK_SET));
				//Winks_printf("Winks_save_number:lock");
				//flock(fd,LOCK_EX|LOCK_NB);
				//flock(fd,LOCK_EX);		
				//Winks_printf("Winks_save_number:len=%d",len);
				fwrite(&len,sizeof(int),1,fp);
				//Winks_printf("Winks_save_number:len=%d",len);
				fwrite(num,len,1,fp);
				
				//fcntl(fd, F_SETLK, file_lock(F_UNLCK, SEEK_SET));
				//flock(fd,LOCK_UN);
				//Winks_printf("Winks_save_number:unlock");
				
				
				//Winks_printf("Winks_save_number:number=%s",tmp);
				fclose(fp);
				//写缓存
				sync();
			} 
			else 
			{			
				Winks_printfErr("Winks_save_number:error fp=%d,errno=%s",fp,strerror(errno));
			}
		} 
				
	}
	else 
	{	Winks_printf("out Winks_save_number1");
		if(access(WINKS_STORE_FILE_PATH, 0) == 0)
		{
			int ret = system("rm -f "WINKS_STORE_FILE_PATH);
			Winks_printf("rm the file /var/mobile/winks_store !ret =%d",ret);
		}		
	}
	
#endif
	
	Winks_printf("out Winks_save_number");
	
#if 0
	Winks_printf("Winks_save_number 1");
	int shm_id;
	key_t key;
	char *p_map_number;
	//char* name = "/dev/shm/winksshm";
	char* name = "/Library/MobileSubstrate/DynamicLibraries/WinksCallHook.dylib";
	
	/*先用ftok创建一个key*/
	key = ftok(name,0);
	//key = ftok(" /",0);
	if(key==-1)
		perror("ftok error");
	Winks_printf("Winks_save_number 2 key = %d",key);

	/*调用shmget，创建一块共享内存区域*/
	shm_id=shmget(key,50,IPC_CREAT|0666); 
	Winks_printf("Winks_save_number the shm_id=%d",shm_id);
	if(shm_id==-1)
	{
		perror("shmget error");
		Winks_printf("shmsave error");
		return;
	}
	Winks_printf("Winks_save_number 3");
	/*将这块共享内存区附加到自己的内存段*/
	p_map_number=(char*)shmat(shm_id,NULL,0);
	memcpy(p_map_number,num,strlen(num));

	/*写数据完毕，将其从自己的内存段中“删除”出去*/
	if(shmdt(p_map_number)==-1)
		perror(" detach error ");
	Winks_printf("Winks_save_number 4");
#endif
	
	
}
#endif //if 0

/*获取去电或来电号码*/
#if 0
char* Winks_get_number () {
	
	Winks_printf("into Winks_get_number");
	
	//WINKS_STORE_FILE_PATH
	//int ret = system("rm -f /var/mobile/winks_store");
	//Winks_printf("rm the file /var/mobile/winks_store !ret =%d",ret);
#if 1
	FILE *fp = NULL;
	int ret = -2;
			
	fp = fopen(WINKS_STORE_FILE_PATH,"rb");
	
	if (fp != NULL) 
	{
		int len = 0;
		//int ret = 0;
		int fd;
		fd = fileno( fp );
		//fcntl(fd, F_SETLKW, file_lock(F_WRLCK, SEEK_SET));
		//Winks_printf("Winks_get_number:lock");
		//flock(fd,LOCK_EX|LOCK_NB);
		//flock(fd,LOCK_EX);
		
		ret = fread(&len,sizeof(int),1,fp);
		if(ret<1)
		{
			Winks_printf("Winks_get_number:fread len error");
			fclose(fp);
			return NULL;
		}
		//Winks_printf("Winks_get_number:len=%d",len);
		//panqj add
		if(len<1||len>128)
		{
			fclose(fp);
			return NULL;
		}
		
		char *num = malloc(len+1);
		if(num == NULL)
		{
			Winks_printfErr("Winks_get_number:malloc error");
			fclose(fp);
			return NULL;
		}
		memset(num, 0, len+1);
		ret = fread(num,len,1,fp);
		if(ret<1)
		{
			Winks_printfErr("Winks_get_number:fread num error");
			fclose(fp);
			free(num);
			num = NULL;
			return NULL;
		}
		
		//Winks_printf("Winks_get_number:num=%s",num);
	
		//flock(fd,LOCK_UN);
		//Winks_printf("Winks_get_number:unlock");
		//fcntl(fd, F_SETLK, file_lock(F_UNLCK, SEEK_SET));
		
		fclose(fp);		
		
		return num;
	} 
	else 
	{
		Winks_printfErr("Winks_get_number:error fp=%d,errno=%s",fp,strerror(errno));
	}
	
#endif	
	Winks_printf("out Winks_get_number");

	
#if 0
	Winks_printf("Winks_get_number 1");
	int shm_id;
	key_t key;
	char *p_map_number;
	char* name = "/dev/shm/winksshm";
	
	/*先用ftok创建一个key*/
	key = ftok(name,0);
	if(key == -1)
		perror("ftok error");
	Winks_printf("Winks_get_number 2");
	/*调用shmget，指向共享内存区域*/
	shm_id = shmget(key,50,IPC_R|0666);        
	if(shm_id == -1)
	{
		perror("shmget error");
		return NULL;
	}
	/*将这块共享内存区附加到自己的内存段*/
	p_map_number = (char*)shmat(shm_id,NULL,0);
	Winks_printf("Winks_get_number 3");
	/*获取手机号*/
	char* tmp = malloc(strlen(p_map_number));
	memcpy(tmp,p_map_number,strlen(p_map_number));

	/*将其从自己的内存段中“删除”出去*/
	if(shmdt(p_map_number) == -1)
		perror(" detach error ");
	Winks_printf("Winks_get_number 4");
	//实际删除共享内存
	shmctl(shm_id,IPC_RMID,NULL);
	Winks_printf("Winks_get_number 5");
#endif
	return NULL;//"15811244672";//123456
}
#endif //if 0

// 系统当前语言
static NSString* GetCurrentLanguage () {
	NSArray* languages = [NSLocale preferredLanguages];
	
	return [languages objectAtIndex:0];
}


/**************************************************************************\
 功能描述:
 获取系统当前的设置语言
 参数说明:
 无
 返回值:
 返回WINKS平台统一定义的语言代码
 实现描述:
 \**************************************************************************/
Winks_Language_s Winks_GetCurrentLanguage(void){
	NSString *lang;
	Winks_Language_s ret_lang;
	lang = GetCurrentLanguage();
	if([lang isEqualToString:@"en"]){
		ret_lang = WINKS_LANG_ENGLISH;
	}else if([lang isEqualToString:@"zh-Hans"]){ // 简体中文
		ret_lang = WINKS_LANG_CHINESE;
	}else if([lang isEqualToString:@"zh-Hant"]){ // 繁体中文
		ret_lang = WINKS_LANG_CHINESETW;
	}else {
		ret_lang = WINKS_LANG_ENGLISH;
	}
	
	return ret_lang;
}

// timer是否已存在（是否已开始计时）.存在返回1，否则返回0
static int timerexist(){
	
	struct itimerval tv;
	Winks_printf("timerexist:get timer");
	
	int ret = getitimer(ITIMER_REAL, &tv);
	if(0 != ret)
	{
		Winks_printf("timer not exist,return=%d",ret);
		return -1;
	}
	Winks_printf("timerexist:get timer end");
	if(tv.it_value.tv_sec || tv.it_value.tv_usec || tv.it_interval.tv_sec || tv.it_interval.tv_usec){
		Winks_printf("timerexist:exist timer");
		return 1;
	}
	Winks_printf("timerexist:no timer");
	return 0;
}


/******************************************************************\
 定时器机制不需要create直接用ITIMER_REAL句柄故也不需要创建定时器
 \******************************************************************/
int Winks_api_StartTimer(unsigned long delay,void (*timerfunc)(int)){
	Winks_printf("into Winks_api_StartTimer");
	if(timerexist()){
		Winks_api_StopTimer();
	}
	
	// 定时器不自动重新启动
	struct itimerval value;
	value.it_value.tv_sec = delay / 1000;
	value.it_value.tv_usec = (delay % 1000) * 1000;
	value.it_interval.tv_sec = 0;
	value.it_interval.tv_usec = 0;

	signal(SIGALRM, timerfunc); /* 定时信号处理注册 */
	setitimer(ITIMER_REAL, &value, NULL); /* 定时开始 */
	Winks_printf("out Winks_api_StartTimer:");
	return 0;
}

/***********************************
 定时器句柄无需销毁，这里停止定时器。
 **********************************/
int Winks_api_StopTimer(void){
	Winks_printf("into Winks_api_StopTimer");
	if(timerexist()){
		Winks_printf("stopping Winks_api_StopTimer");
		struct itimerval value;
		Winks_printf("value.it_value.tv_sec=%d,value.it_value.tv_usec=%d,value.it_interval.tv_sec =%d,value.it_interval.tv_usec=%d",value.it_value.tv_sec,value.it_value.tv_usec,value.it_interval.tv_sec,value.it_interval.tv_usec);
		value.it_value.tv_sec = 0;
		value.it_value.tv_usec = 0;
		value.it_interval.tv_sec = 0;
		value.it_interval.tv_usec = 0;
		if(setitimer(ITIMER_REAL, &value, NULL) == -1)
		{
			Winks_printfErr("error stoptimer:%s",strerror(errno));
		}
	}
	//printf("stoptimer call end\n");
	Winks_printf("out Winks_api_StopTimer");
	return 0;
}

/*获取unicode字符串的长度*/
int winks_strlen_ucs2_type(char * text)
{
    int nCount = 0;
    
    if (text == NULL)
    {
        return (0);
    }
    else
    {
        if(*text == 0 && *(text+ 1) == 0)
            return 0;
		
        if (text)
        {
            while (text[nCount] != 0 || text[nCount + 1] != 0)
            {
				
                nCount += 2;
            }
        }
        return nCount;
    }   
} 


/*拷贝unicode字符串*/
int winks_ccsw_UnicodeStrcpy( char* pdest, const char* psour )
{
	//Winks_printf("winks_ccsw_UnicodeStrcpy:psour len=%d",winks_strlen_ucs2_type(psour));
    int len = winks_strlen_ucs2_type( (char* )psour );
	//Winks_printf("winks_ccsw_UnicodeStrcpy:len=%d",len);
    memset(pdest,0,len + 2);
    memcpy( pdest, psour, len );
	//Winks_printf("winks_ccsw_UnicodeStrcpy:pdest len=%d",winks_strlen_ucs2_type(pdest));
    return 0;
}

/*获取未激活状态时的unicode编码的问候语*/
char* winks_ccsw_GetUniGreeting () {
	int lang = Winks_GetCurrentLanguage();
	
	if (lang == WINKS_LANG_CHINESE) {
		//Winks_printf("WINKS_LANG_CHINESE winks_ccsw_GetUniGreeting");
		//简体中文
//		const unsigned char *utf8_str = "欢迎使用彩像，您尚未激活，马上激活体验彩像的精彩吧";
		unsigned char uni_array[] = {0x6B,0x22,0x8F,0xCE,0x4F,0x7F,0x75,0x28,0x5F,0x69,0x8C,0x61,0xFF,0x0C,0x60,0xA8,0x8F,0xD8,0x6C,0xA1,
		0x67,0x09,0x6F,0xC0,0x6D,0x3B,0xFF,0x0C,0x8D,0x76,0x5F,0xEB,0x6F,0xC0,0x6D,0x3B,0x4F,0x53,0x9A,0x8C,0x5F,0x69,0x8C,0x61,
		0x76,0x84,0x7C,0xBE,0x5F,0x69,0x54,0x27,0xFF,0x01,0x00,0x00};
		//Winks_printf("WINKS_LANG_CHINESE winks_ccsw_GetUniGreeting:uni_array len=%d",sizeof(uni_array));
		//Winks_HEX_DataOut((unsigned char*)uni_array, sizeof(uni_array), (unsigned char*)"[uni_array:DATA]");
		
		unsigned char *uni_str = malloc(sizeof(uni_array));
		memset(uni_str, 0, sizeof(uni_array));
		memcpy(uni_str, uni_array, sizeof(uni_array));
		
		//Winks_HEX_DataOut(uni_str, winks_strlen_ucs2_type(uni_str), "[uni_str:DATA]");
		//Winks_printf("WINKS_LANG_CHINESE winks_ccsw_GetUniGreeting:uni_str len=%d",winks_strlen_ucs2_type(uni_str));
		return (char *)uni_str;
	} else if (lang == WINKS_LANG_CHINESETW) {
		//繁体中文
		//		const unsigned char *utf8_str = "欢迎使用彩像，您尚未激活，马上激活体验彩像的精彩吧";
		unsigned char uni_array[] = {0x6B,0x22,0x8F,0xCE,0x4F,0x7F,0x75,0x28,0x5F,0x69,0x8C,0x61,0xFF,0x0C,0x60,0xA8,0x8F,0xD8,0x6C,0xA1,
			0x67,0x09,0x6F,0xC0,0x6D,0x3B,0xFF,0x0C,0x8D,0x76,0x5F,0xEB,0x6F,0xC0,0x6D,0x3B,0x4F,0x53,0x9A,0x8C,0x5F,0x69,0x8C,0x61,
		0x76,0x84,0x7C,0xBE,0x5F,0x69,0x54,0x27,0xFF,0x01,0x00,0x00};
		//Winks_printf("WINKS_LANG_CHINESETW winks_ccsw_GetUniGreeting:uni_array len=%d",sizeof(uni_array));
		//Winks_HEX_DataOut((unsigned char*)uni_array, sizeof(uni_array), (unsigned char*)"[uni_array:DATA]");
		
		unsigned char *uni_str = malloc(sizeof(uni_array));
		memset(uni_str, 0, sizeof(uni_array));
		memcpy(uni_str, uni_array, sizeof(uni_array));
		
		//Winks_HEX_DataOut(uni_str, winks_strlen_ucs2_type(uni_str), "[uni_str:DATA]");
		//Winks_printf("WINKS_LANG_CHINESETW winks_ccsw_GetUniGreeting:uni_str len=%d",winks_strlen_ucs2_type(uni_str));
		return (char *)uni_str;
	} else {
		//英文
		//		const unsigned char *utf8_str = "欢迎使用彩像，您尚未激活，马上激活体验彩像的精彩吧";
		unsigned char uni_array[] = {0x6B,0x22,0x8F,0xCE,0x4F,0x7F,0x75,0x28,0x5F,0x69,0x8C,0x61,0xFF,0x0C,0x60,0xA8,0x8F,0xD8,0x6C,0xA1,
			0x67,0x09,0x6F,0xC0,0x6D,0x3B,0xFF,0x0C,0x8D,0x76,0x5F,0xEB,0x6F,0xC0,0x6D,0x3B,0x4F,0x53,0x9A,0x8C,0x5F,0x69,0x8C,0x61,
		0x76,0x84,0x7C,0xBE,0x5F,0x69,0x54,0x27,0xFF,0x01,0x00,0x00};
		//Winks_printf("WINKS_LANG_ENGLISH winks_ccsw_GetUniGreeting:uni_array len=%d",sizeof(uni_array));
		//Winks_HEX_DataOut((unsigned char*)uni_array, sizeof(uni_array), (unsigned char*)"[uni_array:DATA]");
		
		unsigned char *uni_str = malloc(sizeof(uni_array));
		memset(uni_str, 0, sizeof(uni_array));
		memcpy(uni_str, uni_array, sizeof(uni_array));
		
		//Winks_HEX_DataOut((unsigned char*)uni_str, winks_strlen_ucs2_type(uni_str), "[uni_str:DATA]");
		//Winks_printf("WINKS_LANG_ENGLISH winks_ccsw_GetUniGreeting:uni_str len=%d",winks_strlen_ucs2_type(uni_str));
		return (char *)uni_str;
	}
	return NULL;
}


//panqj add 2010.01.20
///////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
/* 获取指定模板的制定元素
 * function: Winks_GetShowElement
 * parameters:
 *   char *number (in)电话号码，不能为NULL。
 *          可通过输入宏（WINKS_DFT_CC_NO、WINKS_PRI_CC_NO、WINKS_BUS_CC_NO），达到特定的效果。
 *   unsigned long extDataFlag (in)外部数据标识。
 *          为WINKS_TRUE表示外部数据(pageshow中的数据)，为WINKS_FALSE表示内部数据(template中multi-part数据)。
 *   char *eTag (in)待获取元素的标识，不能为NULL。对应tempate中的CID.
 *   unsigned long *fileFlag (out)用来标识返回数据是否是文件名。
 *          为WINKS_TRUE表示eBuff中返回的是文件名，所取数据存在该文件中；
 *          为WINKS_FALSE表示eBuff中存放的是数据可以直接使用。
 *   char * eDataBuf (out)返回数据。
 *   unsigned long *eDataBufSize (in, out)传入eDataBufSize的size。当传入的buffSize不足时，返回需要的size，
 *          用户应备好足够的输出空间，重新调用该函数获取数据。
 *   char * tIdBuf (out)返回对应的TID。如果不需要获取该参数，可以传入NULL。
 *   unsigned long tIdBufSize (in)传入tIdBuf的size。
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
unsigned long Glob_fileFlag = 0;
char *Glob_eDataBuf = NULL;
unsigned long Glob_eDataBufLen = 0;
char *Glob_tIdBuf = NULL;
unsigned long Glob_tIdBufLen = 0;
int Winks_GetShowElement(char *number, unsigned long extDataFlag, char *eTag, unsigned long *fileFlag, 
						 char * eDataBuf, unsigned long* eDataBufSize, char *tIdBuf, unsigned long tIdBufSize)
{
	//for test
	char file[] = "/Users/luke/ImgWorkhard.gif";
	//char file[] = "/Users";
	int fileLen = strlen(file) + 1;
	memcpy(eDataBuf, file, fileLen);
	*eDataBufSize = fileLen;
	fileFlag = WINKS_TRUE;
	
	//test end
#if 0	
	if((!number)||(!eTag))
	{
		return -1;
	}	
	
	// 由于第一次传入获取eDataBuf不够大，第二次传入进行获取时的处理方式
	if((Glob_eDataBuf != NULL)&&(Glob_eDataBufLen != 0))
	{
		if(*eDataBufSize >= Glob_eDataBufLen)
		{
			*eDataBufSize = Glob_eDataBufLen;
			memcpy(eDataBuf,Glob_eDataBuf,Glob_eDataBufLen);
			
			if(tIdBuf != NULL&&(Glob_tIdBuf != NULL))
			{
				memcpy(tIdBuf,Glob_tIdBuf,tIdBufSize);
			}
			
			if(Glob_tIdBuf)
			{
				free(Glob_tIdBuf);
				Glob_tIdBuf = NULL;
			}		
		}
		else
		{
			*eDataBufSize = Glob_eDataBufLen;
			return -1;
		}	
		
		*fileFlag = Glob_fileFlag;
		Glob_fileFlag = 0;
		
		free(Glob_eDataBuf);
		Glob_eDataBuf = NULL;
		Glob_eDataBufLen = 0;
		
		return 0;
	}
	
#ifndef WINKS_LONG_SOCKET
	if (Winks_InitSocket() != WINKS_RETURN_SUCCESS)
		return WINKS_RETURN_FAILURE;
#endif
	
	int processid = WINKS_PROCESSID;
	int numLen = strlen(number) + 1;
	int ret = 0;
	int eTagLen = 0;
	if(eTag)
	{
		eTagLen = strlen(eTag) + 1;
	}	
	
	int totallen = WINKS_INT_LENGTH * (24-7) + numLen + eTagLen;
	
	int funid = WINKS_GETSHOWELEMENT;
	int funlen = totallen - WINKS_INT_LENGTH * 4;
	
	char *msgdata = NULL;
	char *tmpdata = NULL;	
	
	msgdata = malloc(totallen);
	if(msgdata)
	{
		memset(msgdata, 0, totallen);
		tmpdata = msgdata;
		memcpy(tmpdata, &processid, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		ret = totallen - 8;
		memcpy(tmpdata, &ret, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, &funid, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, &funlen, WINKS_INT_LENGTH);
		
		//第一参数
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		ret = 0;
		memcpy(tmpdata, &ret, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, &numLen, WINKS_INT_LENGTH);		
		if(numLen>0)
		{
			tmpdata = tmpdata + WINKS_INT_LENGTH;
			memcpy(tmpdata, number, numLen);
			tmpdata = tmpdata + numLen;
		}
		
		//第二参数		
		ret = 1;
		memcpy(tmpdata, &ret, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		ret = sizeof(unsigned long);
		memcpy(tmpdata, &ret, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, &extDataFlag, ret);			
		tmpdata = tmpdata + ret;
		
		//第三参数
		ret = 2;
		memcpy(tmpdata, &ret, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		
		memcpy(tmpdata, &eTagLen, WINKS_INT_LENGTH);		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		if(eTagLen)
		{
			memcpy(tmpdata, eTag, eTagLen);
			tmpdata = tmpdata + eTagLen;
		}		
		
		//第四参数		
		ret = 3;
		memcpy(tmpdata, &ret, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		if(eDataBufSize)
		{
			ret = sizeof(unsigned long);
		}
		else
		{
			ret = 0;			
		}
		
		memcpy(tmpdata, &ret, WINKS_INT_LENGTH);		
		if(eDataBufSize)
		{
			tmpdata = tmpdata + ret;
			memcpy(tmpdata, eDataBufSize, ret);			
		}
		else
		{
			tmpdata = tmpdata + WINKS_INT_LENGTH;
		}
		
		//第五参数		
		ret = 4;
		memcpy(tmpdata, &ret, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		ret = sizeof(unsigned long);
		memcpy(tmpdata, &ret, WINKS_INT_LENGTH);
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, &tIdBufSize, ret);		
		
		ret = send(client_sockfd,msgdata,totallen,0);
		
		if(msgdata)
		{
			free(msgdata);
			msgdata = NULL;
		}
		
		if (ret <= 0) 
		{
			Winks_printfErr("Winks_Write:error=%s  the ret=%d",strerror(errno),ret);
#ifndef WINKS_LONG_SOCKET	
			Winks_CloseSocket();
#endif
			return WINKS_RETURN_FAILURE;
		}		
	}
	else
	{
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}
	
	/* 等待获取返回值*/
	//读数据到缓冲
	if (Winks_Read_flush() < 0) {
		Winks_printfErr("ERROR: the server socket retrun error=%s",strerror(errno));
		Winks_Close_Read_flush();
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}
	
	//校验返回结果	
	int errcode = 0;
	Winks_Read(&errcode,WINKS_INT_LENGTH);
	if (errcode < 0) {
		Winks_printfErr("ERROR: the server socket retrun errcode=%d",errcode);
		Winks_Close_Read_flush();
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}
	
	//获取返回值总长度
	int reslen = 0;
	//Winks_Lseek(WINKS_INT_LENGTH,SEEK_CUR);
	Winks_Read(&reslen,WINKS_INT_LENGTH);
	//Winks_printf("Winks_UserGetCCShow:the result length=%d",reslen);
	
	//获取返回值
	int res = 0;
	Winks_Lseek(WINKS_INT_LENGTH*2,SEEK_CUR);
	Winks_Read(&res,WINKS_INT_LENGTH);
	if (res < 0) {
		Winks_printfErr("Winks_GetShowTemplate:return the result res=%d",res);
		Winks_Close_Read_flush();
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}	
	
	//获取fileflag 值
	Winks_Lseek(WINKS_INT_LENGTH*2,SEEK_CUR);
	Winks_Read(&Glob_fileFlag,WINKS_INT_LENGTH);
	*fileFlag = Glob_fileFlag;
	
	//获取eDatabuf值
	int tmplen = 0;	
	Winks_Lseek(WINKS_INT_LENGTH,SEEK_CUR);
	Winks_Read(&tmplen,WINKS_INT_LENGTH);
	if(tmplen>*eDataBufSize)
	{
		Glob_eDataBuf = malloc(tmplen);
		memset(Glob_eDataBuf,0,tmplen);
		Winks_Read(Glob_eDataBuf,tmplen);	
	}
	else
	{
		//eDataBufSize = tmplen;
		Winks_Read(eDataBuf,tmplen);
	}
	
	//获取eDataBufSize
	Winks_Lseek(WINKS_INT_LENGTH*2,SEEK_CUR);
	Winks_Read(&Glob_eDataBufLen,WINKS_INT_LENGTH);	
	if(Glob_eDataBufLen<=*eDataBufSize)
	{
		*eDataBufSize = Glob_eDataBufLen;
		Glob_eDataBufLen = 0;
	}
	else
	{
		*eDataBufSize = Glob_eDataBufLen;
	}	
	
	/*获取tIdBuf值*/
	Winks_Lseek(WINKS_INT_LENGTH,SEEK_CUR);
	Winks_Read(&tmplen,WINKS_INT_LENGTH);
	
	//说明edatabuf空间不够，获取的值需要保存
	if(tmplen>0)
	{
		Glob_tIdBuf = malloc(tmplen);
		if(Glob_tIdBuf)
		{
			memset(Glob_tIdBuf,0,tmplen);
			Winks_Read(Glob_tIdBuf,tmplen);
			Glob_tIdBufLen = tmplen;
		}
		
		if(Glob_eDataBufLen == 0)
		{			
			if(tmplen>tIdBufSize)
			{
				memcpy(tIdBuf,Glob_tIdBuf,tIdBufSize);
			}
			else
			{
				memcpy(tIdBuf,Glob_tIdBuf,tmplen);
			}
			
			if(Glob_tIdBuf)
			{
				free(Glob_tIdBuf);
				Glob_tIdBuf = NULL;
			}
		}		
	}
	
	Winks_Close_Read_flush();
#ifndef WINKS_LONG_SOCKET	
	Winks_CloseSocket();
#endif
	Winks_printf("out Winks_GetShowTemplate the res=%d",res);
	return res;	
#endif
	return 0;
}


////////////////////////////////////////////////////////////////////////////////////

/* 通过电话号码获取彩像展示模板。
 * function: Winks_GetShowTemplate
 * parameters:
 *   char* number (in)电话号码，不能为NULL。
 *          可通过输入上述宏（WINKS_DFT_CC_NO、WINKS_PRI_CC_NO、WINKS_BUS_CC_NO），达到特定的效果。
 *   char* fileName (out)返回模板存放的文件路径。
 *   unsigned long fileSize (in)fileName的size。
 *   int *allowCollectFlag (out)返回是否允许收藏的标志。
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_GetShowTemplate(char* number, char* fileName, unsigned long fileSize, int *allowCollectFlag)
{
	
#ifndef WINKS_LONG_SOCKET
	if (Winks_InitSocket() != WINKS_RETURN_SUCCESS)
		return WINKS_RETURN_FAILURE;
#endif
	int ret = 0;
	int processid = WINKS_PROCESSID;
	int numLen = strlen(number) + 1;
	if(numLen == 1)
	{
		numLen = 0;
	}
	int totallen = WINKS_INT_LENGTH * 6 + numLen;
	
	int funid = WINKS_GETSHOWTEMPLATE;
	int funlen = totallen - WINKS_INT_LENGTH * 4;
	
	char *msgdata = NULL;
	char *tmpdata = NULL;	
	
	msgdata = malloc(totallen);
	if(msgdata)
	{
		memset(msgdata, 0, totallen);
		tmpdata = msgdata;
		memcpy(tmpdata, &processid, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		ret = totallen - 8;
		memcpy(tmpdata, &ret, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, &funid, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, &funlen, WINKS_INT_LENGTH);
		
		//第一参数
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		ret = 0;
		memcpy(tmpdata, &ret, WINKS_INT_LENGTH);
		
		tmpdata = tmpdata + WINKS_INT_LENGTH;
		memcpy(tmpdata, &numLen, WINKS_INT_LENGTH);		
		if(numLen>0)
		{
			tmpdata = tmpdata + WINKS_INT_LENGTH;
			memcpy(tmpdata, number, numLen);			
		}
		
		ret = send(client_sockfd,msgdata,totallen,0);
		
		if(msgdata)
		{
			free(msgdata);
			msgdata = NULL;
		}
		
		if (ret <= 0) 
		{
			Winks_printfErr("Winks_Write:error=%s  the ret=%d",strerror(errno),ret);
#ifndef WINKS_LONG_SOCKET	
			Winks_CloseSocket();
#endif
			return WINKS_RETURN_FAILURE;
		}		
	}
	else
	{
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}
	
	/* 等待获取返回值*/
	//读数据到缓冲
	if (Winks_Read_flush() < 0) {
		Winks_printfErr("ERROR: the server socket retrun error=%s",strerror(errno));
		Winks_Close_Read_flush();
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}
	
	//校验返回结果	
	int errcode = 0;
	Winks_Read(&errcode,WINKS_INT_LENGTH);
	if (errcode < 0) {
		Winks_printfErr("ERROR: the server socket retrun errcode=%d",errcode);
		Winks_Close_Read_flush();
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}
	
	//获取返回值总长度
	int reslen = 0;
	//Winks_Lseek(WINKS_INT_LENGTH,SEEK_CUR);
	Winks_Read(&reslen,WINKS_INT_LENGTH);
	//Winks_printf("Winks_UserGetCCShow:the result length=%d",reslen);
	
	//获取返回值
	int res = 0;
	Winks_Lseek(WINKS_INT_LENGTH*2,SEEK_CUR);
	Winks_Read(&res,WINKS_INT_LENGTH);
	if (res < 0) {
		Winks_printfErr("Winks_GetShowTemplate:return the result res=%d",res);
		Winks_Close_Read_flush();
#ifndef WINKS_LONG_SOCKET	
		Winks_CloseSocket();
#endif
		return WINKS_RETURN_FAILURE;
	}	
	
	//获取xml file值
	int tmplen = 0;	
	Winks_Lseek(WINKS_INT_LENGTH,SEEK_CUR);
	Winks_Read(&tmplen,WINKS_INT_LENGTH);
	
	Winks_Read(fileName,tmplen);
	
	/*获取一键收藏值*/
	Winks_Lseek(WINKS_INT_LENGTH*2,SEEK_CUR);
	Winks_Read(&allowCollectFlag,WINKS_INT_LENGTH);	
	
	Winks_Close_Read_flush();
#ifndef WINKS_LONG_SOCKET	
	Winks_CloseSocket();
#endif
	Winks_printf("out Winks_GetShowTemplate the res=%d",res);
	return res;	
}

/*--------------------------------
 *
 *-------------------------------*/
Winks_CCDW_RgBase_s* winks_ccsw_GetRegion( char* PhoneNumber)
{
    FILE* pf = NULL;
    //char* pnum = NULL;
    char* pbuf = NULL;
    //char pfilename[WINKS_PATH_MAX + 1] = {"DISKOS1/projects/test/DrawTemplate/template.xml"};
    Winks_CCSW_Global_s* pGlobal = &gCCSW;
    int tval = 0, collect = 0;
	int option = WK_CCSW_TITLE;
    Winks_XmlMem_s XmlMem;
    Winks_Xml_s* pXml = NULL;
    Winks_CCDW_RgBase_s* pret = NULL;
	
	//NSString *templatefile = [[NSBundle mainBundle] pathForResource:@"template" ofType:@"xml"];
   // pnum = winks_ccsw_GetShowNum( option );
   
    //Winks_mem_set( pfilename, 0, WINKS_PATH_MAX + 1 );
    //tval = Winks_GetShowTemplate( PhoneNumber, pfilename, WINKS_PATH_MAX, &collect );
	
   // if( tval < 0 )
       // return NULL;
	
    if( (pf = fopen("/Users/luke/template.xml", "rb+" )) == NULL )
        return NULL;
	
    if( Winks_fGetFileSize( pf, (unsigned long* )(&tval) ) != 0 )
        return NULL;
	
    if( (pbuf = Winks_salAlloc( tval + 1 )) == NULL )
    {
        Winks_fclose( pf );
        return NULL;
    }
    
    memset( pbuf, 0, tval + 1 );
	
    if( (Winks_fread( pbuf, 1, tval, pf )) <= 0 )
    {
        Winks_salFree( pbuf );
        Winks_fclose( pf );
        return NULL;
    }
    
    Winks_fclose( pf );
	
    XmlMem.pnext = NULL;
    XmlMem.len = tval;
    XmlMem.offset = 0;
    XmlMem.pdata = pbuf;
	
    if( Winks_XML_Parse( &XmlMem, &pXml ) < 0 )
    {
        Winks_salFree( pbuf );
        return NULL;
    }
	
    pret = winks_ccsw_ParseXml( pXml );
	
    Winks_salFree( pbuf );
    Winks_XML_Free( pXml );
	
   pGlobal->Info.showtype = option;
	
#ifdef WINKS_COLLECT
    if( collect )
        pGlobal->flag |= WK_CCSW_LSKCOLLECT;
#endif	
	
    if(pret && winks_ccsw_CheckMediaEle(pret))
    {
        return pret;
    }
    else
    {
		//pGlobal->flag &= ~WK_CCSW_LSKCOLLECT;
        return NULL;
    }
}


