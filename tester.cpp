////////////////////////////////////////
// tester.cpp
////////////////////////////////////////

#include "tester.h"

#define WINDOWTITLE	"Spinning Cube"

////////////////////////////////////////////////////////////////////////////////

GLfloat black[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat gray[4] = { 0.2f, 0.2f, 0.2f, 1.0f };

static Tester *TESTER;
using namespace std;


int main(int argc, char **argv) {
	glutInit(&argc, argv);
	TESTER = new Tester(argc,argv);
	glutMainLoop();
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

// These are really HACKS to make glut call member functions instead of static functions
static void display()									{TESTER->Draw();}
static void idle()										{TESTER->Update();}
static void resize(int w,int h)							{TESTER->Resize(w,h);}
static void keyboard(unsigned char key,int x,int y)		{TESTER->Keyboard(key,x,y);}
static void mousebutton(int btn,int state,int x,int y)	{TESTER->MouseButton(btn,state,x,y);}
static void mousemotion(int x, int y)					{TESTER->MouseMotion(x,y);}
static void speccallback(int key, int x, int y)			{ TESTER->SpecCallback(key, x, y); }

////////////////////////////////////////////////////////////////////////////////

Tester::Tester(int argc, char **argv) {
	cameraPosition[0] = 0.0f;
	cameraPosition[1] = 0.0f;
	cameraPosition[2] = -Cam.GetDistance();

	lightPosition[0] = 1.0f;
	lightPosition[1] = 0.0f;
	lightPosition[2] = 4.0f;

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	width = 512;   // set window width in pixels here
	height = 512;   // set window height in pixels here
	shadowMap = false;
	hairModel = false;

	camX = 0;
	camY = 0;
	camZ = 0;

	// mouse stuff
	movement = OFF;
	ROTSCALE = 10.0;
	ZOOMSCALE = .004;

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);   // open an OpenGL context with double buffering, RGB colors, and depth buffering
	glutInitWindowSize(width, height);      // set initial window size
	WindowHandle = glutCreateWindow("CSE 274");    	      // open window and set window title
	glutSetWindow(WindowHandle);

	rotLight = Light();

	// parse bunny
	std::cout << "parsing bunny..." << std::endl;
	bunny = ParserObj();
	bunny.parse("C:/Users/Jade/Documents/Graphics/OBJs/bunny.obj");
	std::cout << "done parsing bunny. " << std::endl;

	// parse hairball
	std::cout << "parsing hairball..." << std::endl;
	hairball = ParserObj();
	hairball.parse("C:/Users/Jade/Documents/Visual Studio 2013/Projects/CSE274_deepshadowmapping/hairball_simplified.obj");
	hairball.setScale(.60);
	hairball.setScale2(1.20);
	lastPos = Vector3();

	WinX = 640;
	WinY = 480;
	LeftDown = MiddleDown = RightDown = false;
	mousex = mousey = 0;

	Cam.SetAspect(float(WinX) / float(WinY));

	// Background color
	//glClearColor(0., 0., 0., 1.);

	// Callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);

	glutSpecialFunc(speccallback);

	glutMouseFunc(mousebutton);
	glutMotionFunc(mousemotion);
	glutPassiveMotionFunc(mousemotion);


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// --------- shadow mapping tutorial ---------


	if (!GLEE_ARB_depth_texture || !GLEE_ARB_shadow)
	{
		printf("I require ARB_depth_texture and ARB_shadow extensionsn\n");
	}

	glMatrixMode(GL_MODELVIEW); // load modelview matrix
	glLoadIdentity();

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f); 
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);

	glEnable(GL_NORMALIZE);

	// create shadow map texture
	glGenTextures(1, &shadowMapTexture);
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapSize, shadowMapSize, 0,
		GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	// set color to ambient + diffuse
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glMaterialfv(GL_FRONT, GL_SPECULAR, white); // I might not need this?
	glMaterialf(GL_FRONT, GL_SHININESS, 16.0f);

	glPushMatrix();

	// calculate camera projection matrix
	glLoadIdentity();
	gluPerspective(60.0f, 1.33f, 0.1f, 1000.0f);
	glGetDoublev(GL_MODELVIEW_MATRIX, cameraProjectionMatrix.getPointer());

	//cameraProjectionMatrix = cameraProjectionMatrix.transpose();

	// calculate camera view matrix
	glLoadIdentity();
	gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2],
		0.0f, 0.0f, -5.0f,
		0.0f, 1.0f, 0.0f);
	glGetDoublev(GL_MODELVIEW_MATRIX, cameraViewMatrix.getPointer());

	//cameraViewMatrix = cameraViewMatrix.transpose();

	// calculate light projection matrix
	glLoadIdentity();
	gluPerspective(45.0f, 1.0f, 2.0f, 8.0f);
	glGetDoublev(GL_MODELVIEW_MATRIX, lightProjectionMatrix.getPointer());

	lightProjectionMatrix = lightProjectionMatrix.transpose();

	// calculate light view matrix
	glLoadIdentity();
	gluLookAt(lightPosition[0], lightPosition[1], lightPosition[2],
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f);
	glGetDoublev(GL_MODELVIEW_MATRIX, lightViewMatrix.getPointer());

	lightViewMatrix = lightViewMatrix.transpose();

	glPopMatrix();

	glLightfv(GL_LIGHT1, GL_AMBIENT, gray);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white);

	//glColorMaterial(GL_FRONT, GL_DIFFUSE); // these two lines are VERY IMPORTANT
	//glEnable(GL_COLOR_MATERIAL); // in order to get glColor to work

}

