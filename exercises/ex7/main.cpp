#include <iostream>
#include <functional>
#include "../../headers/billwindow.h"
#include "../../headers/billGLfunctions.h"
#include "../../headers/billmaterialpoint.h"
#include "../../headers/billrigidbody.h"
#include "../../headers/billRBengine.h"
#include "box.h"
#include "RBintegrators.h"

void renderScene(void);
void mainLoop(void);

class Boxes : public bill::BillSetOfRigidBodies {
public:
  Boxes():bill::BillSetOfRigidBodies(){}

  virtual void Act(){
    for(unsigned int i = 0; i<bodies.size(); ++i)
      for(unsigned int j = 0; j<i; ++j)
        if(bill::vector::norm(bodies[i]->future_position() - bodies[j]->future_position())< bodies[i]->BS()+bodies[j]->BS()){
          bodies[i]->disable();
          bodies[j]->disable();
        }

  };

  virtual void Draw(){
    bill::BillSetOfRigidBodies::Draw();

    glColor4f(0.28627451, 0.15686275, 0.12156863,0.8);
    glBegin(GL_QUADS);
    glVertex3f(-5.0f, -0.05-0.2, -5.0f);
    glVertex3f(-5.0f, -0.05-0.2,  5.0f);
    glVertex3f( 5.0f, -0.05,  5.0f);
    glVertex3f( 5.0f, -0.05, -5.0f);
    glEnd();
    glPopMatrix();
  }

} SetOfBodies;

bill::BillRBEngine engine;

int main(int argc, char **argv){

  bill::GLaux::eye=bill::vector({-1,0,0});
  bill::GLaux::center=bill::vector({0,0,0});

  SetOfBodies.AddBody(new box(bill::RBVerlet,0.2,0.2,0.2,bill::vector({0.,0.0,0.3}),bill::vector({0.,0.0,0}),bill::quaternion({0.,0.,1.,0.}),bill::vector({0.,0.,0.})));
  SetOfBodies.AddBody(new box(bill::RBVerlet,0.1,0.1,0.1,bill::vector({0.,-0.05,-0.3}),bill::vector({0.0,0.0,0.02}),bill::quaternion({0.,0.,1.,0.}),bill::vector({0.,0.,0.})));

  engine = bill::BillRBEngine(SetOfBodies);

  bill::Window window(argc,argv);
  window.set_processNormalKeys(bill::GLaux::processNormalKeys);
  window.set_processNormalKeysUp(bill::GLaux::processNormalKeysUp);
  window.set_renderScene(renderScene);
  window.set_mainLoop(mainLoop);

  window.initiate();
  return 0;
}

void mainLoop(void){
  engine.step();
  renderScene();
}


void renderScene(void) {
  if(bill::GLaux::moveParallel|bill::GLaux::movePerpendicular|bill::GLaux::rotateParallel|bill::GLaux::rotatePerpendicular)
    bill::GLaux::computePos();
 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0,800.0/600.0,0.1,1000000000.0);
  glMatrixMode(GL_MODELVIEW);
  
  
  glLoadIdentity();
  gluLookAt(bill::GLaux::eye[0],	bill::GLaux::eye[1],		bill::GLaux::eye[2],
	    bill::GLaux::center[0],	bill::GLaux::center[1],		bill::GLaux::center[2],
	    bill::GLaux::versor_up[0],	bill::GLaux::versor_up[1],	bill::GLaux::versor_up[2]);


  SetOfBodies.Draw();
  
  glutSwapBuffers();
}

