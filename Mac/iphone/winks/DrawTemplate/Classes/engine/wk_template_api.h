/*
 *  wk_template_api.h
 *  DrawTemplate
 *
 *  Created by qingjun pan on 2/3/10.
 *  Copyright 2010 winksi. All rights reserved.
 *
 */
#ifndef _WK_TEMPLATE_API_H_
#define _WK_TEMPLATE_API_H_
#include <stdio.h>
#include "wk_iphoneccshw.h"


int Winks_Ascii2UnicodeString(const char *in_ascii, char *out_unicode, unsigned int size);
int Winks_showtext_in_area(winks_scrolling_text_s * text);
void winks_image_stop_animation();
int winks_ccsw_CheckMediaEle(Winks_CCDW_RgBase_s* pBase);
int winks_ui_drawanimation_by_filenametype ( const char *file_name ,int string_type,int x, int y,int view_width, int view_height,unsigned long show_flag,WINKS_ANIM_HANDLE  *P_handle);
/* 获取通话的开始时间和持续时间 */
int Winks_GetCallTime(Winks_DateTime_s *start_time, Winks_DateTime_s *duration);
/*关闭一个文件*/
int Winks_fclose(FILE *stream);
int Winks_set_clip_off(int x,int y,int x1,int y1);


#define WINKS_CLIP_OFF(x,y,x1,y1)     Winks_set_clip_off(x,y,x1,y1)
/*取文件大小，成功返回0*/
int Winks_fGetFileSize(FILE *stream, unsigned long *size);
int Winks_sprintf(char * pStr, const char *fmt, ...);
void winks_ui_handle_scrolling_text(winks_scrolling_text_s *s);
int Winks_GetSysDateTime(Winks_DateTime_s *dateTime_p);
int Winks_Utf2UnicodeString(const char *in_utf8, char *out_unicode, unsigned int size);
int Winks_salFree( void* p );
int Winks_fread(void *data, size_t size, size_t count, FILE *stream);
/*打开指定的文件*/
FILE* Winks_fopen(const char *filename,const char *type);
int Winks_set_clip_on(int x,int y,int x1,int y1);
#define WINKS_CLIP_ON(x,y,x1,y1)  Winks_set_clip_on(x,y, x1, y1)

void* Winks_salAlloc( int size );
int Winks_mem_cpy( void* pd, void* ps, int len );
int Winks_mem_set( void* pd, int data, int len );
int winks_ccsw_FindKeyRegion( Winks_CCDW_Global_s* pCCDW );
#endif
