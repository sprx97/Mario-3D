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
#include <GL/gl.h>
#include <GL/glu.h>	    
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
#include "../objLoader/draw_flower.h"
#include "../objLoader/draw_mushroom.h"
#include "../objLoader/draw_goomba.h"
#include "../objLoader/draw_pipe.h"
#include "../objLoader/draw_flag.h"
#include "../objLoader/draw_star.h"
#include "../objLoader/draw_coin.h"
#include "../objLoader/draw_fireball.h"

// local includes

#ifndef GLUT_KEY_ESC
#define GLUT_KEY_ESC 27
#endif

#define SKIP_MENUS
	//#define DRAW_HITBOXES

using namespace std;

GLuint program;
GLint attribute_coord3d, attribute_texcoord;
GLint uniform_mvp;

#define TITLE_STATE 0
#define MENU_STATE 1
#define GAME_STATE 2

#ifdef SKIP_MENUS
int state = GAME_STATE;
#else
int state = TITLE_STATE;
#endif

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
int ncubes = 0;
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
			   //Cube* aitest; // cube controlled by computer
Cube* mushroom[11]; // mushroom locations
Cube* fireball;
Cube* star;

draw_flower* flower;
draw_mushroom* mushgraph;
draw_goomba* goomba;
draw_pipe* xyz;
draw_star* astar;
draw_flag* flag;
draw_coin* coin;
draw_fireball* myfire;

glm::mat4 view, projection;

GLfloat diffuse[] = { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat specular[] = { 20 * cubesize, cubesize, -4 * cubesize, 1.0f }; // lighting coordinates and values
GLfloat specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };

float movespeed = 0.004;
float aimovespeed = movespeed * .5;
float mushmovespeed = 0.0005;
float firemovespeed = .1;
float mousespeed = 0.001;
float jumpvel = .0125;
glm::vec3 gravity = glm::vec3(0, -.000005, 0);
glm::vec3 termvel = glm::vec3(0, -.05, 0);
bool lowgrav = false;

float lastframe = 0; // last frame in ms from GLUT_ELAPSED_TIME
float MAX_FPS = 60.0; // 60 frames per second
int test = 0;
int mushnum = 0;
//stuff for fireball
bool mushdraw = false;
bool hasfire = true;
bool firedraw = false;
bool hasShot = false; //has fireball been shot
//stuff for star
float starmovespeed = .005;
float starbouncespeed = .005;
float starmaxheight = 5;
bool stardraw = false;

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
		//aitest = new Cube(20 * cubesize, 3*cubesize, -4 * cubesize, "questionblock", cubesize);
	goomba = new draw_goomba(glm::vec3(20 * cubesize, 3*cubesize, -4 * cubesize), glm::vec3(.5, .5, .5), glm::vec3(0, -90, 0));
	goomba->destroyed = false;
		//aitest->destroyed = false;
	for (int n = 1; n < mushnum+1; n++) {
		mushroom[n]->destroyed = false;
	}
	mushnum = 0;
	mushgraph = new draw_mushroom(glm::vec3(cubesize, 7*cubesize, -(pathwidth-1)/2*cubesize), glm::vec3(.5, .5, .5), glm::vec3(0, -90, 0));
	mushdraw = false;
	mushgraph->destroyed = true;
	
	angle = glm::vec3(M_PI/2, -M_PI/32, 0);
	for (int n = 0; n < pathlength/16; n++) aircubes[n]->hit = false;
	for (int n = 1; n < 11; n++) mushroom[n] = new Cube(4,cubesize,-(pathwidth-1)/2*cubesize, "brickblock", cubesize);

	stardraw = false;
	new Cube(4, cubesize, -(pathwidth-1)/2*cubesize, "questionblock", cubesize);
}

