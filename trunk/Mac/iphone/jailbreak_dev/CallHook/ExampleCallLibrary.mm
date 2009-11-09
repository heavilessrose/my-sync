/*
 *  ExampleHookLibrary.mm
 *  
 *
 *  Created by John on 10/4/08.
 *  Copyright 2008 Gojohnnyboi. All rights reserved.
 *
 */

#import "ExampleCallLibrary.h"
#import "GifVideo.h"
#import "VideoSource.h"
#import "VideoTexture.h"
#import "PlayerView.h"
#import "MyView.h"
#include <unistd.h>


static int incomingCallCount = 0;
static int dialCallCount = 0;
static int isIncomingCall = 0;

static int threadCount = 0;

static PlayerView *plView = NULL;
static PlayerView *plView2 = NULL;

static int testCount = 0;

//NSString* CTSettingCopyMyPhoneNumber(void* tmp);
NSString *CTSIMSupportCopyMobileSubscriberIdentity(void* tmp);

void launchApp () {
	#if 0
	   pid_t pID = fork();
	   if (pID == 0)                // child
	   {
		execlp("/Applications/HelloWorld.app/HelloWorld",NULL);
	    }
	    else if (pID < 0)            // failed to fork
	    {
			CallLog("Failed to fork");
	    }
	    else                          // parent
	    {
	    }
	#endif
	#if 0
		BOOL tmp = [[UIApplication sharedApplication]  launchApplicationWithIdentifier:@"com.pjtrix.helloworld" suspended:FALSE];
		if (tmp) 
		{
			CallLog("Launching true");
		}else {
			CallLog("Launching false");
		}
	#endif
	
	#if 1
		BOOL tmp = [[UIApplication sharedApplication]  openURL:[NSURL URLWithString:@"MyTestApp://"]];
		if (tmp) 
		{
			CallLog("Launching true");
		}else {
			CallLog("Launching false");
		}
	#endif
}
static void turnOnView (int &istatus)
	{
	    NSArray *windows = [[UIApplication sharedApplication] windows];
	    UIWindow *mainWindow = [windows objectAtIndex:0];
		//CallLog("windows count:");
	//	int aa = windows.count;
//	CallLogInt(aa);
	    //const char *tmp = [[mainWindow description]  cStringUsingEncoding:NSASCIIStringEncoding];
	   // CallLog(tmp);
		id phone_number = CTSettingCopyMyPhoneNumber();
		NSLog( @"%@", phone_number );
		[phone_number release];
		
		//const char *tmp = [CTSettingCopyMyPhoneNumber() cStringUsingEncoding:NSASCIIStringEncoding];
      // CallLog(tmp);

	    if (mainWindow != NULL)
	    {
		    
		      //[mainWindow addSubview:contentView];
			   //int i = [mainWindow.subviews count];
			   //CallLogInt(i);
		       UIView *originalView = [mainWindow.subviews objectAtIndex:0];
		       UIView *desktopView = [originalView.subviews objectAtIndex:0];


		       //int ii = [originalView.subviews count];
		       //CallLogInt(ii);
		       [desktopView removeFromSuperview];
		   	 
		    if((3 == istatus)||(1 ==istatus))
			 {
				
	          UIView *plView = [[UIView alloc] initWithFrame:CGRectMake(0.0f,100.0f, 320.0f, 275.0f)];
	          //MyView *plView = [[MyView alloc] initWithFrame:CGRectMake(0.0f,0.0f, 320.0f, 480.0f)];
	          UITextView *text = [[UITextView alloc] initWithFrame:CGRectMake(0.0f, 48.0f, 320.0f, 480.0f)];
	          [text setText: [[NSString alloc]initWithString: @"Hello World"]];
	          [plView addSubview: text];

	          [plView autorelease];
	          [text autorelease];

		       [originalView insertSubview:plView atIndex:0];
		       UIView *desktopView1 = [originalView.subviews objectAtIndex:1];
		       desktopView1.alpha = 0.5f;
		   }
		   else if(5 == istatus)
		   {			 
			   UIView *desktopView1 = [originalView.subviews objectAtIndex:0];
			    desktopView1.alpha = 1.0f;
		   }		
	     }

	}


