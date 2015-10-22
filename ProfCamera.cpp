////////////////////////////////////////
// camera.cpp
////////////////////////////////////////

#include "ProfCamera.h"

#define PI 3.14159265
////////////////////////////////////////////////////////////////////////////////

using namespace std;

ProfCamera::ProfCamera() {
	Reset();
}

////////////////////////////////////////////////////////////////////////////////

void ProfCamera::Update() {
}
void ProfCamera::UpdateCam(Vector3 Min,Vector3 Max)
{
    float maxX = max(fabs(Min.x),fabs(Max.x));
    float maxY = max(fabs(Max.y),fabs(Min.y));
    float maxZ = max(fabs(Max.z),fabs(Min.z));

    float a = max(maxX,maxY);
	a = max(a, maxZ);
    float g;
   /* if(a > 1)
    {
        g = maxZ;
    }
    else
    { */
        g = a;
   // }
    Distance = fmax (g, a / tan (FOV/2));
    if(Distance >= 1)
    {
        Distance *= 2;
    }
}
////////////////////////////////////////////////////////////////////////////////

void ProfCamera::Reset() {
	FOV = 60.0f;
	Aspect = 1.33f;
	NearClip = 0.1f;
	FarClip = 1000.0f;

	Distance = 5.0f;
    x = 0, y = 0;
	Azimuth = 0.0f;
	Incline = 0.0f;
}

////////////////////////////////////////////////////////////////////////////////

void ProfCamera::Draw() {
	// Tell GL we are going to adjust the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Set perspective projection
	gluPerspective(FOV, Aspect, NearClip, FarClip);

	// Place camera
	//glTranslatef(-x, -y, -z);
    glTranslatef(0, 0, -Distance);
	glRotatef(Incline, 1.0f, 0.0f, 0.0f);
	glRotatef(Azimuth, 0.0f, 1.0f, 0.0f);
    

	// Return to modelview matrix mode
	glMatrixMode(GL_MODELVIEW);
}

////////////////////////////////////////////////////////////////////////////////
