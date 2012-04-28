// Jeremy Vercillo and Ryan Liebscher (I want some damn credit :P )
// 2/9/12
// Final Project - Mario 3D
// Main driver

#include <string>
#include <time.h>
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
#define PRINT_FPS

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
int onpipe = -1;
bool warping1 = false;
bool warping2 = false;

static glm::vec3 angle;
static glm::vec3 forward;
static glm::vec3 rightvec;
static glm::vec3 lookat;

//A* terms go here

int keys[256] = {0}; // array of whether keys are pressed

int cubesize = 2;
int pathlength = 100;
double pathwidth = 8;
int ncubes = 0;
vector<Cube*> cubes; // array of cubes
Cube* title; // title graphic
Cube* border; // menu graphic
Cube* startbutton; // start button graphic
Cube* quitbutton; // quit button graphic
Cube* settings1; // normal gravity
Cube* settings2; // low gravity
Cube* bg; // background skycube
Cube* camcube; // player "model"

#define MUSHROOM 0
#define STARMAN 1
#define FLOWER 2 

vector<draw_object*> prizes; // flowers, mushrooms, starmen, etc
vector<draw_pipe*> pipes;
vector<draw_fireball*> fireballs;
draw_goomba* goomba; // will be vector of goomba called enemies

draw_flag* flag;
draw_coin* coin;

glm::mat4 view, projection;

GLfloat diffuse[] = { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat specular[] = { 20 * cubesize, cubesize, -4 * cubesize, 1.0f }; // lighting coordinates and values
GLfloat specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };

float movespeed = 0.01;
float aimovespeed = movespeed * .5;
float mushmovespeed = 0.0025;
float firemovespeed = .1;
float mousespeed = 0.002;
float jumpvel = .025;
glm::vec3 gravity = glm::vec3(0, -.00002, 0);
glm::vec3 termvel = glm::vec3(0, -.1, 0);
bool lowgrav = false;

float dt = 1; // for time based movement
float lastidle = 0;
float lastframe = 0; // last frame in ms from GLUT_ELAPSED_TIME
float MAX_FPS = 60.0; // 60 frames per second
int test = 0;

bool hasfire = false;
bool invincible = false;
int destroycountdown = -1;
int knockbackcountdown = -1;

int numlives = 3;

float distance(float x1, float y1, float z1, float x2, float y2, float z2) {
    return sqrt(abs((x2-x1)*(x2-x1)) + abs((y2-y1)*(y2-y1)) + abs((z2-z1)*(z2-z1)));
}// distance between 2 points

void ai_chase(Cube* c) {
	c->position -= forward * aimovespeed * dt;
}

void changegrav() {
	lowgrav = !lowgrav;
	if (!lowgrav) gravity = glm::vec3(0, -.000005, 0);
	if (lowgrav) gravity = glm::vec3(0, -.0000025, 0);	
}

void AIphysics(Cube* c) {
    c->velocity += gravity * dt;
	c->position += c->velocity;
	if(c->velocity.y < termvel.y) c->velocity = termvel;
	for(int n = 0; n < cubes.size(); n++) {
		if(cubes[n]->collidesWith(c, dt)) {
			c->position -= c->velocity * dt;
			c->velocity = glm::vec3(0, 0, 0);
			break;
		}
	}
} // ai physics

