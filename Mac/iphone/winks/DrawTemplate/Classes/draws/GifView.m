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

@synthesize plView;

- (int)prepareGifLayer:(PlayerView *)pplView gifPath:(NSString *)pgifpath
{
	// gif层
	FILE *fp;
	NSString *gifPath = [[NSBundle mainBundle] pathForResource:pgifpath ofType:nil];
	if((fp = fopen([gifPath cStringUsingEncoding:NSASCIIStringEncoding], "r")) == NULL)
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

- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
		self.alpha = 0.0f;
		
		self.plView = [[PlayerView alloc] initWithFrame:CGRectMake(42.0f,40.0f, 240.0f, 236.0f)];
		int ret = [self prepareGifLayer:plView gifPath:@"defwinks.gif"];
		if (ret == 0) {
			[self addSubview:plView];
		}
		[plView release];
    }
    return self;
}

- (void)drawRect:(CGRect)rect {
    // Drawing code
	draw_frame(self.bounds, 0x00ff00ff);
}


- (void)dealloc {
	NSLog(@"GifView dealloc ---- ");
    [super dealloc];
	
	NSLog(@"gifview = %u", [self retainCount]);
	NSLog(@"plView = %u", [plView retainCount]);
	if(plView != nil){
		[plView removeFromSuperview];
		[plView stopAnimation];
		[plView release];
		plView = nil;
	}
}


@end
