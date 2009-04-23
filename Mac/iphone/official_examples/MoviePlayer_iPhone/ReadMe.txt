
### MoviePlayer ###

===========================================================================
DESCRIPTION:

Demonstrates how to use the Media Player Framework to play a movie full-screen. The sample contains code to configure the movie background color, playback controls and scaling mode via the built-in Settings application. Also shows how to draw custom overlay controls on top of the movie during playback. 

The Media Player framework provides basic facilities for playing movie files. Within the Media Player framework, a MPMoviePlayerController object defines a full-screen movie player. You can use this class to play back movies stored in your application’s bundle directory or support directories. You can also use it to play movies loaded from a network-based URL.

After the application launches, press the Play Movie button (or touch anywhere in the movie image) and the included movie file will play full-screen. When launched, notice the movie background color, playback controls and scaling mode. Touch anywhere on the movie and the the movie playback controls will show (unless you've turned them off via the "Settings" application). Also notice the custom overlay controls that are drawn on top of the movie. Touches anywhere on the overlay controls are handled specifically by the application, not by the movie player. Quit the application and launch the built-in "Settings" application. At the end of the settings list you will find a section for "MoviePlayer". Open it and from there you can set the background color, playback controls and scaling mode. Quit Settings and return to MoviePlayer. Press the Play Movie button again and notice the above settings have changed. 

This sample offers an Xcode project already pre-configured to build your Settings bundle as a target. To customize your settings UI, change the Root.plist file.

===========================================================================
BUILD REQUIREMENTS:

Mac OS X 10.5.3, Xcode 3.1, iPhone OS 2.0

===========================================================================
RUNTIME REQUIREMENTS:

Mac OS X 10.5.3, iPhone OS 2.0

===========================================================================
PACKAGING LIST:

MoviePlayerAppDelegate.h
MoviePlayerAppDelegate.m
A simple UIApplication delegate class that adds the MyViewController view to the window as a subview. Also sets the application settings based on the defaults in the application bundle.

MyViewController.h
MyViewController.m
A UIViewController controller subclass that loads the MoviePlayer nib file that contains its view. Instantiates a MPMoviePlayerController object, defines notification routines that are called when the movie preroll has completed, when the movie has finished playing and when the movie scaling mode is changed. Contains an action method that is called when the Play Movie button is pressed to play the movie. Updates and saves the user settings for the currently set values. Overrides the inherited shouldAutorotateToInterfaceOrientation: method so that the view can respond to device rotation.

MyImageView.h
MyImageView.m
A UIImageView subclass that implements the UIResponder event-handling method touchesBegan: withEvent: in order to receive finger touch event messages. Any finger touch events in this view will start the movie playing.

MyOverlayView.h
MyOverlayView.m
A UIView subclass that is added as a subview to the movie player window. This view and its associated controls will display on top of the movie window and receive any touch events while the movie plays underneath it.

MainWindow.xib
Interface Builder 'nib' file that defines the interface for the application: the main window, the view and other user interface items. Also contains the movie file used for playback, and the plist files defining the application movie playback settings.

main.m
Entry point for the application. Creates the application object and causes the event loop to start.

Root.plist
The scheme file for the settings bundle.

Movie.m4v
The movie file to be played.

===========================================================================
CHANGES FROM PREVIOUS VERSIONS:

1.0 - First Release
1.1 - Updated for and tested with iPhone OS 2.0. First public release.
1.2 - Added custom overlay controls that draw on top of the movie.
1.2.1 - Fixed dealloc method in MyViewController.m 


===========================================================================
Copyright (C) 2008 Apple Inc. All rights reserved.