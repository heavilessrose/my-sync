

/******************************************************************************
*              Copyright (C) 2008, Winks Technologies, Inc.
*                            All Rights Reserved
******************************************************************************
* Filename:   wk_ui_draw.c
*
* Description:
*     Winks ui call 's ui draw func.
*     @Jackiechen 2008-04-11.
******************************************************************************/
#ifndef MTK_CC_VERSION
/**************************************************************
 * Include Files
**************************************************************/
#include "wk_public.h"
#include "wk_osal.h"
#include "wk_engin.h"
#include "Wk_ui.h"
//extern char * Winks_UIFM_GetImage(int id);
//extern char * Winks_UIFM_GetString(int id);
extern void winks_ui_destroy_timer(WINKS_TIMERID timer_id)
{
	int i;
	WINKS_UI_CONTEXT_DEF()
		WINKS_UI_CONTEXT_INIT()
		
		for (i = 0;i < WINKS_MAX_TIMER_NUMBER; i++)
		{
			if(Winks_UIContext->winks_ui_time_a[i].a_timer_id == timer_id)
			{
				Winks_DestroyTimer(Winks_UIContext->winks_ui_time_a[i].a_uitimer);
				Winks_UIContext->winks_ui_time_a[i].a_uitimer =  0;//ks_CreateTimer(winks_UI_Handle, WINKS_SECONDS2TICKS, WINKS_TIMER_DEFAULT);
				Winks_UIContext->winks_ui_time_a[i].Winks_TimeProc = NULL;
				break;
			}
		}
		
}

void winks_ui_start_timer(WINKS_TIMERID timer_id, WINKS_TICKS timeOutPeriod, unsigned long flag,WinksEventProc TimeCallBackFunc)
{
	int i;
	WINKS_TIMER_ID v_timeid;
	WINKS_UI_CONTEXT_DEF()
		WINKS_UI_CONTEXT_INIT()
		
		for (i = 1;i < WINKS_MAX_TIMER_NUMBER; i++)
		{
			if(Winks_UIContext->winks_ui_time_a[i].a_timer_id == timer_id)
			{
				winks_ui_destroy_timer(timer_id);
				Winks_UIContext->winks_ui_time_a[i].a_timer_id = 0;
				Winks_UIContext->winks_ui_time_a[i].a_uitimer =  0;//ks_CreateTimer(winks_UI_Handle, WINKS_SECONDS2TICKS, WINKS_TIMER_DEFAULT);
				Winks_UIContext->winks_ui_time_a[i].Winks_TimeProc = (WinksEventProc)NULL;
			}
		}
		
		v_timeid = Winks_CreateTimer(Winks_UIContext->winks_ui_chn, timeOutPeriod, flag);
		for (i = 1;i < WINKS_MAX_TIMER_NUMBER; i++)
		{
			if(Winks_UIContext->winks_ui_time_a[i].a_uitimer == 0)
			{
				Winks_UIContext->winks_ui_time_a[i].a_timer_id = timer_id;
				Winks_UIContext->winks_ui_time_a[i].a_uitimer =  v_timeid;//ks_CreateTimer(winks_UI_Handle, WINKS_SECONDS2TICKS, WINKS_TIMER_DEFAULT);
				Winks_UIContext->winks_ui_time_a[i].Winks_TimeProc = (WinksEventProc)TimeCallBackFunc;
				break;
			}
		}
		Winks_StartTimer(v_timeid);
}

#define WINKS_SEPARATOR_FLAG 0X00000001
#define WINKS_BLANK_FLAG 0X00000010
#define WINKS_STRCAT_SIGN L"..."
extern void Winks_UI_Goto_Next_Item(winks_List_Screen_s *m);
extern void Winks_UI_Goto_Previous_Item(winks_List_Screen_s *m);
extern void Winks_UI_Goto_Next_Page(winks_List_Screen_s *m);
extern void Winks_UI_Goto_Previous_Page(winks_List_Screen_s *m);   
static void wk_gui_scrollbar_draw_button( winks_icon_button_s *b);  
void winks_ui_CreateRightSoftKey(int stringid); 
void winks_ui_createtitle(int stringid);
static void winks_ui_drawtitle(winks_Title_s *p_Title);
extern void winks_ui_start_timer(WINKS_TIMERID timer_id, WINKS_TICKS timeOutPeriod, unsigned long flag, WinksEventProc TimeCallBackFunc);
extern void winks_ui_destroy_timer(WINKS_TIMERID timer_id);
void winks_ui_redrawlistmenu(winks_List_Screen_s *p_List, int oldicon_index, int newicon_index);
static int winks_get_text_string_oneword_lenth(char* text);
extern int winks_show_string_in_rect(winks_text_String_rect_s * rect_text_String, winks_Rect_s* pRect, unsigned long TextAlign);
static int winks_get_text_string_oneword_width(char* text, int lenth);
static int winks_get_char_byte_number(void);
static void winks_ui_handle_scrolling_text(void);
extern void winks_ui_handle_scrolling_text_colorcall(void);
//extern winks_scrolling_text_s  winks_scroll_string;
static int winks_if_is_separator(char * string);
static int creat_text_String_line_type(char* text,int line_width,int* string_lenth);
int Winks_showtext_in_area (char* string_text,int string_text_lenth,int x, int y, int width,int height );
void  winks_ui_drawcolorcall_part(int x , int y, Winks_Colorcall_part_s *p_colorcall_part);
#if 0
winks_Softkey_s  winks_left_softkey;
winks_Softkey_s  winks_right_softkey;
extern int scrooltext_flag = 0;
winks_Title_s g_Title;

unsigned int  winks_hdc ;  //winks_hdc wk_ui.h call.
unsigned int  winks_hdc_temp, winks_current_hdc;    //arena will use it
#endif
//extern WINKS_CHN_ID winks_ui_chn;
//extern winks_List_Screen_s* gv_ListScreen;

//extern Winks_ApnScreen_s * winks_apn_screen;
//extern char gv_OneTimeFlag;
//extern winks_POPMenuListScreen_s *gv_PopmenuScreen;
//Winks_CcImageType_e Winks_Get_ImageType_By_String(char* v_FileStr);

//extern int anim_flag ;  //wk_draw.c will call

#if 1
#ifndef WINKS_UNSUPPORTED_GLOBAL_VAR
 Winks_UI_GlobalVariable_s  v_Winks_UIContext;
#endif


#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
#define WINKS_UI_CONTEXT_DEF()  \
    Winks_UI_GlobalVariable_s  *Winks_UIContext = NULL;
#define WINKS_UI_CONTEXT_INIT() \
    Winks_UIContext = (Winks_UI_GlobalVariable_s *)get_module_glob(WK_MODULE_UI); 

#else
#define WINKS_UI_CONTEXT_DEF()  \
    Winks_UI_GlobalVariable_s *Winks_UIContext = NULL; 
#define WINKS_UI_CONTEXT_INIT() \
    Winks_UIContext = &v_Winks_UIContext; 
#endif
#endif
#if 0
/* 彩像图片类型 */
typedef enum {
    E_IMAGE_JPG = 1,
		E_IMAGE_PNG,
		E_IMAGE_TIFF,
		E_IMAGE_SPL,
		E_IMAGE_SWF,
		E_IMAGE_GIF,
		
		E_IMAGE_PPT
} Winks_CcImageType_e;
//extern winks_text_String_rect_s  text_String_rect;

static Winks_CcImageType_e Winks_Get_ImageType_By_String(char* v_FileStr)
{
	char *v_ptr ;
    int   Len;
    char  ExtName[16];
	
	v_ptr = v_FileStr + winks_strlen_ucs2_type((char*)v_FileStr) - 1;
	
	while(*v_ptr != '.')
	{
		v_ptr--;
	}
	
    //Len = Winks_Unicode2AsciiString(v_ptr, ExtName);
    Len = Winks_Unicode2AsciiString(v_ptr, ExtName, 16);
    if(Len >= sizeof(ExtName))
        return E_IMAGE_JPG;
	
    v_ptr = ExtName;
	
	if (stricmp(v_ptr, ".jpeg") ==0 || stricmp(v_ptr, ".jpg") ==0 )
	{
		return   E_IMAGE_JPG;
	}
	else if (stricmp(v_ptr, ".png") ==0 )
	{
		return   E_IMAGE_PNG;
	}
	else if (stricmp(v_ptr, ".gif") ==0)
	{
		return   E_IMAGE_GIF;
	}
	else if (stricmp((char*)v_ptr, ".tiff") ==0)
	{
		return   E_IMAGE_TIFF;
	}
	else if (stricmp((char*)v_ptr, ".spl") ==0)
	{
		return   E_IMAGE_SPL;
	}
	else if (stricmp((char*)v_ptr, ".swf") ==0)
	{
		return   E_IMAGE_SWF;
	}
	else if (stricmp((char*)v_ptr, ".ppt") ==0)
	{
		return   E_IMAGE_PPT;
	}
	return 0;
}
#endif


#if 0
/**************************************************************

	FUNCTION NAME		: winks_ui_drawIconButton

  	PURPOSE		        : draw a Icon Button.

	INPUT PARAMETERS	: winks_Rect_s vp_Rect
    					  char v_State
    					  int v_IconID
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void winks_ui_drawIconButton(winks_icon_button_s *b)
{	
    /*winks_gui_create_icon_button(button *b, int v_Rect ->x, int v_Rect ->y, 
    int v_Rect ->width, int v_Rect ->height ,PU8 button_icon);
    winks_gui_show_button(button *b);*/
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int x1, y1, x2, y2;
    winks_color f;
    int icon = 0;
    //    int button_icon;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    x1 = b->x;
    y1 = b->y;
    x2 = x1 + b->width - 1;
    y2 = y1 + b->height - 1; 
   
    /*if (b->flags & UI_BUTTON_STATE_FOCUSSED)  
    {
        if (b->flags & UI_BUTTON_STATE_DOWN)
        {
            f = b->focussed_down_color;
            button_icon = b->focussed_down_icon;
        }
        else
        {
            f = b->focussed_up_color;
            button_icon = b->focussed_up_icon;
        }
    }
    else if (b->flags & UI_BUTTON_STATE_DISABLED)  
    {
        if (b->flags & UI_BUTTON_STATE_DOWN)
        {
            f = b->disabled_down_filler;
            button_icon = b->disabled_down_icon;
        }
        else
        {
            f = b->disabled_up_filler;
            button_icon = b->disabled_up_icon;
        }
    }
    else
    {
        if (b->flags & UI_BUTTON_STATE_DOWN)   
        {
            f = b->normal_down_filler;
            button_icon = b->normal_down_icon;
        }
        else
        {
            f = b->normal_up_filler;
            button_icon = b->normal_up_icon;
        }
    }*/
    if (b->flags == WINKS_UI_BUTTON_STATE_DOWN)
    {
    	f = b->focussed_down_color;
    }
    else
    {
    	f = b->normal_color;
    }
    if (b->flags == WINKS_UI_BUTTON_STATE_DOWN)
    {
    	icon = b->normal_down_icon;
        b->flags = WINKS_UI_BUTTON_STATE_UP;
    }
    else
    {
    	icon = b->normal_up_icon;
    }
    /* fill the buton with color */
    // winks_ui_fillRect(x1, y1, x2, y2, f);
    if(icon != 0)
    {
        winks_ui_drawGraphicFromFile(b->icon_x, b->icon_y,Winks_UIFM_GetImage(icon));// Winks_UIFM_GetImage(b->normal_up_icon)gui_show_transparent_image(x1 + b->icon_x, y1 + b->icon_y, button_icon, b->icon_transparent_color);
    }



}



/**************************************************************

	FUNCTION NAME		: winks_ui_drawRadio

  	PURPOSE		        : draw a Radio.

	INPUT PARAMETERS	: winks_Rect_s vp_Rect
    					  char v_State
    					  int v_IconID
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

/*void  winks_ui_drawRadio(winks_Rect_s vp_Rect, char v_State, int v_IconID)
{	
    //winks_ui_drawGraphicFromFile(vp_Rect.v_Top,vp_Rect.v_Left,GetImage(v_IconID));
} */



/**************************************************************

	FUNCTION NAME		: winks_ui_drawCheckbox

  	PURPOSE		        : draw a Checkbox.

	INPUT PARAMETERS	: winks_Rect_s vp_Rect
    					  char v_State
    					  int v_IconID
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

/*void  winks_ui_drawCheckbox (winks_Rect_s vp_Rect, char v_State, int v_IconID)
{	
	//winks_ui_drawGraphicFromFile(vp_Rect.v_Top,vp_Rect.v_Left,GetImage(v_IconID));
}*/
#if 0

/**************************************************************

	FUNCTION NAME		: winks_ui_create_vertical_scrollbar

  	PURPOSE		        : create a VerticalScrollBar.

	INPUT PARAMETERS	: winks_vertical_scrollbar_s * v
     					  int value
     					  int range
     					  int scale
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/
void winks_ui_create_vertical_scrollbar(winks_vertical_scrollbar_s *v, int value, int range, int scale)
{

    //v->x = WINKS_UPSCROLLBAR_X;
    //v->y = WINKS_UPSCROLLBAR_Y;
    //v->width = WINKS_SLIDESCROLLBAR_WIDTH;//WINKS_SCREEN_WIDTH-WINKS_UPSCROLLBAR_X;
    //v->height = WINKS_DOWNSCROLLBAR_Y -WINKS_UPSCROLLBAR_Y + WINKS_SLIDESCROLLBAR_Y-WINKS_UPSCROLLBAR_Y;
    v->value = value - 1;
    v->range = range;
    v->scale = scale;
    v->flags = 0;
    v->background_color = WINKS_PIXEL_CYAN;
    v->v_scroll_bg = WINKS_PIXEL_GREEN;
    v->up_button.focussed_down_color = WINKS_PIXEL_RED;
    v->up_button.x = WINKS_UPSCROLLBAR_X;
    v->up_button.y = WINKS_UPSCROLLBAR_Y;
    v->up_button.width = WINKS_SLIDESCROLLBAR_WIDTH;//WINKS_SCREEN_WIDTH-WINKS_UPSCROLLBAR_X;
    v->up_button.height  =  WINKS_SLIDESCROLLBAR_Y-WINKS_UPSCROLLBAR_Y;
    v->up_button.normal_down_icon = WINKS_UP_FOCUS_ICONID;
    v->up_button.normal_up_icon = WINKS_UP_ICONID;
    v->up_button.icon_x = WINKS_UPSCROLLBAR_X;
    v->up_button.icon_y = WINKS_UPSCROLLBAR_Y;

    v->scroll_button.normal_color = WINKS_PIXEL_BLACK;
    v->scroll_button.focussed_down_color = WINKS_PIXEL_GREEN;
    v->scroll_button.x = WINKS_UPSCROLLBAR_X;
    v->scroll_button.y = WINKS_UPSCROLLBAR_Y + v->up_button.height;
    v->scroll_button.width = WINKS_SLIDESCROLLBAR_WIDTH;//WINKS_SCREEN_WIDTH-WINKS_UPSCROLLBAR_X;
    v->scroll_button.height  =  WINKS_SLIDESCROLLBAR_Y-WINKS_UPSCROLLBAR_Y;;
    v->scroll_button.normal_color = WINKS_PIXEL_BLACK;



    v->down_button.x = WINKS_DOWNSCROLLBAR_X,
    v->down_button.y = WINKS_DOWNSCROLLBAR_Y;
    v->down_button.width = WINKS_SLIDESCROLLBAR_WIDTH;//WINKS_SCREEN_WIDTH-WINKS_DOWNSCROLLBAR_X;
    v->down_button.height  =  v->up_button.height;
    v->down_button.normal_color = WINKS_PIXEL_BLACK;
    v->down_button.normal_down_icon = WINKS_DOWN_FOCUS_ICONID;
    v->down_button.normal_up_icon = WINKS_DOWN_ICONID;
    v->down_button.icon_x = WINKS_DOWNSCROLLBAR_X;
    v->down_button.icon_y = WINKS_DOWNSCROLLBAR_Y;
    v->x = WINKS_UPSCROLLBAR_X;
    v->y = WINKS_UPSCROLLBAR_Y + v->up_button.height;
    v->width = WINKS_SLIDESCROLLBAR_WIDTH;//WINKS_SCREEN_WIDTH-WINKS_UPSCROLLBAR_X;
    v->height = WINKS_DOWNSCROLLBAR_Y -WINKS_UPSCROLLBAR_Y -( WINKS_SLIDESCROLLBAR_Y - WINKS_UPSCROLLBAR_Y);

}

/**************************************************************

	FUNCTION NAME		: winks_ui_drawVerticalScrollBar

  	PURPOSE		        : draw a VerticalScrollBar.

	INPUT PARAMETERS	: winks_Rect_s vp_Rect
					      int v_Scale
					      int v_Progress
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/
void  winks_ui_drawVerticalScrollBar(winks_vertical_scrollbar_s * p_scrollbar)
{	
    /*winks_gui_create_vertical_scrollbar(vertical_scrollbar *v, int x, int y, int width, int height)
    winks_gui_show_vertical_scrollbar (vertical_scrollbar *v);*/

    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int x1, y1, x2, y2, sbutton_height, sb_y, sb_yrange, r, min_height, max_height;
    //char buttons_flag = 1;
    //winks_Rect_s *f;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    if (winks_draw_condition(WINKS_MINGUI_HWND_USER) != WINKS_RETURN_SUCCESS)
    {
        return;
    }

    x1 = p_scrollbar->x;
    x2 = x1 + p_scrollbar->width - 1;
    y1 = p_scrollbar->y;
    y2 = y1 + p_scrollbar->height - 1;

    //gdi_draw_solid_rect(x1, y1, x2, y2, source_key_value);


    //winks_ui_drawRect(x1 - 1, y1 - 1, x2 + 1, y2 + 1, WINKS_PIXEL_BLACK);
    winks_ui_fillRect(x1, y1, x2, y2, p_scrollbar->background_color);



    //   winks_ui_drawIconButton(&p_scrollbar->up_button);
    //    winks_ui_drawIconButton(&p_scrollbar->down_button);

    //  gui_scrollbar_draw_button(GUI_SCROLLBAR_BUTTON_UP, &v->up_button);
    // gui_scrollbar_draw_button(GUI_SCROLLBAR_BUTTON_DOWN, &v->down_button);

    //max_height = p_scrollbar->height - p_scrollbar->up_button.height - p_scrollbar->down_button.height;
    max_height = p_scrollbar->height;
    /* Show the scroll button based on range and value */
    r = p_scrollbar->range - p_scrollbar->scale;
    //#if defined(UI_SCROLLBAR_STYLE_1)
    min_height = p_scrollbar->width;
    //#elif defined(UI_SCROLLBAR_STYLE_2) || defined(UI_SCROLLBAR_STYLE_3)
    //   min_height = gui_scrollbar_get_button_min_size(GUI_SCROLLBAR_BUTTON_VERTICAL_SCROLL, v->width);
    //#endif 
    if (min_height > max_height)
    {
    	min_height = max_height;
    }
    if (p_scrollbar->scale > 0)
    {
        if (p_scrollbar->range > 0)
        {
        	 sbutton_height = (p_scrollbar->scale * max_height) / p_scrollbar->range;
        }
        else
        {
            sbutton_height = max_height;
        }
    }
    else
    {
        sbutton_height = min_height;
    }
    if (sbutton_height < min_height)
    {
        sbutton_height = min_height;
    }
    if (sbutton_height > max_height)
    {
        sbutton_height = max_height;
    }
    sb_yrange = max_height - sbutton_height;
    if (r > 0)
    {
        /* In case of very large r & v->value(double) */
        sb_y = (int) ( sb_yrange * p_scrollbar->value / r);
    }
    else
    {
        sb_y = 0;
    }
    if (sb_y < 0)
    {
        sb_y = 0;
    }
    if (sb_y > sb_yrange)
    {
        sb_y = sb_yrange;
    }

    p_scrollbar->scroll_button.height = sbutton_height;
    //p_scrollbar->scroll_button.y = sb_y + y1 + (buttons_flag ? p_scrollbar->up_button.height : 0);
    p_scrollbar->scroll_button.y = sb_y + y1;
    //#if defined(UI_SCROLLBAR_STYLE_1)
    //   gui_show_icon_button(&v->scroll_button);
    //#elif defined(UI_SCROLLBAR_STYLE_2)
    wk_gui_scrollbar_draw_button(&p_scrollbar->scroll_button);
    //#elif defined(UI_SCROLLBAR_STYLE_3)

    //#endif 
    winks_ui_drawIconButton(&p_scrollbar->up_button);
    winks_ui_drawIconButton(&p_scrollbar->down_button);

}


/**************************************************************

	FUNCTION NAME		: wk_gui_scrollbar_draw_button

  	PURPOSE		        : draw a scrollbar's button.

	INPUT PARAMETERS	: winks_icon_button_s *b
					 
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

static void wk_gui_scrollbar_draw_button( winks_icon_button_s *b)
{
    int x2, y2;
    int  i;
    winks_color c,cs[2]= {WINKS_PIXEL_GREEN,WINKS_PIXEL_GREEN - 80};
    winks_color c_border = WINKS_PIXEL_DARKGRAY;
    int x, y, width, height;
    int num_shadow_line;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    x = b->x;
    y = b->y -1 ;
    width = b->width;
    height = b->height;
    c = b->focussed_down_color;
    num_shadow_line = 2;
    if (!(width % 2))
    {
        width--;
    }
    if (height >= 8)
    {
        /* Leave 1-pixel space */
        y++;
        height -= 2;
    }

    x2 = x + width - 1;
    y2 = y + height + 1;// - 1;

    if (width <= 5)
    {
        winks_ui_drawLine(x,y + 1, x,y2 - 1, c_border);
        winks_ui_drawLine(x2,y + 1, x2,y2 - 1,  c_border);
        winks_ui_drawLine(x + 1,  y,x2 - 1, y, c_border);
        winks_ui_drawLine(x + 1,y2,  x2 - 1, y2, c_border);
        for (i = x + 1; i < x2 - num_shadow_line; i++)
        {
            winks_ui_drawLine( i,y + 1,  i,y2 - 1,  c);//used diffrent  color
        }

        for (i = 0; i < num_shadow_line; i++)
        {
            winks_ui_drawLine(x2 - 1 - i, y + 1,x2 - 1 - i,  y2 - 1, cs[i]);
        }
    }
    else
    {
        #if 0
        winks_ui_drawLine( x,y + 2,  x,y2 - 2, c_border);
        winks_ui_drawLine(x2,y + 2,x2, y2 - 2,  c_border);
        winks_ui_drawLine(x + 2,y,  x2 - 2,y,  c_border);
        winks_ui_drawLine(x + 2, y2,x2 - 2, y2, c_border);
        #endif 
        winks_ui_drawLine( x,y,  x,y2, c_border);
        winks_ui_drawLine(x2,y,x2, y2,  c_border);
        winks_ui_drawLine(x,y,  x2,y,  c_border);
        winks_ui_drawLine(x, y2,x2, y2, c_border);
        for (i = x + 1; i < x2 - num_shadow_line; i++)
        {
            winks_ui_drawLine(i,y + 1, i,y2 - 1,  c);
        }

        for (i = 0; i < num_shadow_line; i++)
        {
            winks_ui_drawLine( x2 - 1 - i,y + 1,  x2 - 1 - i,y2 - 1, cs[i]);
        }
        /* rect c_border button c*/
        #if 0
        winks_ui_drawPoint(x + 1, y + 1, c);
        winks_ui_drawPoint(x2 - 1, y + 1, c);
        winks_ui_drawPoint(x + 1, y2 - 1, c);
        winks_ui_drawPoint(x2 - 1, y2 - 1, c);
        #endif

    }
}

#endif
/**************************************************************

	FUNCTION NAME		: winks_ui_drawHorizontalProgressBar

  	PURPOSE		        : draw a HorizontalProgressBar.

	INPUT PARAMETERS	: winks_Rect_s vp_Rect
					      int v_Scale
					      int v_Progress
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

//void winks_ui_drawHorizontalProgressBar (winks_Rect_s vp_Rect,int v_Scale,int v_Progress)
//{	
	/*Winks_gui_create_horizontal_progress_indicator(
	                    vertical_progress_indicator *pi,
	                    int x,
	                    int y,
	                    int width,
	                    int height);
	Winks_gui_show_horizontal_progress_indicator(vertical_progress_indicator *pi);*/
//}


/**************************************************************

	FUNCTION NAME		: winks_ui_drawPOPMenuItem

  	PURPOSE		        : draw a POPMenuItem.

	INPUT PARAMETERS	: winks_Rect_s v_ItemArea
					      char v_IsHighlight
					      winks_PopMenuItem_s* p_PopupMenuItem 
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void  winks_ui_drawPOPMenuItem(winks_PopMenuItem_s* p_PopupMenuItem ,int index_in_screen)
{
    int x,y;

    x = WINKS_POPUP_STRING_X;
    y = WINKS_POPUP_BOTTOM_Y - index_in_screen * WINKS_POPUP_ITEM_HEIGHT + (WINKS_POPUP_ITEM_HEIGHT - WINKS_GETFONTHEIGHT) / 2;

    winks_set_font(WINKS_MEDIUM_FONT);
    WINKS_SET_TEXTCOLOR(WINKS_PIXEL_BLACK);
    winks_ui_textOutlen(x, y,Winks_UIFM_GetString(p_PopupMenuItem->v_PopMenuItem_StringId),winks_strlen_ucs2_type((char *)Winks_UIFM_GetString(p_PopupMenuItem->v_PopMenuItem_StringId)));
}


/**************************************************************

	FUNCTION NAME		: winks_ui_drawlistitem

  	PURPOSE		        : draw a POPMenuItem.

	INPUT PARAMETERS	: winks_Rect_s v_ItemArea
					      char v_ListType
					      winks_Listitem_s* p_ListItem
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void  winks_ui_drawListItem(winks_Listitem_s* p_ListItem, int index_in_screen)
{	
    //Winks_gui_create_fixed_text_menuitem();
    //Winks_gui_show_fixed_text_menuitem();
    int x;
    int y;
    int icon_x,icon_y;
    int radio_on  =WINKS_UI_RADIOSEL_IMAGEID;
    int radio_off =WINKS_UI_RADIO_IMAGEID;
    int check_on = WINKS_UI_CHECKBOXSEL_IMAGEID;
    int check_off =WINKS_UI_CHECKBOX_IMAGEID;

    int icon = 0;

    //char * temp[1024] ;
    char * text = NULL;
    int textclip_lefttop_x,textclip_lefttop_y;
    int textclip_rightbottom_x,textclip_rightbottom_y;
    int w = 0, h = 0;
    int excursion = 0;
    //winks_Rect_s v_rect;


    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()


    winks_set_font(WINKS_MEDIUM_FONT);
    
    if(p_ListItem->v_ListType == WINKS_UI_LIST_TYPE_TEXT)
    {
        x = WINKS_LIST_FIRSTSTRINGONLY_X;
        y = ( index_in_screen -1)* WINKS_HIGHLIGHT_HEIGHT+ WINKS_LIST_FIRSTSTRINGONLY_Y;

        }
    else
    {
        if(p_ListItem->v_ListType == WINKS_UI_LIST_TYPE_RADIO)
        {
        	if (Winks_UIContext->gv_ListScreen->v_FocusItemIndxInScrn == index_in_screen)//(p_ListItem->v_ItemSetFlag)
        	{
        		icon = radio_on;
        	}
        	else
        	{
        		icon = radio_off;
        	}
        }
        if(p_ListItem->v_ListType == WINKS_UI_LIST_TYPE_CHECKBOX)
        {
        	if(p_ListItem->v_ItemSetFlag)
        	{
        		icon = check_on;
        	}
        	else
        	{
        		icon = check_off;
        	}
        }
        icon_x = WINKS_LIST_ICON_X;
        icon_y =  ( index_in_screen -1) * WINKS_HIGHLIGHT_HEIGHT+ WINKS_LIST_ICON_Y;
        if (icon != 0)
        {
            WINKS_CLIP_ON(icon_x, icon_y,icon_x + WINKS_LIST_ICON_WIDTH, icon_y + WINKS_LIST_ICON_HEIGHT);
            winks_ui_drawGraphicFromFile(icon_x, icon_y,Winks_UIFM_GetImage(icon));//Winks_UIFM_GetImage(icon)
            WINKS_CLIP_OFF(icon_x, icon_y,icon_x + WINKS_LIST_ICON_WIDTH, icon_y + WINKS_LIST_ICON_HEIGHT);
        }
        //x = WINKS_LIST_FIRSTSTRINGONLY_X + WINKS_LIST_ICON_X;
        x = WINKS_LIST_ICON_X + WINKS_LIST_ICON_WIDTH;
        y =   ( index_in_screen -1) * WINKS_HIGHLIGHT_HEIGHT+ WINKS_LIST_FIRSTSTRINGONLY_Y;
    }
    if(p_ListItem->v_flags != FALSE)	
    {
        //winks_ui_textOutlen(x, y,Winks_UIFM_GetString( p_ListItem->v_ItemStrID),strlen(Winks_UIFM_GetString( p_ListItem->v_ItemStrID)));
        text = (char *)Winks_UIFM_GetString( p_ListItem->v_ItemStrID);
    }
    else
    {
        //winks_ui_textOutlen(x, y,(char *)p_ListItem->v_ItemStrID,strlen((char *)p_ListItem->v_ItemStrID));
        text = (char *)p_ListItem->v_ItemStrID;
    }
    {
        winks_get_string_wight_high(text, winks_strlen_ucs2_type(text),&w,&h);
    }
        excursion = (WINKS_HIGHLIGHT_LISTITEM_HEIGHT  - WINKS_GETFONTHEIGHT) >> 1;  
        textclip_lefttop_x = x;
        textclip_lefttop_y = y - excursion;//WINKS_HIGHLIGHT_EXCURSION_HEIGHT;
        if(Winks_UIContext->gv_ListScreen->v_TotalItemsNum > WINKS_LIST_MAX_NUMBER)
        {
        textclip_rightbottom_x = WINKS_DOWNSCROLLBAR_X - WINKS_LIST_STRINGONLY_SCROOBAR_GAP;
        }
        else
        {
            textclip_rightbottom_x = WINKS_SCREEN_WIDTH - 1;
        }
        textclip_rightbottom_y = textclip_lefttop_y + WINKS_HIGHLIGHT_LISTITEM_HEIGHT;
    if(Winks_UIContext->gv_ListScreen->v_FocusItemIndxInScrn == index_in_screen)
    {
       
        {
            winks_get_string_wight_high(text, winks_strlen_ucs2_type(text),&w,&h);
        }
        WINKS_SET_TEXTCOLOR(WINKS_PIXEL_LIGHTWHITE);
        if(w > (textclip_rightbottom_x - textclip_lefttop_x ))
        {
        Winks_font_feature_s font_feature = {0};
			  font_feature.font_size = WINKS_MEDIUM_FONT;
        
#if 1
            if(Winks_UIContext->scrooltext_flag == 0) 
            {
              
                winks_ui_create_scrolling_text(
                &Winks_UIContext->winks_scroll_string,
                textclip_lefttop_x,  
                textclip_lefttop_y,
                textclip_rightbottom_x - textclip_lefttop_x,
                textclip_rightbottom_y - textclip_lefttop_y,
                text,
                winks_ui_handle_scrolling_text,
                //void (*draw_background) (int x1, int y1, int x2, int y2),
                WINKS_PIXEL_LIGHTWHITE,
				WINKS_STRINGSCROLL_LEFT,
                WINKS_HILIGHT_COLOR,
                NULL,
                //winks_color border_color
				font_feature
                );


                //winks_ui_redraw_scrolling_text(&s);
                Winks_UIContext->scrooltext_flag = 1;
                winks_ui_scrolling_text_start(&Winks_UIContext->winks_scroll_string);
            }
			#endif
            winks_ui_redraw_scrolling_text(&Winks_UIContext->winks_scroll_string);
        }
        else
        {
            WINKS_CLIP_ON(  textclip_lefttop_x,  
                            textclip_lefttop_y,
                            textclip_rightbottom_x,
                            textclip_rightbottom_y);
            winks_ui_textOutlen(x,y,text,winks_strlen_ucs2_type(text)); 
            WINKS_CLIP_OFF( textclip_lefttop_x,  
                            textclip_lefttop_y,
                            textclip_rightbottom_x,
                            textclip_rightbottom_y);
            
        }
    }
  
    else
    {
        WINKS_SET_TEXTCOLOR(WINKS_PIXEL_LIGHTWHITE);
        WINKS_CLIP_ON( textclip_lefttop_x,  
                            textclip_lefttop_y,
                            textclip_rightbottom_x,
                            textclip_rightbottom_y);
        winks_ui_textOutlen(x,y,text,winks_strlen_ucs2_type(text));  
        WINKS_CLIP_OFF( textclip_lefttop_x,  
                        textclip_lefttop_y,
                        textclip_rightbottom_x,
                        textclip_rightbottom_y); 
    }
     /*   v_rect.v_Bottom = textclip_lefttop_x;  
        v_rect.v_Top = textclip_lefttop_y;
                        v_rect.v_Left = textclip_rightbottom_x;
                        
    
    v_rect.v_Right = textclip_rightbottom_x - textclip_lefttop_x;
    
    Winks_bitblt(& v_rect);*/  