void rotateToFaceCamAI(draw_object *c) {
		//We want ai to face camcube->position.x
		//Use P.T. to find hypot then rotate using angle between hypot and dist of x
	float asplusbs, rootc, rotangle;
	float rotx = 8;
	if (c->position.x > camcube->position.x && abs(c->position.x - camcube->position.x) > abs(c->position.z - camcube->position.z)) {
		asplusbs = pow((c->position.x - camcube->position.x),2) + pow((c->position.z - camcube->position.z),2);
		rootc = sqrt(asplusbs);
		rotangle = cosf((((c->position.x - camcube->position.x)/rootc)*M_PI)/180);
		glm::vec3 newrot (0.0f,-90+(rotx * (camcube->position.z - c->position.z))*rotangle,0.0f);
		c->rotate(newrot);
	}
	if (c->position.x > camcube->position.x && abs(c->position.x - camcube->position.x) < abs(c->position.z - camcube->position.z)) {
		asplusbs = pow((c->position.x - camcube->position.x),2) + pow((c->position.z - camcube->position.z),2);
		rootc = sqrt(asplusbs);
		rotangle = cosf((((c->position.z - camcube->position.z)/rootc)*M_PI)/180);
		glm::vec3 newrot (0.0f,-180+(rotx * (c->position.x - camcube->position.x))*rotangle,0.0f);
		c->rotate(newrot);
	}
	if (c->position.x < camcube->position.x && c->position.z > camcube->position.z && abs(c->position.x - camcube->position.x) < abs(c->position.z - camcube->position.z)) {
		asplusbs = pow((c->position.x - camcube->position.x),2) + pow((c->position.z - camcube->position.z),2);
		rootc = sqrt(asplusbs);
		rotangle = cosf((((c->position.x - camcube->position.x)/rootc)*M_PI)/180);
		glm::vec3 newrot (0.0f,-180+(rotx * (c->position.x - camcube->position.x))*rotangle,0.0f);
		c->rotate(newrot);
	}
	if (c->position.x < camcube->position.x && abs(c->position.x - camcube->position.x) > abs(c->position.z - camcube->position.z)) {
		asplusbs = pow((c->position.x - camcube->position.x),2) + pow((c->position.z - camcube->position.z),2);
		rootc = sqrt(asplusbs);
		rotangle = cosf((((c->position.x - camcube->position.x)/rootc)*M_PI)/180);
		glm::vec3 newrot (0.0f,-270+(rotx * (c->position.z - camcube->position.z))*rotangle,0.0f);
		c->rotate(newrot);
	}
	if (c->position.x < camcube->position.x && c->position.z < camcube->position.z && abs(c->position.x - camcube->position.x) < abs(c->position.z - camcube->position.z)) {
		asplusbs = pow((c->position.x - camcube->position.x),2) + pow((c->position.z - camcube->position.z),2);
		rootc = sqrt(asplusbs);
		rotangle = cosf((((c->position.z - camcube->position.z)/rootc)*M_PI)/180);
		glm::vec3 newrot (0.0f,(rotx * (camcube->position.x - c->position.x))*rotangle,0.0f);
		c->rotate(newrot);
	}
	if (c->position.x > camcube->position.x && c->position.z < camcube->position.z && abs(c->position.x - camcube->position.x) < abs(c->position.z - camcube->position.z)) {
		asplusbs = pow((c->position.x - camcube->position.x),2) + pow((c->position.z - camcube->position.z),2);
		rootc = sqrt(asplusbs);
		rotangle = cosf((((c->position.x - camcube->position.x)/rootc)*M_PI)/180);
		glm::vec3 newrot (0.0f,(rotx * (camcube->position.x - c->position.x))*rotangle,0.0f);
		c->rotate(newrot);
	}
}

