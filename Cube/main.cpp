// Jeremy Vercillo and Ryan Liebscher (I want some damn credit :P )
// 2/9/12
// Final Project - Mario 3D
// Main driver

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <QuartzCore/QuartzCore.h> // Apple pointer warp
#endif
#ifdef __linux__
#include <GL/glut.h>
#endif
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// libraries

#include "Files.h"
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

#define TITLE_STATE 0
#define MENU_STATE 1
#define GAME_STATE 2
int state = TITLE_STATE;

GLuint title_id;
GLuint vbo_title_vertices;
GLuint vbo_title_texcoords;
GLuint ibo_title_elements;

int windowid;
int screen_width = 800, screen_height = 600; // screen size
int lastx = screen_width/2;
int lasty = screen_height/2; // last mouse position
int midwindowx = screen_width/2; // Middle of the window horizontally
int midwindowy = screen_height/2; // Middle of the window vertically
int pathwidthcheck = 1; // ai checks path width WIP
int pathlengthcheck = 1; // ai checks path width WIP
bool fullscreen = true;
bool mouseinit = false;
bool jump = false;
bool lighttest = false;
float dist;

static glm::vec3 angle;
static glm::vec3 forward;
static glm::vec3 rightvec;
static glm::vec3 lookat;

//A* terms go here

int keys[256] = {0}; // array of whether keys are pressed

int cubesize = 2;
int pathlength = 100;
int pathwidth = 8; // actually means 7
Cube* cubes[1000]; // array of cubes
Cube* aircubes[1000];
Cube* title; // title graphic
Cube* border; // menu graphic
Cube* startbutton; // start button graphic
Cube* quitbutton; // quit button graphic
Cube* settings1; // normal gravity
Cube* settings2; // low gravity
Cube* bg; // background skycube
Cube* camcube; // player "model"
Cube* aitest; // cube controlled by computer
Cube* mushroom[11]; // mushroom locations

glm::mat4 view, projection;

GLfloat diffuse[] = { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat specular[] = { 20 * cubesize, cubesize, -4 * cubesize, 1.0f }; // lighting coordinates and values
GLfloat specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };

float movespeed = 0.004;
float aimovespeed = movespeed * .5;
float mushmovespeed = 0.0005;
float mousespeed = 0.001;
float jumpvel = .0125;
glm::vec3 gravity = glm::vec3(0, -.000005, 0);
glm::vec3 termvel = glm::vec3(0, -.05, 0);
bool lowgrav = false;

float lastframe = 0; // last frame in ms from GLUT_ELAPSED_TIME
float MAX_FPS = 60.0; // 60 frames per second
int test = 0;
int mushnum = 0;
bool mushdraw = false;
int numlives = 3;

void Astar() {

}  // A* algorithm (what I want to make the ai)

float distance(float x1, float y1, float z1, float x2, float y2, float z2) {
    return sqrt(abs((x2-x1)*(x2-x1)) + abs((y2-y1)*(y2-y1)) + abs((z2-z1)*(z2-z1)));
}// distance between 2 points

void ai_chase(Cube* c) {
	c->position -= forward * aimovespeed;
}

void changegrav() {
	lowgrav = !lowgrav;
	if (!lowgrav) gravity = glm::vec3(0, -.000005, 0);
	if (lowgrav) gravity = glm::vec3(0, -.0000025, 0);	
}

void AIphysics(Cube* c) {
    c->velocity += gravity;
	c->position += c->velocity;
	if(c->velocity.y < termvel.y) c->velocity = termvel;
	for(int n = 0; n < pathlength*(pathwidth-1); n++) {
		if(cubes[n]->collidesWith(c)) {
			c->position -= c->velocity;
			c->velocity = glm::vec3(0, 0, 0);
			break;
		}
	}
} // ai physics

void setSize(Cube* c) {

}

void destroy(Cube* c) {
	c->destroyed = true;
}

void reset() {
	camcube = new Cube(0, 3*cubesize, -(pathwidth-1)/2*cubesize, "brickblock", cubesize); 
	aitest = new Cube(20 * cubesize, 3*cubesize, -4 * cubesize, "questionblock", cubesize);
	aitest->destroyed = false;
	for (int n = 1; n < mushnum+1; n++) {
		mushroom[n]->destroyed = false;
	}
	mushnum = 0;
	mushdraw = false;
	angle = glm::vec3(M_PI/2, -M_PI/32, 0);
	for (int n = 0; n < pathlength/16; n++) aircubes[n]->hit = false;
	for (int n = 1; n < 11; n++) mushroom[n] = new Cube(4,cubesize,-(pathwidth-1)/2*cubesize, "brickblock", cubesize);
}

