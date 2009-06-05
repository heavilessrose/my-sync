

#include "wk_public.h"
#include "wk_osal.h"
#include "wk_engin.h"
#include "wk_uifm.h"
#include "wk_uifml.h"
//#include "wk_uifml.h"
#include "wk_wdt.h"
//#include "wk_ui_touch.h"
#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
#define WINKS_UIFMGETGLOBALVAR ((Winks_UIFM_Global_s *)(get_module_glob(WK_MODULE_UI)))
#else
//Winks_UIFM_Global_s Winks_UIFM_Global_GB;
extern Winks_UIFM_Global_s Winks_UIFM_Global_GB;
#define WINKS_UIFMGETGLOBALVAR (&Winks_UIFM_Global_GB)
#endif

/*WINKS_PEN_FLAWINKS_PEN_FLAG_PENMOVE 后的 UP无作用*/
/*当控件没有被pendouwn时无作用*/

//#define   DEBUG_WIDGET
#ifdef DEBUG_WIDGET
#define NormalTitle_bg_color WINKS_PIXEL_DARKBLUE
#define Editbox_bg_color  WINKS_PIXEL_WHITE
#define Editbox_hl_color  WINKS_PIXEL_DARKBLUE
#define Button_bg_color  WINKS_PIXEL_BLUE
#define Button_up_color   WINKS_PIXEL_GREEN
#define Button_down_color  WINKS_PIXEL_RED
#define ListItem_bg_color   WINKS_PIXEL_YELLOW
#define ListItem_hl_color    WINKS_PIXEL_DARKRED
#define ListItem_smallicon_color  WINKS_PIXEL_RED
#define ListItem_infoicon_color  WINKS_PIXEL_DARKBLUE
#define ListGroup_bg_color  WINKS_PIXEL_LIGHTGRAY
#define TapListItem_bg_color  WINKS_PIXEL_DARKYELLOW
#define TapListItem_hl_color  WINKS_PIXEL_DARKGREEN
#define TapTitle_bg_color       WINKS_PIXEL_BLACK
#define PopUpItem_bg_color  WINKS_PIXEL_YELLOW
#define PopUpItem_hl_color   WINKS_PIXEL_GREEN
#define PopUpMenu_bg_color  WINKS_PIXEL_LIGHTGRAY
#define  SoftKey_bg_color   WINKS_PIXEL_BLUE
#define  SoftKey_down_color   WINKS_PIXEL_RED
#define SoftKey_hl_color   WINKS_PIXEL_YELLOW
#define  PopUp_bg_color    WINKS_PIXEL_LIGHTGRAY
#define Static_bg_color      WINKS_PIXEL_YELLOW
#define MatrixMenuItem_hl_color   WINKS_PIXEL_GREEN
#define MatrixMenu_bg_color   WINKS_PIXEL_BLACK
#endif

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
void winks_ui_create_vertical_scrollbar(winks_vertical_scrollbar_s *v, int value, int range, int scale,winks_Rect_s Rect)
{

    //v->x = WINKS_UPSCROLLBAR_X;
    //v->y = WINKS_UPSCROLLBAR_Y;
    //v->width = WINKS_SLIDESCROLLBAR_WIDTH;//WINKS_SCREEN_WIDTH-WINKS_UPSCROLLBAR_X;
    //v->height = WINKS_DOWNSCROLLBAR_Y -WINKS_UPSCROLLBAR_Y + WINKS_SLIDESCROLLBAR_Y-WINKS_UPSCROLLBAR_Y;
    v->value = value;
    v->range = range;
    v->scale = scale;
    v->flags = 0;
    v->background_color = WINKS_PIXEL_CYAN;
    v->v_scroll_bg = WINKS_PIXEL_GREEN;
#if 0
    v->up_button.focussed_down_color = WINKS_PIXEL_RED;
    v->up_button.x = WINKS_UPSCROLLBAR_X;
    v->up_button.y = WINKS_UPSCROLLBAR_Y;
    v->up_button.width = WINKS_SLIDESCROLLBAR_WIDTH;//WINKS_SCREEN_WIDTH-WINKS_UPSCROLLBAR_X;
    v->up_button.height  =  WINKS_SLIDESCROLLBAR_Y-WINKS_UPSCROLLBAR_Y;
    v->up_button.normal_down_icon = WINKS_UP_FOCUS_ICONID;
    v->up_button.normal_up_icon = WINKS_UP_ICONID;
    v->up_button.icon_x = WINKS_UPSCROLLBAR_X;
    v->up_button.icon_y = WINKS_UPSCROLLBAR_Y;
#endif
    v->scroll_button.normal_color = WINKS_PIXEL_BLACK;
    v->scroll_button.focussed_down_color = WINKS_PIXEL_GREEN;
    v->scroll_button.x = Rect.v_Left;//WINKS_UPSCROLLBAR_X;
    v->scroll_button.y = Rect.v_Top;//WINKS_UPSCROLLBAR_Y ;//+ v->up_button.height;
    v->scroll_button.width = Rect.v_Right - Rect.v_Left;//WINKS_SLIDESCROLLBAR_WIDTH;//WINKS_SCREEN_WIDTH-WINKS_UPSCROLLBAR_X;
    v->scroll_button.height  =  Rect.v_Bottom- Rect.v_Top;//WINKS_SLIDESCROLLBAR_Y-WINKS_UPSCROLLBAR_Y;
    v->scroll_button.normal_color = WINKS_PIXEL_BLACK;


#if 0
    v->down_button.x = WINKS_DOWNSCROLLBAR_X,
    v->down_button.y = WINKS_DOWNSCROLLBAR_Y;
    v->down_button.width = WINKS_SLIDESCROLLBAR_WIDTH;//WINKS_SCREEN_WIDTH-WINKS_DOWNSCROLLBAR_X;
    v->down_button.height  =  v->up_button.height;
    v->down_button.normal_color = WINKS_PIXEL_BLACK;
    v->down_button.normal_down_icon = WINKS_DOWN_FOCUS_ICONID;
    v->down_button.normal_up_icon = WINKS_DOWN_ICONID;
    v->down_button.icon_x = WINKS_DOWNSCROLLBAR_X;
    v->down_button.icon_y = WINKS_DOWNSCROLLBAR_Y;
#endif
    v->x = Rect.v_Left;//WINKS_UPSCROLLBAR_X;
    v->y = Rect.v_Top;//WINKS_UPSCROLLBAR_Y + v->up_button.height;
    v->width = Rect.v_Right - Rect.v_Left;//WINKS_SLIDESCROLLBAR_WIDTH;//WINKS_SCREEN_WIDTH-WINKS_UPSCROLLBAR_X;
    v->height = Rect.v_Bottom- Rect.v_Top;//WINKS_DOWNSCROLLBAR_Y -WINKS_UPSCROLLBAR_Y -( WINKS_SLIDESCROLLBAR_Y - WINKS_UPSCROLLBAR_Y);

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
    //winks_ui_drawIconButton(&p_scrollbar->up_button);
   // winks_ui_drawIconButton(&p_scrollbar->down_button);

}



#define  WINKS_LISTGROUP_MAX_NUMBER(p_ListGroup) winks_ListGroup_Max_number(p_ListGroup)
static int if_pos_in_area(winks_pen_point_struct Pen_Pos,int x, int y, int width, int height)
{
    if (((Pen_Pos.Pen_x >= x) && (Pen_Pos.Pen_x <= (x + width))
            &&(Pen_Pos.Pen_y >= y) && (Pen_Pos.Pen_y <= (y + height))))
            return TRUE;
    return FALSE;
}

int winks_ListGroup_Max_number(Winks_Widget_ListGroup_s * p_ListGroup)
{
    int  show_list_max_number = 0;

    if (p_ListGroup->ifScrollBar == 0) 
    {
        show_list_max_number = p_ListGroup->v_TotalItemsNum;//WINKS_LIST_MAX_NUMBER;
    }
    else
    {
		if ( p_ListGroup->pa_ItemListData && p_ListGroup->pa_ItemListData[0].bwidget.high )
			show_list_max_number = p_ListGroup->bwidget.high / p_ListGroup->pa_ItemListData[0].bwidget.high;
    }
    return show_list_max_number;
}

#define WINKS_IFISID(Strid) ((Strid <= 255 && Strid > 0)?1:0)
static int if_pos_in_slide_scoorbar_area(winks_pen_point_struct Pen_Pos)
{
    winks_vertical_scrollbar_s*   p_scrollbar = NULL;

    //WINKS_UI_CONTEXT_DEF();
    ////WINKS_UI_CONTEXT_INIT();

    p_scrollbar = &(WINKS_UIFMGETGLOBALVAR->gv_scrollbar);

    return if_pos_in_area(Pen_Pos, p_scrollbar->scroll_button.x,p_scrollbar->scroll_button.y,p_scrollbar->scroll_button.width,p_scrollbar->scroll_button.height);


}

int wk_wg_NormalTitle_Init(Winks_Widget_s*  p_NormalTitle)
{
    Winks_Widget_NormalTitle_s* pNormalTitle = (Winks_Widget_NormalTitle_s* )p_NormalTitle;
    //pNormalTitle->bgimage = 0;
    pNormalTitle->bwidget.font.size = WINKS_MEDIUM_FONT;
    pNormalTitle->bwidget.font.style = WINKS_FONT_STANDARD;
    pNormalTitle->bwidget.font.type =WK_WG_FONTSONG;
    pNormalTitle->bwidget.font.way =WK_WG_FONTNOSCROLL;
    pNormalTitle->bwidget.font.color =WINKS_PIXEL_WHITE;
    pNormalTitle->bwidget.flag &= ~WK_WG_FGGETFOCUS;
    pNormalTitle->bwidget.bgimage = 0;
    pNormalTitle->bwidget.hlimage = 0;
	pNormalTitle->bwidget.bgcolor = WINKS_PIXEL_BLACK;
	pNormalTitle->bwidget.hlcolor = WINKS_PIXEL_BLACK;
    pNormalTitle->v_IconID = WINKS_IMGID_NLTITLE_BLUE_PIC;
    return 0;
}

int wk_wg_NormalTitle_Draw(Winks_Widget_s*  p_NormalTitle)
{
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    int icon_x = 0;
    int icon_y = 0;
    int hliicon_x = 0 ;
    int hliicon_y = 0;
    int x = 0;
    int y = 0;
    int icon = 0;
    int w,h;
    char temp[10] = {0};
    char temp1[10] = {0};
    char * p_temp = NULL;
    Winks_Widget_NormalTitle_s* pNormalTitle = (Winks_Widget_NormalTitle_s* )p_NormalTitle;
    //int flag = pNormalTitle->bwidget.flag & WK_WG_FGGRAY;

    x1 = pNormalTitle->bwidget.lux;
    y1 = pNormalTitle->bwidget.luy;
    x2 = x1 + pNormalTitle->bwidget.width;
    y2 = y1 + pNormalTitle->bwidget.high; 

    WINKS_CLIP_ON(x1,y1, x2, y2);
    //WINKS_GRAY_BEGIN(flag,x1,y1, x2, y2) ;
    Winks_set_Widget_Font(&pNormalTitle->bwidget.font);

    if(pNormalTitle->bwidget.bgimage != 0)
    {
        winks_ui_drawGraphicFromFile(x1 , y1 ,Winks_UIFM_GetImage(pNormalTitle->bwidget.bgimage));
    }
    else
    {
 		winks_ui_fillRect(x1, y1, x2, y2, pNormalTitle->bwidget.bgcolor);//~pNormalTitle->bwidget.font.color);
    }

    if(pNormalTitle->v_IconID != 0)
    {
        winks_ui_drawGraphicFromFile(x1 + icon_x, y1 + icon_y, Winks_UIFM_GetImage(pNormalTitle->v_IconID));
    }
    WINKS_SET_TEXTCOLOR(WINKS_PIXEL_LIGHTWHITE);
    if(WINKS_IFISID(pNormalTitle->v_TitleStringID))
    {
        p_temp = (char *)Winks_UIFM_GetString(pNormalTitle->v_TitleStringID);
    }
    else
    {
        p_temp = (char *)pNormalTitle->v_TitleStringID;

    }
    winks_get_string_wight_high((char *)p_temp,winks_strlen_ucs2_type(p_temp),&w,&h);
    y = y1 + ((pNormalTitle->bwidget.high - WINKS_GETFONTHEIGHT) >> 1);
    x  = x1+((pNormalTitle->bwidget.width - w) >> 1);
    if (w <= ( pNormalTitle->bwidget.width - WINKS_TITLE_STRING_X ))
    {
        winks_ui_textOutlen(x,y,p_temp,winks_strlen_ucs2_type(p_temp));
    }
    else
    {
        winks_ui_textOutlen((x2 - w),y,p_temp,winks_strlen_ucs2_type(p_temp));
    }

    if (pNormalTitle->v_icondigit > 0) 
    {
        sprintf(temp1,"%d",pNormalTitle->v_icondigit);

        Winks_Ascii2UnicodeString(temp, temp1, strlen(temp1));
        winks_get_string_wight_high(temp,(char *)winks_strlen_ucs2_type(temp),&w,&h);

        x = x2 - w -WINKS_TITLE_DIGIT_X;
        //winks_ui_textOutlen(x1,y1,p_temp,(char *)winks_strlen_ucs2_type(p_temp));
        winks_ui_textOutlen(x,y,temp,WK_STRLEN(temp));
    }
    //WINKS_GRAY_END(flag,x1,y1, x2, y2) ;
    WINKS_CLIP_OFF(x1,y1, x2, y2);
    return 0;
}

int wk_wg_NormalTitle_PenHandler( Winks_Widget_s*  p_NormalTitle, int PenType, int x, int y )
{
    return 0;
}

int wk_wg_NormalTitle_KeyHandler( Winks_Widget_s*  p_NormalTitle, int KeyCode, int KeyType )
{
    return 0;
}

int wk_wg_NormalTitle_StateModify( Winks_Widget_s*  p_NormalTitle, int Option, int ifvar )
{
    Winks_Widget_NormalTitle_s* pNormalTitle = (Winks_Widget_NormalTitle_s* )p_NormalTitle;
    return 0;
}
int wk_wg_Editbox_Init(Winks_Widget_s*  p_Editbox)
{
    Winks_Widget_EditBox_s* pEditbox = (Winks_Widget_EditBox_s* )p_Editbox;
    //pEditbox.bgimage = 0;
    pEditbox->bwidget.font.size = WINKS_MEDIUM_FONT;
    pEditbox->bwidget.font.style = WINKS_FONT_STANDARD;
    pEditbox->bwidget.font.type =WK_WG_FONTSONG;
    pEditbox->bwidget.font.way =WK_WG_FONTNOSCROLL;
    pEditbox->bwidget.font.color =WINKS_PIXEL_BLACK;
    pEditbox->bwidget.flag |= WK_WG_FGGETFOCUS;
    pEditbox->bwidget.bgimage = WINKS_IMGID_EDIT_PIC4;
    pEditbox->bwidget.hlimage = WINKS_IMGID_EDIT_PIC4;
	pEditbox->bwidget.bgcolor = WINKS_PIXEL_GRAY;
	pEditbox->bwidget.hlcolor = WINKS_PIXEL_YELLOW;
    return 0;
	
}
int wk_wg_Editbox_Draw(Winks_Widget_s* p_Editbox)
{
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    int icon_x = 0 ;
    int icon_y = 0;
    int hliicon_x = 0 ;
    int hliicon_y = 0;
    int icon = 0;
    Winks_Widget_EditBox_s* pEditbox = (Winks_Widget_EditBox_s* )p_Editbox;
    //int flag = pEditbox->bwidget.flag & WK_WG_FGGRAY;
    char * p_temp = NULL;

    x1 = pEditbox->bwidget.lux;
    y1 = pEditbox->bwidget.luy;
    x2 = x1 + pEditbox->bwidget.width;
    y2 = y1 + pEditbox->bwidget.high; 



    WINKS_CLIP_ON(x1,y1, x2, y2);
    //WINKS_GRAY_BEGIN(flag,x1,y1, x2, y2) ;

    Winks_set_Widget_Font(&pEditbox->bwidget.font);
//     if(pEditbox->bwidget.bgcolor != WINKS_PIXEL_TRANSPARENT)
//         winks_ui_fillRect(x1, y1, x2, y2, pEditbox->bwidget.bgcolor);
//     if(pEditbox->bwidget.bgimage != 0)
//     {
// 
//         winks_ui_drawGraphicFromFile(x1 + icon_x , y1 + icon_y,Winks_UIFM_GetImage(pEditbox->bwidget.bgimage));
// 
//     }


    if(pEditbox->bwidget.flag & WK_WG_FGFOCUS)
    {
		if(pEditbox->bwidget.hlcolor != WINKS_PIXEL_TRANSPARENT)
			winks_ui_fillRect(x1, y1, x2, y2, pEditbox->bwidget.hlcolor);
        if(pEditbox->bwidget.hlimage != 0)
        {
#ifdef DEBUG_WIDGET
            winks_ui_drawRect(x1, y1, x2, y2, Editbox_hl_color);
#endif
            winks_ui_drawGraphicFromFile(x1 + hliicon_x , y1 + hliicon_y,Winks_UIFM_GetImage(pEditbox->bwidget.hlimage));
        }
        else
        {
            winks_ui_drawGraphicFromFile(x1 + hliicon_x , y1 + hliicon_y,Winks_UIFM_GetImage(pEditbox->bwidget.bgimage));
        }
    }
	else
	{
		if(pEditbox->bwidget.bgcolor != WINKS_PIXEL_TRANSPARENT)
			winks_ui_fillRect(x1, y1, x2, y2, pEditbox->bwidget.bgcolor);
		if(pEditbox->bwidget.bgimage != 0)
		{
			winks_ui_drawGraphicFromFile(x1 + icon_x , y1 + icon_y,Winks_UIFM_GetImage(pEditbox->bwidget.bgimage));
		}
	}

    if( pEditbox->dfstring && WINKS_IFISID(pEditbox->dfstring ))
    {

        p_temp = (char *)Winks_UIFM_GetString(pEditbox->dfstring );
    }
    else
    {
        p_temp = (char *)pEditbox->dfstring ;

    }
    pEditbox->scrolling_text.text = p_temp;
    pEditbox->scrolling_text.font_feature = pEditbox->bwidget.font;
    pEditbox->scrolling_text.x= pEditbox->bwidget.lux;
    pEditbox->scrolling_text.y= pEditbox->bwidget.luy;
    pEditbox->scrolling_text.width= pEditbox->bwidget.width;
    pEditbox->scrolling_text.height= pEditbox->bwidget.high;
    Winks_showtext_in_area(&(pEditbox->scrolling_text));
#if 0
    if ( pEditbox->dfstring )/*TEMP strategy*/
    {
        int w, h;
        char *p_temp = NULL;
        p_temp = (char *)(pEditbox->dfstring);
        winks_get_string_wight_high(p_temp,(char *)winks_strlen_ucs2_type(p_temp),&w,&h);
        winks_ui_textOutlen(x1,y1,p_temp,(char *)winks_strlen_ucs2_type(p_temp));
    }
#endif
    //WINKS_GRAY_BEGIN(flag,x1,y1, x2, y2) ;
    WINKS_CLIP_OFF(x1,y1, x2, y2);
    return 0;
}

int wk_wg_Editbox_PenHandler( Winks_Widget_s* p_Editbox, int PenType, int x, int y )
{
    Winks_Widget_EditBox_s * pEditbox = (Winks_Widget_EditBox_s *)p_Editbox;
    switch( PenType )
    {
        case WINKS_PEN_FLAG_PENDOWN:
        {
            pEditbox->bwidget.flag |= WK_WG_FGFOCUS;
            wk_wg_Editbox_Draw( p_Editbox );
        }
        break;
        case WINKS_PEN_FLAG_PENUP:
        {
            pEditbox->bwidget.flag &= ~WK_WG_FGFOCUS;
            wk_wg_Editbox_Draw( p_Editbox );
        }
        break;



        default:
        {

        }
        //error message;
        break;
    }
    return 0;
}

int wk_wg_Editbox_KeyHandler( Winks_Widget_s* p_Editbox, int KeyCode, int KeyType )
{
    int ret = 0;
    Winks_Widget_EditBox_s * p = (Winks_Widget_EditBox_s *) p_Editbox;
    switch (KeyCode)
    {
        case WINKS_KEY_UP:
        {
            if (KeyType == WINKS_KEY_FLAG_KEYDOWN)
                ret = WK_UIFM_FOCUSCHANGE;
        }
            break;
        case WINKS_KEY_DOWN:
        {
            if (KeyType == WINKS_KEY_FLAG_KEYDOWN)
                ret = WK_UIFM_FOCUSCHANGE;
        }
            break;
        case WINKS_KEY_LEFT:
        {
            if (KeyType == WINKS_KEY_FLAG_KEYDOWN)
                ret = WK_UIFM_FOCUSCHANGE;
        }
            break;
        case WINKS_KEY_RIGHT:
        {
            if (KeyType == WINKS_KEY_FLAG_KEYDOWN)
                ret = WK_UIFM_FOCUSCHANGE;
        }
            break;
        default:
        {
            break;
        }
    }
    return ret;
}

int wk_wg_Editbox_StateModify( Winks_Widget_s* pEditbox, int Option, int ifvar )
{
    const char* pchar = NULL;
    Winks_Widget_s *pWidget = pEditbox;

    if( (Option == WK_WG_FGFOCUS) )
    {
        if( (pWidget->flag & WK_WG_FGFOCUS) && !ifvar )
        {
            pWidget->flag &= ~WK_WG_FGFOCUS;
            wk_wg_Editbox_Draw(pWidget);
            return WK_UIFM_CONTENTCHANGE;
        }
        else if( (!(pWidget->flag & WK_WG_FGFOCUS)) && ifvar )
        {
            pWidget->flag |= WK_WG_FGFOCUS;
            wk_wg_Editbox_Draw(pWidget);
            return WK_UIFM_CONTENTCHANGE;
        }
    }
    return WK_UIFM_SUCCESS;
}