void reset() {
	camcube = new Cube(0, 3*cubesize, -(pathwidth-1)/2*cubesize, "brickblock", cubesize); 

	goomba = new draw_goomba(glm::vec3(20 * cubesize, 3*cubesize, -4 * cubesize), glm::vec3(.5, .5, .5), glm::vec3(0, -90, 0));
	goomba->destroyed = false;
	
	angle = glm::vec3(M_PI/2, -M_PI/32, 0);
	for (int n = 0; n < cubes.size(); n++) cubes[n]->hit = false;
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
    float dist = distance(camcube->position.x, camcube->position.y, camcube->position.z, c->position.x, c->position.y, c->position.z);
    if (dist < (10 * cubesize) && camcube->position.y <= (c->position.y + 2*cubesize)) behavior = 1;
    else behavior = 0;

    if(destroycountdown < 0 && knockbackcountdown < 0) {
		c->velocity = glm::vec3(0, c->velocity.y, 0);
		switch (behavior){
			case 0:
				c->velocity.z += pathwidthcheck * (aimovespeed * .1f) * dt;
				c->velocity.x -= pathlengthcheck * (aimovespeed * .25f) * dt;
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
				if (c->position.z >= camcube->position.z) {
					c->velocity.z -= aimovespeed * .5f * dt;
					for(int n = 0; n < pipes.size(); n++) {
						if(c->collidesZ(pipes[n], dt)) c->velocity.z += (aimovespeed+.01f)*.5f * dt;
					}
				}
				if (c->position.z <= camcube->position.z) {
					c->velocity.z += aimovespeed * .5f * dt;
					for(int n = 0; n < pipes.size(); n++) {
						if(c->collidesZ(pipes[n], dt)) c->velocity.z -= (aimovespeed+.01f)*.5f * dt;
					}
				}
				if (c->position.x >= camcube->position.x) {
					c->velocity.x -= aimovespeed * .5f * dt;
					for(int n = 0; n < pipes.size(); n++) {
						if(c->collidesX(pipes[n], dt)) c->velocity.x += (aimovespeed+.01f)*.5f * dt;
					}
				}
				if (c->position.x <= camcube->position.x) {
					c->velocity.x += aimovespeed * .5f * dt;
					for(int n = 0; n < pipes.size(); n++) {
						if(c->collidesX(pipes[n], dt)) c->velocity.x -= (aimovespeed+.01f)*.5f * dt;
					}
				}
				rotateToFaceCamAI(c);
				break;
		}
	}
	
    c->velocity += gravity * dt;
	if(c->velocity.y < termvel.y) c->velocity = termvel;
	for(int n = 0; n < cubes.size(); n++) {
		if(c->collidesTopY(cubes[n], dt)) {
			c->velocity.y = 0;
			break;
		}
	} // ai physics

	if(destroycountdown > 0) {
		destroycountdown--;
		if(destroycountdown == 0) {
			c->destroyed = true;
			destroycountdown--;
		}
	}
	else if(knockbackcountdown >= 0) {
		knockbackcountdown--;
	}
	else if(c->collidesWith(camcube, dt) && !c->destroyed && destroycountdown < 0){
		if(c->collidesBottomY(camcube, dt) && !c->collidesX(camcube, dt) && !c->collidesZ(camcube, dt)) {
			if(!invincible) camcube->velocity.y = jumpvel/2;
			c->destroyed = true;
		}
		else {
			if(invincible) {
				destroycountdown = 1000;
				// angle between user and gooma
				float dx = abs(c->position.x - camcube->position.x);
				float dz = abs(c->position.z - camcube->position.z);
				float anglebetween = atan(dz/dx);
				
				int modz = 1; // modifies to make up for losing part of circle in atan
				int modx = 1;
				if(c->position.z-camcube->position.z < 0) modz = -1;
				if(c->position.x-camcube->position.x < 0) modx = -1;
				c->velocity.y = .01;
				c->velocity.x = 4*modx*aimovespeed*cos(anglebetween);
				c->velocity.z = 4*modz*aimovespeed*sin(anglebetween);
			}
			else if(camcube->size != cubesize) {
//				printf("Mushroom lost\n");
				camcube->position.y -= camcube->size/2;
				camcube->size = cubesize;
				camcube->position.y += camcube->size/2;
				
				knockbackcountdown = 1000;
				
				// angle between user and gooma
				float dx = abs(c->position.x - camcube->position.x);
				float dz = abs(c->position.z - camcube->position.z);
				float anglebetween = atan(dz/dx);
				
				int modz = 1; // modifies to make up for losing part of circle in atan
				int modx = 1;
				if(c->position.z-camcube->position.z < 0) modz = -1;
				if(c->position.x-camcube->position.x < 0) modx = -1;
				c->velocity.y = .01;
				c->velocity.x = 2*modx*aimovespeed*cos(anglebetween);
				c->velocity.z = 2*modz*aimovespeed*sin(anglebetween);
			}
			else {
//				printf("You died\n");
				numlives--;
				reset();
			}
		}
	}
	c->position += c->velocity * dt;	
	glm::vec3 newpos(c->position.x,c->position.y,c->position.z);
	c->move(newpos);
	
	float angletouser = 180/M_PI*atan((newpos.z-camcube->position.z)/(newpos.x-camcube->position.x));
	if(newpos.x-camcube->position.x > 0) c->rotate(glm::vec3(0, -90-angletouser, 0));
	else c->rotate(glm::vec3(0, 90-angletouser, 0));
} // Simple test AI

