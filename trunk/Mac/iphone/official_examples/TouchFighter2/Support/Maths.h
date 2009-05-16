/*
File: Maths.h
Abstract: Mathematical functions (linear algebra, ...). 

Version: 1.4
*/

#import "TouchFighter_Types.h"

//MACROS:

#define	DegreesToRadians(x) ((x) * M_PI / 180.0)
#define	RadiansToDegrees(x) ((x) * 180.0 / M_PI)
#define	Norm(X, Y, Z) (sqrt((X)*(X) + (Y)*(Y) + (Z)*(Z)))
#define Sign(X) ((X < 0.) ? -1 : ((X == 0.) ? 0 : 1))
#define Clamp(X, A, B) ((X < A) ? A : ((X > B) ? B : X))

//STRUCTURES:

typedef enum {
	kIndice_x1 = 0,
	kIndice_y1,
	kIndice_z1,
	kIndice_w1,
	
	kIndice_x2,
	kIndice_y2,
	kIndice_z2,
	kIndice_w2,
	
	kIndice_x3,
	kIndice_y3,
	kIndice_z3,
	kIndice_w3,
	
	kIndice_x4,
	kIndice_y4,
	kIndice_z4,
	kIndice_w4,
	
	kNumMatrixIndices
} MatrixIndex;

//STRUCTURES:

/* A 4x4 matrix */
typedef float Matrix[kNumMatrixIndices];
typedef float* MatrixPtr;

//CONSTANTS:

Matrix MatrixIdentity;

//FUNCTION PROTOTYPES:

Point3 Point3TransformFromModelViewToModelView(Point3 v, Matrix m1, Matrix m2);
Point3 Point3Transform(Point3 v, Matrix m);
Point4 Point4TransformFromModelViewToModelView(Point4 v, Matrix m1, Matrix m2);
Point3 Point3Normalize(Point3 v1);
float Point3DotProduct(Point3 v1, Point3 v2);
Point3 Point3CrossProduct(Point3 v1, Point3 v2);
void MakeRotationPoint3(Point3Ptr v, float rX, float rY, float rZ);
void TransformPoint4_Copy(MatrixPtr m, Point4Ptr v, Point4Ptr res);
void MakeTransformationMatrix(MatrixPtr matrix, float rX, float rY, float rZ, float tX, float tY, float tZ);
void TransposeMatrix_Copy(MatrixPtr m, MatrixPtr res);
void GetPoint3RotationAxisAndAngle(Point3Ptr vSrc, Point3Ptr vDst, Point3Ptr outAxis, float* outAngle);
NSString* MatrixToString(MatrixPtr m);