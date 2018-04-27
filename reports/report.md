# Playing With OpenGL

## Goal

The goal of this project was to create a simple three-dimensional envioronment in `C` throught the use of the Open Graphics Library (OpenGL).

## Learning Goals

Through working on this project I wanted to get to the point where I felt comfortable working OpenGL. Before now the extent of my interaction with OpenGL was copying code from stack overflow, I had no idea where someone who wanted to make a project from scratch would start. 

## Useful resources

The most useful resource I stumbled upon was [this](http://www.lighthouse3d.com/tutorials/glut-tutorial/) tutorial by lighthouse3d. This tutorial is really helpful in giving the steps to get started with 3D rendering and animation using GLUT (OpenGL Utility Toolkit).

Another useful resource was the GLUT [spec](https://www.opengl.org/resources/libraries/glut/spec3/spec3.html). This provided great in-depth documentation of GLUT and was very useful for expanding away from the tutorial.

## Project State

In it's current state the project, when run, displays a 3D environment with a sphere and dynamic lighting and shadows.

![Sphere](https://raw.githubusercontent.com/TShapinsky/PlayingWithOpenGL/master/reports/sphere.png)

### Mouse and Keyboard Control

Control via mouse and keyboard is accomplished through the use of function calllbacks registered in `main`:

```C
	glutKeyboardFunc(keyboard);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(motion);
	glutMouseFunc(mouse);
```

When the `keyboard` function is called it identifies which key the user pressed and updates the value of a movement vector accordingly, this value is multiplied by the duration of the frame so that motion is smooth across different systems. The vector must then be transformed into world space so the camera can move relative to where the camera is pointing:

```C
void keyboard(unsigned char key, int x, int y) {
	//initialize motion vector to zero
    float fx = 0, fz = 0;
	//calculate length of look vector
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
	//add rotated motion vector to camera position vector
    cx += fz*(lz/mag) - fx*(lx/mag);
    cz -= fz*(lx/mag) + fx*(lz/mag);   
}
```

When the mouse is moved the `motion` is called to handle rotating the camera, however the `motion` function relies on the `mouse` function to detect when the left mouse button is down. Again coordinate transforms are important for getting the code to run correctly.

```C
void motion(int x, int y) {
    int deltaX = x - lastX;
    int deltaY = y - lastY;
    lastX = x;
    lastY = y;
    if(down) {
		//calculated delta angles and do a rotation arround the camera look vector around the y axis
		float angle = -0.01f*deltaX;
		lz = lz*cos(angle) - lx*sin(angle);
		lx = lz*sin(angle) + lx*cos(angle);
		//calculate the delta angles and do a rotation arround the pitch axis
		angle = -0.01f*deltaY;
		float mag = sqrt(lz*lz + lx*lx);
		lz = lz*cos(angle) - ly*sin(angle);
		lx = lx*cos(angle) - ly*sin(angle);
		ly = mag*sin(angle) + ly*cos(angle);
    }
}

void mouse(int button, int state, int x, int y) {
	//Check if left button is down and set global variable accordingly
    if(button == GLUT_LEFT_BUTTON) {
		if(state == GLUT_DOWN) {
			down = 1;
		} else {
			down = 0;  
		}
    }
}
```

### Rendering and Lighting

Setting up rendering and lighting begins in the `main` function with the `glutInitDisplayMode` function. 
```C
glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
``` 
In this case the display is initialized with a depth buffer to allow for occlusion, and double buffering to allow for smooth animation.

Another step that needs to happen to set up rendering and lighting is that desired OpenGL features need to be enabled. This is also done in `main`, most of the names are fairly self explanatory. 
```C
//enabled lighting
glEnable(GL_LIGHTING);
//enabled the zeroth light (used later)
glEnable(GL_LIGHT0);
//Color lighting
glEnable(GL_COLOR_MATERIAL);
//enables smooth lighting shading
glShadeModel(GL_SMOOTH);
```

To finish the setup the render callback must be registered as both `glutIdleFunc` and `glutDisplayFunc`. This allows rendering to continue even when system events are not being received, allowing for smooth animation.

The actual rendering is split into several steps per frame. First information from the previous frame is cleared. Next, the scene is built (Camera, lights, and geometry). Finally the function `glutSwapBuffers` is called (this is because we told glut that we wanted double bufering) which swaps the front and back buffers, displaying to the user the render that was just built.

```C
void display(void) {
    updateFrameDuration();
    
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    //Camera
    gluLookAt(	cx, 1.0f, cz,
		cx+lx, 1.0f+ly,  cz+lz,
		0.0f, 1.0f,  0.0f);


    //Lights
    GLfloat whiteDiffuseLight[] = {0.0, 1.0, 1.0};
    GLfloat lightPosition[] = {sin(lightAngle),cos(lightAngle),1.0,1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteDiffuseLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    //rotates the light by 1 rad/s
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
```

## Reflection

Over the course of this project I was able to build up an understanding of OpenGL and its surrounding libraries which I didn't possess before. I would say that at this point I am pretty comfortable with setting up an OpenGL program from scratch which is a nice skill to add to my belt. The thing in this project that actually gave me the most trouble was getting the coordinate transforms to work in the way that I wanted them to. 

This project did meet the goals which I had outlined in the project proposal though I think in general my entire progress could have used more rigor.

I hope that you have enjoyed reading this report and have learned something about computer graphics.