bool mushroomAI(draw_object* c) {
	c->velocity += (gravity*.5f) * dt;
	for(int n = 0; n < cubes.size(); n++) {
		if(c->collidesTopY(cubes[n], dt)) {
			c->velocity.y = 0;
			break;
		}
		else if(c->collidesX(cubes[n], dt)) {
			c->velocity.x *= -1;
			break;
		}
		else if(c->collidesZ(cubes[n], dt)) {
			c->velocity.z *= -1;
			break;
		}
	}
	for(int n = 0; n < pipes.size(); n++) {
		if(c->collidesTopY(pipes[n], dt)) {
			c->velocity.y = 0;
			break;
		}
		else if(c->collidesX(pipes[n], dt)) {
			c->velocity.x *= -1;
			break;
		}
		else if(c->collidesZ(pipes[n], dt)) {
			c->velocity.z *= -1;
			break;
		}
	}
	c->position += c->velocity * dt;
	c->move(c->position);
}

bool fireballAI(draw_fireball* c) {
  if(c->distancetraveled > 1000) return true;
  else {
	c->distancetraveled += 1;
	c->velocity.y += 10*gravity.y * dt;

    if(c->collidesWith(goomba, dt)) {
	  goomba->destroyed = true;
	  return true;
    }
    //if path collision, fireball is destroyed
    for(int i = 0; i < cubes.size(); i++) {
		if(c->collidesWith(cubes[i], dt)) {
			if(c->collidesY(cubes[i], dt)) c->velocity.y *= -.75;
			else if(c->collidesX(cubes[i], dt)) c->velocity.x *= -.75;
			else if(c->collidesZ(cubes[i], dt)) c->velocity.z *= -.75;
			break;
		}
    }
	for(int n = 0; n < pipes.size(); n++) {
		if(c->collidesWith(pipes[n], dt)) {
			if(c->collidesWith(pipes[n], dt)) {
				if(c->collidesY(pipes[n], dt)) c->velocity.y *= -.75;
				else if(c->collidesX(pipes[n], dt)) c->velocity.x *= -.75;
				else if(c->collidesZ(pipes[n], dt)) c->velocity.z *= -.75;
				break;
			}
		}
	}
	c->position += c->velocity * dt;
	c->move(c->position);
	return false;
  }
}

void starAI(draw_object* c) {
	if(c->velocity.y > .04) c->velocity.y = .04;
	c->velocity.y += 10*gravity.y * dt;	
    for(int i = 0; i < cubes.size(); i++) {
		if(c->collidesWith(cubes[i], dt)) {
			if(c->collidesY(cubes[i], dt)) c->velocity.y *= -1;
			else if(c->collidesX(cubes[i], dt)) c->velocity.x *= -1;
			else if(c->collidesZ(cubes[i], dt)) c->velocity.z *= -1;
			break;
		}
    }
	for(int n = 0; n < pipes.size(); n++) {
		if(c->collidesWith(pipes[n], dt)) {
			if(c->collidesWith(pipes[n], dt)) {
				if(c->collidesY(pipes[n], dt)) c->velocity.y *= -1;
				else if(c->collidesX(pipes[n], dt)) c->velocity.x *= -1;
				else if(c->collidesZ(pipes[n], dt)) c->velocity.z *= -1;
				break;
			}
		}
	}
	c->position += c->velocity * dt;
	c->move(c->position);
	c->rot += glm::vec3(0, .5, 0);
	c->rotate(c->rot);
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
	camcube->velocity += gravity * dt;
	if(camcube->velocity.y < termvel.y) {
		camcube->velocity.y = termvel.y;
	}
	for(int n = 0; n < cubes.size(); n++) {
		if(cubes[n]->collidesY(camcube, dt)) {
			if(cubes[n]->collidesBottomY(camcube, dt)) jump = false;
			camcube->velocity.y = 0;
			break;
		}
	}
	onpipe = -1;
	for(int n = 0; n < pipes.size(); n++) {
		if(pipes[n]->collidesBottomY(camcube, dt)) {
			onpipe = n;
			jump = false;
			camcube->velocity.y = 0;
			break;
		}
	}
} // moves by physics instead of input

