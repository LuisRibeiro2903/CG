

#include<stdio.h>
#include<stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

#include <chrono>
#include <IL/il.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif


float camX = 00, camY = 30, camZ = 40;
int startX, startY, tracking = 0;

int alpha = 0, beta = 45, r = 50;

int altura, largura;

unsigned char *imageData;

int seed = 29112003;
int r_clareira = 35;

GLuint vertices[2];

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the correct perspective
	gluPerspective(45,ratio,1,1000);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


float h (int i, int j) {

	float scale = (50 / float(255)) * 2;
	return (imageData[i * largura + j] * scale);
}

float hf(float px, float pz){

	int x1 = floor(px);
	int x2 = x1 + 1;
	int z1 = floor(pz);
	int z2 = z1 + 1;

	float fz = pz - z1;
	float fx = px - x1;

	float h_x1_z = h(x1, z1) * (1 - fz) + h(x1, z2) * fz;
	float h_x2_z = h(x2, z1) * (1 - fz) + h(x2, z2) * fz;

	float height_xz = h_x1_z * (1 - fx) + h_x2_z * fx;

	return height_xz;
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
		int rand_x = rand() % 255;
		int rand_z = rand() % 255;
		if (pow((rand_x - 127.5),2) + pow((rand_z - 127.5),2) > r_clareira * r_clareira)
		{
			glPushMatrix();
			glTranslatef(rand_x, hf(rand_x, rand_z), rand_z);
			drawTree();
			glPopMatrix();
			i++;	
		}
	}
}

void drawTorus () {
	glPushMatrix();
	glColor3f(0.922, 0.059, 0.459);
	glTranslatef(127.5, 2.5, 127.5);
	glutSolidTorus(0.5, 2, 100, 100);
	glPopMatrix();
}

void drawCowboys(int cowboys)
{
    float step = 360 / float(cowboys);
    glColor3f(0.0, 0.0, 1.0);
    glPushMatrix();
    glTranslatef(127.5, 1, 127.5);
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
	glTranslatef(127.5, 1.25, 127.5);
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


void drawTerrain() {

    // colocar aqui o código de desnho do terreno usando VBOs com TRIANGLE_STRIPS
	glBindBuffer(GL_ARRAY_BUFFER, vertices[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	for(int i = 0; i < altura -1; i++) {

		glDrawArrays(GL_TRIANGLE_STRIP, largura * 2 * i, largura * 2);
	}

}


void drawAxis()
{
	glBegin(GL_LINES);
		// X axis in red
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-255, 0.0f, 0.0f);
		glVertex3f( 255, 0.0f, 0.0f);
		// Y Axis in Green
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, -255, 0.0f);
		glVertex3f(0.0f, 255, 0.0f);
		// Z Axis in Blue
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, -255);
		glVertex3f(0.0f, 0.0f, 255);
	glEnd();
}


void renderScene(void) {

	float pos[4] = {-1.0, 1.0, 1.0, 0.0};

	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(camX, camY, camZ, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

	drawAxis();
	glColor3f(0.114, 0.659, 0.114);
	glPushMatrix();
	glTranslatef(-127.5, 0, -127.5);
	drawTerrain();
	drawTrees(1000);
	drawTorus();
	drawCowboys(10);
	drawAttackers(10);
	glPopMatrix();

	

// End of frame
	glutSwapBuffers();
}



void processKeys(unsigned char key, int xx, int yy) {

// put code to process regular keys in here
}



void processMouseButtons(int button, int state, int xx, int yy) {
	
	if (state == GLUT_DOWN)  {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
		else
			tracking = 0;
	}
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha += (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			
			r -= yy - startY;
			if (r < 3)
				r = 3.0;
		}
		tracking = 0;
	}
}


void processMouseMotion(int xx, int yy) {

	int deltaX, deltaY;
	int alphaAux, betaAux;
	int rAux;

	if (!tracking)
		return;

	deltaX = xx - startX;
	deltaY = yy - startY;

	if (tracking == 1) {


		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0)
			betaAux = 85.0;
		else if (betaAux < -85.0)
			betaAux = -85.0;

		rAux = r;
	}
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r - deltaY;
		if (rAux < 3)
			rAux = 3;
	}
	camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camY = rAux * 							     sin(betaAux * 3.14 / 180.0);
}




void init() {

// 	Load the height map "terreno.jpg"
	unsigned int t, tw, th;
	ilInit();
	ilGenImages(1,&t);
	ilBindImage(t);
	// terreno.jpg is the image containing our height map
	ilLoadImage((ILstring)"terreno.jpg");
	// convert the image to single channel per pixel
	// with values ranging between 0 and 255
	ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);
	// important: check tw and th values
	// both should be equal to 256
	// if not there was an error loading the image
	// most likely the image could not be found
	tw = ilGetInteger(IL_IMAGE_WIDTH);
	th = ilGetInteger(IL_IMAGE_HEIGHT);
	// imageData is a LINEAR array with the pixel values
	imageData = ilGetData();

// 	Build the vertex arrays
	altura = th;
	largura = tw;

	std::vector<float> v;

	for(int y = 0; y < th; y++) {
		for(int x = 0; x < tw; x++) {
			v.push_back(y + 1);
			v.push_back(h(y + 1, x));
			v.push_back(x);

			v.push_back(y);
			v.push_back(h(y, x));
			v.push_back(x);
			
		}
	}

	glGenBuffers(1, vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vertices[0]);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(float), v.data(), GL_STATIC_DRAW);
	
// 	OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
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
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);

	glewInit();
	glEnableClientState(GL_VERTEX_ARRAY);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	init();	

// enter GLUT's main cycle
	glutMainLoop();
	
	return 0;
}

