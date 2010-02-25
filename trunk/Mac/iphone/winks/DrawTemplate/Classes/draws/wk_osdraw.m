//
//  wk_osdraw.m
//  GraphTest
//
//  Created by luke on 10-1-15.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#include "wk_osdraw.h"
#import <UIKit/UIKit.h>

#pragma mark --------------- 颜色 ---------------

#ifdef __APPLE__
#import "TargetConditionals.h"
#endif

/* 适应IPhone或Mac */
#define TARGET_IPHONE (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)

#if TARGET_IPHONE
#define RGB_COLOR(r, g, b) [UIColor colorWithRed:r/255.0 green:g/255.0 blue:b/255.0 alpha:1.0]
#define RGBA_COLOR(r, g, b, a) [UIColor colorWithRed:r/255.0 green:g/255.0 blue:b/255.0 alpha:a/255.0]
#else
// for mac
#define RGB_COLOR(r, g, b) [NSColor colorWithCalibratedRed:r/255.0 green:g/255.0 blue:b/255.0 alpha:1.0]
#define RGBA_COLOR(r, g, b, a) [NSColor colorWithCalibratedRed:r/255.0 green:g/255.0 blue:b/255.0 alpha:a/255.0]
#endif

static CGContextRef _context;

#pragma mark -
#pragma mark 内部函数
// 角度转换为弧度
static inline double radians (double degrees) {return degrees * M_PI/180;}

static CGPoint _makePoint(int px, int py)
{
	return CGPointMake((CGFloat)px, (CGFloat)py);
}

// 根据左上右下坐标得到CGRect
static CGRect _makeRect(int pLefttopX, int pLefttopY, int pRightBottomX, int pRightBottomY)
{
	return CGRectMake((CGFloat)pLefttopX, (CGFloat)pLefttopY, (CGFloat)(pRightBottomX - pLefttopX), (CGFloat)(pRightBottomY - pLefttopY));
}

static void _drawLine(CGPoint pstartPoint, CGPoint pendPoint)
{
	CGContextMoveToPoint(_context, pstartPoint.x, pstartPoint.y);
	CGContextAddLineToPoint(_context, pendPoint.x, pendPoint.y);
	// 实际绘制
	CGContextStrokePath(_context);
	//CGContextDrawPath(_context, kCGPathFill);
}

static void _drawRect(CGRect prect, CGPathDrawingMode pmode, unsigned int pcolor)
{
	// 设置笔触大小
	CGContextSetLineWidth(_context, 1.0f);
	// 设置笔划颜色
	CGContextSetStrokeColorWithColor(_context, makeColor(pcolor));
	if (pmode == kCGPathFillStroke) {
		// 设置填充颜色
		CGContextSetFillColorWithColor(_context, makeColor(pcolor));
	}
	CGContextAddRect(_context, prect);
	CGContextDrawPath(_context, pmode);
}

// 清除上下文属性
static void _clearDraw()
{
	
}

// 绘制一个透明矩形
void _clear_rect(CGRect prect)
{
	CGContextClearRect(_context, prect);
}
#pragma mark -
#pragma mark 公开函数

CGContextRef init_drawContext()
{
	_context = UIGraphicsGetCurrentContext();
	//winks_ui_drawRect(_context, CGContext, <#int RightBottom_x#>, <#int RightBottom_y#>, <#unsigned int v_Color#>);
	return _context;
}

void set_drawContext(CGContextRef pcontext)
{
	_context = pcontext;
}

void fill_frame(CGRect pframe, unsigned int v_Color)
{
	_drawRect(pframe, kCGPathFillStroke, v_Color);
}

void draw_frame(CGRect pframe, unsigned int v_Color)
{
	_drawRect(pframe, kCGPathStroke, v_Color);
}

CGColorRef makeColor(unsigned int v_Color)
{
	CGColorRef color = RGBA_COLOR(((v_Color & 0xFF000000) >> 24), ((v_Color & 0xFF0000) >> 16), ((v_Color & 0xFF00) >> 8), (v_Color & 0xFF)).CGColor;
	return color;
}

void set_curColor(unsigned int pcolor)
{
	// 设置笔划颜色
	CGContextSetStrokeColorWithColor(_context, makeColor(pcolor));
	// 设置填充颜色
	CGContextSetFillColorWithColor(_context, makeColor(pcolor));
}

