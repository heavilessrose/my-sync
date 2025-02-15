#import "TTGlobal.h"

@protocol TTStyleDelegate;
@class TTShape, TTStyleContext;

@interface TTStyleContext : NSObject {
  id<TTStyleDelegate> _delegate;
  CGRect _frame;
  CGRect _contentFrame;
  TTShape* _shape;
  UIFont* _font;
  BOOL _didDrawContent;
}

@property(nonatomic,assign) id<TTStyleDelegate> delegate;
@property(nonatomic) CGRect frame;
@property(nonatomic) CGRect contentFrame;
@property(nonatomic,retain) TTShape* shape;
@property(nonatomic,retain) UIFont* font;
@property(nonatomic) BOOL didDrawContent;

@end

///////////////////////////////////////////////////////////////////////////////////////////////////

@interface TTStyle : NSObject {
  TTStyle* _next;
}

@property(nonatomic,retain) TTStyle* next;

- (id)initWithNext:(TTStyle*)next;

- (void)draw:(TTStyleContext*)context;

- (UIEdgeInsets)addToInsets:(UIEdgeInsets)insets forSize:(CGSize)size;
- (CGSize)addToSize:(CGSize)size context:(TTStyleContext*)context;

- (void)addStyle:(TTStyle*)style;

- (id)firstStyleOfClass:(Class)cls;
- (id)styleForPart:(NSString*)name;

@end

///////////////////////////////////////////////////////////////////////////////////////////////////

@interface TTContentStyle : TTStyle

+ (TTContentStyle*)styleWithNext:(TTStyle*)next;

@end

///////////////////////////////////////////////////////////////////////////////////////////////////

@interface TTPartStyle : TTStyle {
  NSString* _name;
  TTStyle* _style;
}

@property(nonatomic,copy) NSString* name;
@property(nonatomic,retain) TTStyle* style;

+ (TTPartStyle*)styleWithName:(NSString*)name style:(TTStyle*)style next:(TTStyle*)next;

- (void)drawPart:(TTStyleContext*)context;

@end

///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Causes all layers going forward to use a particular shape.
 */
@interface TTShapeStyle : TTStyle {
  TTShape* _shape;
}

@property(nonatomic,retain) TTShape* shape;

+ (TTShapeStyle*)styleWithShape:(TTShape*)shape next:(TTStyle*)next;

@end

///////////////////////////////////////////////////////////////////////////////////////////////////

@interface TTInsetStyle : TTStyle {
  UIEdgeInsets _inset;
}

@property(nonatomic) UIEdgeInsets inset;

+ (TTInsetStyle*)styleWithInset:(UIEdgeInsets)inset next:(TTStyle*)next;

@end

///////////////////////////////////////////////////////////////////////////////////////////////////

@interface TTBoxStyle : TTStyle {
  UIEdgeInsets _margin;
  UIEdgeInsets _padding;
  CGSize _minSize;
  TTPosition _position;
}

@property(nonatomic) UIEdgeInsets margin;
@property(nonatomic) UIEdgeInsets padding;
@property(nonatomic) CGSize minSize;
@property(nonatomic) TTPosition position;

+ (TTBoxStyle*)styleWithMargin:(UIEdgeInsets)margin next:(TTStyle*)next;
+ (TTBoxStyle*)styleWithPadding:(UIEdgeInsets)padding next:(TTStyle*)next;
+ (TTBoxStyle*)styleWithFloats:(TTPosition)position next:(TTStyle*)next;
+ (TTBoxStyle*)styleWithMargin:(UIEdgeInsets)margin padding:(UIEdgeInsets)padding
               next:(TTStyle*)next;
+ (TTBoxStyle*)styleWithMargin:(UIEdgeInsets)margin padding:(UIEdgeInsets)padding
               minSize:(CGSize)minSize position:(TTPosition)position next:(TTStyle*)next;

@end

///////////////////////////////////////////////////////////////////////////////////////////////////