void simpleAI(Cube* c) {    
    int behavior;   // state ai is in, 0 is normal patrol, 1 is chase
    dist = distance(camcube->position.x, camcube->position.y, camcube->position.z, c->position.x, c->position.y, c->position.z);
    if (dist < (10 * cubesize)) behavior = 1;
    else behavior = 0;
    
    switch (behavior){
        case 0:
			c->position.z += pathwidthcheck * (aimovespeed * .1f);
			c->position.x += pathlengthcheck * (aimovespeed * .5f);
			if(c->position.z > 0 || c->position.z < -(pathwidth-2)*cubesize) pathwidthcheck = -pathwidthcheck;
			if(c->position.x < 12 * cubesize) pathlengthcheck = 1;
			if(c->position.x > 28 * cubesize) pathlengthcheck = -1;
			break;
        case 1:
			if (c->position.z >= camcube->position.z) c->position.z -= aimovespeed;
			if (c->position.z <= camcube->position.z) c->position.z += aimovespeed;
				//if (c->position.z == camcube->position.z) c->position.z += camcube->velocity.z;
			if (c->position.x >= camcube->position.x) c->position.x -= aimovespeed;
			if (c->position.x <= camcube->position.x) c->position.x += aimovespeed;
				//if (c->position.x == camcube->position.x) c->position.x += camcube->velocity.z;
            break;
    }

    aitest->velocity += gravity;
	if(aitest->velocity.y < termvel.y) aitest->velocity = termvel;
	for(int n = 0; n < pathlength*(pathwidth-1); n++) {
		if(cubes[n]->collidesY(aitest)) {
			aitest->velocity.y = 0;
			break;
		}
	} // ai physics
	aitest->position += aitest->velocity;
	if(camcube->collidesWith(c) && !c->destroyed || c->collidesWith(camcube) && !c->destroyed){
		if(camcube->collidesTopY(c)) destroy(c);
		else{
			if(camcube->size != cubesize) {
				printf("Mushroom lost\n");
				camcube->position.y -= camcube->size/2;
				camcube->size = cubesize;
				camcube->position.y += camcube->size/2;
			}
			else {
				printf("You died\n");
				numlives--;
				reset();
			}
		}
	}
} // Simple test AI

void mushroomAI(Cube* c) {
    c->position.x += mushmovespeed;
	if(camcube->collidesWith(c) && !c->destroyed) {
		printf("Mushroom get\n");
		camcube->position.y -= camcube->size/2;
		camcube->size = cubesize*8;
		camcube->position.y += camcube->size/2;
		mushdraw = false;
		destroy(c);
	}
}

int initShaders() {
	GLuint vs, fs;
	if((vs = create_shader("cubeshader.v.glsl", GL_VERTEX_SHADER)) == 0) return 0;
	if((fs = create_shader("cubeshader.f.glsl", GL_FRAGMENT_SHADER)) == 0) return 0;
    //if((fs = create_shader("cartoonfrag.f.glsl", GL_FRAGMENT_SHADER)) == 0) return 0;
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

void applyGravity() {
	camcube->velocity += gravity;
	if(camcube->velocity.y < termvel.y) {
		camcube->velocity.y = termvel.y;
	}	
	for(int n = 0; n < pathlength*(pathwidth-1); n++) {
		if(cubes[n]->collidesY(camcube)) {
			jump = false;
			camcube->velocity.y = 0;
			break;
		}
	}
	for(int n = 0; n < pathlength/16; n++) {
		if(camcube->collidesY(aircubes[n])) {
			if(camcube->collidesTopY(aircubes[n])) jump = false;
			camcube->velocity.y = 0;
			break;
		} // collision from below
	}
} // moves by physics instead of input

void motion(int x, int y) {
	angle.x -= (x - lastx) * mousespeed; // phi
	angle.y -= (y - lasty) * mousespeed; // theta
	// moves camera
	
	if(angle.x < -M_PI) angle.x += M_PI * 2;
	if(angle.x > M_PI) angle.x -= M_PI * 2;
	if(angle.y < -M_PI * 0.49) angle.y = -M_PI * 0.49;
	if(angle.y > M_PI * 0.49) angle.y = M_PI * 0.49;
	// keeps camera in range (can't do flips, etc)
	
	lastx = x;
	lasty = y;
	// sets last mouse position
} // moves camera based on current key states

void setVectors() {
	forward.x = sinf(angle.x);
	forward.y = 0;
	forward.z = cosf(angle.x);
	
	rightvec.x = -cosf(angle.x);
	rightvec.y = 0;
	rightvec.z = sinf(angle.x);
	
	lookat.x = sinf(angle.x) * cosf(angle.y);
	lookat.y = sinf(angle.y);
	lookat.z = cosf(angle.x) * cosf(angle.y);
	
	screen_width = glutGet(GLUT_WINDOW_WIDTH);
	screen_height = glutGet(GLUT_WINDOW_HEIGHT);
	lastx = midwindowx = screen_width/2;
	lasty = midwindowy = screen_height/2;
	
#ifdef __APPLE__
	CGWarpMouseCursorPosition(CGPointMake(midwindowx, midwindowy));
#endif
#ifdef __linux__
	glutWarpPointer(midwindowx, midwindowy);
#endif
} // sets player vectors and mouse location

void drawCube(Cube* c) {
	glBindTexture(GL_TEXTURE_2D, c->texture_id);
	
	glBindBuffer(GL_ARRAY_BUFFER, c->vbo_texcoords);
	glVertexAttribPointer(attribute_texcoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, c->vbo_vertices);
	glVertexAttribPointer(attribute_coord3d, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, c->ibo_elements);
	int size; glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(c->position.x, c->position.y, c->position.z));
	// translate to position from origin
	glm::mat4 mvp = projection * view * model;	
	glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
	glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
} // draws a cube

