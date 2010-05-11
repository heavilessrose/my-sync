//
//  pcAudioClipsViewController.h
//  MSN
//
//  Created by  apple on 09-12-4.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface pcAudioClipsViewController : UITableViewController {
	NSMutableArray*				audioClips;
	NSIndexPath*				selectedIndexPath;
}
@property (nonatomic, retain) NSMutableArray* audioClips;
@property (nonatomic, retain) NSIndexPath* selectedIndexPath;

- (IBAction)ok: (id)sender;
- (IBAction)cancel: (id)sender;
@end
