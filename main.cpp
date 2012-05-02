// Jeremy Vercillo, Ryan Liebscher, Natalia Woodbine
// 2/9/12 - 5/4/12
// CSE20212 Final Project - Mario 3D
// Main driver

#define PRINT_LOADING
//#define DEBUG_LEVEL
#define PLAY_SOUNDS
#define SKIP_MENUS
//#define DRAW_HITBOXES
#define PRINT_FPS
//#define OBJLOADER_DEBUG
#define LITE_MODE // less enemies for faster loading

#include <string>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <stdint.h>
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

#ifdef PLAY_SOUNDS
#include "FMODapi/inc/fmod.h"
#include "FMODapi/inc/fmod.hpp"
#include "FMODapi/inc/fmod_errors.h"
#endif

#include "Cube/Cube.h"
#include "Shaders/shader_utils.h"
#include "objLoader/draw_flower.h"
#include "objLoader/draw_shell.h"
#include "objLoader/draw_mushroom.h"
#include "objLoader/draw_goomba.h"
#include "objLoader/draw_koopa.h"
#include "objLoader/draw_enemy.h"
#include "objLoader/draw_pipe.h"
#include "objLoader/draw_flag.h"
#include "objLoader/draw_star.h"
#include "objLoader/draw_coin.h"
#include "objLoader/draw_fireball.h"
// local includes

#ifndef GLUT_KEY_ESC
#define GLUT_KEY_ESC 27
#endif

using namespace std;

GLuint program;
GLint attribute_coord3d, attribute_texcoord;
GLint uniform_mvp;

#define TITLE_STATE 0
#define MENU_STATE 1
#define GAME_STATE 2

#define BLOCK_SIZE 512 

#ifdef SKIP_MENUS
int state = GAME_STATE;
#else
int state = TITLE_STATE;
#endif

GLuint title_id;
GLuint vbo_title_vertices;
GLuint vbo_title_texcoords;
GLuint ibo_title_elements;

#ifdef PLAY_SOUNDS
bool musicplaying = false;
FMOD::System* fmodSystem;	// the global variable for talking to FMOD
FMOD::Sound *music;
FMOD::Sound *starsound;
FMOD::Sound *coinsound;
FMOD::Sound *jumpsound;
FMOD::Sound *prizesound;
FMOD::Sound *mushgetsound;
FMOD::Sound *mushlosesound;
FMOD::Sound *stompsound;
FMOD::Sound *fireballsound;
FMOD::Sound *shellsound;
FMOD::Sound *deathsound;
FMOD::Sound *gameoversound;
FMOD::Sound *winsound;
FMOD::Sound *oneup;
FMOD::Sound *pipesound;
FMOD::Sound *flagsound;
FMOD::Sound *breaksound;
FMOD::Channel* starChannel;
FMOD::Channel* musicChannel;
FMOD_RESULT resultchan1;
FMOD_RESULT resultchan2;
#endif

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
bool winning = false;

static glm::vec3 angle;
static glm::vec3 forward;
static glm::vec3 rightvec;
static glm::vec3 lookat;

//A* terms go here

int keys[256] = {0}; // array of whether keys are pressed

int cubesize = 2;
double pathwidth = 4;
int pathlength = 100; // these 2 change for each level
vector<Cube*> cubes; // array of cubes
Cube* title; // title graphic
Cube* loading; // loading screen
Cube* border; // menu graphic
Cube* startbutton; // start button graphic
Cube* quitbutton; // quit button graphic
Cube* settings1; // normal gravity
Cube* settings2; // low gravity
Cube* bg; // background skycube
Cube* camcube; // player "model"
bool loadscreendraw = false;

#define MUSHROOM 0
#define STARMAN 1
#define FLOWER 2
#define SHELL 3

vector<draw_object*> prizes; // flowers, mushrooms, starmen, etc
vector<draw_pipe*> pipes;
vector<draw_fireball*> fireballs;
vector<draw_coin*> coins;
vector<draw_enemy*> enemies; // goombas

draw_flag* flag;

glm::mat4 view, projection;

GLfloat diffuse[] = { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat specular[] = { 20 * cubesize, cubesize, -4 * cubesize, 1.0f }; // lighting coordinates and values
GLfloat specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };

float movespeed = 0.01;
float aimovespeed = movespeed * .5;
float mushmovespeed = 0.01;
float firemovespeed = .1;
float mousespeed = 0.002;
float jumpvel = .1;
glm::vec3 gravity = glm::vec3(0, -.00045, 0);
glm::vec3 termvel = glm::vec3(0, -.1, 0);
bool lowgrav = false;

float dt = 5; // for time based movement
int idlecount = 0;
float lastidle = 0;
float lastframe = 0; // last frame in ms from GLUT_ELAPSED_TIME
float MAX_FPS = 60.0; // 60 frames per second
int freezetime = 0;

bool hasfire = false;
int invincible = 0;

int levelnum = 0;
int numlives = 3;
int coincount = 0;

void loadDebugLevel();
void loadWorld1_1();

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
	c->position += c->velocity * dt;
	if(c->velocity.y < termvel.y) c->velocity = termvel;
	for(int n = 0; n < cubes.size(); n++) {
		if(cubes[n]->collidesWith(c, dt)) {
			c->position -= c->velocity * dt;
			c->velocity = glm::vec3(0, 0, 0);
			break;
		}
	}
} // ai physics

#ifdef PLAY_SOUNDS
void AudioError(FMOD_RESULT result)	// this checks for FMOD errors
{						
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}
}

