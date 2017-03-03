#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <cmath>
#include "../../headers/billmaterialpoint.h"
#include "../../headers/billvector.h"

class oscillator : public bill::BillMaterialPoint{
private:
  double k;

  virtual bill::vector Force();

public:
  oscillator(bill::BillIntegrator algorithm, double k, bill::vector position=bill::vector({0.,0.,0.}), bill::vector velocity=bill::vector({0.,0.,0.}), double mass=1.0, bill::vector color=bill::vector({1.0,0.0,0.0}));
}; //end of class oscillator
#endif
