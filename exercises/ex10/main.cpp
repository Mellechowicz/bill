#include <iostream>
#include <functional>
#include "../../headers/billwindow.h"
#include "../../headers/billGLfunctions.h"
#include "../../headers/billgrid.h"
#include "../../headers/billPDEengine.h"
#include "../../headers/billvector.h"

// STAŁE

double krok_czasowy=0.001;

void renderScene(void);
void mainLoop(void);

using data = bill::vectorNd<double,2>;

bill::BillPDEEngine<data> engine;
bill::BillPDEIntegrator<data,bill::BillGrid<data>> RK2 = [](bill::BillGrid<data>* g, void* params, double step){
// metoda Rungego Kutty drugiego rzędu w czasie
  auto result = new std::vector<data>(g->size(),0.);

  auto g1=*g;
  g1.open_edit();
  auto g2=*g;
  g2.open_edit();

// Układ równań (równanie falowe) to:
//  (A-i) u_t = v
// (A-ii) v_t = u_xx

//TODO: układ równań Boussinesqa:
//  (B-i) u_t = v
// (B-ii) v_t = 3*v*u_xx + (9/2)*u_x*u_x*u_xx - 3*u_xxxx


  auto ders = bill::BillGrid<data>::Calculate_derivatives(g1);
  for (size_t i = 0; i<g1.size(); ++i){
    double v = g1[i].get_value()[0];    // bo funkcję pomocniczą przechowujemy w pierwszym polu
//  double u = g1[i].get_value()[1];    // bo funkcję u przechowujemy w drugim polu
//  double vxx = ((*(*ders)[i])[0])[0]; // ders jest wskaźnikiem na std::vector, którego i-te pole
                                        // jest wskaźnikiem na std::vector<data> pochodnych,
                                        // z których na pierwszym polu (z indeksem 0) jest pochodna druga
                                        // a znów na pierwszym polu tej pochodnej jest wartość drugiej pochodnej z funkcji pomocniczej
    double uxx = ((*(*ders)[i])[0])[1]; // ders jest wskaźnikiem na std::vector, którego i-te pole
                                        // jest wskaźnikiem na std::vector<data> pochodnych,
                                        // z których na pierwszym polu (z indeksem 0) jest pochodna druga
                                        // a znów na drugim polu tej pochodnej jest wartość drugiej pochodnej z funkcji u

    double ku = step*v;  // całkowanie równania  (A-i)
    double kv = step*uxx;// całkowanie równania (A-ii)
    auto k = data({kv,ku});
    (*result)[i]=g2[i].get_value();
    g2[i] += 0.5*k;
  }

  ders = bill::BillGrid<data>::Calculate_derivatives(g2);
  for (size_t i = 0; i<g2.size(); ++i){
    double v = g2[i].get_value()[0];    // bo funkcję pomocniczą przechowujemy w pierwszym polu
//  double u = g2[i].get_value()[1];    // bo funkcję u przechowujemy w drugim polu
//  double vxx = ((*(*ders)[i])[0])[0]; // ders jest wskaźnikiem na std::vector, którego i-te pole
                                        // jest wskaźnikiem na std::vector<data> pochodnych,
                                        // z których na pierwszym polu (z indeksem 0) jest pochodna druga
                                        // a znów na pierwszym polu tej pochodnej jest wartość drugiej pochodnej z funkcji pomocniczej
    double uxx = ((*(*ders)[i])[0])[1]; // ders jest wskaźnikiem na std::vector, którego i-te pole
                                        // jest wskaźnikiem na std::vector<data> pochodnych,
                                        // z których na pierwszym polu (z indeksem 0) jest pochodna druga
                                        // a znów na drugim polu tej pochodnej jest wartość drugiej pochodnej z funkcji u

    double ku = step*v;  // całkowanie równania  (A-i)
    double kv = step*uxx;// całkowanie równania (A-ii)
    auto k = data({kv,ku});
    (*result)[i]+=k;
  }

  return result;
};

// musimy powiedzieć klasie jak ma się rysować
class Wave : public bill::BillGrid<data>{
public:
  Wave(double& s) : bill::BillGrid<data>(s){};
  virtual void Draw(){
    for(const auto& point : grid){ 
      auto v = point->get_value();
      auto x = point->get_position();
      x[1]=v[1]; 
      bill::GLaux::drawBall(x,bill::vector({v[0],0.5,v[1]}),0.01); 
    } 
    for(const auto& point : constants){ 
      auto v = point->get_value(); 
      bill::GLaux::drawBall(point->get_position(),bill::vector({v[0],0.5,v[1]}),0.01); 
    }
  }
} Grid(krok_czasowy);

//************************************************************************************
//************************************************************************************
//************************************************************************************
//************************************************************************************
//************************************************************************************
//************************************************************************************

int main(int argc, char **argv){

  bill::GLaux::eye=bill::vector({-1,0,0});
  bill::GLaux::center=bill::vector({0,0,0});

  using Tpoint  = std::shared_ptr<bill::BillPoint<data>>;
  using Tpoints = std::vector<Tpoint>;

  Tpoints Ps;
  double dz=0.01;
  // tworzymy punkty
  for(int i=-30; i<=30; ++i){
    Ps.push_back(Tpoint(new bill::BillPoint<data>({0.,0.05*sin(i*M_PI/30)},bill::vector({0.,0.,i*dz}))));
  }

  // dodajemy pierwszy punkt bez pochodnych
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
  Grid.AddConstantPoint(Ps.back());


  //dodajemy układ równań
  Grid.SetEquations(RK2);

  // wyłączamy edycję siatki (potrzebne ze względu na pamięć)
  Grid.close_edit();

  engine = bill::BillPDEEngine<data>(Grid);

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

