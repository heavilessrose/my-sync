/*
File: Maths.m
Abstract: Mathematical functions (linear algebra, ...). 

Version: 1.4
*/


#import "Maths.h"

//CONSTANTS:

#define	MatrixGetPoint31(m)	((Point4Ptr) &(m)[kIndice_x1])
#define	MatrixGetPoint32(m)	((Point4Ptr) &(m)[kIndice_x2])
#define	MatrixGetPoint33(m)	((Point4Ptr) &(m)[kIndice_x3])
#define	MatrixGetPoint34(m)	((Point4Ptr) &(m)[kIndice_x4])

Point3 Point3Zero = (Point3){0., 0., 0.};
Point4 Point4Zero = (Point4){0., 0., 0., 1.};
Matrix MatrixIdentity = (Matrix){1., 0., 0., 0., 0., 1., 0., 0., 0., 0., 1., 0., 0., 0., 0., 1.};

//INTERNAL FUNCTIONS:

void _Matrix_Clear(MatrixPtr m)
{
	m[kIndice_x1] = 1.0;
	m[kIndice_y1] = 0.0;
	m[kIndice_z1] = 0.0;
	m[kIndice_w1] = 0.0;
	
	m[kIndice_x2] = 0.0;
	m[kIndice_y2] = 1.0;
	m[kIndice_z2] = 0.0;
	m[kIndice_w2] = 0.0;
	
	m[kIndice_x3] = 0.0;
	m[kIndice_y3] = 0.0;
	m[kIndice_z3] = 1.0;
	m[kIndice_w3] = 0.0;
	
	m[kIndice_x4] = 0.0;
	m[kIndice_y4] = 0.0;
	m[kIndice_z4] = 0.0;
	m[kIndice_w4] = 1.0;
}

void _Matrix_Copy(const MatrixPtr m, MatrixPtr res)
{
	memcpy(res, m, sizeof(Matrix));
}

void _Matrix_SetRotationFromAngles(MatrixPtr matrix, float rX, float rY, float rZ)
{
	float			A, B, C, D, E, F;
	
	rX = DegreesToRadians(rX);
	rY = DegreesToRadians(rY);
	rZ = DegreesToRadians(rZ);
	
	A = (rX ? cos(rX) : 1.0);
	B = (rX ? sin(rX) : 0.0);
	C = (rY ? cos(rY) : 1.0);
	D = (rY ? sin(rY) : 0.0);
	E = (rZ ? cos(rZ) : 1.0);
	F = (rZ ? sin(rZ) : 0.0);
	
	matrix[kIndice_x1] = C * E;
	matrix[kIndice_y1] = B * D * E + A * F;
	matrix[kIndice_z1] = B * F - A * D * E;
	matrix[kIndice_w1] = 0.0;
	
	matrix[kIndice_x2] = -C * F;
	matrix[kIndice_y2] = A * E - B * D * F;
	matrix[kIndice_z2] = A * D * F + B * E;
	matrix[kIndice_w2] = 0.0;
	
	matrix[kIndice_x3] = D;
	matrix[kIndice_y3] = -B * C;
	matrix[kIndice_z3] = A * C;
	matrix[kIndice_w3] = 0.0;
	
	matrix[kIndice_x4] = 0.0;
	matrix[kIndice_y4] = 0.0;
	matrix[kIndice_z4] = 0.0;
	matrix[kIndice_w4] = 1.0;
}

void _Matrix_MultiplyByPoint3(const MatrixPtr m, const Point3Ptr v, Point3Ptr res)
{
	float			x = v->x,
					y = v->y,
					z = v->z;
	
	res->x = m[kIndice_x1] * x + m[kIndice_y1] * y + m[kIndice_z1] * z;
	res->y = m[kIndice_x2] * x + m[kIndice_y2] * y + m[kIndice_z2] * z;
	res->z = m[kIndice_x3] * x + m[kIndice_y3] * y + m[kIndice_z3] * z;
}

