typedef enum {
    kLineShape  = 0
    ,kRectShape
	,kEllipseShape
	,kImageShape
} ShapeType;

typedef enum {
	
	kRedColorTab = 0
	,kBlueColorTab
	,kYellowColorTab
	,kGreenColorTab
	,kRandomColorTab
} ColorTabIndex;

#define degreesToRadian(x) (3.14159265358979323846 * x / 180.0)