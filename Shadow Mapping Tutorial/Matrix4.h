
#ifndef _MATRIX4_H_
#define _MATRIX4_H_

#include <string>
#include <iostream>
#include "Vector4.h"
#include "Vector3.h"


#define M_PI 3.14159265358979323846

class Matrix4
{
  protected:
    double m[4][4];   // my implementation of the mult op seems to indicate that it's row-major
	// because matrix * vector means: row1col1 * v1 + row1col2 * v2 + ... row1coln * vn = V1 (the first element of the final vector)
	// so keep that in mind! Row-major! The reason I have to transpose a matrix usually is because opengl is technically "col-major"
    
  public:
    Matrix4();

	Matrix4(double m00, double m01, double m02, double m03,
		double m10, double m11, double m12, double m13,
		double m20, double m21, double m22, double m23,
		double m30, double m31, double m32, double m33);

    Matrix4& operator=(const Matrix4&);
    double* getPointer(); 
    
    Matrix4 transpose();
	
	Matrix4 operator*(Matrix4&);
	Vector4 operator*(Vector4&);
	Vector3 operator*(Vector3&);
	void print(std::string comment);

	static Matrix4 identity();
	static Matrix4 makeRotateX(double);
	static Matrix4 makeRotateY(double);
	static Matrix4 makeRotateZ(double);
	static Matrix4 makeRotate(double, Vector3&);
	static Matrix4 makeScale(double, double, double);
	static Matrix4 makeTranslate(double, double, double);
	static Matrix4 makeTranslate(Vector3);

	double * getRow(int index); // m[0][0], m[0][1], ...
	double * getCol(int index); // m[0][0], m[1][0], ...

	void setX(Vector3);
	void setY(Vector3);
	void setZ(Vector3);
};

#endif