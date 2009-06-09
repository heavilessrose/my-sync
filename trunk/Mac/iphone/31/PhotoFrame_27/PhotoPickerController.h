//
//  PhotoPickerController.h
//  PhotoFrame
//
//  Created by amuck on 10/28/08.
//  Copyright 2008 AppsAmuck LLC. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface PhotoPickerController : UIImagePickerController <UIImagePickerControllerDelegate, UINavigationControllerDelegate>
- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker;
- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingImage:(UIImage *)image editingInfo:(NSDictionary *)editingInfo;
- (void)navigationController:(UINavigationController *)navigationController willShowViewController:(UIViewController *)viewController animated:(BOOL)animated;
- (void)navigationController:(UINavigationController *)navigationController didShowViewController:(UIViewController *)viewController animated:(BOOL)animated;
@end