void initAudio() {
	FMOD_RESULT result;
	
	result = FMOD::System_Create(&fmodSystem);
	AudioError(result);
	
	result = fmodSystem->init(32, FMOD_INIT_NORMAL, 0);
	AudioError(result);
	
	resultchan1 = fmodSystem->createStream("Sounds/mario.wav", 
									  FMOD_SOFTWARE | FMOD_LOOP_NORMAL, 0, &music);
	AudioError(result);
	
	resultchan2 = fmodSystem->createStream("Sounds/mariostar.wav",
									   FMOD_SOFTWARE | FMOD_LOOP_NORMAL, 0, &starsound);
	AudioError(resultchan2);
	
	result = fmodSystem->createSound("Sounds/mariocoin.wav", FMOD_SOFTWARE, 0, 
									 &coinsound);
	AudioError(result);
	
	result = fmodSystem->createSound("Sounds/mariojump.wav", FMOD_SOFTWARE, 0, 
									 &jumpsound);
	AudioError(result);
	
	result = fmodSystem->createSound("Sounds/marioprize.wav", FMOD_SOFTWARE, 0, 
									 &prizesound);
	AudioError(result);
	
	result = fmodSystem->createSound("Sounds/mariogetshroom.wav", FMOD_SOFTWARE, 0, 
									 &mushgetsound);
	AudioError(result);
	
	result = fmodSystem->createSound("Sounds/marioloseshroom.wav", FMOD_SOFTWARE, 0, 
									 &mushlosesound);
	AudioError(result);
	
	result = fmodSystem->createSound("Sounds/mariostomp.wav", FMOD_SOFTWARE, 0, 
									 &stompsound);
	AudioError(result);
	
	result = fmodSystem->createSound("Sounds/marioshootfire.wav", FMOD_SOFTWARE, 0, 
									 &fireballsound);
	AudioError(result);
	
	result = fmodSystem->createSound("Sounds/marioshell.wav", FMOD_SOFTWARE, 0, 
									 &shellsound);
	AudioError(result);

	result = fmodSystem->createSound("Sounds/mariodeath.wav", FMOD_SOFTWARE, 0,
									 &deathsound);
	AudioError(result);
	
	result = fmodSystem->createSound("Sounds/mariogo.wav", FMOD_SOFTWARE, 0,
									 &gameoversound);
	AudioError(result);
	
	result = fmodSystem->createSound("Sounds/mariowin.wav", FMOD_SOFTWARE, 0,
									 &winsound);
	AudioError(result);
	
	result = fmodSystem->createSound("Sounds/1-up.wav", FMOD_SOFTWARE, 0,
									 &oneup);
	AudioError(result);
	
	result = fmodSystem->createSound("Sounds/warppipe.wav", FMOD_SOFTWARE, 0,
									 &pipesound);
	AudioError(result);
	
	result = fmodSystem->createSound("Sounds/breakblock.wav", FMOD_SOFTWARE, 0,
									 &breaksound);
	AudioError(result);
	
	result = fmodSystem->createSound("Sounds/flagpole.wav", FMOD_SOFTWARE, 0, 
								     &flagsound);
	AudioError(result);
	
	resultchan1 = fmodSystem->playSound(FMOD_CHANNEL_FREE, music, true, &musicChannel);
	AudioError(resultchan1);
	
	resultchan2 = fmodSystem->playSound(FMOD_CHANNEL_FREE, starsound, true, &starChannel);
	AudioError(resultchan2);
	
	musicChannel->setPriority(1);
	starChannel->setPriority(0);
}

void playmusic() {
	musicChannel->setPosition(0, FMOD_TIMEUNIT_MS);
	starChannel->setPosition(0, FMOD_TIMEUNIT_MS);
	musicChannel->setPaused(false);
	starChannel->setPaused(true);
	musicplaying = true;
}

void playsound(FMOD::Sound* snd) {
	FMOD_RESULT result;
	result = fmodSystem->playSound(FMOD_CHANNEL_FREE, snd, false, NULL);
	AudioError(result);
}
#endif

void reset() {
	if(numlives < 0) {
		state = TITLE_STATE; // game over state
#ifdef PLAY_SOUNDS
		playsound(gameoversound);
#endif
		numlives = 3;
	}

	hasfire = false;
	invincible = 0;
	winning = false;
	musicplaying = false;
	camcube = new Cube(0, 2*cubesize, -(pathwidth-1)/2*cubesize, "brickblock", cubesize); 

	angle = glm::vec3(M_PI/2, -M_PI/32, 0);
}

