#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <math.h>

bool drawPyramid = false;
bool fill = false;
float TRANSLATE_Z = 0.0;
void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(10.0,10.0,30.0, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);
			  
	
	// Translate here also Translates the axis
	// Translate is unfinished. Needs to also have global variables in x and y.
	glTranslatef(0,0,TRANSLATE_Z);
	
	// put axis drawing in here
	glBegin(GL_LINES);
		// X axis in red
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-100.0f, 0.0f, 0.0f);
		glVertex3f( 100.0f, 0.0f, 0.0f);
		// Y Axis in Green
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, -100.0f, 0.0f);
		glVertex3f(0.0f, 100.0f, 0.0f);
		// Z Axis in Blue
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, -100.0f);
		glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();


	// put the geometric transformations here

	// Translate here only Translates the object
	//glTranslatef(0,0,TRANSLATE_Z);

	// put pyramid drawing instructions here

	if (fill)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	if(drawPyramid)
	{

		glBegin(GL_TRIANGLES);

			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3d(0.0, 5.0, 0.0);
			glVertex3d(-2.5, 0.0, -2.5);
			glVertex3d(2.5, 0.0, -2.5);

			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3d(0.0, 5.0, 0.0);
			glVertex3d(2.5, 0.0, 2.5);
			glVertex3d(2.5, 0.0, -2.5);

			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3d(0.0, 5.0, 0.0);
			glVertex3d(-2.5, 0.0, 2.5);
			glVertex3d(2.5, 0.0, 2.5);


			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex3d(0.0, 5.0, 0.0);
			glVertex3d(-2.5, 0.0, 2.5);
			glVertex3d(-2.5, 0.0, -2.5);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 0.0f);
			glVertex3d(-2.5,0.0,-2.5);
			glVertex3d(-2.5,0.0,2.5);
			glVertex3d(2.5,0.0,2.5);
			glVertex3d(2.5,0.0,-2.5);
		glEnd();
	}



	// End of frame
	glutSwapBuffers();
}



// write function to process keyboard events



void handleKeyboard(unsigned char key, int x, int y)
{

	switch(key)
	{
		case 'p':
			drawPyramid = (drawPyramid) ? false: true;
			break;
		case 'f':
			fill = !fill;
			break;
	}


	glutPostRedisplay();
}

void handleSpecialKeys(int key_code, int x, int y)
{
	switch (key_code)
	{
	case GLUT_KEY_UP:
		TRANSLATE_Z++;
		break;
	case GLUT_KEY_DOWN:
		TRANSLATE_Z--;
		break;
	default:
		break;
	}

	glutPostRedisplay();
}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	
	glutKeyboardFunc(handleKeyboard);
	glutSpecialFunc(handleSpecialKeys);
// put here the registration of the keyboard callbacks



//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
