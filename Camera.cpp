/* author: Janis Jimenez */
#include "Camera.h"

Camera::Camera()
{
	//e = Vector3(0,10,10);
	//d = Vector3(0,0,0);
	e = Vector3(0,0,0);
	d = Vector3(0,0,0);
	up = Vector3(0,1,0);

	recalculateMatrix(e,d,up);
}

Camera::Camera(Vector3 e2, Vector3 d2, Vector3 up2)
{
	e = e2;
	d = d2;
	up = up2;

	recalculateMatrix(e2, d2, up2);
}
// basic camera matrix
// Xc  Yc  Zc  e
// 0   0   0   1

/* Takes in the three points and recalculates Camera and inverse Camera matrices */
void Camera::recalculateMatrix(Vector3 someE, Vector3 someD, Vector3 someUp)
{
	// create camera matrix
	camera2world = Matrix4::identity();
	Vector3 z = (someE - someD).normalize();
	Vector3 x = someUp.cross(z).normalize();
	Vector3 y = z.cross(x).normalize();
	camera2world = camera2world.makeTranslate(someE);

	// inverse translation matrix
	Matrix4 T_inverse = camera2world.makeTranslate(someE.getX()* (-1), someE.getY() * (-1), someE.getZ() * (-1));

	// inverse rotation matrix
	Matrix4 R_inverse = Matrix4::identity();
	R_inverse.setX(x);
	R_inverse.setY(y);
	R_inverse.setZ(z);
	R_inverse = R_inverse.transpose();

	// finish setting up regular camera matrix
	camera2world.setX(x);
	camera2world.setY(y);
	camera2world.setZ(z);

	// calculate inverse
	world2camera = R_inverse * T_inverse;

	// set to column-major
	world2camera = world2camera.transpose();
	camera2world = camera2world.transpose();
}


/* returns the inverted camera matrix */
Matrix4 Camera::getGLMatrix()
{
	return world2camera;
}

/* changes the projection point */
void Camera::setE(Vector3 someE)
{
	e = someE;
}

Vector3 Camera::getE()
{
	return e;
}

/* changes the look-at point */
void Camera::setD(Vector3 someD)
{
	d = someD;
}

Vector3 Camera::getD()
{
	return d;
}

/* changes the up-vector */
void Camera::setUp(Vector3 someUp)
{
	up = someUp;
}

Vector3 Camera::getUp()
{
	return up;
}

Camera::~Camera()
{
}
