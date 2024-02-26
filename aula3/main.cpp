#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

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
	glColor3f(1,0,0);
	glBegin(GL_TRIANGLE_FAN);
		glVertex3d(0,0,0);
		for(int i = 0; i <= slices; i++)
		{
			float alpha_now = i * alpha;
			glVertex3d(sin(alpha_now) * radius, 0, cos(alpha_now) * radius);
		}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
		glVertex3d(0,height,0);
		for(int i = 0; i <= slices; i++)
		{
			float alpha_now = i * alpha;
			glVertex3d(sin(alpha_now) * radius, height, cos(alpha_now) * radius);
		}
	glEnd();
	glColor3f(0,1,0);
	glBegin(GL_TRIANGLE_STRIP);
		for(int i = 0; i <= slices; i++) {
			float alpha_now = i * alpha;
			glVertex3f(sin(alpha_now) * radius, height, cos(alpha_now) * radius);
			glVertex3f(sin(alpha_now) * radius, 0, cos(alpha_now) * radius);
		}
	glEnd();
	

}


float cam_alpha = 0, cam_beta = 0;
float px = 0, py = 0, pz = 8.66, lx = 0, ly = 0, lz = 0;
bool explorer = true;

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(px,py,pz, 
		      lx,ly,lz,
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
		case '1':
			explorer = true;
			cam_alpha = 0, cam_beta = 0;
			px = 0, py = 0, pz = 8.66;
			lx = 0, ly = 0, lz = 0;
			break;
		case '2':
			explorer = false;
			cam_alpha = (5 * M_PI ) / 4, cam_beta = 0;
			px = 5, py = 1, pz = 5;
			lx = px + sin(cam_alpha);
			ly = 1;
			lz = pz + cos(cam_alpha);
			break;
	}

	glutPostRedisplay();

}

void processSpecialKeysFPS(int key)
{
	switch(key)
	{
		case GLUT_KEY_UP:
			px = lx;
			pz = lz;
			break;
		case GLUT_KEY_DOWN:
			px = px + (px - lx);
			pz = pz + (pz - lz);
			break;
		case GLUT_KEY_LEFT:
			cam_alpha += 0.1;
			break;
		case GLUT_KEY_RIGHT:
			cam_alpha -= 0.1;
			break;
		default:
			break;
	}

	lx = px + sin(cam_alpha);
	lz = pz + cos(cam_alpha);
}


void processSpecialKeysExplorer(int key)
{
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

	px = 8.66 * cos(cam_beta) * sin(cam_alpha);
	py = 8.66 * sin(cam_beta);
	pz = 8.66 * cos(cam_alpha) * cos(cam_beta);
}


void processSpecialKeys(int key, int xx, int yy) {

// put code to process special keys in here
	if (explorer)
		processSpecialKeysExplorer(key);
	else
		processSpecialKeysFPS(key);

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