void spawnsMushroom(Cube* c, Cube* Zoidberg) {
    if (c->collidesBottomY(Zoidberg) && c->velocity.y == 0 && !Zoidberg->hit) {
        if (mushnum < 11) mushnum++;
		mushdraw = true;
		Zoidberg->hit = true;
    }
}

void moveCamera() {
	setVectors();
	applyGravity();
    if (!aircubes[0]->hit) spawnsMushroom(camcube, aircubes[0]);

	camcube->velocity.x = 0;
	camcube->velocity.z = 0;
    if(keys['a']) {
		camcube->velocity -= rightvec * movespeed;
		for(int n = 0; n < pathlength; n++) {
			if(cubes[n]->collidesX(camcube) || cubes[n]->collidesZ(camcube)) {
				camcube->velocity += rightvec * movespeed;
				break;
			}
		}
		for(int n = 0; n < pathlength/16; n++) {
			if(aircubes[n]->collidesX(camcube) || aircubes[n]->collidesZ(camcube)) {
				camcube->velocity += rightvec * movespeed;
				break;
			}
		}
	}
	if(keys['d']) {
		camcube->velocity += rightvec * movespeed;
		for(int n = 0; n < pathlength; n++) {
			if(cubes[n]->collidesX(camcube) || cubes[n]->collidesZ(camcube)) {
				camcube->velocity -= rightvec * movespeed;
				break;
			}
		}		
		for(int n = 0; n < pathlength/16; n++) {
			if(aircubes[n]->collidesX(camcube) || aircubes[n]->collidesZ(camcube)) {
				camcube->velocity -= rightvec * movespeed;
				break;
			}
		}	
	}
	if(keys['w']) {
		camcube->velocity += forward * movespeed;
		for(int n = 0; n < pathlength; n++) {
			if(cubes[n]->collidesX(camcube) || cubes[n]->collidesZ(camcube)) {
				camcube->velocity -= forward * movespeed;
				break;
			}
		}	
		for(int n = 0; n < pathlength/16; n++) {
			if(aircubes[n]->collidesX(camcube) || aircubes[n]->collidesZ(camcube)) {
				camcube->velocity -= forward * movespeed;
				break;
			}
		}
	}
	if(keys['s']) {
		camcube->velocity -= forward * movespeed;
		for(int n = 0; n < pathlength; n++) {
			if(cubes[n]->collidesX(camcube) || cubes[n]->collidesZ(camcube)) {
				camcube->velocity += forward * movespeed;
				break;
			}
		}		
		for(int n = 0; n < pathlength/16; n++) {
			if(aircubes[n]->collidesX(camcube) || aircubes[n]->collidesZ(camcube)) {
				camcube->velocity += forward * movespeed;
				break;
			}
		}	
	}
	if(keys[' '] && !jump) {
		camcube->velocity.y = jumpvel;
		jump = true;
	}
	if(jump && !keys[' '] && camcube->velocity.y > .005) {
		camcube->velocity.y = .005;
	}
	// key input
	
	camcube->position += camcube->velocity;
	if (camcube->position.y < -50) {
		numlives--;
		reset();
		}
    simpleAI(aitest);
    if(mushdraw) mushroomAI(mushroom[mushnum]);
	if (numlives<0) {
		numlives = 3;
		state = MENU_STATE;
	}
}

