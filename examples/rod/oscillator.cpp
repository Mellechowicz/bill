#include "oscillator.h"

bill::vector oscillator::Force(){
  bill::vector x0 = position(); // pozycja punktu

  bill::vector ForceLeft ({0,0,0}); // siła z lewej
  bill::vector ForceRight({0,0,0}); // siła z prawej

  if(left!=NULL){
    bill::vector xl = left->position(); // pozycja punktu z lewej
    bill::vector rl = xl - x0;
    double norm_rl = bill::vector::norm(rl);
    ForceLeft = k*(rl/norm_rl)*(norm_rl - l);
  }

  if(right!=NULL){
    bill::vector xp = right->position(); // pozycja punktu z prawej
    bill::vector rp = xp - x0;
    double norm_rp = bill::vector::norm(rp);
    ForceRight = k*(rp/norm_rp)*(norm_rp - l);
  }

  return (ForceLeft+ForceRight)/mass;
}

oscillator::oscillator(bill::BillIntegrator algorithm, double k, double l, bill::vector position, bill::vector velocity, double mass, bill::vector color):bill::BillMaterialPoint(algorithm,position,velocity,mass,color){
	this->k = k;
	this->l = l;
	right=NULL;
	left=NULL;
}

void oscillator::set_right(oscillator* r){
  right=r;
}

void oscillator::set_left(oscillator* l){
  left=l;
}

void oscillator::set_position(bill::vector x){
  std::get<0>(present)=x;
}

void oscillator::Draw()
{
  bill::BillMaterialPoint::Draw(); // rysujemy punkt

  bill::vector x0 = x(); // pobieramy położenie punktu

  if(left!=NULL){
    bill::vector xm = left->x();

    // rysujemy linię
    glPushMatrix();
    glLineWidth(2.5);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    glVertex3f(x0[0],x0[1],x0[2]);
    glVertex3f(xm[0],xm[1],xm[2]);
    glEnd();
    glPopMatrix();
  }
}