int wk_wg_Button_Init(Winks_Widget_s*  p_Button)
{
    Winks_Widget_Button_s * pButton = (Winks_Widget_Button_s *)p_Button;
    //pButton->bgimage = 0;
    pButton->bwidget.font.size = WINKS_MEDIUM_FONT;
    pButton->bwidget.font.style = WINKS_FONT_STANDARD;
    pButton->bwidget.font.type =WK_WG_FONTSONG;
    pButton->bwidget.font.way =WK_WG_FONTNOSCROLL;
    pButton->bwidget.font.color =WINKS_PIXEL_BLACK;
    pButton->bwidget.flag |= WK_WG_FGGETFOCUS;
    pButton->bwidget.bgimage = 0;
    pButton->bwidget.hlimage = 0;
    pButton->bwidget.bgcolor = WINKS_PIXEL_MAGENTA;
    pButton->bwidget.hlcolor = WINKS_PIXEL_BLUE;
    pButton->bwidget.bgfontcolor = WINKS_PIXEL_BLACK;
    pButton->bwidget.hlfontcolor = WINKS_PIXEL_LIGHTWHITE;
    pButton->normal_up_icon = 0;
    pButton->normal_down_icon = 0;
    return 0;

	
}
int wk_wg_Button_Draw(Winks_Widget_s * p_Button)
{

    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    int icon_x = 0 ;
    int icon_y = 0;
    int hliicon_x = 0 ;
    int hliicon_y = 0;
    int icon = 0;
    char * p_temp = NULL;
    int w = 0;
    int h = 0;

    Winks_Widget_Button_s * pButton = (Winks_Widget_Button_s *)p_Button;
    //int flag = pButton->bwidget.flag & WK_WG_FGGRAY;

    x1 = pButton->bwidget.lux;
    y1 = pButton->bwidget.luy;
    x2 = x1 + pButton->bwidget.width ;
    y2 = y1 + pButton->bwidget.high ; 

    WINKS_CLIP_ON(x1,y1, x2, y2);
    //WINKS_GRAY_BEGIN(flag,x1,y1, x2, y2);

    Winks_set_Widget_Font(&pButton->bwidget.font);
    if(pButton->bwidget.bgcolor != WINKS_PIXEL_TRANSPARENT)
    {
        winks_ui_fillRect(x1, y1, x2, y2, pButton->bwidget.bgcolor);
    }
    icon = pButton->bwidget.bgimage;
    if(icon!= 0)
    {

        winks_ui_drawGraphicFromFile(x1  , y1 ,Winks_UIFM_GetImage(icon));

    }

    if(pButton->bwidget.flag & WK_WG_FGFOCUS)
    {
        WINKS_SET_TEXTCOLOR(pButton->bwidget.hlfontcolor);
        if(pButton->bwidget.hlimage != 0)
        {
            winks_ui_drawGraphicFromFile(x1 + hliicon_x , y1 + hliicon_y,Winks_UIFM_GetImage(pButton->bwidget.hlimage));
        }
    }
	else
	{
		WINKS_SET_TEXTCOLOR(pButton->bwidget.bgfontcolor);
	}
    if (pButton->bwidget.flag & WK_WG_FGPUSH)
    {
        icon = pButton->normal_down_icon;
        pButton->bwidget.flag &= ~WK_WG_FGPUSH;
#ifdef DEBUG_WIDGET
        winks_ui_fillRect(x1, y1, x2 , y2 , Button_down_color);
#endif
        
    }
	else
	{
		icon = pButton->normal_up_icon;
	}
    if(icon != 0)
    {
#ifdef DEBUG_WIDGET
        winks_ui_fillRect(x1 + 2, y1 + 2, x2 -2, y2 -2, Button_up_color);
#endif
        winks_ui_drawGraphicFromFile(x1 + icon_x , y1 + icon_y,Winks_UIFM_GetImage(icon));
    }
    if( pButton->v_stringId && WINKS_IFISID(pButton->v_stringId))
    {

        p_temp = (char *)Winks_UIFM_GetString(pButton->v_stringId);
    }
    else
    {
        p_temp = (char *)pButton->v_stringId;

    }
    //Winks_showtext_in_area (p_temp,(char *)winks_strlen_ucs2_type(p_temp),x1,y1, x2, y2);
    winks_get_string_wight_high(p_temp,(char *)winks_strlen_ucs2_type(p_temp),&w,&h);

    if (pButton->bwidget.width > w) 
        x1  += ((pButton->bwidget.width - w) >> 1);
    y1 += ((pButton->bwidget.high - WINKS_GETFONTHEIGHT) >> 1);
    winks_ui_textOutlen(x1,y1,p_temp,(char *)winks_strlen_ucs2_type(p_temp));
    //WINKS_GRAY_BEGIN(flag,x1,y1, x2, y2);
    WINKS_CLIP_OFF(x1,y1, x2, y2);
    return 0;

}

int wk_wg_Button_PenHandler(Winks_Widget_s * p_Button, int PenType, int x, int y )
{
    Winks_Widget_Button_s * pButton = (Winks_Widget_Button_s *)p_Button;
    switch( PenType )
    {
        case WINKS_PEN_FLAG_PENDOWN:
        {
            pButton->bwidget.flag |= WK_WG_FGPUSH;
            wk_wg_Button_Draw( p_Button );
        }
        break;
        case WINKS_PEN_FLAG_PENUP:
        {
            pButton->bwidget.flag |= WK_WG_FGFOCUS;
            wk_wg_Button_Draw( p_Button );
        }
        break;



        default:
        {

        }
        //error message;
        break;
    }
    return 0;
}

int wk_wg_Button_KeyHandler(Winks_Widget_s* p_Button, int KeyCode, int KeyType )
{
    int ret = 0;
    Winks_Widget_Button_s * pButton = (Winks_Widget_Button_s *)p_Button;

    switch( KeyCode )
    {
        case WINKS_KEY_OK:
                            		{
                            			if (KeyType == WINKS_KEY_FLAG_KEYDOWN  ) //WINKS_KEY_FLAG_KEYDOWN
                            			{
                            				pButton->bwidget.flag |= WK_WG_FGPUSH;
                            				wk_wg_Button_Draw( p_Button );
                            			}
                            		}break;
        case WINKS_KEY_UP:
        case WINKS_KEY_DOWN:
        case WINKS_KEY_LEFT:
        case WINKS_KEY_RIGHT:
                            		{
                            			//if (KeyType == WINKS_KEY_FLAG_KEYDOWN)
                            				ret = WK_UIFM_FOCUSCHANGE;//Winks_UI_TapTitle_Previous_Column(pTapTitle);
                            		}break;
        	


        default:
        {

        }
        //error message;
        break;
    }
    return ret;
}


