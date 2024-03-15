#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <chrono>

float alfa = 0.0f, beta = 0.5f, radius = 100.0f;
float camX, camY, camZ;
int seed = 29112003;
int r_clareira = 35;


void spherical2Cartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}


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

void drawAxis()
{
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
}


void drawTree ()
{
	glPushMatrix();
	glColor3f(0.5,0.35,0.05);
	glRotatef(-90, 1, 0, 0);
	glutSolidCone(1, 5, 10, 10);
	glTranslatef(0, 0, 3);
	glColor3f(0,1,0);
	glutSolidCone(2, 5, 10, 10);
	glPopMatrix();
}

void drawTrees(int num_trees)
{
	srand(seed);
	int i = 0;
	while (i < num_trees)
	{
		int rand_x = rand() % 200 - 100;
		int rand_z = rand() % 200 - 100;
		if (rand_x * rand_x + rand_z * rand_z > r_clareira * r_clareira)
		{
			glPushMatrix();
			glTranslatef(rand_x, 0, rand_z);
			drawTree();
			glPopMatrix();
			i++;	
		}
	}
}

void drawTorus () {
	glPushMatrix();
	glColor3f(0.922, 0.059, 0.459);
	glTranslatef(0, 2.5, 0);
	glutSolidTorus(0.5, 2, 100, 100);
	glPopMatrix();
}

void drawCowboys(int cowboys)
{
    float step = 360 / float(cowboys);
    glColor3f(0.0, 0.0, 1.0);
    glPushMatrix();
    glTranslatef(0, 1, 0);
    auto now = std::chrono::system_clock::now();
    auto time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    float rotationAngle = (time_ms.count() / 10) % 360;
    
    for (int i = 0; i < cowboys; i++)
    {
        glPushMatrix();
        glRotatef((i * step) + rotationAngle, 0, 1, 0);
        glTranslatef(5, 0, 0);
        glutSolidTeapot(1);
        glPopMatrix();
    }
    glPopMatrix();
}


void drawAttackers(int attackers)
{
	float step = 360 / float(attackers);
	glColor3f(1.0, 0.0, 0.0);
	glPushMatrix();
	glTranslatef(0, 1.25, 0);
	auto now = std::chrono::system_clock::now();
    auto time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    float rotationAngle = (time_ms.count() / 20) % 360; 
	for (int i = 0; i < attackers; i++)
	{
		glPushMatrix();
		glRotatef((i * step) - rotationAngle, 0, 1, 0);
		glTranslatef(0, 0, 25);
		glutSolidTeapot(1.25);
		glPopMatrix();
	}
	glPopMatrix();
}


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	glColor3f(0.2f, 0.8f, 0.2f);
	glBegin(GL_TRIANGLES);
		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);

		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);
		glVertex3f(100.0f, 0, 100.0f);
	glEnd();
	
	// put code to draw scene in here
	drawAxis();
	drawTrees(1000);
	drawTorus();
	drawCowboys(8);
	drawAttackers(16);
	

	
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here

}


void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {

	case GLUT_KEY_RIGHT:
		alfa -= 0.1; break;

	case GLUT_KEY_LEFT:
		alfa += 0.1; break;

	case GLUT_KEY_UP:
		beta += 0.1f;
		if (beta > 1.5f)
			beta = 1.5f;
		break;

	case GLUT_KEY_DOWN:
		beta -= 0.1f;
		if (beta < -1.5f)
			beta = -1.5f;
		break;

	case GLUT_KEY_PAGE_DOWN: radius -= 10.0f;
		if (radius < 1.0f)
			radius = 1.0f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 10.0f; break;
	}
	spherical2Cartesian();
	glutPostRedisplay();

}


void printInfo() {

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse Arrows to move the camera up/down and left/right\n");
	printf("Home and End control the distance from the camera to the origin");
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
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	spherical2Cartesian();

	printInfo();

// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