#if 0
    if(p_List->a_ItemList != NULL)
    {
        ptemp = &p_List->a_ItemList[index - 1];
    }
    else
    {
        ptemp =&p_List->a_ItemListData[index - 1];
    }
    if(ptemp->v_ListType == WINKS_UI_LIST_TYPE_TEXT)
    {
        textclip_lefttop_x = WINKS_LIST_FIRSTSTRINGONLY_X;
    }
    else
    {
        textclip_lefttop_x = WINKS_LIST_FIRSTSTRINGONLY_X + WINKS_LIST_ICON_X;
    }
    textclip_lefttop_y = highlight_y;
    if(p_List->v_TotalItemsNum > WINKS_LIST_MAX_NUMBER)
    {
        textclip_rightbottom_x = WINKS_DOWNSCROLLBAR_X - 1;
    }
    else
    {
        textclip_rightbottom_x = WINKS_DOWNSCROLLBAR_X + WINKS_SLIDESCROLLBAR_WIDTH;
    }
    textclip_rightbottom_y = highlight_y + WINKS_HIGHLIGHT_HEIGHT;

    if(p_List->a_ItemList != NULL)
    {
        text = (char *)p_List->a_ItemList[p_List->v_FocusItemIndxInScrn - 1].v_ItemStrID;
    }
    else
    {
        text = Winks_UIFM_GetString( p_List->a_ItemListData[p_List->v_FocusItemIndxInScrn - 1].v_ItemStrID);
    }
    {
        char ptmp[500];
        Winks_mem_set(ptmp, 0, 500);
        text_y = ( p_List->v_FocusItemIndxInScrn - 1) * WINKS_HIGHLIGHT_HEIGHT+ WINKS_LIST_ICON_Y;
        gb2312tounicode( ptmp, 500, text, strlen(text) );
        winks_get_string_wight_high(ptmp,500,&w,&h);
    }
    if(w > (textclip_rightbottom_x - textclip_lefttop_x))
    {
        if(Winks_UIContext->scrooltext_flag == 0) 
        {
          
            winks_ui_create_scrolling_text(
            &Winks_UIContext->winks_scroll_string,
            textclip_lefttop_x,
            textclip_lefttop_y,
            textclip_rightbottom_x - textclip_lefttop_x - 40,
            textclip_rightbottom_y - textclip_lefttop_y,
            text,
            //void (*timer_callback) (void),
            //void (*draw_background) (int x1, int y1, int x2, int y2),
            WINKS_PIXEL_LIGHTWHITE,
            WINKS_PIXEL_BLACK,
            NULL
            //winks_color border_color
            );


            //winks_ui_redraw_scrolling_text(&s);
            Winks_UIContext->scrooltext_flag = 1;
            winks_ui_scrolling_text_start(&Winks_UIContext->winks_scroll_string);
        }
        winks_ui_redraw_scrolling_text(&Winks_UIContext->winks_scroll_string);
    }
    else
    {
    winks_ui_textOutlen(textclip_lefttop_x,text_y,text,strlen(text));  
    }
#endif
}
/**************************************************************

	FUNCTION NAME		: winks_ui_drawListMenu_count

  	PURPOSE		        : draw count list.

	INPUT PARAMETERS	: winks_POPMenuListScreen_s *p_Menu
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void winks_ui_drawListMenu_count(winks_List_Screen_s *p_List)
{
    int count = 0;
    char temp_number[256] = {0};
    char number[256] = {0};

    winks_set_font(WINKS_MEDIUM_FONT);
    
    if(p_List->v_TotalItemsNum > WINKS_LIST_MAX_NUMBER)
    {
        count = p_List->v_FirstItemIndx + p_List->v_FocusItemIndxInScrn -1;
        if (count > p_List->v_TotalItemsNum)
            count = count - p_List->v_TotalItemsNum;
        sprintf( temp_number, "%d", count);
	    Winks_Ascii2UnicodeString( temp_number, number, 256);
        winks_clear_screen_fillRect(WINKS_TITLE_DIGIT_X,WINKS_TITLE_DIGIT_Y,WINKS_TITLE_STRING_X + WINKS_CLIENT_WIDTH,WINKS_TITLE_HEIGHT);
        WINKS_SET_TEXTCOLOR(WINKS_PIXEL_LIGHTWHITE);
        winks_ui_textOutlen(WINKS_TITLE_DIGIT_X,WINKS_TITLE_DIGIT_Y,number,winks_strlen_ucs2_type(number));
    }
}
/**************************************************************

	FUNCTION NAME		: winks_ui_drawListScreen

  	PURPOSE		        : draw a List Screen.

	INPUT PARAMETERS	: winks_List_Screen_s *p_List
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void winks_ui_drawListScreen(winks_List_Screen_s *p_List)
{	
    int  index, number;
    int  highlight_x, highlight_y;
    int  highlight_end_x;
    int excursion = 0;

    //winks_vertical_scrollbar_s scrollbar;

	
	
    WINKS_UI_CONTEXT_DEF()
	WINKS_UI_CONTEXT_INIT()


    Winks_assert(p_List != NULL);

    if (winks_draw_condition(WINKS_MINGUI_HWND_USER) != WINKS_RETURN_SUCCESS)
    {
        return;
    }
    if (p_List ->v_TotalItemsNum == 0) 
    {
      	Winks_printf("winks_List_Screen_s Num is 0!!");
      	return;
    }

    winks_set_font(WINKS_MEDIUM_FONT);
    
    //winks_ui_fillRect(WINKS_CLIENT_X,WINKS_CLIENT_Y,WINKS_CLIENT_WIDTH,WINKS_CLIENT_Y + WINKS_CLIENT_HEIGHT,WINKS_TRANSPARENT_COLOR);//WINKS_PIXEL_TRANSPARENT
    winks_clear_screen_fillRect(WINKS_CLIENT_X,WINKS_CLIENT_Y,WINKS_CLIENT_WIDTH,WINKS_CLIENT_Y + WINKS_CLIENT_HEIGHT);
#if 1
    /*画亮度条*/
    excursion = (WINKS_HIGHLIGHT_LISTITEM_HEIGHT  - WINKS_GETFONTHEIGHT) >> 1; 
    highlight_x = WINKS_LIST_ICON_X;
    highlight_y =  (p_List->v_FocusItemIndxInScrn - 1) * WINKS_HIGHLIGHT_HEIGHT+ WINKS_LIST_FIRSTSTRINGONLY_Y - excursion;//WINKS_HIGHLIGHT_EXCURSION_HEIGHT;
    //winks_ui_drawGraphicFromFile(highlight_x, highlight_y, Winks_UIFM_GetImage(WINKS_UI_HIGHLIGHT_IMAGEID) );

    
    if(p_List->v_TotalItemsNum > WINKS_LIST_MAX_NUMBER)
    {
        highlight_end_x = WINKS_CLIENT_WIDTH - WINKS_LIST_STRINGONLY_SCROOBAR_GAP - WINKS_SLIDESCROLLBAR_WIDTH;
    }
    else
    {
        highlight_end_x = WINKS_CLIENT_WIDTH;
    }   
    winks_ui_fillRect(highlight_x,highlight_y,highlight_end_x,highlight_y + WINKS_HIGHLIGHT_LISTITEM_HEIGHT,WINKS_HILIGHT_COLOR);
    /*如果listitem总数 大于 一屏显示的最大数
    画ScrollBar
    if(p_List->v_TotalItemsNum > WINKS_LIST_MAX_NUMBER)
    {
    	winks_ui_create_vertical_scrollbar(&(Winks_UIContext->gv_scrollbar), p_List->v_FirstItemIndx, p_List->v_TotalItemsNum, WINKS_LIST_MAX_NUMBER);
    	winks_ui_drawVerticalScrollBar(&(Winks_UIContext->gv_scrollbar));
    }*/
#endif
    if (p_List->v_TotalItemsNum <= WINKS_LIST_MAX_NUMBER)
    {
        for(index = p_List->v_FirstItemIndx; index <= p_List->v_TotalItemsNum; index++)
        {
            if(p_List->a_ItemList != NULL)
            {
                winks_ui_drawListItem(&p_List->a_ItemList[index - 1],index-p_List->v_FirstItemIndx + 1);
            }
            else
            {
                winks_ui_drawListItem(&p_List->a_ItemListData[index - 1],index-p_List->v_FirstItemIndx + 1);
            }
        }
    }
    else
    {
        // if ((p_List->a_ItemListData[p_List->v_TotalItemsNum - 1].v_NextMenuFlag))
        // {
        for(index = p_List->v_FirstItemIndx,number = WINKS_LIST_MAX_NUMBER;index <= p_List->v_TotalItemsNum  && number > 0;index++, number--)
        {
            if(p_List->a_ItemList != NULL)
            {
                winks_ui_drawListItem(&p_List->a_ItemList[index - 1],index-p_List->v_FirstItemIndx + 1);
            }
            else
            {
                winks_ui_drawListItem(&p_List->a_ItemListData[index - 1],index-p_List->v_FirstItemIndx + 1);
            }
        }
#if 0		
        for(index = 1;(index <= MIN(number,(p_List->v_TotalItemsNum  -(WINKS_LIST_MAX_NUMBER - number))));index++)
        {
            if(p_List->a_ItemList != NULL)
            {
                winks_ui_drawListItem(&p_List->a_ItemList[index - 1],WINKS_LIST_MAX_NUMBER - number + 1);
            }
            else
            {
                winks_ui_drawListItem(&p_List->a_ItemListData[index - 1],WINKS_LIST_MAX_NUMBER - number + 1);
            }
        }

    //}

    else
    	{
        for(index = p_List->v_FirstItemIndx,number = WINKS_LIST_MAX_NUMBER;index <= p_List->v_TotalItemsNum  &&number > 0;index++, number--)
     	{
    		if(p_List->a_ItemList != NULL)
            {
                winks_ui_drawListItem(&p_List->a_ItemList[index - 1],index-p_List->v_FirstItemIndx + 1);
            }
            else
            {
                winks_ui_drawListItem(&p_List->a_ItemListData[index - 1],index-p_List->v_FirstItemIndx + 1);
            }
    	}
     	
    		for(index = 1;index <= MIN(number,(p_List->v_TotalItemsNum -(WINKS_LIST_MAX_NUMBER - number))) > 0;index++)
        {
    		if(p_List->a_ItemList != NULL)
            {
                winks_ui_drawListItem(&p_List->a_ItemList[index - 1],index-p_List->v_FirstItemIndx + 1);
            }
            else
            {
                winks_ui_drawListItem(&p_List->a_ItemListData[index - 1],index-p_List->v_FirstItemIndx + 1);
            }
    	}
    	/* if(number > 0&&(p_List->a_ItemListData[p_List->v_TotalItemsNum - 1].v_NextMenuFlag))
    	 	{
    	 		for(index = 1;number > 0;index++, number--)
    			    {
    					winks_ui_drawListItem(&p_List->a_ItemList[index  - 1],index-p_List->v_FirstItemIndx + 1);
    				}
    	 	}*/
    	}
#endif
    }
#if 0
    /*画亮度条*/
    highlight_x = WINKS_LIST_ICON_X;
    highlight_y =  (p_List->v_FocusItemIndxInScrn - 1) * WINKS_HIGHLIGHT_HEIGHT+ WINKS_LIST_FIRSTSTRINGONLY_Y - WINKS_HIGHLIGHT_EXCURSION_HEIGHT;
winks_draw_opacity_on();

    WINKS_OPACITY_ON(highlight_x,highlight_y,WINKS_SCREEN_WIDTH,WINKS_HIGHLIGHT_HEIGHT);
    winks_ui_drawGraphicFromFile(highlight_x, highlight_y, Winks_UIFM_GetImage(WINKS_UI_HIGHLIGHT_IMAGEID));
    //winks_show_transparent_image(highlight_x, highlight_y, Winks_UIFM_GetImage(WINKS_HIGHLIGHT_POPUP_ICONID));
    WINKS_OPACITY_OFF(highlight_x,highlight_y,WINKS_SCREEN_WIDTH,WINKS_HIGHLIGHT_HEIGHT);
    //winks_image_draw_resized_file(highlight_x + 10, highlight_y+ 30, 113,30,Winks_UIFM_GetImage(WINKS_HIGHLIGHT_POPUP_ICONID));
    /*如果listitem总数 大于 一屏显示的最大数
    画ScrollBar*/
#endif
    //winks_ui_fillRect(highlight_x, highlight_y,highlight_x + 130, highlight_y +WINKS_HIGHLIGHT_HEIGHT,0x0000FF);
    //winks_ui_fillRect(highlight_x, highlight_y,highlight_x + 130, highlight_y +WINKS_HIGHLIGHT_HEIGHT ,  WINKS_PIXEL_RED );

    if(p_List->v_TotalItemsNum > WINKS_LIST_MAX_NUMBER)
    {
    	winks_ui_create_vertical_scrollbar(&(Winks_UIContext->gv_scrollbar), p_List->v_FirstItemIndx, p_List->v_TotalItemsNum, WINKS_LIST_MAX_NUMBER);
    	winks_ui_drawVerticalScrollBar(&(Winks_UIContext->gv_scrollbar));
    }
   // winks_ui_drawListMenu_count(p_List);
    
}

/**************************************************************

	FUNCTION NAME		: winks_ui_drawPOPMenuScreen

  	PURPOSE		        : draw a POPMenu Screen.

	INPUT PARAMETERS	: winks_POPMenuListScreen_s *p_Menu
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void winks_ui_drawPOPMenuScreen(winks_POPMenuListScreen_s *p_Menu )
{	
    int index;
    int highlight_x, highlight_y;
    int width = 0;
    int max = 0;
    /*画个白色矩形区域
    在矩形区域里画item*/
    
    Winks_assert(p_Menu != NULL);
    
    if (winks_draw_condition(WINKS_MINGUI_HWND_USER) != WINKS_RETURN_SUCCESS)
    {
        return;
    }

    winks_set_font(WINKS_MEDIUM_FONT);
    
    #if 1
    for(index = p_Menu->v_TotalItemsNum; index > 0; index--)
    {
        winks_PopMenuItem_s* p_PopupMenuItem;
        char * text = NULL;
        int w = 0,h = 0;
        
    
        p_PopupMenuItem = &p_Menu->a_ItemList[p_Menu->v_TotalItemsNum - index];
        text = (char * )Winks_UIFM_GetString(p_PopupMenuItem->v_PopMenuItem_StringId);
        winks_get_string_wight_high(text, winks_strlen_ucs2_type(text),&w,&h);
        if (max < w)
        {
            max = w;
        }
    }
	index = 0;
    width = max + (( WINKS_POPUP_STRING_X - WINKS_POPUP_BOTTOM_X) << 1);
    winks_ui_fillRect(WINKS_POPUP_BOTTOM_X,WINKS_POPUP_BOTTOM_Y - p_Menu->v_TotalItemsNum * WINKS_POPUP_ITEM_HEIGHT,WINKS_POPUP_BOTTOM_X + width, WINKS_POPUP_BOTTOM_Y, WINKS_PIXEL_LIGHTWHITE );
    /*画亮度条*/
    highlight_x = WINKS_POPUP_BOTTOM_X;
    highlight_y = WINKS_POPUP_BOTTOM_Y - (p_Menu->v_TotalItemsNum + 1 - p_Menu->v_FocusItemIndxInScrn) * WINKS_POPUP_ITEM_HEIGHT ;
    winks_ui_fillRect(highlight_x,highlight_y,highlight_x + width,highlight_y + WINKS_POPUP_ITEM_HEIGHT,WINKS_HILIGHT_COLOR);
    
    #endif
    #if 0
    winks_ui_fillRect(WINKS_POPUP_BOTTOM_X,WINKS_POPUP_BOTTOM_Y - p_Menu->v_TotalItemsNum * WINKS_POPUP_ITEM_HEIGHT,WINKS_POPUP_BOTTOM_X + WINKS_POPUP_WIDTH, WINKS_POPUP_BOTTOM_Y, WINKS_PIXEL_LIGHTWHITE );
    /*画亮度条*/
    highlight_x = WINKS_POPUP_BOTTOM_X;
    highlight_y = WINKS_POPUP_BOTTOM_Y - (p_Menu->v_TotalItemsNum + 1 - p_Menu->v_FocusItemIndxInScrn) * WINKS_POPUP_ITEM_HEIGHT ;
    winks_ui_fillRect(highlight_x,highlight_y,highlight_x + WINKS_POPUP_WIDTH,highlight_y + WINKS_POPUP_ITEM_HEIGHT,WINKS_HILIGHT_COLOR);
    #endif
    for(index = p_Menu->v_TotalItemsNum; index > 0; index--)
    {
        winks_ui_drawPOPMenuItem(&p_Menu->a_ItemList[p_Menu->v_TotalItemsNum -index],index);
    }			     
   

    
    
    /////WINKS_OPACITY_ON(highlight_x,highlight_y,WINKS_POPUP_WIDTH,WINKS_POPUP_ITEM_HEIGHT);
    ////winks_ui_drawGraphicFromFile(highlight_x, highlight_y, Winks_UIFM_GetImage(WINKS_HIGHLIGHT_POPUP_ICONID));
    //winks_show_transparent_image(highlight_x, highlight_y, Winks_UIFM_GetImage(WINKS_HIGHLIGHT_POPUP_ICONID));
    ////WINKS_OPACITY_OFF(highlight_x,highlight_y,WINKS_POPUP_WIDTH,WINKS_POPUP_ITEM_HEIGHT);
}