void simpleAI(draw_object* c) {    
	int behavior;   // state ai is in, 0 is normal patrol, 1 is chase 
    dist = distance(camcube->position.x, camcube->position.y, camcube->position.z, c->position.x, c->position.y, c->position.z);
    if (dist < (10 * cubesize) && camcube->position.y <= (c->position.y + 2*cubesize)) behavior = 1;
    else behavior = 0;
    
    switch (behavior){
        case 0:
			c->position.z += pathwidthcheck * (aimovespeed * .1f);
			c->position.x -= pathlengthcheck * (aimovespeed * .25f);
			if(c->position.z > 0 || c->position.z < -(pathwidth-2)*cubesize) pathwidthcheck = -pathwidthcheck;
			if(c->position.x < 12 * cubesize) {
				pathlengthcheck = -1;
				c->rotate(glm::vec3(0,-270,0));
			}
			if(c->position.x > 28 * cubesize) {
				pathlengthcheck = 1;
				c->rotate(glm::vec3(0,-90,0));
			}
			break;
        case 1:
			if (c->position.z >= camcube->position.z) c->position.z -= aimovespeed * .5f;
			if (c->position.z <= camcube->position.z) c->position.z += aimovespeed * .5f;
				//if (c->position.z == camcube->position.z) c->position.z += camcube->velocity.z;
			if (c->position.x >= camcube->position.x) c->position.x -= aimovespeed * .5f;
			if (c->position.x <= camcube->position.x) c->position.x += aimovespeed * .5f;
				//if (c->position.x == camcube->position.x) c->position.x += camcube->velocity.z;
			rotateToFaceCamAI(c);
            break;
    }
	
    c->velocity += gravity;
	if(c->velocity.y < termvel.y) c->velocity = termvel;
	for(int n = 0; n < pathlength*(pathwidth-1); n++) {
		if(c->collidesTopY(cubes[n])) {
			c->velocity.y = 0;
			break;
		}
	} // ai physics
	if(c->velocity.x > 0) c->velocity.x -= .0005*aimovespeed;
	if(c->velocity.x < -.0005*aimovespeed) c->velocity.x += .0005*aimovespeed;
	if(c->velocity.x >= -.0005*aimovespeed && c->velocity.x < 0) c->velocity.x = 0;
	if(c->velocity.z > 0) c->velocity.z -= .0005*aimovespeed;
	if(c->velocity.z < -.0005*aimovespeed) c->velocity.z += .0005*aimovespeed;
	if(c->velocity.z >= -.0005*aimovespeed && c->velocity.z < 0) c->velocity.z = 0;

	c->position += c->velocity;
	if(c->collidesWith(camcube) && !c->destroyed){
		if(c->collidesBottomY(camcube)) c->destroyed = true;
		else{
			if(camcube->size != cubesize) {
				printf("Mushroom lost\n");
				camcube->position.y -= camcube->size/2;
				camcube->size = cubesize;
				camcube->position.y += camcube->size/2;
				if (camcube->position.x < c->position.x){
					c->velocity.y += .004;
					c->velocity.x += 3*aimovespeed;
				}
				if (camcube->position.x > c->position.x){
					c->velocity.y += .004;
					c->velocity.x += -3*aimovespeed;
				}
				if (c->position.z > camcube->position.z && abs(c->position.x - camcube->position.x) < abs(c->position.z - camcube->position.z)){
					c->velocity.y += .004;
					c->velocity.z += 2*aimovespeed;
					c->velocity.x = 0;
				}
				if (c->position.z < camcube->position.z && abs(c->position.x - camcube->position.x) < abs(c->position.z - camcube->position.z)){
					c->velocity.y += .004;
					c->velocity.z += -2*aimovespeed;
					c->velocity.x = 0;
				}
				c->position += c->velocity;
			}
			else {
				printf("You died\n");
				numlives--;
				reset();
			}
		}
	}
	glm::vec3 newpos (c->position.x,c->position.y,c->position.z);
	c->move(newpos);
} // Simple test AI

void mushroomAI(draw_object* c) {
    c->position.x += mushmovespeed;
	c->velocity += (gravity*.5f);
	for(int n = 0; n < pathlength*(pathwidth-1); n++) {
		if(c->collidesTopY(cubes[n])) {
			c->velocity.y = 0;
			break;
		}
	}
	c->position += c->velocity;
	if(c->collidesWith(camcube) && c->destroyed==false) {
		printf("Mushroom get\n");
		camcube->position.y -= camcube->size/2;
		camcube->size = cubesize*2;
		camcube->position.y += camcube->size/2;
		mushdraw = false;
		c->destroyed = true;
		}
	glm::vec3 newposmush (c->position.x,c->position.y,c->position.z);
	c->move(newposmush);
}
	
void fireballAI(Cube* c, Cube* enemy) {
  if(hasShot == false && hasfire == true) {
    c->move(camcube->position.x+forward.x*3, camcube->position.y+lookat.y*3, camcube->position.z+forward.z*3);
    hasShot = true;
//    hasfire == false;   
  }
  else{
    c->position += c->velocity;
    //x and y aims work, but z does not
    // c->position.z += firemovespeed*(-cosf(angle.z));
    if((c->collidesWith(enemy) && !c->destroyed) || (enemy->collidesWith(c) && !enemy->destroyed)) {
      printf("Hit!\n");
      firedraw = false;
	  hasShot = false;
      destroy(enemy);
//      destroy(fireball);
    }
    //if path collision, fireball is destroyed
    for(int i = 0; i < ncubes; i++) {
      if(c->collidesWith(cubes[i]) && !c->destroyed) {
		firedraw = false;
		hasShot = false;
//		destroy(fireball);
      }
    }
	if(!c->collidesWith(bg) && !c->destroyed) {
		firedraw = false;
		hasShot = false;
	} // if it goes outside the bg, it resets
  }
}

