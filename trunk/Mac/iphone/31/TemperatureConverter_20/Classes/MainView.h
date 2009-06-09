//
//  MainView.h
//  TemperatureConverter
//
//  Created by apps on 10/21/08.
//  Copyright AppsAmuck.com 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface MainView : UIView {
    IBOutlet UITextField *celsius;
    IBOutlet UITextField *fahrenheit;
    IBOutlet UITextField *kelvin;
}
- (IBAction)reset;
@end