/**************************************************************

	FUNCTION NAME		: winks_InfoBox_s

  	PURPOSE		        : draw a Infobox.

	INPUT PARAMETERS	: winks_InfoBox_s *p_infobox
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void  winks_ui_drawInfobox(winks_InfoBox_s *p_infobox)
{	
    winks_Rect_s Rect;
    WINKS_SHOW_FLAG TextAlign = 0;

    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    Winks_assert(p_infobox != NULL);

    if (winks_draw_condition(WINKS_MINGUI_HWND_USER) != WINKS_RETURN_SUCCESS)
    {
        return;
    }
    //for scroll string
    winks_ui_scrolling_text_stop(&Winks_UIContext->winks_scroll_string);

    winks_set_font(WINKS_MEDIUM_FONT);
    
    TextAlign = WINKS_SHOW_CENTER;
    Rect.v_Bottom = WINKS_INFOBOX_STRING_Y + WINKS_INFOBOX_STRING_HEIGHT;
    Rect.v_Left = WINKS_INFOBOX_STRING_X;
    Rect.v_Right = WINKS_INFOBOX_STRING_X + WINKS_INFOBOX_STRING_WIDTH;
    Rect.v_Top = WINKS_INFOBOX_STRING_Y;
    winks_ui_drawGraphicFromFile(WINKS_INFOBOX_X ,WINKS_INFOBOX_Y , Winks_UIFM_GetImage(WINKS_INFOBOX_BGICON));//(p_infobox->v_bgicon)
    WINKS_SET_TEXTCOLOR(WINKS_PIXEL_BLACK);
    //winks_ui_DrawstringInRect(Winks_UIFM_GetString(p_infobox->v_InfoboxStringId), &Rect, TextAlign, strlen(Winks_UIFM_GetString(p_infobox->v_InfoboxStringId)));

    winks_creat_text_string_rect_type(&Winks_UIContext->text_String_rect_infobox, &Rect, TextAlign, (char *)Winks_UIFM_GetString(p_infobox->v_InfoboxStringId), (int )winks_strlen_ucs2_type((char *)Winks_UIFM_GetString(p_infobox->v_InfoboxStringId)));
    winks_show_string_in_rect(&Winks_UIContext->text_String_rect_infobox, &Rect, TextAlign);
}
/**************************************************************

	FUNCTION NAME		: winks_InfoBox_s

  	PURPOSE		        : draw a Infobox by string.

	INPUT PARAMETERS	: char *p_infobox
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/
void  winks_ui_drawInfobox_string(char * text)
{	
    winks_Rect_s Rect;
    WINKS_SHOW_FLAG TextAlign = 0;

    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    Winks_assert(text != NULL);

    if (winks_draw_condition(WINKS_MINGUI_HWND_USER) != WINKS_RETURN_SUCCESS)
    {
        return;
    }

    winks_set_font(WINKS_MEDIUM_FONT);
    
    TextAlign = WINKS_SHOW_CENTER;
    Rect.v_Bottom = WINKS_INFOBOX_STRING_Y + WINKS_INFOBOX_STRING_HEIGHT;
    Rect.v_Left = WINKS_INFOBOX_STRING_X;
    Rect.v_Right = WINKS_INFOBOX_STRING_X + WINKS_INFOBOX_STRING_WIDTH;
    Rect.v_Top = WINKS_INFOBOX_STRING_Y;
    winks_ui_drawGraphicFromFile(WINKS_INFOBOX_X ,WINKS_INFOBOX_Y , Winks_UIFM_GetImage(WINKS_INFOBOX_BGICON));//(p_infobox->v_bgicon)
    WINKS_SET_TEXTCOLOR(WINKS_PIXEL_BLACK);
    //winks_ui_DrawstringInRect(Winks_UIFM_GetString(p_infobox->v_InfoboxStringId), &Rect, TextAlign, strlen(Winks_UIFM_GetString(p_infobox->v_InfoboxStringId)));

    winks_creat_text_string_rect_type(&Winks_UIContext->text_String_rect_infobox, &Rect, TextAlign, text, (int )winks_strlen_ucs2_type(text));
    winks_show_string_in_rect(&Winks_UIContext->text_String_rect_infobox, &Rect, TextAlign);
}

/**************************************************************

	FUNCTION NAME		: winks_InfoBox_s

  	PURPOSE		        : draw a Infobox.

	INPUT PARAMETERS	: winks_InfoBox_s *p_infobox
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void  winks_ui_drawImage_error(int x ,int y ,int width ,int height,char * text)
{	
    winks_Rect_s Rect;
    WINKS_SHOW_FLAG TextAlign = 0;
    //winks_text_String_rect_s text_String_rect_temp;

    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()
    
    if (winks_draw_condition(WINKS_MINGUI_HWND_USER) != WINKS_RETURN_SUCCESS)
    {
        return;
    }
 
    winks_set_font(WINKS_MEDIUM_FONT);
    
    TextAlign = WINKS_SHOW_CENTER;
    Rect.v_Bottom = y + height;
    Rect.v_Left = x;
    Rect.v_Right = x + width;
    Rect.v_Top = y;
	
    WINKS_SET_TEXTCOLOR(WINKS_PIXEL_RED);
    //winks_ui_DrawstringInRect(Winks_UIFM_GetString(p_infobox->v_InfoboxStringId), &Rect, TextAlign, strlen(Winks_UIFM_GetString(p_infobox->v_InfoboxStringId)));

    winks_creat_text_string_rect_type(&Winks_UIContext->text_String_rect_infobox, &Rect, TextAlign, text, (int )winks_strlen_ucs2_type(text));
    winks_show_string_in_rect(&Winks_UIContext->text_String_rect_infobox, &Rect, TextAlign);
}

/**************************************************************

	FUNCTION NAME		: winks_ui_show_colorcall_time

  	PURPOSE		        : show colorcall time.

	INPUT PARAMETERS	: char * ptime_string
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void winks_ui_show_colorcall_time(char * ptime_string)
{
    winks_Rect_s v_Rtc;
    int w = 0, h = 0;
	int i = 0;
    
    char temp[18] = {0};
    
    const char call_time[9] = "00:00:00";//{'0',0,'0',0,':',0,'0',0,'0',0,':',0,'0',0,'0',0,0,0};//L"00:00:00";
    Winks_Colorcall_S * p_temp;	
	int x,y;
	WINKS_UI_CONTEXT_DEF()
		
	WINKS_UI_CONTEXT_INIT()
    if (winks_draw_condition(WINKS_MINGUI_HWND_CALL) != WINKS_RETURN_SUCCESS)
    {
        return;
    }
	//WINKS_BUFFER_ON;
	
	
    //winks_set_font(WINKS_MEDIUM_FONT);
    
    //w = (winks_strlen_ucs2_type(ptime_string) >> 1) * WINKS_GETCHARWIDTH('0'); 

    //Winks_mem_set(temp, 0, sizeof(temp));
    Winks_Ascii2UnicodeString(call_time, temp, sizeof(temp));
    //Winks_mem_cpy(temp, call_time, winks_strlen_ucs2_type(ptime_string));
	
    //winks_get_string_wight_high(ptime_string,winks_strlen_ucs2_type(ptime_string),&w,&h);
    
    Winks_mem_set(&v_Rtc, 0, sizeof(v_Rtc));
	p_temp = (Winks_UIContext->Winks_Colorcall_head).head;
	x  = 0;
	y = 0;
	for ( i = 0 ; i < (Winks_UIContext->Winks_Colorcall_head).total_number; i++)  
	//for (p_temp = (Winks_UIContext->Winks_Colorcall_head).head;p_temp != NULL;p_temp->P_next) 
	{

		if ( p_temp[i].Colorcall_part.content_type == WINKS_TIME_TYPE)
		{
			WINKS_BUFFER_ON;
			#if 1

			v_Rtc.v_Top = y;//p_temp[i].Colorcall_part.y
			v_Rtc.v_Bottom =v_Rtc.v_Top + (int)(WINKS_SCREEN_HEIGHT_UI * ((p_temp[i].Colorcall_part.height) / (float)100));
			v_Rtc.v_Left= x; //p_temp[i].Colorcall_part.x;
			v_Rtc.v_Right = v_Rtc.v_Left + (int)(WINKS_SCREEN_WIDTH_UI * ((p_temp[i].Colorcall_part.width) / (float)100));
			
			winks_clear_time_fillRect(v_Rtc.v_Left,v_Rtc.v_Top + 1,v_Rtc.v_Right,v_Rtc.v_Bottom);
			WINKS_SET_TEXTCOLOR(p_temp[i].Colorcall_part.text_color);
			Winks_set_font_feature(&(p_temp[i].Colorcall_part.font_feature));
			 winks_get_string_wight_high(temp,winks_strlen_ucs2_type(temp),&w,&h);
			//winks_ui_textOutlen(v_Rtc.v_Left + WINKS_COLORCALL_TIME_GAP,v_Rtc.v_Top , ptime_string , winks_strlen_ucs2_type(ptime_string));
			Winks_showtext_in_area (ptime_string,winks_strlen_ucs2_type(ptime_string) ,v_Rtc.v_Left, v_Rtc.v_Top, v_Rtc.v_Right- v_Rtc.v_Left,v_Rtc.v_Bottom - v_Rtc.v_Top);
			#endif
			//winks_ui_drawcolorcall_part(x,y,&(p_temp[i].Colorcall_part));
			WINKS_BUFFER_OFF;
			//Winks_bitblt(&v_Rtc);
			return;
		}
		if ((i == (Winks_UIContext->Winks_Colorcall_head).total_number -1)|| ((i < (Winks_UIContext->Winks_Colorcall_head).total_number -1) && (p_temp[i + 1].Colorcall_part.position_in_screen[0] > p_temp[i ].Colorcall_part.position_in_screen[0] )))
		{
	
			x = 0;
			y = y + (int)(WINKS_SCREEN_HEIGHT_UI * ((p_temp[i].Colorcall_part.height) / (float)100));
			
		}
		else
		{
			x = x + (int)(WINKS_SCREEN_WIDTH_UI * ((p_temp[i].Colorcall_part.width) / (float)100));
		}
   
	}
   #if 0
    v_Rtc.v_Bottom = WINKS_COLORCALL_NAME_HEIGHT;
    v_Rtc.v_Left= WINKS_CLIENT_WIDTH - w - (WINKS_COLORCALL_TIME_GAP << 1);
	
    v_Rtc.v_Right = WINKS_CLIENT_WIDTH;
    v_Rtc.v_Top = WINKS_COLORCALL_TIME_Y;
	
    winks_clear_time_fillRect(v_Rtc.v_Left,v_Rtc.v_Top,v_Rtc.v_Right,v_Rtc.v_Bottom);
    WINKS_SET_TEXTCOLOR(WINKS_PIXEL_BLACK);
    winks_ui_textOutlen(v_Rtc.v_Left + WINKS_COLORCALL_TIME_GAP,v_Rtc.v_Top , ptime_string , winks_strlen_ucs2_type(ptime_string));
    //Winks_Refresh(v_Rtc);
    //Winks_bitblt(&v_Rtc);
    #endif
	
}


/**************************************************************

	FUNCTION NAME		: winks_ui_drawcolorcall

  	PURPOSE		        : draw colorcall.

	INPUT PARAMETERS	: Winks_Colorcall_S *p_infobox
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/
#if 0
void  winks_ui_drawcolorcall(Winks_Colorcall_S *p_colorcall)
{
    //static winks_scrolling_text_s  s;
	//static int flag = 0;
    int w = 0, h = 0;
    int excursion = 0;
    int wight_excursion = 0;
    int result = 0;
	
    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    Winks_assert(p_colorcall != NULL);
    
    if (winks_draw_condition(WINKS_MINGUI_HWND_CALL) != WINKS_RETURN_SUCCESS)
    {
        return;
    }

    winks_set_font(WINKS_MEDIUM_FONT);
    
    winks_get_string_wight_high(p_colorcall->v_greet,winks_strlen_ucs2_type(p_colorcall->v_greet),&w,&h);
    winks_clear_screen_fillRect(WINKS_COLORCALL_NAME_X,WINKS_COLORCALL_NAME_Y,WINKS_CLIENT_WIDTH,WINKS_COLORCALL_NAME_HEIGHT);
    WINKS_SET_TEXTCOLOR(WINKS_PIXEL_LIGHTWHITE);
    if (winks_strlen_ucs2_type(p_colorcall->v_nanum) <= (WINKS_MAX_TEL_NUMBER << 1))
        winks_ui_textOutlen(WINKS_COLORCALL_NAME_X, WINKS_COLORCALL_NAME_Y, p_colorcall->v_nanum,winks_strlen_ucs2_type(p_colorcall->v_nanum) );
    else
    {
        
        char name[(WINKS_MAX_TEL_NUMBER + 1) << 1];
        Winks_mem_set(name, 0, ((WINKS_MAX_TEL_NUMBER + 1)<< 1));
        Winks_mem_cpy(name, p_colorcall->v_nanum, ((WINKS_MAX_TEL_NUMBER - 3) << 1));
        Winks_mem_cpy(name + ((WINKS_MAX_TEL_NUMBER - 3) << 1),WINKS_STRCAT_SIGN,8);
        winks_ui_textOutlen(WINKS_COLORCALL_NAME_X, WINKS_COLORCALL_NAME_Y, name,winks_strlen_ucs2_type(name) );
    }
    WINKS_SET_TEXTCOLOR(WINKS_PIXEL_BLACK);
    //winks_ui_textOutlen(WINKS_COLORCALL_TIME_X,WINKS_COLORCALL_TIME_Y , p_colorcall->v_calltime,winks_strlen_ucs2_type(p_colorcall->v_calltime) );
    //Winks_mem_cpy(p_colorcall->v_filename,WINKS_ROOT_PATH"winks\\image\\4.gif",strlen(WINKS_ROOT_PATH"winks\\image\\4.gif"));
	//*(p_colorcall->v_filename + strlen(WINKS_ROOT_PATH"winks\\image\\4.gif")) = 0;

    Winks_assert(p_colorcall->v_filename != NULL);

    WINKS_CLIP_ON(WINKS_COLORCALL_PIC_X,WINKS_COLORCALL_PIC_Y, WINKS_COLORCALL_PIC_WIGHT, WINKS_COLORCALL_PIC_Y + WINKS_COLORCALL_PIC_HEIGHT);
  #if 0         
           
    if (p_colorcall->v_ImageCode == E_IMAGE_GIF)
    {
        if (winks_anim_frame_count(p_colorcall->v_filename) == 1)
        {
            //anim_flag = -1;
            winks_set_anim_display_count( -1);
            //winks_image_draw_animation(WINKS_COLORCALL_PIC_X,WINKS_COLORCALL_PIC_Y,p_colorcall->v_filename,0,WINKS_COLORCALL_PIC_WIGHT,WINKS_COLORCALL_PIC_HEIGHT);
            result = winks_ui_drawGraphic_uc_file_and_cache(WINKS_COLORCALL_PIC_X,WINKS_COLORCALL_PIC_Y,p_colorcall->v_filename,WINKS_COLORCALL_PIC_WIGHT,WINKS_COLORCALL_PIC_HEIGHT);
            if(result == WINKS_IMAGE_TOO_LARGE)
            {
               winks_ui_drawImage_error(WINKS_COLORCALL_PIC_X,WINKS_COLORCALL_PIC_Y, WINKS_COLORCALL_PIC_WIGHT, WINKS_COLORCALL_PIC_HEIGHT, (char * )Winks_UIFM_GetString(WINKS_UI_DRAW_IMAGE_ERROR_STRINGID));
            }

        }
        else
			#endif
        {
            //anim_flag = 0;
            winks_set_anim_display_count( 0);
            //winks_image_draw_animation(30, 110,WINKS_ROOT_PATH"winks\\image\\a.gif", 0);
            winks_image_draw_animation(WINKS_COLORCALL_PIC_X,WINKS_COLORCALL_PIC_Y,p_colorcall->v_filename, 0, WINKS_COLORCALL_PIC_WIGHT, WINKS_COLORCALL_PIC_HEIGHT);
            winks_anim_set_last_frame_callback();
        }
			#if 0
    }
	
    else
    {
        //anim_flag = -1;
        winks_set_anim_display_count( -1);
        //winks_image_draw_animation(WINKS_COLORCALL_PIC_X,WINKS_COLORCALL_PIC_Y,p_colorcall->v_filename, 0, WINKS_COLORCALL_PIC_WIGHT,WINKS_COLORCALL_PIC_HEIGHT);
        result = winks_ui_drawGraphic_uc_file_and_cache(WINKS_COLORCALL_PIC_X,WINKS_COLORCALL_PIC_Y,p_colorcall->v_filename,WINKS_COLORCALL_PIC_WIGHT,WINKS_COLORCALL_PIC_HEIGHT);
        if(result == WINKS_IMAGE_TOO_LARGE)
        {
           winks_ui_drawImage_error(WINKS_COLORCALL_PIC_X,WINKS_COLORCALL_PIC_Y, WINKS_COLORCALL_PIC_WIGHT, WINKS_COLORCALL_PIC_HEIGHT, (char * )Winks_UIFM_GetString(WINKS_UI_DRAW_IMAGE_ERROR_STRINGID));
        }

    }
	#endif
    WINKS_CLIP_OFF(WINKS_COLORCALL_PIC_X,WINKS_COLORCALL_PIC_Y, WINKS_COLORCALL_PIC_WIGHT, WINKS_COLORCALL_PIC_Y + WINKS_COLORCALL_PIC_HEIGHT);
   
    if(w <= WINKS_COLORCALL_GREETSTRING_WIGHT)
    {
        excursion = (WINKS_COLORCALL_GREETSTRING_HEIGHT - WINKS_GETFONTHEIGHT) >> 1;
        wight_excursion = (WINKS_COLORCALL_GREETSTRING_WIGHT - w) >> 1;
        winks_ui_textOutlen(WINKS_COLORCALL_GREETSTRING_X + wight_excursion, WINKS_COLORCALL_GREETSTRING_Y + excursion, p_colorcall->v_greet,winks_strlen_ucs2_type(p_colorcall->v_greet) );
    }
    else
    {
        if(Winks_UIContext->scrooltext_flag == 0) 
        {
            winks_ui_create_scrolling_text(
            &Winks_UIContext->winks_scroll_string_colorcall,
            WINKS_COLORCALL_GREETSTRING_X,
            WINKS_COLORCALL_GREETSTRING_Y,
            WINKS_COLORCALL_GREETSTRING_WIGHT,
            WINKS_COLORCALL_GREETSTRING_HEIGHT,
            p_colorcall->v_greet,
            winks_ui_handle_scrolling_text_colorcall,
            //void (*draw_background) (int x1, int y1, int x2, int y2),
            WINKS_PIXEL_BLACK,
            WINKS_PIXEL_TRANSPARENT,
            
            NULL
            //winks_color border_color
            );


            //winks_ui_redraw_scrolling_text(&s);
            Winks_UIContext->scrooltext_flag = 1;
            winks_ui_scrolling_text_start(&Winks_UIContext->winks_scroll_string_colorcall);
        }
            winks_ui_redraw_scrolling_text(&Winks_UIContext->winks_scroll_string_colorcall);
    }
    winks_ui_Create_Colorcall_RightSoftKey(WINKS_UI_COLORDISPLAY_EXIT_STRINGID);
}

#endif
/**************************************************************

	FUNCTION NAME		: winks_ui_drawDIYPIC

  	PURPOSE		        : draw DIYPIC.

	INPUT PARAMETERS	: winks_DIY_colorcall_s *p_infobox
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void  winks_ui_drawDIYPIC(winks_DIY_colorcall_s *p_diy_colorcall)
{
    //static int flag = 0;
    //int w = 0, h = 0;
    ///int excursion = 0;
    int result = 0;

    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    Winks_assert(p_diy_colorcall != NULL);

    winks_set_font(WINKS_MEDIUM_FONT);
    
    if (winks_draw_condition(WINKS_MINGUI_HWND_USER) != WINKS_RETURN_SUCCESS)
    {
        return;
    }
    WINKS_CLIP_ON(WINKS_DIY_PIC_X,WINKS_DIY_PIC_Y, WINKS_DIY_PIC_WIDTH, WINKS_DIY_PIC_Y + WINKS_DIY_PIC_HEIGHT);
    result = winks_ui_drawGraphicFromFile_ucstype_filename(WINKS_DIY_PIC_X,WINKS_DIY_PIC_Y,p_diy_colorcall->v_filename, WINKS_DIY_PIC_WIDTH,WINKS_DIY_PIC_HEIGHT);
    if(result == WINKS_IMAGE_TOO_LARGE)
    {
        winks_ui_drawImage_error(WINKS_DIY_PIC_X,WINKS_DIY_PIC_Y, WINKS_DIY_PIC_WIDTH, WINKS_DIY_PIC_HEIGHT, (char * )Winks_UIFM_GetString(WINKS_UI_DRAW_IMAGE_ERROR_STRINGID));
    }
    WINKS_CLIP_OFF(WINKS_DIY_PIC_X,WINKS_DIY_PIC_Y, WINKS_DIY_PIC_WIDTH, WINKS_DIY_PIC_Y + WINKS_DIY_PIC_HEIGHT);
#if 0 //去掉问候语
    //WINKS_CLIP_ON(WINKS_COLORCALL_PIC_X,WINKS_COLORCALL_PIC_Y, WINKS_COLORCALL_PIC_WIGHT, WINKS_COLORCALL_PIC_HEIGHT);
    winks_get_string_wight_high(p_diy_colorcall->v_greetsting,winks_strlen_ucs2_type(p_diy_colorcall->v_greetsting),&w,&h);
    excursion = (WINKS_DIY_GREETSTRING_HEIGHT - WINKS_GETFONTHEIGHT) >> 1;
    winks_ui_drawRect(WINKS_DIY_GREETSTRING_X -1, WINKS_DIY_GREETSTRING_Y - excursion -1, WINKS_DIY_GREETSTRING_X + WINKS_DIY_GREETSTRING_WIGHT+1, WINKS_DIY_GREETSTRING_Y + WINKS_DIY_GREETSTRING_HEIGHT - excursion + 1,WINKS_PIXEL_BLACK);
    winks_ui_fillRect(WINKS_DIY_GREETSTRING_X, WINKS_DIY_GREETSTRING_Y - excursion, WINKS_DIY_GREETSTRING_X + WINKS_DIY_GREETSTRING_WIGHT, WINKS_DIY_GREETSTRING_Y + WINKS_DIY_GREETSTRING_HEIGHT - excursion,WINKS_PIXEL_LIGHTWHITE);
    if(w <= WINKS_DIY_GREETSTRING_WIGHT)
    {
        
        WINKS_SET_TEXTCOLOR(WINKS_PIXEL_BLACK);
        winks_ui_textOutlen(WINKS_DIY_GREETSTRING_X,WINKS_DIY_GREETSTRING_Y, p_diy_colorcall->v_greetsting,winks_strlen_ucs2_type(p_diy_colorcall->v_greetsting) );
    }

    else       
    {
        if(Winks_UIContext->scrooltext_flag == 0) 
        {
            winks_ui_create_scrolling_text(
            &Winks_UIContext->winks_scroll_string,
            WINKS_DIY_GREETSTRING_X,
            WINKS_DIY_GREETSTRING_Y,
            WINKS_DIY_GREETSTRING_WIGHT,   
            WINKS_DIY_GREETSTRING_HEIGHT,
            p_diy_colorcall->v_greetsting,
            winks_ui_handle_scrolling_text,
            //void (*draw_background) (int x1, int y1, int x2, int y2),
            WINKS_PIXEL_BLACK,
            WINKS_PIXEL_LIGHTWHITE,
            NULL
            //winks_color border_color
            );

   
            //winks_ui_redraw_scrolling_text(&s);
            Winks_UIContext->scrooltext_flag = 1;
            winks_ui_scrolling_text_start(&Winks_UIContext->winks_scroll_string);
        }
        winks_ui_redraw_scrolling_text(&Winks_UIContext->winks_scroll_string);
    }
#endif
}



/**************************************************************

	FUNCTION NAME		: winks_ui_UpdateRect

  	PURPOSE		        : Update a Rect.

	INPUT PARAMETERS	: winks_Rect_s v_freshlcd
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void winks_ui_updateRect (winks_Rect_s* v_freshlcd)
{		
    //BitBlt (winks_hdc, v_freshlcd->v_Left, v_freshlcd->v_Top, v_freshlcd->v_Right, v_freshlcd->v_Bottom,winks_current_hdc, v_freshlcd->v_Left, v_freshlcd->v_Top, 0);
    //Winks_bitblt( v_freshlcd );
}


/**************************************************************

	FUNCTION NAME		: winks_ui_drawpanel

  	PURPOSE		        : draw a panel.

	INPUT PARAMETERS	: winks_Panel_s *p_Panel
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void winks_ui_drawpanel()
{	
    winks_ui_drawGraphicFromFile(0,0, Winks_UIFM_GetImage(WINKS_UI_MAINMENUBG_IMAGEID));
    //winks_Panel_s  p_Panel;
    //winks_Set_panel_icon(p_Panel ->IconID);
    //winks_ui_fillRect(p_Panel->v_panel.v_Left,p_Panel->v_panel.v_Top,p_Panel->v_panel.v_Right,p_Panel->v_panel.v_Bottom, p_Panel ->v_BgColor);
    //winks_ui_drawGraphic(p_Panel ->p_Panel ->v_panel, p_Panel ->v_BgIconID);
    //winks_ui_drawGraphicFromFile(p_Panel->v_panel.v_Left ,p_Panel->v_panel.v_Top , Winks_UIFM_GetImage(p_Panel->v_BgIconID));
}


/**************************************************************

	FUNCTION NAME		: winks_ui_drawuserFrame

  	PURPOSE		        : draw a userFrame.

	INPUT PARAMETERS	: winks_UserFrame_s *p_userframe
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void winks_ui_drawuserFrame(winks_UserFrame_s *p_userframe)
{
    //winks_Set_userFrame_icon(p_userframe->IconID);	
    //	winks_ui_fillRect(p_userframe->v_userframe.v_Left,p_userframe->v_userframe.v_Top,p_userframe->v_userframe.v_Right,p_userframe->v_userframe.v_Bottom, p_userframe->v_BgColor);
    //	winks_ui_drawGraphicFromFile(p_userframe->v_userframe.v_Left,p_userframe->v_userframe.v_Top, Winks_UIFM_GetImage(p_userframe->v_BgIconID));
}


/**************************************************************

	FUNCTION NAME		: winks_ui_createtitle

  	PURPOSE		        : create a title.

	INPUT PARAMETERS	: winks_Title_s *p_Title
					      int stringid
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void winks_ui_createtitle(int stringid)
{  
    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()
    //winks_Title_s p_Title;
    (Winks_UIContext->g_Title).v_BgColorID =0;
    //p_Title->v_color = ;
    Winks_UIContext->g_Title.v_IconID = 0;
    //p_Title.v_icondigit
    Winks_UIContext->g_Title.v_TitleStringID = stringid;
    winks_ui_drawtitle(&Winks_UIContext->g_Title);

}


/**************************************************************

	FUNCTION NAME		: winks_ui_drawtitle

  	PURPOSE		        : draw a title.

	INPUT PARAMETERS	: winks_Title_s *p_Title
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void winks_ui_drawtitle(winks_Title_s *p_Title)
{	
    int w,h;
    //int string_x = 0;
    winks_set_font(WINKS_MEDIUM_FONT);
    
    /*gdi_draw_solid_rect(MMI_title_x, 0, MMI_title_x + MMI_title_width - 1, MMI_title_height - 1, WINKS_COLOR_TRANSPARENT); */
    if (p_Title->v_BgColorID != 0)
    {
        winks_ui_drawGraphicFromFile(0, 0, Winks_UIFM_GetImage(p_Title->v_BgColorID));
    }
    else
    {
        winks_clear_screen_fillRect(WINKS_TITLE_STRING_X,WINKS_TITLE_ICON_Y,WINKS_TITLE_STRING_X + WINKS_CLIENT_WIDTH,WINKS_TITLE_HEIGHT);
        /*	winks_ui_fillRect(WINKS_TITLE_BGICON_X,
        WINKS_TITLE_BGICON_Y,
        WINKS_TITLE_BGICON_X + WINKS_TITLE_WIGHT - 1,
        WINKS_TITLE_BGICON_Y + WINKS_TITLE_HEIGHT - 1,p_Title->v_color);*/
    }
    if(p_Title->v_IconID != 0)
    {
        winks_ui_drawGraphicFromFile(WINKS_TITLE_ICON_X, WINKS_TITLE_ICON_Y, Winks_UIFM_GetImage(p_Title->v_IconID));
    }
    WINKS_SET_TEXTCOLOR(WINKS_PIXEL_LIGHTWHITE);
    winks_get_string_wight_high((char *)Winks_UIFM_GetString(p_Title->v_TitleStringID),winks_strlen_ucs2_type((char *)Winks_UIFM_GetString(p_Title->v_TitleStringID)),&w,&h);
    if (w <= (WINKS_CLIENT_WIDTH - WINKS_TITLE_STRING_X ))
    {
        winks_ui_textOutlen(WINKS_TITLE_STRING_X,WINKS_TITLE_STRING_Y,Winks_UIFM_GetString(p_Title->v_TitleStringID),winks_strlen_ucs2_type((char *)Winks_UIFM_GetString(p_Title->v_TitleStringID)));
    }
    else
    {
        winks_ui_textOutlen((WINKS_CLIENT_WIDTH - w),WINKS_TITLE_STRING_Y,Winks_UIFM_GetString(p_Title->v_TitleStringID),winks_strlen_ucs2_type((char *)Winks_UIFM_GetString(p_Title->v_TitleStringID)));
    }
    //winks_ui_textOutlen(WINKS_TITLE_DIGIT_X,WINKS_TITLE_DIGIT_Y,itoa(p_Title->v_icondigit),strlen(itoa(p_Title->v_icondigit)));
}


/**************************************************************

	FUNCTION NAME		: winks_ui_DrawSoftKey

  	PURPOSE		        : draw a SoftKey.

	INPUT PARAMETERS	: winks_Softkey_s *p_Softkey
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void winks_ui_DrawSoftKey(winks_Softkey_s *p_Softkey)
{	
    // int 
    //set_softkey_label(UI_string_type s, winks_WGUI_SOFTKEY_ENUM key);
    //set_softkey_icon(PU8 icon, winks_WGUI_SOFTKEY_ENUM key);
    //show_softkey(winks_WGUI_SOFTKEY_ENUM key);*/
 

    winks_set_font(WINKS_MEDIUM_FONT);
    
    if(p_Softkey->v_stringId != 0)
    {   
        WINKS_BUFFER_ON;
        //Winks_printf( "winks_ui_DrawSoftKey, x %d, y %d", p_Softkey->v_string_x, p_Softkey->v_string_y );
#ifdef __WINKS_STRINGBORDER__
        WINKS_SET_TEXTCOLOR(WINKS_PIXEL_BLACK);
        //winks_ui_drawIconButton(&p_Softkey->v_button);
        
        winks_clear_screen_fillRect(p_Softkey->v_string_x - WINKS_SOFTKEY_STRING_EXCURSION_X,  p_Softkey->v_button.y,p_Softkey->v_string_x + WINKS_LEFTSOFTKEY_BUTTON_WIDTH,p_Softkey->v_string_y + WINKS_SOFTKEYBAR_HEIGHT);
        if (p_Softkey->v_softkey_flag == WINKS_Softkey_SHOW_DOWN)
        {

            p_Softkey->v_softkey_flag = WINKS_Softkey_SHOW_UP;
            Winks_UI_StringBordered(  p_Softkey->v_string_x + WINKS_SOFTKEY_STRING_EXCURSION_X, p_Softkey->v_string_y + WINKS_SOFTKEY_STRING_EXCURSION_Y, WINKS_PIXEL_LIGHTWHITE, Winks_UIFM_GetString(p_Softkey->v_stringId), WINKS_GETFONTHEIGHT );
           // winks_ui_textOutlen( p_Softkey->v_string_x, p_Softkey->v_string_y + WINKS_SOFTKEY_STRING_EXCURSION_Y, Winks_UIFM_GetString(p_Softkey->v_stringId), winks_strlen_ucs2_type((char *)Winks_UIFM_GetString(p_Softkey->v_stringId)));
        }
        else
        {
            Winks_UI_StringBordered(  p_Softkey->v_string_x, p_Softkey->v_string_y, WINKS_PIXEL_LIGHTWHITE, Winks_UIFM_GetString(p_Softkey->v_stringId), WINKS_GETFONTHEIGHT );
            //winks_ui_textOutlen( p_Softkey->v_string_x, p_Softkey->v_string_y, Winks_UIFM_GetString(p_Softkey->v_stringId), winks_strlen_ucs2_type((char *)Winks_UIFM_GetString(p_Softkey->v_stringId)));
        }
#else
    WINKS_SET_TEXTCOLOR(WINKS_PIXEL_LIGHTWHITE);
    winks_clear_screen_fillRect(p_Softkey->v_string_x,p_Softkey->v_string_y,p_Softkey->v_string_x + WINKS_LEFTSOFTKEY_BUTTON_WIDTH,p_Softkey->v_string_y + WINKS_SOFTKEYBAR_HEIGHT);
    if (p_Softkey->v_softkey_flag == WINKS_Softkey_SHOW_DOWN)
    {

        p_Softkey->v_softkey_flag = WINKS_Softkey_SHOW_UP;
        winks_ui_textOutlen(  p_Softkey->v_string_x + WINKS_SOFTKEY_STRING_EXCURSION_X, p_Softkey->v_string_y + WINKS_SOFTKEY_STRING_EXCURSION_Y, Winks_UIFM_GetString(p_Softkey->v_stringId), winks_strlen_ucs2_type((char *)Winks_UIFM_GetString(p_Softkey->v_stringId)));
        // winks_ui_textOutlen( p_Softkey->v_string_x, p_Softkey->v_string_y + WINKS_SOFTKEY_STRING_EXCURSION_Y, Winks_UIFM_GetString(p_Softkey->v_stringId), winks_strlen_ucs2_type((char *)Winks_UIFM_GetString(p_Softkey->v_stringId)));
    }
    else
    {
        winks_ui_textOutlen(  p_Softkey->v_string_x, p_Softkey->v_string_y, Winks_UIFM_GetString(p_Softkey->v_stringId), winks_strlen_ucs2_type((char *)Winks_UIFM_GetString(p_Softkey->v_stringId)));
        //winks_ui_textOutlen( p_Softkey->v_string_x, p_Softkey->v_string_y, Winks_UIFM_GetString(p_Softkey->v_stringId), winks_strlen_ucs2_type((char *)Winks_UIFM_GetString(p_Softkey->v_stringId)));
    }
#endif
        WINKS_BUFFER_OFF;
    }
    }

/**************************************************************

	FUNCTION NAME		: winks_ui_DrawSoftKey

  	PURPOSE		        : draw a SoftKey.

	INPUT PARAMETERS	: winks_Softkey_s *p_Softkey
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null
***************************************************************/
void winks_ui_DrawSoftKey_lsk_down()
{	
    // int 
    //set_softkey_label(UI_string_type s, winks_WGUI_SOFTKEY_ENUM key);
    //set_softkey_icon(PU8 icon, winks_WGUI_SOFTKEY_ENUM key);
    //show_softkey(winks_WGUI_SOFTKEY_ENUM key);*/
    winks_Softkey_s *p_Softkey = NULL;

    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    
    
    p_Softkey = &Winks_UIContext->winks_left_softkey;
    p_Softkey->v_softkey_flag = WINKS_Softkey_SHOW_DOWN;
    winks_ui_DrawSoftKey(p_Softkey);
}

/**************************************************************

	FUNCTION NAME		: winks_ui_DrawSoftKey

  	PURPOSE		        : draw a SoftKey.

	INPUT PARAMETERS	: winks_Softkey_s *p_Softkey
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null
***************************************************************/
void winks_ui_DrawSoftKey_rsk_down()
{	
    // int 
    //set_softkey_label(UI_string_type s, winks_WGUI_SOFTKEY_ENUM key);
    //set_softkey_icon(PU8 icon, winks_WGUI_SOFTKEY_ENUM key);
    //show_softkey(winks_WGUI_SOFTKEY_ENUM key);*/
    winks_Softkey_s *p_Softkey = NULL;

    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    
    
    p_Softkey = &Winks_UIContext->winks_right_softkey;
    p_Softkey->v_softkey_flag = WINKS_Softkey_SHOW_DOWN;
    winks_ui_DrawSoftKey(p_Softkey);
}

/**************************************************************

	FUNCTION NAME		: winks_ui_CreateLeftSoftKey

  	PURPOSE		        : Create a LeftSoftKey.

	INPUT PARAMETERS	: int stringid
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void winks_ui_CreateLeftSoftKey(int stringid)
{
    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    if (stringid == 0)
        return;
    Winks_UIContext->winks_left_softkey.v_stringId = stringid;
    Winks_UIContext->winks_left_softkey.v_string_x = WINKS_LEFTSOFTKEY_STRING_X;
    Winks_UIContext->winks_left_softkey.v_string_y = WINKS_SOFTKEY_STRING_Y;
    Winks_UIContext->winks_left_softkey.v_button.focussed_down_color= 0;
    Winks_UIContext->winks_left_softkey.v_button.height = 0;
    Winks_UIContext->winks_left_softkey.v_button.icon_x = 0;
    Winks_UIContext->winks_left_softkey.v_button.icon_y = 0;
    Winks_UIContext->winks_left_softkey.v_button.normal_color = 0;
    Winks_UIContext->winks_left_softkey.v_button.normal_up_icon = 0;
    Winks_UIContext->winks_left_softkey.v_button.width = 0;
    Winks_UIContext->winks_left_softkey.v_button.x = 0;
    Winks_UIContext->winks_left_softkey.v_button.y = WINKS_LEFTSOFTKEY_BUTTON_Y;
    Winks_UIContext->winks_left_softkey.v_softkey_flag = WINKS_Softkey_SHOW_UP;
    winks_ui_DrawSoftKey(&Winks_UIContext->winks_left_softkey);	


}


/**************************************************************

	FUNCTION NAME		: winks_ui_CreateRightSoftKey

  	PURPOSE		        : Create a RightSoftKey.

	INPUT PARAMETERS	:int stringid
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void winks_ui_CreateRightSoftKey(int stringid)
{
    int w, h;
    int stlen = 0;
    //char ptmp[32];
    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()
    
    if (stringid == 0)
    return;
    stlen = winks_strlen_ucs2_type((char *)Winks_UIFM_GetString(stringid));
    //gb2312tounicode( ptmp, 32, Winks_UIFM_GetString(stringid), stlen );
    winks_get_string_wight_high( (char * )Winks_UIFM_GetString(stringid), stlen, &w, &h );
    //winks_get_string_wight_high(Winks_UIFM_GetString(stringid),strlen(Winks_UIFM_GetString(stringid)),&w,&h);
    //Winks_printf( "winks_ui_CreateRightSoftKey w %d, h %d\r\n", w, h );
    Winks_UIContext->winks_right_softkey.v_stringId = stringid;
    Winks_UIContext->winks_right_softkey.v_string_x = WINKS_SOFTKEYBAR_WIDTH - WINKS_LEFTSOFTKEY_STRING_X - w ;
    Winks_UIContext->winks_right_softkey.v_string_y = WINKS_SOFTKEY_STRING_Y;
    Winks_UIContext->winks_right_softkey.v_button.focussed_down_color= 0 ;
    Winks_UIContext->winks_right_softkey.v_button.height = 0;
    Winks_UIContext->winks_right_softkey.v_button.icon_x = 0;
    Winks_UIContext->winks_right_softkey.v_button.icon_y = 0;
    Winks_UIContext->winks_right_softkey.v_button.normal_color = 0;
    Winks_UIContext->winks_right_softkey.v_button.normal_up_icon = 0 ;
    Winks_UIContext->winks_right_softkey.v_button.width = 0;
    Winks_UIContext->winks_right_softkey.v_button.x = 0;
    Winks_UIContext->winks_right_softkey.v_button.y = WINKS_LEFTSOFTKEY_BUTTON_Y;
    Winks_UIContext->winks_right_softkey.v_softkey_flag = WINKS_Softkey_SHOW_UP;
    winks_ui_DrawSoftKey(&Winks_UIContext->winks_right_softkey);	
}


/**************************************************************

	FUNCTION NAME		: winks_ui_Create_Colorcall_RightSoftKey

  	PURPOSE		        : Create a RightSoftKey.

	INPUT PARAMETERS	:int stringid
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void winks_ui_Create_Colorcall_RightSoftKey(int stringid)
{
    int w, h;
    int stlen = 0;
    //char ptmp[32];
    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()
    stlen = winks_strlen_ucs2_type((char *)Winks_UIFM_GetString(stringid));
    //gb2312tounicode( ptmp, 32, Winks_UIFM_GetString(stringid), stlen );
    winks_get_string_wight_high( (char * )Winks_UIFM_GetString(stringid), stlen, &w, &h );
    //winks_get_string_wight_high(Winks_UIFM_GetString(stringid),strlen(Winks_UIFM_GetString(stringid)),&w,&h);
    //Winks_printf( "winks_ui_CreateRightSoftKey w %d, h %d\r\n", w, h );
    Winks_UIContext->winks_right_softkey.v_stringId = stringid;
    Winks_UIContext->winks_right_softkey.v_string_x = WINKS_SOFTKEYBAR_WIDTH - w - WINKS_LEFTSOFTKEY_STRING_X ;
    Winks_UIContext->winks_right_softkey.v_string_y = WINKS_COLORCALL_SOFTKEY_STRING_Y;
    Winks_UIContext->winks_right_softkey.v_button.focussed_down_color= 0 ;
    Winks_UIContext->winks_right_softkey.v_button.height = 0;
    Winks_UIContext->winks_right_softkey.v_button.icon_x = 0;
    Winks_UIContext->winks_right_softkey.v_button.icon_y = 0;
    Winks_UIContext->winks_right_softkey.v_button.normal_color = 0;
    Winks_UIContext->winks_right_softkey.v_button.normal_up_icon = 0 ;
    Winks_UIContext->winks_right_softkey.v_button.width = 0;
    Winks_UIContext->winks_right_softkey.v_button.x = 0;
    Winks_UIContext->winks_right_softkey.v_button.y = WINKS_COLORCALL_SOFTKEY_STRING_Y;
    Winks_UIContext->winks_right_softkey.v_softkey_flag = WINKS_Softkey_SHOW_UP;
    winks_ui_DrawSoftKey(&Winks_UIContext->winks_right_softkey);	
}
/**************************************************************

	FUNCTION NAME		: winks_ui_DrawSkFrame

  	PURPOSE		        : draw a SkFrame.

	INPUT PARAMETERS	: winks_SkFrame_s *p_Skframe
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void winks_ui_DrawSkFrame(void)
{	
    //winks_ui_fillRect(WINKS_LEFTSOFTKEY_BUTTON_X,WINKS_LEFTSOFTKEY_BUTTON_Y,WINKS_LEFTSOFTKEY_BUTTON_X + WINKS_SOFTKEYBAR_WIDTH,WINKS_LEFTSOFTKEY_BUTTON_Y + WINKS_SOFTKEYBAR_HEIGHT,WINKS_SOFTKEYBAR_COLOR);
    //winks_ui_DrawSoftKey(&Winks_UIContext->winks_left_softkey);
    //winks_ui_DrawSoftKey (&Winks_UIContext->winks_right_softkey);
    //winks_ui_DrawSoftKey(winks_Softkey_s *p_Softkey,winks_WGUI_SOFTKEY_ENUM key);
}



/**************************************************************

	FUNCTION NAME		: winks_ui_Drawstring

  	PURPOSE		        : draw a string.

	INPUT PARAMETERS	: winks_string_s * p_string
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void winks_ui_Drawstring(winks_string_s * p_string)
{	
    //winks_set_font(p_string->v_font);//SelectFont (hdc, logfontgb16);
    /*	WINKS_SET_TEXTCOLOR(p_string->v_font_color);//SetTextColor (hdc, PIXEL_black);
    WINKS_SET_BKMODE(p_string->bkmode);//SetBkMode (hdc, BM_TRANSPARENT); 
    WINKS_GETFONTHEIGHT;// GetFontHeight(HDC hdc);
    WINKS_GETMAXFONTWIDTH;// GetMaxFontWidth (HDC hdc)
    //TextOutLen (winks_hdc, x, y,  p_string->v_string, -1);// sizeoflen
    */
}



