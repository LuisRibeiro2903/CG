#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

int slices = 10;

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


void drawCylinder(float radius, float height, int slices) {
	glColor3f(1,1,1);
	float alpha = 2 * M_PI / slices;
	glBegin(GL_TRIANGLES);

		for(int i = 0; i < slices; i++)
		{
			float alpha_now = i * alpha;
			glColor3f(1,0,0);
			glVertex3d(0,0,0);
			glVertex3d(sin(alpha_now + alpha) * radius, 0, cos(alpha_now + alpha) * radius);
			glVertex3d(sin(alpha_now) * radius, 0, cos(alpha_now) * radius);

			glVertex3d(0,height,0);
			glVertex3d(sin(alpha_now) * radius, height, cos(alpha_now) * radius);
			glVertex3d(sin(alpha_now + alpha) * radius, height, cos(alpha_now + alpha) * radius);

			glColor3f(0,1,0);
			glVertex3d(sin(alpha_now + alpha) * radius, height, cos(alpha_now + alpha) * radius);
			glVertex3d(sin(alpha_now) * radius, height, cos(alpha_now) * radius);
			glVertex3d(sin(alpha_now) * radius, 0, cos(alpha_now) * radius);

			glColor3f(0,0.75,0);
			glVertex3d(sin(alpha_now + alpha) * radius, height, cos(alpha_now + alpha) * radius);
			glVertex3d(sin(alpha_now) * radius, 0, cos(alpha_now) * radius);
			glVertex3d(sin(alpha_now + alpha) * radius, 0, cos(alpha_now + alpha) * radius);

		}
	glEnd();

}


float cam_alpha = 0, cam_beta = 0;

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	float px = 8.66 * cos(cam_beta) * sin(cam_alpha), py = 8.66 * sin(cam_beta), pz = 8.66 * cos(cam_alpha) * cos(cam_beta);
	gluLookAt(px,py,pz, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

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

	
	drawCylinder(1,2,slices);

	// End of frame
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here
	switch(c)
		{
			case '+':
				slices++;
				break;
			case '-':
				slices--;
				break;
			case 'f':
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				break;
			case 'l':
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				break;
		}


		glutPostRedisplay();

}


void processSpecialKeys(int key, int xx, int yy) {

// put code to process special keys in here
	switch (key)
	{
	case GLUT_KEY_UP:
		if(cam_beta < 1.5)
			cam_beta += 0.1;
		break;
	case GLUT_KEY_DOWN:
		if(cam_beta > -1.5)
			cam_beta -= 0.1;
		break;
	case GLUT_KEY_LEFT:
		cam_alpha -= 0.1;
		break;
	case GLUT_KEY_RIGHT:
		cam_alpha += 0.1;
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
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