int wk_wg_Button_StateModify( Winks_Widget_s* p_Button, int Option, int ifvar )
{
    Winks_Widget_Button_s * pButton = (Winks_Widget_Button_s *)p_Button;
    if (Option == WK_WG_FGGRAY) 
    {
        if (ifvar == 1) 
            pButton->bwidget.flag |=  WK_WG_FGGRAY;//|=  WK_WG_FGGRAY;
        else
        {
            //if (ifvar == 1) 
            pButton->bwidget.flag &=  ~WK_WG_FGGRAY;
        }

    }
    if (Option == WK_WG_FGFOCUS) 
    {
        if (ifvar == 1) 
            pButton->bwidget.flag |=  WK_WG_FGFOCUS;//|=  WK_WG_FGFOCUS;
        else
        {
            //if (ifvar == 1) 
            pButton->bwidget.flag &=  ~WK_WG_FGFOCUS;
        }

    }
    wk_wg_Button_Draw(p_Button);
    return 0;
}
static int wk_wg_ListItem_Init(Winks_Widget_Listitem_s*  pListItem)
{
    //pListItem->bgimage = 0;
    pListItem->bwidget.font.size = WINKS_MEDIUM_FONT;
    pListItem->bwidget.font.style = WINKS_FONT_STANDARD;
    pListItem->bwidget.font.type =WK_WG_FONTSONG;
    pListItem->bwidget.font.way =WK_WG_FONTNOSCROLL;
    pListItem->bwidget.font.color =WINKS_PIXEL_BLACK;
    pListItem->bwidget.flag |= WK_WG_FGGETFOCUS;
    pListItem->bwidget.bgimage = WINKS_IMGID_ADVANCESETTING_ITEMBKG;
    pListItem->bwidget.hlimage = WINKS_IMGID_ADVANCESETTING_ITEMHL;
    pListItem->bwidget.bgcolor = WINKS_PIXEL_GRAY;
    pListItem->bwidget.hlcolor = WINKS_PIXEL_GRAY;
    pListItem->bwidget.bgfontcolor = WINKS_PIXEL_BLACK;
    pListItem->bwidget.hlfontcolor = WINKS_PIXEL_BLACK;
    return 0;

	
}
int wk_wg_ListItem_Draw(Winks_Widget_Listitem_s * pListItem,int Group_flag)
{	
    //Winks_gui_create_fixed_text_menuitem();
    //Winks_gui_show_fixed_text_menuitem();

    int x = 0;
    int y = 0;
#if 0
    int radio_on  =WINKS_UI_RADIOSEL_IMAGEID;
    int radio_off =WINKS_UI_RADIO_IMAGEID;
    int check_on = WINKS_UI_CHECKBOXSEL_IMAGEID;
    int check_off =WINKS_UI_CHECKBOX_IMAGEID;
#endif
    //int radio_on  =0;
    // int radio_off =0;
    //int check_on = WINKS_IMGID_ICON_RIGHTMARK;
    // int check_off =0;
    int gap_y = 0;
    int icon_gap = ICON_GAP_X;



    //char * temp[1024] ;
    char * text = NULL;
    int textclip_lefttop_x,textclip_lefttop_y;
    int textclip_rightbottom_x,textclip_rightbottom_y;
    int w = 0, h = 0;
    int excursion = 0;


    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    int icon_x = 0 ;
    int icon_y = 0;
    int hliicon_x = 0 ;
    int hliicon_y = 0;
    int icon = 0;
    //int flag = pListItem->bwidget.flag & WK_WG_FGGRAY;
    //Winks_Widget_Listitem_s * pListItem = (Winks_Widget_Listitem_s *)p_ListItem;

    ////WINKS_UI_CONTEXT_DEF()
    //////WINKS_UI_CONTEXT_INIT();

    x1 = pListItem->bwidget.lux;
    y1 = pListItem->bwidget.luy;
    x2 = x1 + pListItem->bwidget.width;
    y2 = y1 + pListItem->bwidget.high; 

    WINKS_CLIP_ON(x1,y1, x2, y2);
    //WINKS_GRAY_BEGIN(flag,x1,y1, x2, y2);

    Winks_set_Widget_Font(&pListItem->bwidget.font);
    if(pListItem->bwidget.bgcolor != WINKS_PIXEL_TRANSPARENT)
        winks_ui_fillRect(x1, y1, x2, y2, pListItem->bwidget.bgcolor);

    if(WINKS_IFISID(pListItem->v_ItemStrID ))	
    {
        //winks_ui_textOutlen(x, y,(char *)Winks_UIFM_GetString( pListItem->v_ItemStrID),strlen((char *)Winks_UIFM_GetString( pListItem->v_ItemStrID)));
        text = (char *)Winks_UIFM_GetString( pListItem->v_ItemStrID);
    }
    else
    {
        //winks_ui_textOutlen(x, y,(char *)pListItem->v_ItemStrID,strlen((char *)pListItem->v_ItemStrID));
        text = (char *)pListItem->v_ItemStrID;
    }
    {
        winks_get_string_wight_high(text, winks_strlen_ucs2_type(text),&w,&h);
    }
    if(pListItem->bwidget.bgimage != 0)
    {

        winks_ui_drawGraphicFromFile(x1  , y1 ,Winks_UIFM_GetImage(pListItem->bwidget.bgimage));
    }

    if(pListItem->bwidget.flag & WK_WG_FGFOCUS)
    {
        if(Group_flag& WK_WG_FGFOCUS)
        {
            if(pListItem->bwidget.hlimage != 0)
            {

#ifdef DEBUG_WIDGET
                winks_ui_fillRect(x1, y1, x2, y2, ListItem_hl_color);
#endif
                winks_ui_drawGraphicFromFile(x1 + hliicon_x , y1 + hliicon_y,Winks_UIFM_GetImage(pListItem->bwidget.hlimage));
            }
        }

    }

    if(pListItem->v_ListType == WK_LIST_TYPE_TEXT)
    {
        x = x1 + WINKS_LIST_FIRSTSTRINGONLY_X;
        textclip_lefttop_x = x;
        y = y1+ (( pListItem->bwidget.high -h) >> 1);
        textclip_rightbottom_x = x2;

    }
    else if(pListItem->v_ListType == WK_LIST_TYPE_TEXT_ICON)
    {
        if(pListItem->v_IconID != 0)
        {
            textclip_lefttop_x = x1+ (icon_gap << 1) + SMALL_ICON_WIDTH;
            WINKS_CLIP_ON(x1+icon_gap, y1 + gap_y,x1+icon_gap + WINKS_LIST_ICON_WIDTH, y1 + gap_y + WINKS_LIST_ICON_HEIGHT);
            winks_ui_drawGraphicFromFile(x1+icon_gap, y1 + gap_y,Winks_UIFM_GetImage(pListItem->v_IconID));//Winks_UIFM_GetImage(icon)
            WINKS_CLIP_OFF(x1+icon_gap, y1 + gap_y,x1+icon_gap + WINKS_LIST_ICON_WIDTH, y1 + gap_y + WINKS_LIST_ICON_HEIGHT);
        }
        else
        {
            //textclip_lefttop_x = x1+ (icon_gap << 1) ;
            textclip_lefttop_x = x1+ (icon_gap << 1) + SMALL_ICON_WIDTH;
        }
        gap_y = (pListItem->bwidget.high - SMALL_ICON_HEIGHT) >> 1;
        if(pListItem->v_ItemSetFlag) //标记
        {
            textclip_lefttop_x = x1+ (icon_gap << 1) + SMALL_ICON_WIDTH;
            WINKS_CLIP_ON(x1+icon_gap, y1 + gap_y,x1+icon_gap + WINKS_LIST_ICON_WIDTH, y1 + gap_y + WINKS_LIST_ICON_HEIGHT);
             #ifdef DEBUG_WIDGET
            winks_ui_fillRect(x1+icon_gap, y1 + gap_y,x1+icon_gap + WINKS_LIST_ICON_WIDTH, y1 + gap_y + WINKS_LIST_ICON_HEIGHT, ListItem_smallicon_color);
#endif

            winks_ui_drawGraphicFromFile(x1+icon_gap, y1 + gap_y,Winks_UIFM_GetImage(pListItem->v_IconID ));//Winks_UIFM_GetImage(icon)
            WINKS_CLIP_OFF(x1+icon_gap, y1 + gap_y,x1+icon_gap + WINKS_LIST_ICON_WIDTH, y1 + gap_y + WINKS_LIST_ICON_HEIGHT);
        }


        if(pListItem->v_info_imageId != 0)
        {
            gap_y = (pListItem->bwidget.high - INFO_ICON_HEIGHT) >> 1;
            textclip_rightbottom_x = x2 - INFO_ICON_WIDTH - ICON_GAP_X;
            if(pListItem->bwidget.flag  & WK_WG_FGFOCUS )
            {   
                icon = pListItem->v_hlinfo_imageId;
            }
            else
            {
                icon = pListItem->v_info_imageId;
            }
            WINKS_CLIP_ON(textclip_rightbottom_x+icon_gap, y1 + gap_y,textclip_rightbottom_x+icon_gap + WINKS_LIST_ICON_WIDTH, y1 + gap_y + WINKS_LIST_ICON_HEIGHT);
#ifdef DEBUG_WIDGET
            winks_ui_fillRect(textclip_rightbottom_x+icon_gap, y1 + gap_y,textclip_rightbottom_x+icon_gap + WINKS_LIST_ICON_WIDTH, y1 + gap_y + WINKS_LIST_ICON_HEIGHT, ListItem_infoicon_color);
#endif

            winks_ui_drawGraphicFromFile(textclip_rightbottom_x+icon_gap, y1 + gap_y,Winks_UIFM_GetImage(icon));//Winks_UIFM_GetImage(icon)
            WINKS_CLIP_OFF(textclip_rightbottom_x+icon_gap, y1 + gap_y,textclip_rightbottom_x+icon_gap + WINKS_LIST_ICON_WIDTH, y1 + gap_y + WINKS_LIST_ICON_HEIGHT);

        }
        else 
        {
            textclip_rightbottom_x = x2 - ICON_GAP_X;
        }

    }
    else
    {
        if(pListItem->v_ListType == WK_LIST_TYPE_RADIO)
        {
            if (pListItem->v_ItemSetFlag) //(pListItem->bwidget.flag  &  WK_WG_FGFOCUS)
            {
                icon = pListItem->v_ItemSelected_Icon;
            }
            else
            {
                icon = pListItem->v_ItemNoselected_Icon;
            }
            y = y1 + pListItem->bwidget.high -WINKS_GETFONTHEIGHT;
        }
        if(pListItem->v_ListType == WK_LIST_TYPE_CHECKBOX)
        {
            if(pListItem->v_ItemSetFlag)
            {
                icon = pListItem->v_ItemSelected_Icon;
            }
            else
            {
                icon = pListItem->v_ItemNoselected_Icon;
            }
        }
        if(pListItem->v_info_imageId != 0)
        {
            gap_y = (pListItem->bwidget.high - INFO_ICON_HEIGHT) >> 1;
            textclip_rightbottom_x = x2 - INFO_ICON_WIDTH - ICON_GAP_X;
            if(pListItem->bwidget.flag  & WK_WG_FGFOCUS )
            {   
                icon = pListItem->v_hlinfo_imageId;
            }
            else
            {
                icon = pListItem->v_info_imageId;
            }
            WINKS_CLIP_ON(textclip_rightbottom_x+icon_gap, y1 + gap_y,textclip_rightbottom_x+icon_gap + WINKS_LIST_ICON_WIDTH, y1 + gap_y + WINKS_LIST_ICON_HEIGHT);
#ifdef DEBUG_WIDGET
            winks_ui_fillRect(textclip_rightbottom_x+icon_gap, y1 + gap_y,textclip_rightbottom_x+icon_gap + WINKS_LIST_ICON_WIDTH, y1 + gap_y + WINKS_LIST_ICON_HEIGHT, ListItem_smallicon_color);
#endif

            winks_ui_drawGraphicFromFile(icon_x, icon_y,Winks_UIFM_GetImage(icon));//Winks_UIFM_GetImage(icon)
            WINKS_CLIP_OFF(textclip_rightbottom_x+icon_gap, y1 + gap_y,textclip_rightbottom_x+icon_gap + WINKS_LIST_ICON_WIDTH, y1 + gap_y + WINKS_LIST_ICON_HEIGHT);

        }
        else 
        {
            textclip_rightbottom_x = x2 - ICON_GAP_X;
        }
        icon_x = x1 +WINKS_LIST_ICON_X;
        icon_y = y1+ MAX(((pListItem->bwidget.high - WINKS_LIST_CKECKICON_HEIGHT) >> 1),0);
        if (icon != 0)
        {
            WINKS_CLIP_ON(icon_x, icon_y,icon_x + WINKS_LIST_ICON_WIDTH, icon_y + WINKS_LIST_ICON_HEIGHT);
#ifdef DEBUG_WIDGET
            winks_ui_fillRect(textclip_rightbottom_x+icon_gap, y1 + gap_y,textclip_rightbottom_x+icon_gap + WINKS_LIST_ICON_WIDTH, y1 + gap_y + WINKS_LIST_ICON_HEIGHT, ListItem_smallicon_color);
#endif
            winks_ui_drawGraphicFromFile(icon_x, icon_y,Winks_UIFM_GetImage(icon));//Winks_UIFM_GetImage(icon)
            WINKS_CLIP_OFF(icon_x, icon_y,icon_x + WINKS_LIST_ICON_WIDTH, icon_y + WINKS_LIST_ICON_HEIGHT);
        }
        //x = WINKS_LIST_FIRSTSTRINGONLY_X + WINKS_LIST_ICON_X;
        x = WINKS_LIST_ICON_X + WINKS_LIST_ICON_WIDTH;
        textclip_lefttop_x = x;
        //y =   ( index_in_screen -1) * WINKS_HIGHLIGHT_HEIGHT+ WINKS_LIST_FIRSTSTRINGONLY_Y;
    }

    //y = y1 + (( pListItem->bwidget.high -h) >> 1);
    // excursion = (WINKS_HIGHLIGHT_LISTITEM_HEIGHT  - WINKS_GETFONTHEIGHT) >> 1;  
    //textclip_lefttop_x = x;
    textclip_lefttop_y = y1;// - excursion;//WINKS_HIGHLIGHT_EXCURSION_HEIGHT;

    //textclip_rightbottom_x = pListItem->bwidget.width;
    textclip_rightbottom_y = y2 ;
    if(pListItem->bwidget.flag == WK_WG_FGFOCUS )
    {
    WINKS_SET_TEXTCOLOR(pListItem->bwidget.hlfontcolor);
    {
    winks_get_string_wight_high(text, winks_strlen_ucs2_type(text),&w,&h);
    }
#if  0  
    //WINKS_SET_TEXTCOLOR(WINKS_PIXEL_LIGHTWHITE);
    if(w > (textclip_rightbottom_x - textclip_lefttop_x ))
    {

    pListItem->scrolling_text.draw_backgroundimage = (char *)Winks_UIFM_GetImage(pListItem->bwidget.hlimage);
    pListItem->scrolling_text.x = textclip_lefttop_x;
    pListItem->scrolling_text.y = textclip_lefttop_y;
    pListItem->scrolling_text.width = textclip_rightbottom_x - textclip_lefttop_x;
    pListItem->scrolling_text.height = textclip_rightbottom_y - textclip_lefttop_y;




    pListItem->scrolling_text.font_feature = pListItem->bwidget.font;




    pListItem->scrolling_text.text = text;
    pListItem->scrolling_text.scroll_x = 0;
    pListItem->scrolling_text.text_width = w;
    pListItem->scrolling_text.text_height = h;
    pListItem->scrolling_text.scroll_counter = 4;  
    pListItem->scrolling_text.scroll_size = WINKS_UI_TEXT_MENUITEM_SCROLL_SIZE;
    pListItem->scrolling_text.scroll_gap = WINKS_UI_TEXT_MENUITEM_SCROLL_GAP;
    pListItem->scrolling_text.scroll_pause = WINKS_UI_TEXT_MENUITEM_SCROLL_PAUSE;

    //pListItem->scrolling_text.scroll_time = WINKS_UI_TEXT_MENUITEM_SCROLL_TIME;
    pListItem->scrolling_text.flags = WINKS_SCROLLING_TEXT_TWO_DIRECTION_SCROLL;
    pListItem->scrolling_text.flags |= WINKS_SCROLLING_TEXT_SCROLL_TRAIL_TO_HEAD;
    pListItem->scrolling_text.flags |= WINKS_SCROLLING_TEXT_ENABLE_SCROLL;

    winks_ui_handle_scrolling_text(&pListItem->scrolling_text);
#if 0
    if(WINKS_UIFMGETGLOBALVAR->scrooltext_flag == 0) 
    {

    winks_ui_create_scrolling_text(
    WINKS_UIFMGETGLOBALVAR->winks_scroll_string,
    textclip_lefttop_x,  
    textclip_lefttop_y,
    textclip_rightbottom_x - textclip_lefttop_x,
    textclip_rightbottom_y - textclip_lefttop_y,
    text,
    NULL,//winks_ui_handle_scrolling_text,
    //void (*draw_background) (int x1, int y1, int x2, int y2),
    WINKS_PIXEL_LIGHTWHITE,
    WINKS_STRINGSCROLL_LEFT,
    WINKS_HILIGHT_COLOR,
    NULL,
    //winks_color border_color
    font_feature
    );


    //winks_ui_redraw_scrolling_text(&s);
    WINKS_UIFMGETGLOBALVAR->scrooltext_flag = 1;
    winks_ui_scrolling_text_start(WINKS_UIFMGETGLOBALVAR->winks_scroll_string);
    }
#endif
    winks_ui_redraw_scrolling_text(&pListItem->scrolling_text);
    }
    else
#endif
    {
    x = textclip_lefttop_x;
    y = y1 + (( pListItem->bwidget.high -h) >> 1);
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
    WINKS_SET_TEXTCOLOR(pListItem->bwidget.bgfontcolor);
    //WINKS_SET_TEXTCOLOR(WINKS_PIXEL_LIGHTWHITE);
    x = textclip_lefttop_x;
    y = y1 + (( pListItem->bwidget.high -h) >> 1);
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
    //WINKS_GRAY_END(flag,x1,y1, x2, y2);
    WINKS_CLIP_OFF(x1,y1, x2, y2);

    return 0;
  }

int wk_wg_ListGroup_Init(Winks_Widget_s*  p_ListGroup)
{
    Winks_Widget_ListGroup_s * pListGroup = (Winks_Widget_ListGroup_s * )p_ListGroup;
    Winks_Widget_Listitem_s * pListItem = NULL;
    int i = 0;
    //pListGroup->bgimage = 0;
    pListGroup->bwidget.font.size = WINKS_MEDIUM_FONT;
    pListGroup->bwidget.font.style = WINKS_FONT_STANDARD;
    pListGroup->bwidget.font.type =WK_WG_FONTSONG;
    pListGroup->bwidget.font.way =WK_WG_FONTNOSCROLL;
    pListGroup->bwidget.font.color =WINKS_PIXEL_BLACK;
    pListGroup->bwidget.flag |= WK_WG_FGGETFOCUS;
    pListGroup->bwidget.bgimage = 0;
    pListGroup->bwidget.hlimage = 0;
    pListGroup->bwidget.bgcolor = WINKS_PIXEL_GRAY;
    pListGroup->bwidget.hlcolor = WINKS_PIXEL_GRAY;
    pListGroup->bwidget.bgfontcolor = 0;
    pListGroup->bwidget.hlfontcolor = 0;

    for(i = 0 , pListItem = pListGroup->pa_ItemListData;i < pListGroup->v_TotalItemsNum&&pListItem != NULL;i++, pListItem =pListItem->pnext)
    {
        if(pListGroup->v_FocusItemIndex == i)
            pListItem->bwidget.flag |= WK_WG_FGFOCUS;
        wk_wg_ListItem_Init(pListItem);
    }

    return 0;
}
int wk_wg_ListGroup_Draw(Winks_Widget_s * p_ListGroup)
{
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    int icon_x = 0 ;
    int icon_y = 0;
    int hliicon_x = 0 ;
    int hliicon_y = 0;
    int icon = 0;
    int stlen = 0;
    char * pc_temp = NULL;
    int w = 0, h = 0;
    int row_number, column_number;
    int mainmenuitem_height,mainmenuitem_weight;
    int x_gap = 0, y_gap = 0;
    int excursion = 0;
    int index = 0;
    int show_list_max_number = 0;
    int number = 0;
    int i = 0;
    winks_Rect_s Rect;


    Winks_Widget_ListGroup_s * pListGroup = (Winks_Widget_ListGroup_s * )p_ListGroup;
    Winks_Widget_Listitem_s * pListItem = NULL;
    Winks_Widget_Listitem_s * pListItem_temp = NULL;
    int flag = pListGroup->bwidget.flag;
    x1 = pListGroup->bwidget.lux;
    y1 = pListGroup->bwidget.luy;
    x2 = x1 + pListGroup->bwidget.width;
    y2 = y1 + pListGroup->bwidget.high ; 


    Rect.v_Left = x2 - WINKS_SLIDESCROLLBAR_WIDTH;
    Rect.v_Top= y1;
    Rect.v_Bottom= y2;
    Rect.v_Right= x2;


    WINKS_CLIP_ON(x1,y1, x2, y2);
    //WINKS_GRAY_BEGIN(flag,x1,y1, x2, y2) ;




    Winks_set_Widget_Font(&pListGroup->bwidget.font);
    if(pListGroup->bwidget.bgcolor != WINKS_PIXEL_TRANSPARENT)
        winks_ui_fillRect(x1, y1, x2, y2, pListGroup->bwidget.bgcolor);


    if(pListGroup->bwidget.bgimage != 0)
    {

    winks_ui_drawGraphicFromFile(x1, y1 ,Winks_UIFM_GetImage(pListGroup->bwidget.bgimage));

    }


    for(i = 0 ,pListItem_temp = pListGroup->pa_ItemListData  ;i< pListGroup->v_FirstItemIndx;pListItem_temp = pListItem_temp->pnext,i++ )
    {
     
    }
    if (pListGroup->ifScrollBar == 1) 
    {

        for(index = pListGroup->v_FirstItemIndx,pListItem = pListItem_temp;index < pListGroup->v_TotalItemsNum  && index < pListGroup->v_FirstItemIndx + WINKS_LISTGROUP_MAX_NUMBER(pListGroup);pListItem = pListItem->pnext ,index++)
        {
            //	for (index = 0,pListItem = pListItem_temp; index < pListGroup->v_TotalItemsNum; pListItem = pListItem->pnext ,  index++) 

            pListItem->bwidget.lux = x1;
            pListItem->bwidget.luy = (index -pListGroup->v_FirstItemIndx) * pListItem->bwidget.high +y1;
            if(pListGroup->v_TotalItemsNum > WINKS_LISTGROUP_MAX_NUMBER(pListGroup))
                pListItem->bwidget.width = pListGroup->bwidget.width - (WINKS_UPSCROLLBAR_WIDTH + WINKS_LIST_STRINGONLY_SCROOBAR_GAP);
            else
            {
                pListItem->bwidget.width = pListGroup->bwidget.width;
            }
            //pListItem->bwidget.high = WINKS_HIGHLIGHT_LISTITEM_HEIGHT;
            if(pListGroup->v_FocusItemIndex == index)  
                pListItem->bwidget.flag |= WK_WG_FGFOCUS;
            else
            {
                pListItem->bwidget.flag &= ~WK_WG_FGFOCUS;
            }
            wk_wg_ListItem_Draw(pListItem,flag);
            //show_list_max_number = WINKS_LIST_MAX_NUMBER;

        }
    }
    else
    {
        for(index = 0,pListItem = pListGroup->pa_ItemListData  ; index < pListGroup->v_TotalItemsNum; pListItem = pListItem->pnext,index++) 
        {
            if(pListGroup->v_FocusItemIndex == index)
            {
                pListItem->bwidget.flag |= WK_WG_FGFOCUS;
            }
            else
            {
                pListItem->bwidget.flag &= ~WK_WG_FGFOCUS;
            }
            wk_wg_ListItem_Draw(pListItem, flag);
        }
    }

    if(pListGroup->v_TotalItemsNum > WINKS_LISTGROUP_MAX_NUMBER(pListGroup))
    {
        winks_ui_create_vertical_scrollbar(&(WINKS_UIFMGETGLOBALVAR->gv_scrollbar), pListGroup->v_FirstItemIndx, pListGroup->v_TotalItemsNum, WINKS_LISTGROUP_MAX_NUMBER(pListGroup),Rect);
        winks_ui_drawVerticalScrollBar(&(WINKS_UIFMGETGLOBALVAR->gv_scrollbar));
    }               
    //WINKS_GRAY_END(flag,x1,y1, x2, y2) ;
    WINKS_CLIP_OFF(x1,y1, x2, y2);
    return 0;

}

/**************************************************************

	FUNCTION NAME		: Winks_UI_Goto_Next_Item

  	PURPOSE		        : Go to Next Item.

	INPUT PARAMETERS	: winks_List_Screen_s * p_Screen
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

int  Winks_UI_Goto_Next_ListItem(Winks_Widget_ListGroup_s * pListGroup)
{	
    int old_index = 0;
    int temp_index = 0;
    int ret = 0;
    //Winks_Widget_ListGroup_s * pListGroup = (Winks_Widget_ListGroup_s * )p_ListGroup;

    //WINKS_UI_CONTEXT_DEF()
    ////WINKS_UI_CONTEXT_INIT()

    Winks_assert(pListGroup != NULL);

#if 0
    if (WINKS_UIFMGETGLOBALVAR->winks_scroll_string.scroll_pause == 1)
    {
        winks_ui_scrolling_text_stop(WINKS_UIFMGETGLOBALVAR->winks_scroll_string);
    }
#endif

    WINKS_UIFMGETGLOBALVAR->scrooltext_flag = 0;



    old_index = pListGroup->v_FocusItemIndex;
    pListGroup->v_FocusItemIndex =  pListGroup->v_FocusItemIndex + 1;

	if (pListGroup->v_FocusItemIndex >= pListGroup->v_TotalItemsNum )
	{
		//pListGroup->v_FirstItemIndx = 0;
		pListGroup->v_FocusItemIndex = pListGroup->v_TotalItemsNum- 1;
		ret = WK_UIFM_FOCUSCHANGE;
	}


    if(( (pListGroup->v_FocusItemIndex - pListGroup->v_FirstItemIndx + 1) > WINKS_LISTGROUP_MAX_NUMBER(pListGroup)) || (pListGroup->v_FocusItemIndex > pListGroup->v_TotalItemsNum))
    {	
        if( pListGroup->v_FirstItemIndx >= pListGroup->v_TotalItemsNum - (WINKS_LISTGROUP_MAX_NUMBER(pListGroup)-1))
        {
           	pListGroup->v_FocusItemIndex = pListGroup->v_TotalItemsNum- 1;
			ret = WK_UIFM_FOCUSCHANGE;


        }
        else
        {
            (pListGroup->v_FirstItemIndx)++;
            //pListGroup->v_FocusItemIndex = WINKS_LISTGROUP_MAX_NUMBER(pListGroup);
        }
    }
//     else if (pListGroup->v_FocusItemIndex >= pListGroup->v_TotalItemsNum )
//     {
//         pListGroup->v_FirstItemIndx = 0;
//         pListGroup->v_FocusItemIndex = 0;
// 
//     }



    wk_wg_ListGroup_Draw((Winks_Widget_s *)pListGroup);
    return ret;

}


/**************************************************************

	FUNCTION NAME		: Winks_UI_Goto_Previous_Item

  	PURPOSE		        : Go to Previous Item.

	INPUT PARAMETERS	: winks_List_Screen_s * p_Screen
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

int  Winks_UI_Goto_Previous_ListItem(Winks_Widget_ListGroup_s * pListGroup)
{	
    int old_index = 0;
    int  ret = 0;

    //WINKS_UI_CONTEXT_DEF()
    //WINKS_UI_CONTEXT_INIT()

    Winks_assert(pListGroup != NULL);

#if 0
    if (WINKS_UIFMGETGLOBALVAR->winks_scroll_string.scroll_pause == 1)
    {
        winks_ui_scrolling_text_stop(WINKS_UIFMGETGLOBALVAR->winks_scroll_string);
    }
#endif

    WINKS_UIFMGETGLOBALVAR->scrooltext_flag = 0;



    old_index = pListGroup->v_FocusItemIndex;
    pListGroup->v_FocusItemIndex = pListGroup->v_FocusItemIndex - 1;
    if(pListGroup->v_FocusItemIndex == -1 )
    {	
        ret = WK_UIFM_FOCUSCHANGE;
		pListGroup->v_FocusItemIndex = 0;
    }
#if 0
    if( pListGroup->v_FirstItemIndx == 0)
    {

        if (pListGroup->v_TotalItemsNum <= WINKS_LISTGROUP_MAX_NUMBER(pListGroup))
        {

            pListGroup->v_FirstItemIndx = MAX((pListGroup->v_TotalItemsNum -WINKS_LISTGROUP_MAX_NUMBER(pListGroup) + 1),0);
            pListGroup->v_FocusItemIndex = pListGroup->v_TotalItemsNum;


        }
        else
        {
            // p_Screen->v_FirstItemIndx = p_Screen->v_TotalItemsNum;
            //p_Screen->v_FocusItemIndxInScrn = 1;
            pListGroup->v_FirstItemIndx = MAX((pListGroup->v_TotalItemsNum -WINKS_LISTGROUP_MAX_NUMBER(pListGroup) + 1),1);
            pListGroup->v_FocusItemIndex = WINKS_LISTGROUP_MAX_NUMBER(pListGroup);


        }		

        ret = WK_UIFM_FOCUSCHANGE;

    }
#endif
    else if(pListGroup->v_FirstItemIndx > pListGroup->v_FocusItemIndex)
    {
        (pListGroup->v_FirstItemIndx)--;
        //pListGroup->v_FocusItemIndex = 0;
    }



    wk_wg_ListGroup_Draw((Winks_Widget_s *)pListGroup);
    return ret;

}


/**************************************************************

	FUNCTION NAME		: Winks_UI_Goto_Next_Page

  	PURPOSE		        : Go to Next Page.

	INPUT PARAMETERS	: winks_List_Screen_s * p_Screen
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

int Winks_UI_Goto_Next_ListPage(Winks_Widget_ListGroup_s * pListGroup)
{	
    int highlight_index;
    int page_index;
    int old_index = 0;
    int temp_focus_index = 0;



    //WINKS_UI_CONTEXT_DEF()
    //WINKS_UI_CONTEXT_INIT()

    Winks_assert(pListGroup != NULL);

#if 0
    if (WINKS_UIFMGETGLOBALVAR->winks_scroll_string.scroll_pause == 1)
    {
    winks_ui_scrolling_text_stop(WINKS_UIFMGETGLOBALVAR->winks_scroll_string);
    }
#endif

    WINKS_UIFMGETGLOBALVAR->scrooltext_flag = 0;



    old_index = pListGroup->v_FocusItemIndex;
    highlight_index = pListGroup->v_FocusItemIndex;//p_Screen->v_FirstItemIndx + p_Screen->v_FocusItemIndxInScrn -1;
    page_index = (highlight_index - 1) / WINKS_LISTGROUP_MAX_NUMBER(pListGroup);
    if( page_index != (pListGroup->v_TotalItemsNum -1)/ WINKS_LISTGROUP_MAX_NUMBER(pListGroup))
    {
        pListGroup->v_FirstItemIndx = (page_index +1) * (WINKS_LISTGROUP_MAX_NUMBER(pListGroup)) + 1;
        temp_focus_index = pListGroup->v_TotalItemsNum - pListGroup->v_FirstItemIndx + 1;
        if (pListGroup->v_FocusItemIndex> temp_focus_index)
        {
            pListGroup->v_FocusItemIndex = temp_focus_index;
        }
    }


    wk_wg_ListGroup_Draw((Winks_Widget_s *)pListGroup);
    return WK_UIFM_SUCCESS;
}


/**************************************************************

	FUNCTION NAME		: Winks_UI_Goto_Previous_Page

  	PURPOSE		        : Go to Previous Page.

	INPUT PARAMETERS	: winks_List_Screen_s * p_Screen
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

int Winks_UI_Goto_Previous_ListPage(Winks_Widget_ListGroup_s * pListGroup)
{
    int highlight_index;
    int page_index;
    int old_index = 0;

    //WINKS_UI_CONTEXT_DEF()
    //WINKS_UI_CONTEXT_INIT()

    Winks_assert(pListGroup != NULL);

#if 0
    if (WINKS_UIFMGETGLOBALVAR->winks_scroll_string.scroll_pause == 1)
    {
        winks_ui_scrolling_text_stop(WINKS_UIFMGETGLOBALVAR->winks_scroll_string);
    }
#endif
    
    WINKS_UIFMGETGLOBALVAR->scrooltext_flag = 0;



    old_index = pListGroup->v_FocusItemIndex;
    highlight_index = pListGroup->v_FocusItemIndex;//p_Screen->v_FirstItemIndx + p_Screen->v_FocusItemIndxInScrn -1;
    page_index = (highlight_index - 1) / WINKS_LISTGROUP_MAX_NUMBER(pListGroup);
    
    if(page_index != 0)
    {
        pListGroup->v_FirstItemIndx = (page_index - 1) * WINKS_LISTGROUP_MAX_NUMBER(pListGroup) + 1;
    }
    
    //if (WINKS_UIFMGETGLOBALVAR->gv_ListScreen->v_TotalItemsNum > WINKS_LISTGROUP_MAX_NUMBER(pListGroup))
    {
        //WINKS_UIFMGETGLOBALVAR->gv_scrollbar.up_button.flags = WINKS_UI_BUTTON_STATE_DOWN;
    }
    
    wk_wg_ListGroup_Draw((Winks_Widget_s *)pListGroup);
    return WK_UIFM_SUCCESS;
}

static int if_pos_in_ListGroup_area( Winks_Widget_ListGroup_s*pListGroup,int x,int y)
{

    //Winks_gui_create_fixed_text_menuitem();
    //Winks_gui_show_fixed_text_menuitem();
    int icon_x = 0;
    int icon_y = 0;
    int width = 0;
    int height = 0;


    int index_in_tal = 0;
    int v_old_index = 0;

    int i = 0;
    int temp = 0;
    int row_number, column_number;
    int mainmenuitem_height,mainmenuitem_weight;
    int x_gap = 0, y_gap = 0;
    Winks_Widget_Listitem_s *p_temp = NULL;
    Winks_Widget_Listitem_s * pListItem_temp = NULL;
    winks_pen_point_struct Pen_Pos;


    //WINKS_UI_CONTEXT_DEF()
    ////WINKS_UI_CONTEXT_INIT();
    Pen_Pos.Pen_x = x;
    Pen_Pos.Pen_y = y;



    for(i = 0 ,pListItem_temp = pListGroup->pa_ItemListData  ;i< pListGroup->v_FirstItemIndx;pListItem_temp = pListItem_temp->pnext,i++ )
    {

    }
    for (index_in_tal = pListGroup->v_FirstItemIndx,p_temp = pListItem_temp;(index_in_tal < pListGroup->v_TotalItemsNum);
                index_in_tal++, p_temp = p_temp->pnext)

    {

        if (if_pos_in_area(Pen_Pos, p_temp->bwidget.lux, p_temp->bwidget.luy, p_temp->bwidget.width, p_temp->bwidget.high ) == TRUE)
        {
            v_old_index =  pListGroup->v_FocusItemIndex;
            pListGroup->v_FocusItemIndex = index_in_tal;

            //break;

            if(v_old_index != pListGroup->v_FocusItemIndex)	
            {
                //winks_ui_PopUpmenu(p_PopUp, v_old_index, p_PopUp->v_FocusMatrixItemIndxInScrn);
                wk_wg_ListGroup_Draw((Winks_Widget_s *)pListGroup);
                return TRUE;

            }
        }
    }



    return FALSE;
}

static int change_index_in_slide_scoorbar_move(Winks_Widget_ListGroup_s*p_ListGroup, winks_pen_point_struct Pen_Pos)
{
    winks_vertical_scrollbar_s*   p_scrollbar = NULL;

    int displacement = 0;
    int one_rate = 0;
    int change_index = 0;

    //WINKS_UI_CONTEXT_DEF()
    ////WINKS_UI_CONTEXT_INIT();

    Winks_printf("WINKS_MSG_UI_PEN_EVENT:Pen_pos(x,y)  (%d , %d)\n", Pen_Pos.Pen_x,Pen_Pos.Pen_y);

    displacement = Pen_Pos.Pen_y - WINKS_UIFMGETGLOBALVAR->prior_Pen_Pos.Pen_y;

    p_scrollbar = &(WINKS_UIFMGETGLOBALVAR->gv_scrollbar);

    one_rate = p_scrollbar->scroll_button.height / p_scrollbar->scale;
    change_index = displacement / one_rate;
    WINKS_UIFMGETGLOBALVAR->prior_Pen_Pos.Pen_x = Pen_Pos.Pen_x;
    WINKS_UIFMGETGLOBALVAR->prior_Pen_Pos.Pen_y = Pen_Pos.Pen_y;
    if (change_index < 0)
    {

        p_ListGroup->v_FirstItemIndx = MAX((p_ListGroup->v_FocusItemIndex + change_index), 1);
        p_ListGroup->v_FocusItemIndex = MAX((p_ListGroup->v_FocusItemIndex + change_index), 1);
        return TRUE;
    }
    else if (change_index > 0)
    {

        p_ListGroup->v_FirstItemIndx = MIN((p_ListGroup->v_FirstItemIndx + change_index), ((p_ListGroup->v_TotalItemsNum > WINKS_LIST_MAX_NUMBER)?\
        									(p_ListGroup->v_TotalItemsNum - WINKS_LISTGROUP_MAX_NUMBER(p_ListGroup) + 1):p_ListGroup->v_TotalItemsNum));
        p_ListGroup->v_FocusItemIndex = MIN((p_ListGroup->v_FocusItemIndex + change_index), WINKS_LISTGROUP_MAX_NUMBER(p_ListGroup));
        return TRUE;
    }
    wk_wg_ListGroup_Draw((Winks_Widget_s *)p_ListGroup);
    return TRUE;

}
static int if_pos_in_list_scroobar_spare_bg_area(Winks_Widget_ListGroup_s*p_ListGroup,winks_pen_point_struct Pen_Pos)
{


    int ret = 0;
    winks_vertical_scrollbar_s*   p_scrollbar = NULL;

    //WINKS_UI_CONTEXT_DEF()
    ////WINKS_UI_CONTEXT_INIT();

    p_scrollbar = &(WINKS_UIFMGETGLOBALVAR->gv_scrollbar);

    if (Pen_Pos.Pen_y < p_scrollbar->scroll_button.y)
    {
        p_ListGroup->v_FirstItemIndx --;
        if ((p_ListGroup->v_FocusItemIndex - p_ListGroup->v_FirstItemIndx + 1) != WINKS_LISTGROUP_MAX_NUMBER(p_ListGroup))
        {
            p_ListGroup->v_FocusItemIndex++;
        }
        ret =  TRUE;
    }
    if (Pen_Pos.Pen_y > (p_scrollbar->scroll_button.y + p_scrollbar->scroll_button.height))
    {
        p_ListGroup->v_FirstItemIndx ++;
        if (p_ListGroup->v_FocusItemIndex != 1)
        {
            p_ListGroup->v_FocusItemIndex--;
        }
        ret =  TRUE;
    }
    wk_wg_ListGroup_Draw((Winks_Widget_s *)p_ListGroup);
    return ret;

}
int wk_wg_ListGroup_PenHandler( Winks_Widget_s * p_ListGroup, int PenType, int x, int y )
{
    Winks_Widget_ListGroup_s* pListGroup = (Winks_Widget_ListGroup_s*)p_ListGroup;
    winks_pen_point_struct Pen_Pos;
    Pen_Pos.Pen_x = x;
    Pen_Pos.Pen_y = y;

    switch( PenType )
    {
        case WINKS_PEN_FLAG_PENDOWN:


        case WINKS_PEN_FLAG_PEN_LONG_PRESS:
        case WINKS_PEN_FLAG_PEN_ALWAYS_PRESS :
        {
            if(if_pos_in_scroolbar_area(Pen_Pos))
            {
                if_pos_in_list_scroobar_spare_bg_area(pListGroup,Pen_Pos);
                //pListGroup->bwidget.flag = WK_WG_FGPENSTATE;

            }
            else
            {
                if_pos_in_ListGroup_area(pListGroup, x, y);
            }


        }break;
        case WINKS_PEN_FLAG_PENMOVE :
        {
            if(if_pos_in_slide_scoorbar_area(Pen_Pos))
            {
                change_index_in_slide_scoorbar_move(pListGroup,Pen_Pos) ;
                if_pos_in_list_scroobar_spare_bg_area(pListGroup,Pen_Pos) ;
                pListGroup->bwidget.flag = WK_WG_FGPENSTATE;
            }
            if (if_pos_in_ListGroup_area(pListGroup, x, y) )
            {
                pListGroup->bwidget.flag = WK_WG_FGPENSTATE;
            }


        }break;
        case WINKS_PEN_FLAG_PENUP:

        default:
        {

        }break;
    }
    return 0;
}

int wk_wg_ListGroup_KeyHandler( Winks_Widget_s * p_ListGroup, int KeyCode, int KeyType )
{
    int ret = 0;
    Winks_Widget_ListGroup_s * pListGroup = (Winks_Widget_ListGroup_s * )p_ListGroup;
    switch(KeyCode)
    {
        case WINKS_KEY_UP:
        	{
        		if (KeyType == WINKS_KEY_FLAG_KEYDOWN) ret = Winks_UI_Goto_Previous_ListItem(pListGroup);
        	}break;
        case WINKS_KEY_DOWN:
        	{
        		if (KeyType == WINKS_KEY_FLAG_KEYDOWN)  ret = Winks_UI_Goto_Next_ListItem(pListGroup);
        	}break;
        case WINKS_KEY_LEFT:
        	{
        		if (KeyType == WINKS_KEY_FLAG_KEYDOWN)  ret = Winks_UI_Goto_Previous_ListPage(pListGroup);
        	}break;
        case WINKS_KEY_RIGHT:
        	{
        		if (KeyType == WINKS_KEY_FLAG_KEYDOWN)   ret = Winks_UI_Goto_Next_ListPage(pListGroup);
        	}
        break;
       case WINKS_KEY_OK:
    	{
            Winks_Widget_Listitem_s * pListItem_temp = NULL;
            int i = 0;
            if(KeyType == WINKS_KEY_FLAG_KEYDOWN)
            {

                if (pListGroup->pa_ItemListData->v_ListType == WK_LIST_TYPE_CHECKBOX ||
                    pListGroup->pa_ItemListData->v_ListType == WK_LIST_TYPE_RADIO)
                {      
                    for(i = 0 ,pListItem_temp = pListGroup->pa_ItemListData  ;i< pListGroup->v_FocusItemIndex;pListItem_temp = pListItem_temp->pnext,i++ )
                    {

                    };
                    if (pListItem_temp->v_ItemSetFlag)
                        pListItem_temp->v_ItemSetFlag = FALSE;
                    else
                        pListItem_temp->v_ItemSetFlag = TRUE;
                }
                 wk_wg_ListGroup_Draw((Winks_Widget_s *)p_ListGroup);
            }
        
    	}
        break;
        default:
        	break;
    		
    		
    }

    return ret;
}

int wk_wg_ListGroup_StateModify( Winks_Widget_s * p_ListGroup, int Option, int ifvar )
{
    Winks_Widget_ListGroup_s * pListGroup = (Winks_Widget_ListGroup_s * )p_ListGroup;

	if (Option == WK_WG_FGGRAY) 
	{
		if (ifvar == 1)
		{
			pListGroup->bwidget.flag |= WK_WG_FGGRAY;
		}
		else
		{
			pListGroup->bwidget.flag &= ~WK_WG_FGGRAY;
		}

	}
	if (Option == WK_WG_FGFOCUS) 
	{
		if (ifvar == 1)
		{
			pListGroup->bwidget.flag |= WK_WG_FGFOCUS;
		}
		else
		{
			pListGroup->bwidget.flag &= ~WK_WG_FGFOCUS;
		}
	}

    wk_wg_ListGroup_Draw((Winks_Widget_s * )pListGroup);
    return 0;
}
static int wk_wg_TapTitleItem_Init(Winks_Widget_TapListItem_s*  p_TapTitleItem)
{

	Winks_Widget_TapListItem_s * pTapTitleItem = (Winks_Widget_TapListItem_s *)p_TapTitleItem;
	//pTapTitleItem->bgimage = 0;
	pTapTitleItem->bwidget.font.size = WINKS_MEDIUM_FONT;
	pTapTitleItem->bwidget.font.style = WINKS_FONT_STANDARD;
	pTapTitleItem->bwidget.font.type =WK_WG_FONTSONG;
	pTapTitleItem->bwidget.font.way =WK_WG_FONTNOSCROLL;
	pTapTitleItem->bwidget.font.color =WINKS_PIXEL_WHITE;
	pTapTitleItem->bwidget.flag |= WK_WG_FGGETFOCUS;
	pTapTitleItem->bwidget.bgimage = WINKS_IMGID_TAP_CURTAPBKG;
	pTapTitleItem->bwidget.hlimage = WINKS_IMGID_TAP_CURTAPHL;
	return 0;
	
}
int wk_wg_TapTitleItem_Draw( Winks_Widget_s* p_TapTitleItem, int nTitleFlag)
{
	int x1 = 0;
	int y1 = 0;
	int x2 = 0;
	int y2 = 0;
	int icon_x = 0 ;
	int icon_y = 0;
	int hliicon_x = 0 ;
	int hliicon_y = 0;
	int icon = 0;
	int x =0;
	int y = 0;
	char *p_temp = NULL;
	int stlen = 0;
	int temp_width = 0;

    int w = 0, h = 0;

    int x_gap = 4, y_gap = 4;
    int excursion = 0;
	Winks_Widget_Font_s textFont;
	
	Winks_Widget_TapListItem_s * pTapTitleItem = (Winks_Widget_TapListItem_s *)p_TapTitleItem;

	//int flag = pTapTitleItem->bwidget.flag & WK_WG_FGGRAY;
	x1 = pTapTitleItem->bwidget.lux;
	y1 = pTapTitleItem->bwidget.luy;
	x2 = x1 + pTapTitleItem->bwidget.width;
	y2 = y1 + pTapTitleItem->bwidget.high;

	WINKS_CLIP_ON(x1,y1, x2, y2);
	//WINKS_GRAY_BEGIN(flag,x1,y1, x2, y2) ;

	textFont = pTapTitleItem->bwidget.font;
	
	//if(pTapTitleItem->bwidget.bgimage != 0)
	//{
	//  	     #ifdef DEBUG_WIDGET
	//	winks_ui_fillRect(x1, y1, x2, y2, ~pTapTitleItem->bwidget.font.color);
	//    #endif
 //  		winks_ui_drawGraphicFromFile(x1, y1 ,Winks_UIFM_GetImage(pTapTitleItem->bwidget.bgimage));

	//}
	//else
	//{
	//	winks_ui_fillRect(x1, y1, x2, y2, ~pTapTitleItem->bwidget.font.color);
	//}
	if((pTapTitleItem->bwidget.flag & WK_WG_FGFOCUS) == WK_WG_FGFOCUS)
	{
		if(pTapTitleItem->bwidget.hlimage != 0)
		{
// #ifdef DEBUG_WIDGET
// 			winks_ui_drawRect(x1, y1, x2, y2, TapListItem_hl_color);
// #endif
			if ((nTitleFlag & WK_WG_FGFOCUS) == WK_WG_FGFOCUS)
			{
				textFont.color = ~textFont.color;
				Winks_set_Widget_Font(&textFont);
				winks_ui_drawGraphicFromFile(x1 + hliicon_x , y1 + hliicon_y,Winks_UIFM_GetImage(pTapTitleItem->bwidget.hlimage));
			}
			else
			{
				Winks_set_Widget_Font(&pTapTitleItem->bwidget.font);
				winks_ui_drawGraphicFromFile(x1 + hliicon_x , y1 + hliicon_y,Winks_UIFM_GetImage(pTapTitleItem->bwidget.bgimage));
			}
		}
		else
		{
			winks_ui_fillRect(x1, y1, x2, y2, ~pTapTitleItem->bwidget.font.color);
		}
		if(pTapTitleItem->v_IconID != 0)
		{
		    winks_ui_drawGraphicFromFile(x1 + hliicon_x+ x_gap , y1 + hliicon_y + y_gap,Winks_UIFM_GetImage(pTapTitleItem->v_IconID ));
		}
		if(WINKS_IFISID(pTapTitleItem->v_string))
		{
			
			p_temp = (char *)Winks_UIFM_GetString(pTapTitleItem->v_string);
		}
		else
		{
			p_temp = (char *)pTapTitleItem->v_string;
			
		}
		winks_get_string_wight_high((char *)p_temp,winks_strlen_ucs2_type(p_temp),&w,&h);
		y = y1 + ((pTapTitleItem->bwidget.high - h) >> 1);
		x = x1 + ((pTapTitleItem->bwidget.width - w) >> 1);
		stlen = winks_strlen_ucs2_type(p_temp);
		temp_width = pTapTitleItem->bwidget.width- x_gap - TAPTITLE_ICON_WIDTH;
		if(temp_width > w)
		{
			x = x1 + x_gap +TAPTITLE_ICON_WIDTH + ((pTapTitleItem->bwidget.width- x_gap - TAPTITLE_ICON_WIDTH - w) >> 1);
		}
		else
		{
			x = x1 + x_gap +TAPTITLE_ICON_WIDTH + 1;
		}
		winks_ui_textOutlen(x,y,p_temp,stlen);
	}
	else
	{
		if(pTapTitleItem->v_IconID != 0)
		{
			winks_ui_drawGraphicFromFile(x1 + hliicon_x , y1 + hliicon_y,Winks_UIFM_GetImage(pTapTitleItem->v_IconID ));
		}
	}

	//WINKS_GRAY_BEGIN(flag,x1,y1, x2, y2) ;
	WINKS_CLIP_OFF(x1,y1, x2, y2);
	return 0;
}
int wk_wg_TapTitle_Init(Winks_Widget_s*  p_TapTitle)
{
	Winks_Widget_TapTitle_s * pTapTitle = (Winks_Widget_TapTitle_s *)p_TapTitle;
	int i = 0;
	//pTapTitle->bgimage = 0;
	pTapTitle->bwidget.font.size = WINKS_MEDIUM_FONT;
	pTapTitle->bwidget.font.style = WINKS_FONT_STANDARD;
	pTapTitle->bwidget.font.type =WK_WG_FONTSONG;
	pTapTitle->bwidget.font.way =WK_WG_FONTNOSCROLL;
	pTapTitle->bwidget.font.color =WINKS_PIXEL_WHITE;
	pTapTitle->bwidget.flag |= WK_WG_FGGETFOCUS;
	pTapTitle->bwidget.bgimage = 0;
	pTapTitle->bwidget.hlimage = 0;

	for(i = 0;i < pTapTitle->v_TotalItemsNum; i++)
	{
		wk_wg_TapTitleItem_Init(&(pTapTitle->a_ItemListData[i]));
	}
	return 0;

}
int wk_wg_TapTitle_Draw( Winks_Widget_s* p_TapTitle)
{
	int x1 = 0;
	int y1 = 0;
	int x2 = 0;
	int y2 = 0;
	int icon_x = 0 ;
	int icon_y = 0;
	int hliicon_x = 0 ;
	int hliicon_y = 0;
	int icon = 0;
	int stlen = 0;
	char * pc_temp = NULL;
	int w = 0, h = 0;
	int row_number, column_number;
	int mainmenuitem_height,mainmenuitem_weight;
	int x_gap = 0, y_gap = 0;
	int excursion = 0;
	int index = 0;
	
	Winks_Widget_TapTitle_s * pTapTitle = (Winks_Widget_TapTitle_s *)p_TapTitle;
	int flag = pTapTitle->bwidget.flag;
	x1 = pTapTitle->bwidget.lux;
	y1 = pTapTitle->bwidget.luy;
	x2 = x1 + pTapTitle->bwidget.width;
	y2 = y1 + pTapTitle->bwidget.high; 

	WINKS_CLIP_ON(x1,y1, x2, y2);
	//WINKS_GRAY_BEGIN(flag,x1,y1, x2, y2) ;
	
	Winks_set_Widget_Font(&pTapTitle->bwidget.font);	
	
	if(pTapTitle->bwidget.bgimage != 0)
	{
	   	
	     #ifdef DEBUG_WIDGET
		winks_ui_fillRect(x1, y1, x2, y2, TapTitle_bg_color);
	    #endif
	    winks_ui_drawGraphicFromFile(x1 + icon_x , y1 + icon_y,Winks_UIFM_GetImage(pTapTitle->bwidget.bgimage));


	}
	else
	{
		winks_ui_fillRect(x1, y1, x2, y2, ~pTapTitle->bwidget.font.color);
	}
	row_number = pTapTitle->v_TotalItemsNum;
	//mainmenuitem_weight= (pTapTitle->bwidget.width - HILITAPTITLE_WIDTH - ( WINKS_MAINMENU_FIRSTICON_X << 1)) / (row_number - 1);
	mainmenuitem_weight= (pTapTitle->bwidget.width - HILITAPTITLE_WIDTH) / (row_number - 1);
	mainmenuitem_height = TAPTITLE_ICON_HEIGHT;
	// mainmenuitem_weight = (WINKS_CLIENT_WIDTH - (WINKS_FIRST_MATRIXMAINMENU_X - ((WINKS_MAINMENU_BACKGROUND_WIDTH - WINKS_MAINMENU_ICON_WIDTH) >> 1))) / row_number;
	



	for (index = 0; index < pTapTitle->v_TotalItemsNum; index++) 
	{
		if (index ==  pTapTitle->v_FocusItemIndex) 
		{
			pTapTitle->a_ItemListData[index].bwidget.width = HILITAPTITLE_WIDTH;
			pTapTitle->a_ItemListData[index].bwidget.high = HILITAPTITLE_HEIGHT;
			pTapTitle->a_ItemListData[index].bwidget.flag = WK_WG_FGFOCUS;

			pTapTitle->a_ItemListData[index].bwidget.lux = x1 + index * mainmenuitem_weight;
			pTapTitle->a_ItemListData[index].bwidget.luy = y1 + ((pTapTitle->bwidget.high - HILITAPTITLE_HEIGHT ) >> 1);
			
		}
		else
		{
			pTapTitle->a_ItemListData[index].bwidget.width = mainmenuitem_weight;
			pTapTitle->a_ItemListData[index].bwidget.high = TAPTITLE_ICON_HEIGHT;
			pTapTitle->a_ItemListData[index].bwidget.flag = 0;

			if (index > pTapTitle->v_FocusItemIndex)
			{
				pTapTitle->a_ItemListData[index].bwidget.lux = x1 + HILITAPTITLE_WIDTH + (index - 1) * mainmenuitem_weight;
				pTapTitle->a_ItemListData[index].bwidget.luy = y1 + ((pTapTitle->bwidget.high - TAPTITLE_ICON_HEIGHT ) >> 1);
			}
			else
			{
				pTapTitle->a_ItemListData[index].bwidget.lux = x1 + index * mainmenuitem_weight;
				pTapTitle->a_ItemListData[index].bwidget.luy = y1 + ((pTapTitle->bwidget.high - TAPTITLE_ICON_HEIGHT ) >> 1);
			}
		}
// 		pTapTitle->a_ItemListData[index].bwidget.lux = ((index) % row_number ) * mainmenuitem_weight+ WINKS_FIRST_MATRIXMAINMENU_X;
// 		pTapTitle->a_ItemListData[index].bwidget.luy = y1 + ((pTapTitle->bwidget.high - TAPTITLE_ICON_HEIGHT ) >> 1);
// 	
// 		pTapTitle->a_ItemListData[index].bwidget.high = mainmenuitem_height;
		wk_wg_TapTitleItem_Draw((Winks_Widget_s * )&pTapTitle->a_ItemListData[index], flag);
	}
                      
	//WINKS_GRAY_BEGIN(flag,x1,y1, x2, y2) ;
	WINKS_CLIP_OFF(x1,y1, x2, y2);
	  return 0;
}

void Winks_UI_TapTitle_SetFocus( Winks_Widget_TapTitle_s* pTapTitle, int nFocusIndex)
{
	if (!pTapTitle)
		return;
	pTapTitle->v_FocusItemIndex = nFocusIndex;
	if (nFocusIndex > pTapTitle->v_TotalItemsNum - 1)
	{
		pTapTitle->v_FocusItemIndex = pTapTitle->v_TotalItemsNum - 1;
	}
	else if (pTapTitle->v_FocusItemIndex < 0)
	{
		pTapTitle->v_FocusItemIndex = 0;
	}
}
void  Winks_UI_TapTitle_Previous( Winks_Widget_TapTitle_s* pTapTitle)
{
	
    int oldicon = pTapTitle->v_FocusItemIndex;
    if(pTapTitle->v_FocusItemIndex == 0)
    {
        pTapTitle->v_FocusItemIndex = pTapTitle->v_TotalItemsNum - 1;
    }
    else
    {
        pTapTitle->v_FocusItemIndex--;
    }


    wk_wg_TapTitle_Draw((Winks_Widget_s * )pTapTitle);
	Winks_UIFM_EnterNewScreen(pTapTitle->a_ItemListData[pTapTitle->v_FocusItemIndex].v_ScreenID, (void *)WK_ENTERSCREENFLAG_TAPTITLE);

}

void  Winks_UI_TapTitle_Next( Winks_Widget_TapTitle_s* pTapTitle)
{
    int oldicon = pTapTitle->v_FocusItemIndex;
	

    Winks_assert(pTapTitle != NULL);

    if(pTapTitle->v_FocusItemIndex == pTapTitle->v_TotalItemsNum - 1)
    {
        pTapTitle->v_FocusItemIndex = 0;
    }
    else
    {
        pTapTitle->v_FocusItemIndex++;
    }

    wk_wg_TapTitle_Draw((Winks_Widget_s * )pTapTitle);
	Winks_UIFM_EnterNewScreen(pTapTitle->a_ItemListData[pTapTitle->v_FocusItemIndex].v_ScreenID, (void *)WK_ENTERSCREENFLAG_TAPTITLE);
   
}
static int if_pos_in_taptitle_area( Winks_Widget_TapTitle_s *p_TapTitle,int x,int y)
{

	//Winks_gui_create_fixed_text_menuitem();
	//Winks_gui_show_fixed_text_menuitem();
	int icon_x = 0;
	int icon_y = 0;
	int width = 0;
	int height = 0;

	int index_in_screen = 0;
	int index_in_tal = 0;
	int v_old_index = 0;


	Winks_Widget_TapListItem_s * p_temp;
	winks_pen_point_struct Pen_Pos;


	//WINKS_UI_CONTEXT_DEF()
	////WINKS_UI_CONTEXT_INIT();
	Pen_Pos.Pen_x = x;
	Pen_Pos.Pen_y = y;
	



	for (index_in_screen = 0,index_in_tal = 0;
	        //(index_in_screen <= WINKS_MATRIXMAINMENU_MAX_NUMBER) &&
	        (index_in_tal <= p_TapTitle->v_TotalItemsNum);
	        index_in_screen++, index_in_tal++ )

	{
		p_temp = &p_TapTitle->a_ItemListData[index_in_screen];
		if (if_pos_in_area(Pen_Pos, p_temp->bwidget.lux, p_temp->bwidget.luy, p_temp->bwidget.width, p_temp->bwidget.high ) == TRUE)
		{
			v_old_index =  p_TapTitle->v_FocusItemIndex;
			p_TapTitle->v_FocusItemIndex = index_in_screen;
			
			//break;

			if(v_old_index != p_TapTitle->v_FocusItemIndex)	
			{
				//winks_ui_PopUpmenu(p_PopUp, v_old_index, p_PopUp->v_FocusMatrixItemIndxInScrn);
				wk_wg_TapTitle_Draw((Winks_Widget_s * )p_TapTitle);
				return TRUE;
			
			}    
		}
	}



	return FALSE;

}
int wk_wg_TapTitle_PenHandler( Winks_Widget_s* p_TapTitle, int PenType, int x, int y )
{
	Winks_Widget_TapTitle_s* pTapTitle = (Winks_Widget_TapTitle_s*)p_TapTitle;
	winks_pen_point_struct Pen_Pos;
	Pen_Pos.Pen_x = x;
	Pen_Pos.Pen_x = y;

	switch( PenType )
	{
		case WINKS_PEN_FLAG_PENDOWN:

		
		case WINKS_PEN_FLAG_PEN_LONG_PRESS:
		case WINKS_PEN_FLAG_PEN_ALWAYS_PRESS :
		{
			
			if_pos_in_taptitle_area(pTapTitle, x, y);
		}break;
		case WINKS_PEN_FLAG_PENMOVE :
		{

			if (if_pos_in_taptitle_area(pTapTitle, x, y) == TRUE)
			{
				//pTapTitle->bwidget.flag = WK_WG_FGPENSTATE;
			}
			

		}break;
		case WINKS_PEN_FLAG_PENUP:

		default:
		{

		}break;
	}
	 return 0;
}

int wk_wg_TapTitle_KeyHandler( Winks_Widget_s* p_TapTitle, int KeyCode, int KeyType )
{
	int ret = 0;
	Winks_Widget_TapTitle_s * pTapTitle = (Winks_Widget_TapTitle_s *)p_TapTitle;
	switch(KeyCode)
	{
		case WINKS_KEY_UP:
			{
				if (KeyType == WINKS_KEY_FLAG_KEYDOWN) ret = WK_UIFM_FOCUSCHANGE;//Winks_UI_TapTitle_Previous_Column(pTapTitle);
			}break;
		case WINKS_KEY_DOWN:
			{
				if (KeyType == WINKS_KEY_FLAG_KEYDOWN)  ret = WK_UIFM_FOCUSCHANGE;//Winks_UI_TapTitle_Next_Column(pTapTitle);
				}break;
		case WINKS_KEY_LEFT:
			{
				if (KeyType == WINKS_KEY_FLAG_KEYDOWN) ret = WK_UIFM_SUCCESS;Winks_UI_TapTitle_Previous(pTapTitle);
			}break;
		case WINKS_KEY_RIGHT:
			{
				if (KeyType == WINKS_KEY_FLAG_KEYDOWN)  ret = WK_UIFM_SUCCESS;Winks_UI_TapTitle_Next(pTapTitle);
			}break;
		default:
			{
			}
	                                       

	}
	return ret;
}

int wk_wg_TapTitle_StateModify( Winks_Widget_s* p_TapTitle, int Option, int ifvar )
{
	Winks_Widget_TapTitle_s * pTapTitle = (Winks_Widget_TapTitle_s *)p_TapTitle;

	if (Option == WK_WG_FGGRAY) 
	{
		if (ifvar == 1)
		{
			pTapTitle->bwidget.flag |= WK_WG_FGGRAY;
		}
		else
		{
			pTapTitle->bwidget.flag &= ~WK_WG_FGGRAY;
		}

	}
	if (Option == WK_WG_FGFOCUS) 
	{
		if (ifvar == 1) 
		{
			pTapTitle->bwidget.flag |= WK_WG_FGFOCUS;
		}
		else
		{
			pTapTitle->bwidget.flag &= ~WK_WG_FGFOCUS;
		}

	}
	wk_wg_TapTitle_Draw((Winks_Widget_s * )pTapTitle);
	return 0;
}
int wk_wg_PopUpItem_Init(Winks_Widget_PopMenuItem_s*  p_PopUpItem)
{
	p_PopUpItem->bwidget.font.size = WINKS_MEDIUM_FONT;
	p_PopUpItem->bwidget.font.style = WINKS_FONT_STANDARD;
	p_PopUpItem->bwidget.font.type = WK_WG_FONTSONG;
	p_PopUpItem->bwidget.font.way = WK_WG_FONTNOSCROLL;
	p_PopUpItem->bwidget.font.color = WINKS_PIXEL_WHITE;
	//p_PopUpItem->bwidget.flag |= WK_WG_FGGETFOCUS;
	p_PopUpItem->bwidget.bgimage = WINKS_IMGID_POPUPMENU_ITEM;
	p_PopUpItem->bwidget.hlimage = WINKS_IMGID_POPUPMENU_ITEMHL;
	return 0;
}

int wk_wg_PopUpItem_Draw(Winks_Widget_PopMenuItem_s* p_PopUpItem)
{
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;

    int stlen = 0;
    char * pc_temp = NULL;
    int w = 0, h = 0;

    Winks_Widget_PopMenuItem_s * p =
            (Winks_Widget_PopMenuItem_s *) p_PopUpItem;
    int flag = p->bwidget.flag;
    
    if(flag & WK_WG_FGNOTSHOW)
        return 0;
    
    x1 = p->bwidget.lux;
    y1 = p->bwidget.luy;
    x2 = x1 + p->bwidget.width;
    y2 = y1 + p->bwidget.high;

    WINKS_CLIP_ON(x1, y1, x2, y2);
    //WINKS_GRAY_BEGIN(flag, x1, y1, x2, y2);

    Winks_set_Widget_Font(&p->bwidget.font);

    //winks_ui_fillRect(x1, y1, x2, y2, PopUpItem_bg_color);

    if (p->bwidget.bgimage != 0)
    {
        winks_ui_drawGraphicFromFile(x1, y1, Winks_UIFM_GetImage(
                p->bwidget.bgimage));
    }

    if (p->bwidget.flag & WK_WG_FGFOCUS)
    {
        //winks_ui_fillRect(x1, y1, x2, y2, PopUpItem_hl_color);
        if (p->bwidget.hlimage != 0)
        {
            winks_ui_drawGraphicFromFile(x1, y1, Winks_UIFM_GetImage(
                    p->bwidget.hlimage));
        }

        if (p->v_IconID != 0)
        {
            winks_ui_drawGraphicFromFile(x1, y1, Winks_UIFM_GetImage(
                    p->v_IconID));
        }
    }

    //画文本
    if ( p->v_StringID && WINKS_IFISID(p->v_StringID))
    {
        pc_temp = (char *) Winks_UIFM_GetString(p->v_StringID);
    }
    else
    {
        pc_temp = (char *) p->v_StringID;
    }

    stlen = winks_strlen_ucs2_type(pc_temp);
    winks_get_string_wight_high(pc_temp, stlen, &w, &h);

    winks_ui_textOutlen(x1, y1+(p->bwidget.high-h)/2, pc_temp, stlen);

    //WINKS_GRAY_END(flag, x1, y1, x2, y2);
    WINKS_CLIP_OFF(x1, y1, x2, y2);
    return 0;
}

int wk_wg_PopMenu_Init(Winks_Widget_s*  p_PopUpMenu)
{
	Winks_Widget_PopMenu_s * p = (Winks_Widget_PopMenu_s * )p_PopUpMenu;
	int i = 0;

	p->bwidget.font.size = WINKS_MEDIUM_FONT;
	p->bwidget.font.style = WINKS_FONT_STANDARD;
	p->bwidget.font.type = WK_WG_FONTSONG;
	p->bwidget.font.way = WK_WG_FONTNOSCROLL;
	p->bwidget.font.color =WINKS_PIXEL_WHITE;
	p->bwidget.flag |= WK_WG_FGGETFOCUS;
	p->bwidget.bgimage = 0;
	p->bwidget.hlimage = 0;
	
	if(p->v_TotalItemsNum)
	{
        Winks_Widget_s *prewg = NULL;
        Winks_Widget_s *wg = NULL;
        int w = 0;
        int h = 0;
        int index = 0;

        w = p->bwidget.width;
        h = p->bwidget.high;
        
        for (i = 0; i < p->v_TotalItemsNum ; i++)
        {
            wg = &p->a_ItemList[i].bwidget;
            wg->xindex = 0;
            wg->yindex = i;
            //把所有ITEM用链表串起来
            if(prewg)
                prewg->pnext = wg;
            prewg = wg;
        }
        
        for (i = p->v_TotalItemsNum - 1; i >= 0 ; i--)
        {
            wg = &p->a_ItemList[i].bwidget;
            wk_wg_PopUpItem_Init(&(p->a_ItemList[i]));
            
            if(wg->flag & WK_WG_FGNOTSHOW)
            {
                //不显示
                wg->flag &= ~WK_WG_FGGETFOCUS;
                continue;
            }
            else
            {
                wg->flag |= WK_WG_FGGETFOCUS;
                wg->lux = p->bwidget.lux;
                wg->luy = p->bwidget.luy - (index + 1) * h;
                wg->width = w;
                wg->high = h;
                if(i == p->v_FocusItemIndex)
                    wg->flag |= WK_WG_FGFOCUS;
                else
                    wg->flag &= ~WK_WG_FGFOCUS;
                
                index ++;
            }
        }
	}

	return 0;
}

int wk_wg_PopMenu_Draw(Winks_Widget_s*p_PopUpMenu)
{
	int x1 = 0;
	int y1 = 0;
	int x2 = 0;
	int y2 = 0;
	int i = 0;
	
	Winks_Widget_PopMenu_s * pum = (Winks_Widget_PopMenu_s * )p_PopUpMenu;
    //int flag = pum->bwidget.flag & WK_WG_FGGRAY;

	x1 = pum->bwidget.lux;
	y1 = pum->bwidget.luy;
	x2 = x1 + pum->bwidget.width;
	y2 = y1 + pum->bwidget.high; 

	WINKS_CLIP_ON(x1,y1, x2, y2);
	//WINKS_GRAY_BEGIN(flag, x1, y1, x2, y2) ;

	Winks_set_Widget_Font(&pum->bwidget.font);

#ifdef DEBUG_WIDGET
    winks_ui_fillRect(x1, y1, x2, y2, PopUpMenu_bg_color);
#endif
    
    if(pum->bwidget.bgimage != 0)
	{
	    winks_ui_drawGraphicFromFile(x1, y1, Winks_UIFM_GetImage(pum->bwidget.bgimage));
	}
    
    for(i=0;i<pum->v_TotalItemsNum;i++)
    {
        wk_wg_PopUpItem_Draw(&pum->a_ItemList[i]);
    }

    //WINKS_GRAY_END(flag, x1, y1, x2, y2) ;
	WINKS_CLIP_OFF(x1,y1, x2, y2);
	return 0;
}

static int if_pos_in_popmenu_area( Winks_Widget_PopMenu_s*pPopUpMenu,int x,int y)
{

	//Winks_gui_create_fixed_text_menuitem();
	//Winks_gui_show_fixed_text_menuitem();
	int icon_x = 0;
	int icon_y = 0;
	int width = 0;
	int height = 0;

	int index_in_screen = 0;
	int index_in_tal = 0;
	int v_old_index = 0;


	int temp = 0;
	int row_number, column_number;
	int mainmenuitem_height,mainmenuitem_weight;
	int x_gap = 0, y_gap = 0;
	Winks_Widget_PopMenuItem_s *p_temp;
	winks_pen_point_struct Pen_Pos;


	//WINKS_UI_CONTEXT_DEF()
	////WINKS_UI_CONTEXT_INIT();
	Pen_Pos.Pen_x = x;
	Pen_Pos.Pen_y = y;
	



	for (index_in_screen = 0,index_in_tal = 0;
	        //(index_in_screen <= WINKS_MATRIXMAINMENU_MAX_NUMBER) &&
	        (index_in_tal <= pPopUpMenu->v_TotalItemsNum);
	        index_in_screen++, index_in_tal++ )

	{
		p_temp = &pPopUpMenu->a_ItemList[index_in_screen];
		if (if_pos_in_area(Pen_Pos, p_temp->bwidget.lux, p_temp->bwidget.luy, p_temp->bwidget.width, p_temp->bwidget.high ) == TRUE)
		{
			v_old_index =  pPopUpMenu->v_FocusItemIndex;
			pPopUpMenu->v_FocusItemIndex = index_in_screen;
			
			//break;

			if(v_old_index != pPopUpMenu->v_FocusItemIndex)	
			{
				//winks_ui_PopUpmenu(p_PopUp, v_old_index, p_PopUp->v_FocusMatrixItemIndxInScrn);
				wk_wg_PopMenu_Draw((Winks_Widget_s*)pPopUpMenu);
				return TRUE;
			
}
		}
	}



	return FALSE;

}
int wk_wg_PopMenu_PenHandler( Winks_Widget_s*p_PopUpMenu , int PenType, int x, int y )
{
	Winks_Widget_PopMenu_s* pPopUpMenu = (Winks_Widget_PopMenu_s*)p_PopUpMenu;
	winks_pen_point_struct Pen_Pos;
	Pen_Pos.Pen_x = x;
	Pen_Pos.Pen_x = y;

	switch( PenType )
	{
		case WINKS_PEN_FLAG_PENDOWN:

		
		case WINKS_PEN_FLAG_PEN_LONG_PRESS:
		case WINKS_PEN_FLAG_PEN_ALWAYS_PRESS :
		{
			
			if_pos_in_popmenu_area(pPopUpMenu, x, y);
		}break;
		case WINKS_PEN_FLAG_PENMOVE :
		{

			if (if_pos_in_popmenu_area(pPopUpMenu, x, y) == TRUE)
			{
				pPopUpMenu->bwidget.flag = WK_WG_FGFOCUS;//WK_WG_FGPENSTATE;WK_WG_FGPENSTATE;
			}
			

		}break;
		case WINKS_PEN_FLAG_PENUP:

		default:
		{

		}break;
	}
	 return 0;
}

int wk_wg_PopMenu_KeyHandler( Winks_Widget_s*p_PopUpMenu, int KeyCode, int KeyType )
{
    Winks_Widget_PopMenu_s* p = (Winks_Widget_PopMenu_s*)p_PopUpMenu;
    {
        Winks_Widget_s** ppLostFocus = NULL;
        Winks_Widget_s** ppGetFocus = NULL;
        
        Winks_Widget_s* pWidget = NULL;
        Winks_Widget_s* pTmpWidget = NULL;
        int ret = 0, wdnum = 0, fline = 0, fcolumn = 0;
        Winks_Widget_s* WidgetMatrix[WK_WGAPI_MAXMATRIXX][WK_WGAPI_MAXMATRIXY];

        if( (pWidget = Winks_Widget_GetFocusWidget( &p->a_ItemList[0].bwidget )) == NULL )
        {
            Winks_printf( "Winks Widget can not find focus widget" );
            return -1;
        }

        if( (KeyCode != WINKS_KEY_UP) && (KeyCode != WINKS_KEY_DOWN)
            && (KeyCode != WINKS_KEY_LEFT) && (KeyCode != WINKS_KEY_RIGHT) )
        {
            Winks_fail( "Winks Widget Navkey handler recv key not navkey." );
            return -1;
        }

        if( KeyType == WINKS_KEY_FLAG_KEYDOWN )
        {
            /* 焦点需要发生变化 */
            Winks_mem_set( WidgetMatrix, 0, sizeof(Winks_Widget_s*) * WK_WGAPI_MAXMATRIXX * WK_WGAPI_MAXMATRIXY );
            pTmpWidget = &p->a_ItemList[0].bwidget;
            while( pTmpWidget )
            {
                if( pTmpWidget->flag & WK_WG_FGGETFOCUS )
                {
                    Winks_assert( pTmpWidget->xindex < WK_WGAPI_MAXMATRIXX );
                    Winks_assert( pTmpWidget->yindex < WK_WGAPI_MAXMATRIXY );
                    WidgetMatrix[pTmpWidget->yindex][pTmpWidget->xindex] = pTmpWidget;
                    wdnum ++;
                }

                pTmpWidget = pTmpWidget->pnext;
            }
            if( wdnum <= 1 )
            {
                /* 只有一个可获得焦点的控件，直接返回 */
                if( ppLostFocus )
                    *ppLostFocus = NULL;
                if( ppGetFocus )
                    *ppGetFocus = pWidget;
                return ret;
            }
            
            switch( KeyCode )
            {
            case WINKS_KEY_UP:
                if( (fline = winks_widget_findupline( (Winks_Widget_s** )WidgetMatrix, pWidget->yindex )) != pWidget->yindex )
                {
                    if( (fcolumn = winks_widget_findbeforecolumn( (Winks_Widget_s** )WidgetMatrix, fline, pWidget->xindex )) < 0 )
                        fcolumn = winks_widget_findaftercolumn( (Winks_Widget_s** )WidgetMatrix, fline, pWidget->xindex );
                    pTmpWidget = WidgetMatrix[fline][fcolumn];
                }
                else
                {
                    /* 只有一行 */
                    pTmpWidget = pWidget;
                    pWidget = NULL;
                }
                break;

            case WINKS_KEY_DOWN:
                if( (fline = winks_widget_finddownline( (Winks_Widget_s** )WidgetMatrix, pWidget->yindex )) != pWidget->yindex )
                {
                    if( (fcolumn = winks_widget_findbeforecolumn( (Winks_Widget_s** )WidgetMatrix, fline, pWidget->xindex )) < 0 )
                        fcolumn = winks_widget_findaftercolumn( (Winks_Widget_s** )WidgetMatrix, fline, pWidget->xindex );
                    pTmpWidget = WidgetMatrix[fline][fcolumn];
                }
                else
                {
                    /* 只有一行 */
                    pTmpWidget = pWidget;
                    pWidget = NULL;
                }
                break;

            case WINKS_KEY_LEFT:
                fcolumn = winks_widget_findbeforecolumn( (Winks_Widget_s** )WidgetMatrix, pWidget->yindex, pWidget->xindex - 1 );
                if(  fcolumn < 0 )
                {
                    fline = winks_widget_findupline( (Winks_Widget_s** )WidgetMatrix, pWidget->yindex );
                    fcolumn = winks_widget_findbeforecolumn( (Winks_Widget_s** )WidgetMatrix, fline, WK_WGAPI_MAXMATRIXX - 1 );
                    pTmpWidget = WidgetMatrix[fline][fcolumn];
                }
                else
                {
                    pTmpWidget = WidgetMatrix[pWidget->yindex][fcolumn];
                }
                break;

            case WINKS_KEY_RIGHT:
                fcolumn = winks_widget_findaftercolumn( (Winks_Widget_s** )WidgetMatrix, pWidget->yindex, pWidget->xindex + 1 );
                if( fcolumn < 0 )
                {
                    fline = winks_widget_finddownline( (Winks_Widget_s** )WidgetMatrix, pWidget->yindex );
                    fcolumn = winks_widget_findaftercolumn( (Winks_Widget_s** )WidgetMatrix, fline, 0 );
                    pTmpWidget = WidgetMatrix[fline][fcolumn];
                }
                else
                {
                    pTmpWidget = WidgetMatrix[pWidget->yindex][fcolumn];
                }
                break;
            }
        }
        else
        {
            pTmpWidget = pWidget;
            pWidget = NULL;
        }

        if( pWidget )
        {
            Winks_Widget_PopMenuItem_s* prev = &p->a_ItemList[0];
            p->v_FocusItemIndex = 0;
            while(prev)
            {
                if(pTmpWidget==&prev->bwidget)
                    break;
                p->v_FocusItemIndex ++;
                prev = &p->a_ItemList[p->v_FocusItemIndex];
            }
            /* 如果有新控件获得了焦点，则请求绘制 */
            pWidget->flag &= ~WK_WG_FGFOCUS;
            pTmpWidget->flag |= WK_WG_FGFOCUS;
            wk_wg_PopUpItem_Draw((Winks_Widget_PopMenuItem_s *)pWidget);
            wk_wg_PopUpItem_Draw((Winks_Widget_PopMenuItem_s *)pTmpWidget);
        }

        if( ppLostFocus )
            *ppLostFocus = pWidget;
        if( ppGetFocus )
            *ppGetFocus = pTmpWidget;

        return ret;
    }
    return 0;
}

