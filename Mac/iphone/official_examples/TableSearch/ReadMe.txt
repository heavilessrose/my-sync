TableSearch

Demonstrates how to search the contents of a UITableView using UISearchBar, effectively filtering in and out the contents of that table.  If an iPhone/iPod Touch application has large amounts of table data, this sample shows how to filter it down to a manageable amount if memory usage is a concern or you just want users to scroll through less content in a table.


Build Requirements
Mac OS X 10.5.3, Xcode 3.1, iPhone OS 2.0


Runtime Requirements
Mac OS X 10.5.3, iPhone OS 2.0


Using the Sample
Build and run TableSearch using Xcode 3.1. To run in the simulator, set the Active SDK to Simulator. To run on a device, set the Active SDK to the appropriate Device setting.  When launched tap the search field and as you enter case insensitive text the list shinks/expands based on the filter text.  An empty string will show the entire contents.  To get back the entire contents once you have filtered the content, touch the search bar again, click the clear ('x') button and hit cancel.


Packaging List
main.m - Main source file for this sample.
AppDelegate.h/.m - The application's delegate to setup its window and content.
MainViewController.h/.m - The main view controller controlling the UITableView and UISearchBar.


Changes from Previous Versions
1.1 - Updated UI - no more use of UINavigationController.
1.0 - Updated for and tested with iPhone OS 2.0. First public release.


Copyright (C) 2008 Apple Inc. All rights reserved.