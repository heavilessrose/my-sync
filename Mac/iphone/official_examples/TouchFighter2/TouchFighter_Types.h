/*
File: TouchFighter_Types.h
Abstract: Defines commonly used structures and functions.

Version: 2.0
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