#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <chrono>


void changeSize(int w, int h)
{
	// Prevent a divide by zero, when window is too short
	// (you can�t make a window with zero width).
	if (h == 0)
		h = 1;
	// compute window's aspect ratio
	float ratio = w * 1.0f / h;
	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load the identity matrix
	glLoadIdentity();
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);
	// Set the perspective
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void renderScene(void)
{
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// set camera
	glLoadIdentity();
	auto now = std::chrono::system_clock::now();
	auto time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
	GLdouble eyeZ = 5.0f * (sin(time_ms.count() * 0.001) + 2);
	GLdouble centerZ =(sin(time_ms.count()*0.01));
	GLdouble upX =(sin(time_ms.count()*0.01));
	
	gluLookAt(0.0f, 0.0f, eyeZ,
		centerZ, 0.0f, -1.0f,
		upX, 1.0f, 0.0f);
		
	// put drawing instructions here
	glutWireTeapot(1);
	
	// End of frame
	glutSwapBuffers();
}


int main(int argc, char** argv)
{
	// put GLUT�s init here
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(550, 150);
	glutInitWindowSize(800, 800);
	glutCreateWindow("CG@DI");
	
	// put callback registry here
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	glutDisplayFunc(renderScene);
	
	// some OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	// enter GLUT�s main cycle
	glutMainLoop();
	
	return 1;
}