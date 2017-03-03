#include "../headers/billmaterialpoint.h"
#include "../headers/billvector.h"

bill::vector bill::BillMaterialPoint::Force(){
  return bill::vector({0.,0.,0.});
}	

void bill::BillMaterialPoint::CalculateMove(double step){
  future = algorithm(present,Force(),step);
}

void bill::BillMaterialPoint::Move(){
  past = present;
  present = future;	
}

bill::BillMaterialPoint::BillMaterialPoint(std::function<std::pair<bill::vector,bill::vector>(std::pair<bill::vector,bill::vector>,bill::vector,double)> algorithm, bill::vector position, bill::vector velocity, double mass, bill::vector color){
  this->algorithm=algorithm;
  past    = std::pair<bill::vector,bill::vector>({0,0,0},{0,0,0});
  present = std::pair<bill::vector,bill::vector>(position,velocity);
  future  = std::pair<bill::vector,bill::vector>({0,0,0},{0,0,0});

  this->mass = mass;
  this->color = color;
  radius = 0.1;
}

void bill::BillMaterialPoint::Draw(){
  bill::GLaux::drawBall(std::get<0>(present),color,radius);
}

void bill::BillMaterialPoint::Go(double step){
  Prepare();
  CalculateMove(step);
  Act(step);
  Move();
  Clear();
}

//*************************************************************************

void bill::BillSetOfPoints::AddPoint(BillMaterialPoint* p){
	  points.push_back(p);
}

bill::BillMaterialPoint*  bill::BillSetOfPoints::operator[](size_t n){
  if(n >= points.size()){
    std::cerr<<("bill::BillSetOfPoints::operator[]: there is no field to access!\n");
    exit(-1);
  }
  else
    return points[n]; 
}

void bill::BillSetOfPoints::Go(double step){
  for(auto& point : points)
    point->Go(step);
}

void bill::BillSetOfPoints::Draw(){
  for(auto& point : points)
    point->Draw();	
}
