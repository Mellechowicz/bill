#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <iostream>
#include <vector>
#include <cmath>

#include "../../headers/billvector.h"


float colorR=0.0;
float colorB=0.0;
float colorG=0.0;

int resX = 800;
int resY = 600;

unsigned int starttime = glutGet(GLUT_ELAPSED_TIME);

bill::vector eye({1.0,1.0,0.0});
bill::vector center({0.0,1.0,0.0});
bill::vector versor_up({0.0,1.0,0.0});

// angle of rotation for the camera direction
float phi = 0.0f;
float theta = 0.0f;

// actual vector representing the camera's direction
float lx=0.0f,ly=0.0,lz=-1.0f;

// XZ position of the camera
float x=0.0f, z=5.0f;
float dx=0.0f, dy=0.0f;

// the key states. These variables will be zero
//when no key is being presses
int xOrigin = -1;
int yOrigin = -1;

int moveParallel=0, movePerpendicular=0;
int rotateParallel=0, rotatePerpendicular=0;

double dPhi = 0.01;
bill::quaternion q1(versor_up,dPhi);
bill::quaternion q2(((center - eye).versor())^versor_up,dPhi);

float dws=0.01f, dad=0.01f;

bool IS_CLICKED = false;
int mouseTolerance=0;

void computePos() {
  
  //std::cout<<"center-eye: "<<center-eye<<" "<<bill::vector::norm(center-eye)<<" up: "<<versor_up<<" "<<bill::vector::norm(versor_up)<<std::endl;
  
  if(moveParallel|movePerpendicular){
  bill::vector deltaEyeCen = (center - eye).versor();
  if(moveParallel){
    eye += deltaEyeCen*(moveParallel*dws);
    center += deltaEyeCen*(moveParallel*dws);
  }
  if(movePerpendicular){
    bill::vector perp=deltaEyeCen^versor_up;
    eye+= perp*(movePerpendicular*dad);
    center+= perp*(movePerpendicular*dad);
  }
  }
  
 if(rotatePerpendicular|rotateParallel){ 
  if(rotatePerpendicular)
    center = eye + (q2).rotate(center-eye);//(rotatePerpendicular>0 ? (*q2).rotate(center-eye) : (q2).rotate(center-eye));
   
  if(rotateParallel)
    center = eye + (q1).rotate(center-eye);//(rotateParallel>0 ? (*q1).rotate(center-eye) : (q1).rotate(center-eye));
  
  q2.updateU(((center - eye).versor())^versor_up);
}
  
}

void drawSnowMan() {
  
  glColor3f(1.0f, 1.0f, 1.0f);
  
  // Draw Body
  glTranslatef(0.0f ,0.75f, 0.0f);
  glutSolidSphere(0.75f,20,20);
  
  // Draw Head
  glTranslatef(0.0f, 1.0f, 0.0f);
  glutSolidSphere(0.25f,20,20);
  
  // Draw Eyes
  glPushMatrix();
  glColor3f(0.0f,0.0f,0.0f);
  glTranslatef(0.05f, 0.10f, 0.18f);
  glutSolidSphere(0.05f,10,10);
  glTranslatef(-0.1f, 0.0f, 0.0f);
  glutSolidSphere(0.05f,10,10);
  glPopMatrix();
  
  // Draw Nose
  glColor3f(1.0f, 0.5f , 0.5f);
  glRotatef(0.0f,1.0f, 0.0f, 0.0f);
  glutSolidCone(0.08f,0.5f,10,2);
}

void renderScene(void) {
  
  if(moveParallel|movePerpendicular|rotateParallel|rotatePerpendicular)
    computePos();
  
  
  // Clear Color and Depth Buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glMatrixMode(GL_PROJECTION);
  
  // Reset Matrix
  glLoadIdentity();
  
  // Set the correct perspective.
  gluPerspective(45.0,800.0/600.0,0.1,1000000000.0);
  
  // Get Back to the Modelview
  glMatrixMode(GL_MODELVIEW);
  
  
  glLoadIdentity();
  gluLookAt(eye[0],	eye[1],		eye[2],
	    center[0],	center[1],	center[2],
	    versor_up[0],	versor_up[1],	versor_up[2]);
  
  glColor3f(0.9f, 0.9f, 0.9f);
  glBegin(GL_QUADS);
  glVertex3f(-100.0f, 0.0f, -100.0f);
  glVertex3f(-100.0f, 0.0f,  100.0f);
  glVertex3f( 100.0f, 0.0f,  100.0f);
  glVertex3f( 100.0f, 0.0f, -100.0f);
  glEnd();
  
  // Draw 36 SnowMen
  
  for(int i = -3; i < 3; i++)
    for(int j=-3; j < 3; j++) {
      glPushMatrix();
      glTranslatef(i*10.0,0,j * 10.0);
      drawSnowMan();
      glPopMatrix();
    }
    
    
    glColor3f(1.0,colorG,0.0);
  glutSolidSphere(0.05f,10,10);
  glTranslatef(0.0,0.05,0.0);
  glColor3f(colorR,1.0,0.0);
  glutSolidSphere(0.05f,10,10);
  glTranslatef(0.0,0.05,0.0);
  glColor3f(colorR,colorG,1.0);
  glutSolidSphere(0.05f,10,10);
  
  colorR+=0.01;
  colorG+=0.01;
  colorB+=0.01;
  
  glutSwapBuffers();
}

