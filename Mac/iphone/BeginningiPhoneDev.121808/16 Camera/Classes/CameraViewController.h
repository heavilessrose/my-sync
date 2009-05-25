//
//  CameraViewController.h
//  Camera
//
//  Created by Jeff LaMarche on 8/8/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface CameraViewController : UIViewController <UIImagePickerControllerDelegate, UINavigationControllerDelegate> {
	IBOutlet UIImageView *imageView;
	IBOutlet UIButton *takePictureButton;
	IBOutlet UIButton *selectFromCameraRollButton;
}
@property (nonatomic, retain) UIImageView *imageView;
@property (nonatomic, retain) UIButton *takePictureButton;
@property (nonatomic, retain) UIButton *selectFromCameraRollButton;
- (IBAction)getCameraPicture:(id)sender;
- (IBAction)selectExistingPicture;
@end