// 绘制一个透明矩形
//void clear_rect(int LeftTop_x,int LeftTop_y,int RightBottom_x, int RightBottom_y)
//{
//	CGContextClearRect(_context, _makeRect(LeftTop_x,LeftTop_y,RightBottom_x, RightBottom_y));
//}

void winks_ui_drawPoint(int v_CoorX, int v_CoorY,  unsigned int v_Color)
{
	// 设置笔触大小
	CGContextSetLineWidth(_context, 1.0f);
	// 设置笔划颜色
	CGContextSetStrokeColorWithColor(_context, makeColor(v_Color));
	// 设置填充颜色
	//CGContextSetFillColorWithColor(_context, RGBA_COLOR(0, 255, 0).CGColor);
	CGPoint startPoint = _makePoint(v_CoorX, v_CoorY);
	CGPoint endPoint = _makePoint(v_CoorX + 1, v_CoorY + 1);
	_drawLine(startPoint, endPoint);
}

void winks_ui_drawLine(int v_StartX, int v_StartY, int v_EndX,int v_EndY,  unsigned int v_Color)
{
	// 设置笔触大小
	CGContextSetLineWidth(_context, 1.0f);
	// 设置笔划颜色
	CGContextSetStrokeColorWithColor(_context, makeColor(v_Color));
	// 设置填充颜色
	//CGContextSetFillColorWithColor(_context, RGBA_COLOR(0, 255, 0).CGColor);
	CGPoint startPoint = _makePoint(v_StartX, v_StartY);
	CGPoint endPoint = _makePoint(v_EndX, v_EndY);
	_drawLine(startPoint, endPoint);
}

void winks_ui_drawRect(int LeftTop_x,int LeftTop_y,int RightBottom_x, int RightBottom_y, unsigned int v_Color )
{
	CGRect rect = _makeRect(LeftTop_x, LeftTop_y, RightBottom_x, RightBottom_y);
	_drawRect(rect, kCGPathStroke, v_Color);
}

void winks_ui_fillRect(int LeftTop_x,int LeftTop_y,int RightBottom_x, int RightBottom_y, unsigned int v_Color )
{
	CGRect rect = _makeRect(LeftTop_x, LeftTop_y, RightBottom_x, RightBottom_y);
	_drawRect(rect, kCGPathFillStroke, v_Color);
}


// 带边框的矩形
void winks_ui_drawDashRect(int LeftTop_x, int LeftTop_y, int RightBottom_x, int RightBottom_y, unsigned int v_Color, 
						   int line_height, int line_style)
{
	if(line_height <= 0)
        return;
	CGContextSaveGState(_context);
	set_curColor(v_Color);
	switch (line_style) {
			CGRect rect;
			// 实线矩形
		case 1:
			// 设置笔触大小
			CGContextSetLineWidth(_context, (CGFloat)line_height);
			rect = _makeRect(LeftTop_x, LeftTop_y, RightBottom_x, RightBottom_y);
			_drawRect(rect, kCGPathFillStroke, v_Color);
			break;
			// 虚线矩形
		case 2:
		case 3:
		{
			// 设置多组可实现多种效果
			CGFloat lenghts[] = {
				10.0f, // 要画的长度
				10.0f  // 跳过不画的长度
			};
			rect = _makeRect(LeftTop_x, LeftTop_y, RightBottom_x, RightBottom_y);
			// 设置笔触大小
			CGContextSetLineWidth(_context, (CGFloat)line_height);
			// 设置虚实宽度
			CGContextSetLineDash(_context, 0.0f, lenghts, 2);
			// 画虚线
			//CGContextMoveToPoint(_context, (CGFloat)LeftTop_x, (CGFloat)LeftTop_y);
			//CGContextAddLineToPoint(_context, (CGFloat)LeftTop_x, (CGFloat)LeftTop_y);
			// 画虚框
			CGContextAddRect(_context, rect);
			CGContextStrokePath(_context);
		}
			
			break;
			// 双线矩形
		case 4:
			// 设置笔触大小
			CGContextSetLineWidth(_context, 1.0f);
			rect = _makeRect(LeftTop_x, LeftTop_y, RightBottom_x, RightBottom_y);
			CGRect rect2 = _makeRect(LeftTop_x + line_height, LeftTop_y + line_height, RightBottom_x - line_height, RightBottom_y - line_height);
			_drawRect(rect, kCGPathStroke, v_Color);
			_drawRect(rect2, kCGPathStroke, v_Color);
			break;
		default:
			break;
	}
	CGContextRestoreGState(_context);
}

