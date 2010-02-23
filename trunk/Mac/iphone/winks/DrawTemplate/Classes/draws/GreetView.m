//
//  GreetView.m
//  TemplateShow
//
//  Created by luke on 10-2-7.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "GreetView.h"
#import "wk_osdraw.h"

@implementation GreetView

@synthesize gText;

extern Winks_CCSW_Global_s gCCSW;
static int showtext_in_area(winks_scrolling_text_s * text);
static void handle_scrolling_text(winks_scrolling_text_s *s);

- (id)initWithSection:(Winks_CCDW_Text_s *)pSection
{
	gText = pSection;
	id instance = [self initWithFrame:CGRectMake(gText->base.Section.x, gText->base.Section.y, 
								   gText->base.Section.w, gText->base.Section.h)];
	
	return instance;
}

- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        // Initialization code
		super.backgroundColor = [UIColor colorWithWhite:1.0f alpha:0.0f];
    }
    return self;
}

static int utf2UnicodeString(const char *in_utf8, char *out_unicode, unsigned int size)
{
	NSString *nsUtf8 = [NSString stringWithUTF8String:in_utf8];
	if ([nsUtf8 canBeConvertedToEncoding:NSUnicodeStringEncoding]) {
		memcpy(out_unicode, [nsUtf8 cStringUsingEncoding:NSUnicodeStringEncoding], size);
	} else {
		return -1;
	}
	
	return 0;
}

- (void)drawRect:(CGRect)rect {
    // Drawing code
	CGContextRef context = init_drawContext();
/*
	draw_frame(rect, 0x00ff00ff);
	
	set_curColor(0x00ff00ff);
	
	NSString *teststr = @"wie我我问那你";
	CGRect testRect = CGRectMake(self.bounds.origin.x, self.bounds.size.height - 20, self.bounds.size.width, 20.0f);
	[teststr drawInRect:testRect withFont:[UIFont systemFontOfSize:18.0f] 
		  lineBreakMode:UILineBreakModeClip alignment:UITextAlignmentCenter];
*/	
	
#if 1
	
    int dlen = 0;
    unsigned long fflag = 0;
    char* pdata = NULL;
    //Winks_CCSW_Global_s* pGlobal = &gCCSW;
    printf( "WK CDWW Draw Text %s\r\n", gText->base.pdata );
	
    if( (gText->base.flags & WK_CCDW_RGFLAG_SREF) && (gText->txtstyle.text == NULL) )
    {
        /* get data from server ref */
        dlen = WK_CCDW_TEXTBUFLEN;
        /* deal greeting sepreatedly */
		if( (Winks_GetShowElement( WINKS_DFT_CC_NO, WINKS_TRUE, 
									   gText->base.pdata, &fflag, gText->pbuf, (unsigned long* )(&dlen), NULL, 0 ) < 0) )
        {
            if( dlen > WK_CCDW_TEXTBUFLEN )
            {
                pdata = malloc( dlen + 2 );
                memset( pdata, 0, dlen + 2 );
                if( Winks_GetShowElement( WINKS_DFT_CC_NO, WINKS_TRUE, 
										 gText->base.pdata, &fflag, pdata, (unsigned long* )(&dlen), NULL, 0 ) < 0 )
                {
                    free( pdata );
                    return;
                }
                gText->flags |= WK_CCDW_FTALLOC;
                gText->txtstyle.text = pdata;
            }
            else
                return;
        }
        else
            gText->txtstyle.text = gText->pbuf;
	}
    printf( "WK CDWW Draw Text %s\r\n", gText->txtstyle.text );
	/*
	if( gText->txtstyle.text == NULL )
	{
		if (gText->base.pdata != NULL)
			dlen = strlen( gText->base.pdata ) * 2 + 2;
		if( dlen < WK_CCDW_TEXTBUFLEN )
		{
			gText->txtstyle.text = gText->pbuf;
			utf2UnicodeString( gText->base.pdata, gText->txtstyle.text, dlen );
		}
		else
		{
			gText->txtstyle.text = malloc( dlen );
			memset( gText->txtstyle.text, 0, dlen );
			utf2UnicodeString( gText->base.pdata, gText->txtstyle.text, dlen );
			gText->flags |= WK_CCDW_FTALLOC;
		}
	}
	*/
    assert( gText->txtstyle.text  && "gText->txtstyle.text should not be NULL" );
    if( gText->flags & WK_CCDW_FTNEEDSCROLL )
    {
        handle_scrolling_text( &(gText->txtstyle) );
        gText->base.flags |= WK_CCDW_RGFLAG_REDRAW;
    }
    else
    {
        if( showtext_in_area( &(gText->txtstyle) ) == WINKS_NEEDSCROLLING_FLAG )
        {
            gText->flags |= WK_CCDW_FTNEEDSCROLL;
            gText->base.flags |= WK_CCDW_RGFLAG_REDRAW;
        }
    }
	
    printf( "WK CCDW Draw Text %d, %d, %d, %d ref is %s\r\n", gText->base.Section.x, gText->base.Section.y, 
        gText->base.Section.w, gText->base.Section.h, gText->base.pdata );
    
#endif
}


- (void)dealloc {
    [super dealloc];
}

#pragma mark draw

static int showtext_in_area(winks_scrolling_text_s * ptext)
{
    if((short *)ptext->text == 0 || *(short *)(ptext->text) == 0)
        return 0;
	
    int w = 0;
    int h = 0;
    int ret = 0;
    int text_len = 0;
	
	if(ptext->draw_backgroundimage == NULL || *(short*)ptext->draw_backgroundimage == 0)
    {
        if(ptext->backgroudcolorflags)
			winks_ui_fillRect(ptext->x, ptext->y, ptext->x + ptext->width, ptext->y + ptext->height,ptext->backgroud_color);
    }
    else
    {
        winks_ui_drawGraphicFromFile(ptext->bgimage_x, ptext->bgimage_y, ptext->draw_backgroundimage);
    }
	
	//set_curColor(ptext->font_feature.color);
	set_curColor(0x00ff00ff);
	winks_ui_textOutlen(ptext->x, 0, ptext->text, 0);
	return ret;
}

static void handle_scrolling_text(winks_scrolling_text_s *s)
{
	
}
@end