void starAI(Cube* c) {
  //movement
  c->position.x += starmovespeed;
  c->position.y += starbouncespeed;
  if(c->position.y >= starmaxheight) starbouncespeed = -starbouncespeed;
  if(c->position.y <= 1) starbouncespeed = -starbouncespeed;
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
			if(aircubes[n]->collidesBottomY(camcube)) jump = false;
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

void spawnsPrize(Cube* c, Cube* Zoidberg, int type) {
    if (c->collidesBottomY(Zoidberg) && c->velocity.y == 0 && !Zoidberg->hit) {
      switch (type) {

      case 1 :
        if (mushnum < 11) mushnum++;
	mushdraw = true;
	mushgraph->destroyed = false;
	Zoidberg->hit = true;
	break;

      case 2 :
	stardraw = true;
	Zoidberg->hit = true;
	break;

      }
    }    
}

void moveCamera() {
	setVectors();
	applyGravity();
	if (!aircubes[0]->hit) spawnsPrize(camcube, aircubes[0], 1); //will spawn type mushroom
	//	if (!aircubes[1]->hit) spawnsPrize(camcube, aircubes[1], 2); //will spawn type star
	//if (!aircubes[1]->hit) spawnsPrize(camcube, aircubes[0], 1); //will spawn type mushroom
	if (flower->collidesWith(camcube)) hasfire == true;
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
    simpleAI(goomba);	
    if(mushdraw) mushroomAI(mushgraph);

    //if(stardraw) starAI(star);
    if (numlives<0) {
      numlives = 3;
      state = MENU_STATE;
    }
//    if(firedraw) fireballAI(fireball, aitest);
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
  //clear model view at begginning of display
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

	view = glm::lookAt(camcube->position, camcube->position + lookat, glm::vec3(0.0, 1.0, 0.0));
	projection = glm::perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 5000.0f);

	gluLookAt(camcube->position.x, camcube->position.y, camcube->position.z, camcube->position.x + lookat.x, camcube->position.y + lookat.y, camcube->position.z + lookat.z, 0.0, 1.0, 0.0);

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	gluPerspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 5000.0f);	

	flower->draw();
	if (goomba->destroyed == false) goomba->draw();	
	flag->draw();
	xyz->draw();
	astar->draw();
	coin->draw();
	myfire->draw();
	if (mushgraph->destroyed == false) mushgraph->draw();
	// width and height of the plane that the object is in
	
	glUseProgram(program);
	glEnableVertexAttribArray(attribute_texcoord);
	glEnableVertexAttribArray(attribute_coord3d);

#ifdef DRAW_HITBOXES
	(flower->hitboxes[0])->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);
	(goomba->hitboxes[0])->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);
	(astar->hitboxes[0])->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);
	(coin->hitboxes[0])->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);
	(myfire->hitboxes[0])->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);
	(mushgraph->hitboxes[0])->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);
	for(int n = 0; n < xyz->hitboxes.size(); n++) {
		(xyz->hitboxes[n])->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);
	} // need pipe extension thats just the cylinder
	for(int n = 0; n < flag->hitboxes.size(); n++) {
		(flag->hitboxes[n])->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);
	} // needs to be taller
#endif

	bg->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);
//	camcube->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);


	//if (!aitest->destroyed) aitest->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);
	for(int n = 0; n < pathlength*(pathwidth-1); n++) cubes[n]->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);
	for(int n = 0; n < pathlength/16; n++) aircubes[n]->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);

	//if (stardraw && !star->destroyed) (star)->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);
	//fireball->move(camcube->position.x+3, camcube->position.y, camcube->position.z);
	
	if(firedraw){
	  fireball->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);
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
	