/**************************************************************

	FUNCTION NAME		: winks_ui_DrawWindowForm

  	PURPOSE		        : Draw a WindowForm.

	INPUT PARAMETERS	: short font
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void winks_ui_DrawWindowForm(void)
{	
    winks_UserFrame_s v_ClientArea;

    if (winks_draw_condition(WINKS_MINGUI_HWND_USER) != WINKS_RETURN_SUCCESS)
    {
        return;
    }

    v_ClientArea.v_BgColor = WINKS_PIXEL_RED;
    v_ClientArea.v_BgIconID = 0;
    v_ClientArea.v_userframe.v_Bottom =WINKS_CLIENT_Y + WINKS_CLIENT_HEIGHT;
    v_ClientArea.v_userframe.v_Left = WINKS_CLIENT_X;
    v_ClientArea.v_userframe.v_Right = WINKS_CLIENT_X + WINKS_CLIENT_WIDTH;
    v_ClientArea.v_userframe.v_Top = WINKS_CLIENT_Y;
    winks_ui_drawpanel();
    winks_ui_drawuserFrame(&v_ClientArea);
    winks_ui_DrawSkFrame();
    //winks_ui_drawtitle(&p_WindowForm->v_TitleArea);
    //winks_ui_createtitle(0);

}

#if 0
/**************************************************************

	FUNCTION NAME		: winks_ui_DrawCursor

  	PURPOSE		        : Draw a Cursor.

	INPUT PARAMETERS	: short font
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void winks_ui_DrawCursor(winks_Cursor_s * Cursor)
{	

    static	char UI_cursor_blink_flag = 0;
    if (UI_cursor_blink_flag)  
    {
        winks_ui_fillRect(Cursor->v_left,Cursor->v_top,Cursor->v_left + Cursor->v_wight ,Cursor->v_top + Cursor->v_HEIGHT, Cursor->v_color ); 
        UI_cursor_blink_flag = 0;
    }
    else
    {
        if(Cursor->v_GraphicID != 0)
        {
        	winks_ui_drawGraphicFromFile(Cursor->v_top,Cursor->v_left, Winks_UIFM_GetImage(Cursor->v_GraphicID));
        	//_show_bitmap(UI_cursor_x1, UI_cursor_y1, &UI_cursor_backup_bitmap);
        	UI_cursor_blink_flag = 1;
        }
    }
    //Timer();*/暂时放在外面

}

#endif
#if 0
/**************************************************************

	FUNCTION NAME		: winks_ui_DrawstringInRect

  	PURPOSE		        : Draw a string in the Rect.

	INPUT PARAMETERS	: const char  *s
    					  winks_Rect_s* pRect
    					  int TextAlign
    					  int MaxNumChars
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void winks_ui_DrawstringInRect( char  *s, winks_Rect_s* pRect, int TextAlign, int MaxNumChars) 
{
    winks_Rect_s r;
    char * temp;
    //		  int y = 0;
    //		  int NumLines;
    //const char  *sOrg =s;
    int FontYSize,FontCharSize;
    int  char_tap = 0;
    // int xLine = 0;
    int  string_x = 0;
    int  string_y = 0;
    int linewidth = 0;
    int lineheight = 0;
    // int LineLen = 0;
    // int NumCharsRem;           /* Number of remaining characters */
    int num_in_line = 0;
    // 	  int remain_number = 0;
    int linenumber = 0;
    char * text = (char  *)s;
    int textlen;
    char flag;
    TextAlign = 1;
    textlen = MaxNumChars;
    FontYSize = WINKS_GETFONTHEIGHT;
    FontCharSize = WINKS_GETFONTHEIGHT >> 1;
    lineheight = WINKS_GETFONTHEIGHT;
    linenumber = (pRect->v_Bottom - pRect->v_Top) / lineheight;
    linewidth = pRect->v_Right - pRect->v_Left;

    num_in_line = (linewidth+char_tap) / (FontCharSize + char_tap);

    r = *pRect;
    string_x = pRect->v_Left;
    string_y = 	pRect->v_Top;
    //FontCharSize = FontCharSize - 5;
    /* Output string *///ASCLL
    flag = 0;
    if(flag)
    {
        int NumChars = 0;
        int wordlen = 0;
        //				 int wight = 0;
        char * temp;

        int remain_number;
        //				  int line_tap = 0;
        remain_number = num_in_line;
        while (text != 0 && linenumber > 0) 
        {
            NumChars = 0;
            while( *text ==' ')
            {
                NumChars++;
                text++;
            };
            if (NumChars != 0)
            {
                if(NumChars > remain_number)
                {
                    //换行输出;
                    string_x = pRect->v_Left;//WINKS_INFOBOX_STRING_X;
                    string_y =  string_y + lineheight;
                    remain_number = num_in_line;
                    linenumber--;
                }
                else
                {
                    string_x = string_x +  NumChars*(char_tap + FontCharSize);//
                }
            }
            if(*text == 0)
            break;
            /*if(*text == ' ')
            {text++;}*/
            for (temp = text; *temp !=0 && *temp !=' ' && *temp !='\n'; temp++); 
                wordlen = temp - text;
            if(wordlen > num_in_line )
            {
                while(wordlen > num_in_line )
                {
                    string_x = pRect->v_Left;//WINKS_INFOBOX_STRING_X;
                    string_y =  string_y+ lineheight;
                    wordlen = wordlen -num_in_line ;
                    linenumber--;
                    winks_ui_textOutlen(string_x ,string_y,text,num_in_line);
                    text = text + num_in_line;
                }
                string_x = pRect->v_Left;//WINKS_INFOBOX_STRING_X;
                string_y =  string_y+ lineheight;
            }
            if(wordlen > remain_number)
            {
                //换行输出;

                string_x = pRect->v_Left;//WINKS_INFOBOX_STRING_X;
                string_y =  string_y + lineheight;
                remain_number = num_in_line - wordlen;
                linenumber--;
            }
            else
            {

                remain_number = remain_number - wordlen;
            }
            winks_ui_textOutlen(string_x ,string_y,text,wordlen);
            text = temp;
            string_x =  string_x + (wordlen -1) *(char_tap + FontCharSize);
        }
        //return NumChars;
    }
    else
    {
        //	 int NumChars = 0;
        int wordlen = 0;
        //				 int wight = 0;

        // int  string_x = 0;
        // int  string_y = 0;
        int  remain_number;
        //				  int line_tap = 0;
        remain_number = num_in_line;
        while (text != 0 && linenumber > 0) 
        {
            /*NumChars = 0;
            for( ;*text !=0&&*(text+1) !=' ';text = text + 2)
            {
            NumChars++;
            };
            if(NumChars > remain_number)
            {
            //换行输出;
            string_x = WINKS_INFOBOX_STRING_X;
            string_y =  string_y+ lineHEIGHT;
            remain_number = num_in_line;
            linenumber--;
            }
            else
            {
            string_x =  string_x + NumChars*(char_tap + FontCharSize*2);
            }*/
            if(*text == 0)
                break;
            if((unsigned char)*text <= 0x7f) //)(unsigned char)*(src) <= 0x7f
            {
                temp = text;
                if(*temp ==0 &&*(temp + 1) ==0) return;
                else
                {
                    wordlen = 1;
                	if(wordlen > remain_number)
                	{
                		//换行输出;
                		string_x = pRect->v_Left;//WINKS_INFOBOX_STRING_X;
                		string_y =  string_y+ lineheight;
                		remain_number = num_in_line - wordlen;
                		linenumber--;
                        temp = temp + 1;
                	}
                	else
            		{
            			temp = temp + 1;
            			remain_number = remain_number - wordlen;
            		}
                }
                winks_ui_textOutlen(string_x ,string_y,text,wordlen);
                string_x = string_x + wordlen * (char_tap + (FontCharSize ));
                text = temp;
                //return ;
            
            
                #if 0
                for (temp = text; *temp !=0 &&*(temp + 1) !=0&& 
                     *temp != ' ' &&*(temp + 1) !=0 && 
                	 *temp !='\n' &&*(temp + 1) != 0; temp = temp + 2); 
                    wordlen = temp - text - 1;
                if(wordlen > remain_number)
            	{
            		//换行输出;
            		string_x = pRect->v_Left;//WINKS_INFOBOX_STRING_X;
            		string_y =  string_y+ linehight;
            		remain_number = num_in_line - wordlen;
            		linenumber--;
            	}
                else
            	{
            		string_x =  string_x + (wordlen >> 1) * (char_tap + FontCharSize);
            		remain_number = remain_number - wordlen;
            	}
                #endif
            }
            else
            {
                temp = text;
                if(*temp ==0 &&*(temp + 1) ==0) return;
                else
                {
                    wordlen = 2;
                	if(wordlen > remain_number)
                	{
                		//换行输出;
                		string_x = pRect->v_Left;//WINKS_INFOBOX_STRING_X;
                		string_y =  string_y+ lineheight;
                		remain_number = num_in_line - wordlen;
                		linenumber--;
                        temp = temp +2;
                	}
                	else
            		{
            			temp = temp +2;
            			remain_number = remain_number - wordlen;
            		}
                }
				winks_ui_textOutlen(string_x ,string_y,text,wordlen);
				string_x = string_x + (wordlen >> 1) * (char_tap + (FontCharSize << 1));
				text = temp;
            }
            
        }
        return ;
    }
}


/**************************************************************

	FUNCTION NAME		: Winks_UI_Goto_Next_PopupItem

  	PURPOSE		        : Go to Next PopupItem.

	INPUT PARAMETERS	: winks_POPMenuListScreen_s *p_Popup
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void Winks_UI_Goto_Next_PopupItem(winks_POPMenuListScreen_s *p_Popup)
{
    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    p_Popup->v_FocusItemIndxInScrn = p_Popup->v_FocusItemIndxInScrn + 1;
    if(p_Popup->v_FocusItemIndxInScrn > p_Popup->v_TotalItemsNum )
    {	

        p_Popup->v_FocusItemIndxInScrn = 1;

    }
    
#ifdef __WINKS_MULTI_LAYS__    
    WINKS_BUFFER_ON;
    winks_ui_drawPOPMenuScreen(Winks_UIContext->gv_PopmenuScreen);
    WINKS_BUFFER_OFF;
#else
    winks_Refresh_Full_Screen();
#endif

}

/**************************************************************

	FUNCTION NAME		: Winks_UI_Goto_Previous_PopupItem

  	PURPOSE		        : Go to Previous PopupItem.

	INPUT PARAMETERS	: winks_POPMenuListScreen_s *p_Popup
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void Winks_UI_Goto_Previous_PopupItem(winks_POPMenuListScreen_s *p_Popup)
{
    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    p_Popup->v_FocusItemIndxInScrn = p_Popup->v_FocusItemIndxInScrn - 1;
    if(p_Popup->v_FocusItemIndxInScrn == 0 )
    {	

        p_Popup->v_FocusItemIndxInScrn = p_Popup->v_TotalItemsNum;

    }
    
#ifdef __WINKS_MULTI_LAYS__    
    WINKS_BUFFER_ON;
    winks_ui_drawPOPMenuScreen(Winks_UIContext->gv_PopmenuScreen);
    WINKS_BUFFER_OFF;
#else
    winks_Refresh_Full_Screen();
#endif
   
}


/**************************************************************

	FUNCTION NAME		: Winks_UI_Goto_Next_Item

  	PURPOSE		        : Go to Next Item.

	INPUT PARAMETERS	: winks_List_Screen_s * p_Screen
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void Winks_UI_Goto_Next_ListItem(winks_List_Screen_s * p_Screen)
{	
    int old_index = 0;
    int temp_index = 0;

    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    Winks_assert(p_Screen != NULL);

    if (Winks_UIContext->winks_scroll_string.scroll_pause == 1)
    {
        winks_ui_scrolling_text_stop(&Winks_UIContext->winks_scroll_string);
    }

    Winks_UIContext->scrooltext_flag = 0;



    old_index = (Winks_UIContext->gv_ListScreen)->v_FocusItemIndxInScrn;
    (Winks_UIContext->gv_ListScreen)->v_FocusItemIndxInScrn = (Winks_UIContext->gv_ListScreen)->v_FocusItemIndxInScrn + 1;
    temp_index = p_Screen->v_FirstItemIndx + (Winks_UIContext->gv_ListScreen)->v_FocusItemIndxInScrn - 1;
    
    if((p_Screen->v_FocusItemIndxInScrn > WINKS_LIST_MAX_NUMBER) || (temp_index > (Winks_UIContext->gv_ListScreen)->v_TotalItemsNum))
    {	
    	if( p_Screen->v_FirstItemIndx >= p_Screen->v_TotalItemsNum - (WINKS_LIST_MAX_NUMBER-1))
    	{
            p_Screen->v_FirstItemIndx = 1;
            p_Screen->v_FocusItemIndxInScrn = 1;
            //if (p_Screen->v_TotalItemsNum > WINKS_LIST_MAX_NUMBER)
            {
            //    p_Screen->v_FirstItemIndx = MAX(p_Screen->v_TotalItemsNum - (WINKS_LIST_MAX_NUMBER-1), 1);
            //    p_Screen->v_FocusItemIndxInScrn = MIN(WINKS_LIST_MAX_NUMBER,p_Screen->v_TotalItemsNum);
            }
            /*
            else
            {
                p_Screen->v_FirstItemIndx = 1;
                p_Screen->v_FocusItemIndxInScrn = p_Screen->v_TotalItemsNum;
            }*/
            if(p_Screen->a_ItemList != NULL)
            {
                p_Screen->a_ItemList[p_Screen->v_TotalItemsNum -1].v_NextMenuFlag = 1;
            }
            else
            {
                p_Screen->a_ItemListData[p_Screen->v_TotalItemsNum -1].v_NextMenuFlag = 1;
            }
    	}
    	else
    	{
            (p_Screen->v_FirstItemIndx)++;
            p_Screen->v_FocusItemIndxInScrn = WINKS_LIST_MAX_NUMBER;
    	}
    }
    else if ((Winks_UIContext->gv_ListScreen)->v_FocusItemIndxInScrn > p_Screen->v_TotalItemsNum )
    {
    	p_Screen->v_FirstItemIndx = 1;
    	p_Screen->v_FocusItemIndxInScrn = 1;
    	if(p_Screen->a_ItemList != NULL)
        {
            p_Screen->a_ItemList[p_Screen->v_TotalItemsNum].v_NextMenuFlag = 1;
        }
        else
        {
            p_Screen->a_ItemListData[p_Screen->v_TotalItemsNum].v_NextMenuFlag = 1;
        }
    }
    
    if (Winks_UIContext->gv_ListScreen->v_TotalItemsNum > WINKS_LIST_MAX_NUMBER)
    {
        Winks_UIContext->gv_scrollbar.down_button.flags = WINKS_UI_BUTTON_STATE_DOWN;
    }
    
#ifdef __WINKS_MULTI_LAYS__    
    WINKS_BUFFER_ON;
    winks_ui_drawListScreen(Winks_UIContext->gv_ListScreen);
    //winks_ui_redrawlistmenu(gv_ListScreen, old_index, p_Screen->v_FocusItemIndxInScrn);
    WINKS_BUFFER_OFF;
#else
    winks_Refresh_Full_Screen();
#endif

}


/**************************************************************

	FUNCTION NAME		: Winks_UI_Goto_Previous_Item

  	PURPOSE		        : Go to Previous Item.

	INPUT PARAMETERS	: winks_List_Screen_s * p_Screen
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void Winks_UI_Goto_Previous_ListItem(winks_List_Screen_s * p_Screen)
{	
    int old_index = 0;

    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    Winks_assert(p_Screen != NULL);

    if (Winks_UIContext->winks_scroll_string.scroll_pause == 1)
    {
        winks_ui_scrolling_text_stop(&Winks_UIContext->winks_scroll_string);
    }

    Winks_UIContext->scrooltext_flag = 0;



    old_index = (Winks_UIContext->gv_ListScreen)->v_FocusItemIndxInScrn;
    p_Screen->v_FocusItemIndxInScrn = p_Screen->v_FocusItemIndxInScrn - 1;
    if(p_Screen->v_FocusItemIndxInScrn == 0 )
    {	

        if( p_Screen->v_FirstItemIndx == 1)
        {
            if (p_Screen->v_TotalItemsNum <= WINKS_LIST_MAX_NUMBER)
            {
                
                p_Screen->v_FirstItemIndx = MAX((p_Screen->v_TotalItemsNum -WINKS_LIST_MAX_NUMBER + 1),1);
                p_Screen->v_FocusItemIndxInScrn = p_Screen->v_TotalItemsNum;
                if(p_Screen->a_ItemList != NULL)
                {
                    p_Screen->a_ItemList[p_Screen->v_TotalItemsNum - 1].v_NextMenuFlag = 1;
                }
                else
                {
                    p_Screen->a_ItemListData[p_Screen->v_TotalItemsNum -1].v_NextMenuFlag = 1;
                }
                /*
                p_Screen->v_FirstItemIndx = 1;
                p_Screen->v_FocusItemIndxInScrn = 1;*/
                if(p_Screen->a_ItemList != NULL)
                {
                    p_Screen->a_ItemList[p_Screen->v_TotalItemsNum - 1].v_NextMenuFlag = 1;
                }
                else
                {
                    p_Screen->a_ItemListData[p_Screen->v_TotalItemsNum -1].v_NextMenuFlag = 1;
                }

            }
            else
            {
                // p_Screen->v_FirstItemIndx = p_Screen->v_TotalItemsNum;
                //p_Screen->v_FocusItemIndxInScrn = 1;
                p_Screen->v_FirstItemIndx = MAX((p_Screen->v_TotalItemsNum -WINKS_LIST_MAX_NUMBER + 1),1);
                p_Screen->v_FocusItemIndxInScrn = WINKS_LIST_MAX_NUMBER;
                
                if(p_Screen->a_ItemList != NULL)
                {
                    p_Screen->a_ItemList[p_Screen->v_TotalItemsNum -1].v_NextMenuFlag = 1;
                }
                else
                {
                    p_Screen->a_ItemListData[p_Screen->v_TotalItemsNum - 1].v_NextMenuFlag = 1;
                }

            }		

        }
        else
        {
            (p_Screen->v_FirstItemIndx)--;
            p_Screen->v_FocusItemIndxInScrn = 1;
        }
    }
    
    if (Winks_UIContext->gv_ListScreen->v_TotalItemsNum > WINKS_LIST_MAX_NUMBER)
    {
        Winks_UIContext->gv_scrollbar.up_button.flags = WINKS_UI_BUTTON_STATE_DOWN;
    }
#ifdef __WINKS_MULTI_LAYS__    
    WINKS_BUFFER_ON;
    winks_ui_drawListScreen(Winks_UIContext->gv_ListScreen);
    //winks_ui_redrawlistmenu(gv_ListScreen, old_index, p_Screen->v_FocusItemIndxInScrn);
    WINKS_BUFFER_OFF;
#else
    winks_Refresh_Full_Screen();
#endif
}


/**************************************************************

	FUNCTION NAME		: Winks_UI_Goto_Next_Page

  	PURPOSE		        : Go to Next Page.

	INPUT PARAMETERS	: winks_List_Screen_s * p_Screen
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void Winks_UI_Goto_Next_ListPage(winks_List_Screen_s *p_Screen)
{	
    int highlight_index;
    int page_index;
    int old_index = 0;
    int temp_focus_index = 0;


    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    Winks_assert(p_Screen != NULL);
    
    if (Winks_UIContext->winks_scroll_string.scroll_pause == 1)
    {
        winks_ui_scrolling_text_stop(&Winks_UIContext->winks_scroll_string);
    }
    
    Winks_UIContext->scrooltext_flag = 0;
	

	
    old_index = (Winks_UIContext->gv_ListScreen)->v_FocusItemIndxInScrn;
    highlight_index = p_Screen->v_FirstItemIndx + p_Screen->v_FocusItemIndxInScrn -1;
    page_index = (highlight_index - 1) / WINKS_LIST_MAX_NUMBER;
    if( page_index != (p_Screen->v_TotalItemsNum -1)/ WINKS_LIST_MAX_NUMBER)
    {
        p_Screen->v_FirstItemIndx = (page_index +1) * WINKS_LIST_MAX_NUMBER + 1;
        temp_focus_index = p_Screen->v_TotalItemsNum - p_Screen->v_FirstItemIndx + 1;
        if (p_Screen->v_FocusItemIndxInScrn > temp_focus_index)
        {
            p_Screen->v_FocusItemIndxInScrn = temp_focus_index;
        }
    }

    if (Winks_UIContext->gv_ListScreen->v_TotalItemsNum > WINKS_LIST_MAX_NUMBER)
    {
        Winks_UIContext->gv_scrollbar.down_button.flags = WINKS_UI_BUTTON_STATE_DOWN;
    }
    
#ifdef __WINKS_MULTI_LAYS__    
    WINKS_BUFFER_ON;
    winks_ui_drawListScreen(Winks_UIContext->gv_ListScreen);
    //winks_ui_redrawlistmenu(gv_ListScreen, old_index, p_Screen->v_FocusItemIndxInScrn);
    WINKS_BUFFER_OFF;
#else
    winks_Refresh_Full_Screen();
#endif
}


/**************************************************************

	FUNCTION NAME		: Winks_UI_Goto_Previous_Page

  	PURPOSE		        : Go to Previous Page.

	INPUT PARAMETERS	: winks_List_Screen_s * p_Screen
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void Winks_UI_Goto_Previous_ListPage(winks_List_Screen_s *p_Screen)
{
    int highlight_index;
    int page_index;
    int old_index = 0;

    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    Winks_assert(p_Screen != NULL);

    if (Winks_UIContext->winks_scroll_string.scroll_pause == 1)
    {
        winks_ui_scrolling_text_stop(&Winks_UIContext->winks_scroll_string);
    }

    Winks_UIContext->scrooltext_flag = 0;



    old_index = (Winks_UIContext->gv_ListScreen)->v_FocusItemIndxInScrn;
    highlight_index = p_Screen->v_FirstItemIndx + p_Screen->v_FocusItemIndxInScrn -1;
    page_index = (highlight_index - 1) / WINKS_LIST_MAX_NUMBER;
    
    if(page_index != 0)
    {
        p_Screen->v_FirstItemIndx = (page_index - 1) * WINKS_LIST_MAX_NUMBER + 1;
    }
    
    if (Winks_UIContext->gv_ListScreen->v_TotalItemsNum > WINKS_LIST_MAX_NUMBER)
    {
        Winks_UIContext->gv_scrollbar.up_button.flags = WINKS_UI_BUTTON_STATE_DOWN;
    }
    
#ifdef __WINKS_MULTI_LAYS__    
    WINKS_BUFFER_ON;
    winks_ui_drawListScreen(Winks_UIContext->gv_ListScreen);
    //winks_ui_redrawlistmenu(gv_ListScreen, old_index, p_Screen->v_FocusItemIndxInScrn);
    WINKS_BUFFER_OFF;
#else
    winks_Refresh_Full_Screen();
#endif
}


/**************************************************************

	FUNCTION NAME		: winks_ui_redrawlistmenu

  	PURPOSE		        : Redraw  list menu.

	INPUT PARAMETERS	: winks_List_Screen_s *p_List
	                      int oldicon_index
	                      int newicon_index
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void winks_ui_redrawlistmenu(winks_List_Screen_s *p_List, int oldicon_index, int newicon_index)
{	
    int  index;
    int  highlight_x, highlight_y;
    int  left_x,left_y,rightdown_x,rightdown_y;

    winks_vertical_scrollbar_s scrollbar;

    Winks_assert(p_List != NULL);

    if (winks_draw_condition(WINKS_MINGUI_HWND_USER) != WINKS_RETURN_SUCCESS)
    {
        return;
    }
    
    winks_set_font(WINKS_MEDIUM_FONT);
    
     /*画亮度条*/
    highlight_x = WINKS_LIST_ICON_X;
    highlight_y =  (p_List->v_FocusItemIndxInScrn - 1) * WINKS_HIGHLIGHT_HEIGHT+ WINKS_LIST_FIRSTSTRINGONLY_Y - ((WINKS_HIGHLIGHT_LISTITEM_HEIGHT  - WINKS_GETFONTHEIGHT) >> 1);

    winks_ui_fillRect(highlight_x,highlight_y,WINKS_LIST_ICON_X + WINKS_CLIENT_WIDTH,highlight_y + WINKS_HIGHLIGHT_LISTITEM_HEIGHT,WINKS_HILIGHT_COLOR);
    index = p_List->v_FirstItemIndx + oldicon_index - 1;
    left_x = WINKS_LIST_ICON_X;
    left_y = ( oldicon_index -1)* WINKS_HIGHLIGHT_HEIGHT+ WINKS_LIST_FIRSTSTRINGONLY_Y - (WINKS_HIGHLIGHT_LISTITEM_HEIGHT  - WINKS_GETFONTHEIGHT);
    rightdown_y = left_y + WINKS_HIGHLIGHT_LISTITEM_HEIGHT;
    rightdown_x = WINKS_CLIENT_WIDTH;
    
    /*may be image*/
    winks_ui_fillRect(left_x,left_y,left_x + rightdown_x, rightdown_y, WINKS_PIXEL_BLACK );
    if(p_List->a_ItemList != NULL)
    {
        winks_ui_drawListItem(&p_List->a_ItemList[index - 1],index-p_List->v_FirstItemIndx + 1);
    }
    else
    {
        winks_ui_drawListItem(&p_List->a_ItemListData[index - 1],index-p_List->v_FirstItemIndx + 1);
    }
    index = p_List->v_FirstItemIndx + newicon_index - 1;
    if(p_List->a_ItemList != NULL)
    {
        winks_ui_drawListItem(&p_List->a_ItemList[index - 1],index-p_List->v_FirstItemIndx + 1);
    }
    else
    {
        winks_ui_drawListItem(&p_List->a_ItemListData[index - 1],index-p_List->v_FirstItemIndx + 1);
    }

    /*画亮度条*/
    highlight_x = WINKS_LIST_ICON_X;
    highlight_y =  (p_List->v_FocusItemIndxInScrn - 1) * WINKS_HIGHLIGHT_HEIGHT+ WINKS_LIST_FIRSTSTRINGONLY_Y - ((WINKS_HIGHLIGHT_LISTITEM_HEIGHT  - WINKS_GETFONTHEIGHT) >> 1);

    //winks_ui_fillRect(highlight_x,highlight_y,WINKS_TITLE_STRING_X + WINKS_CLIENT_WIDTH,highlight_y + WINKS_HIGHLIGHT_LISTITEM_HEIGHT,WINKS_HILIGHT_COLOR);
    ////WINKS_OPACITY_ON(highlight_x,highlight_y,WINKS_SCREEN_WIDTH,WINKS_HIGHLIGHT_HEIGHT);
    ////winks_ui_drawGraphicFromFile(highlight_x, highlight_y, Winks_UIFM_GetImage(WINKS_HIGHLIGHT_POPUP_ICONID));
    //winks_show_transparent_image(highlight_x, highlight_y, Winks_UIFM_GetImage(WINKS_HIGHLIGHT_POPUP_ICONID));
    ////WINKS_OPACITY_OFF(highlight_x,highlight_y,WINKS_SCREEN_WIDTH,WINKS_HIGHLIGHT_HEIGHT);
    //winks_image_draw_resized_file(highlight_x + 10, highlight_y+ 30, 113,30,Winks_UIFM_GetImage(WINKS_HIGHLIGHT_POPUP_ICONID));
    /*如果listitem总数 大于 一屏显示的最大数
    画ScrollBar*/
    //winks_ui_fillRect(highlight_x, highlight_y,highlight_x + 130, highlight_y +WINKS_HIGHLIGHT_HEIGHT,0x0000FF);
    //winks_ui_fillRect(highlight_x, highlight_y,highlight_x + 130, highlight_y +WINKS_HIGHLIGHT_HEIGHT ,  WINKS_PIXEL_RED );
    if(p_List->v_TotalItemsNum > WINKS_LIST_MAX_NUMBER)
    {
    	winks_ui_create_vertical_scrollbar(&scrollbar, p_List->v_FirstItemIndx, p_List->v_TotalItemsNum, WINKS_LIST_MAX_NUMBER);
    	winks_ui_drawVerticalScrollBar(&scrollbar);
    }
    
}
#if 0

