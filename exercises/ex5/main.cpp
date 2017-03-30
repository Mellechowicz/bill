#include <iostream>
#include <functional>
#include <vector>
#include <memory>
#include <random>
#include "../../headers/billwindow.h"
#include "../../headers/billGLfunctions.h"
#include "../../headers/billmaterialpoint.h"
#include "../../headers/billengine.h"
#include "../../headers/billintegrators.h"
#include "ball.h"

void renderScene(void);
void mainLoop(void);

class Balls: public bill::BillSetOfPoints{
public:
  Balls():bill::BillSetOfPoints(){}

  virtual void Act(){
    for(unsigned int i = 0; i<points.size(); ++i)
      for(unsigned int j = 0; j<i; ++j)
        if(bill::vector::norm(points[i]->future_position() - points[j]->future_position())< 0.2){
          points[i]->disable();
          points[j]->disable();
        }
  }

  virtual void Draw(){
    bill::BillSetOfPoints::Draw();
    bill::GLaux::drawBall(bill::vector({0,0,0}), bill::vector({1,0,0}), 2.0, 0.2);
  }
} balls;

bill::BillEngine engine;

int main(int argc, char **argv){

  bill::GLaux::eye=bill::vector({-5,0,0});
  bill::GLaux::center=bill::vector({0,0,0});

  std::vector<std::shared_ptr<ball>> O;
  double step = 0.1; // integration step

  int number_of_balls = 20;
  int color_invl = number_of_balls/4;
  if(color_invl<1) color_invl=1;

  std::vector<bill::vector> colors;
  colors.push_back(bill::vector({0.41176471, 0.82352941, 0.90588235}));
  colors.push_back(bill::vector({0.65490196, 0.85882353, 0.84705882}));
  colors.push_back(bill::vector({0.87843137, 0.89411765, 0.8       }));
  colors.push_back(bill::vector({0.95294118, 0.5254902 , 0.18823529}));
  colors.push_back(bill::vector({0.98039216, 0.41176471, 0.        }));

  std::default_random_engine gen;
  std::lognormal_distribution<double> dis_mass(-0.5,1.0);
  std::normal_distribution<double> dis_r(1.0,0.5);
  std::uniform_real_distribution<double> dis_phi(0.0,2*M_PI);
  std::uniform_real_distribution<double> dis_theta(0.0,M_PI);

  double r,phi,theta;

  //Tworzymy punkty
  for(int i=0; i<number_of_balls; ++i){
    int c1 = i/color_invl;
    int c2 = i%color_invl;

    double alpha= static_cast<double>(c2+1)/static_cast<double>(color_invl);
    bill::vector color=(1.-alpha)*colors[c1]+alpha*colors[c1+1];

    bill::vector pos;
    bill::vector vel;

    r     =     dis_r(gen);
    phi   =   dis_phi(gen);
    theta = dis_theta(gen);
    pos[0] = r*sin(theta)*cos(phi);
    pos[1] = r*sin(theta)*sin(phi);
    pos[2] = r*cos(theta);

    r     =     dis_r(gen);
    phi   =   dis_phi(gen);
    theta = dis_theta(gen);
    vel[0] = r*sin(theta)*cos(phi);
    vel[1] = r*sin(theta)*sin(phi);
    vel[2] = r*cos(theta);

    double m = dis_mass(gen);

    O.push_back(std::shared_ptr<ball>(new ball(bill::Verlet,0.5,0.,pos,vel,m,color,step)));
    balls.AddPoint(O.back());
  }

  engine = bill::BillEngine(balls);

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


  balls.Draw();
  
  glutSwapBuffers();
}