void motion(int x, int y) {
	angle.x -= (x - lastx) * mousespeed * dt; // phi
	angle.y -= (y - lasty) * mousespeed * dt; // theta
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

void spawnsPrize(Cube* c, Cube* Zoidberg) {
	if (c->collidesBottomY(Zoidberg, dt) && c->velocity.y == 0 && !Zoidberg->hit) {
		switch(Zoidberg->prizetype) {
			case MUSHROOM:
				prizes.push_back(new draw_mushroom(glm::vec3(Zoidberg->position.x, Zoidberg->position.y+Zoidberg->size, Zoidberg->position.z), 
											 glm::vec3(.5, .5, .5), 
											 glm::vec3(0, -90, 0)));
				prizes[prizes.size()-1]->velocity = glm::vec3(mushmovespeed, 0, 0);
				Zoidberg->hit = true;
				break;

			case FLOWER:
				prizes.push_back(new draw_flower(glm::vec3(Zoidberg->position.x, Zoidberg->position.y+Zoidberg->size, Zoidberg->position.z), 
										 glm::vec3(.25, .25, .25), 
										 glm::vec3(0, 0, 0)));
				// flower doesn't move
				Zoidberg->hit = true;
				break;

			case STARMAN:
				prizes.push_back(new draw_star(glm::vec3(Zoidberg->position.x, Zoidberg->position.y+Zoidberg->size, Zoidberg->position.z),
									 glm::vec3(.2, .2, .2), 
									 glm::vec3(0, -90, 0)));
				prizes[prizes.size()-1]->velocity = glm::vec3(.005, .025, 0);
				Zoidberg->hit = true;
				break;
		}
    }    
}

void moveCamera() {
	if(warping1) {
		for(int n = 0; n < cubes.size(); n++) {
			if(cubes[n]->collidesWith(camcube, dt)) {
				float tempx = (pipes[onpipe]->linkedpipe)->position.x;
				float tempy = (pipes[onpipe]->linkedpipe)->position.y;
				float tempz = (pipes[onpipe]->linkedpipe)->position.z;
				bool flag = true;
				camcube->position = glm::vec3(tempx, tempy, tempz);
				camcube->velocity.y = .005;
				warping1 = false;
				warping2 = true;
			}
		} // "warps" until it hits the ground
		camcube->position += camcube->velocity * dt;
		return;
	}
	else if(warping2) {
		if(!(pipes[onpipe]->linkedpipe)->intersectsWith(camcube) && !((pipes[onpipe]->linkedpipe)->hitboxes[0])->collidesBottomY(camcube, dt)) {
			warping2 = false;
			camcube->velocity.y = .015;
		}
		camcube->position += camcube->velocity * dt;
		return;
	}

	setVectors();
	applyGravity();
	
	for(int n = 0; n < cubes.size(); n++) {
		if(!cubes[n]->hit && cubes[n]->prizetype != -1) spawnsPrize(camcube, cubes[n]);
	}

	camcube->velocity.x = 0;
	camcube->velocity.z = 0;
    if(keys['a']) {
		camcube->velocity -= rightvec * movespeed * dt;
		for(int n = 0; n < cubes.size(); n++) {
			if(cubes[n]->collidesX(camcube, dt) || cubes[n]->collidesZ(camcube, dt)) {
				camcube->velocity += rightvec * movespeed * dt;
				break;
			}
		}
		for(int n = 0; n < pipes.size(); n++) {
			if(pipes[n]->collidesX(camcube, dt) || pipes[n]->collidesZ(camcube, dt)) {
				camcube->velocity += rightvec*((float)(movespeed+.01)) * dt;
				break;
			}		
		}
	}
	if(keys['d']) {
		camcube->velocity += rightvec * movespeed * dt;
		for(int n = 0; n < cubes.size(); n++) {
			if(cubes[n]->collidesX(camcube, dt) || cubes[n]->collidesZ(camcube, dt)) {
				camcube->velocity -= rightvec * movespeed * dt;
				break;
			}
		}		
		for(int n = 0; n < pipes.size(); n++) {
			if(pipes[n]->collidesX(camcube, dt) || pipes[n]->collidesZ(camcube, dt)) {
				camcube->velocity -= rightvec*((float)(movespeed+.01)) * dt;
				break;
			}		
		}
	}
	if(keys['w']) {
		camcube->velocity += forward * movespeed * dt;
		for(int n = 0; n < cubes.size(); n++) {
			if(cubes[n]->collidesX(camcube, dt) || cubes[n]->collidesZ(camcube, dt)) {
				camcube->velocity -= forward * movespeed * dt;
				break;
			}
		}
		for(int n = 0; n < pipes.size(); n++) {
			if(pipes[n]->collidesX(camcube, dt) || pipes[n]->collidesZ(camcube, dt)) {
				camcube->velocity -= forward*((float)(movespeed+.01)) * dt;
				break;
			}		
		}
	}
	if(keys['s']) {
		camcube->velocity -= forward * movespeed * dt;
		for(int n = 0; n < cubes.size(); n++) {
			if(cubes[n]->collidesX(camcube, dt) || cubes[n]->collidesZ(camcube, dt)) {
				camcube->velocity += forward * movespeed * dt;
				break;
			}
		}		
		for(int n = 0; n < pipes.size(); n++) {
			if(pipes[n]->collidesX(camcube, dt) || pipes[n]->collidesZ(camcube, dt)) {
				camcube->velocity += forward*((float)(movespeed+.01)) * dt;
				break;
			}		
		}
	}
	if(keys['c'] && onpipe != -1) {
		if(pipes[onpipe]->linkedpipe != NULL) {
			warping1 = true;
			camcube->velocity.y = -.025;
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

	if(flag->collidesWith(camcube, dt)) state = MENU_STATE; // win the level!
	
	camcube->position += camcube->velocity * dt;
	if (camcube->position.y < -50) {
		numlives--;
		reset();
	}
    
	for(int n = 0; n < prizes.size(); n++) {
		if(strcmp(prizes[n]->type, "mushroom") == 0) {
			mushroomAI(prizes[n]);
			if(prizes[n]->collidesWith(camcube, dt)) {
				camcube->position.y -= camcube->size/2;
				camcube->size = cubesize*2;
				camcube->position.y += camcube->size/2;
				// grows
				
				prizes.erase(prizes.begin()+n, prizes.begin()+n+1);
				n--;
			}
		}
		else if(strcmp(prizes[n]->type, "star") == 0) {
			starAI(prizes[n]);
			if(prizes[n]->collidesWith(camcube, dt)) {
				invincible = true;
				movespeed *= 2;
				prizes.erase(prizes.begin()+n, prizes.begin()+n+1);
				n--;
			}
		}
		else if(strcmp(prizes[n]->type, "flower") == 0) {
			if(prizes[n]->collidesWith(camcube, dt)) {
				hasfire = true;
				prizes.erase(prizes.begin()+n, prizes.begin()+n+1);
				n--;
			}
		}
	}
	
	if(!goomba->destroyed) simpleAI(goomba);	
	for(int n = 0; n < fireballs.size(); n++) {
		if(fireballAI(fireballs[n])) {
			fireballs.erase(fireballs.begin()+n, fireballs.begin()+n+1);
			n--;
		}
	}

    if(numlives<0) {
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

float sum = 0;

void idle() {
//	dt = 3*(glutGet(GLUT_ELAPSED_TIME)-lastidle)/(1000.0/MAX_FPS);
//	cout << dt << endl;
	lastidle = glutGet(GLUT_ELAPSED_TIME);
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

	if(!goomba->destroyed) goomba->draw();	
	flag->draw();
	for( int i = 0; i < pipes.size(); i++) pipes[i]->draw();
	coin->draw();
	for(int n = 0; n < fireballs.size(); n++) fireballs[n]->draw();
	
	for(int n = 0; n < prizes.size(); n++) prizes[n]->draw();
	
	glUseProgram(program);
	glEnableVertexAttribArray(attribute_texcoord);
	glEnableVertexAttribArray(attribute_coord3d);

#ifdef DRAW_HITBOXES
	(goomba->hitboxes[0])->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);
	(coin->hitboxes[0])->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);
	for(int n = 0; n < fireballs.size(); n++)
		(fireballs[n]->hitboxes[0])->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);
	for(int n = 0; n < prizes.size(); n++) {
		for(int m = 0; m < (prizes[n]->hitboxes).size(); m++) {
			(prizes[n]->hitboxes[m])->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);
		}
	}
	for(int l = 0; l < pipes.size(); l++) {
	  for(int m = 0; m < pipes[l]->hitboxes.size(); m++) {
	    (pipes[l]->hitboxes[m])->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);
	  } // need pipe extension thats just the cylinder
	}
	for(int n = 0; n < flag->hitboxes.size(); n++) {
	  (flag->hitboxes[n])->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);
	} // needs to be taller
