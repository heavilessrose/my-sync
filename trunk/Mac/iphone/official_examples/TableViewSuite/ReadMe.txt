### TableViewSuite ###

================================================================================
DESCRIPTION:

This sample shows how to use UITableView through a progression of increasingly advanced applications that display information about time zones.

The first example shows a simple list of the time zone names. It shows how to display a simple data set in a table view.

The second example shows the time zones split into sections by region, with the region name as the section heading. It shows how to create an indexed table view.

The third example shows the time zones split into sections alphabetically, with the first letter of their locale name as the section heading. It shows how to set up a table view to display an index.


When implementing a table view cell, there's a tension between optimal scrolling performance and optimal edit/reordering performance. You should typically use subviews in the cell's content view.

When you have an edit or reordering control, using subviews makes the implementation easier, and the animations perform better because UIKit doesn't have to redraw during animations.

Subviews have two costs:

1) Initialization. This can be largely mitigated by reusing table cells.

2) Compositing. This can be largely mitigated by making the views opaque. Often, one translucent subview is fine, but more than one frequently causes frame drops while scrolling.


If the content is complex, however (more than about three subviews), scrolling performance may suffer. If this becomes a problem, you can instead draw directly in a subview of the table view cell's content view.

The fourth example displays more information about each time zone, such as the time and relative day in that time zone. Its main aim is to show how you can customize a table view cell using subviews. It also introduces custom classes to represent regions and time zones to help reduce the overhead of calculating the required information -- these are also used in the fifth example.

The fifth example is an extension of the fourth. It displays even more information about each time zone, such as the time and relative day in that time zone. Its shows how you can create a custom table view cell that contains a custom view that draws its content in -drawRect:.


================================================================================
BUILD REQUIREMENTS:

Mac OS X 10.5.3, Xcode 3.1, iPhone OS 2.0

================================================================================
RUNTIME REQUIREMENTS:

Mac OS X 10.5.3, iPhone OS 2.0

================================================================================
PACKAGING LIST:


1_SimpleTableView:

1_SimpleTableView/Classes/RootViewController.h
1_SimpleTableView/Classes/RootViewController.m
View controller that sets up the table view and serves as the table view's data source and delegate.

1_SimpleTableView/Classes/SimpleTableViewAppDelegate.h
1_SimpleTableView/Classes/SimpleTableViewAppDelegate.m
Application delegate that configures the view controller.

1_SimpleTableView/main.m
Main source file for this sample.

1_SimpleTableView/MainWindow.xib
The xib file containing the application's main window.

1_SimpleTableView/en.lproj/Localizable.strings
Strings file containing localization dictionary.



2_SimpleSectionedTableView:

2_SimpleSectionedTableView/Classes/RootViewController.h
2_SimpleSectionedTableView/Classes/RootViewController.m
View controller that serves as the table view's data source and delegate. It also set up the data.

2_SimpleSectionedTableView/Classes/SimpleSectionedTableViewAppDelegate.h
2_SimpleSectionedTableView/Classes/SimpleSectionedTableViewAppDelegate.m
Application delegate that configures the view controller.

2_SimpleSectionedTableView/main.m
Main source file for this sample.

2_SimpleSectionedTableView/MainWindow.xib
The xib file containing the application's main window.

2_SimpleSectionedTableView/en.lproj/Localizable.strings
Strings file containing localization dictionary.



3_SimpleIndexedTableView:

3_SimpleIndexedTableView/Classes/RootViewController.h
3_SimpleIndexedTableView/Classes/RootViewController.m
View controller that serves as the table view's data source and delegate. It also set up the data.

3_SimpleIndexedTableView/Classes/SimpleIndexedTableViewAppDelegate.h
3_SimpleIndexedTableView/Classes/SimpleIndexedTableViewAppDelegate.m
Application delegate that configures the view controller.

3_SimpleIndexedTableView/main.m
Main source file for this sample.

