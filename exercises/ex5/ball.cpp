#include "ball.h"

bill::vector ball::Force(){
//  return bill::vector({0.,0.,0.});                       // nie grawitacja
//  return mass*g*bill::vector({0.,-1.,0.})-2*beta*v();    //  izograwitacja
  return mass*g*((-x())/pow(bill::vector::norm(x()),3)); //     grawitacja
//  return mass*g*((x())/pow(bill::vector::norm(x()),3));  // antygrawitacja
}

ball::ball(bill::BillIntegrator algorithm, double g, double beta, bill::vector position, bill::vector velocity, double mass, bill::vector color, double step):bill::BillMaterialPoint(algorithm,position,velocity,mass,color,step){
	this->g=g;
	this->beta=beta;
}

void ball::Draw()
{
  bill::BillMaterialPoint::Draw(); // rysujemy punkt

  bill::vector v  = this->v();      // pobieramy prędkość
  double length = bill::vector::norm(v);

  if(length<radius)  return;

  bill::vector x0 = this->x();          // pobieramy położenie punktu

  bill::GLaux::drawVector(v,x0);
}
