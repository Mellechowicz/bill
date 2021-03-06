#include "../headers/billrigidbody.h"
#include "../headers/billvector.h"

bill::vector bill::BillRigidBody::Force(){
  return bill::vector({0.,0.,0.});
}	
bill::vector bill::BillRigidBody::Torque(){
  return bill::vector({0.,0.,0.});
}	

void bill::BillRigidBody::CalculateMove(bill::vector& force, bill::vector& torque){
  future = algorithm(present,past,force,torque,moment_of_inertia,inverse_moment_of_inertia,step);
}

void bill::BillRigidBody::CalculateMove(bill::BillRBIntegrator algorithm, bill::vector& force, bill::vector& torque){
  future = algorithm(present,past,force,torque,moment_of_inertia,inverse_moment_of_inertia,step);
}

void bill::BillRigidBody::Move(){
  past = present;
  present = future;	
}

bill::BillRigidBody::BillRigidBody(bill::BillRBIntegrator algorithm, bill::vector position, bill::vector velocity, matrix moment_of_inertia, quaternion rotation, vector angular, double mass, bill::vector color, double step){
  this->algorithm=algorithm;
  this->step=step;

  past    = std::tuple<bill::vector,bill::vector,bill::quaternion,bill::vector>(position-velocity*step,{0,0,0},rotation,angular);
  present = std::tuple<bill::vector,bill::vector,bill::quaternion,bill::vector>(position,velocity,rotation,angular);
  future  = std::tuple<bill::vector,bill::vector,bill::quaternion,bill::vector>({0,0,0},{0,0,0},rotation,angular);

  this->moment_of_inertia=moment_of_inertia;
  this->inverse_moment_of_inertia = bill::matrix::inverse(moment_of_inertia);

  this->mass = mass;
  this->color = color;
  radius = 0.1;
  ENABLE_SIMULATION=true;
}

bill::BillRigidBody::~BillRigidBody(){}

double bill::BillRigidBody::BS(){
	return radius;
}

void bill::BillRigidBody::Draw(){
  bill::GLaux::drawBall(std::get<0>(present),color,radius);
}

bill::vector bill::BillRigidBody::xm(){
  return std::get<0>(past);
}
bill::vector bill::BillRigidBody::vm(){
  return std::get<1>(past);
}
bill::vector bill::BillRigidBody::past_position(){
  return xm();
}
bill::vector bill::BillRigidBody::past_velocity(){
  return vm();
}
bill::vector bill::BillRigidBody::x(){
  return std::get<0>(present);
}
bill::vector bill::BillRigidBody::v(){
  return std::get<1>(present);
}
bill::vector bill::BillRigidBody::position(){
  return x();
}
bill::vector bill::BillRigidBody::velocity(){
  return v();
}
bill::vector bill::BillRigidBody::xp(){
  return std::get<0>(future);
}
bill::vector bill::BillRigidBody::vp(){
  return std::get<1>(future);
}
bill::vector bill::BillRigidBody::future_position(){
  return xp();
}
bill::vector bill::BillRigidBody::future_velocity(){
  return vp();
}
bill::quaternion bill::BillRigidBody::qm(){
  return std::get<2>(past);
}
bill::vector bill::BillRigidBody::wm(){
  return std::get<3>(past);
}
bill::quaternion bill::BillRigidBody::past_quaternion(){
  return qm();
}
bill::vector bill::BillRigidBody::past_angular(){
  return wm();
}
bill::quaternion bill::BillRigidBody::q(){
  return std::get<2>(present);
}
bill::vector bill::BillRigidBody::w(){
  return std::get<3>(present);
}
bill::quaternion bill::BillRigidBody::present_quaternion(){
  return q();
}
bill::vector bill::BillRigidBody::angular(){
  return w();
}
bill::quaternion bill::BillRigidBody::qp(){
  return std::get<2>(future);
}
bill::vector bill::BillRigidBody::wp(){
  return std::get<3>(future);
}
bill::quaternion bill::BillRigidBody::future_quaternion(){
  return qp();
}
bill::vector bill::BillRigidBody::future_angular(){
  return wp();
}

double bill::BillRigidBody::m(){
  return mass;
}