#if 0
#ifdef __WINKS_UI_MATRIXMENU_WITH_BACKGROUND__
/**************************************************************

	FUNCTION NAME		: winks_ui_redrawmenu

  	PURPOSE		        : Redraw Next.

	INPUT PARAMETERS	: winks_Matrixmenu_Screen_s *p_Matrixmenu
                          int oldicon_index
                          int newicon_index
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void winks_ui_redrawmenu(winks_Matrixmenu_Screen_s *p_Matrixmenu, int oldicon_index, int newicon_index)
{
    // int index;
    int temp;
    int icon_x,icon_y;
    int row_number, column_number;
    int mainmenuitem_height,mainmenuitem_weight;
    int icon;
    int old_title_id = 0;
    int new_title_id = 0;
    int stlen = 0;
    char * pc_temp = NULL;
    int w = 0, h = 0;

    int x_gap = 0, y_gap = 0;
    int excursion = 0;

    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    Winks_assert(p_Matrixmenu != NULL);

    if (winks_draw_condition(WINKS_MINGUI_HWND_USER) != WINKS_RETURN_SUCCESS)
    {
        return;
    }

    winks_set_font(WINKS_MEDIUM_FONT);

    row_number = p_Matrixmenu->v_numberbycolumn;
    mainmenuitem_weight= (WINKS_CLIENT_WIDTH - ( WINKS_MAINMENU_FIRSTICON_X << 1)) / row_number;
   // mainmenuitem_weight = (WINKS_CLIENT_WIDTH - (WINKS_FIRST_MATRIXMAINMENU_X - ((WINKS_MAINMENU_BACKGROUND_WIDTH - WINKS_MAINMENU_ICON_WIDTH) >> 1))) / row_number;
    column_number = p_Matrixmenu->v_TotalMatrixItemsNum / p_Matrixmenu->v_numberbycolumn ;
    temp = p_Matrixmenu->v_TotalMatrixItemsNum %p_Matrixmenu->v_numberbycolumn;
    if(temp != 0)
    {
        column_number++;
    }
    mainmenuitem_height= WINKS_CLIENT_HEIGHT / column_number  + WINKS_MAINMENU_GAP; 

    new_title_id = WINKS_UI_BUDDYNUM_STRINGID + ((Winks_UIContext->gv_MainMenu)->v_FocusMatrixItemIndxInScrn -1);
    old_title_id = new_title_id + oldicon_index - newicon_index;


       
    //winks_ui_drawtitle(&Winks_UIContext->g_Title);
	//winks_set_font(WINKS_SMALL_FONT);
    
    icon_x = ((oldicon_index - 1) % row_number ) * mainmenuitem_weight+ WINKS_FIRST_MATRIXMAINMENU_X;// 
    icon_y = ((oldicon_index - 1) / row_number ) * mainmenuitem_height+ WINKS_FIRST_MATRIXMAINMENU_Y;

    x_gap = (WINKS_MAINMENU_BACKGROUND_WIDTH - WINKS_MAINMENU_ICON_WIDTH) >> 1;
    //y_gap = (WINKS_MAINMENU_BACKGROUND_HEIGHT - WINKS_MAINMENU_ICON_HEIGHT - WINKS_ICON_TEXT_GAP) >> 1;
    y_gap = WINKS_ICON_BACKGROUND_GAP;
    
    icon = p_Matrixmenu->a_MenuItem[(oldicon_index - 1) ].a_MatrixItemIcon[1];
    //WINKS_CLIP_ON(icon_x,icon_y, WINKS_COLORCALL_PIC_WIGHT, WINKS_COLORCALL_PIC_HEIGHT);
    if( icon )
    {
        WINKS_CLIP_ON(icon_x - x_gap , icon_y - y_gap,icon_x  - x_gap + WINKS_MAINMENU_BACKGROUND_WIDTH, icon_y - y_gap + WINKS_MAINMENU_BACKGROUND_HEIGHT);
        winks_clear_screen_fillRect(icon_x, icon_y,icon_x + WINKS_MAINMENU_ICON_WIDTH, icon_y + WINKS_MAINMENU_ICON_HEIGHT);
        winks_clear_screen_fillRect(icon_x - x_gap , icon_y - y_gap,icon_x - x_gap  + WINKS_MAINMENU_BACKGROUND_WIDTH, icon_y - y_gap + WINKS_MAINMENU_BACKGROUND_HEIGHT);
        winks_ui_drawGraphicFromFile(icon_x, icon_y, Winks_UIFM_GetImage(icon));
        WINKS_SET_TEXTCOLOR(WINKS_PIXEL_LIGHTWHITE);
        stlen = winks_strlen_ucs2_type((char *)Winks_UIFM_GetString(old_title_id));
        //gb2312tounicode( ptmp, 32, Winks_UIFM_GetString(stringid), stlen );
        winks_get_string_wight_high( (char * )Winks_UIFM_GetString(old_title_id), stlen, &w, &h );
        if (w >= WINKS_MAINMENU_BACKGROUND_WIDTH)
        {
            int templen = 0;
            pc_temp = (char*)Winks_UIFM_GetString(old_title_id);
            creat_text_String_line_type((char*)Winks_UIFM_GetString(old_title_id),50,&templen);
            winks_ui_textOutlen(icon_x + 3,icon_y + 35,Winks_UIFM_GetString(old_title_id),templen);
            winks_ui_textOutlen(icon_x + 3,icon_y + 43,(const char*)(pc_temp + templen),winks_strlen_ucs2_type(pc_temp + templen));
            
        }
        else
        {
            excursion = (WINKS_MAINMENU_BACKGROUND_WIDTH - w) >> 1;
            winks_ui_textOutlen(icon_x - x_gap + excursion,icon_y + WINKS_MAINMENU_BACKGROUND_HEIGHT + WINKS_ICON_TEXT_GAP -WINKS_GETFONTHEIGHT,Winks_UIFM_GetString(old_title_id),winks_strlen_ucs2_type((char *)Winks_UIFM_GetString(old_title_id)));
        }
        //winks_ui_textOutlen(icon_x + 3,icon_y + 35,Winks_UIFM_GetString(old_title_id),winks_strlen_ucs2_type((char *)Winks_UIFM_GetString(old_title_id)));
        WINKS_CLIP_OFF(icon_x - x_gap , icon_y - y_gap,icon_x + WINKS_MAINMENU_BACKGROUND_WIDTH, icon_y - y_gap + WINKS_MAINMENU_BACKGROUND_HEIGHT);
    }
    icon_x = ((newicon_index - 1) % row_number ) * mainmenuitem_weight+ WINKS_FIRST_MATRIXMAINMENU_X;// 
    icon_y = ((newicon_index - 1) / row_number ) * mainmenuitem_height+ WINKS_FIRST_MATRIXMAINMENU_Y;
    icon = p_Matrixmenu->a_MenuItem[newicon_index - 1 ].a_MatrixItemIcon[1];
    if( icon )
    {
        WINKS_CLIP_ON(icon_x - x_gap , icon_y - y_gap,icon_x + WINKS_MAINMENU_BACKGROUND_WIDTH, icon_y - y_gap + WINKS_MAINMENU_BACKGROUND_HEIGHT);
        //winks_ui_drawGraphicFromFile(icon_x - x_gap, icon_y - y_gap, Winks_UIFM_GetImage(p_Matrixmenu->a_MenuItem[newicon_index - 1 ].a_MatrixItemIcon[0]));
        winks_ui_drawGraphicFromFile(icon_x - x_gap, icon_y - y_gap, Winks_UIFM_GetImage(WINKS_UI_BG_MATRIXMENU_ITEM_ID));
        WINKS_CLIP_ON(icon_x - x_gap , icon_y - y_gap,icon_x + WINKS_MAINMENU_BACKGROUND_WIDTH, icon_y - y_gap + WINKS_MAINMENU_BACKGROUND_HEIGHT);
        //winks_clear_screen_fillRect(icon_x, icon_y,icon_x + WINKS_MAINMENU_ICON_WIDTH, icon_y + WINKS_MAINMENU_ICON_HEIGHT);
        winks_ui_drawGraphicFromFile(icon_x, icon_y, Winks_UIFM_GetImage(icon));
        WINKS_SET_TEXTCOLOR(WINKS_PIXEL_BLACK);

        WINKS_CLIP_OFF(icon_x, icon_y,icon_x + WINKS_MAINMENU_ICON_WIDTH, icon_y + WINKS_MAINMENU_ICON_HEIGHT);

        stlen = winks_strlen_ucs2_type((char *)Winks_UIFM_GetString(new_title_id));
        //gb2312tounicode( ptmp, 32, Winks_UIFM_GetString(stringid), stlen );
        winks_get_string_wight_high( (char * )Winks_UIFM_GetString(new_title_id), stlen, &w, &h );
        if (w >= WINKS_MAINMENU_BACKGROUND_WIDTH)
        {
            int templen = 0;
            pc_temp = (char*)Winks_UIFM_GetString(new_title_id);
            creat_text_String_line_type((char*)Winks_UIFM_GetString(new_title_id),50,&templen);
            winks_ui_textOutlen(icon_x + 3,icon_y + 35,Winks_UIFM_GetString(new_title_id),templen);
            winks_ui_textOutlen(icon_x + 3,icon_y + 43,(const char*)(pc_temp + templen),winks_strlen_ucs2_type(pc_temp + templen));
            
        }
        else
        {
            excursion = (WINKS_MAINMENU_BACKGROUND_WIDTH - w) >> 1;
            winks_ui_textOutlen(icon_x - x_gap + excursion,icon_y + WINKS_MAINMENU_BACKGROUND_HEIGHT + WINKS_ICON_TEXT_GAP-WINKS_GETFONTHEIGHT,
                                Winks_UIFM_GetString(new_title_id),
                                winks_strlen_ucs2_type((char *)Winks_UIFM_GetString(new_title_id)));
        }
        //winks_ui_textOutlen(icon_x + 3,icon_y + 40,Winks_UIFM_GetString(Winks_UIContext->g_Title.v_TitleStringID),winks_strlen_ucs2_type((char *)Winks_UIFM_GetString(Winks_UIContext->g_Title.v_TitleStringID)));
       
        WINKS_CLIP_OFF(icon_x - x_gap , icon_y - y_gap,icon_x + WINKS_MAINMENU_BACKGROUND_WIDTH, icon_y - y_gap + WINKS_MAINMENU_BACKGROUND_HEIGHT);
    }
    //WINKS_CLIP_ON(WINKS_COLORCALL_PIC_X,WINKS_COLORCALL_PIC_Y, WINKS_COLORCALL_PIC_WIGHT, WINKS_COLORCALL_PIC_HEIGHT);
    WINKS_SET_TEXTCOLOR(WINKS_PIXEL_LIGHTWHITE);
    //winks_set_font(WINKS_MEDIUM_FONT);
       
#if 0
    if( icon )
    winks_ui_drawGraphicFromFile(icon_x, icon_y, Winks_UIFM_GetImage(icon));
    for(index = 0; index < p_Matrixmenu->v_TotalMatrixItemsNum ; index++)
    {
    icon_x = (index % row_number ) * mainmenuitem_weight+ WINKS_FIRST_MATRIXMAINMENU_X;// 
    icon_y = (index / row_number ) * mainmenuitem_height+ WINKS_FIRST_MATRIXMAINMENU_Y;
    if(index == oldicon - 1)
    {
    	icon = p_Matrixmenu->a_MenuItem[index ].a_MatrixItemIcon[0];
    	if( icon )
    		winks_ui_drawGraphicFromFile(icon_x, icon_y, Winks_UIFM_GetImage(icon));
    }
    else if( index == newicon - 1 )
    {
    	icon = p_Matrixmenu->a_MenuItem[index].a_MatrixItemIcon[1];
    	if( icon )
    		winks_ui_drawGraphicFromFile(icon_x, icon_y, Winks_UIFM_GetImage(icon));
    }
    }
#endif

}
#else
/**************************************************************

	FUNCTION NAME		: winks_ui_redrawmenu

  	PURPOSE		        : Redraw Next.

	INPUT PARAMETERS	: winks_Matrixmenu_Screen_s *p_Matrixmenu
                          int oldicon_index
                          int newicon_index
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void winks_ui_redrawmenu(winks_Matrixmenu_Screen_s *p_Matrixmenu, int oldicon_index, int newicon_index)
{
    // int index;
    int temp;
    int icon_x,icon_y;
    int row_number, column_number;
    int mainmenuitem_height,mainmenuitem_weight;
    int icon;


    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    Winks_assert(p_Matrixmenu != NULL);

    if (winks_draw_condition(WINKS_MINGUI_HWND_USER) != WINKS_RETURN_SUCCESS)
    {
        return;
    }

    row_number = p_Matrixmenu->v_numberbycolumn;
    mainmenuitem_weight= (WINKS_CLIENT_WIDTH - WINKS_FIRST_MATRIXMAINMENU_X) / row_number;
	//mainmenuitem_weight = (WINKS_CLIENT_WIDTH - (WINKS_FIRST_MATRIXMAINMENU_X - ((WINKS_MAINMENU_BACKGROUND_WIDTH - WINKS_MAINMENU_ICON_WIDTH) >> 1))) / row_number;
    column_number = p_Matrixmenu->v_TotalMatrixItemsNum / p_Matrixmenu->v_numberbycolumn ;
    temp = p_Matrixmenu->v_TotalMatrixItemsNum %p_Matrixmenu->v_numberbycolumn;
    if(temp != 0)
    {
        column_number++;
    }
    mainmenuitem_height= WINKS_CLIENT_HEIGHT / column_number;

    Winks_UIContext->g_Title.v_TitleStringID = Winks_UIContext->g_Title.v_TitleStringID + newicon_index - oldicon_index;


       
    winks_ui_drawtitle(&Winks_UIContext->g_Title);
    icon_x = ((oldicon_index - 1) % row_number ) * mainmenuitem_weight+ WINKS_FIRST_MATRIXMAINMENU_X;// 
    icon_y = ((oldicon_index - 1) / row_number ) * mainmenuitem_height+ WINKS_FIRST_MATRIXMAINMENU_Y;
    icon = p_Matrixmenu->a_MenuItem[(oldicon_index - 1) ].a_MatrixItemIcon[0];
    //WINKS_CLIP_ON(icon_x,icon_y, WINKS_COLORCALL_PIC_WIGHT, WINKS_COLORCALL_PIC_HEIGHT);
    if( icon )
    {
        WINKS_CLIP_ON(icon_x, icon_y,icon_x + WINKS_MAINMENU_ICON_WIDTH, icon_y + WINKS_MAINMENU_ICON_HEIGHT);
        winks_clear_screen_fillRect(icon_x, icon_y,icon_x + WINKS_MAINMENU_ICON_WIDTH, icon_y + WINKS_MAINMENU_ICON_HEIGHT);
        winks_ui_drawGraphicFromFile(icon_x, icon_y, Winks_UIFM_GetImage(icon));
        WINKS_CLIP_OFF(icon_x, icon_y,icon_x + WINKS_MAINMENU_ICON_WIDTH, icon_y + WINKS_MAINMENU_ICON_HEIGHT);
    }
    icon_x = ((newicon_index - 1) % row_number ) * mainmenuitem_weight+ WINKS_FIRST_MATRIXMAINMENU_X;// 
    icon_y = ((newicon_index - 1) / row_number ) * mainmenuitem_height+ WINKS_FIRST_MATRIXMAINMENU_Y;
    icon = p_Matrixmenu->a_MenuItem[newicon_index - 1 ].a_MatrixItemIcon[1];
    if( icon )
    {
        WINKS_CLIP_ON(icon_x, icon_y,icon_x + WINKS_MAINMENU_ICON_WIDTH, icon_y + WINKS_MAINMENU_ICON_HEIGHT);
        winks_clear_screen_fillRect(icon_x, icon_y,icon_x + WINKS_MAINMENU_ICON_WIDTH, icon_y + WINKS_MAINMENU_ICON_HEIGHT);
        winks_ui_drawGraphicFromFile(icon_x, icon_y, Winks_UIFM_GetImage(icon));
        WINKS_CLIP_OFF(icon_x, icon_y,icon_x + WINKS_MAINMENU_ICON_WIDTH, icon_y + WINKS_MAINMENU_ICON_HEIGHT);
    }
    //WINKS_CLIP_ON(WINKS_COLORCALL_PIC_X,WINKS_COLORCALL_PIC_Y, WINKS_COLORCALL_PIC_WIGHT, WINKS_COLORCALL_PIC_HEIGHT);
       
#if 0
    if( icon )
    winks_ui_drawGraphicFromFile(icon_x, icon_y, Winks_UIFM_GetImage(icon));
    for(index = 0; index < p_Matrixmenu->v_TotalMatrixItemsNum ; index++)
    {
    icon_x = (index % row_number ) * mainmenuitem_weight+ WINKS_FIRST_MATRIXMAINMENU_X;// 
    icon_y = (index / row_number ) * mainmenuitem_height+ WINKS_FIRST_MATRIXMAINMENU_Y;
    if(index == oldicon - 1)
    {
    	icon = p_Matrixmenu->a_MenuItem[index ].a_MatrixItemIcon[0];
    	if( icon )
    		winks_ui_drawGraphicFromFile(icon_x, icon_y, Winks_UIFM_GetImage(icon));
    }
    else if( index == newicon - 1 )
    {
    	icon = p_Matrixmenu->a_MenuItem[index].a_MatrixItemIcon[1];
    	if( icon )
    		winks_ui_drawGraphicFromFile(icon_x, icon_y, Winks_UIFM_GetImage(icon));
    }
    }
#endif

}

#endif
#endif
/**************************************************************

	FUNCTION NAME		: Winks_UI_Mainmenu_Next_Row

  	PURPOSE		        : Go to Next Row.

	INPUT PARAMETERS	: winks_Matrixmenu_Screen_s *p_Matrixmenu
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void  Winks_UI_Mainmenu_Next_Row(winks_Matrixmenu_Screen_s *p_Matrixmenu)
{
    int oldicon = p_Matrixmenu->v_FocusMatrixItemIndxInScrn;

    Winks_assert(p_Matrixmenu != NULL);

    if(p_Matrixmenu->v_FocusMatrixItemIndxInScrn == p_Matrixmenu->v_TotalMatrixItemsNum)
    {
        p_Matrixmenu->v_FocusMatrixItemIndxInScrn = 1;
    }
    else
    {
        p_Matrixmenu->v_FocusMatrixItemIndxInScrn++;
    }
#ifdef __WINKS_MULTI_LAYS__    
    WINKS_BUFFER_ON;
    winks_ui_redrawmenu( p_Matrixmenu, oldicon, p_Matrixmenu->v_FocusMatrixItemIndxInScrn );
    WINKS_BUFFER_OFF;
#else
    winks_Refresh_Full_Screen();
#endif
   
}


/**************************************************************

	FUNCTION NAME		: Winks_UI_Mainmenu_Previous_Row

  	PURPOSE		        : Go to Previous Row.

	INPUT PARAMETERS	: winks_Matrixmenu_Screen_s *p_Matrixmenu
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void  Winks_UI_Mainmenu_Previous_Row(winks_Matrixmenu_Screen_s *p_Matrixmenu)
{
    int oldicon = p_Matrixmenu->v_FocusMatrixItemIndxInScrn;
    if(p_Matrixmenu->v_FocusMatrixItemIndxInScrn == 1)
    {
        p_Matrixmenu->v_FocusMatrixItemIndxInScrn = p_Matrixmenu->v_TotalMatrixItemsNum;
    }
    else
    {
        p_Matrixmenu->v_FocusMatrixItemIndxInScrn--;
    }

#ifdef __WINKS_MULTI_LAYS__    
    WINKS_BUFFER_ON;
    winks_ui_redrawmenu( p_Matrixmenu, oldicon, p_Matrixmenu->v_FocusMatrixItemIndxInScrn );
    WINKS_BUFFER_OFF;
#else
    winks_Refresh_Full_Screen();
#endif
   
}


/**************************************************************

	FUNCTION NAME		: Winks_UI_Mainmenu_Next_Column

  	PURPOSE		        : Go to Next Column.

	INPUT PARAMETERS	: winks_Matrixmenu_Screen_s *p_Matrixmenu
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void  Winks_UI_Mainmenu_Next_Column(winks_Matrixmenu_Screen_s *p_Matrixmenu)
{
    int max_line = 0;
    int oldicon = p_Matrixmenu->v_FocusMatrixItemIndxInScrn;

    Winks_assert(p_Matrixmenu != NULL);

    p_Matrixmenu->v_FocusMatrixItemIndxInScrn = p_Matrixmenu->v_FocusMatrixItemIndxInScrn + p_Matrixmenu->v_numberbycolumn;
    if (p_Matrixmenu->v_FocusMatrixItemIndxInScrn > p_Matrixmenu->v_TotalMatrixItemsNum)
    {
        if ((p_Matrixmenu->v_FocusMatrixItemIndxInScrn % p_Matrixmenu->v_numberbycolumn) == 0)
        {
            max_line = (p_Matrixmenu->v_FocusMatrixItemIndxInScrn / p_Matrixmenu->v_numberbycolumn) - 1;
        }
        else
        {
            max_line = (p_Matrixmenu->v_FocusMatrixItemIndxInScrn / p_Matrixmenu->v_numberbycolumn);
        }

        p_Matrixmenu->v_FocusMatrixItemIndxInScrn = (p_Matrixmenu->v_FocusMatrixItemIndxInScrn - (max_line ) * p_Matrixmenu->v_numberbycolumn);	

    }     

#ifdef __WINKS_MULTI_LAYS__    
    WINKS_BUFFER_ON;
    winks_ui_redrawmenu( p_Matrixmenu, oldicon, p_Matrixmenu->v_FocusMatrixItemIndxInScrn );
    WINKS_BUFFER_OFF;
#else
    winks_Refresh_Full_Screen();
#endif
   

}


/**************************************************************

	FUNCTION NAME		: Winks_UI_Mainmenu_Previous_Column

  	PURPOSE		        : Go to Previous Column.

	INPUT PARAMETERS	: winks_Matrixmenu_Screen_s *p_Matrixmenu
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void  Winks_UI_Mainmenu_Previous_Column(winks_Matrixmenu_Screen_s *p_Matrixmenu)
{
    int max_line = 0;
    int oldicon = p_Matrixmenu->v_FocusMatrixItemIndxInScrn;

    Winks_assert(p_Matrixmenu != NULL);

    p_Matrixmenu->v_FocusMatrixItemIndxInScrn = p_Matrixmenu->v_FocusMatrixItemIndxInScrn - p_Matrixmenu->v_numberbycolumn;
    if(p_Matrixmenu->v_FocusMatrixItemIndxInScrn < 1)
    {
        if (p_Matrixmenu->v_TotalMatrixItemsNum % p_Matrixmenu->v_numberbycolumn == 0)
        {
            max_line = (p_Matrixmenu->v_TotalMatrixItemsNum / p_Matrixmenu->v_numberbycolumn);
        }
        else
        {
            max_line = (p_Matrixmenu->v_TotalMatrixItemsNum / p_Matrixmenu->v_numberbycolumn) + 1;
        }
        p_Matrixmenu->v_FocusMatrixItemIndxInScrn = p_Matrixmenu->v_FocusMatrixItemIndxInScrn + max_line *  p_Matrixmenu->v_numberbycolumn;
        if ( p_Matrixmenu->v_FocusMatrixItemIndxInScrn > p_Matrixmenu->v_TotalMatrixItemsNum)
        {
            p_Matrixmenu->v_FocusMatrixItemIndxInScrn = p_Matrixmenu->v_FocusMatrixItemIndxInScrn - p_Matrixmenu->v_numberbycolumn;
        }
    }
    
#ifdef __WINKS_MULTI_LAYS__    
    WINKS_BUFFER_ON;
    winks_ui_redrawmenu( p_Matrixmenu, oldicon, p_Matrixmenu->v_FocusMatrixItemIndxInScrn );
    WINKS_BUFFER_OFF;
#else
    winks_Refresh_Full_Screen();
#endif
   
}
#endif
#endif
#ifdef __WINKS_UI_MATRIXMENU_WITH_BACKGROUND__
/**************************************************************

	FUNCTION NAME		: winks_ui_MatrixMainmenu

  	PURPOSE		        : SHOW Matrix Mainmenu.

	INPUT PARAMETERS	: winks_Matrixmenu_Screen_s *p_Matrixmenu
	OUTPUT PARAMETERS	: null

	RETURNS				: void

	REMARKS				: null

***************************************************************/

void winks_ui_MatrixMainmenu(winks_Matrixmenu_Screen_s *p_Matrixmenu)
{
    int index;
    int temp;
    int icon_x,icon_y;
    int row_number, column_number;
    int mainmenuitem_height,mainmenuitem_weight;
    int icon;
    int stlen = 0;
    char * pc_temp = NULL;
    int w = 0, h = 0;
    int old_title_id = 0;

    int x_gap = 0, y_gap = 0;
    int excursion = 0;

    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()
    
    Winks_assert(p_Matrixmenu != NULL);
    
    winks_set_font(WINKS_MEDIUM_FONT);
    
    if (winks_draw_condition(WINKS_MINGUI_HWND_USER) != WINKS_RETURN_SUCCESS)
    {
        return;
    }
	//winks_draw_opacity_on();
    row_number = p_Matrixmenu->v_numberbycolumn;
    mainmenuitem_weight= (WINKS_CLIENT_WIDTH - ( WINKS_MAINMENU_FIRSTICON_X << 1)) / row_number;
    //mainmenuitem_weight= (WINKS_CLIENT_WIDTH - (WINKS_FIRST_MATRIXMAINMENU_X << 1)) / row_number;
    column_number = p_Matrixmenu->v_TotalMatrixItemsNum / p_Matrixmenu->v_numberbycolumn ;
    temp = p_Matrixmenu->v_TotalMatrixItemsNum %p_Matrixmenu->v_numberbycolumn;
    if(temp != 0)
    {
        column_number++;
}
    mainmenuitem_height= WINKS_CLIENT_HEIGHT / column_number + WINKS_MAINMENU_GAP ;
    for(index = 0; index < p_Matrixmenu->v_TotalMatrixItemsNum ; index++)
    {
        icon_x = (index % row_number ) * mainmenuitem_weight+ WINKS_FIRST_MATRIXMAINMENU_X;// 
        icon_y = (index / row_number ) * mainmenuitem_height+ WINKS_FIRST_MATRIXMAINMENU_Y;

        x_gap = (WINKS_MAINMENU_BACKGROUND_WIDTH - WINKS_MAINMENU_ICON_WIDTH) >> 1;
        //y_gap = (WINKS_MAINMENU_BACKGROUND_HEIGHT - WINKS_MAINMENU_ICON_HEIGHT - WINKS_ICON_TEXT_GAP) >> 1;
        y_gap = WINKS_ICON_BACKGROUND_GAP;
        if(index == p_Matrixmenu->v_FocusMatrixItemIndxInScrn -1)
        {
            icon = p_Matrixmenu->a_MenuItem[index ].a_MatrixItemIcon[1];

            
            WINKS_CLIP_ON(icon_x - x_gap , icon_y - y_gap,icon_x + WINKS_MAINMENU_BACKGROUND_WIDTH, icon_y - y_gap + WINKS_MAINMENU_BACKGROUND_HEIGHT);
            //winks_ui_drawGraphicFromFile(icon_x - x_gap, icon_y - y_gap, Winks_UIFM_GetImage(icon));
            winks_ui_drawGraphicFromFile(icon_x - x_gap, icon_y - y_gap, Winks_UIFM_GetImage(WINKS_UI_BG_MATRIXMENU_ITEM_ID));
            WINKS_CLIP_OFF(icon_x - x_gap, icon_y - y_gap ,icon_x + WINKS_MAINMENU_BACKGROUND_WIDTH, icon_y - y_gap + WINKS_MAINMENU_BACKGROUND_HEIGHT);
            icon = p_Matrixmenu->a_MenuItem[index].a_MatrixItemIcon[1];
            //winks_ui_textOutlen(icon_x + 40,icon_y + 40,Winks_UIFM_GetString(Winks_UIContext->g_Title.v_TitleStringID),winks_strlen_ucs2_type((char *)Winks_UIFM_GetString(Winks_UIContext->g_Title.v_TitleStringID)));
            WINKS_SET_TEXTCOLOR(WINKS_PIXEL_BLACK);
        }
        else
        {
            WINKS_SET_TEXTCOLOR(WINKS_PIXEL_LIGHTWHITE);
            icon = p_Matrixmenu->a_MenuItem[index].a_MatrixItemIcon[1];
        }
        
        old_title_id = WINKS_UI_BUDDYNUM_STRINGID + index;
        if(icon != 0)
        {
            WINKS_CLIP_ON(icon_x, icon_y,icon_x + WINKS_MAINMENU_ICON_WIDTH, icon_y + WINKS_MAINMENU_ICON_HEIGHT);
            winks_ui_drawGraphicFromFile(icon_x, icon_y, Winks_UIFM_GetImage(icon));
            WINKS_CLIP_OFF(icon_x, icon_y,icon_x + WINKS_MAINMENU_ICON_WIDTH, icon_y + WINKS_MAINMENU_ICON_HEIGHT);
        }
     
        stlen = winks_strlen_ucs2_type((char *)Winks_UIFM_GetString(old_title_id));
        //gb2312tounicode( ptmp, 32, Winks_UIFM_GetString(stringid), stlen );
        winks_get_string_wight_high( (char * )Winks_UIFM_GetString(old_title_id), stlen, &w, &h );
        if (w >= WINKS_MAINMENU_BACKGROUND_WIDTH)
        {
            int templen = 0;
            pc_temp = (char*)Winks_UIFM_GetString(old_title_id);
            creat_text_String_line_type((char*)Winks_UIFM_GetString(old_title_id),50,&templen);
            winks_ui_textOutlen(icon_x + 3,icon_y + 35,Winks_UIFM_GetString(old_title_id),templen);
            winks_ui_textOutlen(icon_x + 3,icon_y + 43,(const char*)(pc_temp + templen),winks_strlen_ucs2_type(pc_temp + templen));
            
        }
        else
        {
            excursion = (WINKS_MAINMENU_BACKGROUND_WIDTH - w) >> 1;
            winks_ui_textOutlen(icon_x - x_gap + excursion,icon_y + WINKS_MAINMENU_BACKGROUND_WIDTH + WINKS_ICON_TEXT_GAP - WINKS_GETFONTHEIGHT,Winks_UIFM_GetString(old_title_id),winks_strlen_ucs2_type((char *)Winks_UIFM_GetString(old_title_id)));
        }
       // winks_ui_textOutlen(icon_x + 3,icon_y + 35,Winks_UIFM_GetString(old_title_id),winks_strlen_ucs2_type((char *)Winks_UIFM_GetString(old_title_id)));
    }
    
	winks_set_font(WINKS_MEDIUM_FONT);
}



#else
/**************************************************************

	FUNCTION NAME		: winks_ui_MatrixMainmenu

  	PURPOSE		        : SHOW Matrix Mainmenu.

	INPUT PARAMETERS	: winks_Matrixmenu_Screen_s *p_Matrixmenu
	OUTPUT PARAMETERS	: null

	RETURNS				: void

	REMARKS				: null

***************************************************************/

void winks_ui_MatrixMainmenu(winks_Matrixmenu_Screen_s *p_Matrixmenu)
{
    int index;
    int temp;
    int icon_x,icon_y;
    int row_number, column_number;
    int mainmenuitem_height,mainmenuitem_weight;
    int icon;

    Winks_assert(p_Matrixmenu != NULL);

    if (winks_draw_condition(WINKS_MINGUI_HWND_USER) != WINKS_RETURN_SUCCESS)
    {
        return;
    }

    row_number = p_Matrixmenu->v_numberbycolumn;
    mainmenuitem_weight= (WINKS_CLIENT_WIDTH - WINKS_FIRST_MATRIXMAINMENU_X) / row_number;
    column_number = p_Matrixmenu->v_TotalMatrixItemsNum / p_Matrixmenu->v_numberbycolumn ;
    temp = p_Matrixmenu->v_TotalMatrixItemsNum %p_Matrixmenu->v_numberbycolumn;
    if(temp != 0)
    {
        column_number++;
    }
    mainmenuitem_height = WINKS_CLIENT_HEIGHT / column_number;
    for(index = 0; index < p_Matrixmenu->v_TotalMatrixItemsNum ; index++)
    {
        icon_x = (index % row_number ) * mainmenuitem_weight+ WINKS_FIRST_MATRIXMAINMENU_X;// 
        icon_y = (index / row_number ) * mainmenuitem_height+ WINKS_FIRST_MATRIXMAINMENU_Y;
        if(index == p_Matrixmenu->v_FocusMatrixItemIndxInScrn -1)
        {
            icon = p_Matrixmenu->a_MenuItem[index ].a_MatrixItemIcon[1];
        }
        else
        {
            icon = p_Matrixmenu->a_MenuItem[index].a_MatrixItemIcon[0];
        }
        if(icon != 0)
        {
            WINKS_CLIP_ON(icon_x, icon_y,icon_x + WINKS_MAINMENU_ICON_WIDTH, icon_y + WINKS_MAINMENU_ICON_HEIGHT);
            winks_ui_drawGraphicFromFile(icon_x, icon_y, Winks_UIFM_GetImage(icon));
            WINKS_CLIP_OFF(icon_x, icon_y,icon_x + WINKS_MAINMENU_ICON_WIDTH, icon_y + WINKS_MAINMENU_ICON_HEIGHT);
        }
    }
}


