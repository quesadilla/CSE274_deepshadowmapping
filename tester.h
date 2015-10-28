////////////////////////////////////////
// tester.h
////////////////////////////////////////

#ifndef CSE169_TESTER_H
#define CSE169_TESTER_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <GL/GLee.h>
#include <GL/glut.h>

#include "Matrix4.h"
#include "ParserObj.h"
#include "Light.h"
#include "ProfCamera.h"
#include <string>

////////////////////////////////////////////////////////////////////////////////

class Tester {
public:
	Tester(int argc,char **argv);
	~Tester();

	void Update();
	void Reset();
	void Draw();

	void Quit();

	// Event handlers
	void Resize(int x,int y);
	void Keyboard(int key,int x,int y);
	void MouseButton(int btn,int state,int x,int y);
	void MouseMotion(int x,int y);
	void SpecCallback(int key, int x, int y);
	void DisplayFPS();

	Vector3 trackBallMapping(int x, int y);

private:
	// shadow mapping------------
	//Camera & light positions
	float cameraPosition[3];
	float lightPosition[3];

	//Size of shadow map
	const int shadowMapSize = 512;

	//Textures
	GLuint shadowMapTexture;

	//-----------------

	// Window management
	int WindowHandle;
	int WinX,WinY;

	// Input
	bool LeftDown,MiddleDown,RightDown;
	int mousex,mousey;

	// FPS
	int frame_count = 0;
	int currTime = 0;
	float lastFrameTime = 0.0f;


	// -------- stuff from 167 project
	int width, height; 	            // window size


	// modes
	bool p5_bunny;

	double camX;
	double camY;
	double camZ;

	// mouse stuff
	static const int ROTATE = 1;
	static const int ZOOM = 0;
	static const int OFF = -1;
	float ROTSCALE;
	float ZOOMSCALE;

	int movement;

	//float cameraProjectionMatrix[16];
	Matrix4 cameraProjectionMatrix;
	Matrix4 cameraViewMatrix;
	Matrix4 lightProjectionMatrix;
	Matrix4 lightViewMatrix;

	// Components
	ProfCamera Cam;
	ParserObj bunny;
	Vector3 lastPos;
	Light rotLight;
};


#endif
