
#pragma once
#include <string>
#include <iostream>
class Vector3
{
public:
	Vector3() { x = 0; y = 0; z = 0; };
	Vector3(double x0, double y0, double z0) { x = x0; y = y0; z = z0; };
	~Vector3() {};
	Vector3 operator+(const Vector3& v) { return Vector3(x + v.x, y + v.y, z + v.z); };
	Vector3 operator-(const Vector3& v) { return Vector3(x - v.x, y - v.y, z - v.z); };
	Vector3 operator*(double d) { return Vector3(d*x, d*y, d*z); };
	Vector3 negate() { return Vector3(-x, -y, -z); };
	double dot(const Vector3& v) { return x * v.x + y * v.y + z * v.z; };
	Vector3 cross(const Vector3& v) { return Vector3(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x); };
	Vector3 normalize() { double l = length(); return Vector3(x / l, y / l, z / l); };
	double length() { return std::sqrt(x*x + y*y + z*z); };
	void print(std::string comment) { std::cout << comment << " (" << x << ", " << y << ", " << z << ")" << std::endl; };
	double getX() { return x; };
	double getY() { return y; };
	double getZ() { return z; };
	void setX(double x0) { x = x0; };
	void setY(double y0) { y = y0; };
	void setZ(double z0) { z = z0; };

private:
	double x, y, z;
};

