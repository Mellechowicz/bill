#include "oscillator.h"

bill::vector oscillator::Force(){
  return -k*std::get<0>(present)/mass;
}

oscillator::oscillator(bill::BillIntegrator algorithm, double k, bill::vector position, bill::vector velocity, double mass, bill::vector color):bill::BillMaterialPoint(algorithm,position,velocity,mass,color){
	this->k = k;
}
