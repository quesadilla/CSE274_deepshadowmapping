//////////////////////////////////////////////////////////////////////////////////////////
//	Scene.cpp
//	Draw the scene for shadow mapping
//	Downloaded from: www.paulsprojects.net
//	Created:	16th September 2003
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "Maths/Maths.h"
#include "scene.h"
#include "main.h"
void DrawScene(float angle, bool drawHairball)
{
	if (drawHairball) { DrawModel(hairball); }
	else { DrawModel(bunny); }

	glPushMatrix();

	glTranslatef(0.0f, 1.0f, 2.0f);
	glRotatef(90, 1, 0.0f, 0.0f);
	glScalef(6.0f, 1.0f, 6.0f);
	glutSolidCube(0.5f);

	glPopMatrix();

	//Display lists for objects
	static GLuint spheresList=0, torusList=0, baseList=0;

	//Create spheres list if necessary
	if(!spheresList)
	{
		spheresList=glGenLists(1);
		glNewList(spheresList, GL_COMPILE);
		{
			glColor3f(0.0f, 1.0f, 0.0f);
			glPushMatrix();

			glTranslatef(0.45f, 1.0f, 0.45f);
			glutSolidSphere(0.2, 24, 24);

			glTranslatef(-0.9f, 0.0f, 0.0f);
			glutSolidSphere(0.2, 24, 24);

			glTranslatef(0.0f, 0.0f,-0.9f);
			glutSolidSphere(0.2, 24, 24);

			glTranslatef(0.9f, 0.0f, 0.0f);
			glutSolidSphere(0.2, 24, 24);

			glPopMatrix();
		}
		glEndList();
	}

	//Create torus if necessary
	if(!torusList)
	{
		torusList=glGenLists(1);
		glNewList(torusList, GL_COMPILE);
		{
	//		glColor3f(1.0f, 0.0f, 0.0f);
	//		glPushMatrix();

	//		glTranslatef(0.0f, 0.5f, 0.0f);
	//		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
			//glutSolidTorus(0.2, 0.5, 24, 48);
			
	

			glPopMatrix();
		}
		glEndList();
	}

	//Create base if necessary
	if(!baseList)
	{
		baseList=glGenLists(1);
		glNewList(baseList, GL_COMPILE);
		{
			glColor3f(0.0f, 0.0f, 1.0f);
			glPushMatrix();

			glScalef(1.0f, 0.05f, 1.0f);
			glutSolidCube(3.0f);

			glPopMatrix();
		}
		glEndList();
	}


	//Draw objects
	glCallList(baseList);
	glCallList(torusList);
	
	glPushMatrix();
	glRotatef(angle, 0.0f, 1.0f, 0.0f);
	glCallList(spheresList);
	glPopMatrix();
}


void DrawModel(ParserObj model) 
{
	glPushMatrix();
	glRotatef(180.0, 0.0, 1.0, 0.0);

	if (model.getName() == "hairball")
	{
		glScalef(0.25, 0.25, 0.25);
	}
	else { glScalef(0.6, 0.6, 0.6); }
	glTranslatef(0.5, 1, 0);
	glBegin(GL_TRIANGLES);
	int indexPos = -1;
	int indexNorm = -1;
	float colorR = 0;
	float colorG = 0;
	float colorB = 0;
	float vertexX = 0;
	float vertexY = 0;
	float vertexZ = 0;
	float normX = 0;
	float normY = 0;
	float normZ = 0;

	int size = model.getFaceSize();

	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

	for (int i = 0; i < size; i++)
	{

		// for each vertex, get the required indices for positions and normals
		// then draw them

		// FIRST vertex
		// get required indices
		indexPos = (model.getFacePos(i).getX()) - 1; // faces start at 1 but first index should be 0
		indexNorm = (model.getFaceNorm(i).getX()) - 1; // same as above

		vertexX = model.getVertex(indexPos).getX(); // set vertex x y z positions
		vertexY = model.getVertex(indexPos).getY();
		vertexZ = model.getVertex(indexPos).getZ();

		colorR = model.getVertColor(indexPos).getX(); // set vertex r g b color
		colorG = model.getVertColor(indexPos).getY();
		colorB = model.getVertColor(indexPos).getZ();

		normX = model.getVertNorm(indexNorm).getX(); // set vertex normal x y z positions
		normY = model.getVertNorm(indexNorm).getY();
		normZ = model.getVertNorm(indexNorm).getZ();

		glNormal3f(normX, normY, normZ);
		glVertex3f(vertexX, vertexY, vertexZ);
		glColor3f(colorR, colorG, colorB);

		// SECOND vertex
		// get required indices
		indexPos = (model.getFacePos(i).getY()) - 1; // faces start at 1 but first index should be 0
		indexNorm = (model.getFaceNorm(i).getY()) - 1; // same as above

		vertexX = model.getVertex(indexPos).getX(); // set vertex x y z positions
		vertexY = model.getVertex(indexPos).getY();
		vertexZ = model.getVertex(indexPos).getZ();

		colorR = model.getVertColor(indexPos).getX(); // set vertex r g b color
		colorG = model.getVertColor(indexPos).getY();
		colorB = model.getVertColor(indexPos).getZ();

		normX = model.getVertNorm(indexNorm).getX(); // set vertex normal x y z positions
		normY = model.getVertNorm(indexNorm).getY();
		normZ = model.getVertNorm(indexNorm).getZ();

		glNormal3f(normX, normY, normZ);
		glVertex3f(vertexX, vertexY, vertexZ);
		glColor3f(colorR, colorG, colorB);


		// THIRD vertex
		// get required indices
		indexPos = (model.getFacePos(i).getZ()) - 1; // faces start at 1 but first index should be 0
		indexNorm = (model.getFaceNorm(i).getZ()) - 1; // same as above

		vertexX = model.getVertex(indexPos).getX(); // set vertex x y z positions
		vertexY = model.getVertex(indexPos).getY();
		vertexZ = model.getVertex(indexPos).getZ();

		colorR = model.getVertColor(indexPos).getX(); // set vertex r g b color
		colorG = model.getVertColor(indexPos).getY();
		colorB = model.getVertColor(indexPos).getZ();

		normX = model.getVertNorm(indexNorm).getX(); // set vertex normal x y z positions
		normY = model.getVertNorm(indexNorm).getY();
		normZ = model.getVertNorm(indexNorm).getZ();

		glNormal3f(normX, normY, normZ);
		glVertex3f(vertexX, vertexY, vertexZ);
		glColor3f(colorR, colorG, colorB);


	}
	glEnd();
	glPopMatrix();

}