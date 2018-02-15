#include <stdio.h>     // - Just for some ASCII messages
#include "gl/glut.h"   // - An interface and windows management library
#define _USE_MATH_DEFINES
#include <math.h>

// angle of rotation for the camera direction
float angle=0.0;
float realAngle=0;
// XYZ position of the camera
float eyex = -5.0f, eyey = 3.0f, eyez = 25.0f;
// XYZ position of the camera center
float x2 = 0, y2 = 3, z2=20;

int cam = 1;
float test = 0;

float carx = 0.0;
float carz = 20.0;

int frame = 0;
int speed = 0;

bool willCollide(float x , float z){

	float boxX = carx + x;
	float boxZ = carz + z;

	boxX += cos(realAngle) * 4.2;
	boxZ += -sin(realAngle)  * 4.2;
	
	if( ( (boxX > -28 && boxX <28 ) && (boxZ < 14 && boxZ > -14) ) || ( (boxX > 69 || boxX < -69) || (boxZ < -35 || boxZ > 35) ) )
		return true;
	return false;
	
}


GLUquadric* quobj = gluNewQuadric();

void drawCar() {

	glTranslatef(0,1,0);

	// Draw Wheels
	for(int i = 0; i < 2; i++){
		for(int j=0; j < 2; j++) {
			glColor3f(0.0f, 0.0f, 0.0f); // black
			glPushMatrix();
			glTranslatef(i*3.0,0,j * 3.0);
			glutSolidTorus(0.2,0.4,30,30); // wheel
			glColor3f(1.0f, 1.0f, 1.0f);
			gluDisk(quobj, 0, 0.2, 30, 30); // kapaki
			glPopMatrix();
		}
		glPushMatrix();
		glTranslatef(i*3.0,0,0);
		gluCylinder(quobj, 0.2, 0.2, 3, 30, 30); // kylindros
		glPopMatrix();
	}

	glTranslatef(0 ,0.4,0.4);
	for(int i = 0; i < 2; i++){
		for(int j=0; j < 2; j++) {
			glColor3f(0.9f, 0.9f, 0.9f); // white
			glPushMatrix();
			glTranslatef(i*3.0,0,j * 2.2);
			glutSolidTorus(0.3,0.2,30,30); // laspotiras
			glPopMatrix();
		}
	}

	glPushMatrix();
	glColor3f(0, 0, 1); // blue
	glTranslatef(1.5,0.15,1.1);
	glScalef(1,0.35,0.7);
	glutSolidSphere(2.35,30,30); // body
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4ub(110, 200, 240, 100); // blue
	glTranslatef(1.5,0.9,1.1);
	glScalef(1,0.3,1);
	glutSolidSphere(1,30,30); // glass
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1); // white
	glTranslatef(3.6,0.1,1.1);
	glScalef(0.3,0.15,1);
	glRotatef(-30,0,0,1);
	glutSolidCube(2.35); // bumper
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1); // white
	glTranslatef(-0.7,0.4,1.1);
	glScalef(0.3,0.15,1);
	glRotatef(-30,0,0,1);
	glutSolidCube(2.35); // bumper
	glPopMatrix();
}

void changeSize(int w, int h)
{

// Prevent a divide by zero, when window is too short
// (you cant make a window of zero width).
if (h == 0)
h = 1;
float ratio = w * 1.0 / h;

// Use the Projection Matrix
glMatrixMode(GL_PROJECTION);

// Reset Matrix
glLoadIdentity();

// Set the viewport to be the entire window
glViewport(0, 0, w, h);

// Set the correct perspective.
gluPerspective(45.0f, ratio, 0.1f, 100.0f);

// Get Back to the Modelview
glMatrixMode(GL_MODELVIEW);
}

