
Reachability

===========================================================================
DESCRIPTION:

The Reachability sample application demonstrates how to use the System Configuration 
framework to determine the network state of an iPhone or iPod touch. In particular, 
it demonstrates how to know when IP traffic might be routed through a carrier 
data network interface (such as EDGE).

The Reachability class provides methods to query the state of the network, which tells you
which network interfaces are available on the device. They return
one of the values in the NetworkStatus enumeration, which include NotReachable, 
ReachableViaCarrierDataNetwork, and ReachableViaWiFiNetwork. The availability of network
interfaces does not tell you if a given host is reachable. For that, the Reachability class
lets you query the reachability of a given host.

The Reachability class operates in two modes: With and without network status changes enabled.
In the default state, it returns the results of the queries you make synchronously, which tells
you the state of the device's network interfaces at the time of the query. In the other state,
Reachability runs asynchronously and will report back changes to the network state and to the
reachability of remote hosts as those changes happen.

===========================================================================
USING THE SAMPLE

Build and run the sample using Xcode. When running the iPhone OS simulator, 
you can exercise the application by disconnecting the Ethernet cable, turning off 
AirPort, or by joining an ad-hoc local WiFi network. If running in asychronous mode,
the application updates to reflect the new network state of the device. If running in
the default, synchronous mode, you'll have to restart the application in the simulator
to get it to reflect the new network state.

By default, the application uses www.apple.com as the remote host to query to determine
reachability. You can change the host it uses in ReachabilityAppDelegate.m, and you can also
supply an IP address. To do that comment out the line that includes 
"[[Reachability sharedReachability] setHostName:[self hostName]]" and uncomment the 
line that includes "[[Reachability sharedReachability] setAddress:@"0.0.0.0"]", changing
the IP address to a different one.

To enable asynchronous network state change notifications, uncomment the line in
ReachabilityAppDelegate.m that includes [[Reachability sharedReachability] setNetworkStatusNotificationsEnabled:YES];

===========================================================================
BUILD REQUIREMENTS

Mac OS X 10.5.3, Xcode 3.1, iPhone OS 2.0

===========================================================================
RUNTIME REQUIREMENTS

Mac OS X 10.5.3, iPhone OS 2.0

===========================================================================
PACKAGING LIST

Reachability.h
Reachability.m
SystemConfiguration framework wrapper.

ReachabilityAppDelegate.h
ReachabilityAppDelegate.m
Class that is the application's controller.

ReachabilityTableCell.h
ReachabilityTableCell.m
Custom table cell.

===========================================================================
CHANGES FROM PREVIOUS VERSIONS

Version 1.5
- Updated for and tested with iPhone OS 2.0. First public release.

Version 1.4
- Updated for Beta 7.

Version 1.3
- Updated for Beta 6.
- Added LSRequiresIPhoneOS key to Info.plist.

Version 1.2
- Updated for Beta 4. Added code signing.

Version 1.1
- Updated for Beta 3 to use a nib file.

Copyright (C)2008 Apple Inc. All rights reserved.