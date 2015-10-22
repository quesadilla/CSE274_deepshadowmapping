////////////////////////////////////////
// tester.h
////////////////////////////////////////

#ifndef CSE169_TESTER_H
#define CSE169_TESTER_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "GLee.h"

#include "Matrix4.h"
#include "shader.h"
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
	Vector3 cameraPosition;
	Vector3 lightPosition;

	//Size of shadow map
	const int shadowMapSize = 512;

	//Textures
	GLuint shadowMapTexture;

	//Matrices
	Matrix4 lightProjectionMatrix, lightViewMatrix;
	Matrix4 cameraProjectionMatrix, cameraViewMatrix;

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
	Shader * fancyShaders;


	// modes
	bool p5_bunny;
	bool shader;

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

	// Components
	ProfCamera Cam;
	ParserObj bunny;
	Vector3 lastPos;
	Light rotLight;
};

////////////////////////////////////////////////////////////////////////////////

/*
The 'Tester' is a simple top level application class. It creates and manages a
window with the GLUT extension to OpenGL and it maintains a simple 3D scene
including a camera and some other components.
*/

#endif
