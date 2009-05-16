/*
File: AccelerometerSmoother.h
Abstract: Smooth accelerometer, calibrate default position, and give sensitivity control. 

Version: 2.0
*/

#import "TouchFighter_Types.h"

@interface AccelerometerSmoother : NSObject <UIAccelerometerDelegate> {
@private
	Point3			_position, _accelerometer;
	CGFloat			_calibration[3],
					_sensitivity,
					_smoothing;
	BOOL			_calibrate,
					_forceCalibration,
					_invert;
	
	CGFloat			_acc[3], _rot[3], _mean[3], _var[3], _cal[3];			
}

/* Updates the accelerometer smoothed values. Must be called from game loop, not event loop, for better smoothing */
- (void) update;

@property(readonly) Point3		position;
@property BOOL					calibrate, forceCalibration, invert;
@property CGFloat				sensitivity, smoothing;
@end
