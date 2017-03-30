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
  bill::vector x1 = x0 + v; // znajdujemy koniec wektora prędkości

  bill::vector vp1; // wektory prostopadłe do v
  bill::vector vp2;

  if(fabs(v[0]) > 1e-5){
    vp1[0]=-v[1];
    vp1[1]= v[0];
    vp1[2]= 0.;
  }
  else{
    vp1[0]= 0.;
    vp1[1]=-v[2];
    vp1[2]= v[1];
  }

  vp2 = vp1^v;

  vp1.normalize();
  vp2.normalize();

  bill::vector s1p = x1 + 0.05*length*vp1 - 0.05*v;
  bill::vector s1m = x1 - 0.05*length*vp1 - 0.05*v;
  bill::vector s2p = x1 + 0.05*length*vp2 - 0.05*v;
  bill::vector s2m = x1 - 0.05*length*vp2 - 0.05*v;


  // rysujemy prędkość
  glPushMatrix();
  glLineWidth(2.5);
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_LINES);
  glVertex3f(x0[0],x0[1],x0[2]);
  glVertex3f(x1[0],x1[1],x1[2]); // główna linia
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_TRIANGLES);
  glVertex3f(x1[0],x1[1],x1[2]);
  glVertex3f(s1p[0],s1p[1],s1p[2]);
  glVertex3f(s2p[0],s2p[1],s2p[2]);
  glVertex3f(x1[0],x1[1],x1[2]);
  glVertex3f(s2p[0],s2p[1],s2p[2]);
  glVertex3f(s1m[0],s1m[1],s1m[2]);
  glVertex3f(x1[0],x1[1],x1[2]);
  glVertex3f(s1m[0],s1m[1],s1m[2]);
  glVertex3f(s2m[0],s2m[1],s2m[2]);
  glVertex3f(x1[0],x1[1],x1[2]);
  glVertex3f(s2m[0],s2m[1],s2m[2]);
  glVertex3f(s1p[0],s1p[1],s1p[2]);
  glEnd();
  glPopMatrix();

/*  glLineWidth(1.0);
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_LINES);
  glVertex3f(x1[0],x1[1],x1[2]);
  glVertex3f(x1[0],x1[1],x1[2]); // strzałka
  glEnd();
  glPopMatrix();*/
}
