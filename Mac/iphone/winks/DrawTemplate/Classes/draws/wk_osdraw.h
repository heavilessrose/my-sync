//
//  wk_osdraw.h
//  GraphTest
//
//  Created by luke on 10-1-15.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#ifndef __WK_OSDRAW_H__
#define __WK_OSDRAW_H__

#include <QuartzCore/QuartzCore.h>
#include <CoreGraphics/CoreGraphics.h>
//#import <UIKit/UIKit.h>
//
//#pragma mark --------------- 颜色 ---------------
//
//#ifdef __APPLE__
//#import "TargetConditionals.h"
//#endif
//
///* 适应IPhone或Mac */
//#define TARGET_IPHONE (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
//
//#if TARGET_IPHONE
//#define RGB_COLOR(r, g, b) [UIColor colorWithRed:r/255.0 green:g/255.0 blue:b/255.0 alpha:1.0]
//#define RGBA_COLOR(r, g, b, a) [UIColor colorWithRed:r/255.0 green:g/255.0 blue:b/255.0 alpha:a/255.0]
//#else
//// for mac
//#define RGB_COLOR(r, g, b) [NSColor colorWithCalibratedRed:r/255.0 green:g/255.0 blue:b/255.0 alpha:1.0]
//#define RGBA_COLOR(r, g, b, a) [NSColor colorWithCalibratedRed:r/255.0 green:g/255.0 blue:b/255.0 alpha:a/255.0]
//#endif


#pragma mark --------------- 方法 ---------------
//CGContextRef winks_prepareContext();
//void winks_drawImg(CGContextRef pcontext, UIImage *pimg, CGRect prect);
//void winks_drawEllipse(CGContextRef pcontext, CGRect prect);
//void winks_drawRect(CGContextRef pcontext, CGRect prect);
//void winks_drawLine(CGContextRef pcontext, CGRect prect);
//void winks_drawStr(NSString *pstr, CGRect prect, UIFont *pfont, UILineBreakMode pmode, UITextAlignment palign);
//
//void debugFrame(CGRect frame);



CGContextRef init_drawContext();
void fill_frame(CGRect pframe, unsigned int v_Color);
void draw_frame(CGRect pframe, unsigned int v_Color);
CGColorRef makeColor(unsigned int v_Color);
void set_drawContext(CGContextRef pcontext);
void winks_ui_drawPoint(int v_CoorX, int v_CoorY,  unsigned int v_Color);
void winks_ui_drawLine(int v_StartX, int v_StartY, int v_EndX,int v_EndY,  unsigned int v_Color);
void winks_ui_drawRect(int LeftTop_x,int LeftTop_y,int RightBottom_x, int RightBottom_y, unsigned int v_Color );
void winks_ui_fillRect(int LeftTop_x,int LeftTop_y,int RightBottom_x, int RightBottom_y, unsigned int v_Color );
void winks_ui_drawDashRect(int LeftTop_x,int LeftTop_y,int RightBottom_x, int RightBottom_y, unsigned int v_Color ,
						   int line_height,int line_style);
void set_curColor(unsigned int pcolor);

//void winks_ui_handle_scrolling_text(winks_scrolling_text_s *s);

void clear_rect(int LeftTop_x,int LeftTop_y,int RightBottom_x, int RightBottom_y);
void _clear_rect(CGRect prect);
// 画图片
void winks_ui_drawGraphicFromFile (int v_Top, int v_Left, const char *file_name);
// 画文字
void winks_ui_textOutlen(int x, int y, const char * text, int len);


#endif
