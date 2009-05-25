//
//  Button_FunViewController.h
//  Button Fun
//
//  Created by Jeff LaMarche on 6/13/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface Button_FunViewController : UIViewController 
{
	IBOutlet	UILabel		*statusText;
}
@property (retain, nonatomic) UILabel *statusText;

-(IBAction)buttonPressed:(id)sender;
@end

