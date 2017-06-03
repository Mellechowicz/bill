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

double wsp_res=0.;
double mu = 0.05;
double poziom=-0.05;
bill::vector normalna_poziomu({0.,1.,0.});

class Boxes : public bill::BillSetOfRigidBodies {
public:
  Boxes():bill::BillSetOfRigidBodies(){}

  virtual void Act(){

    // kolizja dwóch prostopadłościanów
    for(unsigned int i = 0; i<bodies.size(); ++i)
      for(unsigned int j = 0; j<i; ++j)
        if(bill::vector::norm(bodies[i]->future_position() - bodies[j]->future_position())< bodies[i]->BS()+bodies[j]->BS()){
          bodies[i]->disable();
          bodies[j]->disable();
        }

    //kolizja z podłożem
    for(unsigned int i = 0; i<bodies.size(); ++i){
	auto x = bodies[i]->future_position();
	if((x[1] - poziom) <= bodies[i]->BS() + 0.01){ // zderzenie BS z podłożem
	 double cast_min = x*normalna_poziomu - static_cast<box*>(bodies[i].get())->project(normalna_poziomu); //najniższy punkt prostopadłościanu rzucony na normalną

//std::cout<<static_cast<box*>(bodies[i].get())->project(normalna_poziomu)<<std::endl;

	 if(cast_min <= poziom){

//	   std::cout<<cast_min<<std::endl;
	   auto r = static_cast<box*>(bodies[i].get())->support(-normalna_poziomu); // punkt zderzenia (we współrzędnych lokalnych)
	   auto u = bodies[i]->velocity(); // prędkość
	   auto w = bodies[i]->angular(); // prędkość kątowa
	   auto v = u + (w^r); // prędkość w punkcie zderzenia

	   auto vr = (v*normalna_poziomu)*normalna_poziomu;	// składowa równoległa do normalna_poziomu
	   auto vp = v - vr;					// składowa prostopadła do normalna_poziomu 

	   double impuls = -(1.+wsp_res)*(normalna_poziomu*v)/(normalna_poziomu*(normalna_poziomu/bodies[i]->m() + ((bodies[i]->get_inverse_moment_of_inertia()*(r^normalna_poziomu))^r)));

	   auto new_u = u + normalna_poziomu*impuls/bodies[i]->m();
	   auto new_w = w + bodies[i]->get_inverse_moment_of_inertia()*(r^(impuls*normalna_poziomu));

	   if(bill::vector::norm(vp)>1e-15){ // jeżeli poruszamy się prostopadle do płaszczyzny zderzenia pojawia się impuls tarcia (tarcie przy zderzeniu)
	     auto t = -vp.versor(); // kierunek tarcia
//		std::cout<<r<<"x"<<t<<"= "<<(r^t)<<std::endl;
	     double impuls_t = mu*impuls;
	     new_u += t*impuls_t/bodies[i]->m();
	     new_w += bodies[i]->get_inverse_moment_of_inertia()*(r^(impuls_t*t));
	   }

	   bodies[i]->set_velocity(new_u);
	   bodies[i]->set_angular(new_w);

	   auto F = bodies[i]->Force();
	   auto M = bodies[i]->Torque();

	   if(F*normalna_poziomu<0.){
	     auto Fr = (F*normalna_poziomu)*normalna_poziomu;	// składowa równoległa do normalna_poziomu
	     auto Fp = F - Fr;					// składowa prostopadła do normalna_poziomu
	     F = Fp;
	     M+=r^(-Fr); // moment od siły reakcji

	     if(bill::vector::norm(vp)>1e-15){
		F+=-mu*bill::vector::norm(Fr)*vp.versor();	// siła tarcia
		M+=r^(-mu*bill::vector::norm(Fr)*vp.versor());  // moment tarcia
	     }
	   }

	   bodies[i]->CalculateMove(bill::RBModEuler,F,M);
	 }
	}
    }

  }

  virtual void Draw(){
    bill::BillSetOfRigidBodies::Draw();

    glColor4f(0.28627451, 0.15686275, 0.12156863,0.8);
    glBegin(GL_QUADS);
    glVertex3f(-5.0f, poziom-0.2, -5.0f);
    glVertex3f(-5.0f, poziom-0.2,  5.0f);
    glVertex3f( 5.0f, poziom,  5.0f);
    glVertex3f( 5.0f, poziom, -5.0f);
    glEnd();
    glPopMatrix();
  }

} SetOfBodies;

bill::BillRBEngine engine;

int main(int argc, char **argv){

  bill::GLaux::eye=bill::vector({-1,0,0});
  bill::GLaux::center=bill::vector({0,0,0});

  SetOfBodies.AddBody(new box(bill::RBVerlet,0.2,0.2,0.2,bill::vector({0.,0.3,0.3}),bill::vector({0.,0.0,0}),bill::quaternion(bill::vector({1.,0.,0.}),0.01+M_PI/6.),bill::vector({0.,0.,0.})));
  SetOfBodies.AddBody(new box(bill::RBVerlet,0.1,0.1,0.1,bill::vector({0.,0.3,-0.3}),bill::vector({0.0,0.0,0.1}),bill::quaternion(bill::vector({1.,0.,0.}),M_PI/6.),bill::vector({0.,0.,0.})));

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

