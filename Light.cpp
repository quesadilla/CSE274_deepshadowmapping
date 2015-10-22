#include "Light.h"

Light::Light()
{
	spinning = 1.5;

	// light properties 0
	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 }; // default
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 }; // default
	GLfloat light_position[] = { 1.0, 2.0, 1.0, 0.0 };

	// light properties 1
	GLfloat light_ambient1[] = { 0.0, 0.0, 0.0, 1.0 }; // default
	GLfloat light_diffuse1[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat light_specular1[] = { 1.0, 1.0, 1.0, 1.0 }; // default
	GLfloat light_position1[] = { 1.0, 0.0, 4.0, 1.0 };
	GLfloat spot_direction[] = { 0.0, 0.0, -1.0 };

	// set light properties 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);


	// set light properties 1
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	lightMatrix = Matrix4::identity();
	lightMatrix = Matrix4::makeTranslate(1, 2, 1) * lightMatrix;

	spotlightMatrix = Matrix4::identity();
	spotlightMatrix = Matrix4::makeTranslate(1, 0, 4) * spotlightMatrix;

}

void Light::bunnyMaterial()
{
	// material properties -> move these to a Material class
	GLfloat mat_ambient[] = {0.2, 0.2, 0.2, 1.0};
	GLfloat mat_shininess[] = { 0.0 };
	GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };

	// set material properties
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
}

void Light::bearMaterial()
{
	// material properties -> move these to a Material class
	GLfloat mat_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	// set material properties
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
}

void Light::dragonMaterial()
{
	// material properties -> move these to a Material class
	GLfloat mat_ambient[] = { 1.0, 1.0, 0.2, 1.0 };
	GLfloat mat_shininess[] = { 25.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	// set material properties
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
}

void Light::display()
{
	glFlush();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ---------------  Light 0

	Matrix4 transpose = lightMatrix.transpose();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(transpose.getPointer());


	//glDisable(GL_LIGHTING);
	glColor3f(0.0, 0.0, 1.0);
	glutSolidSphere(0.8, 20, 20);
	//glEnable(GL_LIGHTING);

	// --------- Light 1

	Matrix4 transpose1 = spotlightMatrix.transpose();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(transpose1.getPointer());

	//glDisable(GL_LIGHTING);
	glColor3f(1.0, 0.0, 0.0);
	glutSolidCone(0.5, 1, 20, 20);
	//glEnable(GL_LIGHTING);

	glFlush();

}

Matrix4& Light::getMatrix()
{
	return lightMatrix;
}

int Light::getSpin()
{
	return spinning;
}

void Light::update()
{
	//spinning += 1.5;
	lightMatrix = Matrix4::identity() * Matrix4::makeTranslate(2, 4, 2);
	spotlightMatrix = Matrix4::identity() * Matrix4::makeTranslate(-2, -3, 8);

}

Light::~Light()
{
}
