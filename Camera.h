/* author: Janis Jimenez */
#pragma once

#include "Matrix4.h"
#include "Vector3.h"
class Camera
{
	protected:
		Matrix4 camera2world;// camera matrix is the matrix from camera to world
		Matrix4 world2camera; // the inverted camera matrix
		Vector3 e;// projection
		Vector3 d;// look-at vector
		Vector3 up;// up vector

	public:
		Camera();
		Camera(Vector3, Vector3, Vector3);
		~Camera();
		Matrix4 getGLMatrix();
		void recalculateMatrix(Vector3, Vector3, Vector3);
		void setE(Vector3);
		Vector3 getE();
		void setD(Vector3);
		Vector3 getD();
		void setUp(Vector3);
		Vector3 getUp();
};