void timelimited(void){
  unsigned int currenttime = glutGet(GLUT_ELAPSED_TIME);
  
  if((currenttime-starttime)>41){ //24.4 fps
    //eye=std::vector<double>({10*sin(currenttime/100000.0),10*cos(currenttime/100000.0),0.0});
    renderScene();
    starttime=currenttime;
  }
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
  
  // 	gluLookAt(eye[0],	eye[1],		eye[2],
  // 		  center[0],	center[1],	center[2],
  // 		  versor_up[0],	versor_up[1],	versor_up[2]);
  
  // Set the viewport to be the entire window
  glViewport(0, 0, w, h);
  
  // Set the correct perspective.
  gluPerspective(0,ratio,0.1,10000.0);
  
  // Get Back to the Modelview
  glMatrixMode(GL_MODELVIEW);
}

void processNormalKeys(unsigned char key, int x, int y) {
  
  switch (key) {
    case 'x' : exit(0); break;
    case 'w' : 
      moveParallel=1;  
      break;
    case 's' : 
      moveParallel=-1;
      break;
    case 'a' : 
      movePerpendicular=-1;
      break;
    case 'd' : 
      movePerpendicular=1;
      break;
  }
}

void processNormalKeysUp(unsigned char key, int x, int y) {
  
  switch (key) {
    case 'w' : 
      moveParallel=0; 
      break;
    case 's' : 
      moveParallel=0;
      break;
    case 'a' : 
      movePerpendicular=0;
      break;
    case 'd' : 
      movePerpendicular=0;
      break;
  }
}

void pressKey(int key, int xx, int yy) {
  
  switch (key) {
    case GLUT_KEY_UP : break;
    case GLUT_KEY_DOWN : break;
  }
} 

void releaseKey(int key, int x, int y) { 	
  
  switch (key) {
    case GLUT_KEY_UP : break;
    case GLUT_KEY_DOWN : break;
  }
} 
double FDdistribution(double e,double mu,double beta){
  double exponent=exp((e-mu)*beta);
  return exponent/(exponent+1.0);
}

void mouseMove(int x, int y) { 	
  
  double drPa, drPe = 0.0;
  
 drPa = x>resX/2 ? FDdistribution(x-resX*0.5,resX*0.25,0.0331131) : -FDdistribution(resX*0.5-x,resX*0.25,0.0331131);
 drPe = y>resY/2 ? FDdistribution(y-resY*0.5,resY*0.25,0.0331131) : -FDdistribution(resY*0.5-y,resY*0.25,0.0331131);
 
 std::cout<<x<<" "<<drPa<<" "<<y<<" "<<drPe<<std::endl;
 
 if(fabs(drPa)>0.1){
 rotateParallel = 1 ;
 q1.updateU(-0.01*drPa);
 }
 else rotateParallel = 0;
 
 if(fabs(drPe)>0.1){
 rotatePerpendicular = 1 ;
 q2.updateU(0.01*drPe);
 }
 else rotatePerpendicular = 0;

 //std::cout<<q1<<" "<<q2<<std::endl;
 
}
void mouseButton(int button, int state, int x, int y) {
  // only start motion if the left button is pressed
  if (button == GLUT_LEFT_BUTTON) {
    
    // when the button is released
    if (state == GLUT_UP) {
      IS_CLICKED = false;
      rotateParallel=rotatePerpendicular=0;
    }
    else  {// state = GLUT_DOWN
      IS_CLICKED = true;
      xOrigin=x;
      yOrigin=y;
    }
  }
}

int main(int argc, char **argv) {
  
//q1 = bill::quaternion(versor_up,dPhi);
//q2 = bill::quaternion(((center - eye).versor())^versor_up,dPhi);
  //versor_up.normalize();
  // init GLUT and create window
  
  glutInit(&argc,argv);
  glutInitWindowPosition(2500,500);
  glutInitWindowSize(resX,resY);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("test");
  
  // register callbacks
  glutDisplayFunc(renderScene);
  
  
  // Here is our new entry in the main function
  glutReshapeFunc(changeSize);
  
  // Animation
  glutIdleFunc(renderScene);
  
  // key processing
  glutIgnoreKeyRepeat(1);
  glutKeyboardFunc(processNormalKeys);
  glutKeyboardUpFunc(processNormalKeysUp);
  glutSpecialFunc(pressKey);
  glutSpecialUpFunc(releaseKey);
  
  // here are the two new functions
  glutMouseFunc(mouseButton);
  glutMotionFunc(mouseMove);
  glutPassiveMotionFunc(mouseMove);
  
  // OpenGL init
  glEnable(GL_DEPTH_TEST);
  
  // enter GLUT event processing cycle
  glutMainLoop();
  
  return 0;
  
}
