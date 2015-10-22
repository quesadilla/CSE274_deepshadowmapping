////////////////////////////////////////
// tester.cpp
////////////////////////////////////////

#include "tester.h"

#define WINDOWTITLE	"Spinning Cube"

////////////////////////////////////////////////////////////////////////////////

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

Tester::Tester(int argc,char **argv) {

	//Check for necessary extensions
	if (!GLEE_ARB_depth_texture || !GLEE_ARB_shadow)
	{
		printf("I require ARB_depth_texture and ARB_shadow extensionsn\n");
	}

	width = 512;   // set window width in pixels here
	height = 512;   // set window height in pixels here
	p5_bunny = true;
	shader = false;

	camX = 0;
	camY = 0;
	camZ = 0;

	// mouse stuff
	movement = OFF;
	ROTSCALE = 10.0;
	ZOOMSCALE = .004;

	float specular[] = { 1.0, 1.0, 1.0, 1.0 };
	float shininess[] = { 100.0 };
	float position[] = { 0.0, 10.0, 1.0, 0.0 };	// lightsource position

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);   // open an OpenGL context with double buffering, RGB colors, and depth buffering
	glutInitWindowSize(width, height);      // set initial window size
	WindowHandle = glutCreateWindow("CSE 274");    	      // open window and set window title
	glutSetWindow(WindowHandle);

	rotLight = Light();

	glEnable(GL_NORMALIZE);

	// parse bunny
	std::cout << "parsing bunny..." << std::endl;
	bunny = ParserObj();
	bunny.parse("C:/Users/Jade/Documents/Graphics/OBJs/bunny.obj");
	//bunny.parse("C:/Users/Jade/Documents/Visual Studio 2013/Projects/CSE274_deepshadowmapping/hairball_simplified.obj");

	lastPos = Vector3();

	//startShader();

	WinX=640;
	WinY=480;
	LeftDown=MiddleDown=RightDown=false;
	mousex=mousey=0;

	Cam.SetAspect(float(WinX) / float(WinY));

	// Background color
	glClearColor( 0., 0., 0., 1. );

	// Callbacks
	glutDisplayFunc( display );
	glutReshapeFunc(resize);
	glutIdleFunc( idle );
	glutKeyboardFunc( keyboard );

	glutSpecialFunc(speccallback);

	glutMouseFunc( mousebutton );
	glutMotionFunc( mousemotion );
	glutPassiveMotionFunc( mousemotion );

	// --------- shadow mapping tutorial ---------
	cameraPosition = Vector3(0.0f, 0.0f, -20.0f);
	lightPosition = Vector3(1.0f, 0.0f, 4.0f);





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

	if (p5_bunny)
	{
		bunny.update();
	}


	//Draw();
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

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);  // make sure we're in Modelview mode

	Matrix4 glmatrix;

	Cam.Draw();

	/* draw a bunny for assignment 5 */
	if (p5_bunny)
	{
		bunny.draw(&rotLight, &glmatrix);
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

	// project 5 bunny
	if (key == GLUT_KEY_F4)
	{
		p5_bunny = true;
	}


}


////////////////////////////////////////////////////////////////////////////////

void Tester::Keyboard(int key,int x,int y) {
	if (key == 'p')
	{
		shader = (!shader);
		if (shader == true)
		{
			//glDisable(GL_LIGHT1);
			fancyShaders->bind();
			std::cout << "bound!" << std::endl;
		}
		else
		{
			//glEnable(GL_LIGHT1);
			fancyShaders->unbind();
			std::cout << "unbound!" << std::endl;
		}
	}


	else if (p5_bunny)
	{
		bunny.keyEvent(key);
	}




	//switch(key) {
	//	case 0x1b:		// Escape
	//		Quit();
	//		break;
	//	case 'E':
	//	case 'e':		// begin collapsing all edges
	//		beginMorph = true;
	//		break;
	//	case 'Q':
	//	case 'q':		// begin undoing all edge collapses
	//		undoMorph = true;
	//		break;
	//	case '1':		// switch to smooth shading
	//		glShadeModel(GL_SMOOTH);
	//		break;
	//	case '2':		// switch to flat shading
	//		glShadeModel(GL_FLAT);
	//		break;
	//	case '3':		// toggle colors
	//		mesh.coloredTriangles = !mesh.coloredTriangles;
	//		break;
	//	case 'W':
 //       case 'w':mesh.MoveY(-1); // translation stuff
 //           break;
	//	case 'S':
 //       case 's':mesh.MoveY(1);
 //           break;
	//	case 'A':
 //       case 'a':mesh.MoveX(1);
 //           break;
	//	case 'D':
 //       case 'd':mesh.MoveX(-1);
 //           break;
	//	case 'I':
 //       case 'i':mesh.MoveZ(-1); // zooming stuff
 //           break;
	//	case 'O':
 //       case 'o':mesh.MoveZ(1);
 //           break;
	//	case 'r':
	//		Reset();
	//		break;
	//}
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

	//Vector3 direction;
	//float pixel_diff;
	//float rot_angle;
	//float zoom_factor;
	//Vector3 curPoint;


	//switch (movement)
	//{
	//case ROTATE: // left mouse button is held down
	//{
	//	curPoint = trackBallMapping(nx, ny); // map position to a sphere location
	//	direction = curPoint - lastPos;
	//	float velocity = direction.length();

	//	if (velocity > 0.0001) // If little movement - do nothing
	//	{
	//		std::cout << "ROTATING" << std::endl;

	//		// rotate around axis that's perpendicular to circle connecting mouse movements
	//		Vector3 rotAxis = curPoint.cross(lastPos);
	//		//Vector3 rotAxis = Globals::lastPos.cross(curPoint);
	//		rotAxis.print("rotation axis");
	//		rotAxis = rotAxis.normalize();
	//		//Vector3 rotAxis = Vector3(0, 1, 0);
	//		rot_angle = velocity * ROTSCALE;

	//		if (p5_bunny)
	//		{
	//			bunny.setArbitAngle(rot_angle);
	//			bunny.setAxis(rotAxis);

	//		}
	//	}
	//	break;
	//}
	//case ZOOM:
	//{
	//	std::cout << "ZOOMING" << std::endl;
	//	pixel_diff = nx - lastPos.getX();
	//	zoom_factor = 1.0 + pixel_diff * ZOOMSCALE;

	//	if (p5_bunny)
	//	{
	//		bunny.setScale2(zoom_factor);
	//	}
	//	break;
	//}
	//}

	//lastPos = curPoint;



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