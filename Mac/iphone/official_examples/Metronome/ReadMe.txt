
### Metronome ###

===========================================================================
DESCRIPTION:
Metronome sample application demonstrates how to animate a rotation, interpret touch events, and display a simple application preferences view. Open the Metronome project in Xcode and build and run it. To run in the simulator, set the Active SDK to Simulator. To run on a device, set the Active SDK to the appropriate Device setting.

The metronome presents a simple interface. Drag the arm horizontally to start the arm. Tap to stop. Drag the arm vertically to change the tempo. Tap the "i" button to access application preferences.

===========================================================================
BUILD REQUIREMENTS:

Mac OS X 10.5.3, Xcode 3.1, iPhone OS 2.0

===========================================================================
RUNTIME REQUIREMENTS:

Mac OS X 10.5.3, iPhone OS 2.0

===========================================================================
PACKAGING LIST:

MetronomeAppDelegate.h
MetronomeAppDelegate.m
UIApplication's delegate. On start up, this object receives the applicationDidFinishLaunching: delegate message and creates an instance of RootViewController. The RootViewController in turn creates an instance of MetronomeViewController and installs its view (the front side of the application) as a subview of the application's window. The MetronomeAppDelegate is also in charge of saving and restoring the user's time signature preference in user defaults.

RootViewController.h
RootViewController.m
Top-level view controller in the Metronome application. It causes the primary UI of the Metronome application to be loaded and responds to user actions to flip from the primary interface to the preferences interface and back. It also keeps this application from rotating in response to device orientation changes.

MetronomeViewController.h
MetronomeViewController.m
Controls the Metronome view, which is loaded from MetronomeView.nib. It also installs the "i" button, which displays the application preferences.

MetronomeView.h
MetronomeView.m
This view builds the primary application UI. It also implements the touches... methods that respond to user interaction with that display. MetronomeView animates the swing of the arm and updates that animation to reflect changes in tempo, time signature, and sound settings.

PreferencesViewController.h
PreferencesViewController.m
Controls the Preferences View, which is loaded from PreferencesView.nib. It also installs the UINavigationBar that displays the application name and Done button.

PreferencesView.h
PreferencesView.m
This view builds and displays the application preferences UI. In preferences, you can set the sound volume and time signature preferences.

SoundEffect.h
SoundEffect.m
A simple Objective-C wrapper around Audio Services functions that allow the loading and playing of sound files. 

MainWindow.xib
Interface Builder 'nib' file that provides the window for the application.

MetronomeView.xib
Nib file that provides the view for the front side of the application.

PreferencesView.xib
Nib file that provides the view for the preferences side of the application.

main.m
Entry point for the application. Creates the application object and causes the event loop to start.

===========================================================================
CHANGES FROM PREVIOUS VERSIONS:

Version 1.7
- Updated for and tested with iPhone OS 2.0. First public release.

Version 1.6
- Updated for Beta 7.
- Made nib files localizable
- Fixed a problem that made it difficult at times for the 'i' button to register touches.

Version 1.5
- Updated for Beta 6.
- Added LSRequiresIPhoneOS setting to Info.plist
- Time signature tableview now shows selected time signature using a check mark (UITableViewCellAccessoryCheckmark)

Version 1.4
- Updated for Beta 5. 
- Converted sample to use Xcode's Cocoa Touch Utility project template.

Version 1.3
- Updated for Beta 4. 
- For sound playback, replaced AudioFX class with simpler SoundEffect class.

Version 1.2
- Updated for Beta 3. 
- Replaced the ReadMe file with a plain text version.

Version 1.1:
- Improved artwork
- Improved handling of user preferences
- Simplified loading of sound files
- 'Tock' sound now occurs on first beat of measure
- Much improved vertical and horizontal dragging behavior. 
- Simplified code that supports dragging behavior.
- Improved implementation of sound thread.
- General code clean-up

===========================================================================
Copyright (C) 2008 Apple Inc. All rights reserved.