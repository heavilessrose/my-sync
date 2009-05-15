/*

===== IMPORTANT =====

This is sample code demonstrating API, technology or techniques in development.
Although this sample code has been reviewed for technical accuracy, it is not
final. Apple is supplying this information to help you plan for the adoption of
the technologies and programming interfaces described herein. This information
is subject to change, and software implemented based on this sample code should
be tested with final operating system software and final documentation. Newer
versions of this sample code may be provided with future seeds of the API or
technology. For information about updates to this and other developer
documentation, view the New & Updated sidebars in subsequent documentation
seeds.

=====================

File: Maths.h
Abstract: Mathematical functions (linear algebra, ...). 

Version: 1.4

Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple Inc.
("Apple") in consideration of your agreement to the following terms, and your
use, installation, modification or redistribution of this Apple software
constitutes acceptance of these terms.  If you do not agree with these terms,
please do not use, install, modify or redistribute this Apple software.

In consideration of your agreement to abide by the following terms, and subject
to these terms, Apple grants you a personal, non-exclusive license, under
Apple's copyrights in this original Apple software (the "Apple Software"), to
use, reproduce, modify and redistribute the Apple Software, with or without
modifications, in source and/or binary forms; provided that if you redistribute
the Apple Software in its entirety and without modifications, you must retain
this notice and the following text and disclaimers in all such redistributions
of the Apple Software.
Neither the name, trademarks, service marks or logos of Apple Inc. may be used
to endorse or promote products derived from the Apple Software without specific
prior written permission from Apple.  Except as expressly stated in this notice,
no other rights or licenses, express or implied, are granted by Apple herein,
including but not limited to any patent rights that may be infringed by your
derivative works or by other works in which the Apple Software may be
incorporated.

The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
COMBINATION WITH YOUR PRODUCTS.

IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR
DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF
CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF
APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Copyright (C) 2008 Apple Inc. All Rights Reserved.

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