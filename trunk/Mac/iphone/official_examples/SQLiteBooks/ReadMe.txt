SQLite Book List


This sample application is primarily focused on using the SQLite database with an iPhone application, and secondarily with leveraging that database to respond to low memory warnings. In addition, the sample demonstrates one approach to implementing a master-detail interface

Build Requirements
Mac OS X 10.5.3, Xcode 3.1, iPhone OS 2.0.

Runtime Requirements
Mac OS X 10.5.3, iPhone OS 2.0.

Conceptual Overview
The SQLite library lets you embed a lightweight SQL database into your application without running a separate remote database server process. For the iPhone, SQLite is the preferred mechanism for storing structured data. SQLite stores data in a single file than can be located anywhere the application has access - see iPhone documentation for details about limitations imposed by the device. SQLite supports 5 data types - NULL, INTEGER, REAL, TEXT, and BLOB. A large subset of the SQL92 specification is supported, unsupported features are listed in the SQLite documentation at http://www.sqlite.org.

This sample assumes some familiarity with SQL and focuses on the C API for using SQLite. The SQL functionality used here is not advanced, and should be reasonably easy for engineers to figure out following any of the many excellent SQL tutorials available on the web.

Running the Sample
The sample presents a simple master-detail interface. The master is a list of book titles. Selecting a title navigates to the detail view for that book. The master has a navigation bar (at the top) with a "+" button on the right for creating a new book. This creates the new book and then navigates immediately to the detail view for that book. There is also an "Edit" button. This displays a "-" button next to each book. Touching the minus button shows a "Delete" button which will delete the book from the list. 

The detail view displays three fields: title, copyright date, and author. The user can navigate back to the main list by touching the "Books" button in the navigation bar. If "Edit" is touched on this view, the user can modify individual fields.

Memory Management
In iPhone OS, the amount of usable virtual memory is constrained by the device's physical memory. The system will send notifications to applications in cases where available memory runs low. These warnings are received by the application delegate with the expectation is that steps will be taken to reduce in-memory resources. The nature of these steps is left entirely to the developer. Taking no steps at all may in some cases result in the application being terminated. 

This sample presents one strategy for handling these memory constraints. This pattern is most applicable to applications where memory consumption is dominated by application data that can be stored offline - applications in which memory consumption is dominated by user interface elements may not find this pattern as useful.

The strategy is oriented around a concept called "object hydration". Loosely defined, hydration refers to fetching stored attributes of an object from a database. In more detail, a bridge between a database and an object oriented programming language is implemented such that a table in the database maps to a class in the programming language, and each row in the table maps to an instance of the class.

In this application, at launch time the application delegate initializes the database connection and retrieves the title and primary key identifier for every object stored in the database. This is sufficient information to present to the user a list of the objects. When the user choses to view or edit additional detail for an object, the object is "hydrated" - the remaining attributes for the object are fetched into memory. The user can inspect object after object, and each will be fetched into memory on demand, slowly growing the memory footprint of the application.

At some point, the device memory threshold will be reached and it will issue the application a low memory warning. The application delegate will receive the warning and respond by "dehydrating" all of the objects. Dehydrating, as the name implies, is the reverse of hydration. All attributes of the object except the essential title and key are written back to the database (if there were changes) and removed from memory. This dramatically reduces the application memory footprint in response to the low memory warning.

In practice, this implementation may be overly coarse - massive writes to disk might result and cause an unacceptable user experience. A more sophisticated implementation might use the dehydration approach in conjunction with a least-recently-used table, flushing stale objects at regular time intervals. This implementation is left to the discretion of developers interested in using this pattern.

Creating the Database File
The SQLite database file was created outside the application and then attached as resource file to be included in the application bundle. The database was created using the SQLite Command Line Interface (CLI). Although there are cases in which you may wish to create the database at run time, in many cases this is not necessary. To start the SQLite CLI, lauch Terminal.app and enter:

host:~ username$ sqlite3 my_database.sqlite

This will start the SQLite CLI and either open "my_database.sqlite" if it exists, or create it if it does not. Note that this is a path relative to your Terminal session's current directory.

After entering the above command, you should see something like the following:

SQLite version 3.4.0
Enter ".help" for instructions
sqlite> 

Now you can enter standard SQL language queries. The following queries were used to create the database in this sample:

CREATE TABLE book ('pk' INTEGER PRIMARY KEY, 'title' CHAR(32), 'copyright' REAL, 'author' CHAR(48));
INSERT INTO book(title, copyright, author) VALUES ('War and Peace', -3153600000, 'Leo Tolstoy');
INSERT INTO book(title, copyright, author) VALUES ('Mac OS X Internals', 1166832000, 'Amit Singh');
INSERT INTO book(title, copyright, author) VALUES ('The Divine Comedy', -20466864000, 'Dante Alighieri');

Packaging List
main.m
Launches the application and specifies the class name for the application delegate.

AppDelegate
Opens the database connection, starts the user interface layout, and manages the main array of Book objects.

Book
In-memory representation of book data. Interacts extensively with the database to retrieve and update information.

MasterViewController
Creates a table view and navigation controller for listing all books. Provides controls for adding and removing books.

DetailViewController
Manages a detail display for display fields of a single Book. 

AddViewController
Subclass of DetailViewController with functionality for creating new Book objects.

EditingViewController
View for editing a field of data, text or date.

Changes from Previous Versions
1.9 Fixed potential memory issue in +[Book finalizeStatements].
1.8 Updated for and tested with iPhone OS 2.0. First public release.
1.7 Updated for Beta 7: Build settings fixes for codesigning and provisioning.
1.6 Updated for Beta 6: Moved navigation bar item functionality to nibs. 
1.5 Updated for Beta 5: revised for changes to UITableViewDelegate API. tableView:selectionDidChangeToIndexPath:fromIndexPath: is now tableView:didSelectRowAtIndexPath:. Also, the window is made visible programmatically following subview setup rather than via Interface Builder option, to avoid momentary blank display.
1.4 Updated for Beta 4. Primary changes related to navigation items. Converted this documented from rtf to txt.
1.3 Updated for Beta 3. Includes changes to accommodate revisions to the UITableView APIs, specifically the reusable cell functionality. See [UITableView dequeueReusableCellWithIdentifier:] and [UITableViewCell initWithFrame:reuseIdentifier:]. Revised user interface to improve editing experience. Added use of Interface Builder in new "EditingView" nib.
1.2 Updated for Beta 2 with minor changes.
1.1 Revised user interface to comply with iPhone Human Interface Guidelines.
1.0 Initial version published.

Copyright (C) 2008 Apple Inc. All rights reserved.
