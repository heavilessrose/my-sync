/*
 *  wk_template_api.c
 *  DrawTemplate
 *
 *  Created by qingjun pan on 2/3/10.
 *  Copyright 2010 winksi. All rights reserved.
 *
 */

#include "wk_template_api.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
extern Winks_CCSW_Global_s gCCSW; 
#if 1
int Winks_Ascii2UnicodeString(const char *in_ascii, char *out_unicode, unsigned int size)
{
	unsigned char *src, *dest;
	
    if (in_ascii == NULL || out_unicode == NULL || size < 4)
        return -1;
	
    memset(out_unicode, 0, size);
    src = (unsigned char *)in_ascii;
    dest = (unsigned char *)out_unicode;
	
    while (*src != 0 && *src < 0x80)
    {
        if ((char *)dest > out_unicode + size - 4)
            break;
		
		//这里要区分big_endlian和little_endlian了
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
        dest[0] = 0x00;
        dest[1] = *src;
#else
		dest[0] = *src;
        dest[1] = 0x00;
#endif
        
		
        src++;
        dest += 2;
    }
    return ((char *)src - in_ascii);
}


int Winks_showtext_in_area(winks_scrolling_text_s * text)
{
	
	return 0;
}

void winks_image_stop_animation()
{
	
	
}

/*check the existance of media resource, exist? return 1 for yes, 0 for no*/
int winks_ccsw_CheckMediaEle(Winks_CCDW_RgBase_s* pBase)
{
	Winks_CCSW_Global_s* pGlobal = &gCCSW;
	Winks_CCDW_RgBase_s* pBaseEle = pBase;
	int ret = 1;
	
	while(pBaseEle)
	{
		if(pBaseEle->type == WK_CCDW_RGTYPE_MEDIA)
		{
			Winks_CCDW_Media_s* pMedia = (Winks_CCDW_Media_s* )pBaseEle;
			int dlen = WINKS_PATH_MAX, gflag = 0; 
			unsigned long fflag = 0;
			char* pdata = NULL;	
			
			if( pMedia->pdata == NULL )
			{
				
		        if( pMedia->base.flags & WK_CCDW_RGFLAG_SREF )
					gflag = WINKS_TRUE;
				else if( pMedia->base.flags & (WK_CCDW_RGFLAG_CREF | WK_CCDW_RGFLAG_V) )
					gflag = WINKS_FALSE;
				else
				{
					//Winks_fail( "Winks CCShow check media can not get file\r\n" );
					printf("Winks CCShow check media can not get file\r\n");
					return -1;
				}
				char* pShowNum = winks_ccsw_GetShowNum(pGlobal->Info.showtype);
				if( (Winks_GetShowElement(pShowNum , gflag, pMedia->base.pdata, 
										  &fflag, pMedia->pbuf, (unsigned long* )(&dlen), NULL, 0 ) < 0) )
				{
					if( dlen > WINKS_PATH_MAX )
					{
						pdata = Winks_salAlloc( dlen + 2 );
						Winks_mem_set( pdata, 0, dlen + 2 );
						if( Winks_GetShowElement( winks_ccsw_GetShowNum(pGlobal->Info.showtype), gflag, pMedia->base.pdata, 
												 &fflag, pdata, (unsigned long* )(&dlen), NULL, 0 ) < 0 )
						{
							Winks_salFree( pdata );
							ret = 0;
							break;
						}
						else
						{
							Winks_salFree( pdata );	
						}
					}
					else
					{
						ret = 0;
						break;
					}
				}  
			}
		}
		pBaseEle = pBaseEle->pnext;
	}
	
	//Winks_printfInfo("Winks CCShow check media element return %d showtype %d \r\n",ret,pGlobal->Info.showtype);
	return ret;     
}

int winks_ui_drawanimation_by_filenametype ( const char *file_name ,int string_type,int x, int y,int view_width, int view_height,unsigned long show_flag,WINKS_ANIM_HANDLE  *P_handle)
{
	
	return 0;
}

/* 获取通话的开始时间和持续时间 */
int Winks_GetCallTime(Winks_DateTime_s *start_time, Winks_DateTime_s *duration)
{
	
	return 0;
}

/*关闭一个文件*/
int Winks_fclose(FILE *stream)
{
	return fclose(stream);// close(stream);
}

int Winks_set_clip_off(int x,int y,int x1,int y1)
{
	return 0;
}

/*取文件大小，成功返回0*/
int Winks_fGetFileSize(FILE *stream, unsigned long *size)
{
	int iRet = -1;	
	long lOldPos = 0;
	
	if ((lOldPos = ftell(stream)) == -1)
		return iRet;
	
	if (fseek(stream, 0, SEEK_END) == 0){
		if ((*size = ftell(stream)) != -1)
			iRet = 0;
		fseek(stream, lOldPos, SEEK_SET);		
	}
	
	return iRet;
}

int Winks_sprintf(char * pStr, const char *fmt, ...)
{
	
	return 0;
}

void winks_ui_handle_scrolling_text(winks_scrolling_text_s *s)
{
	
}

int Winks_GetSysDateTime(Winks_DateTime_s *dateTime_p)
{
	
	return 0;
}

int Winks_Utf2UnicodeString(const char *in_utf8, char *out_unicode, unsigned int size)
{
	
	return 0;
}

int Winks_salFree( void* p )
{
    free(p);
    return 0;
}

int Winks_fread(void *data, size_t size, size_t count, FILE *stream)
{
    return fread(data, size, count, stream);
}

/*************************************************************************************\
 函数原型：  int Winks_set_clip_on(int x,int y,int x1,int y1)
 
 函数介绍：  设定剪裁区域的参数为区域范围
 
 参数说明：
 参数名称	参数类型	参数介绍	备注
 x           int         左上点的X坐标
 y           int         左上点的Y坐标
 x1          int         右下点的X坐标
 y1          int         右下点的Y坐标			
 函数返回值：int
 成功返回0,失败返回-1;
 实现描述：  调用系统函数
 
 \*************************************************************************************/
int Winks_set_clip_on(int x,int y,int x1,int y1)
{
	
	return 0;
}

/*打开指定的文件*/
FILE* Winks_fopen(const char *filename,const char *type)
{
	return fopen(filename, type);
}
void* Winks_salAlloc( int size )
{
    return malloc(size);
}

int Winks_mem_cpy( void* pd, void* ps, int len )
{
    return (int)memcpy( pd, ps, len );
}

int Winks_mem_set( void* pd, int data, int len )
{
    return (int)memset( pd, data, len );
}


int winks_ccsw_FindKeyRegion( Winks_CCDW_Global_s* pCCDW )
{
    Winks_CCDW_RgBase_s* pBase = NULL;    
	
    pBase = pCCDW->pRegion;
    while( pBase )
    {
        if( (pBase->type == WK_CCDW_RGTYPE_MEDIA) && (pBase->pdata) && (Winks_stricmp( pBase->pdata, "winks" ) == 0) )
        {
            pCCDW->pWinks = (Winks_CCDW_Media_s* )pBase;
        }
        if( (pBase->type == WK_CCDW_RGTYPE_TEXT) && (pBase->pdata) && (Winks_stricmp( pBase->pdata, "greeting" ) == 0) )
        {
            pCCDW->pGreet = (Winks_CCDW_Text_s* )pBase;
        }
		
        pBase = pBase->pnext;
    }
	
    return 0;
	
}


#endif


