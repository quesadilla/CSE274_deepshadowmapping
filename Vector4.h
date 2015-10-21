
#pragma once
#include <string>
#include <iostream>
#include "Vector3.h"

class Vector4
{
public:

	Vector4() { x = 0; y = 0; z = 0; w = 1; };

	Vector4(double x0, double y0, double z0, double w0) { x = x0; y = y0; z = z0; w = w0; };

	~Vector4() { };

	Vector4 operator+(const Vector4& v) { return Vector4(x + v.x, y + v.y, z + v.z, w); };
	Vector4 operator-(const Vector4& v) { return Vector4(x - v.x, y - v.y, z - v.z, w); };
	Vector4 dehomogenize() { return Vector4(x / w, y / w, z / w, 1); };
	Vector3 dehomogenize3() { return Vector3(x / w, y / w, z / w); };
	void print(std::string comment) { std::cout << comment << " (" << x << ", " << y << ", " << z << ", " << w << ")" << std::endl; };

	double getX() { return x; };
	double getY() { return y; };
	double getZ() { return z; };
	double getW() { return w; };
private:
	double w, x, y, z;
};