int wk_wg_PopMenu_StateModify( Winks_Widget_s*p_PopUpMenu, int Option, int ifvar )
{
	Winks_Widget_PopMenu_s * pPopUpMenu = (Winks_Widget_PopMenu_s * )p_PopUpMenu;
	
	if (Option == WK_WG_FGGRAY) 
	{
		if (ifvar == 1) 
		    pPopUpMenu->bwidget.flag =  WK_WG_FGGRAY;//|=  WK_WG_FGGRAY;
		else
		{
			//if (ifvar == 1) 
	    			pPopUpMenu->bwidget.flag = 0;//&=  ~WK_WG_FGGRAY;
		}
			
	}
	if (Option == WK_WG_FGFOCUS) 
	{
		if (ifvar == 1) 
		    pPopUpMenu->bwidget.flag =  WK_WG_FGFOCUS;//|=  WK_WG_FGFOCUS;
		else
		{
			//if (ifvar == 1) 
	    			pPopUpMenu->bwidget.flag = 0;//pPopUpMenu->bwidget.flag &=  ~WK_WG_FGFOCUS;
		}
			
	}
	wk_wg_PopMenu_Draw(p_PopUpMenu);
	 return 0;
}
static int wk_wg_SoftKey_Init(Winks_Widget_SoftKey_s*  pSoftKey)
{


    //pSoftKey->bgimage = 0;
    pSoftKey->bwidget.font.size = WINKS_MEDIUM_FONT;
    pSoftKey->bwidget.font.style = WINKS_FONT_STANDARD;
    pSoftKey->bwidget.font.type =WK_WG_FONTSONG;
    pSoftKey->bwidget.font.way =WK_WG_FONTNOSCROLL;
    pSoftKey->bwidget.font.color =WINKS_PIXEL_WHITE;
    pSoftKey->bwidget.flag &= ~WK_WG_FGGETFOCUS;
    pSoftKey->bwidget.bgimage = 0;
    pSoftKey->bwidget.hlimage = 0;
    return 0;




}
int winks_ui_SoftKey_Draw(Winks_Widget_SoftKey_s* pSoftKey)
{

    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    int icon_x = 0 ;
    int icon_y = 0;
    int hliicon_x = 0 ;
    int hliicon_y = 0;
    int icon = 0;
    int stlen = 0;
    char * p_temp = NULL;
    int w = 0, h = 0;
    int x = 0;
    int y = 0;

    int x_gap = 0, y_gap = 0;
    int excursion = 0;
    //int flag = pSoftKey->bwidget.flag & WK_WG_FGGRAY;

    if( pSoftKey->v_StringID == 0 )
        return 0;

    x1 = pSoftKey->bwidget.lux;
    y1 = pSoftKey->bwidget.luy;
    x2 = x1 + pSoftKey->bwidget.width;
    y2 = y1 + pSoftKey->bwidget.high; 

    WINKS_CLIP_ON(x1,y1, x2, y2);
    //WINKS_GRAY_BEGIN(flag,x1,y1, x2, y2);

    Winks_set_Widget_Font(&pSoftKey->bwidget.font);

    icon = pSoftKey->bwidget.bgimage;
    if(icon!= 0)
    {
#ifdef DEBUG_WIDGET
        winks_ui_fillRect(x1, y1, x2, y2, SoftKey_bg_color);
#endif
        winks_ui_drawGraphicFromFile(x1  , y1 ,Winks_UIFM_GetImage(icon));


    }
    else
    {
        winks_ui_fillRect(x1, y1, x2, y2, ~pSoftKey->bwidget.font.color);
    }
    if(pSoftKey->bwidget.flag & WK_WG_FGFOCUS)
    {
        if(pSoftKey->bwidget.hlimage != 0)
        {
#ifdef DEBUG_WIDGET
            winks_ui_fillRect(x1 +2, y1 +2, x2 - 2, y2 - 2, SoftKey_hl_color);
#endif
            winks_ui_drawGraphicFromFile(x1 + hliicon_x , y1 + hliicon_y,Winks_UIFM_GetImage(pSoftKey->bwidget.hlimage));
        }
    }
    if (pSoftKey->bwidget.flag & WK_WG_FGPUSH)
    {
        icon = pSoftKey->normal_down_icon;
        //pbutton->flags = WINKS_UI_BUTTON_STATE_UP;
#ifdef DEBUG_WIDGET
        winks_ui_fillRect(x1 +2, y1 +2, x2 - 2, y2 - 2, SoftKey_down_color);
#endif
        pSoftKey->bwidget.flag &= ~WK_WG_FGPUSH;
    }
    else
    {
        icon = pSoftKey->normal_up_icon;
    }

    if(icon != 0)
    {
#ifdef DEBUG_WIDGET
        winks_ui_fillRect(x1 +2, y1 +2, x2 - 2, y2 - 2, SoftKey_hl_color);
#endif
        winks_ui_drawGraphicFromFile(x1 + icon_x , y1 + icon_y,Winks_UIFM_GetImage(icon));
    }
    if(WINKS_IFISID(pSoftKey->v_StringID))
    {

        p_temp = (char *)Winks_UIFM_GetString(pSoftKey->v_StringID);
    }
    else
    {
        p_temp = (char *)pSoftKey->v_StringID;

    }
    //Winks_showtext_in_area (p_temp,(char *)winks_strlen_ucs2_type(p_temp),x1,y1, x2, y2);
    winks_get_string_wight_high(p_temp,winks_strlen_ucs2_type(p_temp),&w,&h);



    if (pSoftKey->bwidget.width > w) 
        x  = x1 + ((pSoftKey->bwidget.width - w) >> 1);
    y = y1 + ((pSoftKey->bwidget.high - WINKS_GETFONTHEIGHT) >> 1);

    winks_ui_textOutlen(x,y,p_temp,winks_strlen_ucs2_type(p_temp));


    //WINKS_GRAY_BEGIN(flag,x1,y1, x2, y2);
    WINKS_CLIP_OFF(x1,y1, x2, y2);
    return 0;
}

