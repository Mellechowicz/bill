#include "planet.h"

bill::vector planet::Force(){
  bill::vector Planet = std::get<0>(present);

  return M*mass*((Sun - Planet)/pow(bill::vector::norm(Sun - Planet),3));
}

planet::planet(bill::BillIntegrator algorithm, double M, bill::vector position, bill::vector velocity, bill::vector Sun, double mass, bill::vector color):bill::BillMaterialPoint(algorithm,position,velocity,mass,color){
	this->M = M;
	this->Sun = Sun;
}