void bill::BillRigidBody::set_position(bill::vector new_x){
  x(new_x);
}
void bill::BillRigidBody::x(bill::vector new_x){
  std::get<0>(present)=new_x;
  past = std::tuple<bill::vector,bill::vector,bill::quaternion,bill::vector>(std::get<0>(present)-std::get<1>(present)*step,std::get<1>(present),std::get<2>(present),std::get<3>(present));
}
void bill::BillRigidBody::set_velocity(bill::vector new_v){
  v(new_v);
}
void bill::BillRigidBody::v(bill::vector new_v){
  std::get<1>(present)=new_v;
  past = std::tuple<bill::vector,bill::vector,bill::quaternion,bill::vector>(std::get<0>(present)-std::get<1>(present)*step,std::get<1>(present),std::get<2>(present),std::get<3>(present));
}
void bill::BillRigidBody::set_future_position(bill::vector new_x){
  xp(new_x);
}
void bill::BillRigidBody::xp(bill::vector new_x){
  std::get<0>(future)=new_x;
}
void bill::BillRigidBody::set_future_velocity(bill::vector new_v){
  vp(new_v);
}
void bill::BillRigidBody::vp(bill::vector new_v){
  std::get<1>(future)=new_v;
}

void bill::BillRigidBody::set_quaternion(bill::quaternion new_q){
  q(new_q);
}
void bill::BillRigidBody::q(bill::quaternion new_q){
  std::get<2>(present)=new_q;
}
void bill::BillRigidBody::set_angular(bill::vector new_w){
  w(new_w);
}
void bill::BillRigidBody::w(bill::vector new_w){
  std::get<3>(present)=new_w;
}
void bill::BillRigidBody::set_future_quaternion(bill::quaternion new_q){
  qp(new_q);
}
void bill::BillRigidBody::qp(bill::quaternion new_q){
  std::get<2>(future)=new_q;
}
void bill::BillRigidBody::set_future_angular(bill::vector new_w){
  wp(new_w);
}
void bill::BillRigidBody::wp(bill::vector new_w){
  std::get<3>(future)=new_w;
}

void bill::BillRigidBody::I(bill::matrix new_I){
  moment_of_inertia=new_I;
  inverse_moment_of_inertia=bill::matrix::inverse(new_I);
}
void bill::BillRigidBody::set_moment_of_inertia(bill::matrix new_I){
  I(new_I);
}

bill::matrix bill::BillRigidBody::get_moment_of_inertia(){
  return moment_of_inertia;
}

bill::matrix bill::BillRigidBody::get_inverse_moment_of_inertia(){
  return inverse_moment_of_inertia;
}

void bill::BillRigidBody::disable(){
  ENABLE_SIMULATION=false;
}
void bill::BillRigidBody::enable(){
  ENABLE_SIMULATION=true;
}
bool bill::BillRigidBody::should_run(){
  return ENABLE_SIMULATION;
}
//*************************************************************************

void bill::BillSetOfRigidBodies::AddBody(std::shared_ptr<BillRigidBody> p){
	  bodies.push_back(p);
}

void bill::BillSetOfRigidBodies::AddBody(BillRigidBody* p){
	  bodies.push_back(std::shared_ptr<BillRigidBody>(p));
}

std::shared_ptr<bill::BillRigidBody>  bill::BillSetOfRigidBodies::operator[](size_t n){
  if(n >= bodies.size()){
    std::cerr<<("bill::BillSetOfbodies::operator[]: there is no field to access!\n");
    exit(-1);
  }
  else
    return bodies[n]; 
}

void bill::BillSetOfRigidBodies::Go(){
  Prepare();

  for(auto& body : bodies)
    if(body->should_run()){
      bill::vector force = body->Force()/body->m();
      bill::vector torque = body->Torque();
      body->CalculateMove(force, torque);
    }

  Act();

  for(auto& body : bodies)
    if(body->should_run())
      body->Move();

  Clear();  
}

void bill::BillSetOfRigidBodies::ResetToCenterMass(){
  bill::vector CMx({0,0,0});
  bill::vector CMv({0,0,0});
  double CMm=0.;

  for(auto& body : bodies){
    CMx+=body->m()*body->x();
    CMv+=body->m()*body->v();
    CMm+=body->m();
  }

  CMx/=CMm;
  CMv/=CMm;

  for(auto& body : bodies){
    body->x(body->x()-CMx);
    body->v(body->v()-CMv);
  }
}

void bill::BillSetOfRigidBodies::Draw(){
  for(auto& body : bodies)
    body->Draw();	
}

bill::BillSetOfRigidBodies::~BillSetOfRigidBodies(){
  for(auto& body : bodies)
	body.reset();	
}
