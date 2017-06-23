#ifndef BILLGRID_H
#define BILLGRID_H

#include <iostream>
#include <unordered_set>
#include <map>
#include <cmath>
#include <memory>
#include <tuple>
#include <utility>
#include <algorithm>
#include "billvector.h"
#include "billGLfunctions.h"

namespace bill{

// function combining hashes: from http://www.boost.org/doc/libs/1_35_0/doc/html/boost/hash_combine_id241013.html
template <class T>
inline void hash_combine(std::size_t & seed, const T & v)
{
  std::hash<T> hasher;
  seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template <typename T>
struct BillPointSetHash{
  std::size_t operator()(const std::pair<size_t,std::shared_ptr<T>>& p)const{
    std::size_t seed = 0;
    hash_combine(seed, std::get<0>(p));
    hash_combine(seed, std::get<1>(p));
    return seed;
  }
};

template <typename T>
struct BillPointSetEq{
  bool operator()(const std::pair<size_t,std::shared_ptr<T>>& p1, const std::pair<size_t,std::shared_ptr<T>>& p2) const{
    return std::get<0>(p1) == std::get<0>(p2) &&
           std::get<1>(p1) == std::get<1>(p2);
  }
};

//*********************************************************************
//*********************************************************************
//*********************************************************************
//*********************************************************************
//*********************************************************************
template<typename Tvalue> //Tvalue must have: Tvalue(), Tvalue(0), operator+=, operator* for double, ability to use <<
class BillPoint{
public:
  using myset=std::unordered_set<std::pair<size_t,std::shared_ptr<BillPoint>>,BillPointSetHash<BillPoint>,BillPointSetEq<BillPoint>>;
  using myvec=std::vector<myset>;
  using mymap=std::vector<double>;
protected:
  // point position
  // położenie punktu
  bill::vector position;

  // value of function
  // wartość funkcji
  Tvalue value;

  // vector of unordered sets of pairs: (index of coeff, pointer to neighbor)
  // wektor nieuporządkowanych zbiorów par: (indeks współczynnika, wskaźnik do sąsiada)
  myvec derivatives;

  // map: index of coeff -> coeff
  // mapa: indeks współczynnika -> współczynnik
  mymap coeffs;
  size_t num_of_coeffs;

public:
  BillPoint(Tvalue v=Tvalue(0), bill::vector x=bill::vector()){
    value = v;
    position = x;
    num_of_coeffs=0;
  };

  virtual ~BillPoint(){};

  // calculate nth derivative
  // wylicz n-tą pochodną
  Tvalue der(size_t n)const {
    if(n<0 || n>=derivatives.size()){
      std::cerr<<"bill::BillPoint::der: attempted access to non-existent derivative"<<std::endl;
      return Tvalue();
    }
    Tvalue result=Tvalue(0);
    for(const auto& point : derivatives[n]){
      result+=coeffs[point.first]*point.second->get_value();
    }
    return result;
  }

  // setters
  void set_position(const bill::vector& p){position=p;}
  void set_value(const Tvalue& v){value=v;}

  // getters
  bill::vector get_position() const{return position;}
  Tvalue get_value() const{return value;}
  size_t num_of_ders() const{return derivatives.size();}

  // create new derivative
  // stwórz nową pochodną
  size_t add_der(){ derivatives.push_back(myset()); return derivatives.size()-1; }
  size_t add_der(myset& s){ derivatives.push_back(s); return derivatives.size()-1; }

  // add point to nth derivative
  // dodaj punkt do n-tej pochodnej
  void add_point(size_t n, double coeff, std::shared_ptr<BillPoint> p){
    if(n<0 || n>=derivatives.size()){
      std::cerr<<"bill::BillPoint::add_point: attempted access to non-existent derivative"<<std::endl;
      return;
    }
   auto it = std::find_if(coeffs.begin(),coeffs.end(),[&](double c){auto eq=fabs(c-coeff); return (eq<1e-5);});
   if(it!=coeffs.end()){
     derivatives[n].insert(std::make_pair(it-coeffs.begin(),p));
   }
   else{
     derivatives[n].insert(std::make_pair(num_of_coeffs,p));
     coeffs.push_back(coeff);
     num_of_coeffs=coeffs.size();
   }
  }
  void add_point(size_t n, double coeff, BillPoint* p)const {

  }
  void add_point(size_t n, double coeff, BillPoint& p)const {

  }

  // access to set and map through operators () and []
  myset operator()(size_t n)const {
    if(n<0 || n>=derivatives.size()){
      std::cerr<<"bill::BillPoint::operator(): attempted access to non-existent derivative"<<std::endl;
      return myset();
    }
    return derivatives[n];
  }
  myset& operator()(size_t n){
    if(n<0 || n>=derivatives.size()){
      std::cerr<<"bill::BillPoint::operator(): attempted access to non-existent derivative"<<std::endl;
      return *(new myset);
    }
    return derivatives[n];
  }
  Tvalue operator[](size_t idx) const {
    if(idx<0 || idx>=coeffs.size())  return Tvalue(0);
    return coeffs[idx]; 
  }
  Tvalue& operator[](size_t idx){
    if(idx<0)  return *(new Tvalue);
    auto N = coeffs.size();
    if(idx>=N) coeffs.insert(coeffs.end(),idx-N+1,0.);
    return coeffs[idx];
  }

  // adding and subtracting value to point
  void operator+=(const Tvalue& right){
    auto v = this->get_value();
    this->set_value(v+right);
  }

  BillPoint<Tvalue> operator+(const Tvalue& right) const{
    auto result = *this;
    auto v = this->get_value();
    result.set_value(v+right);
    return result;
  }

  friend BillPoint<Tvalue> operator+(const Tvalue& left, const BillPoint<Tvalue>& right){
    return right+left;
  }

  BillPoint<Tvalue> operator-(const Tvalue& right) const{
    auto result = *this;
    auto v = this->get_value();
    result.set_value(v-right);
    return result;
  }


  // <<
  friend std::ostream& operator<<(std::ostream& stream, const BillPoint& point){
    stream<<"Point at "<<point.get_position()<<" with value "<<point.get_value()<<", with derivatives:"<<std::endl;
    for(size_t i=0; i<point.num_of_ders(); ++i){
      stream<<"\t\tDerivative No "<<i<<": "<<std::endl;
      if(point(i).size()==0)  stream<<"\t\t\tundefined!"<<std::endl;
      for(const auto& leg : point(i)){
        stream<<"\t\t\t"<<point[std::get<0>(leg)]<<" * F("<<std::get<1>(leg)->get_position()<<")"<<std::endl;
      }
      stream<<"\t\tValue: "<<point.der(i)<<std::endl;
    }
    return stream;
  }


}; //end of class BillPoint

template<typename Tvalue, class grid>
using BillPDEIntegrator = std::function<std::vector<Tvalue>* ( grid*,      void*,          double)>;
//                              set of equations depending on   grid, parameters,       time step
//                              układ równań zależny od       siatki, parametrów, kroku czasowego
template<typename Tvalue>
class BillGrid{
public:
  using PointGrid=std::vector<std::shared_ptr<BillPoint<Tvalue>>>;
  using Integrator=BillPDEIntegrator<Tvalue,BillGrid>;
// set of equations depending on   grid, parameters, time step
// układ równań zależny od       siatki, parametrów, kroku czasowego

protected:
  PointGrid grid;
  PointGrid constants;

  std::vector<Tvalue> new_values;
  Integrator F;
  void* params;
  double step;

  bool CAN_RUN;

  // here one can prepare some data needed for time step in Act()
  // tu można przygotować dane potrzebne do kroku czasowego w Act()
  virtual void Prepare(){};

  // here one can act before new data will be assign
  // tu można zadziałać zanim nowe dane zostaną przypisane
  virtual void Act(){}; 

  // here one can clean up data prepared in Prepare()
  // tu można posprzątać dane przygotowane w Prepare()
  virtual void Clear(){};

public:
  BillGrid(double step=0.0001, void* params=NULL){
    CAN_RUN=false;
    this->step=step;
    this->params=params;
  };
  BillGrid(PointGrid& g, double step=0.0001, void* params=NULL){
    CAN_RUN=false;
    grid=g;
    this->step=step;
    this->params=params;
  }
  BillGrid(Integrator& f, double step=0.0001, void* params=NULL){
    CAN_RUN=false;
    F=f;
    this->step=step;
    this->params=params;
  }
  BillGrid(Integrator& f, PointGrid& g, double step=0.0001, void* params=NULL){
    CAN_RUN=false;
    grid=g;
    F=f;
    this->step=step;
    this->params=params;
  }
  virtual ~BillGrid(){};

  size_t size(){return grid.size();}

  // adding simulation grid points
  // dodawanie symulowanych punktów siatki
  void AddPoint(std::shared_ptr<BillPoint<Tvalue>> p){
    if(!CAN_RUN)
      grid.push_back(p);
    else
      std::cerr<<"bill::BillGrid::AddPoint: attempted edit of the grid with closed edit!"<<std::endl<<"Use .open_edit() before this line."<<std::endl;
  }
  void AddPoint(BillPoint<Tvalue>* p){
    if(!CAN_RUN)
      grid.push_back(std::shared_ptr<BillPoint<Tvalue>>(*p));
    else
      std::cerr<<"bill::BillGrid::AddPoint: attempted edit of the grid with closed edit!"<<std::endl<<"Use .open_edit() before this line."<<std::endl;
  }
  void AddPoint(BillPoint<Tvalue>& p){
    if(!CAN_RUN)
      grid.push_back(std::shared_ptr<BillPoint<Tvalue>>(p));
    else
      std::cerr<<"bill::BillGrid::AddPoint: attempted edit of the grid with closed edit!"<<std::endl<<"Use .open_edit() before this line."<<std::endl;
  }

  // adding constant grid points
  // dodawanie stałych punktów siatki
  void AddConstantPoint(std::shared_ptr<BillPoint<Tvalue>> p){
    if(!CAN_RUN)
       constants.push_back(p);
    else
      std::cerr<<"bill::BillGrid::AddConstantPoint: attempted edit of the grid with closed edit!"<<std::endl<<"Use .open_edit() before this line."<<std::endl;
  }
  void AddConstantPoint(BillPoint<Tvalue>* p){
    if(!CAN_RUN)
       constants.push_back(std::shared_ptr<BillPoint<Tvalue>>(*p));
    else
      std::cerr<<"bill::BillGrid::AddConstantPoint: attempted edit of the grid with closed edit!"<<std::endl<<"Use .open_edit() before this line."<<std::endl;
  }
  void AddConstantPoint(BillPoint<Tvalue>& p){
    if(!CAN_RUN)
      constants.push_back(std::shared_ptr<BillPoint<Tvalue>>(p));
    else
      std::cerr<<"bill::BillGrid::AddConstantPoint: attempted edit of the grid with closed edit!"<<std::endl<<"Use .open_edit() before this line."<<std::endl;
  }

  void SetEquations(Integrator f){
    if(!CAN_RUN)
      F=f;
    else
      std::cerr<<"bill::BillGrid::SetEquations: attempted edit of the integrator with closed edit!"<<std::endl<<"Use .open_edit() before this line."<<std::endl;
  }

  template<typename T>
  void SetParams(T& p){
    if(!CAN_RUN)
      params = static_cast<void*>(&p);
    else
      std::cerr<<"bill::BillGrid::SetParams: attempted edit of the parameters with closed edit!"<<std::endl<<"Use .open_edit() before this line."<<std::endl;
  }

  BillPoint<Tvalue>& operator[](size_t n){
    if(n<0 || n>=grid.size()){
      std::cerr<<"bill::BillGrid::operator[]: attempted access to non-existent point!"<<std::endl;
      return *(new BillPoint<Tvalue>);
    }
    if(CAN_RUN){
      std::cerr<<"bill::BillGrid::operator[]: attempted access to the point with closed edit!"<<std::endl<<"Use .open_edit() before this line."<<std::endl;
      return *(new BillPoint<Tvalue>);
    }
    return *grid[n];
  }

  // allows editing grid
  // włącza edytowanie siatki
  void open_edit(){
    CAN_RUN=false;
  }

  // editing grid is now not allowed: neccessary to run simulation
  // wyłącza edytowanie siatki: konieczne do uruchomienia symulacji
  void close_edit(){
    new_values=std::vector<Tvalue>(grid.size(),Tvalue(0));
    CAN_RUN=true;
  }

  // calculating all derivatives
  // wyliczamy wszystkie pochodne
  static std::vector<std::vector<Tvalue>*>* Calculate_derivatives(BillGrid<Tvalue>& g){
    auto ders = new std::vector<std::vector<Tvalue>*>(g.size());
    for(size_t i=0; i<g.size(); ++i){
      (*ders)[i] = new std::vector<Tvalue>(g[i].num_of_ders());
      for(size_t j=0; j<g[i].num_of_ders(); ++j){
        (*(*ders)[i])[j] = g[i].der(j);
      }
    }
    return ders;
  }
  static std::vector<std::vector<Tvalue>*>* Calculate_derivatives(BillGrid<Tvalue>* g){
    auto ders = new std::vector<std::vector<Tvalue>*>(g->size());
    for(size_t i=0; i<g->size(); ++i){
      (*ders)[i] = new std::vector<Tvalue>((*g)[i].num_of_ders());
      for(size_t j=0; j<(*g)[i].num_of_ders(); ++j){
        (*(*ders)[i])[j] = (*g)[i].der(j);
      }
    }
    return ders;
  }


  void Go(){
    if(CAN_RUN){
      Prepare();
  
      new_values= *(F(this,params,step));

      Act();

      for(size_t i=0; i<grid.size(); ++i)
        grid[i]->set_value(new_values[i]);

      Clear();
    }
    else
      std::cerr<<"bill::BillGrid::Go: attempted to run simulation with open edit!"<<std::endl<<"Use .close_edit() before this line."<<std::endl;
  };

  virtual void Draw(){

  };

}; //end of class BillGrid

template<>
void BillGrid<double>::Draw(){
  for(const auto& point : grid){
    auto v = point->get_value();
    if(v<0.)
      bill::GLaux::drawBall(point->get_position(),bill::vector({0,0,-v}),0.01);
    else
      bill::GLaux::drawBall(point->get_position(),bill::vector({v,0,0}),0.01);
  }
  for(const auto& point : constants){
    auto v = point->get_value();
    if(v<0.)
      bill::GLaux::drawBall(point->get_position(),bill::vector({0,0,-v}),0.01);
    else
      bill::GLaux::drawBall(point->get_position(),bill::vector({v,0,0}),0.01);
  }
}
} //end of namespace bill
#endif