#endif
#endif
/**************************************************************

	FUNCTION NAME		: winks_ui_create_scrolling_text

  	PURPOSE		        : create scrolling text.

	INPUT PARAMETERS	: winks_scrolling_text_s *s
						: int x,
       					: int y,
				        : int width,
				        : int height,
				        : char* text
				        : winks_color text_color
	OUTPUT PARAMETERS	: null

	RETURNS				: void

	REMARKS				: null

***************************************************************/

void winks_ui_create_scrolling_text(
        winks_scrolling_text_s *s,
        int x,
        int y,
        int width,
        int height,
        char* text,
        void (*timer_callback) (void),
        //void (*draw_background) (int x1, int y1, int x2, int y2),
        winks_color text_color,
        Winks_string_scrollway_e string_scrollway,
        winks_color backgroud_color,
        char * draw_backgroundimage,
        Winks_font_feature_s  font_feature
        )
{
    //char ptmp[500];
    //memset(ptmp, 0, 500);
//char * p_text = NULL;
//winks_scrolling_text_s * p_winks_scrolling_text = NULL;
//winks_scrolling_text_s * p_temp = NULL;

WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    Winks_assert(s != NULL);
#if 0
	    p_winks_scrolling_text = Winks_Alloc(sizeof(winks_scrolling_text_s));
	
	if(p_winks_scrolling_text == NULL)
	{
     
		Winks_printf("Error: Allocate memory failed!\n");
		return ;
	}
    
    
  
	if (Winks_UIContext->winks_scroll_string_head == NULL) 
	{
		Winks_UIContext->winks_scroll_string_head = p_winks_scrolling_text;
	}
	else
	{
		p_temp = Winks_UIContext->winks_scroll_string_head;
		while (p_temp->p_next != NULL)
		{
			p_temp = p_temp->p_next;
		}
		p_temp->p_next = p_winks_scrolling_text;
	}
#endif
    //s = &p_winks_scrolling_text->winks_scroll_string;
    s->x = x;
    s->y = y;
    s->scroll_x = 0;
    s->width = width;
    s->height = height;
    s->text = text;
    
    s->text_color = text_color;
    s->backgroud_color = backgroud_color;
    s->draw_backgroundimage = draw_backgroundimage;
    //s->text_border_color = border_color;
    /* s->text_font=&MMI_default_font; */
    //s->text_font = *UI_font;
    s->scroll_counter = 4;  
    s->scroll_size = WINKS_UI_TEXT_MENUITEM_SCROLL_SIZE;
    s->scroll_gap = WINKS_UI_TEXT_MENUITEM_SCROLL_GAP;
    s->scroll_pause = WINKS_UI_TEXT_MENUITEM_SCROLL_PAUSE;
    //gb2312tounicode( ptmp, 500, text, strlen(text) );
    //AnsiiToUnicodeString(ptmp,text);
    //gui_set_font(&s->text_font);
    //gui_measure_string(text, &s->text_width, &s->text_height);
    winks_get_string_wight_high(text, winks_strlen_ucs2_type(text), &s->text_width, &s->text_height);
    /*
     * if ( s->width < s->text_width )
     * s->width = s->text_width;
     * if (s->height < s->text_height )
     * s->height = s->text_height;
     */
    /* s->graphics_context = current_LCD_device_bitmap;
    if (timer_callback == NULL)
    {
        timer_callback = UI_dummy_function;
    }
    s->timer_callback = timer_callback;*/
#if 0
    if(0)
    	{
    		char *temp= NULL;
			
		int word_number = 	winks_strlen_ucs2_type(text) >> 1;
		temp = temp + winks_strlen_ucs2_type(text) -2 -1;
		p_text = Winks_salAlloc(winks_strlen_ucs2_type(text) +2);
		Winks_mem_set(p_text,0,(winks_strlen_ucs2_type(text) +2));
		for (int i = 0; i < winks_strlen_ucs2_type(text); i++) 
			{
				p_text[i] = text[i];
			}
		#if 0
		for (int i = 0; i < (word_number >> 1); i++) 
		{
			*(text+ i) =  *(temp);
			*(text+ i + 1) = *(temp +1);
			temp -= 2;
		}
		#endif	
    		
    	}
#endif
	//s->text = p_text;
    s->scroll_time = WINKS_UI_TEXT_MENUITEM_SCROLL_TIME;
    s->flags = WINKS_SCROLLING_TEXT_TWO_DIRECTION_SCROLL;
    s->flags |= string_scrollway;//WINKS_SCROLLING_TEXT_SCROLL_TRAIL_TO_HEAD;
    s->timer_callback = timer_callback;
    s->font_feature = font_feature;
}


/**************************************************************

	FUNCTION NAME		: winks_ui_redraw_scrolling_text

  	PURPOSE		        : redraw scrolling text.

	INPUT PARAMETERS	: winks_scrolling_text_s *s
	OUTPUT PARAMETERS	: null

	RETURNS				: void

	REMARKS				: null

***************************************************************/

void winks_ui_redraw_scrolling_text(winks_scrolling_text_s *s)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int x1, y1, x2, y2;
    int cx1, cy1, cx2, cy2;
    int excursion = 0;
    //winks_Rect_s v_rect;

    Winks_assert(s != NULL);
    
    if ((winks_draw_condition(WINKS_MINGUI_HWND_USER) != WINKS_RETURN_SUCCESS)
        &&(winks_draw_condition(WINKS_MINGUI_HWND_CALL) != WINKS_RETURN_SUCCESS))
    {
        return;
    }
    WINKS_BUFFER_ON;
    //winks_set_font(WINKS_MEDIUM_FONT);
    Winks_set_font_feature(&(s->font_feature)); 
    
    //bitmap *saved_graphics_context = current_LCD_device_bitmap;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /*if (s->graphics_context != current_LCD_device_bitmap)
    {
    if (s->graphics_context == &main_LCD_device_bitmap)
    {
        UI_set_main_LCD_graphics_context();
    }
    else if (s->graphics_context == &sub_LCD_device_bitmap)
    {
        UI_set_sub_LCD_graphics_context();
    }
    }*/
    x1 = s->x;
    y1 = s->y;
    x2 = x1 + s->width - 1;
    y2 = y1 + s->height - 1;
    if (s->flags & WINKS_SCROLLING_TEXT_BORDERED_TEXT)
    {
        cx1 = x1 - 2;
        cy1 = y1 - 1;
        cx2 = x2 + 2;
        cy2 = y2 + 1;
    }
    else
    {
        cx1 = x1;
        cy1 = y1;
        cx2 = x2;
        cy2 = y2;
    }
    excursion = (cy2 - cy1 - WINKS_GETFONTHEIGHT) >> 1;
    if(s->draw_backgroundimage == NULL)
    {
        if (s->backgroud_color == WINKS_PIXEL_TRANSPARENT) 
        {
            winks_clear_time_fillRect(cx1, cy1, cx2, cy2);
        }
        else
        {
            winks_ui_fillRect(cx1, cy1, cx2, cy2,s->backgroud_color);
        }
        
    }
    else
    {
        winks_ui_drawGraphicFromFile(cx1, cy1, s->draw_backgroundimage);
    }
    /* gui_push_clip();
    gui_push_text_clip();
    gui_set_text_clip(cx1, cy1, cx2, cy2);
    gui_set_clip(cx1, cy1, cx2, cy2);
    s->draw_background(cx1, cy1, cx2, cy2);
    gui_set_font(&s->text_font);*/
    // WINKS_CLIP_ON
    // gui_measure_string(s->text, &sw, &sh);
    /* if (r2lMMIFlag)
    {
    gui_move_text_cursor(s->x + s->scroll_x + s->width, s->y);
    }
    else
    {*/
    //   gui_move_text_cursor(s->x + s->scroll_x, s->y);
    /*}*/
    //gui_set_text_color(s->text_color);
    // gui_set_line_height(sh);
    if (s->flags & WINKS_SCROLLING_TEXT_BORDERED_TEXT)
    {
        //    gui_set_text_border_color(s->text_border_color);

        //   gui_print_bordered_text(s->text);
        if (s->flags & WINKS_SCROLLING_TEXT_ENABLE_SCROLL)
        {    
            WINKS_CLIP_ON(cx1, cy1, cx2, cy2);
            //winks_ui_textOutlen(s->x + s->scroll_x + s->text_width + s->scroll_gap, s->y, s->text, strlen(s->text));
            winks_ui_textOutlen(s->x + s->scroll_x +3 , s->y, s->text, winks_strlen_ucs2_type(s->text));
            WINKS_CLIP_OFF(cx1, cy1, cx2, cy2);
        } //s->scroll_gap




    }
    else
    {
#if 0
	    // gui_print_text(s->text);
	    if ((s->flags & UI_SCROLLING_TEXT_MARQUEE_SCROLL) && (s->flags & UI_SCROLLING_TEXT_ENABLE_SCROLL))
	    {
	        /* MTK Elvis for R2L characters */
	        if (r2lMMIFlag)
	        {
	            gui_move_text_cursor(s->x + s->scroll_x + s->width - (s->text_width + s->scroll_gap), s->y);
	        }
	        else
	        {
	            gui_move_text_cursor(s->x + s->scroll_x + s->text_width + s->scroll_gap, s->y);
	        }
	        /* MTK end */
	        gui_set_line_height(sh);
	        gui_print_text(s->text);
	    }
#endif
    }
    /*gui_pop_clip();
    gui_pop_text_clip();*/
    // WINKS_CLIP_OFF(cx1, cy1, cx2, cy2);

    /*if (saved_graphics_context != current_LCD_device_bitmap)
    {
    if (saved_graphics_context == &main_LCD_device_bitmap)
    {
        UI_set_main_LCD_graphics_context();
    }
    else if (saved_graphics_context == &sub_LCD_device_bitmap)
    {
        UI_set_sub_LCD_graphics_context();
    }
    }*/
    //if (s->flags & WINKS_SCROLLING_TEXT_ENABLE_SCROLL)
    {    
        
        WINKS_CLIP_ON(cx1, cy1, cx2, cy2);
        WINKS_SET_TEXTCOLOR(s->text_color);
        WINKS_SET_BKMODE( WINKS_BM_TRANSPARENT);  
        
        //winks_ui_textOutlen(s->x + s->scroll_x + WINKS_UI_TEXT_FIRST_SCROLL_GAP  , s->y + excursion, s->text, winks_strlen_ucs2_type(s->text));
        winks_ui_textOutlen(s->x + s->scroll_x, s->y + excursion, s->text, winks_strlen_ucs2_type(s->text));
        //if(s->flags & WINKS_SCROLLING_TEXT_SCROLL_TRAIL_TO_HEAD)
        {
            winks_ui_textOutlen(s->x + s->scroll_x + s->text_width + s->scroll_gap, s->y + excursion, s->text, winks_strlen_ucs2_type(s->text));
        }
		
        WINKS_CLIP_OFF(cx1, cy1, cx2, cy2);//s->scroll_gap
    }
#if 0
    v_rect.v_Bottom = cy2;
    v_rect.v_Left = cx1;
    v_rect.v_Right = cx2;
    v_rect.v_Top = cy1;
    Winks_bitblt(& v_rect);
#endif
	WINKS_BUFFER_OFF;
}

/*****************************************************************************
 * FUNCTION
 *  winks_ui_handle_scrolling_text_internal
 * DESCRIPTION
 *  
 * PARAMETERS
 *  s       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void winks_ui_handle_scrolling_text_internal(winks_scrolling_text_s *s)
{

    Winks_assert(s != NULL);
    
    if (s->scroll_counter)
    {
        s->scroll_counter--;
    }
    else
    {
        //if (!(s->flags & WINKS_SCROLLING_TEXT_SCROLL_DIRECTION_RIGHT))
       // {
            
            if(s->flags & WINKS_SCROLLING_TEXT_SCROLL_TRAIL_TO_HEAD)
            {
            s->scroll_x -= s->scroll_size;
                if ((s->scroll_x + s->text_width + s->scroll_gap) <= 0)
                {
                    // s->flags |= WINKS_SCROLLING_TEXT_SCROLL_DIRECTION_RIGHT;
                        
                        
                    s->scroll_x = 0;//s->scroll_size;//-( s->text_width - s->width + s->scroll_size);
                    s->scroll_counter = s->scroll_pause;
                	 
                } 
            }
			#if 0
            else if ((s->scroll_x + s->text_width + s->scroll_gap) <= s->width)
            {
                s->scroll_x = s->scroll_size;//-( s->text_width - s->width + s->scroll_size);
                s->scroll_counter = s->scroll_pause;
            }
                    #endif
        //}
        else if (s->flags & WINKS_SCROLLING_TEXT_SCROLL_DIRECTION_RIGHT)
        {
		if(s->flags & WINKS_SCROLLING_TEXT_TWO_DIRECTION_SCROLL)
		{
			s->scroll_x -= s->scroll_size;
			if ((s->scroll_x + s->text_width + s->scroll_gap) <= s->width)
			{
				// s->flags |= WINKS_SCROLLING_TEXT_SCROLL_DIRECTION_RIGHT;
				    

				s->scroll_x += s->scroll_size;//s->scroll_size;//-( s->text_width - s->width + s->scroll_size);
				//s->scroll_counter = s->scroll_pause;
				s->flags &= ~WINKS_SCROLLING_TEXT_TWO_DIRECTION_SCROLL;

        }
        }

		else
        {
            s->scroll_x += s->scroll_size;
            if (s->scroll_x >= 0)
            {
	            	s->scroll_counter = 4;//s->scroll_pause;
	                s->flags |= WINKS_SCROLLING_TEXT_TWO_DIRECTION_SCROLL;
	                //s->scroll_counter = s->scroll_pause;
	            }
            }
        }
    }
    s->flags |= WINKS_SCROLLING_TEXT_DISPLAY_SCROLL;
    winks_ui_redraw_scrolling_text(s);
    // s->flags &= ~WINKS_SCROLLING_TEXT_DISPLAY_SCROLL;
    //  }
    //    gui_start_timer(s->scroll_time, s->timer_callback);
}

/**************************************************************

	FUNCTION NAME		: winks_ui_handle_scrolling_text

  	PURPOSE		        : handle scrolling text.

	INPUT PARAMETERS	: null
	OUTPUT PARAMETERS	: null

	RETURNS				: void

	REMARKS				: null

***************************************************************/

void winks_ui_handle_scrolling_text(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    winks_scrolling_text_s *s; 

    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    s = &Winks_UIContext->winks_scroll_string;
	if( s->timer_id == Winks_UIContext->gv_curtimerid)
    winks_ui_handle_scrolling_text_internal(s);

}

/**************************************************************

	FUNCTION NAME		: winks_ui_handle_scrolling_text_colorcall

  	PURPOSE		        : handle scrolling text.

	INPUT PARAMETERS	: null
	OUTPUT PARAMETERS	: null

	RETURNS				: void

	REMARKS				: null

***************************************************************/

void winks_ui_handle_scrolling_text_colorcall(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    winks_scrolling_text_s *s;
	winks_scrolling_text_s *p_temp;
    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()
winks_ui_handle_scrolling_text();
    s = Winks_UIContext->winks_scroll_string_head;
	if (s == NULL) 
	{
		return;
	}
	else
	{
		p_temp = Winks_UIContext->winks_scroll_string_head;
		while (p_temp != NULL) 
		{
			if (p_temp->timer_id == Winks_UIContext->gv_curtimerid)
				{
					    winks_ui_handle_scrolling_text_internal(p_temp);
						break;
				}
			p_temp = p_temp->p_next;
		}
		//p_temp->p_next = p_winks_scrolling_text;
	}

    //winks_ui_handle_scrolling_text_internal(s);
}

/*****************************************************************************
 * FUNCTION
 *  winks_ui_scrolling_text_start
 * DESCRIPTION
 *  
 * PARAMETERS
 *  s       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
 
void winks_ui_scrolling_text_start(winks_scrolling_text_s *s)
{
    // WINKS_TIMER_ID v_timeid;

	WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    Winks_assert(s != NULL);
    
    
    if(s->scroll_pause == 0)
    {
        if (s->text_width > s->width)
        {
            s->scroll_x = 0;

            if ((s->flags & WINKS_SCROLLING_TEXT_TWO_DIRECTION_SCROLL))
            {
                //s->flags |= WINKS_SCROLLING_TEXT_ENABLE_SCROLL;
                //s->flags &= ~WINKS_SCROLLING_TEXT_SCROLL_DIRECTION_RIGHT;
                //s->scroll_counter = s->scroll_pause;
                // gui_start_timer(s->scroll_time, s->timer_callback);

				s->timer_id = Winks_CreateTimer(Winks_UIContext->winks_ui_chn,  s->scroll_time, WINKS_TIMER_AUTO_LOOP);

	Winks_StartTimer(s->timer_id );
                #if 0//waiting 
                winks_ui_start_timer(WINKS_UI_SCROOL_STRING, s->scroll_time, WINKS_TIMER_AUTO_LOOP,s->timer_callback);

                #endif
            }
            //	winks_ui_handle_scrolling_text(s);
           // winks_ui_handle_scrolling_text();
            //winks_ui_redraw_scrolling_text(s);
        }
        /*modified by test*/
        //s->timer_callback();
        s->scroll_pause = 1;
    }
}
/*****************************************************************************
 * FUNCTION
 *  winks_ui_scrolling_text_start
 * DESCRIPTION
 *  
 * PARAMETERS
 *  s       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
 
void winks_ui_scrolling_text_resume(winks_scrolling_text_s *s)
{
    // WINKS_TIMER_ID v_timeid;
    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    //Winks_assert(s != NULL);
    if(s == NULL)return;
    //Winks_UIContext->scrooltext_flag = 0;
    if(s->scroll_pause == 1)
    {
        if (s->text_width > s->width)
        {
          
            	s->timer_id = Winks_CreateTimer(Winks_UIContext->winks_ui_chn,  s->scroll_time, WINKS_TIMER_AUTO_LOOP);

	Winks_StartTimer(s->timer_id );
            //	winks_ui_handle_scrolling_text(s);
            // winks_ui_handle_scrolling_text();
            //winks_ui_redraw_scrolling_text(s);

            /*modified by test*/
            //s->timer_callback();

            s->scroll_pause = 0;
             
         }
       
    }
}


/*****************************************************************************
 * FUNCTION
 *  winks_ui_scrolling_text_stop
 * DESCRIPTION
 *  
 * PARAMETERS
 *  s       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
 
void winks_ui_scrolling_text_stop(winks_scrolling_text_s *s)
{
    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    Winks_assert(s != NULL);
    
    if (s->flags & WINKS_SCROLLING_TEXT_ENABLE_SCROLL)
    {
        //        gui_cancel_timer(s->timer_callback);
        winks_ui_destroy_timer(WINKS_UI_SCROOL_STRING);
        s->flags &= ~WINKS_SCROLLING_TEXT_ENABLE_SCROLL;
    }
    Winks_UIContext->scrooltext_flag = 0;
    //s->scroll_pause = 0;
    Winks_mem_set(s, 0, sizeof(winks_scrolling_text_s));
}
void winks_ui_scrolling_colorcalltext_stop(winks_scrolling_text_s *s)
{
	winks_scrolling_text_s * p_winks_scrolling_text = NULL;

    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    //Winks_assert(s != NULL);
	if (s == NULL) 
		return ;
#if 0
    if (s->flags & WINKS_SCROLLING_TEXT_ENABLE_SCROLL)
    {
        //        gui_cancel_timer(s->timer_callback);
        winks_ui_destroy_timer(WINKS_UI_SCROOL_STRING);
        s->flags &= ~WINKS_SCROLLING_TEXT_ENABLE_SCROLL;
    }
    Winks_UIContext->scrooltext_flag = 0;
		    p_winks_scrolling_text = Winks_Alloc(sizeof(winks_scrolling_text_list_s));
	
	if(p_winks_scrolling_text == NULL)
	{
     
		Winks_printf("Error: Allocate memory failed!\n");
		return ;
	}

	if (s->flags & WINKS_SCROLLING_TEXT_ENABLE_SCROLL)
    {
        //        gui_cancel_timer(s->timer_callback);
		if (s->timer_id != 0) 
			Winks_DestroyTimer(s->timer_id);
        s->flags &= ~WINKS_SCROLLING_TEXT_ENABLE_SCROLL;
    }
#endif
#if 1
    	if (Winks_UIContext->winks_scroll_string_head != NULL) 
    		{
	    		p_winks_scrolling_text = Winks_UIContext->winks_scroll_string_head;
				do{
					Winks_UIContext->winks_scroll_string_head = p_winks_scrolling_text;
					Winks_DestroyTimer(p_winks_scrolling_text->timer_id);
					
					p_winks_scrolling_text = p_winks_scrolling_text->p_next;
					Winks_Free(Winks_UIContext->winks_scroll_string_head);
					
					
			}while(p_winks_scrolling_text);
				Winks_UIContext->winks_scroll_string_head = NULL;
	    		
    		}
    
#endif
    //s->scroll_pause = 0;
    //Winks_mem_set(s, 0, sizeof(winks_scrolling_text_s));
Winks_UIContext->scrooltext_flag = 0;
}
/*****************************************************************************
 * FUNCTION
 *  winks_ui_scrolling_text_suspend
 * DESCRIPTION
 *  
 * PARAMETERS
 *  s       [?]     
 * RETURNS
 *  void
 *****************************************************************************/

void winks_ui_scrolling_text_suspend(winks_scrolling_text_s *s)
{
    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    //Winks_assert(s != NULL);
    if (s == NULL)
		return ;
    if (s->flags & WINKS_SCROLLING_TEXT_ENABLE_SCROLL)
    {
        //        gui_cancel_timer(s->timer_callback);
        		if (s->timer_id != 0) 
			Winks_DestroyTimer(s->timer_id);
        s->flags &= ~WINKS_SCROLLING_TEXT_ENABLE_SCROLL;
    }
    //Winks_UIContext->scrooltext_flag = 1;
    s->scroll_pause = 1;
}

/*****************************************************************************
 * FUNCTION
 *  winks_ui_show_scrolling_text
 * DESCRIPTION
 *  
 * PARAMETERS
 *  s       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
 
void winks_ui_show_scrolling_text(winks_scrolling_text_s *s)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    winks_ui_redraw_scrolling_text(s);
    winks_ui_scrolling_text_start(s);
}




#if 1 
/**************************************************************

	FUNCTION NAME		: winks_if_is_separator

  	PURPOSE		        : if is a separator..

	INPUT PARAMETERS	: char * string
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

int winks_if_is_separator(char * string)
{

    int i = 0;

    Winks_assert(string != NULL);
    
    switch(WINKS_ENCODE_TYPE)
    {
        case WINKS_UNICODE:
                            {
                                if (((*string == '\\')&&(*(string + 1) == 0))
									&&((*(string + 2) == 'n')&&(*(string + 3) == 0)))
                                {
                                    i = WINKS_SEPARATOR_FLAG;
                                }
                            }
             break;
        case WINKS_GB2312:
                            i = 0;
             break;
    }
    return i;
}
/**************************************************************

	FUNCTION NAME		: winks_if_is_blank

  	PURPOSE		        : if is a blank.

	INPUT PARAMETERS	: char * string
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

int winks_if_is_blank(char * string)
{

    int i = 0;

    Winks_assert(string != NULL);
    
    switch(WINKS_ENCODE_TYPE)
    {
        case WINKS_UNICODE:
                            {
                                if(((*string == '\\')&&(*(string + 1) == 0))
									&&((*(string + 2) == 'n')&&(*(string + 3) == 0))
									&&((*(string + 4) == ' ')&&(*(string + 5) == 0))
									||((*(string ) == ' ')&&(*(string + 1) == 0)))
                                {
                                    i = WINKS_BLANK_FLAG;
                                }
                            }
             break;
        case WINKS_GB2312:
                            i = 0;
             break;
    }
    return i;
}
/**************************************************************

	FUNCTION NAME		: winks_separator_len

  	PURPOSE		        : get a separator len.

	INPUT PARAMETERS	: 
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/
int winks_separator_len()
{

    int i = 0;
    
    switch(WINKS_ENCODE_TYPE)
    {
        case WINKS_UNICODE:
                            {
                               
                                i = 4;
                                  
                            }
             break;
        case WINKS_GB2312:
                            i = 0;
             break;
    }
    return i;
}

/**************************************************************

	FUNCTION NAME		: winks_blank_len

  	PURPOSE		        : get a blank len.

	INPUT PARAMETERS	: 
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/
int winks_blank_len()
{

    int i = 0;
    
    switch(WINKS_ENCODE_TYPE)
    {
        case WINKS_UNICODE:
                            {
                               
                                i = 2;
                                  
                            }
             break;
        case WINKS_GB2312:
                            i = 0;
             break;
    }
    return i;
}
/**************************************************************

	FUNCTION NAME		: creat_text_String_line_type

  	PURPOSE		        : creat a line's text.

	INPUT PARAMETERS	: char* text
	                      int line_width
	                      int* string_lenth
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

int creat_text_String_line_type(char* text,int line_width,int* string_lenth)
{
    char * temp;
    int currentline_width = 0;
    int currentword_lenth = 0;
    int currentword_width = 0;
    int remain_linelenth = 0;
    int line_lenth = 0;
    int flag = 0;

    Winks_assert(text != NULL);
    
    temp = text;
    remain_linelenth = line_width;
    while (currentline_width + currentword_width < line_width )
    {
        currentline_width = currentline_width + currentword_width;
        temp = temp + currentword_lenth;
        line_lenth = line_lenth + currentword_lenth;
        /*可以判断一下分割符*/
        if (winks_if_is_separator(temp))
        {
            flag |= winks_if_is_separator(temp);
            break;
        }   
        if (*temp == 0 && *(temp + 1) == 0 )
            break;

        currentword_lenth = winks_get_text_string_oneword_lenth(temp);
        currentword_width = winks_get_text_string_oneword_width (temp, currentword_lenth);        
                    
    } 
    if (winks_if_is_separator(temp))
    {
        flag |= winks_if_is_separator(temp);

    }
    if (winks_if_is_blank(temp))
    {
        flag |= winks_if_is_blank(temp);

    }
    if( (int)(temp - text) == 0 && currentword_width >= line_width )
    {
        currentline_width = 0;
        currentword_width = 0;
		currentword_lenth = 0;
		line_lenth = 0;
        while (currentline_width + currentword_width < line_width )
        {
            currentline_width = currentline_width + currentword_width;
            temp = temp + currentword_lenth;
            line_lenth = line_lenth + currentword_lenth;
            /*可以判断一下分割符*/

            if (*temp == 0 && *(temp + 1) == 0 )
                break;

            currentword_lenth = (int)winks_get_char_byte_number();
            currentword_width = winks_get_text_string_oneword_width (temp, currentword_lenth);        
                        
         }
    }
   *string_lenth = line_lenth;
   return flag;
}

/**************************************************************

	FUNCTION NAME		: winks_creat_text_string_rect_type

  	PURPOSE		        : creat a multiline's text.

	INPUT PARAMETERS	: winks_text_String_rect_s * rect_text_String
	                      winks_Rect_s* pRect
	                      char TextAlign
	                      char * string_text
	                      int string_text_lenth
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

int winks_creat_text_string_rect_type(winks_text_String_rect_s * rect_text_String, winks_Rect_s* pRect, WINKS_SHOW_FLAG TextAlign, char * string_text, int string_text_lenth)
{
    char * temp;
    //		  int y = 0;
    //		  int NumLines;
    //const char  *sOrg =s;
    int FontYSize,FontCharSize;
    int  char_tap = 0;
    // int xLine = 0;
    int  string_x = 0;
    int  string_y = 0;
    int linewidth = 0;
    int lineheight = 0;
	int i = 0;
    // int LineLen = 0;
    // int NumCharsRem;           /* Number of remaining characters */
    int num_in_line = 0;
    // 	  int remain_number = 0;
    int linenumber = 0;
    char * text = (char  *)string_text;
    int current_textlen = 0;
    int current_textlinelen = 0;
    int flag;

    //char flag;
    rect_text_String->v_ShowFlag = TextAlign;
    //textlen = MaxNumChars;
    FontYSize = WINKS_GETFONTHEIGHT;
    FontCharSize = WINKS_GETFONTHEIGHT >> 1;
    lineheight= WINKS_GETFONTHEIGHT;
    linenumber = (pRect->v_Bottom - pRect->v_Top) / lineheight;
    linewidth = pRect->v_Right - pRect->v_Left;
   
    num_in_line = (linewidth+char_tap) / (FontCharSize + char_tap);
    rect_text_String->line_number_area = linenumber;

    //r = *pRect;
    string_x = pRect->v_Left;
    string_y = 	pRect->v_Top;
    temp = text;
    #if 1
    for(i = 0; current_textlen < string_text_lenth; i++ ) 
    {
      
        flag = creat_text_String_line_type(temp,linewidth,&current_textlinelen);
        if (flag & WINKS_SEPARATOR_FLAG)
        {
            current_textlinelen = current_textlinelen + winks_separator_len();
            
        }
        if (flag & WINKS_BLANK_FLAG)
        {
            current_textlinelen = current_textlinelen + winks_blank_len();
        
        }
        temp = temp + current_textlinelen;
        current_textlen = current_textlen + current_textlinelen;
        
    }

    current_textlen = 0;
    temp = text;

    if (i > linenumber)
    {
        winks_text_String_line_s * p_temp = NULL;
	linewidth = linewidth - WINKS_SLIDESCROLLBAR_WIDTH;	
        p_temp = (winks_text_String_line_s*)Winks_salAlloc(i * sizeof(winks_text_String_line_s));
        if(p_temp == NULL)
        {
            Winks_printf("Winks_salAlloc  winks_text_String_line_s data error##############!\n");
            Winks_assert(p_temp != NULL);
        }
        else
        {
            rect_text_String->p_lines = p_temp;

            for(i = 0; current_textlen < string_text_lenth; i++ ) 
            {
                rect_text_String->p_lines[i].p_string = temp;
                flag = creat_text_String_line_type(temp,linewidth,&current_textlinelen);
                rect_text_String->p_lines[i].v_stinglenth = current_textlinelen;
                if (flag & WINKS_SEPARATOR_FLAG)
                {
                    current_textlinelen = current_textlinelen + winks_separator_len();

                }
                if (flag & WINKS_BLANK_FLAG)
                {
                    current_textlinelen = current_textlinelen + winks_blank_len();

                }



                temp = temp + current_textlinelen;
                current_textlen = current_textlen + current_textlinelen;
            }
        }
       
    }
    #endif
    else 
    {
        
        for(i = 0; current_textlen < string_text_lenth; i++ ) 
        {
            rect_text_String->a_linesData[i].p_string = temp;
            flag = creat_text_String_line_type(temp,linewidth,&current_textlinelen);
            rect_text_String->a_linesData[i].v_stinglenth = current_textlinelen;
            if (flag & WINKS_SEPARATOR_FLAG)
            {
                current_textlinelen = current_textlinelen + winks_separator_len();
                
            }
            if (flag & WINKS_BLANK_FLAG)
            {
                current_textlinelen = current_textlinelen + winks_blank_len();
                
            }
            
           
            temp = temp + current_textlinelen;
            current_textlen = current_textlen + current_textlinelen;
        }
        rect_text_String->p_lines = NULL;
    }
    rect_text_String->v_FirstlineIndx = 0;
    rect_text_String->v_Stringline_talNum = i - 1;
    WINKS_GETCHARWIDTH(*text);
    return 0; 
}

