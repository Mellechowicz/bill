#include <iostream>
#include <functional>
#include <vector>
#include <memory>
#include "../../headers/billwindow.h"
#include "../../headers/billGLfunctions.h"
#include "../../headers/billmaterialpoint.h"
#include "../../headers/billengine.h"
#include "../../headers/billintegrators.h"
#include "oscillator.h"

void renderScene(void);
void mainLoop(void);

bill::BillSetOfPoints SetOfPoints;

bill::BillEngine engine;

int main(int argc, char **argv){

  bill::GLaux::eye=bill::vector({-5,0,0});
  bill::GLaux::center=bill::vector({0,0,0});

  std::vector<std::shared_ptr<oscillator>> O;
  double step = 0.1; // integration step

  //Tworzymy Oscylatory Sprzężone
  for(int i=-5; i<=5; ++i){
    O.push_back(std::shared_ptr<oscillator>(new oscillator(bill::Verlet,10,0.3,bill::vector({0.0,0.0,0.3*i}),bill::vector({0.0,0.0,0.0}),1.0,bill::vector({1.0,0.2*i,-0.2*i}),step)));
    SetOfPoints.AddPoint(O.back());
  }

  //Tworzymy sprężynki do prawego
  for(int i=0; i<10; ++i){
    O[i]->set_right(O[i+1]);
  }
  //Tworzymy sprężynki do lewego
  for(int i=1; i<11; ++i){
    O[i]->set_left(O[i-1]);
  }

  //Tworzymy sprężynki do 2go prawego
  for(int i=0; i<9; ++i){
    O[i]->set_2nd_right(O[i+2]);
  }
  //Tworzymy sprężynki do 2go lewego
  for(int i=2; i<11; ++i){
    O[i]->set_2nd_left(O[i-2]);
  }

  //Wychylamy środkowy punkt
  O[5]->set_position(bill::vector({0.0,0.3,0.0}));
  O[5]->set_velocity(bill::vector({0.0,0.0,0.0}));
  engine = bill::BillEngine(SetOfPoints);

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


  SetOfPoints.Draw();
  
  glutSwapBuffers();
}

