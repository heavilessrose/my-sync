//
//  PhotoPickerController.m
//  PhotoFrame
//
//  Created by amuck on 10/28/08.
//  Copyright 2008 AppsAmuck LLC. All rights reserved.
//

#import "PhotoPickerController.h"
#import "MainViewController.h"


@implementation PhotoPickerController


- (id) init {
	if (self = [super init]) {
		if ([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypePhotoLibrary])
			self.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
		self.delegate = self;
		self.allowsImageEditing = TRUE;
		self.navigationBar.barStyle = UIBarStyleBlackTranslucent;
	}
	return self;
}


- (void)imagePickerController:(UIImagePickerController *)picker 
		didFinishPickingImage:(UIImage *)image 
				  editingInfo:(NSDictionary *)editingInfo {
	NSString *uniquePath = [[NSHomeDirectory() stringByAppendingPathComponent:@"Documents"] stringByAppendingPathComponent:@"selectedImage.png"];
	[UIImagePNGRepresentation(image) writeToFile: uniquePath atomically:YES];
	
	[self.parentViewController dismissModalViewControllerAnimated:YES];
	[[MainViewController getInstance] loadImage];
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker {
	[self.parentViewController dismissModalViewControllerAnimated:YES];
}

- (void)navigationController:(UINavigationController *)navigationController willShowViewController:(UIViewController *)viewController animated:(BOOL)animated {
}

- (void)navigationController:(UINavigationController *)navigationController didShowViewController:(UIViewController *)viewController animated:(BOOL)animated {
}


@end