void die() {
	freezetime = 240;
	musicChannel->setPaused(true);
	starChannel->setPaused(true);
	if (numlives>=0) playsound(deathsound);
	reset();
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

bool simpleAI(draw_enemy* c) {    
	int behavior;   // state ai is in, 0 is normal patrol, 1 is chase 
    float dist = distance(camcube->position.x, camcube->position.y, camcube->position.z, c->position.x, c->position.y, c->position.z);
    if (dist < (10 * cubesize) && camcube->position.y <= (c->position.y + 2*cubesize)) behavior = 1;
    else behavior = 0;

    if(c->destroycountdown < 0 && c->knockbackcountdown < 0) {
		c->velocity = glm::vec3(0, c->velocity.y, 0);
		switch (behavior){
			case 0:
				c->velocity.z += pathwidthcheck * (aimovespeed * .1f) * dt;
//				c->velocity.x -= pathlengthcheck * (aimovespeed * .25f) * dt;
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

	if(c->destroycountdown > 0) {
		c->destroycountdown--;
		if(c->destroycountdown == 0) {
			return true;
		}
	}
	else if(c->knockbackcountdown >= 0) {
		c->knockbackcountdown--;
	}
	else if(c->collidesWith(camcube, dt) && !c->destroyed && c->destroycountdown < 0){
		if(c->collidesBottomY(camcube, dt) && !c->collidesX(camcube, dt) && !c->collidesZ(camcube, dt)) {
			if(invincible <= 0) camcube->velocity.y = jumpvel/2;
			if(strcmp(c->type, "koopa") == 0) {
				((draw_koopa*)c)->shell->move(glm::vec3(c->position.x, c->position.y, c->position.z));
				((draw_koopa*)c)->shell->rotate(glm::vec3(c->rot.x, c->rot.y, c->rot.z));
				prizes.push_back(((draw_koopa*)c)->shell);
			}
#ifdef PLAY_SOUNDS
			if(strcmp(c->type, "koopa") == 0) playsound(shellsound);
			else playsound(stompsound);
#endif
			return true;
		}
		else {
			if(invincible > 0) {
				c->destroycountdown = 60;
				// angle between user and gooma
				float dx = abs(c->position.x - camcube->position.x);
				float dz = abs(c->position.z - camcube->position.z);
				float anglebetween = atan(dz/dx);
				
				int modz = 1; // modifies to make up for losing part of circle in atan
				int modx = 1;
				if(c->position.z-camcube->position.z < 0) modz = -1;
				if(c->position.x-camcube->position.x < 0) modx = -1;
				c->velocity.y = .075;
				c->velocity.x = 16*modx*aimovespeed*cos(anglebetween);
				c->velocity.z = 16*modz*aimovespeed*sin(anglebetween);
				
				if(strcmp(c->type, "koopa") == 0) playsound(shellsound);
				else playsound(stompsound);
			}
			else if(camcube->size != cubesize || hasfire) {
//				printf("Mushroom lost\n");
				playsound(mushlosesound);
				hasfire = false;
				if(camcube->size != cubesize){
				camcube->position.y -= camcube->size/2;
				camcube->size = cubesize;
				camcube->position.y += camcube->size/2;
				}
				c->knockbackcountdown = 60;
				
				// angle between user and gooma
				float dx = abs(c->position.x - camcube->position.x);
				float dz = abs(c->position.z - camcube->position.z);
				float anglebetween = atan(dz/dx);
				
				int modz = 1; // modifies to make up for losing part of circle in atan
				int modx = 1;
				if(c->position.z-camcube->position.z < 0) modz = -1;
				if(c->position.x-camcube->position.x < 0) modx = -1;
				c->velocity.y = .075;
				c->velocity.x = 8*modx*aimovespeed*cos(anglebetween);
				c->velocity.z = 8*modz*aimovespeed*sin(anglebetween);
			}
			else {
//				printf("You died\n");
				numlives--;
				die();
			}
		}
	}
	for(int n = 0; n < enemies.size(); n++) {
		if(enemies[n] == c) continue;
		if(c->collidesX(enemies[n], dt)) c->velocity.x = 0;
		else if(c->collidesZ(enemies[n], dt)) c->velocity.z = 0;
		else if(c->collidesY(enemies[n], dt)) c->velocity.y = 0;
	}
	
	c->position += c->velocity * dt;	
	glm::vec3 newpos(c->position.x,c->position.y,c->position.z);
	c->move(newpos);
	
	float angletouser = 180/M_PI*atan((newpos.z-camcube->position.z)/(newpos.x-camcube->position.x));
	if(newpos.x-camcube->position.x > 0) c->rotate(glm::vec3(0, -90-angletouser, 0));
	else c->rotate(glm::vec3(0, 90-angletouser, 0));
	return false;
} // Simple test AI

bool mushroomAI(draw_object* c) {
	c->velocity += (gravity) * dt;
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

bool shellAI(draw_shell* c) {
	c->velocity.y += gravity.y * dt;
	if(c->velocity.y <= termvel.y) return true;
	for(int n = 0; n < enemies.size(); n++) {
		if(c->collidesWith(enemies[n], dt) && (c->velocity.x != 0 || c->velocity.z != 0)) {
			enemies.erase(enemies.begin()+n, enemies.begin()+n+1);
			n--;
#ifdef PLAY_SOUNDS
			playsound(shellsound);
#endif
		}
	}
	
	if(c->collidesWith(camcube, dt) && c->velocity.x == 0 && c->velocity.z == 0) {
		if(camcube->velocity.y < 0) {
			camcube->velocity.y = jumpvel/2;
		} // going down bounces
		
		// angle between user and enemy
		float dx = abs(c->position.x - camcube->position.x);
		float dz = abs(c->position.z - camcube->position.z);
		float anglebetween = atan(dz/dx);
		
		int modz = 1; // modifies to make up for losing part of circle in atan
		int modx = 1;
		if(c->position.z-camcube->position.z < 0) modz = -1;
		if(c->position.x-camcube->position.x < 0) modx = -1;
		c->velocity.y = 0;
		c->velocity.x = 20*modx*aimovespeed*cos(anglebetween);
		c->velocity.z = 20*modz*aimovespeed*sin(anglebetween);
		
#ifdef PLAY_SOUNDS
		playsound(shellsound);
#endif
	}
	else if(c->collidesWith(camcube, dt) && invincible <= 0) {
		numlives--;
		die();
		return true;	
	} // is moving
	
	for(int n = 0; n < cubes.size(); n++) {
		if(c->collidesTopY(cubes[n], dt)) {
			c->velocity.y = 0;
			break;
		}
	} 
	for(int n = 0; n < pipes.size(); n++) {
		if(c->collidesTopY(pipes[n], dt)) {
			c->velocity.y = 0;
			break;
		}
	} // collision with floors
	
	for(int i = 0; i < cubes.size(); i++) {
		if(c->collidesWith(cubes[i], dt)) {
			if(c->collidesX(cubes[i], dt)) c->velocity.x *= -1;
			else if(c->collidesZ(cubes[i], dt)) c->velocity.z *= -1;
			break;
		}
	}
	for(int n = 0; n < pipes.size(); n++) {
		if(c->collidesWith(pipes[n], dt)) {
			if(c->collidesX(pipes[n], dt)) c->velocity.x *= -1;
			else if(c->collidesZ(pipes[n], dt)) c->velocity.z *= -1;
			break;
		}
	}
	
	c->position += c->velocity * dt;
	c->move(c->position);
	return false;
}

bool fireballAI(draw_fireball* c) {
  if(c->distancetraveled > 100) return true;
  else {
	c->distancetraveled += 1;
	c->velocity.y += 2*gravity.y * dt;
	if(c->velocity.y > .2) c->velocity.y = .2;
	for(int n = 0; n < enemies.size(); n++) {
		if(c->collidesWith(enemies[n], dt)) {
			enemies.erase(enemies.begin()+n, enemies.begin()+n+1);
			n--;
#ifdef PLAY_SOUNDS
			playsound(shellsound);
#endif
			return true;
		}
	}
	
	bool ychanged = false;
	for(int n = 0; n < pipes.size(); n++) {
		if(c->collidesY(pipes[n], dt)) {
			c->velocity.y *= -1;
			ychanged = true;
			break;
		}
	}
	for(int n = 0; n < cubes.size(); n++) {
		if(c->collidesY(cubes[n], dt)) {
			c->velocity.y *= -1;
			ychanged = true;
			break;
		}
	}

	if(!ychanged) {
		for(int i = 0; i < cubes.size(); i++) {
			if(c->collidesWith(cubes[i], dt)) {
				if(c->collidesX(cubes[i], dt)) c->velocity.x *= -.75;
				else if(c->collidesZ(cubes[i], dt)) c->velocity.z *= -.75;
				break;
			}
		}
		for(int n = 0; n < pipes.size(); n++) {
			if(c->collidesWith(pipes[n], dt)) {
				if(c->collidesX(pipes[n], dt)) c->velocity.x *= -.75;
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
	c->velocity.y += 2*gravity.y * dt;
	if(c->velocity.y > .075) c->velocity.y = .075;
	bool ychanged = false;
	for(int n = 0; n < cubes.size(); n++) {
		if(c->collidesY(cubes[n], dt)) {
			c->velocity.y *= -1;
			ychanged = true;
			break;
		}
	}
	for(int n = 0; n < pipes.size(); n++) {
		if(c->collidesY(pipes[n], dt)) {
			c->velocity.y *= -1;
			ychanged = true;
			break;
		}
	}
	if(!ychanged) {
		for(int i = 0; i < cubes.size(); i++) {
			if(c->collidesWith(cubes[i], dt)) {
				if(c->collidesX(cubes[i], dt)) c->velocity.x *= -1;
				else if(c->collidesZ(cubes[i], dt)) c->velocity.z *= -1;
				break;
			}
		}
		for(int n = 0; n < pipes.size(); n++) {
			if(c->collidesWith(pipes[n], dt)) {
				if(c->collidesX(pipes[n], dt)) c->velocity.x *= -1;
				else if(c->collidesZ(pipes[n], dt)) c->velocity.z *= -1;
				break;
			}
		}
	}
	
	c->position += c->velocity * dt;
	c->move(c->position);
	c->rot += glm::vec3(0, 2.5, 0);
	c->rotate(c->rot);
}

void spawnsPrize(Cube* c, Cube* Zoidberg) {
	if (c->collidesBottomY(Zoidberg, dt) && !Zoidberg->hit) {
#ifdef PLAY_SOUNDS
		playsound(prizesound);
#endif
		prizes.push_back(Zoidberg->prize);
		// darkened texture
		Zoidberg->hit = true;
		Zoidberg->prize = NULL;
		if(strcmp(prizes[prizes.size()-1]->type, "mushroom") == 0) {
			if(((draw_mushroom*)(prizes[prizes.size()-1]))->is1up == false) prizes[prizes.size()-1]->velocity = glm::vec3(mushmovespeed, 0, 0);
			else prizes[prizes.size()-1]->velocity = glm::vec3(0, 0, 0);
		}
		else if(strcmp(prizes[prizes.size()-1]->type, "flower") == 0) {}
		else if(strcmp(prizes[prizes.size()-1]->type, "star") == 0) {
			prizes[prizes.size()-1]->velocity = glm::vec3(.025, .1, 0);
		}
    }    
}

int initShaders() {
	GLuint vs, fs;
	if((vs = create_shader("Cube/cubeshader.v.glsl", GL_VERTEX_SHADER)) == 0) return 0;
	if((fs = create_shader("Cube/cubeshader.f.glsl", GL_FRAGMENT_SHADER)) == 0) return 0;
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

void moveCamera() {
	if(warping1) {
		for(int n = 0; n < cubes.size(); n++) {
			if(cubes[n]->collidesWith(camcube, dt)) {
				camcube->position = glm::vec3((pipes[onpipe]->linkedpipe)->position.x, (pipes[onpipe]->linkedpipe)->position.y, (pipes[onpipe]->linkedpipe)->position.z);
				camcube->velocity.y = jumpvel/2;
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
			camcube->velocity.y = jumpvel/2;
		}
		camcube->position += camcube->velocity * dt;
		return;
	}
	
	if(winning) {
		for(int n = 0; n < cubes.size(); n++) {
			if(cubes[n]->collidesWith(camcube, dt)) {
				playsound(winsound);
				freezetime = 480;
			}
		}
		camcube->position += camcube->velocity * dt;
		return;
	}

	if(invincible > 0) invincible--;

	setVectors();
	applyGravity();
	
	for(int n = 0; n < cubes.size(); n++) {
		if(cubes[n]->destroycountdown > 0) {
			cubes[n]->destroycountdown--;
			if(cubes[n]->destroycountdown == 0) {
				// turn into darkened texture
				cubes[n]->hit = true;
			}
		}
		if(cubes[n]->prize != NULL) spawnsPrize(camcube, cubes[n]);
		else if(strcmp(cubes[n]->texturename, "questionblock") == 0 && camcube->collidesBottomY(cubes[n], dt) && !camcube->collidesX(cubes[n], dt) && !camcube->collidesZ(cubes[n], dt) && !cubes[n]->hit) {
			if(camcube->velocity.y > 0) {
				coincount++; // get coin
#ifdef PLAY_SOUNDS
				playsound(coinsound);
#endif
			}
			if(cubes[n]->destroycountdown < 0) cubes[n]->destroycountdown = 300;
		}
		 if(strcmp(cubes[n]->texturename, "brickblock") == 0 && camcube->collidesBottomY(cubes[n], dt) && !camcube->collidesX(cubes[n], dt) && !camcube->collidesZ(cubes[n], dt)) {
			playsound(breaksound);
			cubes.erase(cubes.begin()+n, cubes.begin()+n+1);
			camcube->velocity.y = -.1;
		}
		else if(camcube->collidesBottomY(cubes[n], dt)) camcube->velocity.y = -.01;
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
			playsound(pipesound);
			camcube->velocity.y = -jumpvel;
		}
	}
	if(keys[' '] && !jump) {
		camcube->velocity.y = jumpvel;
		jump = true;
#ifdef PLAY_SOUNDS
		playsound(jumpsound);
#endif
	}
	if(jump && !keys[' '] && camcube->velocity.y > jumpvel/5) {
		camcube->velocity.y = jumpvel/5;
	}
	// key input

	if(flag->collidesWith(camcube, dt)) {
		musicChannel->setPaused(true);
		starChannel->setPaused(true);
		playsound(flagsound);
		winning = true;
		camcube->velocity = glm::vec3(0, -jumpvel/4, 0);
		return;
	}
	for(int n = 0; n < coins.size(); n++) {
		coins[n]->rotate(glm::vec3(coins[n]->rot.x, coins[n]->rot.y, coins[n]->rot.z+1));
		if(coins[n]->collidesWith(camcube, dt)) {
			coincount++;
			if(coincount == 100) {
				numlives++;
				playsound(oneup);
				coincount = 0;
			}
			
			coins.erase(coins.begin()+n, coins.begin()+n+1);
			n--;
#ifdef PLAY_SOUNDS
			playsound(coinsound);
#endif
		}
	}
	for(int n = 0; n < prizes.size(); n++) {
		if(strcmp(prizes[n]->type, "mushroom") == 0) {
			mushroomAI(prizes[n]);
			if(prizes[n]->collidesWith(camcube, dt)) {
				if(((draw_mushroom*)(prizes[n]))->is1up) {
					numlives++;
#ifdef PLAY_SOUNDS
					playsound(oneup);
#endif
				}
				else {
					camcube->position.y -= camcube->size/2;
					camcube->size = cubesize*2;
					camcube->position.y += camcube->size/2;
#ifdef PLAY_SOUNDS
					playsound(mushgetsound);
#endif
				} // grows
				
				prizes.erase(prizes.begin()+n, prizes.begin()+n+1);
				n--;
			}
		}
		else if(strcmp(prizes[n]->type, "star") == 0) {
			starAI(prizes[n]);
			if(prizes[n]->collidesWith(camcube, dt)) {
				if(invincible <= 0) {
					movespeed *= 1.5; // only first starman
					musicChannel->setPaused(true);
					starChannel->setPosition(0, FMOD_TIMEUNIT_MS);
					starChannel->setPaused(false);
				}
				invincible = 600; // 10 seconds
				prizes.erase(prizes.begin()+n, prizes.begin()+n+1);
				n--;
			}
		}
		else if(strcmp(prizes[n]->type, "flower") == 0) {
			if(prizes[n]->collidesWith(camcube, dt)) {
				hasfire = true;
				prizes.erase(prizes.begin()+n, prizes.begin()+n+1);
				n--;
#ifdef PLAY_SOUNDS
				playsound(mushgetsound);
#endif
			}
		}
		else if(strcmp(prizes[n]->type, "shell") == 0) {
			if(shellAI((draw_shell*)prizes[n])) {
				prizes.erase(prizes.begin()+n, prizes.begin()+n+1);
				n--;
			}
		}
	}
	
	for(int n = 0; n < enemies.size(); n++) {
		if(simpleAI(enemies[n])) {
			enemies.erase(enemies.begin()+n, enemies.begin()+n+1);
			n--;
		}
	}
	for(int n = 0; n < fireballs.size(); n++) {
		if(fireballAI(fireballs[n])) {
			fireballs.erase(fireballs.begin()+n, fireballs.begin()+n+1);
			n--;
		}
	}
	
	camcube->position += camcube->velocity * dt;
	if (camcube->position.y < -50) {
		numlives--;
		die();
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
#ifdef PLAY_SOUNDS
	if(!musicplaying) playmusic();
#endif
	moveCamera();
} // idle function for when in game state

void menuIdle() {

} // idle function for menu state

void titleIdle() {
	
} // idle function for title state

void idle() {
//	idlecount++;
//	dt = (glutGet(GLUT_ELAPSED_TIME)-lastidle)/(1000.0/MAX_FPS);
//	cout << (glutGet(GLUT_ELAPSED_TIME)-lastidle)/(1000.0/MAX_FPS) << endl;
//	lastidle = glutGet(GLUT_ELAPSED_TIME);
	if(freezetime > 0) {
		freezetime--;
		if(freezetime == 0 && winning) state = TITLE_STATE;
	}
	else if(state == TITLE_STATE) titleIdle();
	else if(state == MENU_STATE) menuIdle();
	else if(state == GAME_STATE) gameIdle();
} // constantly calculates redraws

void timer(int value) {
//	cout << idlecount << endl;
//	idlecount = 0;
	glutTimerFunc((1000.0/MAX_FPS), timer, 0);
	idle();
	if(freezetime <= 0) glutPostRedisplay();
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

	flag->draw();
	for(int n = 0; n < enemies.size(); n++) enemies[n]->draw();
	for(int n = 0; n < pipes.size(); n++) pipes[n]->draw();
	for(int n = 0; n < coins.size(); n++) coins[n]->draw();
	for(int n = 0; n < fireballs.size(); n++) fireballs[n]->draw();
	
	for(int n = 0; n < prizes.size(); n++) prizes[n]->draw();
	
	glUseProgram(program);
	glEnableVertexAttribArray(attribute_texcoord);
	glEnableVertexAttribArray(attribute_coord3d);

#ifdef DRAW_HITBOXES
	for(int n = 0; n < enemies.size(); n++)
		(enemies[n]->hitboxes[0])->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);
	for(int n = 0; n < coins.size(); n++) 
		(coins[n]->hitboxes[0])->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);
	for(int n = 0; n < fireballs.size(); n++)
		(fireballs[n]->hitboxes[0])->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);
	for(int n = 0; n < prizes.size(); n++)
		(prizes[n]->hitboxes[0])->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);
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
	
		//if (!loadscreendraw) title->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);
	loading->draw(view, projection, attribute_coord3d, attribute_texcoord, uniform_mvp);

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
	glDisable(GL_DEPTH_TEST);

    //set the projection matrix to be orthographic, but save the old one first...
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0,1,0,1);
	
    //clear the model-view matrix before we render that quad...
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	if(state == GAME_STATE) {
		char* printlives = new char[10];
		sprintf(printlives, "Lives: %d", numlives);
		renderGLUTText(0.0, 0.95, printlives, mPoint(0, 0, 0));
		
		char* printcoins = new char[10];
		sprintf(printcoins, "Coins: %d", coincount);
		renderGLUTText(0.0, 0.925, printcoins, mPoint(0, 0, 0));
	
		char* printlevel = new char[10];
		sprintf(printlevel, "Level: %d", levelnum);
		renderGLUTText(0.0, 0.975, printlevel, mPoint(0, 0, 0));
		
		char* printpowers = new char[30];
		const char* starpower = ""; 
		const char* mushpower = "";
		const char* firepower = "";
		if(hasfire) firepower = "fire ";
		if(camcube->size == cubesize*2) mushpower = "mushroom ";
		if(invincible > 0) starpower = "starman ";
		sprintf(printpowers, "Powers: %s%s%s", firepower, mushpower, starpower);
		renderGLUTText(0.0, .90, printpowers, mPoint(0, 0, 0));
		
#ifdef PRINT_FPS
		char* fps = new char[20];
		sprintf(fps, "%.2f FPS", 1000.0/(glutGet(GLUT_ELAPSED_TIME) - lastframe));
		renderGLUTText(0.0, 0.0, fps, mPoint(0, 0, 0)); // x, y, string, color
#endif // prints FPS to bottom left

		lastframe = glutGet(GLUT_ELAPSED_TIME);
   }
   
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
	
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

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
	
	if(state == TITLE_STATE) {
		if(key == 'q') {
			glutDestroyWindow(windowid);
			free_resources();
			exit(0);
		}
		loadscreendraw = true;

		enemies.clear();
		prizes.clear();
		cubes.clear();
		pipes.clear();
		fireballs.clear();

		numlives=3;
		coincount=0;
		movespeed=.01;

#ifdef DEBUG_LEVEL
		loadDebugLevel();
#else
		loadWorld1_1();
#endif
		reset();
		state = GAME_STATE; // next state
	}
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
	}
} // watches keyboard

void key_released(unsigned char key, int x, int y) {
	keys[key] = 0; // key is no longer pressed
} // watches keyboard

void mouse_click(int button, int mstate, int x, int y) {
	if (mstate == GLUT_DOWN) {
		if(state == TITLE_STATE) {
			loadscreendraw = true;

			enemies.clear();
			prizes.clear();
			cubes.clear();
			pipes.clear();
			fireballs.clear();

			numlives=3;
			coincount=0;
			movespeed=.01;

#ifdef DEBUG_LEVEL
			loadDebugLevel();
#else
			loadWorld1_1();
#endif
			reset();
			state = GAME_STATE; // next state
		}
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
		  if(hasfire) {
			draw_fireball* newfire = new draw_fireball(glm::vec3(camcube->position.x, camcube->position.y, camcube->position.z), 
													   glm::vec3(.5, .5, .5), 
													   glm::vec3(0, 0, 0));
			newfire->velocity.x = (lookat.x)*firemovespeed;
			newfire->velocity.y = (lookat.y)*firemovespeed;
			newfire->velocity.z = (lookat.z)*firemovespeed;
			fireballs.push_back(newfire);
#ifdef PLAY_SOUNDS
			playsound(fireballsound);
#endif
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

void loadDebugLevel() {
	levelnum = 0;
	pathlength = 100;
	pathwidth = 8;

#ifdef PRINT_LOADING
	cout << "Loading Cubes..." << endl;
#endif

	bg = new Cube(0.0, 0.0, 0.0, "skybox", 3000);
    for (int m = 0; m < pathwidth; m++) {
        for (int n = 0; n < pathlength; n++) {
            cubes.push_back(new Cube(cubesize*n, 0.0, -m*cubesize, "groundblock", cubesize));
		}
	}    
#ifdef PRINT_LOADING
	cout << "Loading Objects..." << endl;
#endif
    for (int n = 0; n < pathlength/16; n++) {
		int r = rand()%4;
		draw_object* newobj;
		switch(r) {
			case MUSHROOM:
				newobj = new draw_mushroom(glm::vec3(cubesize*n*16, 5 * cubesize + 1.5*cubesize, -(pathwidth-1)/2*cubesize-.75), 
										   glm::vec3(.75, .75, .75), 
										   glm::vec3(0, -90, 0));
				break;
			case FLOWER:
				newobj = new draw_flower(glm::vec3(cubesize*n*16, 5 * cubesize + cubesize, -(pathwidth-1)/2*cubesize), 
										 glm::vec3(.25, .25, .25), 
										 glm::vec3(0, 180, 0));
				break;
			case STARMAN:
				newobj = new draw_star(glm::vec3(cubesize*n*16, 5 * cubesize + cubesize, -(pathwidth-1)/2*cubesize),
									 glm::vec3(.2, .2, .2), 
									 glm::vec3(0, -90, 0));
				break;
			default:
				newobj = NULL;
				break;
		}
	
		cubes.push_back(new Cube(cubesize*n*16, 5 * cubesize, -(pathwidth-1)/2*cubesize, ("questionblock"), cubesize, newobj));
	} // floating ? cubes

#ifdef PRINT_LOADING
	cout << "Loading Pipes..." << endl;
#endif
  
	for( int o = 0; o < pathlength/32; o++) {
      pipes.push_back(new draw_pipe(glm::vec3(cubesize*o*32+20, 1, -(pathwidth-1)/2*cubesize), glm::vec3(.1, .17, .1), glm::vec3(0, 0, 0)));	    
    }
	pipes[0]->linkedpipe = pipes[pathlength/32 - 1]; // links first pipe to last pipe
	pipes[pathlength/32 -1]->linkedpipe = pipes[0]; // links last pipe to first pipe
	flag = new draw_flag(glm::vec3(cubesize*6*16, 8, -(pathwidth-1)/2*cubesize), glm::vec3(.75, .75, .75), glm::vec3(0, 90, 0)); 
	camcube = new Cube(0, 2*cubesize, -(pathwidth-1)/2*cubesize, "brickblock", cubesize); 
    //these are all of the graphics. they can be easily modified so let me know

#ifdef PRINT_LOADING
	cout << "Loading Enemies..." << endl;
#endif

	enemies.push_back(new draw_goomba(glm::vec3(20 * cubesize, 3*cubesize, -3 * cubesize), glm::vec3(.5, .5, .5), glm::vec3(0, -90, 0)));
	enemies.push_back(new draw_koopa(glm::vec3(16 * cubesize, 3*cubesize, -1 * cubesize), glm::vec3(3, 3, 3), glm::vec3(0, -90, 0)));
	enemies.push_back(new draw_goomba(glm::vec3(18 * cubesize, 3*cubesize, 0 * cubesize), glm::vec3(.5, .5, .5), glm::vec3(0, -90, 0)));

#ifdef PRINT_LOADING
	cout << "Loading Coins..." << endl;
#endif
	
	for(int n = 0; n < 100; n++) {
		coins.push_back(new draw_coin(glm::vec3(cubesize*n, 1.5, -(pathwidth-3)/2*cubesize), glm::vec3(.025, .025, .025), glm::vec3(90, 0, 90)));
		coins.push_back(new draw_coin(glm::vec3(cubesize*n, 1.5, -(pathwidth+1)/2*cubesize), glm::vec3(.025, .025, .025), glm::vec3(90, 0, 90)));
	}
}

void loadWorld1_1() {
	levelnum = 1;
	
	pathlength = 213;
	pathwidth = 3;
	
	bg = new Cube(0.0, 0.0, 0.0, "skybox", 3000);

	camcube = new Cube(0, 2*cubesize, -(pathwidth-1)/2*cubesize, "brickblock", cubesize); 
	flag = new draw_flag(glm::vec3(200*cubesize, 9, -(pathwidth-1)/2*cubesize), glm::vec3(.75, .75, .75), glm::vec3(0, 90, 0)); 

#ifdef PRINT_LOADING
	cout << "Loading Cubes..." << endl;
#endif

    for (int m = 0; m < pathwidth; m++) {
        for (int n = 0; n < pathlength; n++) {
			if(n == 71 || n == 72 || n == 88 || n == 89 || n == 90 || n == 155 || n == 156) continue;
			cubes.push_back(new Cube(cubesize*n, 0.0, -m*cubesize, "groundblock", cubesize));
			if(n == 136 || n == 137 || n == 138 || n == 139 || n == 142 || n == 143 || n == 144 || n == 145
			|| n == 150 || n == 151 || n == 152 || n == 153 || n == 154 || n == 157 || n == 158 || n == 159
			|| n == 160 || (n >= 183 && n <= 191) || (n == 200 && m == 1)) {
				cubes.push_back(new Cube(cubesize*n, 1*cubesize, -m*cubesize, "groundblock", cubesize));
			} // level 1
			if(n == 137 || n == 138 || n == 139 || n == 142 || n == 143 || n == 144 || n == 151 || n == 152
			|| n == 153 || n == 154 || n == 157 || n == 158 || n == 159 || (n >= 184 && n <= 191)) {
				cubes.push_back(new Cube(cubesize*n, 2*cubesize, -m*cubesize, "groundblock", cubesize));
			} // level 2
			if(n == 138 || n == 139 || n == 142 || n == 143 || n == 152 || n == 153 || n == 154 || n == 157 
			|| n == 158 || (n >= 185 && n <= 191)) {
				cubes.push_back(new Cube(cubesize*n, 3*cubesize, -m*cubesize, "groundblock", cubesize));
			} // level 3
			if(n == 139 || n == 142 || n == 153 || n == 154 || n == 157 || (n >= 186 && n <= 191)) {
				cubes.push_back(new Cube(cubesize*n, 4*cubesize, -m*cubesize, "groundblock", cubesize));
			} // level 4
			if(n >= 187 && n <= 191) {
				cubes.push_back(new Cube(cubesize*n, 5*cubesize, -m*cubesize, "groundblock", cubesize));
			} // 5
			if(n >= 188 && n <= 191) {
				cubes.push_back(new Cube(cubesize*n, 6*cubesize, -m*cubesize, "groundblock", cubesize));
			} // 6
			if(n >= 189 && n <= 191) {
				cubes.push_back(new Cube(cubesize*n, 7*cubesize, -m*cubesize, "groundblock", cubesize));			
			} // 7
			if(n >= 190 && n <= 191) {
				cubes.push_back(new Cube(cubesize*n, 8*cubesize, -m*cubesize, "groundblock", cubesize));			
			} // 8
			if(n == 191) {
				cubes.push_back(new Cube(cubesize*n, 9*cubesize, -m*cubesize, "groundblock", cubesize));			
			} // 9
		}
	} // ground blocks (and stair blocks)
	
	for(int n = 0; n < pathlength; n++) {
		if(n == 20 || n == 22 || n == 24 || n == 79 || n == 81 || n == 96 || n == 102 || n == 120 || n == 131 
		|| n == 132 || n == 173 || n == 171 || n == 170) {
			cubes.push_back(new Cube(cubesize*n, 5*cubesize, -(pathwidth-1)/2*cubesize, "brickblock", cubesize));
		} // bricks	
		if((n >= 82 && n <= 89) || n == 93 || n == 94 || n == 95 || n == 123 || n == 124 || n == 125 || n == 130 || n == 133) {
			cubes.push_back(new Cube(cubesize*n, 10*cubesize, -(pathwidth-1)/2*cubesize, "brickblock", cubesize));
			if(n >= 82 && n <= 89) {
				cubes.push_back(new Cube(cubesize*n, 10*cubesize, -2*cubesize, "brickblock", cubesize));
				cubes.push_back(new Cube(cubesize*n, 10*cubesize, 0*cubesize, "brickblock", cubesize));
			}
		}
		if(n == 65) {
			cubes.push_back(new Cube(cubesize*n, 7*cubesize, -(pathwidth-1)/2*cubesize, "groundblock", cubesize));
			prizes.push_back(new draw_mushroom(glm::vec3(cubesize*n, 7*cubesize + 1.5*cubesize, -(pathwidth-1)/2*cubesize-.76),
											   glm::vec3(.75, .75, .75),
											   glm::vec3(0, -90, 0), 
											   1));
			// should be "used" ? block
		}
	} // floating brick blocks

#ifdef PRINT_LOADING
	cout << "Loading Objects..." << endl;
#endif
	
	draw_object* newobj = NULL;
	for(int n = 0; n < pathlength; n++) {
		if(n == 16) {
			newobj = NULL;
			cubes.push_back(new Cube(cubesize*n, 5*cubesize, -(pathwidth-1)/2*cubesize, "questionblock", cubesize, newobj));
		}
		if(n == 21) {
			newobj = new draw_mushroom(glm::vec3(cubesize*n, 5 * cubesize + 1.5*cubesize, -(pathwidth-1)/2*cubesize-.75), 
									   glm::vec3(.75, .75, .75), 
									   glm::vec3(0, -90, 0));
			cubes.push_back(new Cube(cubesize*n, 5*cubesize, -(pathwidth-1)/2*cubesize, "questionblock", cubesize, newobj));
		}
		if(n == 22) {
			newobj = new draw_flower(glm::vec3(cubesize*n, 10 * cubesize + cubesize, -(pathwidth-1)/2*cubesize), 
									 glm::vec3(.25, .25, .25), 
									 glm::vec3(0, 180, 0));
			cubes.push_back(new Cube(cubesize*n, 10*cubesize, -(pathwidth-1)/2*cubesize, "questionblock", cubesize, newobj));
		}
		if(n == 23) {
			newobj = NULL;
			cubes.push_back(new Cube(cubesize*n, 5*cubesize, -(pathwidth-1)/2*cubesize, "questionblock", cubesize, newobj));
		}
		if(n == 80) {
			newobj = new draw_mushroom(glm::vec3(cubesize*n, 5 * cubesize + 1.5*cubesize, -(pathwidth-1)/2*cubesize-.75), 
									   glm::vec3(.75, .75, .75), 
									   glm::vec3(0, -90, 0));
			cubes.push_back(new Cube(cubesize*n, 5*cubesize, -(pathwidth-1)/2*cubesize, "questionblock", cubesize, newobj));			
		}
		if(n == 96) {
			newobj = NULL;
			cubes.push_back(new Cube(cubesize*n, 10*cubesize, -(pathwidth-1)/2*cubesize, "questionblock", cubesize, newobj));
		}
		if(n == 103) {
			newobj = new draw_star(glm::vec3(cubesize*n, 5 * cubesize + cubesize, -(pathwidth-1)/2*cubesize),
								   glm::vec3(.2, .2, .2), 
								   glm::vec3(0, -90, 0));
			cubes.push_back(new Cube(cubesize*n, 5*cubesize, -(pathwidth-1)/2*cubesize, "questionblock", cubesize, newobj));
		}
		if(n == 108 || n == 111 || n == 114) {
			newobj = NULL;
			cubes.push_back(new Cube(cubesize*n, 5*cubesize, -(pathwidth-1)/2*cubesize, "questionblock", cubesize, newobj));
		}
		if(n == 111) {
			newobj = new draw_mushroom(glm::vec3(cubesize*n, 10 * cubesize + 1.5*cubesize, -(pathwidth-1)/2*cubesize-.75), 
									   glm::vec3(.75, .75, .75), 
									   glm::vec3(0, -90, 0));
			cubes.push_back(new Cube(cubesize*n, 10*cubesize, -(pathwidth-1)/2*cubesize, "questionblock", cubesize, newobj));
		}
		if(n == 131 || n == 132) {
			newobj = NULL;
			cubes.push_back(new Cube(cubesize*n, 10*cubesize, -(pathwidth-1)/2*cubesize, "questionblock", cubesize, newobj));
		}
		if(n == 172) {
			newobj = new draw_flower(glm::vec3(cubesize*n, 10 * cubesize + cubesize, -(pathwidth-1)/2*cubesize), 
									 glm::vec3(.25, .25, .25), 
									 glm::vec3(0, 180, 0));
			cubes.push_back(new Cube(cubesize*n, 5*cubesize, -(pathwidth-1)/2*cubesize, "questionblock", cubesize, newobj));
		}
	} // ? blocks and objs

#ifdef PRINT_LOADING
	cout << "Loading Pipes..." << endl;
#endif
	
	pipes.push_back(new draw_pipe(glm::vec3(cubesize*28, -1.5, -(pathwidth-1)/2*cubesize), glm::vec3(.1, .17, .1), glm::vec3(0, 0, 0)));
	pipes.push_back(new draw_pipe(glm::vec3(cubesize*39, -1, -(pathwidth-1)/2*cubesize), glm::vec3(.1, .17, .1), glm::vec3(0, 0, 0)));
	pipes.push_back(new draw_pipe(glm::vec3(cubesize*47, 1, -(pathwidth-1)/2*cubesize), glm::vec3(.1, .17, .1), glm::vec3(0, 0, 0)));
	pipes.push_back(new draw_pipe(glm::vec3(cubesize*58, 1, -(pathwidth-1)/2*cubesize), glm::vec3(.1, .17, .1), glm::vec3(0, 0, 0)));
	pipes.push_back(new draw_pipe(glm::vec3(cubesize*165, -1.5, -(pathwidth-1)/2*cubesize), glm::vec3(.1, .17, .1), glm::vec3(0, 0, 0)));
	pipes.push_back(new draw_pipe(glm::vec3(cubesize*181, -1.5, -(pathwidth-1)/2*cubesize), glm::vec3(.1, .17, .1), glm::vec3(0, 0, 0)));
	pipes[3]->linkedpipe = pipes[4];
	// pipes

#ifdef PRINT_LOADING
	cout << "Loading Enemies..." << endl;
#endif
	
	enemies.push_back(new draw_goomba(glm::vec3(cubesize*20, cubesize, -2*cubesize), glm::vec3(.5, .5, .5), glm::vec3(0, -90, 0)));
	enemies.push_back(new draw_goomba(glm::vec3(cubesize*42, cubesize, -1*cubesize), glm::vec3(.5, .5, .5), glm::vec3(0, -90, 0)));
	enemies.push_back(new draw_koopa(glm::vec3(cubesize*50, cubesize, -1*cubesize), glm::vec3(3, 3, 3), glm::vec3(0, -90, 0)));
	enemies.push_back(new draw_goomba(glm::vec3(cubesize*57, cubesize, -1*cubesize), glm::vec3(.5, .5, .5), glm::vec3(0, -90, 0)));
	enemies.push_back(new draw_goomba(glm::vec3(cubesize*97, cubesize, -2*cubesize), glm::vec3(.5, .5, .5), glm::vec3(0, -90, 0)));
	enemies.push_back(new draw_koopa(glm::vec3(cubesize*108, cubesize, -1*cubesize), glm::vec3(3, 3, 3), glm::vec3(0, -90, 0)));
	enemies.push_back(new draw_goomba(glm::vec3(cubesize*126, cubesize, -1*cubesize), glm::vec3(.5, .5, .5), glm::vec3(0, -90, 0)));
	enemies.push_back(new draw_goomba(glm::vec3(cubesize*141, cubesize, -1*cubesize), glm::vec3(.5, .5, .5), glm::vec3(0, -90, 0)));
	enemies.push_back(new draw_goomba(glm::vec3(cubesize*177, cubesize, -1*cubesize), glm::vec3(.5, .5, .5), glm::vec3(0, -90, 0)));
#ifndef LITE_MODE
	enemies.push_back(new draw_goomba(glm::vec3(cubesize*20, cubesize, 0*cubesize), glm::vec3(.5, .5, .5), glm::vec3(0, -90, 0)));
	enemies.push_back(new draw_koopa(glm::vec3(cubesize*46, cubesize, -1*cubesize), glm::vec3(3, 3, 3), glm::vec3(0, -90, 0)));
	enemies.push_back(new draw_goomba(glm::vec3(cubesize*56, cubesize, -2*cubesize), glm::vec3(.5, .5, .5), glm::vec3(0, -90, 0)));
	enemies.push_back(new draw_goomba(glm::vec3(cubesize*56, cubesize, 0*cubesize), glm::vec3(.5, .5, .5), glm::vec3(0, -90, 0)));
	enemies.push_back(new draw_koopa(glm::vec3(cubesize*87, cubesize*12, -1*cubesize), glm::vec3(3, 3, 3), glm::vec3(0, -90, 0)));
	enemies.push_back(new draw_goomba(glm::vec3(cubesize*88, cubesize*12, -1*cubesize), glm::vec3(.5, .5, .5), glm::vec3(0, -90, 0)));
	enemies.push_back(new draw_goomba(glm::vec3(cubesize*99, cubesize, 0*cubesize), glm::vec3(.5, .5, .5), glm::vec3(0, -90, 0)));
	enemies.push_back(new draw_goomba(glm::vec3(cubesize*125, cubesize, -2*cubesize), glm::vec3(.5, .5, .5), glm::vec3(0, -90, 0)));
	enemies.push_back(new draw_goomba(glm::vec3(cubesize*125, cubesize, 0*cubesize), glm::vec3(.5, .5, .5), glm::vec3(0, -90, 0)));
	enemies.push_back(new draw_goomba(glm::vec3(cubesize*127, cubesize, -2*cubesize), glm::vec3(.5, .5, .5), glm::vec3(0, -90, 0)));
	enemies.push_back(new draw_goomba(glm::vec3(cubesize*127, cubesize, 0*cubesize), glm::vec3(.5, .5, .5), glm::vec3(0, -90, 0)));
	enemies.push_back(new draw_goomba(glm::vec3(cubesize*175, cubesize, -1*cubesize), glm::vec3(.5, .5, .5), glm::vec3(0, -90, 0)));
	enemies.push_back(new draw_goomba(glm::vec3(cubesize*179, cubesize, -1*cubesize), glm::vec3(.5, .5, .5), glm::vec3(0, -90, 0)));
#endif
	// enemies

#ifdef PRINT_LOADING
	cout << "Loading Coins..." << endl;
#endif

	for(int n = 200; n < 213; n++) {
		coins.push_back(new draw_coin(glm::vec3(cubesize*n, 1.5, -(pathwidth-1)/2*cubesize), glm::vec3(.025, .025, .025), glm::vec3(90, 0, 90)));
	}
	// coins
}

int main(int argc, char* argv[]) {
	srand(time(NULL));

#ifdef PRINT_LOADING
	cout << "Loading Sounds..." << endl;
#endif
	initAudio();

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
	loading = new Cube(0.0, 0.0, 4.0, "loadscreen", 2); // inits loading screen

	border = new Cube(0.0, 0.0, 6.0, "border", 4.0f); // inits menu screen
	startbutton = new Cube(0.25, 0.2, 3.0, "start", 0.5f);
	quitbutton = new Cube(-0.25, 0.2, 3.0, "quit", 0.5f);
	settings1 = new Cube(0.0, -0.3, 3.0, "lowgrav", 0.5f);
	settings2 = new Cube(0.0, -0.3, 3.0, "normgrav", 0.5f);

#ifdef DEBUG_LEVEL
	loadDebugLevel();
#else
	loadWorld1_1();
#endif	

#ifdef __APPLE__
	CGSetLocalEventsSuppressionInterval(0.0); // deprecated, but working
#endif

#ifdef PRINT_LOADING
	cout << "Initializing GLUT..." << endl;
#endif
		
	if(initShaders()) {
		initLighting();
		glutSetCursor(GLUT_CURSOR_CROSSHAIR);
		glutDisplayFunc(onDisplay);
		glutTimerFunc(1000.0/MAX_FPS, timer, 0);
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
