// PCM_GSM.cpp : Defines the entry point for the console application.
//

#include <stdlib.h>
#include <string.h>
#include <string>
#include "GSM.H"
#include "WaveHead.h"
#include "Converter.h"
using namespace std;
//extern "C" void conv65( unsigned char * c, unsigned char * d);
//extern "C" void conv66(unsigned char * d, unsigned char * c);

// The following code implements a GSM 6.10 to WAV#49 converter
//---------------------------------------------------------------------------
void GSM2WAV(unsigned char * d, unsigned char *c){
// converts 66 bytes (2 frames) of gsm to 65 bytes (one frame)
// of wav
        conv66(d, c);
}
//---------------------------------------------------------------------------
void WAV2GSM(unsigned char * d, unsigned char *c){
// converts 65 bytes (one frame) of wav to 66 bytes (two frames)
// of gsm
        conv65(d,c);
}
//---------------------------------------------------------------------------




// The following code lets to build a GSM6.10 to PCM 16 bit linear converter.
gsm r;
//---------------------------------------------------------------------------
void GSMcreate(void){    // creates the object gsm r
    r=gsm_create();
}
//---------------------------------------------------------------------------
void GSMencode(gsm_signal s[160],gsm_frame d){
        gsm_encode(r,s,d);// encodes 160 samples 16 bit linear to one GSM frame (33 bytes)
}
//---------------------------------------------------------------------------
void GSMdecode(gsm_frame s,gsm_signal d[160]){
        gsm_decode(r,s,d); // decodes one GSM frame (33 bytes) to 160 samples 16 bit linear
}
//---------------------------------------------------------------------------
void GSMdestroy(void){
    gsm_destroy(r);  // destroys the object r
}
//---------------------------------------------------------------------------
int Fread(const char *pInPut,unsigned char **ppOut,long * pLen)
{
	FILE *stream;	
	long   lSize,lCount; 
	unsigned char  *pBuff = NULL;
	//∂¡»°“Ù∆µŒƒº˛
	stream  = fopen(pInPut,"rb+");
	if(stream == NULL)
	{
		printf("False Open FILE\n");
		return -1;
	}
	
	fseek(stream,0l,SEEK_END);   
	lSize=ftell(stream);   
	lCount=lSize/sizeof(unsigned char);   
	pBuff=(unsigned char*)malloc(lSize);   
	rewind(stream);   
	lSize = fread(pBuff,sizeof(unsigned char),lCount,stream);
	if(lSize!=lCount)
	{		
		printf("Error Read File\n");
		free(pBuff);
		pBuff = NULL;
		return -2;
	}
	fclose(stream);
	*ppOut = pBuff;
	* pLen = lSize;
	return 0;
}
int PcmToGsm(const char *pInPut,const char *pOutName)
{
	unsigned char *pBuff1,*pBuff2,*pBuff3;
	unsigned char tempPcm[66];
	unsigned char tempGsm[65];
	TWaveHeadGSM strHeadGSM;
	long len,nNow;
	int ret = 0;
	int nCount = 0;
	int temp1,temp2;
	gsm_signal s[160];
	gsm_frame d;
	FILE *fsd;
	ret  = Fread(pInPut,&pBuff1,&len);
	if(ret < 0)
	{
		return -1;
	}
	nNow = len / 320+1;
	pBuff2 = (unsigned char *)malloc(nNow*33);
	pBuff3 = (unsigned char *)malloc((nNow/2+1)*65);
	memset(pBuff2,0,nNow*33);
	memset(pBuff3,0,(nNow/2+1)*65);
	memset(&strHeadGSM,0,sizeof(TWaveHeadGSM));
	nNow = 0;
	GSMcreate();
	while(nNow<len-320)
	{			
		memcpy(s,pBuff1+nNow,320);
		GSMencode(s,d);
		nNow += 320;
		memcpy(pBuff2+nCount*33,d,33);
		nCount++;
	}
	nCount = nCount/2;
	temp1 = 0;
	temp2 = 0;
	while(temp1<nCount*66)
	{
		memcpy(tempPcm,pBuff2+temp1,66);
		temp1 += 66;
		GSM2WAV(tempPcm, tempGsm);
		memcpy(pBuff3+temp2,tempGsm,65);
		temp2 += 65;		
	}
	GSMdestroy();
	
	strcpy(strHeadGSM.riff,"RIFF");
	strHeadGSM.size_8 = nCount*65+52;
	strcpy(strHeadGSM.wave,"WAVE");
	strcpy(strHeadGSM.fmt,"fmt ");
	strHeadGSM.fmtSize = 20;
	
	strHeadGSM.wFormatTag = 49;//WAVE_FORMAT_GSM610
	strHeadGSM.nChannels = 1;//Õ®µ¿ ˝   =   1   
	strHeadGSM.nSamplesPerSec = 8000;//≤…—˘¬  
	strHeadGSM.nAvgBytesPerSec = 1625;//√ø√Î◊÷Ω⁄ ˝   
	strHeadGSM.nBlockAlign = 65;//√ø≤…—˘µ„◊÷Ω⁄ ˝
	strHeadGSM.wBitsPerSample = 0; //¡øªØ±»Ãÿ ˝
	strHeadGSM.cbSize = 2; //2
	strHeadGSM.wSamplesPerBlock = 320; //320
 
	memcpy(strHeadGSM.dwFact,"fact",4);
	strHeadGSM.dwFactSize = 4;
	strcpy(strHeadGSM.data,"data");
	strHeadGSM.datasize = nCount*65;//   

	fsd  = fopen(pOutName,"wb+");
	fwrite(&strHeadGSM,1,sizeof(TWaveHeadGSM),fsd);
	fwrite(pBuff3,1,nCount*65,fsd);
	fclose(fsd);
	free(pBuff1);	
	free(pBuff2);
	free(pBuff3);
	return 0;
}
int GsmToPcm(const char *pInPut,const char *pOutName)
{
	unsigned char *pBuff1,*pBuff2;	
	unsigned char tempGsm[65];
	unsigned char tempPcm[66];
	TWaveHeadPCM strHeadPCM;
	
	long len,nNow;
	int ret = 0;
	int nCount = 0;
	int temp1;
	gsm_signal d[160];
	gsm_frame s;
	FILE *fsd;
	ret  = Fread(pInPut,&pBuff1,&len);
	if(ret < 0)
	{
		return -1;
	}
	nNow = (len-60)/65;
	
	pBuff2 = (unsigned char *)malloc(nNow*320*2+1);
	
	nCount = 0;
	temp1 = 60;	
	
	GSMcreate();
	while(temp1<len)
	{
		memcpy(tempGsm,pBuff1+temp1,65);
		temp1 += 65;
		WAV2GSM(tempGsm,tempPcm);

		memcpy(s,tempPcm,33);
		GSMdecode(s,d);
		memcpy(pBuff2+nCount,d,320);
		nCount+=320;

		memcpy(s,tempPcm+33,33);
		GSMdecode(s,d);
		memcpy(pBuff2+nCount,d,320);
		nCount+=320;
	}	
	GSMdestroy();
	
	strcpy(strHeadPCM.riff,"RIFF");
	strHeadPCM.size_8 = nNow*320*2+52;
	strcpy(strHeadPCM.wave,"WAVE");
	strcpy(strHeadPCM.fmt,"fmt ");
	strHeadPCM.fmtSize = 16;

	strHeadPCM.wFormatTag             =   1;//WAVE_FORMAT_PCM;   
	strHeadPCM.wChannels             =   1;   
	strHeadPCM.dwSamplesPerSec     =   8000;   
	strHeadPCM.dwAvgBytesPerSec   =   16000;   
	strHeadPCM.wBlockAlign         =   2;   
	strHeadPCM.wBitsPerSample     =   16;   
 

	strcpy(strHeadPCM.data,"data");
	strHeadPCM.datasize = nNow*320*2;//
	
	fsd  = fopen(pOutName,"wb+");
	fwrite(&strHeadPCM,1,sizeof(TWaveHeadPCM),fsd);
	fwrite(pBuff2,1,nNow*320*2,fsd);
	fclose(fsd);
	
	free(pBuff1);
	free(pBuff2);
	
	return 0;
}
#if 0
int main(int argc, char* argv[])
{	
	if (argc < 4)
	{
		printf("Error argument\n");
		return 0;
	}


	if (strcmp(argv[1] ,"gsm2qcp") == 0)
	{
		string tmp_name;
		tmp_name = "tmp_" + string(argv[2]);
		string str_command;

		rename(argv[2] ,tmp_name.c_str());
		GsmToPcm(tmp_name.c_str() , argv[2]);

		str_command = "./pvconv " + string(argv[2]);
		system(str_command.c_str());
		
		remove(tmp_name.c_str());
	}
	else if (strcmp(argv[1] ,"qcp2gsm") == 0)
	{
		string tmp_name;
		tmp_name = "tmp_" + string(argv[3]);
		string str_command;

		str_command = "./pvconv " + string(argv[2]);
		system(str_command.c_str());

		rename(argv[3] ,tmp_name.c_str());

		PcmToGsm(tmp_name.c_str() , argv[3]);

		remove(tmp_name.c_str());
	}
	else if (strcmp(argv[1] ,"amr2gsm") == 0)//add by liangbo 20090803 for amr
	{
		string str_name = argv[2];

		string::size_type nfind_wav = str_name.rfind(".amr");	//∏ƒ±‰¿©’π√˚
		if (nfind_wav != string::npos)
		{
			str_name.replace(nfind_wav,4,".wav");
		}

		string str_command = "./xconv " +str_name;
		system(str_command.c_str());

		string str_tmpname = "tmp_" + str_name;
		rename(str_name.c_str() , str_tmpname.c_str());
		PcmToGsm(str_tmpname.c_str() , argv[3]);

		remove(str_tmpname.c_str());
	}	
	else if (strcmp(argv[1] ,"gsm2amr") == 0 ) //add by liangbo 20090803 for amr
	{
		string str_name = argv[2];
		string str_tmpname = "tmp_" + str_name;		

		rename(str_name.c_str() , str_tmpname.c_str());
		GsmToPcm(str_tmpname.c_str() , str_name.c_str());

		remove(str_tmpname.c_str());

		string::size_type nfind_wav = str_name.rfind(".wav");	//∏ƒ±‰¿©’π√˚
		if (nfind_wav != string::npos)
		{
			str_name.replace(nfind_wav,4,".amr");
		}

		string str_command = "./xconv " +str_name;
		system(str_command.c_str());

	}
	//int ret = 0;
	//ret = PcmToGsm("tt.wav","test_gsm.wav");
	
	//ret = GsmToPcm("test_gsm.wav","tt.wav");
	
	return 0;
}
#endif