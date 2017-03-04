#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <cmath>
#include "../../headers/billmaterialpoint.h"
#include "../../headers/billvector.h"

class oscillator : public bill::BillMaterialPoint{
private:
  double k; // współczynnik sprężystości
  double l; // odległość spoczynkowa

  virtual bill::vector Force();

  oscillator* left; // wsk. do lewego
  oscillator* right; // wsk. do prawego

public:
  oscillator(bill::BillIntegrator algorithm, double k, double l, bill::vector position=bill::vector({0.,0.,0.}), bill::vector velocity=bill::vector({0.,0.,0.}), double mass=1.0, bill::vector color=bill::vector({1.0,0.0,0.0}));

  void set_right(oscillator* r); // ustawianie wsk. do prawego
  void set_left(oscillator* l); // ustawianie wsk. do lewego
  void set_position(bill::vector x); // ustawianie pozycji

  virtual void Draw(); // rysowanie punktu i linii
}; //end of class oscillator
#endif
