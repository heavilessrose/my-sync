#ifndef _WAVEHEAD_
#define _WAVEHEAD_
#define TWAVEHEADPCMLEN 44
#define TWAVEHEADGSMLEN 60
#define DWORD uint32_t
#define WORD uint16_t
//wav pcm�ļ�ͷ
typedef   struct   
 {   
  char         riff[4];         //="RIFF";   
  DWORD       size_8;           //   ��   ��Ƶ�ļ�   -   8   (   =��Ƶ���ݳ��ȣ��ýṹ�峤��   -   8)   
  char         wave[4];         //="WAVE";   
  char         fmt[4];         //="fmt   ";   
  DWORD       fmtSize;         //��һ���ṹ��Ĵ�С   ��   16; 
  
  //WAVEFORMAT WAVEFORMAT_struct;  //WAVEFORMATEX   
  //struct   WAVEFORMAT   defined   in   "mmreg.h"   
  WORD         wFormatTag; //WAVE_FORMAT_PCM   (   =   1);   
  WORD         wChannels; //ͨ����   =   1   
  DWORD       dwSamplesPerSec; //������   (8000   ��   16000)   
  DWORD       dwAvgBytesPerSec;         //ÿ���ֽ���   (   =   ������   ��   ����������   ��   8   (nSamplesPerSec   *   wBitsPerSample   /   8   ))   
  WORD         wBlockAlign; //ÿ�������ֽ���   (������������8   (wBitsPerSample   /   8))   
  WORD         wBitsPerSample; //����������((��������Ϊ8000ʱ��8,��������Ϊ16000ʱ��16))   
  
  char         data[4]; //="data";   
  DWORD       datasize; //�����ݳ���   
 }TWaveHeadPCM,   *PTWaveHeadPCM; 
//wav gsm�ļ�ͷ
typedef   struct   
 {   
	char         riff[4];         //="RIFF";   
	DWORD       size_8;           //   ��   ��Ƶ�ļ�   -   8   (   =��Ƶ���ݳ��ȣ��ýṹ�峤��   -   8)   
	char         wave[4];         //="WAVE";   
	char         fmt[4];         //="fmt   ";   
	DWORD       fmtSize;         //��һ���ṹ��Ĵ�С    
   
	WORD    wFormatTag;//WAVE_FORMAT_PCM   (   =   1); 
	WORD    nChannels;//ͨ����   =   1   
	DWORD   nSamplesPerSec;//������   (8000   ��   16000)  
	DWORD   nAvgBytesPerSec;//ÿ���ֽ���   (   =   ������   ��   ����������   ��   8   (nSamplesPerSec   *   wBitsPerSample   /   8   ))   
	WORD    nBlockAlign;//ÿ�������ֽ���   (������������8   (wBitsPerSample   /   8))
	WORD    wBitsPerSample; //����������((��������Ϊ8000ʱ��8,��������Ϊ16000ʱ��16))  
	WORD    cbSize;
	WORD    wSamplesPerBlock;

	char dwFact[4];        // "fact"
    DWORD dwFactSize;    // always 4: 0x00000004
    DWORD dwFactValue;   // number of samples contained in this file
	char         data[4]; //="data";   
	DWORD       datasize; //�����ݳ���   
 }TWaveHeadGSM,   *PTWaveHeadGSM; 
#endif