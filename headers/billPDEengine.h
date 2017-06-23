#ifndef BILLPDEENGINE_H
#define BILLPDEENGINE_H

#include <iostream>
#include <cmath>
#include "billgrid.h"

namespace bill{

template<typename Tvalue>
class BillPDEEngine{
private:
  BillGrid<Tvalue>* grid;
  unsigned int starttime;
  unsigned int now;
  double timestep;

public:
  BillPDEEngine(){
    starttime=glutGet(GLUT_ELAPSED_TIME);
    now=starttime;
    grid=NULL;
  }

  BillPDEEngine(BillGrid<Tvalue>& grid){ 
    starttime=glutGet(GLUT_ELAPSED_TIME);
    now=starttime;
    this->grid= &grid;
    this->timestep=timestep;
  }

  virtual ~BillPDEEngine(){


  }
 
  void step(){
    if(grid!=NULL){
      now=glutGet(GLUT_ELAPSED_TIME);
      if((now-starttime)>20){
        grid->Go();
        starttime=now;
      }
    }
  }
}; //end of class BillPDEEngine
} //end of namespace bill
#endif
