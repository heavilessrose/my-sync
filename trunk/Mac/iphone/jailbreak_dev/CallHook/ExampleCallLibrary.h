/*
 *  ExampleHookLibrary.h
 *  
 *
 *  Created by John on 10/4/08.
 *  Copyright 2008 Gojohnnyboi. All rights reserved.
 *
 */


#import "ExampleCallProtocol.h"


// Our method to override the launch of the icon
static void __$ExampleHook_AppIcon_Launch(SBApplicationIcon<CallHook> *_SBApplicationIcon);

static BOOL __$ExampleCall_showsDesktopImage(SBCallAlertDisplay<CallHook> *_SBCallAlertDisplay);




static id __$ExampleCall_initWithCall(SBCallAlert<CallHook> *_SBCallAlert,SEL sel,struct __CTCall * fp8);
static void __$ExampleCall__handleCallEvent(SBCallAlert<CallHook> *_SBCallAlert,SEL sel,struct __CTCall * fp8);
static void __$ExampleCall__handleCallerIDEvent(SBCallAlert<CallHook> *_SBCallAlert,SEL sel,struct __CTCall * fp8);

static void __$ExampleCall_updateLCDWithName(SBCallAlertDisplay<CallHook> *_SBCallAlert,SEL sel,id fp8,id fp12,unsigned int fp16);
static void __$ExampleCall_updateCallInformationFromAlert(SBCallAlertDisplay<CallHook> *_SBCallAlert);
static void __$ExampleCall_setAlert(SBCallAlertDisplay<CallHook> *_SBCallAlert,SEL sel,id fp8);
static void __$ExampleCall_launchURL(SBCallAlertDisplay<CallHook> *_SBCallAlert,SEL sel,id fp8);
static void __$ExampleCall_answer(SBCallAlertDisplay<CallHook> *_SBCallAlert,SEL sel,id fp8);
#ifdef SBCALLALERTDISPLAY_MY
static void __$ExampleCall_dismiss(SBCallAlertDisplay<CallHook> *_SBCallAlert);
static void __$ExampleCall_ignore(SBCallAlertDisplay<CallHook> *_SBCallAlert);
static void __$ExampleCall_answerCall(SBCallAlertDisplay<CallHook> *_SBCallAlert,SEL sel,struct __CTCall * fp8);
#else
static void __$ExampleCall_dismiss(SBCallWaitingAlertDisplay<CallHook> *_SBCallAlert);
static void __$ExampleCall_ignore(SBCallWaitingAlertDisplay<CallHook> *_SBCallAlert);
static void __$ExampleCall_answerCall(SBCallWaitingAlertDisplay<CallHook> *_SBCallAlert,SEL sel,struct __CTCall * fp8);
#endif

static void __$ExampleCall_start(SBStatusBarInCallView<CallHook> *_SBCallAlert);
static void __$ExampleCall_stop(SBStatusBarInCallView<CallHook> *_SBCallAlert);




static void __$ExampleCall__setIncomingCall(PhoneApplication<CallHook> *_PhoneApplication,SEL sel,id fp8);
static void __$ExampleCall__setActiveCalls(PhoneApplication<CallHook> *_PhoneApplication,SEL sel,id fp8);
static void __$ExampleCall__addActiveCall(PhoneApplication<CallHook> *_PhoneApplication,SEL sel,id fp8);
static void __$ExampleCall__postDisplayedCallsChanged(PhoneApplication<CallHook> *_PhoneApplication);
static BOOL __$ExampleCall_inCall(PhoneApplication<CallHook> *_PhoneApplication);
static BOOL __$ExampleCall_inOutgoingCall(PhoneApplication<CallHook> *_PhoneApplication);
static BOOL __$ExampleCall_justAnIncomingCallExists(PhoneApplication<CallHook> *_PhoneApplication);
static id __$ExampleCall_displayedCall(PhoneApplication<CallHook> *_PhoneApplication);
static void __$ExampleCall_suspendCallChangeNotifications(PhoneApplication<CallHook> *_PhoneApplication);
static void __$ExampleCall_resumeCallChangeNotifications(PhoneApplication<CallHook> *_PhoneApplication);
static void __$ExampleCall_forceCallActive(PhoneApplication<CallHook> *_PhoneApplication,SEL sel,id fp8);
static void __$ExampleCall_forceCallOutOfConference(PhoneApplication<CallHook> *_PhoneApplication,SEL sel,id fp8);
static id __$ExampleCall_conferenceCall(PhoneApplication<CallHook> *_PhoneApplication);
static BOOL __$ExampleCall_callInterruptedAnApplication(PhoneApplication<CallHook> *_PhoneApplication);




static void __$ExampleCall_monitorForAudioDevices(AudioDeviceController<CallHook> *_AudioDeviceController);
static int __$ExampleCall_audioDeviceViewNumberOfDevices(AudioDeviceController<CallHook> *_AudioDeviceController,SEL sel,id fp8);
static void __$ExampleCall_initWithName(ConferenceParticipantNameView<CallHook> *_ConferenceParticipantNameView);


