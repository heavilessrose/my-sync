HeaderFooter

Demonstrates how to implement and customize the 2 UIView properties of UITableView: header ('tableHeaderView') and footer ('tableFooterView').  It is designed to somewhat resemble the "Contacts" application, showing you ways to design your own header and footer content.  It uses the UITableViewStyle: UITableViewStyleGrouped to achieve a more similar appearance as well.
This sample implements two basic UIViews from a nib file: one for the header and one for the footer.  Each as IBOutlets are set as header and footer properties of the UITableView.  Additional code is provided to illustrate how to set their transparent background colors, but the same work that can also be done in the UIView's xib.
Build Requirements
Mac OS X 10.5.3, Xcode 3.1, iPhone OS 2.0.


Runtime Requirements
Mac OS X 10.5.3, iPhone OS 2.0.


Using the Sample
Launch the HeaderFooter project using Xcode 3.1.

To run in the simulator, set the Active SDK to Simulator. To run on a device, set the Active SDK to the appropriate Device setting.  When launched notice the customized header and foot for the grouped-sytle UITableView.


Packaging List
main.m - Main source file for this sample.
AppDelegate.h/.m - The application's delegate to setup its window and content.
MyTableViewController.h/.m - The main UIViewController containing the header/footer example for UITableView.


Changes from Previous Versions
1.0 - First release.


Copyright (C) 2008 Apple Inc. All rights reserved.