int wk_wg_LeftSoftKey_Init(Winks_Widget_s*  p_LeftSoftKey)
{

    wk_wg_SoftKey_Init((Winks_Widget_SoftKey_s*)p_LeftSoftKey);
    return 0;

}
int wk_wg_LeftSoftKey_Draw(Winks_Widget_s* p_LeftSoftKey)
{
    int w, h;
    int stlen = 0;
    Winks_Widget_SoftKey_s* pLeftSoftKey = (Winks_Widget_SoftKey_s*)p_LeftSoftKey;
    //char ptmp[32];
    //WINKS_UI_CONTEXT_DEF()
    //WINKS_UI_CONTEXT_INIT()
    //stlen = winks_strlen_ucs2_type((char *)Winks_UIFM_GetString(pLeftSoftKey->v_StringID));
    //gb2312tounicode( ptmp, 32, (char *)Winks_UIFM_GetString(stringid), stlen );
    // winks_get_string_wight_high( (char * )(char *)Winks_UIFM_GetString(pLeftSoftKey->v_StringID), stlen, &w, &h );
    //winks_get_string_wight_high((char *)Winks_UIFM_GetString(stringid),strlen((char *)Winks_UIFM_GetString(stringid)),&w,&h);
    //Winks_printf( "winks_ui_CreateRightSoftKey w %d, h %d\r\n", w, h );
    //    pLeftSoftKey->bwidget.lux = 0;
    //	  pLeftSoftKey->bwidget.luy = (WINKS_SCREEN_HEIGHT -  WINKS_SOFTKEY_HEIGHT);
    //	    pLeftSoftKey->bwidget.width = WINKS_SOFTKEY_WIDTH;
    //		  pLeftSoftKey->bwidget.high = WINKS_SOFTKEYBAR_HEIGHT;


    winks_ui_SoftKey_Draw(pLeftSoftKey);	
    return 0;

}
int wk_wg_RightSoftKey_Init(Winks_Widget_s*  p_RightSoftKey)
{

    wk_wg_SoftKey_Init((Winks_Widget_SoftKey_s*)p_RightSoftKey);

    return 0;
}
int wk_wg_RightSoftKey_Draw(Winks_Widget_s*  p_RightSoftKey)
{

    Winks_Widget_SoftKey_s* pRightSoftKey = (Winks_Widget_SoftKey_s*)p_RightSoftKey;
    //	pRightSoftKey->bwidget.lux = (WINKS_SCREEN_WIDTH -  WINKS_SOFTKEY_WIDTH);
    //	pRightSoftKey->bwidget.luy = (WINKS_SCREEN_HEIGHT -  WINKS_SOFTKEY_HEIGHT);
    //	pRightSoftKey->bwidget.width = WINKS_SOFTKEYBAR_WIDTH;
    //	pRightSoftKey->bwidget.high = WINKS_SOFTKEYBAR_HEIGHT;
    // pLeftSoftKey->winks_right_softkey.v_string_x = WINKS_SOFTKEYBAR_WIDTH - w - WINKS_LEFTSOFTKEY_STRING_X ;
    //pLeftSoftKey->winks_right_softkey.v_string_y = WINKS_COLORCALL_SOFTKEY_STRING_Y;
    winks_ui_SoftKey_Draw(pRightSoftKey);	
    return 0;
}


int wk_wg_SoftKey_PenHandler(Winks_Widget_s * p_SoftKey, int PenType, int x, int y )
{

    Winks_Widget_SoftKey_s* pSoftKey = (Winks_Widget_SoftKey_s*)p_SoftKey;
    switch( PenType )
    {
        case WINKS_PEN_FLAG_PENDOWN:
        {
            pSoftKey->bwidget.flag |= WK_WG_FGPUSH;
            winks_ui_SoftKey_Draw( pSoftKey );
        }
        break;
        case WINKS_PEN_FLAG_PENUP:
        {
            pSoftKey->bwidget.flag &= ~WK_WG_FGPUSH;
            winks_ui_SoftKey_Draw( pSoftKey );
        }
        break;


        default:
        {

        }
        //error message;
        break;
    }
    return 0;
}



int wk_wg_LeftSoftKey_PenHandler(Winks_Widget_s * p_LeftSoftKey, int PenType, int x, int y )
{
    return wk_wg_SoftKey_PenHandler(p_LeftSoftKey,PenType,  x, y);
}
int wk_wg_RightSoftKey_PenHandler(Winks_Widget_s * p_RightSoftKey, int PenType, int x, int y )
{
    return wk_wg_SoftKey_PenHandler(p_RightSoftKey,PenType,  x, y);
}  