/**************************************************************

	FUNCTION NAME		: winks_get_char_byte_number

  	PURPOSE		        : get a char's byte number.

	INPUT PARAMETERS	: 
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

int winks_get_char_byte_number()
{
    //if(Winks_GetCurrentLanguage() == 0)
    //    return 1;
    int i = 0;
    switch(WINKS_ENCODE_TYPE)
    {
       case WINKS_UNICODE:
                            i = 2;
             break;
       case WINKS_GB2312:
                            i = 1;
             break;
    }
    return i;
}

/**************************************************************

	FUNCTION NAME		: winks_get_word_byte_number

  	PURPOSE		        : get a word's byte number.

	INPUT PARAMETERS	: 
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

int winks_get_word_byte_number()
{

    int i = 0;
    switch(WINKS_ENCODE_TYPE)
    {
        case WINKS_UNICODE:
                            i = 2;
             break;
        case WINKS_GB2312:
                            i = 2;
             break;
    }
    return i;
}


/**************************************************************

	FUNCTION NAME		: winks_get_text_string_oneword_width

  	PURPOSE		        : get a word's width by pixel.

	INPUT PARAMETERS	: char* text
	                      int lenth
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/
/*返回一个word所占的字节数 width返回word显示所占的宽度*/
int winks_get_text_string_oneword_width(char* text, int lenth)
{
    int width;
    int HEIGHT;
    char string[256];

    Winks_assert(text != NULL);
    
    Winks_mem_set(string, 0 , 256);
    Winks_mem_cpy(string,text,lenth);
    winks_get_string_wight_high(string,lenth,&width,&HEIGHT);
    return width;
}

/**************************************************************

	FUNCTION NAME		: winks_get_text_string_oneword_lenth

  	PURPOSE		        : get a word's lenth.

	INPUT PARAMETERS	: char* text

	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

int winks_get_text_string_oneword_lenth(char* text)
{
    
    int lenth;
    char * temp = NULL;

    Winks_assert(text != NULL);
    
    temp = text;
    #if 1
    if((unsigned char)*(temp + 1) == 0x00) //)(unsigned char)*(src) <= 0x7f
    {
        if(*temp ==0 &&*(temp + 1) ==0) return 0;
        if (*temp ==' ')
        {
             lenth = (int)winks_get_char_byte_number();
        }
        else
        {
	    int i = 0;
            for (temp = text; (*temp !=0 && *temp !=' ') && (unsigned char)*(temp + 1) == 0x00; temp =  temp + winks_get_char_byte_number())
			{
				i++;
			}  
            lenth = (int)((int)temp - (int)text);
        }
    }
    else
        #endif
    {
        lenth = (int)winks_get_word_byte_number();
    }
        
    return lenth;
}



#if 0
/**************************************************************

	FUNCTION NAME		: winks_show_multiline_text

  	PURPOSE		        : show multiline text.

	INPUT PARAMETERS	: char* text
                          int text_lenth
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void winks_show_multiline_text(char* text, int text_lenth)
{
    winks_Rect_s Rect;
    //winks_vertical_scrollbar_s scrollbar;
    WINKS_SHOW_FLAG TextAlign = 0;
    int linenumber = 0;
    
    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    TextAlign = WINKS_SHOW_LEFT;
    Rect.v_Bottom = WINKS_MULTILINE_TEXT_Y + WINKS_MULTILINE_TEXT_HEIGHT;
    Rect.v_Left = WINKS_MULTILINE_TEXT_X;
    Rect.v_Right = WINKS_MULTILINE_TEXT_X + WINKS_MULTILINE_TEXT_WIDTH;
    Rect.v_Top = WINKS_MULTILINE_TEXT_Y;
    //winks_ui_drawGraphicFromFile(WINKS_INFOBOX_X ,WINKS_INFOBOX_Y , Winks_UIFM_GetImage(WINKS_INFOBOX_BGICON));//(p_infobox->v_bgicon)
    WINKS_SET_TEXTCOLOR(WINKS_PIXEL_LIGHTWHITE);
    winks_clear_screen_fillRect(Rect.v_Left,Rect.v_Top,Rect.v_Right, Rect.v_Bottom);
    //WINKS_SET_TEXTCOLOR(WINKS_PIXEL_BLACK);
    //winks_ui_DrawstringInRect(Winks_UIFM_GetString(p_infobox->v_InfoboxStringId), &Rect, TextAlign, strlen(Winks_UIFM_GetString(p_infobox->v_InfoboxStringId)));
    //winks_ui_fillRect(WINKS_CLIENT_X,WINKS_CLIENT_Y,WINKS_CLIENT_X + WINKS_CLIENT_WIDTH, WINKS_CLIENT_Y + WINKS_CLIENT_HEIGHT, WINKS_PIXEL_LIGHTWHITE );
    if (Winks_UIContext->gv_OneTimeFlag == 1)
    {
        winks_creat_text_string_rect_type(&(Winks_UIContext->text_String_rect), &Rect, TextAlign, text, text_lenth);
        Winks_UIContext->gv_OneTimeFlag = 0;
    }

    winks_show_string_in_rect(&Winks_UIContext->text_String_rect, &Rect, TextAlign);
    linenumber = (WINKS_MULTILINE_TEXT_HEIGHT) /(WINKS_GETFONTHEIGHT + WINKS_LINE_GAP);
    if(Winks_UIContext->text_String_rect.v_Stringline_talNum >= linenumber)
    {
        winks_ui_create_vertical_scrollbar(&(Winks_UIContext->gv_scrollbar), Winks_UIContext->text_String_rect.v_FirstlineIndx + 1, Winks_UIContext->text_String_rect.v_Stringline_talNum + 1, linenumber);
        winks_ui_drawVerticalScrollBar(&(Winks_UIContext->gv_scrollbar));
    }

}

#endif
/**************************************************************

	FUNCTION NAME		: winks_ui_multiline_text_stop

  	PURPOSE		        : handle memry malloced.

	INPUT PARAMETERS	: winks_text_String_rect_s * rect_text_String
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/
 
void winks_ui_multiline_text_stop(winks_text_String_rect_s * rect_text_String)
{
    Winks_assert(rect_text_String != NULL);
    
    if(rect_text_String->p_lines != NULL)
    {
        Winks_salFree(rect_text_String->p_lines);
        rect_text_String->p_lines = NULL;
    }
}

#if 0
/**************************************************************

	FUNCTION NAME		: Winks_UI_Goto_Previous_multiline_text_line

  	PURPOSE		        : Go to Previous line.

	INPUT PARAMETERS	: winks_text_String_rect_s *p_multiline_text
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void Winks_UI_Goto_Previous_multiline_text_line(winks_text_String_rect_s *p_multiline_text)
{
    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    Winks_assert(p_multiline_text != NULL);
    
    if (p_multiline_text->v_FirstlineIndx > 0 )    
    {	

        p_multiline_text->v_FirstlineIndx = p_multiline_text->v_FirstlineIndx - 1;;

    }
    if (Winks_UIContext->text_String_rect.v_Stringline_talNum >= WINKS_MULTILINE_MAX_NUMBER)
    {
        Winks_UIContext->gv_scrollbar.up_button.flags = WINKS_UI_BUTTON_STATE_DOWN;
    }

#ifdef __WINKS_MULTI_LAYS__    
    WINKS_BUFFER_ON;
    winks_show_multiline_text(NULL, 0);
    WINKS_BUFFER_OFF;;
#else
    winks_Refresh_Full_Screen();
#endif
   

}


/**************************************************************

	FUNCTION NAME		: Winks_UI_Goto_Next_multiline_text_line

  	PURPOSE		        : Go to Next line.

	INPUT PARAMETERS	: winks_text_String_rect_s *p_multiline_text
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void Winks_UI_Goto_Next_multiline_text_line(winks_text_String_rect_s *p_multiline_text)
{
    int linenumber = 0;

    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    Winks_assert(p_multiline_text != NULL);
    
    linenumber = WINKS_MULTILINE_TEXT_HEIGHT /(WINKS_GETFONTHEIGHT + WINKS_LINE_GAP);
    if(p_multiline_text->v_FirstlineIndx <= (p_multiline_text->v_Stringline_talNum - linenumber ))    
    {	

        p_multiline_text->v_FirstlineIndx = p_multiline_text->v_FirstlineIndx + 1;

    }

    if (Winks_UIContext->text_String_rect.v_Stringline_talNum >= WINKS_MULTILINE_MAX_NUMBER)
    {
        Winks_UIContext->gv_scrollbar.down_button.flags = WINKS_UI_BUTTON_STATE_DOWN;
    }
        
#ifdef __WINKS_MULTI_LAYS__    
    WINKS_BUFFER_ON;
    winks_show_multiline_text(NULL, 0);
    WINKS_BUFFER_OFF;;
#else
    winks_Refresh_Full_Screen();
#endif
}

/**************************************************************

	FUNCTION NAME		: Winks_UI_Goto_PreviousPage_multiline_text_line

  	PURPOSE		        : Go to Previous Page.

	INPUT PARAMETERS	: winks_text_String_rect_s *p_multiline_text
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void Winks_UI_Goto_PreviousPage_multiline_text_line(winks_text_String_rect_s *p_multiline_text)
{
    int linenumber = 0;
    int page_index = 0;
    int page_max = 0;

    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    Winks_assert(p_multiline_text != NULL);

    linenumber = WINKS_MULTILINE_TEXT_HEIGHT /(WINKS_GETFONTHEIGHT + WINKS_LINE_GAP);
    page_index = p_multiline_text->v_FirstlineIndx / linenumber;
    page_max = Winks_UIContext->text_String_rect.v_Stringline_talNum / linenumber;

    if(page_index  >  0)    
    {	

        p_multiline_text->v_FirstlineIndx = (page_index - 1) * linenumber;

    }

    if (Winks_UIContext->text_String_rect.v_Stringline_talNum >= WINKS_MULTILINE_MAX_NUMBER)
    {
        Winks_UIContext->gv_scrollbar.up_button.flags = WINKS_UI_BUTTON_STATE_DOWN;
    }
  
#ifdef __WINKS_MULTI_LAYS__    
    WINKS_BUFFER_ON;
    winks_show_multiline_text(NULL, 0);
    WINKS_BUFFER_OFF;;
#else
    winks_Refresh_Full_Screen();
#endif
}

/**************************************************************

	FUNCTION NAME		: Winks_UI_Goto_NextPage_multiline_text_line

  	PURPOSE		        : Go to Next  Page.

	INPUT PARAMETERS	: winks_text_String_rect_s *p_multiline_text
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void Winks_UI_Goto_NextPage_multiline_text_line(winks_text_String_rect_s *p_multiline_text)
{
    int linenumber = 0;
    int page_index = 0;
    int page_max = 0;

    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    Winks_assert(p_multiline_text != NULL);

    linenumber = WINKS_MULTILINE_TEXT_HEIGHT /(WINKS_GETFONTHEIGHT + WINKS_LINE_GAP);
    page_index = p_multiline_text->v_FirstlineIndx / linenumber;
    page_max = Winks_UIContext->text_String_rect.v_Stringline_talNum / linenumber;

    if ((Winks_UIContext->text_String_rect.v_Stringline_talNum + 1) % linenumber != 0 )
    {
        page_max++;
    }

    if(page_index  < page_max - 1 )    
    {	

        p_multiline_text->v_FirstlineIndx = (page_index + 1)* linenumber;

    }

    if (Winks_UIContext->text_String_rect.v_Stringline_talNum >= WINKS_MULTILINE_MAX_NUMBER)
    {
        Winks_UIContext->gv_scrollbar.down_button.flags = WINKS_UI_BUTTON_STATE_DOWN;
    }
    
#ifdef __WINKS_MULTI_LAYS__    
    WINKS_BUFFER_ON;
    winks_show_multiline_text(NULL, 0);
    WINKS_BUFFER_OFF;;
#else
    winks_Refresh_Full_Screen();
#endif
}
#endif
#if 0

/**************************************************************

	FUNCTION NAME		: winks_ui_draw_apnItem

  	PURPOSE		        : draw  apn item .

	INPUT PARAMETERS	: Winks_ApnItem_s* p_ApnItem
	                      int index_in_screen
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void winks_ui_draw_apnItem(Winks_ApnItem_s* p_ApnItem, int index_in_screen)
{
    int titel_x = 0;
    int titel_y = 0;
    int content_x = 0;
    int content_y = 0;


    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    Winks_assert(p_ApnItem != NULL);

    if (winks_draw_condition(WINKS_MINGUI_HWND_USER) != WINKS_RETURN_SUCCESS)

    {
        return;
    }

    winks_set_font(WINKS_MEDIUM_FONT);
    
    titel_x = WINKS_APN_TITLE_X;
    titel_y = WINKS_APN_TITLE_Y + WINKS_APN_NEXTITEM_HEIGHT * index_in_screen + WINKS_APN_TITLE_TEXT_GAP;

    content_x = WINKS_APN_TITLE_X;
    content_y = WINKS_APN_TITLE_Y + WINKS_APN_NEXTITEM_HEIGHT * index_in_screen + WINKS_APN_TITLE_CONTEN_GAP + WINKS_APN_ITEM_HEIGHT; 
    WINKS_SET_TEXTCOLOR(WINKS_PIXEL_LIGHTWHITE);
    winks_ui_textOutlen(titel_x, titel_y, Winks_UIFM_GetString(p_ApnItem->v_StrId), winks_strlen_ucs2_type((char *)Winks_UIFM_GetString(p_ApnItem->v_StrId)));
    winks_ui_drawRect(content_x - 1,content_y - 1,content_x + WINKS_APN_CONTENTITEM_WIDTH + 1, content_y + WINKS_APN_ITEM_HEIGHT + 1, WINKS_PIXEL_BLACK); 
    winks_ui_fillRect(content_x,content_y,content_x + WINKS_APN_CONTENTITEM_WIDTH, content_y + WINKS_APN_ITEM_HEIGHT, WINKS_PIXEL_LIGHTWHITE );

    if(index_in_screen == Winks_UIContext->winks_apn_screen->index)
    {
#if 0
        WINKS_OPACITY_ON(content_x,content_y,WINKS_APN_CONTENTITEM_WIDTH, WINKS_APN_ITEM_HEIGHT);
        winks_ui_drawGraphicFromFile(content_x,content_y, Winks_UIFM_GetImage(WINKS_HIGHLIGHT_POPUP_ICONID));
        //winks_show_transparent_image(highlight_x, highlight_y, Winks_UIFM_GetImage(WINKS_HIGHLIGHT_POPUP_ICONID));
        WINKS_OPACITY_OFF(content_x,content_y,WINKS_APN_CONTENTITEM_WIDTH, WINKS_APN_ITEM_HEIGHT);
#endif
        winks_ui_fillRect(content_x,content_y,content_x + WINKS_APN_CONTENTITEM_WIDTH, content_y + WINKS_APN_ITEM_HEIGHT, WINKS_HILIGHT_COLOR );
    }

    WINKS_SET_TEXTCOLOR(WINKS_PIXEL_BLACK);
    winks_ui_textOutlen(content_x,content_y + WINKS_APN_TITLE_TEXT_GAP, p_ApnItem->v_string, winks_strlen_ucs2_type(p_ApnItem->v_string));

}

/**************************************************************

	FUNCTION NAME		: winks_ui_draw_apn

  	PURPOSE		        : draw  apn screen .

	INPUT PARAMETERS	: Winks_ApnScreen_s * p_apn_screen
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void winks_ui_draw_apn_screen(Winks_ApnScreen_s * p_apn_screen)
{
    int index;

    Winks_assert(p_apn_screen != NULL);
    
    for (index = 0; index < p_apn_screen->v_TotalItemsNum; index++)
    {
        winks_ui_draw_apnItem(&p_apn_screen->apn_string[index], index);

    }

}


void winks_ui_draw_apn_screen_2(Winks_ApnScreen_s * p_apn_screen)
{
    int index;

    Winks_assert(p_apn_screen != NULL);
    
    for (index = 0; index < 2; index++)
    {
        winks_ui_draw_apnItem(&p_apn_screen->apn_string[index], index);

    }

}

/**************************************************************

	FUNCTION NAME		: winks_ui_redaw_apnitem

  	PURPOSE		        : Redraw  apn item .

	INPUT PARAMETERS	: Winks_ApnItem_s* p_ApnItem
	                      int oldicon_index
	                      int newicon_index
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void winks_ui_redaw_apnitem(Winks_ApnScreen_s * p_apn_screen, int oldicon_index, int newicon_index)
{
   
    int titel_y = 0;
    int content_x = 0;
    int content_y = 0;

    Winks_assert(p_apn_screen != NULL);
    

    if (winks_draw_condition(WINKS_MINGUI_HWND_USER) != WINKS_RETURN_SUCCESS)
       
    {
        return;
    }
    
    winks_set_font(WINKS_MEDIUM_FONT);
    
    titel_y = WINKS_APN_TITLE_Y + WINKS_APN_NEXTITEM_HEIGHT * oldicon_index + WINKS_APN_TITLE_TEXT_GAP;

    content_x = WINKS_APN_TITLE_X;
    content_y = WINKS_APN_TITLE_Y + WINKS_APN_NEXTITEM_HEIGHT * oldicon_index + WINKS_APN_TITLE_CONTEN_GAP + WINKS_APN_ITEM_HEIGHT; 
    
	
    winks_ui_drawRect(content_x - 1,content_y - 1,content_x + WINKS_APN_CONTENTITEM_WIDTH + 1, content_y + WINKS_APN_ITEM_HEIGHT + 1, WINKS_PIXEL_BLACK); 
    winks_ui_fillRect(content_x,content_y,content_x + WINKS_APN_CONTENTITEM_WIDTH, content_y + WINKS_APN_ITEM_HEIGHT, WINKS_PIXEL_LIGHTWHITE );
    WINKS_SET_TEXTCOLOR(WINKS_PIXEL_BLACK);
    winks_ui_textOutlen(content_x, content_y + WINKS_APN_TITLE_TEXT_GAP, p_apn_screen->apn_string[oldicon_index].v_string, winks_strlen_ucs2_type(p_apn_screen->apn_string[oldicon_index].v_string));
    titel_y = WINKS_APN_TITLE_Y + WINKS_APN_NEXTITEM_HEIGHT * newicon_index + WINKS_APN_TITLE_TEXT_GAP;

    content_x = WINKS_APN_TITLE_X;
    content_y = WINKS_APN_TITLE_Y + WINKS_APN_NEXTITEM_HEIGHT * newicon_index + WINKS_APN_TITLE_CONTEN_GAP + WINKS_APN_ITEM_HEIGHT; 
    
   
    winks_ui_drawRect(content_x - 1,content_y - 1,content_x + WINKS_APN_CONTENTITEM_WIDTH + 1, content_y + WINKS_APN_ITEM_HEIGHT + 1, WINKS_PIXEL_BLACK); 
    winks_ui_fillRect(content_x,content_y,content_x + WINKS_APN_CONTENTITEM_WIDTH, content_y + WINKS_APN_ITEM_HEIGHT, WINKS_PIXEL_LIGHTWHITE );
    winks_ui_fillRect(content_x,content_y,content_x + WINKS_APN_CONTENTITEM_WIDTH, content_y + WINKS_APN_ITEM_HEIGHT, WINKS_HILIGHT_COLOR);
    WINKS_SET_TEXTCOLOR(WINKS_PIXEL_BLACK);
    winks_ui_textOutlen(content_x,content_y + WINKS_APN_TITLE_TEXT_GAP, p_apn_screen->apn_string[newicon_index].v_string, winks_strlen_ucs2_type(p_apn_screen->apn_string[newicon_index].v_string));
#if 0
    WINKS_OPACITY_ON(content_x,content_y,WINKS_APN_CONTENTITEM_WIDTH, WINKS_APN_ITEM_HEIGHT);
    winks_ui_drawGraphicFromFile(content_x,content_y, Winks_UIFM_GetImage(WINKS_HIGHLIGHT_POPUP_ICONID));
    //winks_show_transparent_image(highlight_x, highlight_y, Winks_UIFM_GetImage(WINKS_HIGHLIGHT_POPUP_ICONID));
    WINKS_OPACITY_OFF(content_x,content_y,WINKS_APN_CONTENTITEM_WIDTH, WINKS_APN_ITEM_HEIGHT);
#endif
	

}
/**************************************************************

	FUNCTION NAME		: Winks_UI_Goto_Next_ApnItem

  	PURPOSE		        : Go to Next apn's Item.

	INPUT PARAMETERS	: Winks_ApnScreen_s * p_apn_screen
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void Winks_UI_Goto_Next_ApnItem(Winks_ApnScreen_s * p_apn_screen)
{
    int old_index = 0;
	
	WINKS_UI_CONTEXT_DEF()
	WINKS_UI_CONTEXT_INIT()

    Winks_assert(p_apn_screen != NULL);
	
    old_index = p_apn_screen->index;
    p_apn_screen->index = p_apn_screen->index + 1;

    if(p_apn_screen->index >= p_apn_screen->v_TotalItemsNum )//p_apn_screen->v_TotalItemsNum
    {	

        p_apn_screen->index = 0;

    }
#ifdef __WINKS_MULTI_LAYS__    
    WINKS_BUFFER_ON;
    winks_ui_redaw_apnitem(Winks_UIContext->winks_apn_screen, old_index, p_apn_screen->index);
    WINKS_BUFFER_OFF;
#else
    winks_Refresh_Full_Screen();
#endif
   
}

/**************************************************************

	FUNCTION NAME		: Winks_UI_Goto_Previous_ApnItem

  	PURPOSE		        : Go to Previous apn's Item.

	INPUT PARAMETERS	: Winks_ApnScreen_s * p_apn_screen
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void Winks_UI_Goto_Previous_ApnItem(Winks_ApnScreen_s * p_apn_screen)
{
    int old_index = 0;

    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    Winks_assert(p_apn_screen != NULL);

    old_index = p_apn_screen->index;

    if(p_apn_screen->index == 0 )
    {	

        p_apn_screen->index = 2;// p_apn_screen->v_TotalItemsNum;

    } 
    else
    {
        p_apn_screen->index = p_apn_screen->index - 1;
    }

#ifdef __WINKS_MULTI_LAYS__    
    WINKS_BUFFER_ON;
    winks_ui_redaw_apnitem(Winks_UIContext->winks_apn_screen, old_index, p_apn_screen->index);
    WINKS_BUFFER_OFF;
#else
    winks_Refresh_Full_Screen();
#endif
}

void winks_ui_drawuparrow_nomal()
{
    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()
    if (Winks_UIContext->gv_ScreenStyle.v_state & WINKS_NORMAL_STATE)
    {
        if (Winks_UIContext->gv_ListScreen->v_TotalItemsNum > WINKS_LIST_MAX_NUMBER)
        {
            WINKS_BUFFER_ON;
            winks_ui_drawIconButton(&(Winks_UIContext->gv_scrollbar.up_button));
            WINKS_BUFFER_OFF;
        }
    }
    if (Winks_UIContext->gv_ScreenStyle.v_state & WINKS_MULTYEDIT_STATE)
    {
        if (Winks_UIContext->text_String_rect.v_Stringline_talNum >= WINKS_MULTILINE_MAX_NUMBER)
        {
            WINKS_BUFFER_ON;
            winks_ui_drawIconButton(&(Winks_UIContext->gv_scrollbar.up_button));
            WINKS_BUFFER_OFF;
        }
    }
}
void winks_ui_drawdownarrow_nomal()
{
    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()
    if (Winks_UIContext->gv_ScreenStyle.v_state & WINKS_NORMAL_STATE)
    {
        if (Winks_UIContext->gv_ListScreen->v_TotalItemsNum > WINKS_LIST_MAX_NUMBER)
        {
            WINKS_BUFFER_ON;
            winks_ui_drawIconButton(&(Winks_UIContext->gv_scrollbar.down_button));
            WINKS_BUFFER_OFF;
        }
    }
    if (Winks_UIContext->gv_ScreenStyle.v_state & WINKS_MULTYEDIT_STATE)
    {
        if (Winks_UIContext->text_String_rect.v_Stringline_talNum >= WINKS_MULTILINE_MAX_NUMBER)
        {
            WINKS_BUFFER_ON;
            winks_ui_drawIconButton(&(Winks_UIContext->gv_scrollbar.down_button));
            WINKS_BUFFER_OFF;
        }
    }
}
/**************************************************************

	FUNCTION NAME		: winks_strlen_ucs2_type

  	PURPOSE		        : get strlen of ucs2 type .

	INPUT PARAMETERS	: char * text
	OUTPUT PARAMETERS	: null

	RETURNS			    : int

	REMARKS			    : null

***************************************************************/
#endif
int winks_strlen_ucs2_type(char * text)
{
    int nCount = 0;
    //int nLength = 0;

    if (text == NULL)
    {
        return (0);
    }
    else
    {


        /*----------------------------------------------------------------*/
        /* Code Body                                                      */
        /*----------------------------------------------------------------*/
        /* Check for NULL character only at the odd no. of bytes 
           assuming forst byte start from zero */
        if (text)
        {
            while (text[nCount] != 0 || text[nCount + 1] != 0)
            {
                //++nLength;
                nCount += 2;
            }
        }
        return nCount;
    }   
}    
#if 0
void winks_ui_redraw_change_area( )
{
    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()
#ifdef __WINKS_MULTI_LAYS__    
    WINKS_BUFFER_ON;
    if (Winks_UIContext->gv_ScreenStyle.v_state & WINKS_POPUPMENU_STATE)
    {

        winks_ui_drawPOPMenuScreen(Winks_UIContext->gv_PopmenuScreen);
        
    }
    else if (Winks_UIContext->gv_ScreenStyle.v_state & WINKS_NORMAL_STATE)
    {
        if (Winks_UIContext->winks_scroll_string.scroll_pause == 1)
        {
            winks_ui_scrolling_text_stop(&Winks_UIContext->winks_scroll_string);
        }

        Winks_UIContext->scrooltext_flag = 0;
        winks_ui_drawListScreen(Winks_UIContext->gv_ListScreen);
       
    }
    else if (Winks_UIContext->gv_ScreenStyle.v_state & WINKS_MAINMENU_STATE)
    {
    
      //   winks_ui_redrawmenu(Winks_UIContext->gv_MainMenu, Winks_UIContext->gv_old_index, Winks_UIContext->gv_MainMenu->v_FocusMatrixItemIndxInScrn);
        
    }
    else if (Winks_UIContext->gv_ScreenStyle.v_state & WINKS_APNLIST_STATE)
    {
   
        winks_ui_redaw_apnitem(Winks_UIContext->winks_apn_screen, Winks_UIContext->gv_old_index, Winks_UIContext->winks_apn_screen->index);
       
    }
    else if (Winks_UIContext->gv_ScreenStyle.v_state & WINKS_MULTYEDIT_STATE)
    {
   
         winks_show_multiline_text(NULL, 0);
       
    }

    WINKS_BUFFER_OFF;
#else
    winks_Refresh_Full_Screen();
#endif

}
  
#endif 
#endif



