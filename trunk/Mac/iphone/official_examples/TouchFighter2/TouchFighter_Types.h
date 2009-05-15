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

File: TouchFighter_Types.h
Abstract: Defines commonly used structures and functions.

Version: 2.0

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

#import <math.h>

//MACROS:

#define POINT3_COORD_AT_INDEX(P, X) ((float*)&P)[X]

//STRUCTURES:

/* A 3D point */
typedef struct {
	float			x,
					y,
					z;
} Point3;
typedef Point3* Point3Ptr;

/* A 4D point (quaternion) */
typedef struct {
	float			x,
					y,
					z,
					w;
} Point4;
typedef Point4* Point4Ptr;

//CONSTANTS:

extern Point3 Point3Zero;
extern Point4 Point4Zero;

//FUNCTIONS:

/* Point3 */

static inline Point3 Point3Make(float x, float y, float z)
{
	return (Point3) {x, y, z};
}

static inline Point3 Point3Add(Point3 p1, Point3 p2)
{
	return (Point3) {p1.x + p2.x, p1.y + p2.y, p1.z + p2.z};
}

static inline Point3 Point3Sub(Point3 p1, Point3 p2)
{
	return (Point3) {p1.x - p2.x, p1.y - p2.y, p1.z - p2.z};
}

static inline Point3 Point3Mult(Point3 p1, float l)
{
	return (Point3) {p1.x * l, p1.y * l, p1.z * l};
}

static inline bool Point3EqualsPoint3(Point3 p1, Point3 p2)
{
	return (p1.x == p2.x) && (p1.y == p2.y) && (p1.z == p2.z);
}

static inline float Point3Norm(Point3 p)
{
	return sqrt((p.x * p.x) + (p.y * p.y) + (p.z * p.z));
}

/* Point4 */

static inline Point4 Point4Make(float x, float y, float z, float w)
{
	return (Point4) {x, y, z, w};
}

static inline Point4 Point4Mult(Point4 p1, float l)
{
	return (Point4) {p1.x * l, p1.y * l, p1.z * l, p1.w * l};
}