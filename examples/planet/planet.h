#ifndef PLANET_H
#define PLANET_H

#include <cmath>
#include "../../headers/billmaterialpoint.h"
#include "../../headers/billvector.h"

class planet : public bill::BillMaterialPoint{
protected:
  double M;
  bill::vector Sun;

  virtual bill::vector Force();

public:
  planet(bill::BillIntegrator algorithm, double M, bill::vector position=bill::vector({0.,0.,0.}),  bill::vector velocity=bill::vector({0.,0.,0.}), bill::vector Sun=bill::vector({0.,0.,0.}), double mass=1.0, bill::vector color=bill::vector({1.0,0.0,0.0}));
}; //end of class oscillator
#endif