@interface TTTextStyle : TTStyle {
  UIFont* _font;
  UIColor* _color;
  UIColor* _shadowColor;
  CGSize _shadowOffset;
  CGFloat _minimumFontSize;
  UITextAlignment _textAlignment;
  UIControlContentVerticalAlignment _verticalAlignment;
  UILineBreakMode _lineBreakMode;
}

@property(nonatomic,retain) UIFont* font;
@property(nonatomic,retain) UIColor* color;
@property(nonatomic,retain) UIColor* shadowColor;
@property(nonatomic) CGFloat minimumFontSize;
@property(nonatomic) CGSize shadowOffset;
@property(nonatomic) UITextAlignment textAlignment;
@property(nonatomic) UIControlContentVerticalAlignment verticalAlignment;
@property(nonatomic) UILineBreakMode lineBreakMode;

+ (TTTextStyle*)styleWithFont:(UIFont*)font next:(TTStyle*)next;
+ (TTTextStyle*)styleWithColor:(UIColor*)color next:(TTStyle*)next;
+ (TTTextStyle*)styleWithFont:(UIFont*)font color:(UIColor*)color next:(TTStyle*)next;
+ (TTTextStyle*)styleWithFont:(UIFont*)font color:(UIColor*)color
                textAlignment:(UITextAlignment)textAlignment next:(TTStyle*)next;
+ (TTTextStyle*)styleWithFont:(UIFont*)font color:(UIColor*)color
                shadowColor:(UIColor*)shadowColor shadowOffset:(CGSize)shadowOffset
                next:(TTStyle*)next;
+ (TTTextStyle*)styleWithFont:(UIFont*)font color:(UIColor*)color
                minimumFontSize:(CGFloat)minimumFontSize
                shadowColor:(UIColor*)shadowColor shadowOffset:(CGSize)shadowOffset
                next:(TTStyle*)next;

@end

///////////////////////////////////////////////////////////////////////////////////////////////////

@interface TTImageStyle : TTStyle {
  NSString* _imageURL;
  UIImage* _image;
  UIImage* _defaultImage;
  UIViewContentMode _contentMode;
  CGSize _size;
}

@property(nonatomic,copy) NSString* imageURL;
@property(nonatomic,retain) UIImage* image;
@property(nonatomic,retain) UIImage* defaultImage;
@property(nonatomic) CGSize size;
@property(nonatomic) UIViewContentMode contentMode;

+ (TTImageStyle*)styleWithImageURL:(NSString*)imageURL next:(TTStyle*)next;
+ (TTImageStyle*)styleWithImageURL:(NSString*)imageURL defaultImage:(UIImage*)defaultImage
                 next:(TTStyle*)next;
+ (TTImageStyle*)styleWithImageURL:(NSString*)imageURL defaultImage:(UIImage*)defaultImage
                 contentMode:(UIViewContentMode)contentMode
                 size:(CGSize)size next:(TTStyle*)next;
+ (TTImageStyle*)styleWithImage:(UIImage*)image next:(TTStyle*)next;
+ (TTImageStyle*)styleWithImage:(UIImage*)image defaultImage:(UIImage*)defaultImage
                 next:(TTStyle*)next;
+ (TTImageStyle*)styleWithImage:(UIImage*)image defaultImage:(UIImage*)defaultImage
                 contentMode:(UIViewContentMode)contentMode
                 size:(CGSize)size next:(TTStyle*)next;

@end

///////////////////////////////////////////////////////////////////////////////////////////////////

@interface TTMaskStyle : TTStyle {
  UIImage* _mask;
}

@property(nonatomic,retain) UIImage* mask;

+ (TTMaskStyle*)styleWithMask:(UIImage*)mask next:(TTStyle*)next;

@end

///////////////////////////////////////////////////////////////////////////////////////////////////

@interface TTSolidFillStyle : TTStyle {
  UIColor* _color;
}

@property(nonatomic,retain) UIColor* color;