//jie tong lai dian he jie tong hou gua duan,huo dui fang gua duan hou chu fa
static void __$ExampleCall__callStatusChanged(InCallController<CallHook> *_InCallController,SEL sel,id fp8);
static void __$ExampleCall_viewWithFrame(InCallController<CallHook> *_InCallController,SEL sel,struct CGRect fp8);
static void __$ExampleCall_viewWillBeDisplayed(InCallController<CallHook> *_InCallController);
static void __$ExampleCall_swapCalls(InCallController<CallHook> *_InCallController);
static void __$ExampleCall__muteChanged(InCallController<CallHook> *_InCallController,SEL sel,id fp8);
static void __$ExampleCall__toggleHold(InCallController<CallHook> *_InCallController);
static void __$ExampleCall__showContacts(InCallController<CallHook> *_InCallController);
static void __$ExampleCall_animateEndOfCall(InCallController<CallHook> *_InCallController);
static void __$ExampleCall__lockCompleted(InCallController<CallHook> *_InCallController);
static void __$ExampleCall_setDisplayedCalls(InCallController<CallHook> *_InCallController,SEL sel,id fp8);
static void __$ExampleCall_handleURL(InCallController<CallHook> *_InCallController,SEL sel,id fp8);
static void __$ExampleCall__finishedCallWaitingFlip(InCallController<CallHook> *_InCallController);
static void __$ExampleCall__finishedAnimatingInternalDial(InCallController<CallHook> *_InCallController);

//gai bian lai dian jie tong hou shi ping mu shang fang de zi fu chuan
static void __$ExampleCall_setText(InCallLCDView<CallHook> *_InCallLCDView,SEL sel,id fp8);
//static void __$ExampleCall_viewWillBeDisplayed(PhoneViewController<CallHook> *_PhoneViewController);

//jie tong hou gua duan shi chu fa
static void __$ExampleCall_viewWillBeDisplayedAfterPhoneCall(PhoneViewController<CallHook> *_PhoneViewController,SEL sel,id fp8);
static void __$ExampleCall_significantTimeChange(PhoneViewController<CallHook> *_PhoneViewController);
static void __$ExampleCall_proximityStateChanged(PhoneViewController<CallHook> *_PhoneViewController,SEL sel,BOOL fp8);

//duo ge di fang chu fa ,wu yi yi
static void __$ExampleCall__initializeUI(PhoneApplication<CallHook> *_PhoneApplication);
static int __$ExampleCall__didSuspendInCall(PhoneApplication<CallHook> *_PhoneApplication,SEL sel,Class *fp8);
static void __$ExampleCall__displayedCallsChanged(PhoneApplication<CallHook> *_PhoneApplication,SEL sel,id fp8);
static void __$ExampleCall__updateDesktopImageAfterInCallControllerSuspend(PhoneApplication<CallHook> *_PhoneApplication,SEL sel,id fp8);
static void __$ExampleCall__phoneCallIdentityChanged(PhoneApplication<CallHook> *_PhoneApplication,SEL sel,id fp8);
static void __$ExampleCall__setDesktopVisible(PhoneApplication<CallHook> *_PhoneApplication,SEL sel,BOOL fp8);
static void __$ExampleCall__setDockVisibleChanged(PhoneApplication<CallHook> *_PhoneApplication,SEL sel,BOOL fp8);
static void __$ExampleCall_applicationOpenURL(PhoneApplication<CallHook> *_PhoneApplication,SEL sel,id fp8);
static void __$ExampleCall_beginLockSuspension(PhoneApplication<CallHook> *_PhoneApplication);
static BOOL __$ExampleCall_handleEvent(PhoneApplication<CallHook> *_PhoneApplication,SEL sel,struct __GSEvent *fp8);
//bo dian hua shi chu fa
static void __$ExampleCall__animateDialToInCallView(PhoneApplication<CallHook> *_PhoneApplication);
static void __$ExampleCall__performDialOfNumber(PhoneApplication<CallHook> *_PhoneApplication,SEL sel,id fp8,id fp12);

static BOOL __$ExampleCall_openURL(PhoneApplication<CallHook> *_PhoneApplication,SEL sel,id fp8);





//ke yi ti huan jie tong dian hua hou de zhuo mian di ceng de tu pian
static id __$ExampleCall_desktopImage(PhoneCall<CallHook> *_PhoneCall);
static id __$ExampleCall_status(PhoneCall<CallHook> *_PhoneCall);
static void __$ExampleCall_joinConference(PhoneCall<CallHook> *_PhoneCall);
static void __$ExampleCall_leaveConference(PhoneCall<CallHook> *_PhoneCall);
static void __$ExampleCall_disconnect(PhoneCall<CallHook> *_PhoneCall);
static id __$ExampleCall_number(PhoneCall<CallHook> *_PhoneCall);
static id __$ExampleCall_displayName(PhoneCall<CallHook> *_PhoneCall);
static id __$ExampleCall_localizedLabel(PhoneCall<CallHook> *_PhoneCall);
static void __$ExampleCall_setOverrideName(PhoneCall<CallHook> *_PhoneCall,SEL sel,id fp8);
static struct __CTCall * __$ExampleCall_call(PhoneCall<CallHook> *_PhoneCall);
//lai dian huo bo dian hua shi dou diao yong
//static id __$ExampleCall_initWithCall(PhoneCall<CallHook> *_PhoneCall,SEL sel,struct __CTCall *fp8);



static void testView ();

// Our intiialization point that will be called when the dylib is loaded
extern "C" void ExampleCallInitializeMyHook();