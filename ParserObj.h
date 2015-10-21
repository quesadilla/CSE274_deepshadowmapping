#pragma once

#include <vector>
#include <iostream>
#include "Vector3.h"
#include "Matrix4.h"
#include "Light.h"
#include <math.h> 
#include <string>

class ParserObj
{
protected:
	std::vector<Vector3> vertexPositions;
	std::vector<Vector3> vertexColors;
	std::vector<Vector3> vertexNormals;
	std::vector<Vector3> facePositions;
	std::vector<Vector3> faceNormals;

	Matrix4 S;
	Matrix4 S2;
	Matrix4 model2world;

	double angleZ;
	double angleY;
	double angleX;
	double arbitAngle;
	Vector3 axis;

	double objScale;
	double objScale2;
	double minX;
	double maxX;
	double minY;
	double maxY;
	double minZ;
	double maxZ;
	double spinVelocity;

public:
	ParserObj();
	Vector3 getVertex(int);
	Vector3 getVertColor(int);
	Vector3 getVertNorm(int);
	Vector3 getFacePos(int);
	Vector3 getFaceNorm(int);

	void keyEvent(unsigned char);

	Matrix4& getMatrix();
	void setMatrix(Matrix4);

	Vector3 calculateAverage();
	void applyAverage(Vector3);

	void scaleY();	// scaling stuff
	void scaleX();
	void setScale(double);
	void setScale2(double);

	void incRotX(double);// rotation stuff
	void incRotY(double);
	void incRotZ(double);
	void setArbitAngle(double);
	void setAxis(Vector3);

	void update();
	void draw(Light *, Matrix4 *);
	int getFaceSize();
	~ParserObj();
	void parse(const char *);
};

