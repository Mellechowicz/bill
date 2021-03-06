#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <cmath>
#include <memory>
#include "../../headers/billmaterialpoint.h"
#include "../../headers/billvector.h"

class oscillator : public bill::BillMaterialPoint{
private:
  double k; // współczynnik sprężystości
  double kp;// współczynnik sprężystości 2go sąsiada
  double l; // odległość spoczynkowa

  virtual bill::vector Force();

  std::shared_ptr<oscillator> left; // wsk. do lewego
  std::shared_ptr<oscillator> right; // wsk. do prawego
  std::shared_ptr<oscillator> second_left; // wsk. do 2go lewego
  std::shared_ptr<oscillator> second_right; // wsk. do 2go prawego

public:
  oscillator(bill::BillIntegrator algorithm, double k, double l, bill::vector position=bill::vector({0.,0.,0.}), bill::vector velocity=bill::vector({0.,0.,0.}), double mass=1.0, bill::vector color=bill::vector({1.0,0.0,0.0}), double step=0.2);
  virtual ~oscillator(){};
  void set_right(std::shared_ptr<oscillator> r); // ustawianie wsk. do prawego
  void set_left(std::shared_ptr<oscillator> l); // ustawianie wsk. do lewego
  void set_2nd_right(std::shared_ptr<oscillator> r); // ustawianie wsk. do prawego
  void set_2nd_left(std::shared_ptr<oscillator> l); // ustawianie wsk. do lewego

  virtual void Draw(); // rysowanie punktu i linii
}; //end of class oscillator
#endif
