/*
File: HighScoresView.h
Abstract: Subclass of UIView that handles high scores.

Version: 2.0
*/

#import <UIKit/UIKit.h>

@class GameObjectView;

@interface HighScoresView : UIView <UITextFieldDelegate> {
	IBOutlet UIView*			_scores;
	IBOutlet UIView*			_names;
	GameObjectView*				_spaceShipView;
	
	NSArray*					_savedScores;
	NSUInteger					_rank;
}

- (id)initWithFrame:(CGRect)frame;
- (void) addName:(NSString*)name score:(NSUInteger)score;
@end