static void testView() 
{
	CallLog("testView");
	
	UIApplication *app = [UIApplication sharedApplication];
	//const char *tmp4 = [[app description] cStringUsingEncoding:NSASCIIStringEncoding];
	//CallLog(tmp4);
	
	NSArray *windows = [[UIApplication sharedApplication] windows];
	UIWindow *mainWindow = [windows objectAtIndex:0];
	//UIWindow *mainWindow = [[UIApplication sharedApplication] keyWindow];
	
	
	
	// if (mainWindow == NULL) 
	// {
	// 	CallLog("mainwindow==null");
	// 	mainWindow = [[UIWindow alloc] initWithContentRect: [UIHardware fullScreenApplicationContentRect]];
	// 	CallLog("mainwindow==null2");
	// 	NSArray * list = [NSArray arrayWithObject:mainWindow];
	// 	CallLog("mainwindow==null3");
	// 	//[app setWindows:list];
	// 	app.windows = list;
	// 	[mainWindow makeKeyAndVisible];
	// 	[mainWindow autorelease];
	// }
	
	// const char *tmp5 = [[mainWindow description] cStringUsingEncoding:NSASCIIStringEncoding];
	// 	CallLog(tmp5);
	
	// const char *tmp2 = [[windows description] cStringUsingEncoding:NSASCIIStringEncoding];
	// 	CallLog(tmp2);
			
	// int tmp3 = [windows count];
	// 	CallLogInt(tmp3);
	
	#if 0
	if (mainWindow != NULL)
	{
		//UIView *plView = [[UIView alloc] initWithFrame: CGRectMake(0.0f, 0.0f, 320.0f, 480.0f)];
		MyView *plView = [[MyView alloc] initWithFrame: CGRectMake(0.0f, 0.0f, 320.0f, 480.0f)];
		UITextView *text = [[UITextView alloc] initWithFrame: CGRectMake(0.0f, 48.0f, 320.0f, 480.0f)];
		[text setText: [[NSString alloc] 
			initWithString: @"Hello World"]];
		[plView addSubview: text];

		[plView autorelease];
		[text autorelease];
		
		//[mainWindow addSubview:contentView];
		UIView *originalView = [mainWindow.subviews objectAtIndex:0];
		UIView *desktopView = [originalView.subviews objectAtIndex:0];
		
		int ii = [originalView.subviews count];
		CallLogInt(ii);
		
		[desktopView removeFromSuperview];
		[originalView insertSubview:plView atIndex:0];	
		
	}
	#endif
	
	#if 1
	if (mainWindow != NULL)
	{
		//PlayerView *plView = [[PlayerView alloc] initWithFrame: CGRectMake(0.0f, 0.0f, 320.0f, 480.0f)];
		plView2 = [[PlayerView alloc] initWithFrame: CGRectMake(0.0f, 100.0f, 320.0f, 275.0f)];
		/*MyView *plView = [[MyView alloc] initWithFrame: CGRectMake(0.0f, 0.0f, 320.0f, 480.0f)];
		UITextView *text = [[UITextView alloc] initWithFrame: CGRectMake(0.0f, 48.0f, 320.0f, 480.0f)];
		[text setText: [[NSString alloc] initWithString: @"Hello World"]];
		[plView addSubview: text];

		[plView autorelease];
		[text autorelease];*/

		//[mainWindow addSubview:plView];
		UIView *originalView = [mainWindow.subviews objectAtIndex:0];
		UIView *desktopView = [originalView.subviews objectAtIndex:0];
		
		////////////////////////////////////////////////////////////
	    // Load test.gif VideoSource
	    // NSString *str = [[NSBundle mainBundle] pathForResource:@"/var/mobile/test.gif" ofType:nil];
	    // FILE *fp = fopen([str UTF8String], "r");
		FILE *fp = fopen("/var/mobile/rotating.gif", "r");
		VideoSource *src = VideoSource_init(fp, VIDEOSOURCE_FILE);
	    src->writeable = false;

	    // Init video using VideoSource
	    Video *vid = [[GifVideo alloc] initWithSource:src inContext:[plView2 context]];
	    VideoSource_release(src);


	    // Start if loaded
	    if (vid) {
			CallLogInt(555);
	        [plView2 startAnimation:vid];
	        [vid release];
	    } 
		else 
		{
			CallLogInt(666);
			// Cleanup if failed
		    fclose(fp);
		}
	 
		//[desktopView removeFromSuperview];
		[originalView insertSubview:plView2 atIndex:1];
	}
	#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
static void __$ExampleHook_AppIcon_Launch(SBApplicationIcon<CallHook> *_SBApplicationIcon) {
	#if 0
	UIAlertView* __launchView = [[UIAlertView alloc] init];
	__launchView.title = @"No way muchacho";
	__launchView.message = @"You can't touch dis!";
	[__launchView addButtonWithTitle:@"Dismiss"];
	[__launchView show];
	#endif
	// If at any point we wanted to have it actually launch we should do:
	[_SBApplicationIcon __OriginalMethodPrefix_launch];
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
static BOOL __$ExampleCall_showsDesktopImage(SBCallAlertDisplay<CallHook> *_SBCallAlertDisplay)
{
	//
	CallLog("SBCallAlertDisplay_showsDesktopImage");

	return false;
	//return [_SBCallAlertDisplay __OriginalMethodPrefix_showsDesktopImage];
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
static id __$ExampleCall_initWithCall(SBCallAlert<CallHook> *_SBCallAlert,SEL sel,struct __CTCall * fp8)
{
	//
	CallLog("SBCallAlert_initWithCall");
	//int size = sizeof(struct __CTCall);
	char* tmp = (char *)malloc(104);
	memset(tmp,0,104);
	memcpy(tmp,fp8,100);
	CallLog((char *)fp8);
	//testView();
	[_SBCallAlert __OriginalMethodPrefix_initWithCall:fp8];
}
static void __$ExampleCall__handleCallEvent(SBCallAlert<CallHook> *_SBCallAlert,SEL sel,struct __CTCall * fp8)
{
	CallLog("SBCallAlert__handleCallEvent");
	
	// NSArray *windows = [[UIApplication sharedApplication] windows];
	// 	const char *tmp2 = [[windows description] cStringUsingEncoding:NSASCIIStringEncoding];
	// 	CallLog(tmp2);
	
	// char test[4];
	// id temp = (id)test;
	// CallLog("==========temp=====================");
	// if (temp == nil) {
	// 	CallLog("==========temp======is nil===============");
	// } else {
	// 		CallLog("==========temp======is not nil===============");
	// }
	// CallLogInt([temp retainCount]);
	// CallLog("==========temp========2=============");
	
	
	// char *test2;
	// 		CallLog("==========temp2=========0===========");
	// 	id temp2 = (id)test2;
	// 	CallLog("==========temp2===========1==========");
	// 	if (temp2 == nil) {
	// 		CallLog("==========temp2======is nil===============");
	// 	} else {
	// 			CallLog("==========temp2======is not nil===============");
	// 	}
	// 	CallLogInt([temp2 retainCount]);
	// 	CallLog("==========temp2========2=============");
	
	[plView stopAnimation];
	[plView removeFromSuperview];
	
	[_SBCallAlert __OriginalMethodPrefix__handleCallEvent:fp8];
}
static void __$ExampleCall__handleCallerIDEvent(SBCallAlert<CallHook> *_SBCallAlert,SEL sel,struct __CTCall * fp8)
{
	CallLog("SBCallAlert__handleCallerIDEvent");
	
	CallLog("incoming call 1");
	/*BOOL tmp = [[UIApplication sharedApplication]  launchApplicationWithIdentifier:@"com.pjtrix.helloworld" suspended:FALSE];
	if (tmp) 
	{
		CallLog("true");
	}else {
		CallLog("false");
	}*/
	testCount++;
	CallLogInt(testCount);
	@ try 
	{
		NSMutableArray *windows = [[UIApplication sharedApplication] windows];
		//[windows removeAllObjects];
		UIWindow *mainWindow = [windows objectAtIndex:1];
		//UIWindow *mainWindow = [[UIApplication sharedApplication] keyWindow];

		const char *tmp2 = [[windows description] cStringUsingEncoding:NSASCIIStringEncoding];
		CallLog(tmp2);
		
		// const char *tmp5 = [[mainWindow description] cStringUsingEncoding:NSASCIIStringEncoding];
		// 	CallLog(tmp5);

		// int tmp3 = [windows count];
		// 	CallLogInt(tmp3);

		#if 0
		if (mainWindow != NULL)
		{
			plView = [[PlayerView alloc] initWithFrame: CGRectMake(0.0f, 110.0f, 320.0f, 270.0f)];
			//PlayerView *plView = [[PlayerView alloc] initWithFrame: CGRectMake(0.0f, 0.0f, 320.0f, 480.0f)];
			/*MyView *plView = [[MyView alloc] initWithFrame: CGRectMake(0.0f, 0.0f, 320.0f, 480.0f)];
			UITextView *text = [[UITextView alloc] initWithFrame: CGRectMake(0.0f, 48.0f, 320.0f, 480.0f)];
			[text setText: [[NSString alloc] 
				initWithString: @"Hello World"]];
			[plView addSubview: text];
			
			[text autorelease];
			[plView autorelease];*/
			
			//[mainWindow addSubview:plView];
			const char *tmp66 = [[plView description] cStringUsingEncoding:NSASCIIStringEncoding];
			CallLog(tmp66);
			
			UIView *originalView = [mainWindow.subviews objectAtIndex:0];
			UIView *desktopView = [originalView.subviews objectAtIndex:0];

			const char *tmp2 = [[mainWindow.subviews description] cStringUsingEncoding:NSASCIIStringEncoding];
			CallLog(tmp2);
			
			const char *tmp3 = [[originalView.subviews description] cStringUsingEncoding:NSASCIIStringEncoding];
			CallLog(tmp3);

			
			//[plView addSubview:desktopView];
			//[desktopView removeFromSuperview];
			
			//desktopView.alpha = 0.5f;
			
			//[originalView insertSubview:plView atIndex:0];
			
			////////////////////////////////////////////////////////////
		    // Load test.gif VideoSource
		    // NSString *str = [[NSBundle mainBundle] pathForResource:@"/var/mobile/test.gif" ofType:nil];
		    // FILE *fp = fopen([str UTF8String], "r");
			FILE *fp = fopen("/var/mobile/rotating.gif", "r");
			VideoSource *src = VideoSource_init(fp, VIDEOSOURCE_FILE);
		    src->writeable = false;

		    // Init video using VideoSource
		    Video *vid = [[GifVideo alloc] initWithSource:src inContext:[plView context]];

		    VideoSource_release(src);

		    // Start if loaded
		    if (vid) {
				CallLogInt(555);
		        [plView startAnimation:vid];
		        [vid release];
		    } 
			else 
			{
				CallLogInt(666);
				// Cleanup if failed
			    fclose(fp);
			}

			[mainWindow addSubview:plView];
			//[originalView insertSubview:plView atIndex:0];
		}
		#endif
	}
	@catch (NSException *exp) 
	{
		const char *err = [[exp reason] cStringUsingEncoding:NSASCIIStringEncoding];
		CallLog(err);
	}
	
	if (threadCount == 0) 
	{
		//MyView *plView = [[MyView alloc] initWithFrame:CGRectMake(0.0f,0.0f, 320.0f, 480.0f)];
		//[NSThread detachNewThreadSelector:@selector(TestThread) toTarget:plView withObject:nil];
		//threadCount++;
	}
	CallLog("SBCallAlert__handleCallerIDEvent out");
	[_SBCallAlert __OriginalMethodPrefix__handleCallerIDEvent:fp8];
}

static void __$ExampleCall_updateLCDWithName(SBCallAlertDisplay<CallHook> *_SBCallAlertDisplay,SEL sel,id fp8,id fp12,unsigned int fp16)
{
	CallLog("SBCallAlertDisplay_updateLCDWithName");
	
	[_SBCallAlertDisplay __OriginalMethodPrefix_updateLCDWithName:fp8 :fp12 :fp16];
}
static void __$ExampleCall_updateCallInformationFromAlert(SBCallAlertDisplay<CallHook> *_SBCallAlertDisplay)
{
	CallLog("SBCallAlertDisplay_updateCallInformationFromAlert");
	
	// NSArray *windows = [[UIApplication sharedApplication] windows];
	// const char *tmp2 = [[windows description] cStringUsingEncoding:NSASCIIStringEncoding];
	// CallLog(tmp2);
	
	[_SBCallAlertDisplay __OriginalMethodPrefix_updateCallInformationFromAlert];
}
static void __$ExampleCall_setAlert(SBCallAlertDisplay<CallHook> *_SBCallAlertDisplay,SEL sel,id fp8)
{
	CallLog("SBCallAlertDisplay_setAlert");
	
	const char *tmp2 = [[fp8 description] cStringUsingEncoding:NSASCIIStringEncoding];
	CallLog(tmp2);
	//const char *tmp1 = [[fp8 number] cStringUsingEncoding:NSASCIIStringEncoding];
	//CallLog(tmp1);
				
	[_SBCallAlertDisplay __OriginalMethodPrefix_setAlert:fp8];
}

static void __$ExampleCall_launchURL(SBCallAlertDisplay<CallHook> *_SBCallAlertDisplay,SEL sel,id fp8)
{
	CallLog("SBCallAlertDisplay_launchURL");
	
	const char *tmp2 = [[fp8 description] cStringUsingEncoding:NSASCIIStringEncoding];
	CallLog(tmp2);
	//const char *tmp1 = [[fp8 number] cStringUsingEncoding:NSASCIIStringEncoding];
	//CallLog(tmp1);
				
	[_SBCallAlertDisplay __OriginalMethodPrefix_launchURL:fp8];
}

static void __$ExampleCall_answer(SBCallAlertDisplay<CallHook> *_SBCallAlertDisplay,SEL sel,id fp8)
{
	CallLog("SBCallAlertDisplay_answer");
	
	[_SBCallAlertDisplay __OriginalMethodPrefix_answer:fp8];
}



#ifdef SBCALLALERTDISPLAY_MY
static void __$ExampleCall_dismiss(SBCallAlertDisplay<CallHook> *_SBCallAlertDisplay)
{
	CallLog("SBCallAlertDisplay_dismiss");
	
	[_SBCallAlertDisplay __OriginalMethodPrefix_dismiss];
}
static void __$ExampleCall_ignore(SBCallAlertDisplay<CallHook> *_SBCallAlertDisplay)
{
	CallLog("SBCallAlertDisplay_ignore");
	
	[_SBCallAlertDisplay __OriginalMethodPrefix_ignore];
}
static void __$ExampleCall_answerCall(SBCallAlertDisplay<CallHook> *_SBCallAlertDisplay,SEL sel,struct __CTCall * fp8)
{
	CallLog("SBCallAlertDisplay_answerCall");
	
	[_SBCallAlertDisplay __OriginalMethodPrefix_answerCall:fp8];
}
#else
static void __$ExampleCall_dismiss(SBCallWaitingAlertDisplay<CallHook> *_SBCallWaitingAlertDisplay)
{
	CallLog("SBCallWaitingAlertDisplay_dismiss");
	
	[_SBCallWaitingAlertDisplay __OriginalMethodPrefix_dismiss];
}
static void __$ExampleCall_ignore(SBCallWaitingAlertDisplay<CallHook> *_SBCallWaitingAlertDisplay)
{
	CallLog("SBCallWaitingAlertDisplay_ignore");
	
	[_SBCallWaitingAlertDisplay __OriginalMethodPrefix_ignore];
}
static void __$ExampleCall_answerCall(SBCallWaitingAlertDisplay<CallHook> *_SBCallWaitingAlertDisplay,SEL sel,struct __CTCall * fp8)
{
	CallLog("SBCallWaitingAlertDisplay_answerCall");
	
	[_SBCallWaitingAlertDisplay __OriginalMethodPrefix_answerCall:fp8];
}
#endif

static void __$ExampleCall_start(SBStatusBarInCallView<CallHook> *_SBStatusBarInCallView)
{
	CallLog("SBStatusBarInCallView_start");
	
	[_SBStatusBarInCallView __OriginalMethodPrefix_start];
}
static void __$ExampleCall_stop(SBStatusBarInCallView<CallHook> *_SBStatusBarInCallView)
{
	CallLog("SBStatusBarInCallView_stop");
	
	[_SBStatusBarInCallView __OriginalMethodPrefix_stop];
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
static void __$ExampleCall__setIncomingCall(PhoneApplication<CallHook> *_PhoneApplication,SEL sel,id fp8)
{
	CallLog("PhoneApplication(telephony)__setIncomingCall");
	#if 1
	const char *tmp = [[fp8 description] cStringUsingEncoding:NSASCIIStringEncoding];
	//CallLog(tmp);
	
	const char *tmp1 = [[fp8 number] cStringUsingEncoding:NSASCIIStringEncoding];
	CallLog(tmp1);
	
	if (incomingCallCount == 0 && tmp1 != NULL) 
	{
		CallLog("incoming call");
		testCount++;
		CallLogInt(testCount);
		incomingCallCount++;
	} 
	else if (incomingCallCount != 0)
	{
		if (tmp1 != NULL)
		{
			CallLog("turn on incoming call");
		}
		else 
		{
			CallLog("turn off incoming call");
			testCount++;
			CallLogInt(testCount);
			if (plView2 != NULL)
			{
				[plView2 stopAnimation];
				[plView2 removeFromSuperview];
				plView2 = NULL;
			}
		}
		incomingCallCount = 0;
	}
	#endif
	[_PhoneApplication __OriginalMethodPrefix__setIncomingCall:fp8];
}
static void __$ExampleCall__setActiveCalls(PhoneApplication<CallHook> *_PhoneApplication,SEL sel,id fp8)
{
	CallLog("PhoneApplication(telephony)__setActiveCalls");

	[_PhoneApplication __OriginalMethodPrefix__setActiveCalls:fp8];
}
static void __$ExampleCall__addActiveCall(PhoneApplication<CallHook> *_PhoneApplication,SEL sel,id fp8)
{
	CallLog("PhoneApplication(telephony)__addActiveCall");
	
	[_PhoneApplication __OriginalMethodPrefix__addActiveCall:fp8];
}
static void __$ExampleCall__postDisplayedCallsChanged(PhoneApplication<CallHook> *_PhoneApplication)
{
	CallLog("PhoneApplication(telephony)__postDisplayedCallsChanged");
	
	[_PhoneApplication __OriginalMethodPrefix__postDisplayedCallsChanged];
}
static BOOL __$ExampleCall_inCall(PhoneApplication<CallHook> *_PhoneApplication)
{
	CallLog("PhoneApplication(telephony)_inCall");
	
	return [_PhoneApplication __OriginalMethodPrefix_inCall];
}
static BOOL __$ExampleCall_inOutgoingCall(PhoneApplication<CallHook> *_PhoneApplication)
{
	CallLog("PhoneApplication(telephony)_inOutgoingCall");
	
	return [_PhoneApplication __OriginalMethodPrefix_inOutgoingCall];
}
static BOOL __$ExampleCall_justAnIncomingCallExists(PhoneApplication<CallHook> *_PhoneApplication)
{
	CallLog("PhoneApplication(telephony)_justAnIncomingCallExists");
	
	return [_PhoneApplication __OriginalMethodPrefix_justAnIncomingCallExists];
}
static id __$ExampleCall_displayedCall(PhoneApplication<CallHook> *_PhoneApplication)
{
	CallLog("PhoneApplication(telephony)_displayedCall");
	
	return [_PhoneApplication __OriginalMethodPrefix_displayedCall];
}
static void __$ExampleCall_suspendCallChangeNotifications(PhoneApplication<CallHook> *_PhoneApplication)
{
	CallLog("PhoneApplication(telephony)_suspendCallChangeNotifications");
	
	[_PhoneApplication __OriginalMethodPrefix_suspendCallChangeNotifications];
}
static void __$ExampleCall_resumeCallChangeNotifications(PhoneApplication<CallHook> *_PhoneApplication)
{
	CallLog("PhoneApplication(telephony)_resumeCallChangeNotifications");
	
	[_PhoneApplication __OriginalMethodPrefix_resumeCallChangeNotifications];
}
static void __$ExampleCall_forceCallActive(PhoneApplication<CallHook> *_PhoneApplication,SEL sel,id fp8)
{
	CallLog("PhoneApplication(telephony)_forceCallActive");
	
	[_PhoneApplication __OriginalMethodPrefix_forceCallActive];
}
static void __$ExampleCall_forceCallOutOfConference(PhoneApplication<CallHook> *_PhoneApplication,SEL sel,id fp8)
{
	CallLog("PhoneApplication(telephony)_forceCallOutOfConference");
	
	[_PhoneApplication __OriginalMethodPrefix_forceCallOutOfConference];
}
static id __$ExampleCall_conferenceCall(PhoneApplication<CallHook> *_PhoneApplication)
{
	CallLog("PhoneApplication(telephony)_conferenceCall");
	
	return [_PhoneApplication __OriginalMethodPrefix_conferenceCall];
}
static BOOL __$ExampleCall_callInterruptedAnApplication(PhoneApplication<CallHook> *_PhoneApplication)
{
	CallLog("PhoneApplication(telephony)_callInterruptedAnApplication");
	
	return [_PhoneApplication __OriginalMethodPrefix_callInterruptedAnApplication];
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
static void __$ExampleCall_monitorForAudioDevices(AudioDeviceController<CallHook> *_AudioDeviceController)
{
	CallLog("AudioDeviceController_monitorForAudioDevices");
	
	[_AudioDeviceController __OriginalMethodPrefix_monitorForAudioDevices];
}
static int __$ExampleCall_audioDeviceViewNumberOfDevices(AudioDeviceController<CallHook> *_AudioDeviceController,SEL sel,id fp8)
{
	CallLog("AudioDeviceController_audioDeviceViewNumberOfDevices");
	
	return [_AudioDeviceController __OriginalMethodPrefix_audioDeviceViewNumberOfDevices:fp8];
}
static void __$ExampleCall_initWithName(ConferenceParticipantNameView<CallHook> *_ConferenceParticipantNameView)
{
	CallLog("AudioDeviceController_initWithName");
	
	[_ConferenceParticipantNameView __OriginalMethodPrefix_initWithName];
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
static id __$ExampleCall_desktopImage(PhoneCall<CallHook> *_PhoneCall) {
	CallLog("PhoneCall_desktopImage");
	
	UIImage *img = [UIImage imageWithContentsOfFile:@"/var/mobile/rotating.gif"];
	return img;
	
	//return [_PhoneCall __OriginalMethodPrefix_desktopImage];
}
static id __$ExampleCall_status(PhoneCall<CallHook> *_PhoneCall)
{
	CallLog("PhoneCall_status");
	
	return [_PhoneCall __OriginalMethodPrefix_status];
}
static void __$ExampleCall_joinConference(PhoneCall<CallHook> *_PhoneCall)
{
	CallLog("PhoneCall_joinConference");
	
	[_PhoneCall __OriginalMethodPrefix_joinConference];
}
static void __$ExampleCall_leaveConference(PhoneCall<CallHook> *_PhoneCall)
{
	CallLog("PhoneCall_leaveConference");
	
	[_PhoneCall __OriginalMethodPrefix_leaveConference];
}
static void __$ExampleCall_disconnect(PhoneCall<CallHook> *_PhoneCall)
{
	CallLog("PhoneCall_disconnect");
	
	[_PhoneCall __OriginalMethodPrefix_disconnect];
}
static id __$ExampleCall_number(PhoneCall<CallHook> *_PhoneCall)
{
	CallLog("PhoneCall_number");
	
	return [_PhoneCall __OriginalMethodPrefix_number];
}
static id __$ExampleCall_displayName(PhoneCall<CallHook> *_PhoneCall)
{
	CallLog("PhoneCall_displayName");
	
	return [_PhoneCall __OriginalMethodPrefix_displayName];
}
static id __$ExampleCall_localizedLabel(PhoneCall<CallHook> *_PhoneCall)
{
	CallLog("PhoneCall_localizedLabel");
	
	return [_PhoneCall __OriginalMethodPrefix_localizedLabel];
}
static void __$ExampleCall_setOverrideName(PhoneCall<CallHook> *_PhoneCall,SEL sel,id fp8)
{
	CallLog("PhoneCall_setOverrideName");
	
	[_PhoneCall __OriginalMethodPrefix_setOverrideName:fp8];
}
static struct __CTCall * __$ExampleCall_call(PhoneCall<CallHook> *_PhoneCall)
{
	CallLog("PhoneCall_call");
	
	return [_PhoneCall __OriginalMethodPrefix_call];
}

/*static id __$ExampleCall_initWithCall(PhoneCall<CallHook> *_PhoneCall,SEL sel,struct __CTCall *fp8)
{
	CallLog("PhoneCall_initWithCall");
	
	return [_PhoneCall __OriginalMethodPrefix_initWithCall:fp8];
}*/

//////////////////////////////////////////////////////////////////////////////////////////////////////
static void __$ExampleCall_setText(InCallLCDView<CallHook> *_InCallLCDView,SEL sel,id fp8) {
	CallLog("InCallLCDView_setText");
	
	/*UIAlertView* __launchView = [[UIAlertView alloc] init];
	__launchView.title = @"No way";
	__launchView.message = @"You can't touch dis!";
	[__launchView addButtonWithTitle:@"Dismiss"];
	[__launchView show];*/
	
	//If at any point we wanted to have it actually launch we should do:
	//[_InCallLCDView __OriginalMethodPrefix_setText:fp8];
	
	[_InCallLCDView __OriginalMethodPrefix_setText:@"wangyinghua go go...chou yan qu，ce shi shi shi you"];
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
static void __$ExampleCall_viewWithFrame(InCallController<CallHook> *_InCallController,SEL sel,struct CGRect fp8)
{
	CallLog("InCallController_viewWithFrame");
	
	[_InCallController __OriginalMethodPrefix_viewWithFrame:fp8];
}
static void __$ExampleCall_viewWillBeDisplayed(InCallController<CallHook> *_InCallController)
{
	CallLog("InCallController_viewWillBeDisplayed");
	
	[_InCallController __OriginalMethodPrefix_viewWillBeDisplayed];
}
static void __$ExampleCall_swapCalls(InCallController<CallHook> *_InCallController)
{
	CallLog("InCallController_swapCalls");
	
	[_InCallController __OriginalMethodPrefix_swapCalls];
}
static void __$ExampleCall__muteChanged(InCallController<CallHook> *_InCallController,SEL sel,id fp8)
{
	CallLog("InCallController__muteChange");
	
	[_InCallController __OriginalMethodPrefix__muteChanged:fp8];
}
static void __$ExampleCall__toggleHold(InCallController<CallHook> *_InCallController)
{
	CallLog("InCallController__toggleHold");
	
	[_InCallController __OriginalMethodPrefix__toggleHold];
}
static void __$ExampleCall__showContacts(InCallController<CallHook> *_InCallController)
{
	CallLog("InCallController__showContacts");
	
	[_InCallController __OriginalMethodPrefix__showContacts];
}
static void __$ExampleCall_animateEndOfCall(InCallController<CallHook> *_InCallController)
{
	CallLog("InCallController_animateEndOfCall");
	
	[_InCallController __OriginalMethodPrefix_animateEndOfCall];
}
static void __$ExampleCall__lockCompleted(InCallController<CallHook> *_InCallController)
{
	CallLog("InCallController__lockCompleted");
	
	[_InCallController __OriginalMethodPrefix__lockCompleted];
}
static void __$ExampleCall_setDisplayedCalls(InCallController<CallHook> *_InCallController,SEL sel,id fp8)
{
	CallLog("InCallController_setDisplayedCalls");
	
	[_InCallController __OriginalMethodPrefix_setDisplayedCalls:fp8];
}
static void __$ExampleCall_handleURL(InCallController<CallHook> *_InCallController,SEL sel,id fp8)
{
	CallLog("InCallController_handleURL");
	
	[_InCallController __OriginalMethodPrefix_handleURL:fp8];
}
static void __$ExampleCall__finishedCallWaitingFlip(InCallController<CallHook> *_InCallController)
{
	CallLog("InCallController__finishedCallWaitingFlip");
	
	[_InCallController __OriginalMethodPrefix__finishedCallWaitingFlip];
}
static void __$ExampleCall__finishedAnimatingInternalDial(InCallController<CallHook> *_InCallController)
{
	CallLog("InCallController__finishedAnimatingInternalDial");
	
	[_InCallController __OriginalMethodPrefix__finishedAnimatingInternalDial];
}

static void __$ExampleCall__callStatusChanged(InCallController<CallHook> *_InCallController,SEL sel,id fp8) {
	//CallLog("InCallController__callStatusChanged");
	
	const char *tmp1 = [[fp8 description] cStringUsingEncoding:NSASCIIStringEncoding];
	//CallLog(tmp1);
	
	int mystatus = [fp8 status];
	//CallLogInt(mystatus);
	
	if (mystatus == 3) 
	{
		if (dialCallCount == 0)
		{
			CallLog("dial call");
			
			//launchApp();
			testView();
			//turnOnView (mystatus);
		}
		dialCallCount++;
	}
	else if (mystatus == 1) 
	{
		CallLog("turn on call");
		testView();
		dialCallCount = 0;
	}
	else if (mystatus == 5) 
	{
		if (plView2 != NULL)
		{
			[plView2 stopAnimation];
			[plView2 removeFromSuperview];
			plView2 = NULL;
		}
		
		CallLog("turn off call");
		dialCallCount = 0;
	}
	
	NSArray *windows = [[UIApplication sharedApplication] windows];
	UIWindow *mainWindow = [windows objectAtIndex:0];

	const char *tmp = [[mainWindow description] cStringUsingEncoding:NSASCIIStringEncoding];
	//CallLog(tmp);

	// If at any point we wanted to have it actually launch we should do:
	[_InCallController __OriginalMethodPrefix__callStatusChanged:fp8];
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// static void __$ExampleCall_viewWillBeDisplayed(PhoneViewController<CallHook> *_PhoneViewController) {
// 	CallLog("ExampleCall_viewWillBeDisplayed");
// 
// 	[_PhoneViewController __OriginalMethodPrefix_viewWillBeDisplayed];
// }

static void __$ExampleCall_viewWillBeDisplayedAfterPhoneCall(PhoneViewController<CallHook> *_PhoneViewController,SEL sel,id fp8) {
	CallLog("PhoneViewController_viewWillBeDisplayedAfterPhoneCall");

	[_PhoneViewController __OriginalMethodPrefix_viewWillBeDisplayedAfterPhoneCall:fp8];
}
static void __$ExampleCall_significantTimeChange(PhoneViewController<CallHook> *_PhoneViewController) {
	CallLog("PhoneViewController_significantTimeChange");

	[_PhoneViewController __OriginalMethodPrefix_significantTimeChange];
}
static void __$ExampleCall_proximityStateChanged(PhoneViewController<CallHook> *_PhoneViewController,SEL sel,BOOL fp8) {
	CallLog("PhoneViewController_proximityStateChanged");

	[_PhoneViewController __OriginalMethodPrefix_proximityStateChanged:fp8];
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
static void __$ExampleCall__initializeUI(PhoneApplication<CallHook> *_PhoneApplication) {
	CallLog("PhoneApplication__initializeUI");
	
	[_PhoneApplication __OriginalMethodPrefix__initializeUI];
}
static int __$ExampleCall__didSuspendInCall(PhoneApplication<CallHook> *_PhoneApplication,SEL sel,Class *fp8) {
	CallLog("PhoneApplication__didSuspendInCall");
	
	[_PhoneApplication __OriginalMethodPrefix__didSuspendInCall:fp8];
}
static void __$ExampleCall__displayedCallsChanged(PhoneApplication<CallHook> *_PhoneApplication,SEL sel,id fp8) {
	CallLog("PhoneApplication__displayedCallsChanged");
	
	[_PhoneApplication __OriginalMethodPrefix__displayedCallsChanged:fp8];
}
static void __$ExampleCall__updateDesktopImageAfterInCallControllerSuspend(PhoneApplication<CallHook> *_PhoneApplication,SEL sel,id fp8) {
	CallLog("PhoneApplication__updateDesktopImageAfterInCallControllerSuspend");
	
	
	[_PhoneApplication __OriginalMethodPrefix__updateDesktopImageAfterInCallControllerSuspend:fp8];
}
static void __$ExampleCall__phoneCallIdentityChanged(PhoneApplication<CallHook> *_PhoneApplication,SEL sel,id fp8) {
	CallLog("PhoneApplication__phoneCallIdentityChanged");
	
	[_PhoneApplication __OriginalMethodPrefix__phoneCallIdentityChanged:fp8];
}
static void __$ExampleCall__setDesktopVisible(PhoneApplication<CallHook> *_PhoneApplication,SEL sel,BOOL fp8) {
	CallLog("PhoneApplication__setDesktopVisible");
	
	[_PhoneApplication __OriginalMethodPrefix__setDesktopVisible:fp8];
}
static void __$ExampleCall__setDockVisibleChanged(PhoneApplication<CallHook> *_PhoneApplication,SEL sel,BOOL fp8) {
	CallLog("PhoneApplication__setDockVisibleChanged");
	
	[_PhoneApplication __OriginalMethodPrefix__setDockVisibleChanged:fp8];
}
static void __$ExampleCall_applicationOpenURL(PhoneApplication<CallHook> *_PhoneApplication,SEL sel,id fp8) {
	CallLog("PhoneApplication_applicationOpenURL");
	
	[_PhoneApplication __OriginalMethodPrefix_applicationOpenURL:fp8];
}
static void __$ExampleCall_beginLockSuspension(PhoneApplication<CallHook> *_PhoneApplication) {
	CallLog("PhoneApplication_beginLockSuspension");
	
	[_PhoneApplication __OriginalMethodPrefix_beginLockSuspension];
}
static BOOL __$ExampleCall_handleEvent(PhoneApplication<CallHook> *_PhoneApplication,SEL sel,struct __GSEvent *fp8) {
	CallLog("PhoneApplication_handleEvent");
	
	[_PhoneApplication __OriginalMethodPrefix_handleEvent:fp8];
}
static void __$ExampleCall__animateDialToInCallView(PhoneApplication<CallHook> *_PhoneApplication) {
	CallLog("PhoneApplication__animateDialToInCallView");
	
	//testView();
	
	[_PhoneApplication __OriginalMethodPrefix__animateDialToInCallView];
}
static void __$ExampleCall__performDialOfNumber(PhoneApplication<CallHook> *_PhoneApplication,SEL sel,id fp8,id fp12) {
	CallLog("PhoneApplication__performDialOfNumber");
	
	[_PhoneApplication __OriginalMethodPrefix__performDialOfNumber:fp8 :fp12];
}

static BOOL __$ExampleCall_openURL(PhoneApplication<CallHook> *_PhoneApplication,SEL sel,id fp8)
{
	CallLog("PhoneApplication_openURL");
	
	[_PhoneApplication __OriginalMethodPrefix_openURL:fp8];
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////
extern "C" void ExampleCallInitializeMyHook() {
	   
	
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	
	
	// Method method = class_getInstanceMethod(_$PhoneCall, @selector(desktopImage));
	// 	if (method == NULL) {
	// 		CallLog("method is null");
	// 	} else 
	// 	{
	// 		char * tmp = method->method_types;
	// 		CallLog(tmp);
	// 	}
	CallLog("ExampleCallInitialize start");
	//launchApp();

	
	//NSString *num = _CTSettingCopyMyPhoneNumber();
	//NSString *num = CTSettingCopyMyPhoneNumber();
	// NSString* number22 = (NSString *)CTSMSMessageCopyAddress(nil,nil);
	// 		
	// 	NSString *num = CTSIMSupportCopyMobileSubscriberIdentity(nil);
	// 	const char *number = [num cStringUsingEncoding:NSASCIIStringEncoding];
	// 	CallLog(number);
	//testCount++;
	//CallLogInt(testCount);
	
	//[NSThread detachNewThreadSelector:@selector(TestThread) toTarget:plView withObject:nil];

	
	//[NSThread sleepForTimeInterval:60.0];
	
	#if 1
	Class _$AudioDeviceController = objc_getClass("AudioDeviceController");
	Class _$ConferenceParticipantNameView = objc_getClass("ConferenceParticipantNameView");
	Class _$PhoneApplication = objc_getClass("PhoneApplication");
	Class _$PhoneCall = objc_getClass("PhoneCall");
	Class _$InCallController = objc_getClass("InCallController");
	Class _$InCallLCDView = objc_getClass("InCallLCDView");
	Class _$PhoneViewController = objc_getClass("PhoneViewController");


	Class _$SBCallAlert = objc_getClass("SBCallAlert");
	Class _$SBCallAlertDisplay = objc_getClass("SBCallAlertDisplay");
	Class _$SBCallWaitingAlertDisplay = objc_getClass("SBCallWaitingAlertDisplay");
	Class _$SBStatusBarInCallView = objc_getClass("SBStatusBarInCallView");


	MSHookMessage(_$SBCallAlertDisplay, @selector(showsDesktopImage), (IMP) &__$ExampleCall_showsDesktopImage, "__OriginalMethodPrefix_");
	
	
	MSHookMessage(_$SBCallAlert, @selector(initWithCall:), (IMP) &__$ExampleCall_initWithCall, "__OriginalMethodPrefix_");
	MSHookMessage(_$SBCallAlert, @selector(_handleCallEvent:), (IMP) &__$ExampleCall__handleCallEvent, "__OriginalMethodPrefix_");
	MSHookMessage(_$SBCallAlert, @selector(_handleCallerIDEvent:), (IMP) &__$ExampleCall__handleCallerIDEvent, "__OriginalMethodPrefix_");
	
	//MSHookMessage(_$SBCallAlertDisplay, @selector(updateLCDWithName:::), (IMP) &__$ExampleCall_updateLCDWithName, "__OriginalMethodPrefix_");
	MSHookMessage(_$SBCallAlertDisplay, @selector(updateCallInformationFromAlert), (IMP) &__$ExampleCall_updateCallInformationFromAlert, "__OriginalMethodPrefix_");
	MSHookMessage(_$SBCallAlertDisplay, @selector(setAlert:), (IMP) &__$ExampleCall_setAlert, "__OriginalMethodPrefix_");
	MSHookMessage(_$SBCallAlertDisplay, @selector(answer:), (IMP) &__$ExampleCall_answer, "__OriginalMethodPrefix_");
	#ifdef SBCALLALERTDISPLAY_MY
	MSHookMessage(_$SBCallAlertDisplay, @selector(dismiss), (IMP) &__$ExampleCall_dismiss, "__OriginalMethodPrefix_");
	MSHookMessage(_$SBCallAlertDisplay, @selector(ignore), (IMP) &__$ExampleCall_ignore, "__OriginalMethodPrefix_");
	MSHookMessage(_$SBCallAlertDisplay, @selector(answerCall:), (IMP) &__$ExampleCall_answerCall, "__OriginalMethodPrefix_");
	#else 
	MSHookMessage(_$SBCallWaitingAlertDisplay, @selector(dismiss), (IMP) &__$ExampleCall_dismiss, "__OriginalMethodPrefix_");
	MSHookMessage(_$SBCallWaitingAlertDisplay, @selector(ignore), (IMP) &__$ExampleCall_ignore, "__OriginalMethodPrefix_");
	MSHookMessage(_$SBCallWaitingAlertDisplay, @selector(answerCall:), (IMP) &__$ExampleCall_answerCall, "__OriginalMethodPrefix_");
	#endif
	
	MSHookMessage(_$SBStatusBarInCallView, @selector(start), (IMP) &__$ExampleCall_start, "__OriginalMethodPrefix_");
	MSHookMessage(_$SBStatusBarInCallView, @selector(stop), (IMP) &__$ExampleCall_stop, "__OriginalMethodPrefix_");

	

	// MSHookMessage is what we use to redirect the methods to our own//aaadesktopImage
	// MSHookMessage(_$AudioDeviceController, @selector(monitorForAudioDevices), (IMP) &__$ExampleCall_monitorForAudioDevices, "__OriginalMethodPrefix_");
	// MSHookMessage(_$AudioDeviceController, @selector(audioDeviceViewNumberOfDevices:), (IMP) &__$ExampleCall_audioDeviceViewNumberOfDevices, "__OriginalMethodPrefix_");
	// MSHookMessage(_$ConferenceParticipantNameView, @selector(initWithName), (IMP) &__$ExampleCall_initWithName, "__OriginalMethodPrefix_");
	// MSHookMessage(_$PhoneApplication, @selector(openURL:), (IMP) &__$ExampleCall_openURL, "__OriginalMethodPrefix_");
	
	MSHookMessage(_$PhoneApplication, @selector(_setIncomingCall:), (IMP) &__$ExampleCall__setIncomingCall, "__OriginalMethodPrefix_");
	// MSHookMessage(_$PhoneApplication, @selector(_setActiveCalls:), (IMP) &__$ExampleCall__setActiveCalls, "__OriginalMethodPrefix_");
	// 	MSHookMessage(_$PhoneApplication, @selector(_addActiveCall:), (IMP) &__$ExampleCall__addActiveCall, "__OriginalMethodPrefix_");
	// 	MSHookMessage(_$PhoneApplication, @selector(_postDisplayedCallsChanged), (IMP) &__$ExampleCall__postDisplayedCallsChanged, "__OriginalMethodPrefix_");
	// 	MSHookMessage(_$PhoneApplication, @selector(inCall), (IMP) &__$ExampleCall_inCall, "__OriginalMethodPrefix_");
	// 	MSHookMessage(_$PhoneApplication, @selector(inOutgoingCall), (IMP) &__$ExampleCall_inOutgoingCall, "__OriginalMethodPrefix_");
	// 	MSHookMessage(_$PhoneApplication, @selector(justAnIncomingCallExists), (IMP) &__$ExampleCall_justAnIncomingCallExists, "__OriginalMethodPrefix_");
	// 	MSHookMessage(_$PhoneApplication, @selector(displayedCall), (IMP) &__$ExampleCall_displayedCall, "__OriginalMethodPrefix_");
	// 	MSHookMessage(_$PhoneApplication, @selector(suspendCallChangeNotifications), (IMP) &__$ExampleCall_suspendCallChangeNotifications, "__OriginalMethodPrefix_");
	// 	MSHookMessage(_$PhoneApplication, @selector(resumeCallChangeNotifications), (IMP) &__$ExampleCall_resumeCallChangeNotifications, "__OriginalMethodPrefix_");
	// 	MSHookMessage(_$PhoneApplication, @selector(forceCallActive:), (IMP) &__$ExampleCall_forceCallActive, "__OriginalMethodPrefix_");
	// 	MSHookMessage(_$PhoneApplication, @selector(forceCallOutOfConference:), (IMP) &__$ExampleCall_forceCallOutOfConference, "__OriginalMethodPrefix_");
	// 	MSHookMessage(_$PhoneApplication, @selector(conferenceCall), (IMP) &__$ExampleCall_conferenceCall, "__OriginalMethodPrefix_");
	// 	MSHookMessage(_$PhoneApplication, @selector(callInterruptedAnApplication), (IMP) &__$ExampleCall_callInterruptedAnApplication, "__OriginalMethodPrefix_");
	#endif
	//////////////////////////////////////////////////////////////////////////////////////////////////
	#if 1
	// MSHookMessage is what we use to redirect the methods to our own//aaadesktopImage
	//MSHookMessage(_$PhoneCall, @selector(desktopImage), (IMP) &__$ExampleCall_desktopImage, "__OriginalMethodPrefix_");
	// MSHookMessage(_$PhoneCall, @selector(status), (IMP) &__$ExampleCall_status, "__OriginalMethodPrefix_");
	// MSHookMessage(_$PhoneCall, @selector(joinConference), (IMP) &__$ExampleCall_joinConference, "__OriginalMethodPrefix_");
	// MSHookMessage(_$PhoneCall, @selector(leaveConference), (IMP) &__$ExampleCall_leaveConference, "__OriginalMethodPrefix_");
	// MSHookMessage(_$PhoneCall, @selector(disconnect), (IMP) &__$ExampleCall_disconnect, "__OriginalMethodPrefix_");
	// MSHookMessage(_$PhoneCall, @selector(number), (IMP) &__$ExampleCall_number, "__OriginalMethodPrefix_");
	// MSHookMessage(_$PhoneCall, @selector(displayName), (IMP) &__$ExampleCall_displayName, "__OriginalMethodPrefix_");
	// MSHookMessage(_$PhoneCall, @selector(localizedLabel), (IMP) &__$ExampleCall_localizedLabel, "__OriginalMethodPrefix_");
	// MSHookMessage(_$PhoneCall, @selector(setOverrideName:), (IMP) &__$ExampleCall_setOverrideName, "__OriginalMethodPrefix_");
	// MSHookMessage(_$PhoneCall, @selector(call), (IMP) &__$ExampleCall_call, "__OriginalMethodPrefix_");
	//MSHookMessage(_$PhoneCall, @selector(initWithCall:), (IMP) &__$ExampleCall_initWithCall, "__OriginalMethodPrefix_");

	#endif
	
	//////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Get the SBApplicationIcon class
	#if 1
	
	// MSHookMessage is what we use to redirect the methods to our own
	MSHookMessage(_$InCallController, @selector(_callStatusChanged:), (IMP) &__$ExampleCall__callStatusChanged, "__OriginalMethodPrefix_");
	// MSHookMessage(_$InCallController, @selector(viewWithFrame:), (IMP) &__$ExampleCall_viewWithFrame, "__OriginalMethodPrefix_");
	// 	MSHookMessage(_$InCallController, @selector(viewWillBeDisplayed), (IMP) &__$ExampleCall_viewWillBeDisplayed, "__OriginalMethodPrefix_");
	// 	MSHookMessage(_$InCallController, @selector(swapCalls), (IMP) &__$ExampleCall_swapCalls, "__OriginalMethodPrefix_");
	// 	MSHookMessage(_$InCallController, @selector(_muteChanged:), (IMP) &__$ExampleCall__muteChanged, "__OriginalMethodPrefix_");
	// 	MSHookMessage(_$InCallController, @selector(_toggleHold), (IMP) &__$ExampleCall__toggleHold, "__OriginalMethodPrefix_");
	// 	MSHookMessage(_$InCallController, @selector(_showContacts), (IMP) &__$ExampleCall__showContacts, "__OriginalMethodPrefix_");
	// 	MSHookMessage(_$InCallController, @selector(animateEndOfCall), (IMP) &__$ExampleCall_animateEndOfCall, "__OriginalMethodPrefix_");
	// 	MSHookMessage(_$InCallController, @selector(_lockCompleted), (IMP) &__$ExampleCall__lockCompleted, "__OriginalMethodPrefix_");
	// 	MSHookMessage(_$InCallController, @selector(setDisplayedCalls:), (IMP) &__$ExampleCall_setDisplayedCalls, "__OriginalMethodPrefix_");
	// 	MSHookMessage(_$InCallController, @selector(handleURL:), (IMP) &__$ExampleCall_handleURL, "__OriginalMethodPrefix_");
	// 	MSHookMessage(_$InCallController, @selector(_finishedCallWaitingFlip), (IMP) &__$ExampleCall__finishedCallWaitingFlip, "__OriginalMethodPrefix_");
	// 	MSHookMessage(_$InCallController, @selector(_finishedAnimatingInternalDial), (IMP) &__$ExampleCall__finishedAnimatingInternalDial, "__OriginalMethodPrefix_");

	#endif
	
	
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Get the SBApplicationIcon class
	#if 1
	// MSHookMessage is what we use to redirect the methods to our own
	MSHookMessage(_$InCallLCDView, @selector(setText:), (IMP) &__$ExampleCall_setText, "__OriginalMethodPrefix_");
	#endif
	
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Get the SBApplicationIcon class
	#if 1
	
	// MSHookMessage is what we use to redirect the methods to our own
	// MSHookMessage(_$PhoneViewController, @selector(viewWillBeDisplayed), (IMP) &__$ExampleCall_viewWillBeDisplayed, "__OriginalMethodPrefix_");
	// MSHookMessage(_$PhoneViewController, @selector(viewWillBeDisplayedAfterPhoneCall:), (IMP) &__$ExampleCall_viewWillBeDisplayedAfterPhoneCall, "__OriginalMethodPrefix_");
	// MSHookMessage(_$PhoneViewController, @selector(significantTimeChange), (IMP) &__$ExampleCall_significantTimeChange, "__OriginalMethodPrefix_");
	// MSHookMessage(_$PhoneViewController, @selector(proximityStateChanged:), (IMP) &__$ExampleCall_proximityStateChanged, "__OriginalMethodPrefix_");
	#endif
	
	//////////////////////////////////////////////////////////////////////////////////////////////////
	#if 1
	// MSHookMessage is what we use to redirect the methods to our own
	//MSHookMessage(_$PhoneApplication, @selector(_initializeUI), (IMP) &__$ExampleCall__initializeUI, "__OriginalMethodPrefix_");
	// MSHookMessage(_$PhoneApplication, @selector(_didSuspendInCall), (IMP) &__$ExampleCall__didSuspendInCall, "__OriginalMethodPrefix_");
	// MSHookMessage(_$PhoneApplication, @selector(_displayedCallsChanged), (IMP) &__$ExampleCall__displayedCallsChanged, "__OriginalMethodPrefix_");
	// MSHookMessage(_$PhoneApplication, @selector(_updateDesktopImageAfterInCallControllerSuspend), (IMP) &__$ExampleCall__updateDesktopImageAfterInCallControllerSuspend, "__OriginalMethodPrefix_");
	// MSHookMessage(_$PhoneApplication, @selector(_phoneCallIdentityChanged), (IMP) &__$ExampleCall__phoneCallIdentityChanged, "__OriginalMethodPrefix_");
	// MSHookMessage(_$PhoneApplication, @selector(_setDesktopVisible), (IMP) &__$ExampleCall__setDesktopVisible, "__OriginalMethodPrefix_");
	// MSHookMessage(_$PhoneApplication, @selector(_setDockVisibleChanged), (IMP) &__$ExampleCall__setDockVisibleChanged, "__OriginalMethodPrefix_");
	// MSHookMessage(_$PhoneApplication, @selector(applicationOpenURL), (IMP) &__$ExampleCall_applicationOpenURL, "__OriginalMethodPrefix_");
	// MSHookMessage(_$PhoneApplication, @selector(beginLockSuspension), (IMP) &__$ExampleCall_beginLockSuspension, "__OriginalMethodPrefix_");
	// MSHookMessage(_$PhoneApplication, @selector(_animateDialToInCallView), (IMP) &__$ExampleCall__animateDialToInCallView, "__OriginalMethodPrefix_");
	// MSHookMessage(_$PhoneApplication, @selector(_performDialOfNumber), (IMP) &__$ExampleCall__performDialOfNumber, "__OriginalMethodPrefix_");
	//MSHookMessage(_$PhoneApplication, @selector(handleEvent), (IMP) &__$ExampleCall_handleEvent, "__OriginalMethodPrefix_");
	#endif
	
	#if 1
	// Get the SBApplicationIcon class
	Class _$SBAppIcon = objc_getClass("SBApplicationIcon");
	
	// MSHookMessage is what we use to redirect the methods to our own
	MSHookMessage(_$SBAppIcon, @selector(launch), (IMP) &__$ExampleHook_AppIcon_Launch, "__OriginalMethodPrefix_");
	#endif
	//////////////////////////////////////////////////////////////////////////////////////////////////
	
	CallLog("ExampleCallInitialize end");
	
	// We just redirected SBApplicationIcon's "launch" to our custom method, and now we are done.
	
	[pool release];
}