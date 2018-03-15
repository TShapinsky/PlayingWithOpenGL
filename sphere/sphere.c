#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <GL/glut.h>
#include <GL/gl.h>

// angle of rotation for the camera direction
float angle=0.0;
// actual vector representing the camera's direction
float lx=0.0f,lz=-1.0f, ly=0.0f;
// XZ position of the camera
float cx=0.0f,cz=5.0f;

float lightAngle = 0.0f;

GLfloat whiteSpecularMaterial[] = {0.5, 0.5, 0.5};
GLfloat redDiffuseMaterial[] = {1.0, 0.0, 0.0};

//frame timing
struct timeval lastFrame, currentFrame;
float frameDuration;

//mouse
int lastX, lastY;
int down;

float updateFrameDuration(void) {
    gettimeofday(&currentFrame, NULL);
    frameDuration = (currentFrame.tv_sec - lastFrame.tv_sec)  + (currentFrame.tv_usec - lastFrame.tv_usec)/1000000.0f;
    lastFrame = currentFrame;
}

void display(void) {
    updateFrameDuration();
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(	cx, 1.0f, cz,
		cx+lx, 1.0f+ly,  cz+lz,
		0.0f, 1.0f,  0.0f);


    //Lights
    GLfloat whiteDiffuseLight[] = {0.0, 1.0, 1.0};
    GLfloat lightPosition[] = {sin(lightAngle),cos(lightAngle),1.0,1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteDiffuseLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    lightAngle += 1.0*frameDuration;

    //Ground
    glColor3f(.9,.9,.9);
    glBegin(GL_QUADS);
    glVertex3f(-100.0f,0.0f,-100.0f);
    glVertex3f( 100.0f,0.0f,-100.0f);
    glVertex3f( 100.0f,0.0f, 100.0f);
    glVertex3f(-100.0f,0.0f, 100.0f);	
    glEnd();

    //Objects
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whiteSpecularMaterial);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redDiffuseMaterial);
    glColor3f(1.0,0.0,0.0);
    glTranslatef(0.0,.25f, 0.0);
    glutSolidSphere(0.25f,20,20);

    //Push Frame
    glutSwapBuffers();
}

void changeSize(int w, int h) {
    
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;
	float ratio = 1.0* w / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

        // Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,0.1,100);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}


void keyboard(unsigned char key, int x, int y) {
    float fx = 0, fz = 0;
    float angle = atan(lx/lz);
    float mag = sqrt(lz*lz + lx*lx);
    switch(key) {
    case 'w':
	fx -= 1.0*frameDuration;
	break;
    case 's':
	fx += 1.0*frameDuration;
	break;
    case 'a':
	fz += 1.0*frameDuration;
	break;
    case 'd':
	fz -= 1.0*frameDuration;
	break;
    }
    cx += fz*(lz/mag) - fx*(lx/mag);
    cz -= fz*(lx/mag) + fx*(lz/mag);

    
}

void motion(int x, int y) {
    int deltaX = x - lastX;
    int deltaY = y - lastY;
    lastX = x;
    lastY = y;
    if(down) {
	float angle = -0.01f*deltaX;
	lz = lz*cos(angle) - lx*sin(angle);
	lx = lz*sin(angle) + lx*cos(angle);
	angle = -0.01f*deltaY;
	float mag = sqrt(lz*lz + lx*lx);
	lz = lz*cos(angle) - ly*sin(angle);
	lx = lx*cos(angle) - ly*sin(angle);
	ly = mag*sin(angle) + ly*cos(angle);
    }
}

void mouse(int button, int state, int x, int y) {
    if(button == GLUT_LEFT_BUTTON) {
	if(state == GLUT_DOWN) {
	    down = 1;
	} else {
	    down = 0;  
	}
    }
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(300, 300);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Sphere");
	glutReshapeFunc(changeSize);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glutIdleFunc(display);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(motion);
	glutMouseFunc(mouse);

	gettimeofday(&lastFrame,NULL);
	glutMainLoop();
	return 0;
}
