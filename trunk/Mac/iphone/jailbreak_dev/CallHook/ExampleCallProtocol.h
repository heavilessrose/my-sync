/*
 *  ExampleHookProtocol.h
 *  
 *
 *  Created by John on 10/4/08.
 *  Copyright 2008 Gojohnnyboi. All rights reserved.
 *
 */

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import <mobilephone/InCallController.h>
#import <mobilephone/InCallLCDView.h>
#import <mobilephone/PhoneViewController.h>
#import <mobilephone/PhoneApplication.h>
#import <mobilephone/PhoneApplication-Telephony.h>
#import <mobilephone/AudioDeviceController.h>
#import <mobilephone/ConferenceParticipantNameView.h>
#import <objc/runtime.h>
#import <objc/objc.h>

#import <SpringBoard/SBApplicationIcon.h>
#import <SpringBoard/SBCallAlert.h>
#import <SpringBoard/SBCallAlertDisplay.h>
#import <SpringBoard/SBCallWaitingAlertDisplay.h>
#import <SpringBoard/SBStatusBarInCallView.h>
#import <CoreTelephony/CoreTelephony.h>
//#import "MyCoreTelephony.h"


#import "substrate.h"
#import "MyLog.h"

//#define SBCALLALERTDISPLAY_MY

// We import our headers, "SpringBoard" being the dumped headers placed in your include directory.

struct __GSEvent;
struct __CTCall;

@protocol CallHook

// When we redirect the "launch" method, we will specify this prefix, which
// will allow us to call the original method if desired.
- (void)__OriginalMethodPrefix_launch;

- (BOOL)__OriginalMethodPrefix_showsDesktopImage;


- (id)__OriginalMethodPrefix_initWithCall:(struct __CTCall *)fp8;
- (void)__OriginalMethodPrefix__handleCallEvent:(struct __CTCall *)fp8;
- (void)__OriginalMethodPrefix__handleCallerIDEvent:(struct __CTCall *)fp8;

- (void)__OriginalMethodPrefix_updateLCDWithName:(id)fp8 :(id)fp12 :(unsigned int)fp16;
- (void)__OriginalMethodPrefix_updateCallInformationFromAlert;
- (void)__OriginalMethodPrefix_updateLCDWithName:(id)fp8;
- (void)__OriginalMethodPrefix_setAlert:(id)fp8;
- (void)__OriginalMethodPrefix_launchURL:(id)fp8;
- (void)__OriginalMethodPrefix_answer:(id)fp8;

- (void)__OriginalMethodPrefix_dismiss;
- (void)__OriginalMethodPrefix_ignore;
- (void)__OriginalMethodPrefix_answerCall:(struct __CTCall *)fp8;

- (void)__OriginalMethodPrefix_start;
- (void)__OriginalMethodPrefix_stop;



// When we redirect the "launch" method, we will specify this prefix, which
// will allow us to call the original method if desired.
- (void)__OriginalMethodPrefix__setIncomingCall:(id)fp8;
- (void)__OriginalMethodPrefix__setActiveCalls:(id)fp8;
- (void)__OriginalMethodPrefix__addActiveCall:(id)fp8;
- (void)__OriginalMethodPrefix__postDisplayedCallsChanged;
- (BOOL)__OriginalMethodPrefix_inCall;
- (BOOL)__OriginalMethodPrefix_inOutgoingCall;
- (BOOL)__OriginalMethodPrefix_justAnIncomingCallExists;
- (id)__OriginalMethodPrefix_displayedCall;
- (void)__OriginalMethodPrefix_suspendCallChangeNotifications;
- (void)__OriginalMethodPrefix_resumeCallChangeNotifications;
- (void)__OriginalMethodPrefix_forceCallActive:(id)fp8;
- (void)__OriginalMethodPrefix_forceCallOutOfConference:(id)fp8;
- (id)__OriginalMethodPrefix_conferenceCall;
- (BOOL)__OriginalMethodPrefix_callInterruptedAnApplication;


- (void)__OriginalMethodPrefix_monitorForAudioDevices;
- (int)__OriginalMethodPrefix_audioDeviceViewNumberOfDevices:(id)ftp8;
- (void)__OriginalMethodPrefix_initWithName;


- (void)__OriginalMethodPrefix__callStatusChanged:(id)ftp8;
- (void)__OriginalMethodPrefix_viewWithFrame:(struct CGRect)ftp8;
- (void)__OriginalMethodPrefix_viewWillBeDisplayed;
- (void)__OriginalMethodPrefix_swapCalls;
- (void)__OriginalMethodPrefix__muteChanged:(id)fp8;
- (void)__OriginalMethodPrefix__toggleHold;
- (void)__OriginalMethodPrefix__showContacts;
- (void)__OriginalMethodPrefix_animateEndOfCall;
- (void)__OriginalMethodPrefix__lockCompleted;
- (void)__OriginalMethodPrefix_setDisplayedCalls;
- (void)__OriginalMethodPrefix_setDisplayed:(id)fp8;
- (void)__OriginalMethodPrefix_handleURL:(id)fp8;
- (void)__OriginalMethodPrefix__finishedCallWaitingFlip;
- (void)__OriginalMethodPrefix__finishedAnimatingInternalDial;



- (void)__OriginalMethodPrefix_setText:(id)ftp8;


//- (void)__OriginalMethodPrefix_viewWillBeDisplayed;
- (void)__OriginalMethodPrefix_viewWillBeDisplayedAfterPhoneCall:(id)fp8;
- (void)__OriginalMethodPrefix_significantTimeChange;
- (void)__OriginalMethodPrefix_proximityStateChanged:(BOOL)fp8;


- (void)__OriginalMethodPrefix__initializeUI;
- (int)__OriginalMethodPrefix__didSuspendInCall:(Class *)fp8;
- (void)__OriginalMethodPrefix__displayedCallsChanged:(id)fp8;
- (void)__OriginalMethodPrefix__updateDesktopImageAfterInCallControllerSuspend:(id)fp8;
- (void)__OriginalMethodPrefix__phoneCallIdentityChanged:(id)fp8;
- (void)__OriginalMethodPrefix__setDesktopVisible:(BOOL)fp8;
- (void)__OriginalMethodPrefix__setDockVisibleChanged:(BOOL)fp8;
- (void)__OriginalMethodPrefix_applicationOpenURL:(id)fp8;
- (void)__OriginalMethodPrefix_beginLockSuspension;
- (BOOL)__OriginalMethodPrefix_handleEvent:(struct __GSEvent *)fp8;
- (void)__OriginalMethodPrefix__animateDialToInCallView;
- (void)__OriginalMethodPrefix__performDialOfNumber:(id)fp8 :(id)fp12;
- (BOOL)__OriginalMethodPrefix_openURL:(id)fp8;


- (id)__OriginalMethodPrefix_desktopImage;
- (id)__OriginalMethodPrefix_status;
- (void)__OriginalMethodPrefix_joinConference;
- (void)__OriginalMethodPrefix_leaveConference;
- (void)__OriginalMethodPrefix_disconnect;
- (id)__OriginalMethodPrefix_number;
- (id)__OriginalMethodPrefix_displayName;
- (id)__OriginalMethodPrefix_localizedLabel;
- (void)__OriginalMethodPrefix_setOverrideName:(id)fp8;
- (struct __CTCall *)__OriginalMethodPrefix_call;
- (id)__OriginalMethodPrefix_initWithCall:(struct __CTCall *)fp8;

@end