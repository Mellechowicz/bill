#include "box.h"

box::box(bill::BillRBIntegrator algorithm, double _a, double _b, double _c, bill::vector position, bill::vector velocity, bill::quaternion rotation, bill::vector angular):bill::BillRigidBody(algorithm,position,velocity,bill::matrix::loadID(),rotation,angular){
 a = _a;
 b = _b;
 c = _c;

 mass=1.0;

 double mul=mass/12.;
 set_moment_of_inertia(bill::matrix({{mul*(b*b+c*c),0.,0.},{0,mul*(a*a+c*c),0.},{0.,0.,mul*(a*a+b*b)}}));

 colors.push_back(bill::vector({ 0.95686275,  0.34117647,  0.03921569}));
 colors.push_back(bill::vector({ 0.95686275,  0.34117647,  0.03921569}));
 colors.push_back(bill::vector({ 0.56470588,  0.6627451,   0.78039216}));
 colors.push_back(bill::vector({ 0.56470588,  0.6627451,   0.78039216}));
 colors.push_back(bill::vector({ 0.95294118,  0.75294118,  0.78431373}));
 colors.push_back(bill::vector({ 0.95294118,  0.75294118,  0.78431373}));

 radius = sqrt(0.25*(a*a+b*b+c*c));

 punkt_tarcia = bill::vector({0.,-.5*b,0.});
}

void box::rotatevector(bill::vector& v){
  std::get<2>(present).rotateMe(v);
}

void box::rotatedcoordinates(bill::vector& i, bill::vector& j, bill::vector& k){
 i= bill::vector({1.,0.,0.});
 j= bill::vector({0.,1.,0.});
 k= bill::vector({0.,0.,1.});

 rotatevector(i);
 rotatevector(j);
 rotatevector(k);
}

void box::vertices(std::vector<bill::vector>& vers){
 vers.clear();

 bill::vector i,j,k;
 rotatedcoordinates(i,j,k); 

 bill::vector x = std::get<0>(present);

 vers.push_back(x+0.5*(-a*i+b*j+c*k));
 vers.push_back(x+0.5*( a*i+b*j+c*k));
 vers.push_back(x+0.5*( a*i+b*j-c*k));
 vers.push_back(x+0.5*(-a*i+b*j-c*k));
 vers.push_back(x+0.5*(-a*i-b*j+c*k));
 vers.push_back(x+0.5*( a*i-b*j+c*k));
 vers.push_back(x+0.5*( a*i-b*j-c*k));
 vers.push_back(x+0.5*(-a*i-b*j-c*k));
}

void box::Draw(){
  std::vector<bill::vector> points;
  vertices(points);

//  std::cout<<support(bill::vector({0.,1.,0.}))<<std::endl; 

//  bill::GLaux::drawBall(x()+support(bill::vector({0.,1.,0.}))); //rysowanie punktu zderzenia
  bill::GLaux::drawBox(points,colors,1.);
}

double box::get_size(const unsigned int i) const{
  switch(i){
    case 0:  return a;
    case 1:  return b;
    case 2:  return c;
    default: return -1.;
  }
} //zwraca wymiar prostopadłościaniu w kierunku: 0:x, 1:y, 2:z 


bill::vector box::get_versor(const unsigned int i){
  bill::vector result({0.,0.,0.});
  result[i] = 1.;
  rotatevector(result);
  return result;
} // zwraca wersor tworzący wierzchołek: 0:x, 1:y, 2:z 


void box::get_vertices(std::vector<bill::vector>& vers){
  vertices(vers);
} // zwraca std::vector wektorów do wierzchołków

bill::vector box::Tarcie(){
  bill::vector v0 = v();
  return -0.001*v0.versor(); 
} // zwraca siłę w

bill::vector box::Force(){
  return 0.1*bill::vector({0,-1,0})-0.1*v();  
} // zwraca siłę wypadkową

bill::vector box::Torque(){
  return -0.01*w();//bill::vector({0.,0.,0.});
} // zwraca wypadkowy moment siły


double box::project(bill::vector n){
  bill::vector i = get_versor(0);
  bill::vector j = get_versor(1);
  bill::vector k = get_versor(2); // lokalne osi

  return 0.5*a*fabs(i*n) + 0.5*b*fabs(j*n) + 0.5*c*fabs(k*n);
} // zwraca najdalszy punkt w kierunku n

bill::vector box::support(bill::vector n){

  auto sgn = [](double x) { return x<0. ? -1. : 1.;};

  bill::vector i = get_versor(0);
  bill::vector j = get_versor(1);
  bill::vector k = get_versor(2); // lokalne osi

  double cast_i = i*n;
  double cast_j = j*n;
  double cast_k = k*n;

//  std::cout<<"i= "<<i<<", j= "<<j<<", k= "<<k<<std::endl;
//  std::cout<<"i*n= "<<cast_i<<", j*n= "<<cast_j<<", k*n= "<<cast_k<<std::endl;

  // jeżeli cast_k ==  1., zderzenie na ścianie z normalną  k
  // jeżeli cast_k == -1., zderzenie na ścianie z normalną -k

  if(fabs(fabs(cast_i)-1.)<1e15)
    return 0.5*sgn(cast_i)*a*i;
  if(fabs(fabs(cast_j)-1.)<1e15)
    return 0.5*sgn(cast_j)*b*j;
  if(fabs(fabs(cast_k)-1.)<1e15)
    return 0.5*sgn(cast_k)*c*k;

  // jeżeli cast_k == 0. i cast_i i cast_j != 1., zderzenie z krawędzią wzdłuż k

  if(fabs(cast_i)<1.e15)
    return 0.5*sgn(cast_j)*b*j + 0.5*sgn(cast_k)*c*k;
  if(fabs(cast_j)<1.e15)
    return 0.5*sgn(cast_k)*c*k + 0.5*sgn(cast_i)*a*i;
  if(fabs(cast_k)<1.e15)
    return 0.5*sgn(cast_i)*a*i + 0.5*sgn(cast_j)*b*j;

  // jeżeli cast_ != 0. -> zderzenie z jednym punktem

  return 0.5*sgn(cast_i)*a*i + 0.5*sgn(cast_j)*b*j + 0.5*sgn(cast_k)*c*k;
} // zwraca wszy punkt w kierunku n