+ (TTSolidFillStyle*)styleWithColor:(UIColor*)color next:(TTStyle*)next;

@end

///////////////////////////////////////////////////////////////////////////////////////////////////

@interface TTLinearGradientFillStyle : TTStyle {
  UIColor* _color1;
  UIColor* _color2;
}

@property(nonatomic,retain) UIColor* color1;
@property(nonatomic,retain) UIColor* color2;

+ (TTLinearGradientFillStyle*)styleWithColor1:(UIColor*)color1 color2:(UIColor*)color2
                              next:(TTStyle*)next;

@end

///////////////////////////////////////////////////////////////////////////////////////////////////

@interface TTReflectiveFillStyle : TTStyle {
  UIColor* _color;
}

@property(nonatomic,retain) UIColor* color;

+ (TTReflectiveFillStyle*)styleWithColor:(UIColor*)color next:(TTStyle*)next;

@end

///////////////////////////////////////////////////////////////////////////////////////////////////

@interface TTShadowStyle : TTStyle {
  UIColor* _color;
  CGFloat _blur;
  CGSize _offset;
}

@property(nonatomic,retain) UIColor* color;
@property(nonatomic) CGFloat blur;
@property(nonatomic) CGSize offset;

+ (TTShadowStyle*)styleWithColor:(UIColor*)color blur:(CGFloat)blur offset:(CGSize)offset
                  next:(TTStyle*)next;

@end

///////////////////////////////////////////////////////////////////////////////////////////////////

@interface TTInnerShadowStyle : TTShadowStyle
@end

///////////////////////////////////////////////////////////////////////////////////////////////////

@interface TTSolidBorderStyle : TTStyle {
  UIColor* _color;
  CGFloat _width;
}

@property(nonatomic,retain) UIColor* color;
@property(nonatomic) CGFloat width;

+ (TTSolidBorderStyle*)styleWithColor:(UIColor*)color width:(CGFloat)width next:(TTStyle*)next;

@end

///////////////////////////////////////////////////////////////////////////////////////////////////

@interface TTFourBorderStyle : TTStyle {
  UIColor* _top;
  UIColor* _right;
  UIColor* _bottom;
  UIColor* _left;
  CGFloat _width;
}

@property(nonatomic,retain) UIColor* top;
@property(nonatomic,retain) UIColor* right;
@property(nonatomic,retain) UIColor* bottom;
@property(nonatomic,retain) UIColor* left;
@property(nonatomic) CGFloat width;

+ (TTFourBorderStyle*)styleWithTop:(UIColor*)top right:(UIColor*)right
                      bottom:(UIColor*)bottom left:(UIColor*)left width:(CGFloat)width
                      next:(TTStyle*)next;

@end

///////////////////////////////////////////////////////////////////////////////////////////////////

@interface TTBevelBorderStyle : TTStyle {
  UIColor* _highlight;
  UIColor* _shadow;
  CGFloat _width;
  NSInteger _lightSource;
}

@property(nonatomic,retain) UIColor* highlight;
@property(nonatomic,retain) UIColor* shadow;
@property(nonatomic) CGFloat width;
@property(nonatomic) NSInteger lightSource;

+ (TTBevelBorderStyle*)styleWithColor:(UIColor*)color width:(CGFloat)width next:(TTStyle*)next;

+ (TTBevelBorderStyle*)styleWithHighlight:(UIColor*)highlight shadow:(UIColor*)shadow
                       width:(CGFloat)width lightSource:(NSInteger)lightSource next:(TTStyle*)next;

@end

///////////////////////////////////////////////////////////////////////////////////////////////////

@protocol TTStyleDelegate <NSObject>

@optional

- (NSString*)textForLayerWithStyle:(TTStyle*)style;

- (UIImage*)imageForLayerWithStyle:(TTStyle*)style;

- (void)drawLayer:(TTStyleContext*)context withStyle:(TTStyle*)style;

@end