//	title->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);
	border->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);
	startbutton->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);
	quitbutton->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);
	if(lowgrav) settings1->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);
	else settings2->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);
	
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
	
	title->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);

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

	glDisable(GL_LIGHTING);	
	glColor3f(0.0f, 0.0f, 0.0f);
	glRasterPos2f(0.0f, 0.0f);

	char* s = new char[20];
	sprintf(s, "%.2f FPS\n", 1000.0/(glutGet(GLUT_ELAPSED_TIME) - lastframe));
	for (int n = 0; n < strlen(s); n++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[n]);
	}
	lastframe = glutGet(GLUT_ELAPSED_TIME);
	glEnable(GL_LIGHTING);
	
	// posts FPS to screen
	glutSwapBuffers();
} // displays to screen

void reshape(int width, int height) {
	screen_width = width;
	screen_height = height;
	glViewport(0, 0, screen_width, screen_height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//if (screen_width <= screen_height) glOrtho(0.0, 16.0, 0.0, 16.0*screen_height/screen_width, -10.0, 10.0);
	//else glOrtho(0.0, 16.0*screen_width/screen_height, 0.0, 16.0, -100.0, 100.0);
	gluPerspective(45.0, width / (float) height, 0.1, 100000);
	glMatrixMode(GL_MODELVIEW);
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
		else if(state == GAME_STATE) {
		  if(hasfire && !firedraw) {
			firedraw = true;
			fireball->velocity.x = forward.x*firemovespeed;
			fireball->velocity.y = lookat.y*firemovespeed;
			fireball->velocity.z = forward.z*firemovespeed;
		  }
		}
	}
} // watches for mouse to be clicked

void initLighting() {
	GLfloat ambient[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat position0[] = {0.0, 3.0, 3.0, 0.0};
 
	GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
	GLfloat local_view[] = {0.0};
  
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
 
	glLightfv(GL_LIGHT0, GL_POSITION, position0);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);  
 
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
//	glEnable(GL_COLOR_MATERIAL); // this breaks stuff
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glClearDepth(1.0);				
	glDepthFunc(GL_LESS);                       
	glEnable(GL_DEPTH_TEST); 
}

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
	    ncubes++;
		}
	}    
    for (int n = 0; n < pathlength/16; n++) {
        aircubes[n] = new Cube(cubesize*n*16, 6 * cubesize, -(pathwidth-1)/2*cubesize, ("questionblock"), cubesize);
    }
    for (int n = 1; n < 11; n++) mushroom[n] = new Cube(4,cubesize,-(pathwidth-1)/2*cubesize, "brickblock", cubesize);
    camcube = new Cube(0, 3*cubesize, -(pathwidth-1)/2*cubesize, "brickblock", cubesize); 
	//aitest = new Cube(20 * cubesize, 3*cubesize, -4 * cubesize, "questionblock", cubesize);
    fireball = new Cube(12, 3, -3, "questionblock", 1);
    //these are all of the graphics. they can be easily modified so let me know
    //xyz is a pipe...=/
    //sorry the fireball is lame. i can work on it

	flower = new draw_flower(glm::vec3(12, 4, -5), glm::vec3(.25, .25, .25), glm::vec3(0, 0, 0));
	goomba = new draw_goomba(glm::vec3(20 * cubesize, 3*cubesize, -4 * cubesize), glm::vec3(.5, .5, .5), glm::vec3(0, -90, 0));
	xyz = new draw_pipe(glm::vec3(20, 5, -8), glm::vec3(.05, .05, .05), glm::vec3(0, 0, 0));
	astar = new draw_star(glm::vec3(12, 2, -4), glm::vec3(.1, .1, .1), glm::vec3(0, -90, 0)); 
	flag = new draw_flag(glm::vec3(12, 6, -2), glm::vec3(.5, .5, .5), glm::vec3(0, 90, 0)); 
	coin = new draw_coin(glm::vec3(10, 3,-7), glm::vec3(.025, .025, .025), glm::vec3(0, 20, 90)); 
	myfire = new draw_fireball(glm::vec3(15, 8, -5), glm::vec3(.5, .5, .5), glm::vec3(0, 0, 0));
	mushgraph = new draw_mushroom(glm::vec3(cubesize, 7*cubesize, -(pathwidth-1)/2*cubesize), glm::vec3(.5, .5, .5), glm::vec3(0, -90, 0));
	mushgraph->destroyed = true;
	
#ifdef __APPLE__
	CGSetLocalEventsSuppressionInterval(0.0); // deprecated, but working
#endif
	
	if(initShaders()) {
		initLighting();
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
        glEnable(GL_TEXTURE_3D);

        glutMainLoop();
	}
	return 0;
}