////////////////////////////////////////////////////////////////////////////////

Tester::~Tester() {
	glFinish();
	glutDestroyWindow(WindowHandle);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Update() {

	Cam.Update();
	currTime = glutGet(GLenum(GLUT_ELAPSED_TIME));

	cameraPosition[0] = 0.0f;
	cameraPosition[1] = 0.0f;
	cameraPosition[2] = -Cam.GetDistance();

	if (hairModel)
	{
		hairball.update();
	}
	else
	{
		bunny.update();
	}
	glutSetWindow(WindowHandle);
	glutPostRedisplay();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Reset() {
	Cam.Reset();
	Cam.SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Draw() {

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color and depth buffers
	//glMatrixMode(GL_MODELVIEW);  // make sure we're in Modelview mode
	Matrix4 glmatrix;
	Cam.Draw();

	//First pass - from light's point of view
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	if (!shadowMap)
	{
		// so it seems that whatever's messing up the drawing is coming from loading these two matrices...
		//glLoadIdentity();
		//gluPerspective(60.0f, 1.33f, 0.1f, 1000.0f);
		//glGetDoublev(GL_MODELVIEW_MATRIX, cameraProjectionMatrix.getPointer());


		//glMatrixMode(GL_PROJECTION);
		//glLoadMatrixd(cameraProjectionMatrix.getPointer());

		//glMatrixMode(GL_MODELVIEW);
		//glLoadMatrixd(cameraViewMatrix.getPointer());

		if (hairModel)
		{
			hairball.draw(&rotLight, &glmatrix);
		}
		else
		{
			bunny.draw(&rotLight, &glmatrix);
		}
		
	}

	else{
		glDisable(GL_LIGHTING);

		//glMatrixMode(GL_PROJECTION);
		//glLoadMatrixd(lightProjectionMatrix.getPointer());

		//lightProjectionMatrix.print("lightProjectionMatrix:");

		//glMatrixMode(GL_MODELVIEW);
		//glLoadMatrixd(lightViewMatrix.getPointer());
		gluLookAt(
			1, 0, 4,
			0, 0, -1,
			0,1,0);

		//lightViewMatrix.print("lightViewMatrix:");

		//Use viewport the same size as the shadow map
		glViewport(0, 0, shadowMapSize, shadowMapSize);

		//Draw back faces into the shadow map
		glCullFace(GL_FRONT);

		//Disable color writes, and use flat shading for speed
		glShadeModel(GL_FLAT);
		glColorMask(0, 0, 0, 0);

		//Draw the scene
		if (hairModel) { hairball.draw(&rotLight, &glmatrix); }
		else { bunny.draw(&rotLight, &glmatrix); }

		//Read the depth buffer into the shadow map texture
		glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, shadowMapSize, shadowMapSize);

		//restore states
		glCullFace(GL_BACK);
		glShadeModel(GL_SMOOTH);
		glColorMask(1, 1, 1, 1);




		//2nd pass - Draw from camera's point of view
		glClear(GL_DEPTH_BUFFER_BIT);

		//glMatrixMode(GL_PROJECTION);
		//glLoadMatrixd(cameraProjectionMatrix.getPointer());

		//glMatrixMode(GL_MODELVIEW);
		//glLoadMatrixd(cameraViewMatrix.getPointer());

		glViewport(0, 0, width, height);
		Cam.Draw();

		//Use dim light to represent shadowed areas
		//glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
		glLightfv(GL_LIGHT1, GL_AMBIENT, gray);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, gray);
		glLightfv(GL_LIGHT1, GL_SPECULAR, black);
		glEnable(GL_LIGHT1);
		glDisable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
		
		if (hairModel) { hairball.draw(&rotLight, &glmatrix); }
		else { bunny.draw(&rotLight, &glmatrix); }



		//3rd pass
		//Draw with bright light
		glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
		glLightfv(GL_LIGHT1, GL_SPECULAR, white);

		//Calculate texture matrix for projection
		//This matrix takes us from eye space to the light's clip space
		//It is postmultiplied by the inverse of the current view matrix when specifying texgen
		Matrix4 biasMatrix(0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.5f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f);	//bias from [-1, 1] to [0, 1]

		// transpose matrices to row-major before multiplying them
		Matrix4 textureMatrix = (biasMatrix.transpose()) * (lightProjectionMatrix.transpose()) * (lightViewMatrix.transpose());

		// now transpose textureMatrix back to column for opengl stuff
		textureMatrix = textureMatrix.transpose();
		//// or we can just get the necessary elements in row-major

		////Set up texture coordinate generation.
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
		glTexGendv(GL_S, GL_EYE_PLANE, textureMatrix.getCol(0)); // textureMatrix.GetRow(0));
		glEnable(GL_TEXTURE_GEN_S);

		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
		glTexGendv(GL_T, GL_EYE_PLANE, textureMatrix.getCol(1)); //textureMatrix.GetRow(1));
		glEnable(GL_TEXTURE_GEN_T);

		glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
		glTexGendv(GL_R, GL_EYE_PLANE, textureMatrix.getCol(2)); //textureMatrix.GetRow(2));
		glEnable(GL_TEXTURE_GEN_R);

		glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
		glTexGendv(GL_Q, GL_EYE_PLANE, textureMatrix.getCol(3)); //textureMatrix.GetRow(3));
		glEnable(GL_TEXTURE_GEN_Q);

		////Bind & enable shadow map texture
		glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
		glEnable(GL_TEXTURE_2D);

		////Enable shadow comparison
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE);

		////Shadow comparison should be true (ie not in shadow) if r<=texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);

		////Shadow comparison should generate an INTENSITY result
		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);

		////Set alpha test to discard false comparisons
		glAlphaFunc(GL_GEQUAL, 0.99f);
		glEnable(GL_ALPHA_TEST);

		if (hairModel) { hairball.draw(&rotLight, &glmatrix); }
		else { bunny.draw(&rotLight, &glmatrix); }

		////Disable textures and texgen
		glDisable(GL_TEXTURE_2D);

		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_GEN_R);
		glDisable(GL_TEXTURE_GEN_Q);

		////Restore other states
		glDisable(GL_LIGHTING);
		glDisable(GL_ALPHA_TEST);


		//Set matrices for ortho
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		//reset matrices
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

	}

	DisplayFPS();

	glFlush();
	glutSwapBuffers();

}