void _Matrix_MultiplyByQuaternion(const MatrixPtr m, const Point4Ptr v, Point4Ptr res)
{
	float			x = v->x,
					y = v->y,
					z = v->z,
					w = v->w;
	
	res->x = m[kIndice_x1] * x + m[kIndice_y1] * y + m[kIndice_z1] * z + m[kIndice_w1] * w;
	res->y = m[kIndice_x2] * x + m[kIndice_y2] * y + m[kIndice_z2] * z + m[kIndice_w2] * w;
	res->z = m[kIndice_x3] * x + m[kIndice_y3] * y + m[kIndice_z3] * z + m[kIndice_w3] * w;
	res->w = m[kIndice_x4] * x + m[kIndice_y4] * y + m[kIndice_z4] * z + m[kIndice_w4] * w;
}

void _Matrix_MultiplyByMatrix(const MatrixPtr m1, const MatrixPtr m2, MatrixPtr res)
{
	_Matrix_MultiplyByQuaternion(m1, MatrixGetPoint31(m2), MatrixGetPoint31(res));
	_Matrix_MultiplyByQuaternion(m1, MatrixGetPoint32(m2), MatrixGetPoint32(res));
	_Matrix_MultiplyByQuaternion(m1, MatrixGetPoint33(m2), MatrixGetPoint33(res));
	_Matrix_MultiplyByQuaternion(m1, MatrixGetPoint34(m2), MatrixGetPoint34(res));
}

//FUNCTIONS:

void MakeRotationPoint3(Point3Ptr v, float rX, float rY, float rZ)
{
	Matrix m;
	
	_Matrix_SetRotationFromAngles((MatrixPtr)&m, rX, rY, rZ);
	_Matrix_MultiplyByPoint3((MatrixPtr)&m, v, v);
}

void MakeRotationQuaternion(Point4Ptr v, float rX, float rY, float rZ)
{
	Matrix m;
	
	_Matrix_SetRotationFromAngles((MatrixPtr)&m, rX, rY, rZ);
	_Matrix_MultiplyByQuaternion((MatrixPtr)&m, v, v);
}

void GetPoint3RotationAxisAndAngle(Point3Ptr vSrc, Point3Ptr vDst, Point3Ptr outAxis, float* outAngle)
{
	Point3		v1 = Point3Normalize(*vSrc);
	Point3		v2 = Point3Normalize(*vDst);
	Point3		axis = Point3CrossProduct(v1, v2);
	float		dot = Point3DotProduct(v1, v2);
	
	*outAngle = acos(dot);	
	*outAxis = Point3Normalize(axis);
}

void TransformPoint3_Copy(MatrixPtr m, Point3Ptr v, Point3Ptr res)
{
	Point3			vt;
	
	_Matrix_MultiplyByPoint3(m, v, (Point3Ptr)&vt);
	
	res->x = vt.x;
	res->y = vt.y;
	res->z = vt.z;
}

void TransformPoint4_Copy(MatrixPtr m, Point4Ptr v, Point4Ptr res)
{
	Point4		vt;
	
	_Matrix_MultiplyByQuaternion(m, v, (Point4Ptr)&vt);
	
	res->x = vt.x/vt.w;
	res->y = vt.y/vt.w;
	res->z = vt.z/vt.w;
	res->w = vt.w/vt.w;	
}

void MakeTransformationMatrix(MatrixPtr matrix, float rX, float rY, float rZ, float tX, float tY, float tZ)
{
	if((rX != 0.0) || (rY != 0.0) || (rZ != 0.0))
	{
		_Matrix_SetRotationFromAngles(matrix, rX, rY, rZ);
	}
	else
	{
		_Matrix_Clear(matrix);
	}
	matrix[kIndice_x4] = tX;
	matrix[kIndice_y4] = tY;
	matrix[kIndice_z4] = tZ;
}

void MultMatrix_Copy(MatrixPtr matrix1, MatrixPtr matrix2, MatrixPtr res)
{
	Matrix mt;
	
	_Matrix_MultiplyByMatrix(matrix1, matrix2, (MatrixPtr)&mt);
	
	_Matrix_Copy((MatrixPtr)&mt, (MatrixPtr)&res);
}

