/*
File: Utils.h
Abstract: Utility functions and macros.

Version: 2.0
*/

#import "Texture2D.h"
#import "Maths.h"

#define FAST_RAND_MAX RAND_MAX

//MACROS:
#define fast_unit_rand() ((float)fast_rand() / FAST_RAND_MAX)
#define Random(A, B) (fast_unit_rand() * (B-A) + A)
#define CHECK_CREATION(X)					if (X==nil) {\
												NSLog (@"Object creation failed");\
												[self release];\
												return nil;\
											}
//FUNCTIONS:

Texture2D* ShipTexture();
Texture2D* EnemyShipTexture();
Texture2D* StarTexture();
Texture2D* FlameTexture();
Texture2D* MissileTexture();
void ShipTextureRelease();
void EnemyShipTextureRelease();
void StarTextureRelease();
void FlameTextureRelease();
void MissileTextureRelease();

/* Model Utils */
void SetOpenGLStates();
void UnsetOpenGLStates();
void SetMaterialParameters(float shininess, float r_diffuseColor, float g_diffuseColor, float b_diffuseColor, float r_specularColor, float g_specularColor, float b_specularColor);


/* Fast random number generation */
void fast_rand_init();
unsigned long fast_rand();

@interface Texture2D(TouchFighterSupport)
+ (void) drawTexture:(Texture2D*)texture atPoint:(CGPoint)point depth:(CGFloat)depth;
- (id) initWithString:(NSString*)string dimensions:(CGSize)dimensions fontSize:(CGFloat)size;
- (id) initWithPVR4DataFile:(NSString*)path pixelsWide:(NSUInteger)width pixelsHigh:(NSUInteger)height;
@end