void gameIdle() {
	if(!mouseinit) {
		screen_width = glutGet(GLUT_WINDOW_WIDTH);
		screen_height = glutGet(GLUT_WINDOW_HEIGHT);
		lastx = midwindowx = screen_width/2;
		lasty = midwindowy = screen_height/2;
#ifdef __APPLE__
		CGWarpMouseCursorPosition(CGPointMake(midwindowx, midwindowy));
#endif
		mouseinit = true;
	}
    
	moveCamera();
} // idle function for when in game state

void menuIdle() {

} // idle function for menu state

void titleIdle() {
	
} // idle function for title state

void idle() {
	if(state == TITLE_STATE) titleIdle();
	else if(state == MENU_STATE) menuIdle();
	else if(state == GAME_STATE) gameIdle();
} // constantly calculates redraws

void timer(int value) {
	glutPostRedisplay();
	glutTimerFunc((1000.0/MAX_FPS), timer, 0);
}

void gameDisplay() {
	view = glm::lookAt(camcube->position, camcube->position + lookat, glm::vec3(0.0, 1.0, 0.0));
	projection = glm::perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 5000.0f);

	glUseProgram(program);
	glEnableVertexAttribArray(attribute_texcoord);
	glEnableVertexAttribArray(attribute_coord3d);

	drawCube(bg);
   // drawCube(camcube);
    if (!aitest->destroyed) drawCube(aitest);
	for(int n = 0; n < pathlength*(pathwidth-1); n++) drawCube(cubes[n]);
    for(int n = 0; n < pathlength/16; n++) drawCube(aircubes[n]);
	if (mushdraw && !mushroom[mushnum]->destroyed) {
		drawCube(mushroom[mushnum]);
    }

	glDisableVertexAttribArray(attribute_coord3d);
	glDisableVertexAttribArray(attribute_texcoord);
	glUseProgram(0);
} // display function for game state

void menuDisplay() {
	view = glm::lookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 1.0, 0.0));
	projection = glm::perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 5000.0f);
	
	glUseProgram(program);
	glEnableVertexAttribArray(attribute_texcoord);
	glEnableVertexAttribArray(attribute_coord3d);
	
		//drawCube(title);
	drawCube(border);
	drawCube(startbutton);
	drawCube(quitbutton);
	if (lowgrav) drawCube(settings1);
	else drawCube(settings2);
	
	glDisableVertexAttribArray(attribute_coord3d);
	glDisableVertexAttribArray(attribute_texcoord);
	glUseProgram(0);
} // display function for menu state

void titleDisplay() {
	view = glm::lookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 1.0, 0.0));
	projection = glm::perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 5000.0f);

	glUseProgram(program);
	glEnableVertexAttribArray(attribute_texcoord);
	glEnableVertexAttribArray(attribute_coord3d);
	
	drawCube(title);

	glDisableVertexAttribArray(attribute_coord3d);
	glDisableVertexAttribArray(attribute_texcoord);
	glUseProgram(0);
} // display function for title state

void onDisplay() {
	glClearColor(0.0, 0.0, 0.0, 1.0); // black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// clears the screen
	
	if(state == TITLE_STATE) titleDisplay();
	else if (state == MENU_STATE) menuDisplay();
	else if(state == GAME_STATE) gameDisplay();
    
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
	for(int n = 0; n < pathlength*(pathwidth-1); n++) {
		delete cubes[n];
        delete aircubes[n];
	}
    delete camcube;
    delete bg;
} // cleans up memory

void toggleFullscreen() {
    switch(fullscreen){
        case 1:	
            glutReshapeWindow(800,600);
            screen_width = glutGet(GLUT_WINDOW_X);
            screen_height = glutGet(GLUT_WINDOW_Y);
            midwindowx = screen_width / 2;
            midwindowy = screen_height / 2;
            fullscreen = false;
            break;
        case 0:
            glutFullScreen();
            screen_width = glutGet(GLUT_WINDOW_X);
            screen_height = glutGet(GLUT_WINDOW_Y);
            midwindowx = screen_width / 2;
            midwindowy = screen_height / 2;
            fullscreen = true;
            break;
	}
}