// 画图片
void winks_ui_drawGraphicFromFile (int v_Top, int v_Left, const char *file_name)
{
	NSString *filePath = [NSString stringWithCString:file_name encoding:NSASCIIStringEncoding];
	UIImage *uiimg = [UIImage imageNamed:filePath];
	
	
	
	CGImageRef img = [uiimg CGImage];
	size_t imgW = CGImageGetWidth(img);
	size_t imgH = CGImageGetHeight(img);
	/* // 镜像
	CGContextSaveGState(_context);
	// 翻转Y轴
	CGContextTranslateCTM(_context, imgW, imgH); //x轴+1, y轴-1
	//CGContextScaleCTM(_context, 2, 1);// 拉伸x轴为原来的两倍
	CGContextRotateCTM(_context, 180 * M_PI / 180); // 左右镜像
	//CGContextRotateCTM(_context, 180 * M_PI / 180); // 左右镜像
	 
	// CGRect rect = _makeRect(v_Top, v_Left, v_Top + imgW, v_Left + imgH);
	CGRect rect = _makeRect(v_Top, v_Left, v_Top, v_Left);
	CGContextDrawImage(_context, rect, img);
	CGContextRestoreGState(_context);
	 */
	
	[uiimg drawInRect:CGRectMake(v_Top, v_Left, imgW, imgH)];
}

// 画文字
void winks_ui_textOutlen(int x, int y, const char * text, int len)
{
	//CGContextSaveGState(_context);
	//CGContextSetFillColorWithColor(_context, [[UIColor whiteColor] CGColor]);
	
	NSString *nstext = [NSString stringWithCString:text encoding:NSUTF8StringEncoding];
	//[nstext drawAtPoint:CGPointMake(x, y) withFont:[UIFont fontWithName:@"Zapfino" size:20]];
	[nstext drawAtPoint:CGPointMake(x, y) withFont:[UIFont systemFontOfSize:[UIFont buttonFontSize]]];
//	[nstext drawInRect:nstext withFont:[UIFont systemFontOfSize:[UIFont buttonFontSize]] 
//		  lineBreakMode:UILineBreakModeClip alignment:UITextAlignmentCenter];
	/*
	 char *commentsMsg = (char *)text;
	 UIFont *commentsFont = [UIFont systemFontOfSize:7.0];
	 //commentsMsg = (char *)[text UTF8String];
	 NSString *nstext = [NSString stringWithCString:text encoding:NSUTF8StringEncoding];
	 CGSize textSize = [nstext sizeWithFont:commentsFont];
	 CGContextSelectFont(_context, (char *)[[commentsFont fontName] UTF8String], textSize.height, kCGEncodingMacRoman);
	 CGContextSetTextDrawingMode(_context, kCGTextFill);
	 
	 CGAffineTransform myTextTransform =CGAffineTransformScale(CGAffineTransformIdentity, 1.f, -1.f );
	 CGContextSetTextMatrix (_context, myTextTransform);
	 
	 CGContextShowTextAtPoint(_context, x, y, commentsMsg, strlen(commentsMsg));
	 */
	//UIGraphicsGetImageFromCurrentImageContext();
	
	//CGContextRestoreGState(_context);
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
 实现描述：  调用系统函数. 需要在使用前保存当前上下文状态在不需要clip时恢复上下文状态.
 \*************************************************************************************/
//int Winks_set_clip_on(int x,int y,int x1,int y1)
//{
//	CGContextBeginPath (_context);
//	CGContextAddRect(_context, _makeRect(x, y, x1, y1));
//	CGContextClosePath (_context);
//	CGContextClip (_context);
//	return 0;
//}
//
//
//int Winks_set_clip_off(int x,int y,int x1,int y1)
//{
//	return 0;
//}
//