int wk_wg_LeftSoftKey_KeyHandler( Winks_Widget_s* p_SoftKey, int KeyCode, int KeyType )
{
    int ret = 0;
    Winks_Widget_SoftKey_s* pSoftKey = (Winks_Widget_SoftKey_s*)p_SoftKey;

    switch( KeyCode )
    {
        //case WINKS_KEY_OK:

        case WINKS_KEY_LSK:
                                            {
                                                if (KeyType == WINKS_KEY_FLAG_KEYDOWN) 
                                                {
                                                    pSoftKey->bwidget.flag |= WK_WG_FGPUSH;
                                                    winks_ui_SoftKey_Draw( pSoftKey );
                                                }
                                                if (KeyType == WINKS_KEY_FLAG_KEYDOWN) 
                                                {
                                                    pSoftKey->bwidget.flag &= ~WK_WG_FGPUSH;
                                                    winks_ui_SoftKey_Draw( pSoftKey );
                                                }
                                            }break;
        case WINKS_KEY_UP:
        case WINKS_KEY_DOWN:
        case WINKS_KEY_LEFT:
        case WINKS_KEY_RIGHT:
                                            {
                                                //if (KeyType == WINKS_KEY_FLAG_KEYDOWN)
                                                ret = WK_UIFM_FOCUSCHANGE;//Winks_UI_TapTitle_Previous_Column(pTapTitle);
                                            }break;



        default:
        {

        }
        //error message;
        break;


    }
    return ret;
}
int wk_wg_RightSoftKey_KeyHandler( Winks_Widget_s* p_RightSoftKey, int KeyCode, int KeyType )
{
    int ret = 0;
    Winks_Widget_SoftKey_s* pRightSoftKey = (Winks_Widget_SoftKey_s*)p_RightSoftKey;

    switch( KeyCode )
    {


    	case WINKS_KEY_RSK:
    						{
    							if (KeyType == WINKS_KEY_FLAG_KEYDOWN) 
    							{
    								pRightSoftKey->bwidget.flag |= WK_WG_FGPUSH;
    								winks_ui_SoftKey_Draw( pRightSoftKey );
    							}
    							if (KeyType == WINKS_KEY_FLAG_KEYDOWN) 
    							{
    								pRightSoftKey->bwidget.flag &= ~WK_WG_FGPUSH;
    								winks_ui_SoftKey_Draw( pRightSoftKey );
    							}
    						}break;
    	case WINKS_KEY_UP:
    	case WINKS_KEY_DOWN:
    	case WINKS_KEY_LEFT:
    	case WINKS_KEY_RIGHT:
    						{
    							//if (KeyType == WINKS_KEY_FLAG_KEYDOWN)
    								ret = WK_UIFM_FOCUSCHANGE;//Winks_UI_TapTitle_Previous_Column(pTapTitle);
    						}break;
    					


    	default:
    	{
    		
    	}
    	//error message;
    	break;
                                           

    }
    return ret;
}
int wk_wg_SoftKey_StateModify( Winks_Widget_SoftKey_s* pSoftKey, int Option, int ifvar )
{
    if (Option == WK_WG_FGGRAY) 
    {
    	if (ifvar == 1) 
    	    pSoftKey->bwidget.flag |=  WK_WG_FGGRAY;
    	else
    	{
            //if (ifvar == 1) 
            pSoftKey->bwidget.flag &=  ~WK_WG_FGGRAY;
    	}
    		
    }
    if (Option == WK_WG_FGFOCUS) 
    {
    	if (ifvar == 1) 
    	    pSoftKey->bwidget.flag |=  WK_WG_FGFOCUS;
    	else
    	{
            //if (ifvar == 1) 
            pSoftKey->bwidget.flag  &=  ~WK_WG_FGFOCUS;
    	}
    		
    }
    winks_ui_SoftKey_Draw(pSoftKey);
    return 0;
}
int wk_wg_CenterSoftKey_Init(Winks_Widget_s*  p_CenterSoftKey)
{

    wk_wg_SoftKey_Init((Winks_Widget_SoftKey_s*)p_CenterSoftKey);
    return 0;
	
}
int wk_wg_CenterSoftKey_Draw(Winks_Widget_s* p_CenterSoftKey)
{
    return winks_ui_SoftKey_Draw((Winks_Widget_SoftKey_s*)p_CenterSoftKey);
}

int wk_wg_CenterSoftKey_PenHandler( Winks_Widget_s* p_CenterSoftKey, int PenType, int x, int y )
{
    return 0;
}

int wk_wg_CenterSoftKey_KeyHandler( Winks_Widget_s* p_CenterSoftKey, int KeyCode, int KeyType )
{
    return 0;
}

int wk_wg_CenterSoftKey_StateModify( Winks_Widget_s* p_CenterSoftKey, int Option, int ifvar )
{
    return wk_wg_SoftKey_StateModify((Winks_Widget_SoftKey_s*)p_CenterSoftKey,Option,  ifvar);
}
int wk_wg_LeftSoftKey_StateModify(  Winks_Widget_s*  p_LeftSoftKey, int Option, int ifvar )
{
    return wk_wg_SoftKey_StateModify((Winks_Widget_SoftKey_s*)p_LeftSoftKey,Option,  ifvar);
}
int wk_wg_RightSoftKey_StateModify(  Winks_Widget_s*  p_RightSoftKey, int Option, int ifvar )
{
    return wk_wg_SoftKey_StateModify((Winks_Widget_SoftKey_s*)p_RightSoftKey,Option,  ifvar);
}
winks_show_multiline_text(Winks_Widget_Static_s*p_Static)
{
    winks_Rect_s Rect;
    //winks_vertical_scrollbar_s scrollbar;
    //WINKS_SHOW_FLAG TextAlign = 0;
    int TextAlign;
    int linenumber = 0;
    char * p_temp = NULL;

    //WINKS_UI_CONTEXT_DEF()
    //WINKS_UI_CONTEXT_INIT()

    TextAlign = p_Static->d_flag;
    Rect.v_Bottom = p_Static->bwidget.luy + p_Static->bwidget.high;
    Rect.v_Left = p_Static->bwidget.lux;
    Rect.v_Right = p_Static->bwidget.lux + p_Static->bwidget.width;
    Rect.v_Top = p_Static->bwidget.luy;
    //winks_ui_drawGraphicFromFile(WINKS_INFOBOX_X ,WINKS_INFOBOX_Y , Winks_UIFM_GetImage(WINKS_INFOBOX_BGICON));//(p_infobox->v_bgicon)
    //WINKS_SET_TEXTCOLOR(WINKS_PIXEL_LIGHTWHITE);
    //winks_clear_screen_fillRect(Rect.v_Left,Rect.v_Top,Rect.v_Right, Rect.v_Bottom);
    //winks_ui_fillRect(Rect.v_Left,Rect.v_Top,Rect.v_Right, Rect.v_Bottom,WINKS_PIXEL_RED);

    //WINKS_SET_TEXTCOLOR(WINKS_PIXEL_BLACK);
    //winks_ui_DrawstringInRect((char *)Winks_UIFM_GetString(p_infobox->v_InfoboxStringId), &Rect, TextAlign, strlen((char *)Winks_UIFM_GetString(p_infobox->v_InfoboxStringId)));
    //winks_ui_fillRect(WINKS_CLIENT_X,WINKS_CLIENT_Y,WINKS_CLIENT_X + WINKS_CLIENT_WIDTH, WINKS_CLIENT_Y + WINKS_CLIENT_HEIGHT, WINKS_PIXEL_LIGHTWHITE );

    if(WINKS_IFISID(p_Static->v_ContentID))
    {
        p_temp = (char *)Winks_UIFM_GetString(p_Static->v_ContentID);
    }
    else
    {
        p_temp = (char *)p_Static->v_ContentID;
    }

    if (p_Static->rect_text_String.v_Stringline_talNum == 0)
    {
        winks_creat_text_string_rect_type(&p_Static->rect_text_String, &Rect, TextAlign, p_temp, winks_strlen_ucs2_type(p_temp));

    }
    p_Static->rect_text_String.first_inshow_text = p_temp;
    if(p_Static->rect_text_String.v_Stringline_talNum >= p_Static->rect_text_String.line_number_area)
           Rect.v_Right = Rect.v_Right - WINKS_SCROLLBAR_WIDTH -1;  
    winks_show_string_in_rect(&p_Static->rect_text_String, &Rect, TextAlign);

    if(p_Static->rect_text_String.v_Stringline_talNum >= p_Static->rect_text_String.line_number_area)
    {
        Rect.v_Right = p_Static->bwidget.lux + p_Static->bwidget.width;
        Rect.v_Left = Rect.v_Right - WINKS_SLIDESCROLLBAR_WIDTH + 1;

        winks_ui_create_vertical_scrollbar(&(WINKS_UIFMGETGLOBALVAR->gv_scrollbar), p_Static->rect_text_String.v_FirstlineIndx , p_Static->rect_text_String.v_Stringline_talNum , p_Static->rect_text_String.line_number_area,Rect);
        winks_ui_drawVerticalScrollBar(&(WINKS_UIFMGETGLOBALVAR->gv_scrollbar));
    }

}
int wk_wg_PopUp_Init(Winks_Widget_s*  p_PopUp)
{

	Winks_Widget_PopUp_s* pPopUp = (Winks_Widget_PopUp_s*)p_PopUp;
	//pPopUp->bgimage = 0;
	pPopUp->bwidget.font.size = WINKS_MEDIUM_FONT;
	pPopUp->bwidget.font.style = WINKS_FONT_STANDARD;
	pPopUp->bwidget.font.type =WK_WG_FONTSONG;
	pPopUp->bwidget.font.way =WK_WG_FONTNOSCROLL;
	pPopUp->bwidget.font.color =WINKS_PIXEL_BLACK;
	pPopUp->bwidget.flag &= ~WK_WG_FGGETFOCUS;
	pPopUp->bwidget.bgimage = WINKS_IMGID_POPUPDLG_BKG;
	pPopUp->bwidget.hlimage = 0;
	return 0;
}

int wk_wg_PopUp_Draw(Winks_Widget_s*p_PopUp )
{
	int x1 = 0;
	int y1 = 0;
	int x2 = 0;
	int y2 = 0;
	int icon_x = 0 ;
	int icon_y = 0;
	int hliicon_x = 0 ;
	int hliicon_y = 0;
	int icon = 0;
	int w = 0;
	int h = 0;
	int stlen = 0; 

	int title_x = 0;
	int title_y = 0;

	Winks_Widget_PopUp_s* pPopUp = (Winks_Widget_PopUp_s*)p_PopUp;
	//int flag = pPopUp->bwidget.flag & WK_WG_FGGRAY;

	char *  p_temp = NULL;

	x1 = pPopUp->bwidget.lux;
	y1 = pPopUp->bwidget.luy;
	x2 = x1 + pPopUp->bwidget.width;
	y2 = y1 + pPopUp->bwidget.high;

	WINKS_CLIP_ON(x1,y1, x2, y2);
	//WINKS_GRAY_BEGIN(flag,x1,y1, x2, y2);

	Winks_set_Widget_Font(&pPopUp->bwidget.font);

	icon = pPopUp->bwidget.bgimage;

	if(icon!= 0)
	{
		winks_ui_drawGraphicFromFile(x1, y1, Winks_UIFM_GetImage(icon));
	}
	else
	{
		winks_ui_fillRect(x1, y1, x2, y2, ~pPopUp->bwidget.font.color);
	}

	//if(p_PopUp->flag == WK_DIALOG_TYPE_SCREEN)
// 	{
// 
// 		if(WINKS_IFISID(pPopUp->v_TitleStringID))
// 		{
// 			p_temp = (char *)Winks_UIFM_GetString(pPopUp->v_TitleStringID);
// 		}
// 		else
// 		{
// 			p_temp = (char *)pPopUp->v_TitleStringID;
// 		}
// 		stlen = winks_strlen_ucs2_type(p_temp);
// 		//gb2312tounicode( ptmp, 32, (char *)Winks_UIFM_GetString(stringid), stlen );
// 		winks_get_string_wight_high( (char *)(p_temp), stlen, &w, &h );
// 		title_x = x1 + ((pPopUp->bwidget.width-DIALOG_SHOW_AREA_BLANKWIDTH - w) >> 1);
// 		title_y = y1 + DIALOG_TITLE_EXCURSION_Y;
// 		winks_ui_textOutlen(title_x,title_y,p_temp,stlen);
// 
// 	}
// 
// 	if(p_PopUp->flag == WK_DIALOG_TYPE_NOMAL)
	{

		if(WINKS_IFISID(pPopUp->v_UpStringID))
		{
			p_temp = (char *)Winks_UIFM_GetString(pPopUp->v_UpStringID);
		}
		else
		{
			p_temp = (char *)pPopUp->v_UpStringID;
		}
		stlen = winks_strlen_ucs2_type(p_temp);

		if(pPopUp->v_Icon == 0)
		{
			title_x = x1 + DIALOG_SHOW_AREA_BLANKWIDTH;
			w = pPopUp->bwidget.width - 2 * DIALOG_SHOW_AREA_BLANKWIDTH;
		}
		else
		{
			title_x = x1 + DIALOG_SHOW_PIC_WIDTH + DIALOG_SHOW_AREA_BLANKWIDTH;
			w = pPopUp->bwidget.width - 2 * DIALOG_SHOW_AREA_BLANKWIDTH - DIALOG_SHOW_PIC_WIDTH;
		}
		title_y = y1;
		h = pPopUp->bwidget.high;
		//winks_ui_textOutlen(title_x, title_y, p_temp, stlen);
		Winks_showtext_in_area1 (p_temp, stlen, title_x, title_y, w, h);
	}
	if(pPopUp->v_Icon != 0)
	{
		winks_ui_drawGraphicFromFile(x1 + DIALOG_SHOW_PIC_HBLANK, y1 + DIALOG_SHOW_PIC_VBLANK, Winks_UIFM_GetImage(pPopUp->v_Icon));
	}
// 	else if(WINKS_IFISID(pPopUp->v_Icon))
// 	{
// 		p_temp = (char *)Winks_UIFM_GetImage(pPopUp->v_Icon);
// 	}
// 	else if(WINKS_IFISID(pPopUp->v_DownStringID))
// 	{
// 		p_temp = (char *)pPopUp->v_DownStringID;
// 	}

	//WINKS_GRAY_BEGIN(flag,x1,y1, x2, y2);
	WINKS_CLIP_OFF(x1,y1, x2, y2);
	return 0;
}

int wk_wg_PopUp_PenHandler( Winks_Widget_s*p_PopUp , int PenType, int x, int y )
{
	Winks_Widget_Static_s* pPopUp = (Winks_Widget_Static_s*)p_PopUp;
	winks_pen_point_struct Pen_Pos;
	int ret = 0;
	Pen_Pos.Pen_x = x;
	Pen_Pos.Pen_x = y;

	switch( PenType )
	{
		case WINKS_PEN_FLAG_PENDOWN:

		
		case WINKS_PEN_FLAG_PEN_LONG_PRESS:
		case WINKS_PEN_FLAG_PEN_ALWAYS_PRESS :
		{
			
			//if_pos_in_popmenu_area(pPopUp, x, y);
		}break;
		case WINKS_PEN_FLAG_PENMOVE :
		{

			//if (if_pos_in_popmenu_area(pPopUp, x, y) == TRUE)
			//{
			//	pPopUp->bwidget.flag = WK_WG_FGPENSTATE;
			//}
			

		}break;
		case WINKS_PEN_FLAG_PENUP:

		default:
		{

		}break;
	}
	return ret;

}

int wk_wg_PopUp_KeyHandler( Winks_Widget_s*p_PopUp , int KeyCode, int KeyType )
{
	int ret = 0; 
	Winks_Widget_PopUp_s* pPopUp = (Winks_Widget_PopUp_s*)p_PopUp;

			switch(KeyCode)
			{
				case WINKS_KEY_UP:

				case WINKS_KEY_DOWN:
;
				case WINKS_KEY_LEFT:

				case WINKS_KEY_RIGHT:
					{
						ret = WK_UIFM_SUCCESS;
					}break;
				default:
					{
					}


			}
		
	return ret;
}

int wk_wg_PopUp_StateModify( Winks_Widget_s*p_PopUp , int Option, int ifvar )
{
	Winks_Widget_PopUp_s* pPopUp = (Winks_Widget_PopUp_s*)p_PopUp;
	
	if (Option == WK_WG_FGGRAY) 
	{
		if (ifvar == 1) 
		    pPopUp->bwidget.flag |=  WK_WG_FGGRAY;
		else
		{
			//if (ifvar == 1) 
	    			pPopUp->bwidget.flag &=  ~WK_WG_FGGRAY;
		}
			
	}
	if (Option == WK_WG_FGFOCUS) 
	{
		if (ifvar == 1) 
		    pPopUp->bwidget.flag |=  WK_WG_FGFOCUS;
		else
		{
			//if (ifvar == 1) 
	    			pPopUp->bwidget.flag &=  ~WK_WG_FGFOCUS;
		}
			
	}
	wk_wg_PopUp_Draw((Winks_Widget_s*)pPopUp);
	return 0;
}
int wk_wg_PopUp_Change( Winks_Widget_s* p_PopUp, int conflag )

{
	Winks_Widget_PopUp_s* pPopUp = (Winks_Widget_PopUp_s*)p_PopUp;
	

	wk_wg_PopUp_Draw((Winks_Widget_s*)pPopUp);
	return 0;
}


/**************************************************************

	FUNCTION NAME		: winks_show_string_in_rect

  	PURPOSE		        : show strinf in the rect.

	INPUT PARAMETERS	: winks_text_String_rect_s * rect_text_String
                       	  winks_Rect_s* pRect
                       	  char TextAlign
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/
int  winks_show_string_in_rect(winks_text_String_rect_s * rect_text_String, winks_Rect_s* pRect, unsigned long TextAlign)
{
    //char * temp;
    //		  int y = 0;
    //		  int NumLines;
    //const char  *sOrg =s;
    int FontYSize,FontCharSize;
    int  char_tap = 0;
    // int xLine = 0;
    int  string_x = pRect->v_Left;
    int  string_y = pRect->v_Top;
    int linewidth = 0;
    int lineheight = 0;
    // int LineLen = 0;
    // int NumCharsRem;           /* Number of remaining characters */
    int num_in_line = 0;
    // 	  int remain_number = 0;
    int linenumber = 0;
    int  height = pRect->v_Bottom -pRect->v_Top + 1;
    //char * text = (char  *)rect_text_String->a_linesData[0].p_string;
    //int current_textlen = 0;
    //int current_textlinelen = 0;
    //char flag;
    int i = 0;
    int temp_text_y = 0;
    int temp_text_x = 0;
    int w = 0,h = 0,strlen = 0;
    int current_textlen = 0;
    int current_textlinelen = 0;
    int string_text_lenth = winks_strlen_ucs2_type(rect_text_String->first_inshow_text);

   char * temp = rect_text_String->first_inshow_text;
   int flag = 0;

    Winks_assert(rect_text_String != NULL);
    
    if (winks_draw_condition(WINKS_MINGUI_HWND_USER) != WINKS_RETURN_SUCCESS)
    {
        return -1;
    }
     
   
     
    //winks_creat_text_string_rect_type(rect_text_String, pRect, TextAlign, string_text, string_text_lenth);
    //textlen = MaxNumChars;
    FontYSize = WINKS_GETFONTHEIGHT;
    FontCharSize = WINKS_GETFONTHEIGHT >> 1;
    lineheight= WINKS_GETFONTHEIGHT + WINKS_LINE_GAP;
    linenumber = (pRect->v_Bottom - pRect->v_Top + WINKS_LINE_GAP) / (lineheight);
    linewidth = pRect->v_Right - pRect->v_Left + 1;

    num_in_line = (linewidth+char_tap) / (FontCharSize + char_tap);
     for(i = 0; current_textlen < string_text_lenth; i++ ) 
    {
        if(i == rect_text_String ->v_FirstlineIndx)break;
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
    
   
    //if (i< linenumber) 
 	//	string_y += (height - i*lineheight) >> 1;
   //else
   	{
   		string_y += (height - linenumber*lineheight) >> 1;
   	}
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
       
	if (i < linenumber )
	{
		winks_get_string_wight_high(temp, current_textlinelen,&w,&h);
		
		if(TextAlign == WINKS_SHOW_CENTER) 
		{
			string_x = pRect->v_Left + ((linewidth - w) >> 1);
		}
		else if(TextAlign == WINKS_SHOW_RIGHT) 
		{
			string_x = pRect->v_Left + ((linewidth - w) );
		}
		winks_ui_textOutlen(string_x, string_y, temp, current_textlinelen);
	}
	
	temp = temp + current_textlinelen;
	current_textlen = current_textlen + current_textlinelen;
	string_y = string_y + lineheight;
        
    }

    
    return 0; 
}
  

