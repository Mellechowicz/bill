#include "../headers/billengine.h"

bill::BillEngine::BillEngine(){
  starttime=glutGet(GLUT_ELAPSED_TIME);
  now=starttime;
  timestep=0.01;
  set=NULL;
}

bill::BillEngine::BillEngine(BillSetOfPoints& set, double timestep){ 
  starttime=glutGet(GLUT_ELAPSED_TIME);
  now=starttime;
  this->set = &set;
  this->timestep=timestep;
}

void bill::BillEngine::step(){
  if(set!=NULL){
    now=glutGet(GLUT_ELAPSED_TIME);
    if((now-starttime)>20){
      set->Go(timestep);
      starttime=now;
    }
  }
}