#endif

	bg->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);
//	camcube->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);

	for(int n = 0; n < cubes.size(); n++) cubes[n]->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);
	
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

#ifdef PRINT_FPS
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

    //set the projection matrix to be orthographic, but save the old one first...
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0,1,0,1);
	
    //clear the model-view matrix before we render that quad...
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	char* s = new char[20];
	sprintf(s, "%.2f FPS", 1000.0/(glutGet(GLUT_ELAPSED_TIME) - lastframe));
	renderGLUTText(0.0, 0.0, s, mPoint(0, 0, 0)); // x, y, string, color

	lastframe = glutGet(GLUT_ELAPSED_TIME);
   
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
	
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
#endif // posts FPS to screen
	
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
	for(int n = 0; n < cubes.size(); n++) {
		delete cubes[n];
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
//		if(key == 'r') {
//			reset();
//		} // reset
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
		  if(hasfire && fireballs.size() < 5) {
			draw_fireball* newfire = new draw_fireball(glm::vec3(camcube->position.x, camcube->position.y, camcube->position.z), 
													   glm::vec3(.5, .5, .5), 
													   glm::vec3(0, 0, 0));
			newfire->velocity.x = (lookat.x)*firemovespeed;
			newfire->velocity.y = (lookat.y)*firemovespeed;
			newfire->velocity.z = (lookat.z)*firemovespeed;
			fireballs.push_back(newfire);
//			cout << "mouse click" <<endl;
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
	srand(time(NULL));
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
            cubes.push_back(new Cube(cubesize*(n%100), 0.0, -m*cubesize, "groundblock", cubesize));
			ncubes++;
		}
	}    
    for (int n = 0; n < pathlength/16; n++) {
        cubes.push_back(new Cube(cubesize*n*16, 6 * cubesize, -(pathwidth-1)/2*cubesize, ("questionblock"), cubesize, rand()%3));
	} // floating ? cubes
    for( int o = 0; o < pathlength/32; o++) {
      pipes.push_back(new draw_pipe(glm::vec3(cubesize*o*32+20, 1, -(pathwidth-1)/2*cubesize), glm::vec3(.1, .17, .1), glm::vec3(0, 0, 0)));	    
    }
	pipes[0]->linkedpipe = pipes[pathlength/32 - 1]; // links first pipe to last pipe
	pipes[pathlength/32 -1]->linkedpipe = pipes[0]; // links last pipe to first pipe
	flag = new draw_flag(glm::vec3(cubesize*6*16, 8, -(pathwidth-1)/2*cubesize), glm::vec3(.75, .75, .75), glm::vec3(0, 90, 0)); 
	camcube = new Cube(0, 3*cubesize, -(pathwidth-1)/2*cubesize, "brickblock", cubesize); 
    //these are all of the graphics. they can be easily modified so let me know

	goomba = new draw_goomba(glm::vec3(20 * cubesize, 3*cubesize, -4 * cubesize), glm::vec3(.5, .5, .5), glm::vec3(0, -90, 0));
		
	coin = new draw_coin(glm::vec3(10, 3,-7), glm::vec3(.025, .025, .025), glm::vec3(0, 20, 90)); 
	
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
