### GLPaint ###

================================================================================
DESCRIPTION:

The GLPaint sample application demonstrates how to support single-finger painting using OpenGL ES. This sample also shows how to detect a "shake" motion of the device using the UIAccelerometer class.

By looking at the code you'll see how to set up an OpenGL ES view and use it for rendering painting strokes. The application creates a brush texture from an image by first drawing the image into a Core Graphics bitmap context. It then uses the bitmap data for the texture. The image used for a texture must have dimensions that are a power of 2. 

To use this sample, open it in Xcode and click Build and Go. After the application paints "Shake Me", shake the device to erase the words. Touch  a color to choose it. Paint by dragging a finger.

NOTE: If you run the device in the simulator, you won't be able to erase. You will, however, be able to paint.

================================================================================
BUILD REQUIREMENTS:

Mac OS X v10.5.3, Xcode 3.1, iPhone OS 2.0

================================================================================
RUNTIME REQUIREMENTS:

Mac OS X v10.5.3, iPhone OS 2.0

================================================================================
PACKAGING LIST:

AppController.h
AppController.m
UIApplication's delegate class i.e. the central controller of the application.

PaintingView.h
PaintingView.m
The class responsible for the finger painting.

SoundEffect.h
SoundEffect.m
A simple Objective-C wrapper around Audio Services functions that allow the loading and playing of sound files.

EAGLView.h
EAGLView.m
Convenience class that wraps the CAEAGLLayer from CoreAnimation into a UIView subclass.

main.m
The main entry point for the GLPaint application.

Recording.data
Contains the path used to display "Shake Me" after the application launches.

Particle.png
The texture used for the paint brush.

================================================================================
CHANGES FROM PREVIOUS VERSIONS:

Version 1.6
Updated for and tested with iPhone OS 2.0. First public release.

Version 1.5
Minor changes to the comments.
There are no code changes in this version.

Version 1.4
Updated for Beta 6.
Updated code to use revised EAGL API.
Removed TouchView and Texture2D classes.
Replaced the views used to choose brush color with a segmented control.
Replace the Texture2D class with code that creates a texture using a Core Graphic bitmap graphics context.
Speeded up the "Shake Me" instructions that appear at the start of the application.
Revised touch handling to use the begin, moved, end, and cancelled methods instead of touchesChanged:withEvent;

Version 1.3
Updated for Beta 4. 
Changed project setting related to code signing.
Replaced pixel buffer objects with framebuffer objects.

Version 1.2
Added an icon and a default.png file.

Version 1.1 
Updated for Beta 2.

================================================================================
Copyright (C) 2008 Apple Inc. All rights reserved.