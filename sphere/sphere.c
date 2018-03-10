#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>

// angle of rotation for the camera direction
float angle=0.0;
// actual vector representing the camera's direction
float lx=0.0f,lz=-1.0f;
// XZ position of the camera
float x=0.0f,z=5.0f;


void display(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(	x, 1.0f, z,
		x+lx, 1.0f,  z+lz,
		0.0f, 1.0f,  0.0f);

    glColor3f(.5,.5,.5);
    glBegin(GL_QUADS);
    glVertex3f(-100.0f,0.0f,-100.0f);
    glVertex3f( 100.0f,0.0f,-100.0f);
    glVertex3f( 100.0f,0.0f, 100.0f);
    glVertex3f(-100.0f,0.0f, 100.0f);	
    glEnd();


    glColor3f(1.0,0.0,0.0);
    glTranslatef(0.0,.25f, 0.0);
    glutSolidSphere(0.25f,20,20);
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

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(300, 300);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Sphere");
	glutDisplayFunc(display);
	glutReshapeFunc(changeSize);

	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
	return 0;
}