3_SimpleIndexedTableView/MainWindow.xib
The xib file containing the application's main window.

3_SimpleIndexedTableView/en.lproj/Localizable.strings
Strings file containing localization dictionary.



4_TableViewCellSubviews:

4_TableViewCellSubviews/Classes/TableViewCellSubviewsAppDelegate.h
4_TableViewCellSubviews/Classes/TableViewCellSubviewsAppDelegate.m
Application delegate that sets up the navigation controller and the root view controller.

4_TableViewCellSubviews/Classes/RootViewController.h
4_TableViewCellSubviews/Classes/RootViewController.m
View controller that sets up the table view and the time zone data.

4_TableViewCellSubviews/Classes/Region.h
4_TableViewCellSubviews/Classes/Region.m
Object to represent a region containing the corresponding time zone wrappers.

4_TableViewCellSubviews/Classes/TimeZoneWrapper.h
4_TableViewCellSubviews/Classes/TimeZoneWrapper.m
Object to represent a time zone, caching various derived properties that are expensive to compute.

4_TableViewCellSubviews/main.m
Main source file for this sample.

4_TableViewCellSubviews/MainWindow.xib
The xib file containing the application's main window.

4_TableViewCellSubviews/en.lproj/Localizable.strings
Strings file containing localization dictionary.



5_CustomTableViewCell:

5_CustomTableViewCell/Classes/CustomTableViewCellAppDelegate.h
5_CustomTableViewCell/Classes/CustomTableViewCellAppDelegate.m
Application delegate that sets up the navigation controller and the root view controller.

5_CustomTableViewCell/Classes/RootViewController.h
5_CustomTableViewCell/Classes/RootViewController.m
View controller that sets up the table view and the time zone data.

5_CustomTableViewCell/Classes/Region.h
5_CustomTableViewCell/Classes/Region.m
Object to represent a region containing the corresponding time zone wrappers.

5_CustomTableViewCell/Classes/TimeZoneWrapper.h
5_CustomTableViewCell/Classes/TimeZoneWrapper.m
Object to represent a time zone, caching various derived properties that are expensive to compute.

5_CustomTableViewCell/Classes/TimeZoneCell.h
5_CustomTableViewCell/Classes/TimeZoneCell.m
A table view cell to display various pieces of information about a time zone.. 

5_CustomTableViewCell/Classes/TimeZoneView.h
5_CustomTableViewCell/Classes/TimeZoneView.m
A view to display various pieces of information about a time zone.

5_CustomTableViewCell/main.m
Main source file for this sample.

5_CustomTableViewCell/MainWindow.xib
The xib file containing the application's main window.

5_CustomTableViewCell/en.lproj/Localizable.strings
Strings file containing localization dictionary.



================================================================================
CHANGES FROM PREVIOUS VERSIONS:

Version 1.8
- Updated for and tested with iPhone OS 2.0. First public release.
- Modified fourth and fifth examples to make timer-based updates more efficient.

Version 1.7
- Modified fourth and added fifth example.

Version 1.6
- Updated the Default.png image.
- Added LSRequiresIPhoneOS flag to Info.plist files.

Version 1.5
- Updated for Beta 5.
- Added localized strings files.
- Made minor changes to project file -- added ReadMe, removed project-level override for ALWAYS_SEARCH_USER_PATHS, added override at target level.
- Removed Visible At Launch flag from window in MainWindow.xib; added [window makeKeyAndVisible] in application delegate.

Version 1.4
- Updated for Beta 4.
- Adopts the new pattern in the Cocoa Touch List project template.
- The application delegate serves as the controller for the application's data; the table view controllers retrieve data from the application delegate.
- Minor changes to artwork.

Version 1.3
- Updated for Beta 3.
- The samples now use nib files and UITableViewController; they also adopt the new pattern for table cell reuse.

================================================================================
Copyright (C) 2008 Apple Inc. All rights reserved.
