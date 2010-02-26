//
//  GifView.m
//  TemplateShow
//
//  Created by luke on 10-2-7.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "GifView.h"
#import "../glgif/GifVideo.h"
#import "../glgif/VideoSource.h"
#import "../draws/wk_osdraw.h"

@implementation GifView

@synthesize plView, gMedia;


- (id)initWithSection:(Winks_CCDW_Media_s *)pSection
{
	gMedia = pSection;
	id instance = [self initWithFrame:CGRectMake(gMedia->base.Section.x, gMedia->base.Section.y, 
												 gMedia->base.Section.w, gMedia->base.Section.h)];
	
	return instance;
}

- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
		//self.alpha = 0.0f;
		
		//self.plView = [[PlayerView alloc] initWithFrame:CGRectMake(42.0f,40.0f, 240.0f, 236.0f)];
		self.plView = [[PlayerView alloc] initWithFrame:CGRectMake(0, 0, self.frame.size.width, self.frame.size.height)];
		int ret = [self prepareGifLayer:plView gifPath:gMedia->pbuf];
		if (ret == 0) {
			//[self addSubview:plView];
			[self.layer addSublayer:plView.layer];
		}
		[plView release];
    }
    return self;
}

- (void)drawRect:(CGRect)rect {
    // Drawing code
	CGContextRef context = init_drawContext();
	CGRect prect = CGRectMake(0, 0, self.frame.size.width, self.frame.size.height);
	draw_frame(prect, 0xff0000ff);
}


- (void)dealloc {
	NSLog(@"GifView dealloc ---- ");
	
	NSLog(@"gifview = %u", [self retainCount]);
	NSLog(@"plView = %u", [plView retainCount]);
	if(plView != nil){
		[plView removeFromSuperview];
		[plView stopAnimation];
		[plView release];
		plView = nil;
	}
    [super dealloc];
}


#pragma mark gif
- (int)prepareGifLayer:(PlayerView *)pplView gifPath:(char *)pgifpath
{
	// gif层
	FILE *fp;
	//NSString *gifPath = [[NSBundle mainBundle] pathForResource:pgifpath ofType:nil];
	//if((fp = fopen([gifPath cStringUsingEncoding:NSASCIIStringEncoding], "r")) == NULL)
	if((fp = fopen(pgifpath, "r")) == NULL)
	{
		return -1;
	}
	else 
	{
		VideoSource *src = VideoSource_init(fp, VIDEOSOURCE_FILE);
		src->writeable = false;
		// Init video using VideoSource
		Video *vid = [[GifVideo alloc] initWithSource:src inContext:[pplView context]];
		VideoSource_release(src);
		// Start if loaded
		if (vid) {
			[pplView startAnimation:vid];
			[vid release];
			//[self.view addSubview:pplView];
		}else{
			// Cleanup if failed
			fclose(fp);
			return -1;
		}
	}
	return 0;
}

@end