int wk_wg_Static_Init(Winks_Widget_s*  p_Static)
{

    Winks_Widget_Static_s* pStatic = (Winks_Widget_Static_s*)p_Static;

    pStatic->bwidget.font.size = WINKS_MEDIUM_FONT;
    pStatic->bwidget.font.style = WINKS_FONT_STANDARD;
    pStatic->bwidget.font.type =WK_WG_FONTSONG;
    pStatic->bwidget.font.way =WK_WG_FONTNOSCROLL;
    pStatic->bwidget.font.color =WINKS_PIXEL_BLACK;
    pStatic->bwidget.flag &= ~WK_WG_FGGETFOCUS;
    pStatic->bwidget.bgimage = 0;
    pStatic->bwidget.hlimage = 0;
    pStatic->d_flag = WINKS_SHOW_LEFT;
    pStatic->bwidget.bgcolor = WINKS_PIXEL_GRAY;
    pStatic->bwidget.hlcolor = WINKS_PIXEL_GRAY;
    pStatic->bwidget.bgfontcolor = 0;
    pStatic->bwidget.hlfontcolor = 0;

    return 0;




}
int wk_wg_Static_Draw(Winks_Widget_s*p_Static)
{
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;


    int hliicon_x = 0 ;
    int hliicon_y = 0;
    int icon = 0;
    char *  p_temp = NULL;
    Winks_Widget_Static_s* pStatic = (Winks_Widget_Static_s*)p_Static;
    int icon_x = 0;
    int icon_y = 0;
    //int flag = pStatic->bwidget.flag & WK_WG_FGGRAY;

    x1 = pStatic->bwidget.lux;
    y1 = pStatic->bwidget.luy;
    x2 = x1 + pStatic->bwidget.width;
    y2 = y1 + pStatic->bwidget.high; 
    WINKS_CLIP_ON(x1,y1, x2, y2);
    //WINKS_GRAY_BEGIN(flag,x1,y1, x2, y2);
    //WINKS_SET_TEXTCOLOR(WINKS_PIXEL_LIGHTWHITE);
    Winks_set_Widget_Font(&pStatic->bwidget.font);
    if(pStatic->bwidget.bgcolor != WINKS_PIXEL_TRANSPARENT)
        winks_ui_fillRect(x1, y1, x2, y2, pStatic->bwidget.bgcolor);

    icon = pStatic->bwidget.bgimage;
    if(icon!= 0)
    {

        winks_ui_drawGraphicFromFile(x1  , y1 ,Winks_UIFM_GetImage(icon));

    }

    if(pStatic->bwidget.flag & WK_WG_FGFOCUS)
    {
        if(pStatic->bwidget.hlimage != 0)
        {
            winks_ui_drawGraphicFromFile(x1 + hliicon_x , y1 + hliicon_y,Winks_UIFM_GetImage(pStatic->bwidget.hlimage));
        }
    }

    if(pStatic->c_flag == WK_STATIC_TYPE_IMAGE)
    {

        if(WINKS_IFISID(pStatic->v_ContentID))
        {
            winks_ui_drawGraphicFromFile(x1 + icon_x , y1 + icon_y,Winks_UIFM_GetImage(pStatic->v_ContentID));
        }
        else
        {
            winks_ui_drawGraphicFromFile(x1 + icon_x , y1 + icon_y,(char *)pStatic->v_ContentID);
        }

    }
    else if(pStatic->c_flag == WK_STATIC_TYPE_ONLY_READ_TEXT)
    {
        if(WINKS_IFISID(pStatic->v_ContentID))
        {

            p_temp = (char *)Winks_UIFM_GetString(pStatic->v_ContentID);
        }
        else
        {
            p_temp = (char *)pStatic->v_ContentID;

        }
        Winks_showtext_in_area1 (p_temp,winks_strlen_ucs2_type(p_temp),x1,y1, pStatic->bwidget.width , pStatic->bwidget.high);
    }
    else if(pStatic->c_flag == WK_STATIC_TYPE_TEXT)
    {
        if( WINKS_IFISID(pStatic->v_ContentID))
        {

            p_temp = (char *)Winks_UIFM_GetString(pStatic->v_ContentID);
        }
        else
        {
            p_temp = (char *)pStatic->v_ContentID;

        }
        winks_show_multiline_text(pStatic);
    }

    //WINKS_GRAY_BEGIN(flag,x1,y1, x2, y2);
    WINKS_CLIP_OFF(x1,y1, x2, y2);
    return 0;
}
#if 1
static int if_pos_in_scroolbar_area(winks_pen_point_struct Pen_Pos)
{
    winks_vertical_scrollbar_s*   p_scrollbar = NULL;

    //WINKS_UI_CONTEXT_DEF()
    ////WINKS_UI_CONTEXT_INIT();

    p_scrollbar = &(WINKS_UIFMGETGLOBALVAR->gv_scrollbar);
    return if_pos_in_area(Pen_Pos, p_scrollbar->x,p_scrollbar->y,p_scrollbar->width,p_scrollbar->height);


}
static int if_pos_in_multyedit_scroobar_spare_bg_area(Winks_Widget_Static_s* p_Static,winks_pen_point_struct Pen_Pos)
{

    winks_vertical_scrollbar_s*   p_scrollbar = NULL;

    //WINKS_UI_CONTEXT_DEF()
    ////WINKS_UI_CONTEXT_INIT();

    p_scrollbar = &(WINKS_UIFMGETGLOBALVAR->gv_scrollbar);

    if (Pen_Pos.Pen_y < p_scrollbar->scroll_button.y)
    {

        if (p_Static->rect_text_String.v_FirstlineIndx != 0)
        {
            p_Static->rect_text_String.v_FirstlineIndx --;
        }
        return TRUE;
    }
    if (Pen_Pos.Pen_y > (p_scrollbar->scroll_button.y + p_scrollbar->scroll_button.height))
    {

        if (p_Static->rect_text_String.v_FirstlineIndx  < (p_Static->rect_text_String.v_Stringline_talNum - p_Static->rect_text_String.line_number_area  + 1))
        {
            p_Static->rect_text_String.v_FirstlineIndx ++;
        }
        return TRUE;
    }

    return FALSE;

}
static int change_index_in_multyedit_slide_scoorbar_move(Winks_Widget_Static_s*p_Static,winks_pen_point_struct Pen_Pos)
{
    winks_vertical_scrollbar_s*   p_scrollbar = NULL;

    int displacement = 0;
    int one_rate = 0;
    int change_index = 0;

    //WINKS_UI_CONTEXT_DEF()
    ////WINKS_UI_CONTEXT_INIT();

    Winks_printf("WINKS_MSG_UI_PEN_EVENT:Pen_pos(x,y)  (%d , %d)\n", Pen_Pos.Pen_x,Pen_Pos.Pen_y);


    displacement = Pen_Pos.Pen_y - WINKS_UIFMGETGLOBALVAR->prior_Pen_Pos.Pen_y;
    p_scrollbar = &(WINKS_UIFMGETGLOBALVAR->gv_scrollbar);

    one_rate = p_scrollbar->scroll_button.height / p_scrollbar->scale;
    change_index = displacement / one_rate;
    Winks_printf("WINKS_MSG_UI_PEN_EVENT:change_index  %d\n", change_index);

    if (change_index < 0)
    {
        WINKS_UIFMGETGLOBALVAR->prior_Pen_Pos.Pen_x = Pen_Pos.Pen_x;
        WINKS_UIFMGETGLOBALVAR->prior_Pen_Pos.Pen_y = Pen_Pos.Pen_y;
        p_Static->rect_text_String.v_FirstlineIndx = MAX((p_Static->rect_text_String.v_FirstlineIndx + change_index), 0);
        //WINKS_UIFMGETGLOBALVAR->gv_ListScreen->v_FocusItemIndxInScrn = MAX((WINKS_UIFMGETGLOBALVAR->gv_ListScreen->v_FocusItemIndxInScrn + change_index), 0);
        return TRUE;
    }
    else if (change_index > 0)
    {
        WINKS_UIFMGETGLOBALVAR->prior_Pen_Pos.Pen_x = Pen_Pos.Pen_x;
        WINKS_UIFMGETGLOBALVAR->prior_Pen_Pos.Pen_y = Pen_Pos.Pen_y;
        p_Static->rect_text_String.v_FirstlineIndx = MIN((p_Static->rect_text_String.v_FirstlineIndx + change_index), (p_Static->rect_text_String.v_Stringline_talNum - WINKS_MULTILINE_MAX_NUMBER + 1));
        return TRUE;
    }

    return FALSE;
}
int wk_wg_Static_PenHandler( Winks_Widget_s*p_Static, int PenType, int x, int y )
{
	Winks_Widget_Static_s* pStatic = (Winks_Widget_Static_s*)p_Static;
	winks_pen_point_struct Pen_Pos;
	Pen_Pos.Pen_x = x;
	Pen_Pos.Pen_y = y;
	if (pStatic->c_flag == WK_STATIC_TYPE_TEXT)
	{
		switch( PenType )
		{
		case WINKS_PEN_FLAG_PENDOWN:
			
			
		case WINKS_PEN_FLAG_PEN_LONG_PRESS:
		case WINKS_PEN_FLAG_PEN_ALWAYS_PRESS :
			{
				if(if_pos_in_scroolbar_area(Pen_Pos))
					if(if_pos_in_multyedit_scroobar_spare_bg_area(pStatic, Pen_Pos))
					{
						wk_wg_Static_Draw((Winks_Widget_s*)p_Static);
						pStatic->bwidget.flag |= WK_WG_FGPENSTATE;
					}
			}break;
		case WINKS_PEN_FLAG_PENMOVE :
			{
				if(if_pos_in_scroolbar_area(Pen_Pos))
				{
					if (if_pos_in_multyedit_scroobar_spare_bg_area(pStatic,Pen_Pos) == TRUE)
					{
						wk_wg_Static_Draw((Winks_Widget_s*)p_Static);
						pStatic->bwidget.flag |= WK_WG_FGPENSTATE;
					}
					if (change_index_in_multyedit_slide_scoorbar_move(pStatic, Pen_Pos) == TRUE)
					{
						wk_wg_Static_Draw((Winks_Widget_s*)p_Static);
						pStatic->bwidget.flag |= WK_WG_FGPENSTATE;
					}
				}
				pStatic->bwidget.flag |= WK_WG_FGPENSTATE;
				
			}break;
		case WINKS_PEN_FLAG_PENUP:
			
		default:
			{
				
			}break;
		}
	}
	return 0;
}
#if 1
/**************************************************************

	FUNCTION NAME		: Winks_UI_Goto_Previous_multiline_text_line

  	PURPOSE		        : Go to Previous line.

	INPUT PARAMETERS	: winks_text_String_rect_s *p_multiline_text
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void Winks_UI_Goto_Previous_multiline_text_line(Winks_Widget_Static_s*p_Statict)
{

    winks_text_String_rect_s *p_multiline_text = &(p_Statict->rect_text_String);
    Winks_assert(p_multiline_text != NULL);

    if (p_multiline_text->v_FirstlineIndx > 0 )    
    {	

        p_multiline_text->v_FirstlineIndx = p_multiline_text->v_FirstlineIndx - 1;

    }


    //winks_show_multiline_text(NULL, 0,0,p_Statict->,0,0,0);
    wk_wg_Static_Draw((Winks_Widget_s *)p_Statict);




}


/**************************************************************

	FUNCTION NAME		: Winks_UI_Goto_Next_multiline_text_line

  	PURPOSE		        : Go to Next line.

	INPUT PARAMETERS	: winks_text_String_rect_s *p_multiline_text
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void Winks_UI_Goto_Next_multiline_text_line(Winks_Widget_Static_s*p_Statict)
{
    int linenumber = 0;
    winks_text_String_rect_s *p_multiline_text = &(p_Statict->rect_text_String);


    Winks_assert(p_multiline_text != NULL);

    linenumber = p_multiline_text ->line_number_area;
    if(p_multiline_text->v_FirstlineIndx <=  (p_multiline_text->v_Stringline_talNum - linenumber ))    
    {	

        p_multiline_text->v_FirstlineIndx = p_multiline_text->v_FirstlineIndx + 1;

    }



    wk_wg_Static_Draw((Winks_Widget_s *)p_Statict);
    // winks_show_multiline_text(NULL, 0,0,0,0,0,0);

}

/**************************************************************

	FUNCTION NAME		: Winks_UI_Goto_PreviousPage_multiline_text_line

  	PURPOSE		        : Go to Previous Page.

	INPUT PARAMETERS	: winks_text_String_rect_s *p_multiline_text
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void Winks_UI_Goto_PreviousPage_multiline_text_line(Winks_Widget_Static_s*p_Statict)
{
    int linenumber = 0;
    int page_index = 0;
    int page_max = 0;
    winks_text_String_rect_s *p_multiline_text = &(p_Statict->rect_text_String);

    Winks_assert(p_multiline_text != NULL);

    linenumber = p_multiline_text ->line_number_area;
    page_index = p_multiline_text->v_FirstlineIndx / linenumber;
    page_max = p_multiline_text->v_Stringline_talNum / linenumber;

    if(page_index  >  0)    
    {	

        p_multiline_text->v_FirstlineIndx = (page_index - 1) * linenumber;

    }


    wk_wg_Static_Draw((Winks_Widget_s *)p_Statict);
    // winks_show_multiline_text(NULL, 0,0,0,0,0,0);

}

/**************************************************************

	FUNCTION NAME		: Winks_UI_Goto_NextPage_multiline_text_line

  	PURPOSE		        : Go to Next  Page.

	INPUT PARAMETERS	: winks_text_String_rect_s *p_multiline_text
	OUTPUT PARAMETERS	: null

	RETURNS			    : void

	REMARKS			    : null

***************************************************************/

void Winks_UI_Goto_NextPage_multiline_text_line(Winks_Widget_Static_s*p_Statict)
{
    int linenumber = 0;
    int page_index = 0;
    int page_max = 0;
    winks_text_String_rect_s *p_multiline_text = &(p_Statict->rect_text_String);


    Winks_assert(p_multiline_text != NULL);
    linenumber = p_multiline_text ->line_number_area;
    page_index = p_multiline_text->v_FirstlineIndx / linenumber;
    page_max = p_multiline_text->v_Stringline_talNum / linenumber;



    if ((p_multiline_text->v_Stringline_talNum + 1) % linenumber != 0 )
    {
        page_max++;
    }

    if(page_index  < page_max - 1 )    
    {	

        p_multiline_text->v_FirstlineIndx = (page_index + 1)* linenumber;

    }




    wk_wg_Static_Draw((Winks_Widget_s *)p_Statict);
    // winks_show_multiline_text(NULL, 0,0,0,0,0,0);

}
#endif

#endif
int wk_wg_Static_KeyHandler( Winks_Widget_s*p_Static, int KeyCode, int KeyType )
{
	//int ret = 
	Winks_Widget_Static_s* pStatic = (Winks_Widget_Static_s*)p_Static;
	if (pStatic->c_flag == WK_STATIC_TYPE_TEXT)
	{
		switch(KeyCode)
		{
			case WINKS_KEY_UP:
				{
					if (KeyType == WINKS_KEY_FLAG_KEYDOWN) Winks_UI_Goto_Previous_multiline_text_line(pStatic);
				}break;
			case WINKS_KEY_DOWN:
				{
					if (KeyType == WINKS_KEY_FLAG_KEYDOWN) Winks_UI_Goto_Next_multiline_text_line(pStatic);
					}break;
			case WINKS_KEY_LEFT:
				{
					if (KeyType == WINKS_KEY_FLAG_KEYDOWN) Winks_UI_Goto_PreviousPage_multiline_text_line(pStatic);
				}break;
			case WINKS_KEY_RIGHT:
				{
					if (KeyType == WINKS_KEY_FLAG_KEYDOWN) Winks_UI_Goto_NextPage_multiline_text_line(pStatic);
				}break;
			default:
				{
				}


		}
	
	}
    return 0;
}

int wk_wg_Static_StateModify( Winks_Widget_s*p_Static, int Option, int ifvar )
{
	Winks_Widget_Static_s* pStatic = (Winks_Widget_Static_s*)p_Static;
	
	if (Option == WK_WG_FGGRAY) 
	{
		if (ifvar == 1)
		{
		    pStatic->bwidget.flag |= WK_WG_FGGRAY;
		}
		else
		{
   			pStatic->bwidget.flag &= ~WK_WG_FGGRAY;
		}
			
	}
	if (Option == WK_WG_FGFOCUS) 
	{
		if (ifvar == 1)
		{
		    pStatic->bwidget.flag |= WK_WG_FGFOCUS;
		}
		else
		{
   			pStatic->bwidget.flag &= ~WK_WG_FGFOCUS;
		}
	}
	wk_wg_Static_Draw((Winks_Widget_s *)pStatic);
	return 0;
}

int wk_wg_MatrixMenuItem_Init(Winks_Widget_s*  pMatrixMenuItem)
{
    Winks_Widget_MatrixMenuItem_s *p = (Winks_Widget_MatrixMenuItem_s *)pMatrixMenuItem;
    p->bwidget.type = WK_WG_MATRIXMENUITEM;
	p->bwidget.font.size = WINKS_MEDIUM_FONT;
	p->bwidget.font.style = WINKS_FONT_STANDARD;
	p->bwidget.font.type = WK_WG_FONTSONG;
	p->bwidget.font.way = WK_WG_FONTNOSCROLL;
	p->bwidget.font.color = WINKS_PIXEL_BLACK;
	p->bwidget.flag |= WK_WG_FGGETFOCUS;
	p->bwidget.bgimage = 0;
	p->bwidget.hlimage = 0;
	p->hlIconID = WINKS_IMGID_MAINMENU_HLBKG;
	return 0;
}

int wk_wg_MatrixMenuItem_Draw(Winks_Widget_s* pMatrixMenuItem)
{
    Winks_Widget_MatrixMenuItem_s *p = (Winks_Widget_MatrixMenuItem_s *)pMatrixMenuItem;
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    int offx = 0;
    int offy = 0;
    int stlen = 0;
    char *str = NULL;
    int w = 0, h = 0;

    int x_gap = 0, y_gap = 0;
    int excursion = 0;
    //int flag = p->bwidget.flag & WK_WG_FGGRAY;

    x1 = p->bwidget.lux;
    y1 = p->bwidget.luy;
    x2 = x1 + p->bwidget.width;
    y2 = y1 + p->bwidget.high;

    WINKS_CLIP_ON(x1,y1, x2, y2);
    //WINKS_GRAY_BEGIN(flag,x1,y1, x2, y2);

    //winks_ui_fillRect(x1, y1, x2, y2, MatrixMenuItem_bg_color);

    //计算文字
    Winks_set_Widget_Font(&p->bwidget.font);
    str = (char *) Winks_UIFM_GetString(p->v_StringID);
    stlen = winks_strlen_ucs2_type(str);
    
    winks_get_string_wight_high(str, stlen, &w, &h);
    
    offx = (p->bwidget.width - WINKS_MAINMENU_ICON_WIDTH)/2;
    offy = (p->bwidget.high - WINKS_MAINMENU_ICON_HEIGHT - h)/2;

    //画背景图
    if (p->bwidget.bgimage != 0)
    {
        winks_ui_drawGraphicFromFile(x1, y1, Winks_UIFM_GetImage(
                p->bwidget.bgimage));
    }
    //画icon
    {
        if (p->nmIconID != 0)
        {
            winks_ui_drawGraphicFromFile(x1 + offx, y1 + offy,
                    Winks_UIFM_GetImage(p->nmIconID));
        }
    }
    //焦点状态，画高亮icon
    if (p->bwidget.flag & WK_WG_FGFOCUS)
    {
        if (p->hlIconID != 0)
        {
            winks_ui_drawGraphicFromFile(x1 + offx, y1 + offy,
                    Winks_UIFM_GetImage(p->hlIconID));
        }
    }
    //画文字
    offx = (p->bwidget.width - w)/2;
    offy = (p->bwidget.high - h - WINKS_MAINMENU_ICON_HEIGHT)/2;
    winks_ui_textOutlen(x1 + offx, y2 - offy, str, stlen);

    //WINKS_GRAY_BEGIN(flag,x1,y1, x2, y2);
    WINKS_CLIP_OFF(x1,y1, x2, y2);
    return 0;

}

int wk_Redraw_MatrixMenuItem(Winks_Widget_MatrixMenu_s* p_MatrixMenu,int old_index, int new_index)
{
				 Winks_set_Widget_Font(&p_MatrixMenu->bwidget.font);
	if(old_index != p_MatrixMenu->v_FocusMatrixItemIndex ) 
	{
		p_MatrixMenu->a_MenuItem[old_index].bwidget.flag &= ~WK_WG_FGFOCUS;
		wk_wg_MatrixMenuItem_Draw((Winks_Widget_s *)&p_MatrixMenu->a_MenuItem[old_index]);
		
		p_MatrixMenu->a_MenuItem[p_MatrixMenu->v_FocusMatrixItemIndex].bwidget.flag |=WK_WG_FGFOCUS;
		wk_wg_MatrixMenuItem_Draw((Winks_Widget_s *)&p_MatrixMenu->a_MenuItem[p_MatrixMenu->v_FocusMatrixItemIndex]);
	}
	 return 0;
}

int wk_wg_MatrixMenuItem_StateModify( Winks_Widget_s* p_MatrixMenuItem, int Option, int ifvar )
{
    Winks_Widget_MatrixMenuItem_s *p = (Winks_Widget_MatrixMenuItem_s *)p_MatrixMenuItem;
    
    if (Option == WK_WG_FGGRAY) 
    {
        if (ifvar == 1) 
            p->bwidget.flag =  WK_WG_FGGRAY;
        else
        {
            //if (ifvar == 1) 
                    p->bwidget.flag &=  ~WK_WG_FGGRAY;
        }
            
    }
    if (Option == WK_WG_FGFOCUS) 
    {
        if (ifvar == 1) 
            p->bwidget.flag |=  WK_WG_FGFOCUS;
        else
        {
            //if (ifvar == 1) 
                    p->bwidget.flag &=  ~WK_WG_FGFOCUS;
        }
    }
    return 0;
}

int wk_wg_MatrixMenu_Init(Winks_Widget_s*  p_MatrixMenu)
{
	Winks_Widget_MatrixMenu_s* pmm = (Winks_Widget_MatrixMenu_s*)p_MatrixMenu;

	pmm->bwidget.font.size = WINKS_MEDIUM_FONT;
	pmm->bwidget.font.style = WINKS_FONT_STANDARD;
	pmm->bwidget.font.type = WK_WG_FONTSONG;
	pmm->bwidget.font.way = WK_WG_FONTNOSCROLL;
	pmm->bwidget.font.color = WINKS_PIXEL_BLACK;
	pmm->bwidget.flag |= WK_WG_FGGETFOCUS;
	pmm->bwidget.bgimage = 0;
	pmm->bwidget.hlimage = 0;
	
	if(pmm->v_TotalMatrixItemsNum>0 && pmm->v_numberbycolumn)
	{
	    Winks_Widget_s *prewg = NULL;
        Winks_Widget_s *wg = NULL;
	    int i = 0;
	    int w = pmm->bwidget.width/pmm->v_numberbycolumn;
	    int h = pmm->bwidget.high/(pmm->v_TotalMatrixItemsNum/pmm->v_numberbycolumn 
	            + ((pmm->v_TotalMatrixItemsNum%pmm->v_numberbycolumn)>0?1:0));
        //初始化每个ITEM的位置
        for(i = 0;i < pmm->v_TotalMatrixItemsNum; i++)
        {
            wg = &pmm->a_MenuItem[i].bwidget;
            //把所有ITEM用链表串起来
            if(prewg)
                prewg->pnext = wg;
            prewg = wg;

            wk_wg_MatrixMenuItem_Init((Winks_Widget_s *)&(pmm->a_MenuItem[i]));
            wg->xindex = i%pmm->v_numberbycolumn;
            wg->yindex = i/pmm->v_numberbycolumn;
            wg->lux = pmm->bwidget.lux + w * wg->xindex;
            wg->luy = pmm->bwidget.luy + h * wg->yindex;
            wg->width = w;
            wg->high = h;
            wg->WidgetID = i;
        }
	}

	return 0;
}
int wk_wg_MatrixMenu_Draw(Winks_Widget_s* p_MatrixMenu)
{
	int x1 = 0;
	int y1 = 0;
	int x2 = 0;
	int y2 = 0;
	int i = 0;
	Winks_Widget_MatrixMenu_s* pmm = (Winks_Widget_MatrixMenu_s*)p_MatrixMenu;
	
	//int flag = pmm->bwidget.flag & WK_WG_FGGRAY;
	
	x1 = pmm->bwidget.lux;
	y1 = pmm->bwidget.luy;
	x2 = x1 + pmm->bwidget.width;
	y2 = y1 + pmm->bwidget.high; 

	WINKS_CLIP_ON(x1,y1, x2, y2);
	////WINKS_GRAY_BEGIN(flag,x1,y1, x2, y2) ;
	
	//填充背景
    //winks_ui_fillRect(x1, y1, x2, y2, MatrixMenuItem_bg_color);

	if(pmm->bwidget.bgimage != 0)
    {
	    winks_ui_drawGraphicFromFile(x1, y1, Winks_UIFM_GetImage(pmm->bwidget.bgimage));
	}

	//画所有的ITEM控件
	for (i = 0; i < pmm->v_TotalMatrixItemsNum; i++) 
	{
		wk_wg_MatrixMenuItem_Draw((Winks_Widget_s *)&pmm->a_MenuItem[i]);
	}
                      
	////WINKS_GRAY_BEGIN(flag, x1, y1, x2, y2) ;
	WINKS_CLIP_OFF(x1, y1, x2, y2);
	return 0;
	 
}

static int if_pos_in_matrixmenu_with_bg_area(Winks_Widget_MatrixMenu_s* p_MatrixMenu,int x,int y)
{

	//Winks_gui_create_fixed_text_menuitem();
	//Winks_gui_show_fixed_text_menuitem();
	int icon_x = 0;
	int icon_y = 0;
	int width = 0;
	int height = 0;

	int index_in_screen = 0;
	int index_in_tal = 0;
	int v_old_index = 0;


	int temp = 0;
	int row_number, column_number;
	int mainmenuitem_height,mainmenuitem_weight;
	int x_gap = 0, y_gap = 0;
	Winks_Widget_MatrixMenuItem_s *p_temp;
	winks_pen_point_struct Pen_Pos;


	//WINKS_UI_CONTEXT_DEF()
	////WINKS_UI_CONTEXT_INIT();
	Pen_Pos.Pen_x = x;
	Pen_Pos.Pen_y = y;
	



	for (index_in_screen = 0,index_in_tal = 0;
	        //(index_in_screen <= WINKS_MATRIXMAINMENU_MAX_NUMBER) &&
	        (index_in_tal <= p_MatrixMenu->v_TotalMatrixItemsNum);
	        index_in_screen++, index_in_tal++ )

	{
		p_temp = &p_MatrixMenu->a_MenuItem[index_in_screen];
		if (if_pos_in_area(Pen_Pos, p_temp->bwidget.lux, p_temp->bwidget.luy, p_temp->bwidget.width, p_temp->bwidget.high ) == TRUE)
		{
			v_old_index =  p_MatrixMenu->v_FocusMatrixItemIndex;
			p_MatrixMenu->v_FocusMatrixItemIndex = index_in_screen;
			
			//break;

			if(v_old_index != p_MatrixMenu->v_FocusMatrixItemIndex)	
			{
				wk_Redraw_MatrixMenuItem(p_MatrixMenu, v_old_index, p_MatrixMenu->v_FocusMatrixItemIndex);
				
				return TRUE;
			
}
		}
	}



	return FALSE;

}

int wk_wg_MatrixMenu_PenHandler( Winks_Widget_s* p_MatrixMenu, int PenType, int x, int y )
{
	Winks_Widget_MatrixMenu_s * pMatrixMenu = (Winks_Widget_MatrixMenu_s *)p_MatrixMenu;
	switch( PenType )
	{
		case WINKS_PEN_FLAG_PENDOWN:

		case WINKS_PEN_FLAG_PENUP:
		case WINKS_PEN_FLAG_PEN_LONG_PRESS:
		case WINKS_PEN_FLAG_PEN_ALWAYS_PRESS :
		{
			if_pos_in_matrixmenu_with_bg_area(pMatrixMenu, x, y);
		}break;
		case WINKS_PEN_FLAG_PENMOVE :
		{
			if (if_pos_in_matrixmenu_with_bg_area(pMatrixMenu, x, y) == TRUE)
			{
				pMatrixMenu->bwidget.flag = WK_WG_FGPENSTATE;
			}

		}break;

		default:
		{

		}break;
	}
	return 0;
}

