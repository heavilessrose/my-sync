/*

File: ReachabilityAppDelegate.h
Abstract: The application's controller.

Version: 1.5

*/

#import <UIKit/UIKit.h>
#import "Reachability.h"

@interface ReachabilityAppDelegate : NSObject <UITableViewDataSource, UITableViewDelegate> {
    IBOutlet UIWindow *window;
    IBOutlet UIView *contentView;
    IBOutlet UILabel *summaryLabel;
	
	UIImage *carrierDataNetworkImage;
	UIImage *wiFiImage;
	UIImage *stopImage;
	
	UITableView *tableView;
	
	NetworkStatus remoteHostStatus;
	NetworkStatus internetConnectionStatus;
	NetworkStatus localWiFiConnectionStatus;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) UIView *contentView;
@property (nonatomic, retain) UILabel *summaryLabel;

@property (nonatomic, retain) UIImage *carrierDataNetworkImage;
@property (nonatomic, retain) UIImage *wiFiImage;
@property (nonatomic, retain) UIImage *stopImage;
@property (nonatomic, retain) UITableView *tableView;
@property NetworkStatus remoteHostStatus;
@property NetworkStatus internetConnectionStatus;
@property NetworkStatus localWiFiConnectionStatus;

- (void)updateStatus;

@end
