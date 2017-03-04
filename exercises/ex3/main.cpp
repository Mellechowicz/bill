#include <iostream>
#include <functional>
#include "../../headers/billwindow.h"
#include "../../headers/billGLfunctions.h"
#include "../../headers/billmaterialpoint.h"
#include "../../headers/billengine.h"
#include "oscillator.h"

void renderScene(void);
void mainLoop(void);

bill::BillIntegrator HollyWood = [](std::pair<bill::vector,bill::vector> PhasePoint0, std::pair<bill::vector,bill::vector> PhasePointM, bill::vector Force, double step){

bill::vector x = std::get<0>( PhasePoint0 );
bill::vector v = std::get<1>( PhasePoint0 );

v+=step*Force;
x+=step*v;

return std::pair<bill::vector,bill::vector>(x,v);

};

bill::BillSetOfPoints SetOfPoints;

bill::BillEngine engine;

int main(int argc, char **argv){

  bill::GLaux::eye=bill::vector({-1,0,0});
  bill::GLaux::center=bill::vector({0,0,0});

  oscillator pointA(HollyWood,0.1,bill::vector({0.0,0.3,0.0}));
  SetOfPoints.AddPoint(&pointA);
  engine = bill::BillEngine(SetOfPoints,0.05);

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