////////////////////////////////////////////////////////////////////////////////

void Tester::Quit() {
	glFinish();
	glutDestroyWindow(WindowHandle);
	exit(0);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Resize(int w,int h) {
	cerr << "Window::reshapeCallback called" << endl;

	WinX = w;
	WinY = h;
	Cam.SetAspect(float(WinX) / float(WinY));

	//width = w;
	//height = h;
	//glViewport(0, 0, w, h);  // set new viewport size
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(60.0, double(width) / (double)height, 1.0, 1000.0); // set perspective projection viewing frustum
	//glTranslatef(0, 0, -20);
	//glMatrixMode(GL_MODELVIEW);


	//Update the camera's projection matrix
	//glPushMatrix();
	//glLoadIdentity();
	//gluPerspective(45.0f, (float)width / height, 1.0f, 100.0f);
	//glGetDoublev(GL_MODELVIEW_MATRIX, cameraProjectionMatrix.getPointer());
	//glPopMatrix();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::SpecCallback(int key, int x, int y) {
	// robot mode keys
	// move camera left and right
	if (key == GLUT_KEY_LEFT)
	{
		camX += .2;
	}
	else if (key == GLUT_KEY_RIGHT)
	{
		camX -= .2;
	}

	// robot mode keys
	// move camera up and down
	if (key == GLUT_KEY_UP)
	{
		camY -= .2;
	}
	else if (key == GLUT_KEY_DOWN)
	{
		camY += .2;
	}

	// toggle shadowmap
	if (key == GLUT_KEY_F1)
	{
		shadowMap = !shadowMap;
		std::cout << "shadowmap is now " << shadowMap << std::endl;

		// restore original properties of light 1
		if (!shadowMap) { 
			glLightfv(GL_LIGHT1, GL_AMBIENT, gray);
			glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
			glLightfv(GL_LIGHT1, GL_SPECULAR, white);
		}
	}

	// switch between models
	if (key == GLUT_KEY_F2)
	{
		hairModel = !hairModel;
		std::cout << "hairModel is now " << hairModel << std::endl;
	}

}


////////////////////////////////////////////////////////////////////////////////

void Tester::Keyboard(int key, int x, int y) {

	if (hairModel)
	{
		hairball.keyEvent(key);
	}
	else
	{
		bunny.keyEvent(key);
	}

}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseButton(int btn,int state,int x,int y) {

	// on left mouse button down
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		std::cout << "about to rotate" << std::endl;
		// turn on user interactive rotations
		movement = ROTATE;

		// keep track of previous position (ignore z position?)
		lastPos = trackBallMapping(x, y); // actually you should pass this into the trackBallMapping method

		// make sure you're in model view
		glMatrixMode(GL_MODELVIEW);
	}
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		// turn off user interactive rotations
		movement = OFF;
	}

	// on right mouse button down
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		std::cout << "about to zoom" << std::endl;

		lastPos = trackBallMapping(x, y);

		// turn on user interactive zoom
		movement = ZOOM;
	}
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		// turn off user interactive zoom
		movement = OFF;
	}


	if(btn==GLUT_LEFT_BUTTON) {
		LeftDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_MIDDLE_BUTTON) {
		MiddleDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_RIGHT_BUTTON) {
		RightDown = (state==GLUT_DOWN);
	}
}

