#pragma once

#include "Matrix4.h"
#include <GL/glut.h>

class Light
{
	protected:
		Matrix4 lightMatrix;
		Matrix4 spotlightMatrix;
		double spinning;

public:
	Light();

	void display(); // show the light
	void update(); // update matrix
	void bunnyMaterial();
	void dragonMaterial();
	void bearMaterial();
	Matrix4& getMatrix();
	int getSpin();
	void resetLightValues();

	~Light();
};

