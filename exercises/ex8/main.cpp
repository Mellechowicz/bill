#include <iostream>
#include <functional>
#include "../../headers/billwindow.h"
#include "../../headers/billGLfunctions.h"
#include "../../headers/billgrid.h"
#include "../../headers/billPDEengine.h"


// STAŁE

double krok_czasowy=0.0001;
double wsp_przew_cieplnego=0.1;


void renderScene(void);
void mainLoop(void);

bill::BillGrid<double> Grid(krok_czasowy);
bill::BillPDEEngine<double> engine;
bill::BillPDEIntegrator<double,bill::BillGrid<double>> RK4 = [](bill::BillGrid<double>* g, void* params, double step){
// metoda Rungego Kutty czwartego rzędu w czasie
  auto result = new std::vector<double>(g->size(),0.);

  auto g1=*g;
  g1.open_edit();
  auto g2=*g;
  g2.open_edit();
  auto g3=*g;
  g3.open_edit();
  auto g4=*g;
  g4.open_edit();

  auto ders = bill::BillGrid<double>::Calculate_derivatives(g1);
  for (size_t i = 0; i<g1.size(); ++i){
    auto k1 = step*wsp_przew_cieplnego*(*(*ders)[i])[0];
    (*result)[i]=g2[i].get_value()+k1/6.;
    g2[i] += 0.5*k1;
  }

  ders = bill::BillGrid<double>::Calculate_derivatives(g2);
  for (size_t i = 0; i<g2.size(); ++i){
    auto k2 = step*wsp_przew_cieplnego*(*(*ders)[i])[0];
    (*result)[i]+=k2/6.;
    g3[i] += 0.5*k2;
  }

  ders = bill::BillGrid<double>::Calculate_derivatives(g3);
  for (size_t i = 0; i<g3.size(); ++i){
    auto k3 = step*wsp_przew_cieplnego*(*(*ders)[i])[0];
    (*result)[i]+=k3/6.;
    g4[i] += k3;
  }

  ders = bill::BillGrid<double>::Calculate_derivatives(g4);
  for (size_t i = 0; i<g2.size(); ++i){
    auto k4 = step*wsp_przew_cieplnego*(*(*ders)[i])[0];
    (*result)[i]+=k4/6.;
  }

  return result;
};

int main(int argc, char **argv){

  bill::GLaux::eye=bill::vector({-1,0,0});
  bill::GLaux::center=bill::vector({0,0,0});

  using Tpoint  = std::shared_ptr<bill::BillPoint<double>>;
  using Tpoints = std::vector<Tpoint>;

  Tpoints Ps;
  double dz=0.01;
  // tworzymy punkty
  for(int i=-30; i<=30; ++i)
    Ps.push_back(Tpoint(new bill::BillPoint<double>(0.,bill::vector({0.,0.,i*dz}))));

  // dodajemy pierwszy punkt bez pochodnych
  Ps.front()->set_value(20.);
  Grid.AddConstantPoint(Ps.front());

  // dodajemy środkowe punkty ze schematem trzypunktowym drugiej pochodnej
  for(size_t i=1; i<Ps.size()-1; ++i){  // druga pochodna d2f_{i} = (1/d^2)*f_{i-1} + (-2/d^2)*f_{i} + (1/d^2)*f_{i+1}, gdzie d to odległość między punktami siatki 
    auto der = Ps[i]->add_der();
    Ps[i]->add_point(der,1./(dz*dz),Ps[i-1]);
    Ps[i]->add_point(der,-2./(dz*dz),Ps[i]);
    Ps[i]->add_point(der,1./(dz*dz),Ps[i+1]);

    Grid.AddPoint(Ps[i]);
  } 

  //dodajemy ostatni punkt bez pochodnej
  Ps.back()->set_value(-20.);
  Grid.AddConstantPoint(Ps.back());


  //dodajemy układ równań
  Grid.SetEquations(RK4);

  // wyłączamy edycję siatki (potrzebne ze względu na pamięć)
  Grid.close_edit();

  engine = bill::BillPDEEngine<double>(Grid);

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


  Grid.Draw();
  
  glutSwapBuffers();
}

