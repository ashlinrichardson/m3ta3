#ifndef __BIKES_H
#define __BIKES_H
#pragma once

#include "../SA.h"
#include "../rkf45/rkf45.h"
#include "pthread.h"
#include "../glut2/newzpr.h"
#include <cmath>

using namespace std;
static char time_string[STR_MAX];
class odeSIM;
static odeSIM * theSimulation;
float r4_epsilon();

static void renderBitmapString(float x, float y, void *font, char *string){
  char *c;
  glRasterPos2f(x,y);
  for(c = string; *c != '\0'; c++){
    glutBitmapCharacter(font, *c);
  }
}

// http://www.codeproject.com/Articles/80923/The-OpenGL-and-GLUT-A-Powerful-Graphics-Library-an
static void setOrthographicProjection(int WINDOWX, int WINDOWY){
  int h = WINDOWY;
  int w = WINDOWX;
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, w, 0, h);
  glScalef(1, -1, 1);
  glTranslatef(0, -h, 0);
  glMatrixMode(GL_MODELVIEW);
}

static void resetPerspectiveProjection(){
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}

class glBike{
  public:
  zprInstance * myZprInstance;
  glBasicSphere * mySphere;
  float * x; float * y; float * z;
  float arrow1x, arrow1y, arrow1z;
  float arrow2x, arrow2y, arrow2z;
  SA<float> * vX;
  SA<float> * vY;
  SA<float> * vZ;
  odeSIM * mySIM;
  float dWall1, dWall2, interacX, interacY;
  glBike(odeSIM * mySim,zprInstance * myZpr, float myX, float myY, float myZ, float myR, float myG, float myB, float mySize, int myCircles, int myStacks);
  void drawMe();
};

float Si(float x, float d, float e){
  // p. 10 of http://ashy.ca/2010_agueh_swarm.pdf
  if(x <= d){
    return (1.);
  }
  else if(x < d + e){
    return ( 0.5 + 0.5*tanh( (1./(x-d)) + (1./(x-(d+e))) ));
  }
  else{
    return 0.;
  }
}
float rnd(){
  return (float) rand()/((float)RAND_MAX);
}

class odeSIM: public glPlottable{
  public:
  vector<glBasicSphere *> * mySpheres;
  vector<glBike *> * myBikes;
  float rX, rY, rZ;
  zprInstance * myZpr;

  pthread_t thread;
  odeSIM(zprInstance * myZpr_);
  odeSIM * Instance(zprInstance * myZpr_);
  void myr4_f2 ( float t, float y[], float yp[] );
  void initBikes();

  float abserr;
  int flag;
  int i_step;
  int n_step;
  int neqn;
  float relerr;
  float t;
  float t_out;
  float t_start;
  float t_stop;
  float *y;
  float *yp;

  void initSpheres();
  void drawSpheres();

  void glAxes();

  void initSimulation();

  void test02( );
  void r4_f1 ( float t, float y[], float yp[] );
  float r4_y1x ( float t );
  void r8_f1 ( double t, double y[], double yp[] );
  double r8_y1x ( double t );
  void r8_f2 ( double t, double y[], double yp[] );

  void initIC();

  void rebootODE();

  void drawTime();

  void * myThreadfun(void *arg);

  void initODE();

  void drawMe();
};

void r4_f2( float t, float y[], float yp[] );

void * threadfun( void *arg);

/* Draw axes */
#define STARTX 400
#define STARTY 400

#define STARTXpos 0
#define STARTYpos 0

//ZPR
/* Mouse Manipulation API */
//#define MYFONT GLUT_BITMAP_HELVETICA_18
//#define MYFONT GLUT_BITMAP_8_BY_13
//#define MYFONT GLUT_BITMAP_9_BY_15
//#define MYFONT GLUT_BITMAP_TIMES_ROMAN_10
//#define MYFONT GLUT_BITMAP_TIMES_ROMAN_24
//#define MYFONT GLUT_BITMAP_HELVETICA_10
#define MYFONT GLUT_BITMAP_HELVETICA_12
//define MYFONT GLUT_BITMAP_HELVETICA_18

//====================================================================
// SIMULATION MODULE...................
//physical.................
#define NUMBIKES 60 //need to implement computationally efficient bike-comparison (cone, "geolocated"..sparse)
#define DRIVER_REPULSION_DISTANCE_THRESHOLD 2.5
#define DRIVER_REPULSION_DISTANCE_THRESHOLD_RADIUS 2.5

static int nBikes; //=NUMBIKES;//125;
static float myPylonInterval; // = 10.;
static int nPylons; // = 100.;
static float roadWidth; // = 20.;

// aesthetic
#define MYSTACKS 11
static float arrowFactor; // = 8.;

static int PAUSE_FLAG;
static int DISPLAY_TIME; static float t_step; static float myTime;

//=====================================================================
//graphics module...
static GLint toTrack;
#define STR_MAX 1000 // opengl code resources: http://www.opengl.org/wiki/Code_Resources
//====================================================================

static int Window1;
static int Window2;

static GLint tex1name;
static GLint tex2name;

static int mlrow, mlcol;
static float * dat;// = NULL;
static int renderflag;

static int textureflag;// =false;
static int ready;// = false;

static GLubyte * datBYTE;
static GLubyte * datTexture;

static GLuint texture;// = NULL;
static GLuint texName;// = NULL;

/* Create checkerboard texture */
#define checkImageWidth 64
#define checkImageHeight 64
#define datTextureWidth 512
#define datTextureHeight 512

static GLubyte checkImage[checkImageHeight][checkImageWidth][4];
static void makeCheckImage(void){
  int i, j, c;
  for (i = 0; i < checkImageHeight; i++) {
    for (j = 0; j < checkImageWidth; j++) {
      c = ((((i&0x8)==0)^((j&0x8))==0))*255;
      checkImage[i][j][0] = (GLubyte) c;
      checkImage[i][j][1] = (GLubyte) c;
      checkImage[i][j][2] = (GLubyte) c;
      checkImage[i][j][3] = (GLubyte) 255;
    }
  }
}

static int TEX_WIDTH;// = 2048;
static int TEX_HEIGHT;// = 2048;

static GLUquadricObj *quadratic;

static uint8_t *ui_image_copy;// = 0;
static GLfloat tex_param_s;// = 1.0f;
static GLfloat tex_param_t;// = 1.0f;
static GLdouble *tex_mat;// = 0;

static GLuint cube_list;

//from webcam example..

static int height, width, count, init;
static float rot;//fps, rot;

static GLenum format;
static GLuint imageID;

#define IsRGB(s) ((s[0] == 'R') && (s[1] == 'G') && (s[2] == 'B'))
#define IsBGR(s) ((s[0] == 'B') && (s[1] == 'G') && (s[2] == 'R'))

#ifndef GL_CLAMP_TO_BORDER
#define GL_CLAMP_TO_BORDER 0x812D
#endif
#define GL_MIRROR_CLAMP_EXT 0x8742

#endif