void TransposeMatrix_Copy(MatrixPtr m, MatrixPtr res)
{
	Matrix mt;
	
	mt[kIndice_x1] = m[kIndice_x1];
	mt[kIndice_y1] = m[kIndice_x2];
	mt[kIndice_z1] = m[kIndice_x3];
	mt[kIndice_w1] = m[kIndice_x4];
	
	mt[kIndice_x2] = m[kIndice_y1];
	mt[kIndice_y2] = m[kIndice_y2];
	mt[kIndice_z2] = m[kIndice_y3];
	mt[kIndice_w2] = m[kIndice_y4];
	
	mt[kIndice_x3] = m[kIndice_z1];
	mt[kIndice_y3] = m[kIndice_z2];
	mt[kIndice_z3] = m[kIndice_z3];
	mt[kIndice_w3] = m[kIndice_z4];
		
	mt[kIndice_x4] = m[kIndice_w1];
	mt[kIndice_y4] = m[kIndice_w2];
	mt[kIndice_z4] = m[kIndice_w3];
	mt[kIndice_w4] = m[kIndice_w4];

	
	_Matrix_Copy((MatrixPtr)&mt, res);	
}

void InvertTransformationMatrix_Copy(MatrixPtr m, MatrixPtr res)
{
	Matrix mt;
	
	TransposeMatrix_Copy(m, mt);
	
	_Matrix_Copy((MatrixPtr)&mt, res);
	res[kIndice_x4] = -mt[kIndice_w1];
	res[kIndice_y4] = -mt[kIndice_w2];
	res[kIndice_z4] = -mt[kIndice_w3];
}

Point4 Point4TransformFromModelViewToModelView(Point4 v, Matrix m1, Matrix m2)
{
	Matrix		m;
	
	TransposeMatrix_Copy(m1, (MatrixPtr)&m);
	TransformPoint4_Copy(m, (Point4Ptr)&v, (Point4Ptr)&v);
	TransformPoint4_Copy(m2, (Point4Ptr)&v, (Point4Ptr)&v);
	
	return v;
}

Point3 Point3Transform(Point3 v, Matrix m)
{
	Matrix		tm;
	
	TransposeMatrix_Copy(m, (MatrixPtr)&tm);
	TransformPoint4_Copy(tm, (Point4Ptr)&v, (Point4Ptr)&v);
	
	return v;
}

Point3 Point3TransformFromModelViewToModelView(Point3 p, Matrix m1, Matrix m2)
{
	Point4		v = Point4TransformFromModelViewToModelView(Point4Make(p.x, p.y, p.z, 1.), m1, m2);
	
	return Point3Make(v.x, v.y, v.z);
}

float Point3DotProduct(Point3 v1, Point3 v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

Point3 Point3CrossProduct(Point3 v1, Point3 v2)
{
	return Point3Make(v1.y*v2.z-v1.z*v2.y, v1.z*v2.x-v1.x*v2.z, v1.x*v2.y-v1.y*v2.x);
}

Point3 Point3Normalize(Point3 p)
{
	float norm = Norm(p.x, p.y, p.z);
	return Point3Make(p.x/norm, p.y/norm, p.z/norm);
}


NSString* MatrixToString(MatrixPtr mt)
{
	return [NSString stringWithFormat:@"\n[ %f %f %f %f ]\n[ %f %f %f %f ]\n[ %f %f %f %f ]\n[ %f %f %f %f ]",	mt[kIndice_x1], mt[kIndice_y1], mt[kIndice_z1], mt[kIndice_w1], 
																								mt[kIndice_x2], mt[kIndice_y2], mt[kIndice_z2], mt[kIndice_w2],
																								mt[kIndice_x3], mt[kIndice_y3], mt[kIndice_z3], mt[kIndice_w3],
																								mt[kIndice_x4], mt[kIndice_y4], mt[kIndice_z4], mt[kIndice_w4]];
}