#ifndef _WAVEHEAD_
#define _WAVEHEAD_
#define TWAVEHEADPCMLEN 44
#define TWAVEHEADGSMLEN 60
#define DWORD uint32_t
#define WORD uint16_t
//wav pcm文件头
typedef   struct   
 {   
  char         riff[4];         //="RIFF";   
  DWORD       size_8;           //   ＝   音频文件   -   8   (   =音频数据长度＋该结构体长度   -   8)   
  char         wave[4];         //="WAVE";   
  char         fmt[4];         //="fmt   ";   
  DWORD       fmtSize;         //下一个结构体的大小   ＝   16; 
  
  //WAVEFORMAT WAVEFORMAT_struct;  //WAVEFORMATEX   
  //struct   WAVEFORMAT   defined   in   "mmreg.h"   
  WORD         wFormatTag; //WAVE_FORMAT_PCM   (   =   1);   
  WORD         wChannels; //通道数   =   1   
  DWORD       dwSamplesPerSec; //采样率   (8000   或   16000)   
  DWORD       dwAvgBytesPerSec;         //每秒字节数   (   =   采样率   ×   量化比特数   ÷   8   (nSamplesPerSec   *   wBitsPerSample   /   8   ))   
  WORD         wBlockAlign; //每采样点字节数   (量化比特数÷8   (wBitsPerSample   /   8))   
  WORD         wBitsPerSample; //量化比特数((当采样率为8000时＝8,当采样率为16000时＝16))   
  
  char         data[4]; //="data";   
  DWORD       datasize; //纯数据长度   
 }TWaveHeadPCM,   *PTWaveHeadPCM; 
//wav gsm文件头
typedef   struct   
 {   
	char         riff[4];         //="RIFF";   
	DWORD       size_8;           //   ＝   音频文件   -   8   (   =音频数据长度＋该结构体长度   -   8)   
	char         wave[4];         //="WAVE";   
	char         fmt[4];         //="fmt   ";   
	DWORD       fmtSize;         //下一个结构体的大小    
   
	WORD    wFormatTag;//WAVE_FORMAT_PCM   (   =   1); 
	WORD    nChannels;//通道数   =   1   
	DWORD   nSamplesPerSec;//采样率   (8000   或   16000)  
	DWORD   nAvgBytesPerSec;//每秒字节数   (   =   采样率   ×   量化比特数   ÷   8   (nSamplesPerSec   *   wBitsPerSample   /   8   ))   
	WORD    nBlockAlign;//每采样点字节数   (量化比特数÷8   (wBitsPerSample   /   8))
	WORD    wBitsPerSample; //量化比特数((当采样率为8000时＝8,当采样率为16000时＝16))  
	WORD    cbSize;
	WORD    wSamplesPerBlock;

	char dwFact[4];        // "fact"
    DWORD dwFactSize;    // always 4: 0x00000004
    DWORD dwFactValue;   // number of samples contained in this file
	char         data[4]; //="data";   
	DWORD       datasize; //纯数据长度   
 }TWaveHeadGSM,   *PTWaveHeadGSM; 
#endif