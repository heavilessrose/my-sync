### ViewTransitions ###

================================================================================
DESCRIPTION:
The ViewTransitions sample application demonstrates how to perform transitions between two views using built-in Core Animation transitions. By looking at the code, you'll see how to use a CATransition object to set up and control transitions.

To try out the sample, build it using Xcode and run it in the simulator or on the device. Click the 'Transition' button to perform a transition from one image to another. The application choses a transition effect at random.

================================================================================
BUILD REQUIREMENTS:

Mac OS X 10.5.3, Xcode 3.1, iPhone OS 2.0

================================================================================
RUNTIME REQUIREMENTS:

Mac OS X 10.5.3, iPhone OS 2.0

================================================================================
PACKAGING LIST:

AppDelegate.h
AppDelegate.m
The UIApplication delegate class, the central controller of the application.

TransitionView.h
TransitionView.m
A class that animates the replacement of one view with another view using CoreAnimation transitions.

main.m
The main entry point for the ViewTransitions application.

================================================================================
CHANGES FROM PREVIOUS VERSIONS:

Version 1.6
- Updated for and tested with iPhone OS 2.0. First public release.

Version 1.5
- Updated for Beta 6.
- Added LSRequiresIPhoneOS key to Info.plist

Version 1.4
- Updated for Beta 5.
- Added a button to start the transition.
- Updated to use a nib file.

Version 1.3
- Updated for Beta 4.
- Added code signing.

Version 1.2
- Updated for Beta 3.
- Added icon.

Version 1.1
- Updated for Beta 2.

================================================================================
Copyright (C) 2008 Apple Inc. All rights reserved.