void key_pressed(unsigned char key, int x, int y) {
	if(key == GLUT_KEY_ESC) toggleFullscreen();
	
	if(state == TITLE_STATE) state = GAME_STATE; // next state
	else if(state == MENU_STATE) {
		keys[key] = 1; // key is pressed
		if(key == 'q') {
			glutDestroyWindow(windowid);
			free_resources();
			exit(0);
		}
	}
	else if(state == GAME_STATE) {
		keys[key] = 1; // key is pressed
		if(key == 'q') {
			glutDestroyWindow(windowid);
			free_resources();
			exit(0);
		}
		if(key == 'r') {
			reset();
		} // reset
	}
} // watches keyboard

void key_released(unsigned char key, int x, int y) {
	keys[key] = 0; // key is no longer pressed
} // watches keyboard

void mouse_click(int button, int mstate, int x, int y) {
	if (mstate == GLUT_DOWN) {
		if(state == TITLE_STATE) state = MENU_STATE; // next state
		else if(state == MENU_STATE) {
			//printf("%d, %d\n", screen_width, screen_height);
			//printf("%d, %d\n", x, y);
			if(y>=305.0/900.0 * screen_height && y<=430.0/900.0 * screen_height) {
				if(x>=555.0/1440.0 * screen_width && x<=690.0/1440.0 * screen_width) {
					state = GAME_STATE;
				}
				if(x>=750.0/1440.0 * screen_width && x<=880.0/1440.0 * screen_width) {
					glutDestroyWindow(windowid);
					free_resources();
					exit(0);
				}
			}
			if (y>=500.0/900.0 * screen_height && y<=630.0/900.0 * screen_height && x>=650.0/1440.0 * screen_width && x<=790.0/1440.0 * screen_width) {
				changegrav();
				//printf("Gravity Changed\n");
			}
		}
		else {}
	}
} // watches for mouse to be clicked

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(screen_width, screen_height);
	windowid = glutCreateWindow("Mario 3D");
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

	angle = glm::vec3(M_PI/2, -M_PI/32, 0);

	title = new Cube(0.0, 0.0, 4.0, "title", 2); // inits title screen

	border = new Cube(0.0, 0.0, 6.0, "border", 4.0f); // inits title screen
	startbutton = new Cube(0.25, 0.2, 3.0, "start", 0.5f);
	quitbutton = new Cube(-0.25, 0.2, 3.0, "quit", 0.5f);
	settings1 = new Cube(0.0, -0.3, 3.0, "lowgrav", 0.5f);
	settings2 = new Cube(0.0, -0.3, 3.0, "normgrav", 0.5f);

	bg = new Cube(0.0, 0.0, 0.0, "skybox", 3000);
    for (int m = 0; m < pathwidth; m++) {
        for (int n = (100*m); n < (m*100)+100; n++) {
            cubes[n] = new Cube(cubesize*(n%100), 0.0, -m*cubesize, "groundblock", cubesize);
		}
	}    
    for (int n = 0; n < pathlength/16; n++) {
        aircubes[n] = new Cube(cubesize*n*16, 6 * cubesize, -(pathwidth-1)/2*cubesize, ("questionblock"), cubesize);
    }
    for (int n = 1; n < 11; n++) mushroom[n] = new Cube(4,cubesize,-(pathwidth-1)/2*cubesize, "brickblock", cubesize);
    camcube = new Cube(0, 3*cubesize, -(pathwidth-1)/2*cubesize, "brickblock", cubesize); 
    aitest = new Cube(20 * cubesize, 3*cubesize, -4 * cubesize, "questionblock", cubesize);

#ifdef __APPLE__
	CGSetLocalEventsSuppressionInterval(0.0); // deprecated, but working
#endif
	
	if(initShaders()) {
		glutSetCursor(GLUT_CURSOR_CROSSHAIR);
		glutDisplayFunc(onDisplay);
		glutTimerFunc(1000.0/MAX_FPS, timer, 0);
		glutIdleFunc(idle);
		glutReshapeFunc(reshape);
		glutKeyboardFunc(key_pressed);
		glutKeyboardUpFunc(key_released); // keyboard keys
		glutMouseFunc(mouse_click); // mouse buttons
		glutPassiveMotionFunc(motion);
		glutMotionFunc(motion);
		// glut functions
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_3D);
/*        glEnable(GL_LIGHTING);
        glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
        glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);
        glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);
        glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
        glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
        glMateriali(GL_FRONT, GL_SHININESS, 128);*/

        glutMainLoop();
	}
	return 0;
}
