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
//metoda Rungego Kutty czwartego rzędu w czasie
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
  double k;
  for (size_t i = 0; i<g1.size(); ++i){
    if((*ders)[i]->size()==1)
      k = step*wsp_przew_cieplnego*((*(*ders)[i])[0]);
    else if((*ders)[i]->size()==2)
      k = step*wsp_przew_cieplnego*((*(*ders)[i])[0] + (*(*ders)[i])[1]);
    else k = 0.;
    (*result)[i]=g2[i].get_value()+k/6.;
    g2[i] += 0.5*k;
  }

  ders = bill::BillGrid<double>::Calculate_derivatives(g2);
  for (size_t i = 0; i<g2.size(); ++i){
    if((*ders)[i]->size()==1)
      k = step*wsp_przew_cieplnego*((*(*ders)[i])[0]);
    else if((*ders)[i]->size()==2)
      k = step*wsp_przew_cieplnego*((*(*ders)[i])[0]);
    else k = 0.;
    (*result)[i]+=k/6.;
    g3[i] += 0.5*k;
  }

  ders = bill::BillGrid<double>::Calculate_derivatives(g3);
  for (size_t i = 0; i<g3.size(); ++i){
    if((*ders)[i]->size()==1)
      k = step*wsp_przew_cieplnego*((*(*ders)[i])[0]);
    else if((*ders)[i]->size()==2)
      k = step*wsp_przew_cieplnego*((*(*ders)[i])[0]);
    else k = 0.;
    (*result)[i]+=k/6.;
    g4[i] += k;
  }

  ders = bill::BillGrid<double>::Calculate_derivatives(g4);
  for (size_t i = 0; i<g2.size(); ++i){
    if((*ders)[i]->size()==1)
      k = step*wsp_przew_cieplnego*((*(*ders)[i])[0] + (*(*ders)[i])[1]);
    else if((*ders)[i]->size()==2)
      k = step*wsp_przew_cieplnego*((*(*ders)[i])[0] + (*(*ders)[i])[1]);
    else k = 0.;
    (*result)[i]+=k/6.;
  }

  return result;
};

//************************************************************************
//************************************************************************
//************************************************************************
//************************************************************************
//************************************************************************
//************************************************************************

int main(int argc, char **argv){

  bill::GLaux::eye=bill::vector({-1,0,0});
  bill::GLaux::center=bill::vector({0,0,0});

  using Tpoint  = std::shared_ptr<bill::BillPoint<double>>;
  using Tpoints = std::vector<Tpoint>;

  Tpoints Ps;
  double d=0.02;

  // Tworzymy punkty na siatce
  for(int i=0; i<=20; ++i)
    for(int j=0; j<=20; ++j)
      Ps.push_back(Tpoint(new bill::BillPoint<double>(0.,bill::vector({0.,j*d-10*d,i*d-10*d}))));

  // ustawiamy warunki początkowe "na szachownicy"
  Ps[21* 5+ 5]->set_value( 100.);
  Ps[21*15+ 5]->set_value(-100.);
  Ps[21* 5+15]->set_value(-100.);
  Ps[21*15+15]->set_value( 100.);
  Ps[21* 8+ 8]->set_value(-100.);
  Ps[21*12+ 8]->set_value( 100.);
  Ps[21* 8+12]->set_value( 100.);
  Ps[21*12+12]->set_value(-100.);

  // dodajemy środkowe punkty ze schematem trzypunktowym drugiej pochodnej
  // druga pochodna d2f_{i} = (1/d^2)*f_{i-1} + (-2/d^2)*f_{i} + (1/d^2)*f_{i+1}, gdzie d to odległość między punktami siatki 
  size_t der;
  for(int i=1; i<20; ++i){
    for(int j=1; j<20; ++j){
      //wzdłóż y
      der = Ps[21*j+i]->add_der();
      Ps[21*j+i]->add_point(der,1./(d*d),Ps[21*j+i-1]);
      Ps[21*j+i]->add_point(der,-2./(d*d),Ps[21*j+i]);
      Ps[21*j+i]->add_point(der,1./(d*d),Ps[21*j+i+1]);
      //wzdłóż z
      der = Ps[21*j+i]->add_der();
      Ps[21*j+i]->add_point(der,1./(d*d),Ps[21*(j-1)+i]);
      Ps[21*j+i]->add_point(der,-2./(d*d),Ps[21*j+i]);
      Ps[21*j+i]->add_point(der,1./(d*d),Ps[21*(j+1)+i]);
      Grid.AddPoint(Ps[21*j+i]);
    } 
    //brzeg wzdłóż y
    der = Ps[i]->add_der();
    Ps[i]->add_point(der,1./(d*d),Ps[i-1]);
    Ps[i]->add_point(der,-2./(d*d),Ps[i]);
    Ps[i]->add_point(der,1./(d*d),Ps[i+1]);
    Grid.AddPoint(Ps[i]);

    der = Ps[21*20+i]->add_der();
    Ps[21*20+i]->add_point(der,1./(d*d),Ps[21*20+i-1]);
    Ps[21*20+i]->add_point(der,-2./(d*d),Ps[21*20+i]);
    Ps[21*20+i]->add_point(der,1./(d*d),Ps[21*20+i+1]);
    Grid.AddPoint(Ps[21*20+i]);

    //brzeg wzdłóż z
    der = Ps[21*i]->add_der();
    Ps[21*i]->add_point(der,1./(d*d),Ps[21*(i-1)]);
    Ps[21*i]->add_point(der,-2./(d*d),Ps[21*i]);
    Ps[21*i]->add_point(der,1./(d*d),Ps[21*(i+1)]);
    Grid.AddPoint(Ps[21*i]);

    der = Ps[21*i+20]->add_der();
    Ps[21*i+20]->add_point(der,1./(d*d),Ps[21*(i-1)+20]);
    Ps[21*i+20]->add_point(der,-2./(d*d),Ps[21*i+20]);
    Ps[21*i+20]->add_point(der,1./(d*d),Ps[21*(i+1)+20]);
    Grid.AddPoint(Ps[21*i+20]);
  }

  //TODO: Skośny schemat pochodnej

  //dodajemy wierzchołki  bez pochodnej
  Ps[0]->set_value(0);
  Grid.AddConstantPoint(Ps[0]);
  Ps[20]->set_value(0);
  Grid.AddConstantPoint(Ps[20]);
  Ps[21*20]->set_value(0);
  Grid.AddConstantPoint(Ps[21*20]);
  Ps[21*20+20]->set_value(0);
  Grid.AddConstantPoint(Ps[21*20+20]);

  //dodajemy układ równań
  Grid.SetEquations(RK4);

  // wyłączamy edycję (potrzebne ze względu na pamięć)
  Grid.close_edit();

  // wrzucamy siatkę do silnika
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

