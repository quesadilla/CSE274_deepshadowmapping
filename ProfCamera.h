////////////////////////////////////////
// camera.h
////////////////////////////////////////

#ifndef CSE169_CAMERA_H
#define CSE169_CAMERA_H

#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <algorithm>
#include "Vector3.h"


////////////////////////////////////////////////////////////////////////////////

class ProfCamera {
public:
	ProfCamera();

	void Update();
    void UpdateCam(Vector3 Min,Vector3 Max);
	void Reset();
	void Draw();

	// Access functions
	void SetAspect(float a)		{ Aspect = a; }
	void SetDistance(float d)	{ Distance = d; }
	void SetAzimuth(float a)	{ Azimuth = a; }
	void SetIncline(float i)	{ Incline = i; }

	float GetDistance()			{ return Distance; }
	float GetAzimuth()			{ return Azimuth; }
	float GetIncline()			{ return Incline; }

private:
	// Perspective controls
	float FOV;		// Field of View Angle
	float Aspect;	// Aspect Ratio
	float NearClip;	// Near clipping plane distance
	float FarClip;	// Far clipping plane distance

	// Polar controls
    float x,y,z;
	float Distance;	// Distance of the camera eye position to the origin
	float Azimuth;	// Rotation of the camera eye position around the Y axis
	float Incline;	// Angle of the camera eye position over the XZ plane
};

////////////////////////////////////////////////////////////////////////////////

/*
The Camera class provides a simple means to controlling the 3D camera. It could
be extended to support more interactive controls. Ultimately. the camera sets the
GL projection and viewing matrices.
*/

#endif