int wk_wg_MatrixMenu_KeyHandler( Winks_Widget_s* p_MatrixMenu, int KeyCode, int KeyType )
{
	Winks_Widget_MatrixMenu_s* p = (Winks_Widget_MatrixMenu_s*)p_MatrixMenu;
	{
	    Winks_Widget_s** ppLostFocus = NULL;
	    Winks_Widget_s** ppGetFocus = NULL;
	    
	    Winks_Widget_s* pWidget = NULL;
	    Winks_Widget_s* pTmpWidget = NULL;
	    int ret = 0, wdnum = 0, fline = 0, fcolumn = 0;
	    Winks_Widget_s* WidgetMatrix[WK_WGAPI_MAXMATRIXX][WK_WGAPI_MAXMATRIXY];

	    if( (pWidget = Winks_Widget_GetFocusWidget( &p->a_MenuItem[0].bwidget )) == NULL )
	    {
	        Winks_printf( "Winks Widget can not find focus widget" );
	        return -1;
	    }

	    if( (KeyCode != WINKS_KEY_UP) && (KeyCode != WINKS_KEY_DOWN)
	        && (KeyCode != WINKS_KEY_LEFT) && (KeyCode != WINKS_KEY_RIGHT) )
	    {
	        Winks_fail( "Winks Widget Navkey handler recv key not navkey." );
	        return -1;
	    }

	    if( KeyType == WINKS_KEY_FLAG_KEYDOWN )
	    {
	        /* 焦点需要发生变化 */
	        Winks_mem_set( WidgetMatrix, 0, sizeof(Winks_Widget_s*) * WK_WGAPI_MAXMATRIXX * WK_WGAPI_MAXMATRIXY );
	        pTmpWidget = &p->a_MenuItem[0].bwidget;
	        while( pTmpWidget )
	        {
	            if( pTmpWidget->flag & WK_WG_FGGETFOCUS )
	            {
	                Winks_assert( pTmpWidget->xindex < WK_WGAPI_MAXMATRIXX );
	                Winks_assert( pTmpWidget->yindex < WK_WGAPI_MAXMATRIXY );
	                WidgetMatrix[pTmpWidget->yindex][pTmpWidget->xindex] = pTmpWidget;
	                wdnum ++;
	            }

	            pTmpWidget = pTmpWidget->pnext;
	        }
	        if( wdnum <= 1 )
	        {
	            /* 只有一个可获得焦点的控件，直接返回 */
	            if( ppLostFocus )
	                *ppLostFocus = NULL;
	            if( ppGetFocus )
	                *ppGetFocus = pWidget;
	            return ret;
	        }
	        
	        switch( KeyCode )
	        {
	        case WINKS_KEY_UP:
	            if( (fline = winks_widget_findupline( (Winks_Widget_s** )WidgetMatrix, pWidget->yindex )) != pWidget->yindex )
	            {
	                if( (fcolumn = winks_widget_findbeforecolumn( (Winks_Widget_s** )WidgetMatrix, fline, pWidget->xindex )) < 0 )
	                    fcolumn = winks_widget_findaftercolumn( (Winks_Widget_s** )WidgetMatrix, fline, pWidget->xindex );
	                pTmpWidget = WidgetMatrix[fline][fcolumn];
	            }
	            else
	            {
	                /* 只有一行 */
	                pTmpWidget = pWidget;
	                pWidget = NULL;
	            }
	            break;

	        case WINKS_KEY_DOWN:
	            if( (fline = winks_widget_finddownline( (Winks_Widget_s** )WidgetMatrix, pWidget->yindex )) != pWidget->yindex )
	            {
	                if( (fcolumn = winks_widget_findbeforecolumn( (Winks_Widget_s** )WidgetMatrix, fline, pWidget->xindex )) < 0 )
	                    fcolumn = winks_widget_findaftercolumn( (Winks_Widget_s** )WidgetMatrix, fline, pWidget->xindex );
	                pTmpWidget = WidgetMatrix[fline][fcolumn];
	            }
	            else
	            {
	                /* 只有一行 */
	                pTmpWidget = pWidget;
	                pWidget = NULL;
	            }
	            break;

	        case WINKS_KEY_LEFT:
	            fcolumn = winks_widget_findbeforecolumn( (Winks_Widget_s** )WidgetMatrix, pWidget->yindex, pWidget->xindex - 1 );
	            if(  fcolumn < 0 )
	            {
	                fline = winks_widget_findupline( (Winks_Widget_s** )WidgetMatrix, pWidget->yindex );
	                fcolumn = winks_widget_findbeforecolumn( (Winks_Widget_s** )WidgetMatrix, fline, WK_WGAPI_MAXMATRIXX - 1 );
	                pTmpWidget = WidgetMatrix[fline][fcolumn];
	            }
	            else
	            {
	                pTmpWidget = WidgetMatrix[pWidget->yindex][fcolumn];
	            }
	            break;

	        case WINKS_KEY_RIGHT:
	            fcolumn = winks_widget_findaftercolumn( (Winks_Widget_s** )WidgetMatrix, pWidget->yindex, pWidget->xindex + 1 );
	            if( fcolumn < 0 )
	            {
	                fline = winks_widget_finddownline( (Winks_Widget_s** )WidgetMatrix, pWidget->yindex );
	                fcolumn = winks_widget_findaftercolumn( (Winks_Widget_s** )WidgetMatrix, fline, 0 );
	                pTmpWidget = WidgetMatrix[fline][fcolumn];
	            }
	            else
	            {
	                pTmpWidget = WidgetMatrix[pWidget->yindex][fcolumn];
	            }
	            break;
	        }
	    }
	    else
	    {
	        pTmpWidget = pWidget;
	        pWidget = NULL;
	    }

	    if( pWidget )
	    {
	        Winks_Widget_MatrixMenuItem_s* prev = &p->a_MenuItem[0];
	        p->v_FocusMatrixItemIndex = 0;
	        while(prev)
            {
                if(pTmpWidget==&prev->bwidget)
                    break;
                p->v_FocusMatrixItemIndex ++;
                prev = &p->a_MenuItem[p->v_FocusMatrixItemIndex];
            }
	        /* 如果有新控件获得了焦点，则请求绘制 */
	        pWidget->flag &= ~WK_WG_FGFOCUS;
	        pTmpWidget->flag |= WK_WG_FGFOCUS;
            wk_wg_MatrixMenuItem_Draw((Winks_Widget_s *)pWidget);
            wk_wg_MatrixMenuItem_Draw((Winks_Widget_s *)pTmpWidget);
	    }

	    if( ppLostFocus )
	        *ppLostFocus = pWidget;
	    if( ppGetFocus )
	        *ppGetFocus = pTmpWidget;

	    return ret;
	}
    return 0;
}

int wk_wg_MatrixMenu_StateModify( Winks_Widget_s* p_MatrixMenu, int Option, int ifvar )
{
	Winks_Widget_MatrixMenu_s* pMatrixMenu = (Winks_Widget_MatrixMenu_s*)p_MatrixMenu;
	
	if (Option == WK_WG_FGGRAY) 
	{
		if (ifvar == 1) 
		    pMatrixMenu->bwidget.flag =  WK_WG_FGGRAY;
		else
		{
			//if (ifvar == 1) 
	    			pMatrixMenu->bwidget.flag &=  ~WK_WG_FGGRAY;
		}
			
	}
	if (Option == WK_WG_FGFOCUS) 
	{
		if (ifvar == 1) 
		    pMatrixMenu->bwidget.flag |=  WK_WG_FGFOCUS;
		else
		{
			//if (ifvar == 1) 
	    			pMatrixMenu->bwidget.flag &=  ~WK_WG_FGFOCUS;
		}
			
	}
	return 0;
}

//test
//SpinBox
#define SpinBoxArrorW 9
#define SpinBoxArrorH 15
#define SpinBoxStaticW 212
#define SpinBoxStaticH 122
#define SpinBoxStaticOffW 6
#define SpinBoxStaticOffH 6

/*WK_WG_SPINBOX      18  //旋钮控件  */ 
int wk_wg_SpinBox_Init(Winks_Widget_s* p_SpinBox)
{
    Winks_Widget_SpinBox_s *p = (Winks_Widget_SpinBox_s *)p_SpinBox;
    p->bg_iconid = WINKS_IMGID_UNACTIVATED_PICFRM;
    p->left_iconid = WINKS_IMGID_ARROW_LEFT;
    p->hl_left_iconid = WINKS_IMGID_ARROW_LEFTHL;
    p->right_iconid = WINKS_IMGID_ARROW_RIGHT;
    p->hl_right_iconid = WINKS_IMGID_ARROW_RIGHTHL;
    if(p->v_TotalItemNums)
    {
        Winks_Widget_s *prewg = NULL;
        Winks_Widget_s *wg = NULL;
        int i = 0;
        int offW = (p->bwidget.width - SpinBoxStaticW)/2;
        int offH = (p->bwidget.high - SpinBoxStaticH)/2;
        int w = SpinBoxStaticW - 2*SpinBoxStaticOffW;
        int h = SpinBoxStaticH - 2*SpinBoxStaticOffH;
        //初始化每个ITEM的位置
        for(i = 0;i < p->v_TotalItemNums; i++)
        {
            wg = &p->a_Static[i].bwidget;
            //把所有ITEM用链表串起来
            if(prewg)
                prewg->pnext = wg;
            prewg = wg;

            wk_wg_Static_Init((Winks_Widget_s *)&(p->a_Static[i]));
            wg->flag = 0;
            wg->xindex = i;
            wg->yindex = 0;
            wg->lux = p->bwidget.lux + offW + SpinBoxStaticOffW;
            wg->luy = p->bwidget.luy + offH + SpinBoxStaticOffH;
            wg->width = w;
            wg->high = h;
        }
    }
    return 0;
}

int wk_wg_SpinBox_Draw(Winks_Widget_s* p_SpinBox)
{
    Winks_Widget_SpinBox_s * p = (Winks_Widget_SpinBox_s *) p_SpinBox;
    int x1 = p->bwidget.lux;//左上角x
    int y1 = p->bwidget.luy;//左上角y
    int x2 = x1 + p->bwidget.width;//右下角x
    int y2 = y1 + p->bwidget.high;//右下角y
    int offW = (p->bwidget.width - SpinBoxStaticW)/2;//背景框和左右边的距离
    int offH = (p->bwidget.high - SpinBoxStaticH)/2;//背景框和上下边的距离
    int lax = x1 + (offW-SpinBoxArrorW)/2;//左箭头x
    int rax = x2 - (offW-SpinBoxArrorW)/2 - SpinBoxArrorW;//右箭头x
    int lray = y1 + (p->bwidget.high-SpinBoxArrorH)/2;//左右箭头y
    int laicon = p->left_iconid;//左箭头图标
    int raicon = p->right_iconid;//右箭头图标
    int bgicon = p->bg_iconid;//静态背景框图标
    int bgcolor = WINKS_PIXEL_GRAY;//背景填充颜色
    //int flag = p->bwidget.flag & WK_WG_FGGRAY;
    
    if (p->bwidget.flag & WK_WG_FGFOCUS)
    {
        bgcolor = WINKS_PIXEL_YELLOW;
        if(p->v_TotalItemNums>1)
        {
            if(p->v_FocusItemIndex + 1 < p->v_TotalItemNums)
                raicon = p->hl_right_iconid;
            else
                raicon = p->right_iconid;
            if(p->v_FocusItemIndex > 0)
                laicon = p->hl_left_iconid;
            else
                laicon = p->left_iconid;
        }
    }
    else
    {
        bgcolor = WINKS_PIXEL_GRAY;
        laicon = p->left_iconid;
        raicon = p->right_iconid;
    }

    WINKS_CLIP_ON(x1, y1, x2, y2);
    ////WINKS_GRAY_BEGIN(flag, x1, y1, x2, y2);

    //填充背景
    winks_ui_fillRect(x1, y1, x2, y2, bgcolor);
    //Static背景边框
    winks_ui_drawGraphicFromFile(x1 + offW, y1 + offH, Winks_UIFM_GetImage(bgicon));
    //左箭头
    winks_ui_drawGraphicFromFile(lax, lray, Winks_UIFM_GetImage(laicon));
    //右箭头
    winks_ui_drawGraphicFromFile(rax, lray, Winks_UIFM_GetImage(raicon));
    //画Static
    wk_wg_Static_Draw((Winks_Widget_s *)&p->a_Static[p->v_FocusItemIndex]);
    //test
    if(0)
    {
        winks_ui_drawGraphicFromFile(x1 + offW + SpinBoxStaticOffW, y1 + offH + SpinBoxStaticOffH, 
                Winks_UIFM_GetImage(p->a_Static[p->v_FocusItemIndex].v_ContentID));
    }
    
    ////WINKS_GRAY_END(flag, x1, y1, x2, y2);
    WINKS_CLIP_OFF(x1, y1, x2, y2);
    return 0;
}

int wk_wg_SpinBox_PenHandler( Winks_Widget_s* p_SpinBox, int PenType, int x, int y )
{
    return 0;
}

int wk_wg_SpinBox_KeyHandler( Winks_Widget_s* p_SpinBox, int KeyCode, int KeyType )
{
    int ret = 0;
    Winks_Widget_SpinBox_s * p = (Winks_Widget_SpinBox_s *) p_SpinBox;
    switch (KeyCode)
    {
    case WINKS_KEY_UP:
    {
        if (KeyType == WINKS_KEY_FLAG_KEYDOWN)
            ret = WK_UIFM_FOCUSCHANGE;
    }
        break;
    case WINKS_KEY_DOWN:
    {
        if (KeyType == WINKS_KEY_FLAG_KEYDOWN)
            ret = WK_UIFM_FOCUSCHANGE;
    }
        break;
    case WINKS_KEY_LEFT:
    {
        if (KeyType == WINKS_KEY_FLAG_KEYDOWN)
            ret = WK_UIFM_SUCCESS;
        if(p->v_TotalItemNums>1)
        {
            if(p->v_FocusItemIndex - 1 >= 0)
            {
                p->v_FocusItemIndex --;
                wk_wg_SpinBox_Draw((Winks_Widget_s *)p);                
            }
            
        }
    }
        break;
    case WINKS_KEY_RIGHT:
    {
        if (KeyType == WINKS_KEY_FLAG_KEYDOWN)
            ret = WK_UIFM_SUCCESS;
        if(p->v_TotalItemNums>1)
        {
            if(p->v_FocusItemIndex + 1 < p->v_TotalItemNums)
            {
                p->v_FocusItemIndex ++;
                wk_wg_SpinBox_Draw((Winks_Widget_s *)p);
                
            }
            
        }
    }
        break;
    default:
    {
        break;
    }
    }
    return ret;
}

int wk_wg_SpinBox_StateModify( Winks_Widget_s* p_SpinBox, int Option, int ifvar )
{
    Winks_Widget_SpinBox_s * p = (Winks_Widget_SpinBox_s *)p_SpinBox;
    
    if (Option == WK_WG_FGGRAY) 
    {
        if (ifvar == 1) 
            p->bwidget.flag |=  WK_WG_FGGRAY;
        else
        {
            p->bwidget.flag &= ~WK_WG_FGGRAY;
        }            
    }
    if (Option == WK_WG_FGFOCUS) 
    {
        if (ifvar == 1) 
            p->bwidget.flag |=  WK_WG_FGFOCUS;
        else
        {
            p->bwidget.flag &= ~WK_WG_FGFOCUS;
        }            
    }
    wk_wg_SpinBox_Draw((Winks_Widget_s * )p);
    return 0;    
}

/*WK_WG_GLIDEPOLE	    19  //滑杆  */ 
int wk_wg_GlidePole_Init(Winks_Widget_s* p_GlidePole)
{
	Winks_Widget_GlidePole_s* pGP = (Winks_Widget_GlidePole_s*)p_GlidePole;

	pGP->bwidget.font.size = WINKS_MEDIUM_FONT;
	pGP->bwidget.font.style = WINKS_FONT_STANDARD;
	pGP->bwidget.font.type = WK_WG_FONTSONG;
	pGP->bwidget.font.way = WK_WG_FONTNOSCROLL;
	pGP->bwidget.font.color = WINKS_PIXEL_BLACK;
	pGP->bwidget.flag |= WK_WG_FGGETFOCUS;
// 	pGP->bwidget.lux = WK_GLIDEPOLE_DFPOSX;
// 	pGP->bwidget.luy = WK_GLIDEPOLE_DFPOSY;
// 	pGP->bwidget.width = WK_GLIDEPOLE_DFWIDTH;
// 	pGP->bwidget.high = WK_GLIDEPOLE_DFHEIGHT;
	pGP->bwidget.bgimage = WINKS_IMGID_UPDATESETTING_GLIDEPOLE;
	pGP->bwidget.hlimage = WINKS_IMGID_UPDATESETTING_GLIDEPOLEHL;
	pGP->v_SliderIconID = WINKS_IMGID_UPDATESETTING_SELBLOCK;
	pGP->v_MinNum = 1;
	pGP->v_MaxNum = 10;
	pGP->v_StepNum = 1;
	pGP->v_CurNum = 1;
	return 0;
}

int wk_wg_GlidePole_Draw(Winks_Widget_s* p_GlidePole)
{
	int bkgx1 = 0, bkgy1 = 0, sliderx1 = 0, slidery1 = 0;
	int nCount = 0, nCur = 0;

	Winks_Widget_GlidePole_s* pGP = (Winks_Widget_GlidePole_s*)p_GlidePole;

	bkgx1 = pGP->bwidget.lux;
	bkgy1 = slidery1 = pGP->bwidget.luy;

	if (pGP->v_MaxNum > pGP->v_MinNum)
	{
		if (pGP->v_CurNum < pGP->v_MinNum || pGP->v_CurNum > pGP->v_MaxNum)
		{
			pGP->v_CurNum = pGP->v_MinNum;
		}
		nCount = pGP->v_MaxNum - pGP->v_MinNum;
		nCur = pGP->v_CurNum - pGP->v_MinNum;
	}
	else
	{
		if (pGP->v_CurNum < pGP->v_MaxNum || pGP->v_CurNum > pGP->v_MinNum)
		{
			pGP->v_CurNum = pGP->v_MinNum;
		}
		nCount = pGP->v_MinNum - pGP->v_MaxNum;
		nCur = pGP->v_CurNum - pGP->v_MaxNum;
	}

	sliderx1 = bkgx1 + nCur * (pGP->bwidget.width - WK_GLIDEPOLE_SLIDERWIDTH) / nCount;

	WINKS_CLIP_ON(bkgx1, bkgy1, bkgx1 + pGP->bwidget.width, bkgy1 + pGP->bwidget.high);
	////WINKS_GRAY_BEGIN(pGP->bwidget.flag, bkgx1, bkgy1, bkgx1 + pGP->bwidget.width, bkgy1 + pGP->bwidget.high) ;

	if((pGP->bwidget.flag & WK_WG_FGFOCUS) == WK_WG_FGFOCUS)
	{
		if(pGP->bwidget.hlimage != 0)
		{
			winks_ui_drawGraphicFromFile(bkgx1, bkgy1, Winks_UIFM_GetImage(pGP->bwidget.hlimage ));
		}
	}
	else
	{
		if(pGP->bwidget.bgimage != 0)
		{
			winks_ui_drawGraphicFromFile(bkgx1, bkgy1, Winks_UIFM_GetImage(pGP->bwidget.bgimage ));
		}
	}

	if(pGP->v_SliderIconID != 0)
	{
		winks_ui_drawGraphicFromFile(sliderx1, slidery1, Winks_UIFM_GetImage(pGP->v_SliderIconID ));
	}

	//WINKS_GRAY_BEGIN(pGP->bwidget.flag, bkgx1, bkgy1, bkgx1 + pGP->bwidget.width, bkgy1 + pGP->bwidget.high) ;
	WINKS_CLIP_OFF(bkgx1, bkgy1, bkgx1 + pGP->bwidget.width, bkgy1 + pGP->bwidget.high);

	return 0;
}

int wk_wg_GlidePole_PenHandler( Winks_Widget_s* p_GlidePole, int PenType, int x, int y )
{
	return 0;
}

int wk_wg_GlidePole_KeyHandler( Winks_Widget_s* p_GlidePole, int KeyCode, int KeyType )
{
	int ret = 0;
	Winks_Widget_GlidePole_s* pGP = (Winks_Widget_GlidePole_s*)p_GlidePole;
	switch(KeyCode)
	{
	case WINKS_KEY_UP:
		{
			if (KeyType == WINKS_KEY_FLAG_KEYDOWN)
			{
				ret = WK_UIFM_FOCUSCHANGE;
			}
		}
		break;
	case WINKS_KEY_DOWN:
		{
			if (KeyType == WINKS_KEY_FLAG_KEYDOWN)
			{
				ret = WK_UIFM_FOCUSCHANGE;
			}
		}
		break;
	case WINKS_KEY_LEFT:
		{
			if (KeyType == WINKS_KEY_FLAG_KEYDOWN)
			{
				if (pGP->v_MaxNum > pGP->v_MinNum)
				{
					if (pGP->v_StepNum > 0)
					{
						pGP->v_CurNum -= pGP->v_StepNum;
					}
					else
					{
						pGP->v_CurNum += pGP->v_StepNum;
					}
					if (pGP->v_CurNum < pGP->v_MinNum)
					{
						pGP->v_CurNum = pGP->v_MinNum;
					}
				}
				else
				{
					if (pGP->v_StepNum > 0)
					{
						pGP->v_CurNum += pGP->v_StepNum;
					}
					else
					{
						pGP->v_CurNum -= pGP->v_StepNum;
					}
					if (pGP->v_CurNum > pGP->v_MinNum)
					{
						pGP->v_CurNum = pGP->v_MinNum;
					}
				}

				wk_wg_GlidePole_Draw((Winks_Widget_s * )pGP);
			}
		}
		break;
	case WINKS_KEY_RIGHT:
		{
			if (KeyType == WINKS_KEY_FLAG_KEYDOWN)
			{
				if (pGP->v_MaxNum > pGP->v_MinNum)
				{
					if (pGP->v_StepNum > 0)
					{
						pGP->v_CurNum += pGP->v_StepNum;
					}
					else
					{
						pGP->v_CurNum -= pGP->v_StepNum;
					}
					if (pGP->v_CurNum > pGP->v_MaxNum)
					{
						pGP->v_CurNum = pGP->v_MaxNum;
					}
				}
				else
				{
					if (pGP->v_StepNum > 0)
					{
						pGP->v_CurNum -= pGP->v_StepNum;
					}
					else
					{
						pGP->v_CurNum += pGP->v_StepNum;
					}
					if (pGP->v_CurNum < pGP->v_MaxNum)
					{
						pGP->v_CurNum = pGP->v_MaxNum;
					}
				}

				wk_wg_GlidePole_Draw((Winks_Widget_s * )pGP);
			}
		}
		break;
	}
	return ret;
}

int wk_wg_GlidePole_StateModify( Winks_Widget_s* p_GlidePole, int Option, int ifvar )
{
	Winks_Widget_GlidePole_s * pGP = (Winks_Widget_GlidePole_s *)p_GlidePole;
	if (Option == WK_WG_FGFOCUS) 
	{
		if (ifvar == 1)
		{
			pGP->bwidget.flag |=  WK_WG_FGFOCUS;
		}
		else
		{
			pGP->bwidget.flag &=  ~WK_WG_FGFOCUS;
		}
	}
	wk_wg_GlidePole_Draw((Winks_Widget_s *)pGP);
	return 0;
}
#if 0
int wk_wg_Draw_Widget(Winks_Widget_s* bwidget )
{
switch( bwidget->type )
{
    case WK_WG_EDITBOX:
      Winks_Draw_Editbox( (Winks_Widget_EditBox_s* )bwidget );
      break;
    case WK_WG_BUTTON:
      Winks_Draw_Button( (Winks_Widget_Butto_s* )bwidget );
      break;
    ……
    default:
      //error message;
      break;
}

return 0;
}

int wk_wg_Widget_Focus( Winks_Widget_s* bwidget, int iffocus )
{
	switch( bwidget->type )
	{
	    case WK_WG_EDITBOX:
	      winks_draw_editbox( (Winks_Widget_EditBox_s* )bwidget );
	      break;
	    case WK_WG_BUTTON:
	      winks_draw_button( (Winks_Widget_Butto_s* )bwidget );
	      break;
	    ……
	    default:
	      //error message;
	      break;
	}
}
int wk_wg_Widget_Change( Winks_Widget_s* bwidget, int conflag )
{
	switch( bwidget->type )
	{
	    case WK_WG_EDITBOX:
	      winks_draw_editbox( (Winks_Widget_EditBox_s* )bwidget );
	      break;
	    case WK_WG_BUTTON:
	      winks_draw_button( (Winks_Widget_Butto_s* )bwidget );
	      break;
	    ……
	    default:
	      //error message;
	      break;
	}
}
int wk_wg_Widget_PenHandler( Winks_Widget_s* bwidget, int PenType, int x, int y );
int wk_wg_Widget_KeyHandler( Winks_Widget_s* bwidget, int KeyCode, int KeyType );

#endif