void renderScene(void) {

	// Clear Color and Depth Buffers

	glClearColor(0.1, 0.4, 0.7, 0.7);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();

	// Set the camera
	gluLookAt(	eyex, eyey, eyez,
			x2, y2,  z2,
			0.0f, 1.0f,  0.0f);

        // Draw ground
	glColor3f(0.3f, 0.3f, 0.3f);
	glBegin(GL_QUADS);
		glVertex3f(-100.0f, 0.0f, -100.0f);
		glVertex3f(-100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f, -100.0f);
	glEnd();
	
	glColor3f(1, 0, 0); // red
	glPushMatrix();
	//glScalef(1.6,10,0.8);
	glScalef(2,10,1);
	glRotatef(45,0,1,0);
	glRotatef(90,1,0,0);
	//aaglutSolidTorus(1,50,30,12); // outer wall
	glutSolidTorus(1,50,30,4); // outer wall
	glPopMatrix();
	
	glColor3f(0.6, 0.3, 0.3); // white
	glPushMatrix();
	//glScalef(1.7,8,0.8);
	glScalef(3,8,1.5);
	glRotatef(45,0,1,0);
	glRotatef(90,1,0,0);
	//glutSolidTorus(1,12,30,18); // inner wall
	glutSolidTorus(1,12,30,4); // inner wall
	glPopMatrix();
	
	if(willCollide(cos(realAngle) * speed * 0.01, -sin(realAngle)  * speed * 0.01)){
		speed = 0;
		angle += 90;
		if(angle >= 360) angle -= 360;
		realAngle = angle * M_PI / 180;

	}
	else {
		carx += cos(realAngle) * speed * 0.01;
		carz += -sin(realAngle)  * speed * 0.01;
	}
	
	frame++;
	printf("frame = %d\ncam = %d\neye x = %f\neye z = %f\ncar x = %f\ncar z = %f\nspeed = %d\nangle = %f\nrealAngle = %f\nsin(angle) = %f\ncos(angle) = %f\n\n",frame,cam,eyex,eyez,carx,carz,speed,angle,realAngle,sin(realAngle) ,cos(realAngle) );

	if(cam == 1){
		eyex = 7 * -cos(realAngle) + carx;
		eyez = 7 * sin(realAngle) + carz;
		x2 = carx;
		z2 = carz;
	}

	glTranslatef(carx,0,carz);

	glRotatef(angle,0,1,0);
	drawCar();

	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y)
{
	
	if (key == 'k'){
		printf("cam = %d\n",cam);
		if(cam == 1){
			eyex = 0.0f;
			eyey = 99.0f;
			eyez = 0.8f;
			x2 = 0;
			y2 = 0.0f;
			z2 = 0;
			cam = 0;
		}
		else{
			eyex = carx - 7;
			eyey = 3.0f;
			eyez = carz + 25;
			x2 = carx;
			y2 = 3.0f;
			z2 = carz + 25;
			cam = 1;
		}
	}
	else if(key == '+'){
		eyey -= 1.0f;
	}
	else if(key == '-'){
		eyey += 1.0f;
	}
	else if (key == 32)
		speed = 0;
	else if (key == 27)
		exit(0);
}

void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {
		case GLUT_KEY_LEFT :
			if(speed > 0){
				angle += 1;
				if(angle >= 360) angle -= 360;
				realAngle = angle * M_PI / 180;
			}
			break;
		case GLUT_KEY_RIGHT :
			if(speed > 0){
				angle -= 1;
				if(angle <= 0) angle +=360;
				realAngle = angle * M_PI / 180;
			}
			break;
		case GLUT_KEY_UP :
			if(speed < 5)
				speed += 1;
			break;
		case GLUT_KEY_DOWN :
			if(speed > 0)
				speed -= 1;
			break;
	}
}


int main(int argc, char **argv) {

	// init GLUT and create window

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(1800,900);
	glutCreateWindow("Car Game");

	
gluQuadricNormals(quobj, GLU_SMOOTH);

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);

	// OpenGL init
	glEnable(GL_DEPTH_TEST);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}
