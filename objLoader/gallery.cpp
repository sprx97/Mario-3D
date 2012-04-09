#include "Mesh.h"
#include <iostream>
//#include "drawableObj.h"
//#include "coin.h"
//#include "pipe.h"
//#include "flag.h"
//#include "draw_mushroom.h"
//#include "draw_star.h"
#include <string>
#include <stdlib.h>

#ifdef __linux__
#include <GL/gl.h>
#include <GL/glu.h>	    
#include <GL/glut.h>
//#include <GL/glew.h>
#endif
#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#include "draw_flower.h"
#include "../common/shader_utils.h"
//#define p

/*using namespace std;

Coin mycoin;
Coin mycoin1;
Coin mycoin2;

Pipe mypipe;
Pipe mypipe1;
Pipe mypipe2;
Flag myflag;

draw_mushroom mush(2, 1, 2, 1, 1, 1, 12, 12, 12);
draw_star star(8, 3, 2, .5, .5, .5, 12, 12, 12);*/
draw_flower flower(12, 3, 2, 1, 1, 1, 0, 90, 0);
//const string filename = "star_smooth.obj";  

//#ifndef p
//Mesh mesh(mPoint(9, 1, 2), mPoint(.5, .5, .5), 
//mPoint(20, 20, 20), Material(mPoint(.8,.8,0), mPoint(.8,.8,0), mPoint(.8,.8,0)));
//#endif
//Mesh *mesh;

void init() {

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
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_AUTO_NORMAL);
  glEnable(GL_NORMALIZE);
  glClearDepth(1.0);				
  glDepthFunc(GL_LESS);                       
  glEnable(GL_DEPTH_TEST); 
    

  //mesh.loadOBJ(filename);   
//  mush.load();
 // star.load();
  flower.load();
  
    
}

void display(){
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 /* 
  mycoin.setDimensions(2, .5);
  mycoin.setPos(2.5, 13, 2);
  mycoin.render();
  mycoin.draw();  

  mypipe.setPos(10, 13, 2);
  mypipe.render();
  mypipe.draw();  

  myflag.setPos(5, 1, 2);
  myflag.setDimensions(1, 2, .15, 10, .3); 
  myflag.render();
  myflag.draw();

  
  mush.draw();
  star.draw();*/
  flower.draw();

  glFlush();
 
}

void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if (w <= h)
    glOrtho(0.0, 16.0, 0.0, 16.0*(GLfloat)h/(GLfloat)w,
	    -10.0, 10.0);
  else
    glOrtho(0.0, 16.0*(GLfloat)w/(GLfloat)h, 0.0, 16.0,
	    -10.0, 10.0);

  glMatrixMode(GL_MODELVIEW);


}

int main(int argc, char **argv) {
  
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(700, 700);
  glutInitWindowPosition(50,50);
  glutCreateWindow(argv[0]);
  init();
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutIdleFunc(display);
  //glutKeyboardFunc (keyboard);
  //mesh = new Mesh(mPoint(1, 1, 2), mPoint(1, 1, 1), 
  //	  mPoint(20, 20, 20), Material("red_dots.tga"));
  glutMainLoop();
  return 0;
  
  
  
}



