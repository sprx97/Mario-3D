// Jeremy Vercillo
// 2/9/12

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLUT/glut.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>
#include <CoreGraphics/CoreGraphics.h>
#include <CoreGraphics/CGEventSource.h>
#include <QuartzCore/QuartzCore.h>
// libraries

#include "Cube.h"
#include "../common/shader_utils.h"
// local includes

#ifndef GLUT_KEY_ESC
#define GLUT_KEY_ESC 27
#endif

#define PI 3.1415926

using namespace std;

GLuint program;
GLint attribute_coord3d, attribute_texcoord;
GLint uniform_mvp;

int screen_width = 800, screen_height = 600; // screen size
int oldx = screen_width/2;
int oldy = screen_height/2; // last mouse position
int midwindowx = screen_width/2; // Middle of the window horizontally
int midwindowy = screen_height/2; // Middle of the window vertically
bool fullscreen = true;
bool mouseinit = false;

static glm::vec3 angle;
static glm::vec3 forward;
static glm::vec3 right;
static glm::vec3 lookat;
static glm::vec3 position;

int keys[256] = {0}; // array of whether keys are pressed

int cubesize = 2;
int numcubes = 100;
Cube* cubes[100]; // array of cubes
Cube* bg; // background skycube

glm::mat4 view, projection;

float movespeed = 0.001;
float mousespeed = 0.001;

float lastframe = 0; // last frame in ms from GLUT_ELAPSED_TIME
float MAX_FPS = 60.0; // 60 frames per second

int initShaders() {
	GLuint vs, fs;
	if((vs = create_shader("cubeshader.v.glsl", GL_VERTEX_SHADER)) == 0) return 0;
	if((fs = create_shader("cubeshader.f.glsl", GL_FRAGMENT_SHADER)) == 0) return 0;
	// creates shaders from my files
	
	GLint link_ok = GL_FALSE;
	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if(!link_ok) {
		fprintf(stderr, "glLinkProgram:");
		return 0;
	}
	// checks linking of shaders
	
	const char* attribute_name = "coord3d";
	attribute_coord3d = glGetAttribLocation(program, attribute_name);
	if(attribute_coord3d == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return 0;
	}
	attribute_name = "texcoord";
	attribute_texcoord = glGetAttribLocation(program, attribute_name);
	if(attribute_texcoord == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return 0;
	}
	// checks binding of attributes
	
	const char* uniform_name;
	uniform_name = "mvp";
	uniform_mvp = glGetUniformLocation(program, uniform_name);
	if(uniform_mvp == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
		return 0;
	}
	
	return 1;
}

float distance(float x1, float y1, float z1, float x2, float y2, float z2) {
	return sqrt((x2*x2-x1*x1) + (y2*y2-y1*y1) + (z2*z2-z1*z1));
} // distance between 2 points

void update_vectors() {
	forward.x = sinf(angle.x);
	forward.y = 0;
	forward.z = cosf(angle.x);
	
	right.x = -cosf(angle.x);
	right.y = 0;
	right.z = sinf(angle.x);
	
	lookat.x = sinf(angle.x) * cosf(angle.y);
	lookat.y = sinf(angle.y);
	lookat.z = cosf(angle.x) * cosf(angle.y);
	
	screen_width = glutGet(GLUT_WINDOW_WIDTH);
	screen_height = glutGet(GLUT_WINDOW_HEIGHT);
	oldx = midwindowx = screen_width/2;
	oldy = midwindowy = screen_height/2;
	CGWarpMouseCursorPosition(CGPointMake(midwindowx, midwindowy));
} // updates camera position vectors

void motion(int x, int y) {
	bool warp = false;
	angle.x -= (x - oldx) * mousespeed /** dt*/;
	angle.y -= (y - oldy) * mousespeed /** dt*/;
	if (x <= 0) {
		oldx = midwindowx;
		x = midwindowx;
	}
	if (x >= screen_width) {
		oldx = midwindowx;
		x = midwindowx;
	}
	if (y >= screen_height) {
		oldy = midwindowy;
		y = midwindowy;
	}
	if (y <= 0) {
		oldy = midwindowy;
		y = midwindowy;
	}
	if(angle.x < -M_PI) angle.x += M_PI * 2;
	if(angle.x > M_PI) angle.x -= M_PI * 2;
	if(angle.y < -M_PI * 0.49) angle.y = -M_PI * 0.49;
	if(angle.y > M_PI * 0.49) angle.y = M_PI * 0.49;
	oldx = x;
	oldy = y;	
} // moves camera based on current key states

void moveCamera() {
    if(keys['a']) position -= right * movespeed;
	if(keys['d']) position += right * movespeed;
	if(keys['w']) position += forward * movespeed;
	if(keys['s']) position -= forward * movespeed;
	if(keys['r']) position.y += movespeed;
	if(keys['t']) position.y -= movespeed;
	
	update_vectors();
}