////////////////////////////////////////////////////////////////////////////////

Vector3 Tester::trackBallMapping(int x, int y)
{
	Vector3 v;
	double vx;
	double vy;
	double vz;
	float d;

	// calculate vector
	vx = (2.0 * x - double(width)) / double(width);
	vy = (double(height) - 2.0 * y) / double(height);
	vz = 0.0;
	v = Vector3(vx, vy, vz);

	d = v.length();
	d = (d<1.0) ? d : 1.0;

	vz = sqrtf(1.001 - d*d);
	v.setZ(vz);

	v = v.normalize();
	return v;
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseMotion(int nx,int ny) {
	int dx = nx - mousex;
	int dy = -(ny - mousey);

	mousex = nx;
	mousey = ny;

	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if(LeftDown) {
		const float rate=1.0f;
		Cam.SetAzimuth(Cam.GetAzimuth()+dx*rate);
		Cam.SetIncline(Cam.GetIncline()-dy*rate);
	}
	if(RightDown) {
		const float rate=0.01f;
		Cam.SetDistance(Cam.GetDistance()*(1.0f-dx*rate));
	}
}

////////////////////////////////////////////////////////////////////////////////

/* Displays amount of frames per second in the top bar of the window
*/
void Tester::DisplayFPS()
{
	frame_count++;
	if (currTime - lastFrameTime > 1000)
	{
		char title[16];
		sprintf_s(title, "FPS: %4.2f",
			frame_count * 1000.0 / (currTime - lastFrameTime));
		glutSetWindowTitle(title);
		lastFrameTime = (float)currTime;
		frame_count = 0;
	}
}