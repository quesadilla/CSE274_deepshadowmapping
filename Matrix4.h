
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
    double m[4][4];   // matrix elements; first index is for rows, second for columns (row-major)
						// WAIT, WHAT?? siiiigh...
    
  public:
    Matrix4();
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


	void setX(Vector3);
	void setY(Vector3);
	void setZ(Vector3);
};

#endif