void idle() {
	if(!mouseinit) {
		screen_width = glutGet(GLUT_WINDOW_WIDTH);
		screen_height = glutGet(GLUT_WINDOW_HEIGHT);
		oldx = midwindowx = screen_width/2;
		oldy = midwindowy = screen_height/2;
		CGWarpMouseCursorPosition(CGPointMake(midwindowx, midwindowy));
		mouseinit = true;
	}
	moveCamera();
} // constantly calculates redraws

void timer(int value) {
	glutPostRedisplay();
	glutTimerFunc((1000.0/MAX_FPS), timer, 0);
}

void drawCube(Cube* c) {
	glBindTexture(GL_TEXTURE_2D, c->texture_id);
	
	glBindBuffer(GL_ARRAY_BUFFER, c->vbo_texcoords);
	glVertexAttribPointer(attribute_texcoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, c->vbo_vertices);
	glVertexAttribPointer(attribute_coord3d, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, c->ibo_elements);
	int size; glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(c->xpos, c->ypos, c->zpos));
	// translate to position from origin
	glm::mat4 mvp = projection * view * model;	
	glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
	glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
} // draws a cube

void onDisplay() {
	glClearColor(0.0, 0.0, 0.0, 0.0); // black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// clears the screen

/*	glUseProgram(program);

	GLfloat color[] = {0.5f, 1.0f, 0.0f, 1.0f};
	GLfloat position[] = {10.0f, 10.0f, 10.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glColor3f(1.0f, 0.0f, 0.0f);
	GLUquadric* quad = gluNewQuadric();
	gluSphere(quad, 20, 10, 10);*/
	
	view = glm::lookAt(position, position + lookat, glm::vec3(0.0, 1.0, 0.0));
	projection = glm::perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 5000.0f);

	glUseProgram(program);
	glEnableVertexAttribArray(attribute_texcoord);
	glEnableVertexAttribArray(attribute_coord3d);

	drawCube(bg);
	for(int n = 0; n < numcubes; n++) drawCube(cubes[n]);

	glDisableVertexAttribArray(attribute_coord3d);
	glDisableVertexAttribArray(attribute_texcoord);
	glUseProgram(0);
	
	glColor3f(0.0f, 0.0f, 0.0f);
	glRasterPos2f(-1.0f, -1.0f);
	char* s = new char[20];
	sprintf(s, "%.2f FPS\n", 1000.0/(glutGet(GLUT_ELAPSED_TIME) - lastframe));
	for (int n = 0; n < strlen(s); n++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[n]);
	}
	lastframe = glutGet(GLUT_ELAPSED_TIME);
	// posts FPS to screen

	glutSwapBuffers();
} // displays to screen

void reshape(int width, int height) {
	screen_width = width;
	screen_height = height;
	glViewport(0, 0, screen_width, screen_height);
} // resizes screen

void free_resources() {
	glDeleteProgram(program);
	for(int n = 0; n < numcubes; n++) {
		delete cubes[n];
	}
} // cleans up memory

void toggleFullscreen() {
	fullscreen = !fullscreen;
	if(fullscreen) glutFullScreen();
	else {
		glutReshapeWindow(800, 600);
		glutPositionWindow(0, 0);
	}
}

void key_pressed(int key, int x, int y) {
	keys[key] = 1; // key is pressed
	if(key == GLUT_KEY_ESC) toggleFullscreen();
	if(key == 'q') exit(0);
} // watches keyboard

void key_released(int key, int x, int y) {
	keys[key] = 0; // key is no longer pressed
} // watches keyboard

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(screen_width, screen_height);
	glutCreateWindow("My Rotating Cube");
	if(fullscreen) glutFullScreen();
	// initializes glut window

	GLenum glew_status = glewInit();
	if(glew_status != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
		return 1;
	}
	if (!GL_VERSION_2_0) {
		fprintf(stderr, "Error: your graphic card does not support OpenGL 2.0\n");
		return 1;
	}
	// initializes GLEW and checks for errors

	position = glm::vec3(0, 5, -20);
	angle = glm::vec3(0, -.15, 0);

	bg = new Cube(0.0, 0.0, 0.0, "skybox", 3000);
	for(int n = 0; n < numcubes; n++) {
		cubes[n] = new Cube(cubesize*n, 0.0, -cubesize, (n%2==0)?("groundblock"):("questionblock"), cubesize);
	}
	
	if(initShaders()) {
		glutSetCursor(GLUT_CURSOR_CROSSHAIR);
		glutDisplayFunc(onDisplay);
		glutTimerFunc(1000.0/MAX_FPS, timer, 0);
		glutIdleFunc(idle);
		glutReshapeFunc(reshape);
		glutSpecialFunc(key_pressed);
		glutSpecialUpFunc(key_released);
		glutPassiveMotionFunc(motion);
		glutMotionFunc(motion);
		// glut functions
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
/*		glEnable(GL_COLOR_MATERIAL);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);*/
		glutMainLoop();
	}
	
	free_resources();
	return 0;
}
