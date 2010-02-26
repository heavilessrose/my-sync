//
//  DrawTemplateViewController.m
//  DrawTemplate
//
//  Created by luke on 10-1-28.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "DrawTemplateViewController.h"
#import "wk_engin.h"
#import "wk_template_api.h"

#import "draws/BgView.h"
#import "draws/GreetView.h"
#import "draws/GifView.h"
#import "draws/RectView.h"
#import "MyLog.h"

@implementation DrawTemplateViewController
Winks_CCSW_Global_s gCCSW;

// 展示区域偏移
#define SW_START_X 0
#define SW_START_Y 40

const int CCDW_DrawType[] = 
{
    WK_CCDW_RGTYPE_RECT,
    WK_CCDW_RGTYPE_TEXT,
    WK_CCDW_RGTYPE_MEDIA,
    WK_CCDW_RGTYPE_WINKS,
    //WK_CCDW_RGTYPE_BUTTON,
    -1
};


#if 0
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
	//获取彩像展示信息
	if (gCCSW.pCCDWglb) 
	{
		winks_ccsw_FreeXml( gCCSW.pCCDWglb->pRegion);
		free(gCCSW.pCCDWglb);
		gCCSW.pCCDWglb = NULL;
		//Winks_printf("WinksStrgFile_CCShow_s free");
	}
	
	Winks_CCDW_Global_s *pCCShow = (Winks_CCDW_Global_s*)malloc(sizeof(Winks_CCDW_Global_s));
	if(pCCShow == NULL)
	{			
		//return -1;			
	}		
	
	//Winks_printf("Winks_CCDW_Global_s alloc");
	gCCSW.pCCDWglb = pCCShow;
	pCCShow->pRegion = (Winks_CCDW_RgBase_s*)winks_ccsw_GetRegion(WINKS_DFT_CC_NO );
	if ((gCCSW.pCCDWglb->pRegion ) == NULL) 
	{				
		free(pCCShow);
		//Winks_printf("WinksStrgFile_CCShow_s free");
		gCCSW.pCCDWglb = NULL;
		
		//return -1;
	}
	
	winks_ccsw_FindKeyRegion( pCCShow );
	
}
#endif


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	
	// 获取彩像展示信息
	[self parseTemplate];
	// 绘制模板
	[self ccsw_draw:gCCSW.pCCDWglb];
}


- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


- (void)dealloc {
	winks_ccsw_FreeXml( gCCSW.pCCDWglb->pRegion );
	free(gCCSW.pCCDWglb);
    [super dealloc];
}

#pragma mark show

// 解析模板得到展示信息
- (int)parseTemplate
{
	Winks_CCDW_Global_s *pCCDW = (Winks_CCDW_Global_s*)malloc( sizeof(Winks_CCDW_Global_s) );
	assert(pCCDW != NULL && "malloc error");
    Winks_mem_set( pCCDW, 0, sizeof(Winks_CCDW_Global_s) );
	
	//获取彩像展示信息
	pCCDW->pRegion = (Winks_CCDW_RgBase_s*)winks_ccsw_GetRegion(WINKS_DFT_CC_NO);
	
	if (pCCDW->pRegion == NULL) 
	{
		free(pCCDW);
		Winks_printfErr("WK CCSW get template error\r\n");
		return -1;
	}
	
	gCCSW.pCCDWglb = pCCDW;
	winks_ccsw_FindKeyRegion( pCCDW );
	return 0;
}

// 依次绘制Winks_CCDW_RgBase_s链表
- (int)ccsw_draw:(Winks_CCDW_Global_s *)pCCDW
{
	Winks_CCDW_RgBase_s* pBase = pCCDW->pRegion;
	
	while (pBase) {
		[self ccdw_draw:pBase];
        pBase = pBase->pnext;
	}
	
	/*
	CGRect showRect = CGRectMake(0, 30, 320, 270);
	
	// 背景色
	BgView *back = [[BgView alloc] initWithFrame:showRect];
	[self.view addSubview:back];
	[back release];
	// 背景图
	UIImageView *imgview = [[UIImageView alloc] initWithFrame:showRect];
	imgview.image = [UIImage imageNamed:@"andr2.png"];
	[self.view addSubview:imgview];
	[imgview release];
	// 彩像gif 或 png
	GifView *gifview = [[GifView alloc] initWithFrame:showRect];
	[gifview retain];
	[self.view.layer addSublayer:gifview.plView.layer];
	NSLog(@"gifview = %u", [gifview retainCount]);
	NSLog(@"plView = %u", [gifview.plView retainCount]);
	//[self.view addSubview:gifview];
	[gifview release];
	// 问候语
	GreetView *greetview = [[GreetView alloc] initWithFrame:showRect];
	[self.view addSubview:greetview];
	[greetview release];
	*/
	return 0;
}

// 确定pBase类型调用相应的绘制方法
- (int)ccdw_draw:(Winks_CCDW_RgBase_s *)pBase
{
	int i = 0;
	
	// 跳过page和canvas
	while(CCDW_DrawType[i] > 0)
	{
		if(WK_CCDW_RGTYPE_RECT == pBase->type)
		{
			[self ccdw_drawRect:pBase];
			return 0;
		} 
		if (WK_CCDW_RGTYPE_TEXT == pBase->type)
		{
			[self ccdw_drawText:pBase];
			return 0;
		} 
		if (WK_CCDW_RGTYPE_MEDIA == pBase->type) 
		{
			[self ccdw_drawMedia:pBase];
			return 0;
		}
		if (WK_CCDW_RGTYPE_WINKS == pBase->type) 
		{
			[self ccdw_drawMedia:pBase];
			return 0;
		}
		i++;
	}
	return -1;
}

- (int)ccdw_drawRect:(Winks_CCDW_RgBase_s *)pSection
{
	pSection->Section.y += SW_START_Y;
	RectView *rectview = [[RectView alloc] initWithSection:(Winks_CCDW_Rect_s *)pSection];
	[self.view addSubview:rectview];
	[rectview release];
	return 0;
}

- (int)ccdw_drawText:(Winks_CCDW_RgBase_s *)pSection
{
	pSection->Section.y += SW_START_Y;
	GreetView *greetview = [[GreetView alloc] initWithSection:(Winks_CCDW_Text_s *)pSection];
	[self.view addSubview:greetview];
	[greetview release];
	return 0;
}

// 静态图及gif
- (int)ccdw_drawMedia:(Winks_CCDW_RgBase_s *)pSection
{
	pSection->Section.y += SW_START_Y;
	GifView *gifview = [[GifView alloc] initWithSection:(Winks_CCDW_Media_s *)pSection];
	//[gifview retain];
	[self.view addSubview:gifview];
	//[self.view.layer addSublayer:gifview.plView.layer];
	[gifview release];
	return 0;
}

@end