extern int winks_get_coordinate(Winks_screen_coordinate_e coordinate_id)
{
     if (coordinate_id <WINKS_TITLE_STRING_Y_ID )
     	{
	 	switch(WINKS_SCREEN_WIDTH)
 		{
	 		case 176: 
					{
							const  int coordinate[WINKS_TITLE_STRING_Y_ID] = {
																			/*WINKS_TITLE_STRING_X*/55,
																				/*WINKS_MAINMENU_ICON_WIDTH*/40,
																				/*WINKS_MAINMENU_BACKGROUND_WIDTH*/54,
																				/*WINKS_LIST_ICON_WIDTH*/16,
																				/*WINKS_UPSCROLLBAR_WIDTH*/9,
																				/*WINKS_INFOBOX_BG_WIDTH*/154,
																				/*WINKS_INFOBOX_STRING_WIDTH */140};
							return coordinate[coordinate_id];
					};break;
			case 240:
					{
							const  int coordinate[WINKS_TITLE_STRING_Y_ID] = {
																			/*WINKS_TITLE_STRING_X*/85,
																				/*WINKS_MAINMENU_ICON_WIDTH*/61,
																				/*WINKS_MAINMENU_BACKGROUND_WIDTH*/78,
																				/*WINKS_LIST_ICON_WIDTH*/25,
																				/*WINKS_UPSCROLLBAR_WIDTH*/11,
																				/*WINKS_INFOBOX_BG_WIDTH*/215,
																				/*WINKS_INFOBOX_STRING_WIDTH */200};
							return coordinate[coordinate_id];
					}break;
			default:
					{
							const  int coordinate[WINKS_TITLE_STRING_Y_ID] = {
							/*WINKS_TITLE_STRING_X*/55,
							/*WINKS_MAINMENU_ICON_WIDTH*/40,
							/*WINKS_MAINMENU_BACKGROUND_WIDTH*/54,
							/*WINKS_LIST_ICON_WIDTH*/16,
							/*WINKS_UPSCROLLBAR_WIDTH*/9,
							/*WINKS_INFOBOX_BG_WIDTH*/154,
							/*WINKS_INFOBOX_STRING_WIDTH */140};
					            return coordinate[coordinate_id];
					
			  		};break;
			};

		
	}
     	

	else{
	 	switch(WINKS_SCREEN_HEIGHT)
	 		{
				case 205: 
						{
							const  int coordinate[WINKS_INFOBOX_STRING_HEIGHT_ID -WINKS_TITLE_STRING_Y_ID + 1 ] ={   /*WINKS_TITLE_STRING_Y */35,
																												/*WINKS_TITLE_HEIGHT */64,
																												/*WINKS_CLIENT_HEIGHT_ID*/122,
																												/*WINKS_MAINMENU_ICON_HEIGHT */40,
																												/*WINKS_MAINMENU_BACKGROUND_HEIGHT */54,
																												/*WINKS_SOFTKEY_HEIGHT */16,
																												/*WINKS_LIST_MAX_NUMBER_ID*/5,
																												/*WINKS_HIGHLIGHT_LISTITEM_HEIGHT*/24,
																												/*WINKS_LIST_ICON_HEIGHT */16,
																												/*WINKS_UPSCROLLBAR_HEIGHT */10,
																												/*WINKS_INFOBOX_BG_HEIGHT */84,
																												/*WINKS_INFOBOX_STRING_AND_BG_GAP*/12,
																												/*WINKS_INFOBOX_STRING_HEIGHT */70};
							return coordinate[coordinate_id - WINKS_TITLE_STRING_Y_ID];
						};break;
				case 208: 
				{
					const  int coordinate[WINKS_INFOBOX_STRING_HEIGHT_ID -WINKS_TITLE_STRING_Y_ID + 1 ] ={   /*WINKS_TITLE_STRING_Y */35,
																										/*WINKS_TITLE_HEIGHT */64,
																										/*WINKS_CLIENT_HEIGHT_ID*/125,
																										/*WINKS_MAINMENU_ICON_HEIGHT */40,
																										/*WINKS_MAINMENU_BACKGROUND_HEIGHT */54,
																										/*WINKS_SOFTKEY_HEIGHT */16,
																										/*WINKS_LIST_MAX_NUMBER_ID*/5,
																										/*WINKS_HIGHLIGHT_LISTITEM_HEIGHT*/24,
																										/*WINKS_LIST_ICON_HEIGHT */16,
																										/*WINKS_UPSCROLLBAR_HEIGHT */10,
																										/*WINKS_INFOBOX_BG_HEIGHT */84,
																										/*WINKS_INFOBOX_STRING_AND_BG_GAP*/12,
																										/*WINKS_INFOBOX_STRING_HEIGHT */70};
							return coordinate[coordinate_id - WINKS_TITLE_STRING_Y_ID];
						};break;
		 		case 220: 
						{
							const  int coordinate[WINKS_INFOBOX_STRING_HEIGHT_ID -WINKS_TITLE_STRING_Y_ID + 1 ] ={   /*WINKS_TITLE_STRING_Y */35,
																													/*WINKS_TITLE_HEIGHT */64,
																													/*WINKS_CLIENT_HEIGHT_ID*/136,
																													/*WINKS_MAINMENU_ICON_HEIGHT */40,
																													/*WINKS_MAINMENU_BACKGROUND_HEIGHT */54,
																													/*WINKS_SOFTKEY_HEIGHT */20,
																													/*WINKS_LIST_MAX_NUMBER_ID*/5,
																													/*WINKS_HIGHLIGHT_LISTITEM_HEIGHT*/24,
																													/*WINKS_LIST_ICON_HEIGHT */16,
																													/*WINKS_UPSCROLLBAR_HEIGHT */10,
																													/*WINKS_INFOBOX_BG_HEIGHT */84,
																													/*WINKS_INFOBOX_STRING_AND_BG_GAP*/12,
																													/*WINKS_INFOBOX_STRING_HEIGHT */70};
					            return coordinate[coordinate_id - WINKS_TITLE_STRING_Y_ID];
			 			};break;
				case 296:
						{
							const  int coordinate[WINKS_INFOBOX_STRING_HEIGHT_ID -WINKS_TITLE_STRING_Y_ID + 1 ] =  {   /*WINKS_TITLE_STRING_Y */45,
																													/*WINKS_TITLE_HEIGHT */83,
																													/*WINKS_CLIENT_HEIGHT_ID*/189,
																											
																													/*WINKS_MAINMENU_ICON_HEIGHT */61,
																													/*WINKS_MAINMENU_BACKGROUND_HEIGHT */78,
																													/*WINKS_SOFTKEY_HEIGHT */26,
																													/*WINKS_LIST_MAX_NUMBER_ID*/6,
																													/*WINKS_HIGHLIGHT_LISTITEM_HEIGHT*/30,
																													/*WINKS_LIST_ICON_HEIGHT */25,
																													/*WINKS_UPSCROLLBAR_HEIGHT */12,
																													/*WINKS_INFOBOX_BG_HEIGHT */115,
																													/*WINKS_INFOBOX_STRING_AND_BG_GAP*/14,
																													/*WINKS_INFOBOX_STRING_HEIGHT */96};
					            return coordinate[coordinate_id - WINKS_TITLE_STRING_Y_ID];
			 			};break;
				case 320:
						{
							const  int coordinate[WINKS_INFOBOX_STRING_HEIGHT_ID -WINKS_TITLE_STRING_Y_ID + 1 ] =  {   /*WINKS_TITLE_STRING_Y */45,
																													/*WINKS_TITLE_HEIGHT */83,
																													/*WINKS_CLIENT_HEIGHT_ID*/210,
																											
																													/*WINKS_MAINMENU_ICON_HEIGHT */61,
																													/*WINKS_MAINMENU_BACKGROUND_HEIGHT */78,
																													/*WINKS_SOFTKEY_HEIGHT */26,
																													/*WINKS_LIST_MAX_NUMBER_ID*/6,
																													/*WINKS_HIGHLIGHT_LISTITEM_HEIGHT*/30,
																													/*WINKS_LIST_ICON_HEIGHT */25,
																													/*WINKS_UPSCROLLBAR_HEIGHT */12,
																													/*WINKS_INFOBOX_BG_HEIGHT */115,
																													/*WINKS_INFOBOX_STRING_AND_BG_GAP*/14,
																													/*WINKS_INFOBOX_STRING_HEIGHT */96};
					            return coordinate[coordinate_id - WINKS_TITLE_STRING_Y_ID];
			 			};break;
				default:
						{
							const  int coordinate[WINKS_INFOBOX_STRING_HEIGHT_ID -WINKS_TITLE_STRING_Y_ID + 1 ] ={   /*WINKS_TITLE_STRING_Y */35,
								/*WINKS_TITLE_HEIGHT */64,
								/*WINKS_CLIENT_HEIGHT_ID*/136,
								/*WINKS_MAINMENU_ICON_HEIGHT */40,
								/*WINKS_MAINMENU_BACKGROUND_HEIGHT */54,
								/*WINKS_SOFTKEY_HEIGHT */20,
								/*WINKS_LIST_MAX_NUMBER_ID*/5,
								/*WINKS_HIGHLIGHT_LISTITEM_HEIGHT*/24,
								/*WINKS_LIST_ICON_HEIGHT */16,
								/*WINKS_UPSCROLLBAR_HEIGHT */10,
								/*WINKS_INFOBOX_BG_HEIGHT */84,
								/*WINKS_INFOBOX_STRING_HEIGHT */70};
						            return coordinate[coordinate_id - WINKS_TITLE_STRING_Y_ID];
										 
			  			};break;
	 		}

		}

}
extern int winks_get_colorcall_coordinate(Winks_colorcall_coordinate_e coordinate_id)
{
     if (coordinate_id <WINKS_COLORCALL_NAME_Y_ID )
     	{
	 	switch(WINKS_SCREEN_WIDTH)
	 	{
	 		case 176: 
						{
							const  int coordinate[WINKS_COLORCALL_NAME_Y_ID] = {
																				/*WINKS_COLORCALL_NAME_X*/4,
			 																	/*WINKS_COLORCALL_NAME_WIDTH*/80,
			 																	/*WINKS_COLORCALL_TIME_X*/130,
			 																	/* WINKS_COLORCALL_TIME_GAP*/3,
			 																	/*WINKS_COLORCALL_PIC_X*/0,
			 																	/*WINKS_COLORCALL_PIC_WIGHT*/176,
			 																	/*WINKS_COLORCALL_GREETSTRING_X */13,
			 																	/*WINKS_COLORCALL_GREETSTRING_WIGHT */150};
					            return coordinate[coordinate_id];
			 			};break;
			case 240:
						{
							const  int coordinate[WINKS_COLORCALL_NAME_Y_ID] =  {
																				/*WINKS_COLORCALL_NAME_X*/4,
			 																	/*WINKS_COLORCALL_NAME_WIDTH*/160,
			 																	/*WINKS_COLORCALL_TIME_X*/190,
			 																	/* WINKS_COLORCALL_TIME_GAP*/3,
			 																	/*WINKS_COLORCALL_PIC_X*/0,
			 																	/*WINKS_COLORCALL_PIC_WIGHT*/240,
			 																	/*WINKS_COLORCALL_GREETSTRING_X */20,
			 																	/*WINKS_COLORCALL_GREETSTRING_WIGHT */200};
					            return coordinate[coordinate_id];
						}break;
			default:
					{
							const  int coordinate[WINKS_COLORCALL_NAME_Y_ID] = {
								/*WINKS_COLORCALL_NAME_X*/4,
								/*WINKS_COLORCALL_NAME_WIDTH*/80,
								/*WINKS_COLORCALL_TIME_X*/130,
								/* WINKS_COLORCALL_TIME_GAP*/3,
								/*WINKS_COLORCALL_PIC_X*/0,
								/*WINKS_COLORCALL_PIC_WIGHT*/176,
								/*WINKS_COLORCALL_GREETSTRING_X */13,
								/*WINKS_COLORCALL_GREETSTRING_WIGHT */150};
						            return coordinate[coordinate_id];
			  		};break;
		};

	}
     	

	else{
	 	switch(WINKS_SCREEN_HEIGHT)
	 		{
	 		case 220: 
						{
							const  int coordinate[WINKS_COLORCALL_HEIGHT_ID -WINKS_COLORCALL_NAME_Y_ID + 1 ] = {   /*WINKS_COLORCALL_NAME_Y */1,
																													/*WINKS_COLORCALL_NAME_HEIGHT */15,
																													/*WINKS_COLORCALL_TIME_Y*/1,
																													/*WINKS_COLORCALL_PIC_Y */29,
																													/*WINKS_COLORCALL_PIC_HEIGHT */140,
																													/*WINKS_COLORCALL_GREETSTRING_Y */170,
																													/*WINKS_COLORCALL_GREETSTRING_HEIGHT */26,
																													/*WINKS_COLORCALLSOFTKEY_BUTTON_Y */198,
																													/*WINKS_COLORCALL_HEIGHT  */26
																													};
					            return coordinate[coordinate_id - WINKS_COLORCALL_NAME_Y_ID];
			 			};break;
			case 320:
						{
							const  int coordinate[WINKS_COLORCALL_HEIGHT_ID -WINKS_COLORCALL_NAME_Y_ID + 1 ] = {   /*WINKS_COLORCALL_NAME_Y */4,
																													/*WINKS_COLORCALL_NAME_HEIGHT */25,
																													/*WINKS_COLORCALL_TIME_Y*/4,
																													/*WINKS_COLORCALL_PIC_Y */29,
																													/*WINKS_COLORCALL_PIC_HEIGHT */236,
																													/*WINKS_COLORCALL_GREETSTRING_Y */270,
																													/*WINKS_COLORCALL_GREETSTRING_HEIGHT */26,
																													/*WINKS_COLORCALLSOFTKEY_BUTTON_Y */300,
																													/*WINKS_COLORCALL_HEIGHT  */20
																													};
					            return coordinate[coordinate_id - WINKS_COLORCALL_NAME_Y_ID];
			 			};break;
			default:
						{
							const  int coordinate[WINKS_COLORCALL_HEIGHT_ID -WINKS_COLORCALL_NAME_Y_ID + 1 ] = {   /*WINKS_COLORCALL_NAME_Y */1,
								/*WINKS_COLORCALL_NAME_HEIGHT */15,
								/*WINKS_COLORCALL_TIME_Y*/1,
								/*WINKS_COLORCALL_PIC_Y */29,
								/*WINKS_COLORCALL_PIC_HEIGHT */140,
								/*WINKS_COLORCALL_GREETSTRING_Y */170,
								/*WINKS_COLORCALL_GREETSTRING_HEIGHT */26,
								/*WINKS_COLORCALLSOFTKEY_BUTTON_Y */198,
								/*WINKS_COLORCALL_HEIGHT  */26
							};
					            return coordinate[coordinate_id - WINKS_COLORCALL_NAME_Y_ID];
				  		};break;
	 		}
		}

}

#if 1
/* color call */
int winks_ui_drawfile_w_h (const char *file_name ,int x, int y, int view_width, int view_height)
{

	return 0 ;
}



#if 0
int if_file_name_is_anim(char * file_name)
{
	
	char *v_ptr ;
    int   Len;
    char  ExtName[16];
	if (*(short*)file_name == 0) 
		return 0;
	v_ptr = file_name + winks_strlen_ucs2_type((char*)file_name) - 1;
	
	while(*v_ptr != '.')
	{
		v_ptr--;
	}

    //Len = Winks_Unicode2AsciiString(v_ptr, ExtName);
    Len = Winks_Unicode2AsciiString(v_ptr, ExtName, 16);
    if(Len >= sizeof(ExtName))
        return E_IMAGE_JPG;

    v_ptr = ExtName;

       if (stricmp(v_ptr, ".gif") ==0)
	{
		return   E_IMAGE_GIF;
	}
	 else
	 {
	 	return   0;
	 }


}
#endif
void  winks_ui_drawcolorcall_part(int x , int y, Winks_Colorcall_part_s *p_colorcall_part)
{
	//static winks_scrolling_text_s  s;
	//static int flag = 0;
	int w = 0, h = 0;
	int excursion = 0;
	int wight_excursion = 0;
	int result = 0;
	int width_area = 0; 
	int height_area = 0;
	
    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    Winks_assert(p_colorcall_part != NULL);
	x = x + (int)(WINKS_SCREEN_WIDTH_UI * ((p_colorcall_part->blank) / (float)100));
    width_area =  (int)(WINKS_SCREEN_WIDTH_UI * (p_colorcall_part->width / (float)100));
    height_area =  (int)(WINKS_SCREEN_HEIGHT_UI * (p_colorcall_part->height/ (float)100));
    if (winks_draw_condition(WINKS_MINGUI_HWND_CALL) != WINKS_RETURN_SUCCESS)
    {
        return;
    }
    WINKS_CLIP_ON(x,y,  x + width_area, y + height_area);
    winks_ui_drawfile_w_h(p_colorcall_part->v_filename,x,y,  x + width_area, y + height_area);      
    Winks_set_font_feature(&(p_colorcall_part->font_feature)); 
	WINKS_SET_TEXTCOLOR((p_colorcall_part->text_color)); 
	if((p_colorcall_part->content != NULL)&&(*(short *)p_colorcall_part->content != 0))
		winks_get_string_wight_high(p_colorcall_part->content,winks_strlen_ucs2_type(p_colorcall_part->content),&w,&h);

    if ( p_colorcall_part->content_type == WINKS_COLORIMAGE_TYPE)
    {
    	if(p_colorcall_part->v_ImageCode == WINKS_FILE_EXT_gif)
	    {
    		if (winks_anim_frame_count(p_colorcall_part->v_filename) == 1)
    		{
    		    //anim_flag = -1;
    		    winks_set_anim_display_count( -1);
    		    //winks_image_draw_animation(WINKS_COLORCALL_PIC_X,WINKS_COLORCALL_PIC_Y,p_colorcall->v_filename,0,WINKS_COLORCALL_PIC_WIGHT,WINKS_COLORCALL_PIC_HEIGHT);
    		    result = winks_ui_drawGraphic_uc_file_and_cache(x, y ,p_colorcall_part->v_filename,width_area,height_area);
    		    if(result == WINKS_IMAGE_TOO_LARGE)
    		    {
    		       winks_ui_drawImage_error(x, y , width_area,height_area, (char * )Winks_UIFM_GetString(WINKS_UI_DRAW_IMAGE_ERROR_STRINGID));
    		    }

    		}
    		else
    		{
    		    //anim_flag = 0;
    		    winks_set_anim_display_count( 0);
    		    //winks_image_draw_animation(30, 110,WINKS_ROOT_PATH"winks\\image\\a.gif", 0);
    		    winks_image_draw_animation(x, y,p_colorcall_part->v_filename, 0, width_area,height_area);
    		    winks_anim_set_last_frame_callback();
    		}
    	}
        else
        {
            //anim_flag = -1;
            winks_set_anim_display_count( -1);
            //winks_image_draw_animation(WINKS_COLORCALL_PIC_X,WINKS_COLORCALL_PIC_Y,p_colorcall->v_filename, 0, WINKS_COLORCALL_PIC_WIGHT,WINKS_COLORCALL_PIC_HEIGHT);
            result = winks_ui_drawGraphic_uc_file_and_cache(x, y ,p_colorcall_part->v_filename,width_area,height_area);
            if(result == WINKS_IMAGE_TOO_LARGE)
            {
                winks_ui_drawImage_error(x, y , width_area,height_area, (char * )Winks_UIFM_GetString(WINKS_UI_DRAW_IMAGE_ERROR_STRINGID));
            }

        }
	}
    //if ( p_colorcall_part->content_type == "greet")
    else
	{
#if 0
        if(w <= width_area)
        {
            excursion = (height_area - WINKS_GETFONTHEIGHT) >> 1;
            wight_excursion = (width_area - w) >> 1;
            winks_ui_textOutlen(x + wight_excursion, y + excursion, p_colorcall_part->content,winks_strlen_ucs2_type(p_colorcall_part->content) );
        }
        else
#endif
        if ((w <= width_area) || (p_colorcall_part->scrollway == WINKS_STRINGSCROLL_NO))
	 	{
	 		Winks_showtext_in_area (p_colorcall_part->content,winks_strlen_ucs2_type(p_colorcall_part->content) ,x, y, width_area,height_area);
	 	}
		else
	 	{
	        if(Winks_UIContext->scrooltext_flag == 0) 
	        {
		        winks_scrolling_text_s *  p_winks_scrolling_text;
				 winks_scrolling_text_s *   p_temp;
	            p_winks_scrolling_text = (winks_scrolling_text_s *)Winks_Alloc(sizeof(winks_scrolling_text_s));
	
            	if(p_winks_scrolling_text == NULL)
            	{
                 
            		Winks_printf("Error: Allocate memory failed!\n");
            		return ;
            	}
                Winks_mem_set(p_winks_scrolling_text, 0 , sizeof(winks_scrolling_text_s));
                	
              
            	if (Winks_UIContext->winks_scroll_string_head == NULL) 
            	{
            		Winks_UIContext->winks_scroll_string_head = p_winks_scrolling_text;
            	}
            	else
            	{
            		p_temp = Winks_UIContext->winks_scroll_string_head;
            		while (p_temp->p_next != NULL)
            		{
            			p_temp = p_temp->p_next;
            		}
            		p_temp->p_next = p_winks_scrolling_text;
            	}
	            winks_ui_create_scrolling_text(
			            p_winks_scrolling_text,
			            x,
			            y,
			            width_area,
			            height_area,
			            p_colorcall_part->content,
			            winks_ui_handle_scrolling_text_colorcall,
			            //void (*draw_background) (int x1, int y1, int x2, int y2),
			            p_colorcall_part->text_color,
			            p_colorcall_part->scrollway,
			            WINKS_PIXEL_TRANSPARENT,
			            
			            NULL,
						p_colorcall_part->font_feature
			            //winks_color border_color
			            );


	            //winks_ui_redraw_scrolling_text(&s);
	            Winks_UIContext->scrooltext_flag = 1;
	            winks_ui_scrolling_text_start(Winks_UIContext->winks_scroll_string_head );
	        }
            winks_ui_redraw_scrolling_text(Winks_UIContext->winks_scroll_string_head );
        }
    }
    WINKS_CLIP_OFF(x,y,  x + width_area, y + height_area);
#if 0
    //winks_ui_textOutlen(WINKS_COLORCALL_TIME_X,WINKS_COLORCALL_TIME_Y , p_colorcall->v_calltime,winks_strlen_ucs2_type(p_colorcall->v_calltime) );
    //Winks_mem_cpy(p_colorcall->v_filename,WINKS_ROOT_PATH"winks\\image\\4.gif",strlen(WINKS_ROOT_PATH"winks\\image\\4.gif"));
	//*(p_colorcall->v_filename + strlen(WINKS_ROOT_PATH"winks\\image\\4.gif")) = 0;

    Winks_assert(p_colorcall_part->v_filename != NULL);

    //WINKS_CLIP_ON(WINKS_COLORCALL_PIC_X,WINKS_COLORCALL_PIC_Y, WINKS_COLORCALL_PIC_WIGHT, WINKS_COLORCALL_PIC_Y + WINKS_COLORCALL_PIC_HEIGHT);
            
	if (WK_STRLEN(p_colorcall_part->v_filename) != 0) 
	{
        if (p_colorcall_part->v_ImageCode == E_IMAGE_GIF)
        {
            if (winks_anim_frame_count(p_colorcall_part->v_filename) == 1)
            {
                //anim_flag = -1;
                winks_set_anim_display_count( -1);
                //winks_image_draw_animation(WINKS_COLORCALL_PIC_X,WINKS_COLORCALL_PIC_Y,p_colorcall->v_filename,0,WINKS_COLORCALL_PIC_WIGHT,WINKS_COLORCALL_PIC_HEIGHT);
                result = winks_ui_drawGraphic_uc_file_and_cache(WINKS_COLORCALL_PIC_X,WINKS_COLORCALL_PIC_Y,p_colorcall_part->v_filename,WINKS_COLORCALL_PIC_WIGHT,WINKS_COLORCALL_PIC_HEIGHT);
                if(result == WINKS_IMAGE_TOO_LARGE)
                {
                   winks_ui_drawImage_error(WINKS_COLORCALL_PIC_X,WINKS_COLORCALL_PIC_Y, WINKS_COLORCALL_PIC_WIGHT, WINKS_COLORCALL_PIC_HEIGHT, (char * )Winks_UIFM_GetString(WINKS_UI_DRAW_IMAGE_ERROR_STRINGID));
                }

            }
            else
            {
                //anim_flag = 0;
                winks_set_anim_display_count( 0);
                //winks_image_draw_animation(30, 110,WINKS_ROOT_PATH"winks\\image\\a.gif", 0);
                winks_image_draw_animation(WINKS_COLORCALL_PIC_X,WINKS_COLORCALL_PIC_Y,p_colorcall_part->v_filename, 0, WINKS_COLORCALL_PIC_WIGHT, WINKS_COLORCALL_PIC_HEIGHT);
                winks_anim_set_last_frame_callback();
            }
        }
        else
        {
            //anim_flag = -1;
            winks_set_anim_display_count( -1);
            //winks_image_draw_animation(WINKS_COLORCALL_PIC_X,WINKS_COLORCALL_PIC_Y,p_colorcall->v_filename, 0, WINKS_COLORCALL_PIC_WIGHT,WINKS_COLORCALL_PIC_HEIGHT);
            result = winks_ui_drawGraphic_uc_file_and_cache(WINKS_COLORCALL_PIC_X,WINKS_COLORCALL_PIC_Y,p_colorcall_part->v_filename,WINKS_COLORCALL_PIC_WIGHT,WINKS_COLORCALL_PIC_HEIGHT);
            if(result == WINKS_IMAGE_TOO_LARGE)
            {
               winks_ui_drawImage_error(WINKS_COLORCALL_PIC_X,WINKS_COLORCALL_PIC_Y, WINKS_COLORCALL_PIC_WIGHT, WINKS_COLORCALL_PIC_HEIGHT, (char * )Winks_UIFM_GetString(WINKS_UI_DRAW_IMAGE_ERROR_STRINGID));
            }

        }
	}
    WINKS_CLIP_OFF(WINKS_COLORCALL_PIC_X,WINKS_COLORCALL_PIC_Y, WINKS_COLORCALL_PIC_WIGHT, WINKS_COLORCALL_PIC_Y + WINKS_COLORCALL_PIC_HEIGHT);

    if(w <= WINKS_COLORCALL_GREETSTRING_WIGHT)
    {
        excursion = (WINKS_COLORCALL_GREETSTRING_HEIGHT - WINKS_GETFONTHEIGHT) >> 1;
        wight_excursion = (WINKS_COLORCALL_GREETSTRING_WIGHT - w) >> 1;
        winks_ui_textOutlen(WINKS_COLORCALL_GREETSTRING_X + wight_excursion, WINKS_COLORCALL_GREETSTRING_Y + excursion, p_colorcall_part->v_greet,winks_strlen_ucs2_type(p_colorcall_part->v_greet) );
    }
    else
    {
        if(Winks_UIContext->scrooltext_flag == 0) 
        {
            winks_ui_create_scrolling_text(
            &Winks_UIContext->winks_scroll_string_colorcall,
            WINKS_COLORCALL_GREETSTRING_X,
            WINKS_COLORCALL_GREETSTRING_Y,
            WINKS_COLORCALL_GREETSTRING_WIGHT,
            WINKS_COLORCALL_GREETSTRING_HEIGHT,
            p_colorcall_part->v_greet,
            winks_ui_handle_scrolling_text_colorcall,
            //void (*draw_background) (int x1, int y1, int x2, int y2),
            WINKS_PIXEL_BLACK,
            WINKS_PIXEL_TRANSPARENT,
            
            NULL
            //winks_color border_color
            );


            //winks_ui_redraw_scrolling_text(&s);
            Winks_UIContext->scrooltext_flag = 1;
            winks_ui_scrolling_text_start(&Winks_UIContext->winks_scroll_string_colorcall);
        }
            winks_ui_redraw_scrolling_text(&Winks_UIContext->winks_scroll_string_colorcall);
    }
#endif
    
}
int  winks_ui_drawcolorcall(Winks_Colorcall_S *p_colorcall)
{
      Winks_Colorcall_S *  p_temp = NULL; 
	int x = 0,y = 0;
	int i = 0;
	WINKS_UI_CONTEXT_DEF()
		
	WINKS_UI_CONTEXT_INIT()
	p_temp = p_colorcall;
	//p_temp = (Winks_UIContext->Winks_Colorcall_head).head;
	  WINKS_BUFFER_ON;
	for ( i = 0 ; i < (Winks_UIContext->Winks_Colorcall_head).total_number; i++)  	
	//for (p_temp = p_colorcall; p_temp != NULL;p_temp->P_next) 
	{
		
		winks_ui_drawcolorcall_part(x,y,&(p_temp[i].Colorcall_part));
		if ((i == (Winks_UIContext->Winks_Colorcall_head).total_number -1)|| ((i < (Winks_UIContext->Winks_Colorcall_head).total_number -1) && (p_temp[i + 1].Colorcall_part.position_in_screen[0] > p_temp[i ].Colorcall_part.position_in_screen[0] )))
		{
	
			x = 0;
			y = y + (int)(WINKS_SCREEN_HEIGHT_UI * ((p_temp[i].Colorcall_part.height) / (float)100));
		
		}
		else
		{
			x = x + (int)(WINKS_SCREEN_WIDTH_UI * ((p_temp[i].Colorcall_part.width + p_temp[i].Colorcall_part.blank) / (float)100)) ;
		}
	}
	
	
	  
	  winks_set_font(WINKS_MEDIUM_FONT);
	  winks_ui_Create_Colorcall_RightSoftKey(WINKS_UI_COLORDISPLAY_EXIT_STRINGID);
	  WINKS_BUFFER_OFF;
	return 0;
}
/**************************************************************

	FUNCTION NAME		: winks_creat_text_string_rect_type

  	PURPOSE		        : creat a multiline's text.

	INPUT PARAMETERS	: winks_text_String_rect_s * rect_text_String
	                      winks_Rect_s* pRect
	                      char TextAlign
	                      char * string_text
	                      int string_text_lenth
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/
int Winks_showtext_in_area (char* string_text,int string_text_lenth,int x, int y, int width,int height )
//int winks_creat_text_string_rect_type(winks_text_String_rect_s * rect_text_String, winks_Rect_s* pRect, WINKS_SHOW_FLAG TextAlign, char * string_text, int string_text_lenth)
{
    char * temp;
    //		  int y = 0;
    //		  int NumLines;
    //const char  *sOrg =s;
    int FontYSize,FontCharSize;
    int  char_tap = 0;
    // int xLine = 0;
    int  string_x = 0;
    int  string_y = 0;
    int linewidth = 0;
    int lineheight = 0;
	int i = 0;
    // int LineLen = 0;
    // int NumCharsRem;           /* Number of remaining characters */
    int num_in_line = 0;
    // 	  int remain_number = 0;
    int linenumber = 0;
    char * text = (char  *)string_text;
    int current_textlen = 0;
    int current_textlinelen = 0;
    int flag;
	int w,h;
	int w1;

    //char flag;
    //rect_text_String->v_ShowFlag = TextAlign;
    //textlen = MaxNumChars;
	if(*(short *)string_text == 0)
		return 0;
	winks_get_string_wight_high(string_text, winks_strlen_ucs2_type(string_text),&w,&h);
    
    FontYSize = WINKS_GETFONTHEIGHT;
    FontCharSize = WINKS_GETFONTHEIGHT >> 1;
    lineheight= h;//WINKS_GETFONTHEIGHT;
    linenumber = height/ lineheight;
    linewidth = width;
   
    num_in_line = (linewidth+char_tap) / (FontCharSize + char_tap);

    //r = *pRect;
    string_x = x;
    string_y = 	y;
    temp = text;
    for(i = 0; current_textlen < string_text_lenth; i++ ) 
    {
      
        flag = creat_text_String_line_type(temp,linewidth,&current_textlinelen);
        if (flag & WINKS_SEPARATOR_FLAG)
        {
            current_textlinelen = current_textlinelen + winks_separator_len();
            
        }
        if (flag & WINKS_BLANK_FLAG)
        {
            current_textlinelen = current_textlinelen + winks_blank_len();
        
        }
	
	temp = temp + current_textlinelen;
	current_textlen = current_textlen + current_textlinelen;
	
    	}
	if (i == 1)
	{
		string_x += ((linewidth - w) >> 1);
	}
     if (i < linenumber) 
    	{
    		string_y += (height - i*lineheight) >> 1;
	}

	current_textlen = 0;
	temp = string_text;
    for(i = 0; current_textlen < string_text_lenth; i++ ) 
    {
      
        flag = creat_text_String_line_type(temp,linewidth,&current_textlinelen);
        if (flag & WINKS_SEPARATOR_FLAG)
        {
            current_textlinelen = current_textlinelen + winks_separator_len();
            
        }
        if (flag & WINKS_BLANK_FLAG)
        {
            current_textlinelen = current_textlinelen + winks_blank_len();
        
        }

	if (i < linenumber -1)
	{
		winks_ui_textOutlen(string_x, string_y, temp, current_textlinelen);
	}
	else
	{
		if((current_textlen + current_textlinelen) < string_text_lenth)
		{
		
			char sp_ucs2[4] = {0x26, 0x20, 0, 0};
			int word_width = winks_get_text_string_oneword_width(sp_ucs2,winks_get_word_byte_number());
			int  text_lenth = current_textlinelen;
			
			winks_get_string_wight_high(text, text_lenth,&w,&h);
			winks_get_string_wight_high(sp_ucs2, winks_strlen_ucs2_type(sp_ucs2),&w1,&h);
			while (w >(linewidth - w1))
			{
				text_lenth -= winks_get_word_byte_number();

				winks_get_string_wight_high(text, text_lenth,&w,&h);
				
			};
			
			winks_ui_textOutlen(string_x, string_y, temp, text_lenth);
			winks_ui_textOutlen(string_x + w, string_y, sp_ucs2, winks_strlen_ucs2_type(sp_ucs2));
		}
		else
		{
			winks_ui_textOutlen(string_x, string_y, temp, current_textlinelen);
		}
		 return 0; 
	}
	temp = temp + current_textlinelen;
	current_textlen = current_textlen + current_textlinelen;
	string_y = string_y + lineheight;
        
    }

    
    return 0; 
}


#endif


#endif /*MTK_CC_VERSION*/

