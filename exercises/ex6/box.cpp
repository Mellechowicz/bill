#include "box.h"

box::box(bill::BillRBIntegrator algorithm, double _a, double _b, double _c, bill::vector position, bill::vector velocity, bill::quaternion rotation, bill::vector angular):bill::BillRigidBody(algorithm,position,velocity,rotation,angular){
 a = _a;
 b = _b;
 c = _c;

 colors.push_back(bill::vector({1.,0.,0.}));
 colors.push_back(bill::vector({1.,0.,0.}));
 colors.push_back(bill::vector({0.,1.,0.}));
 colors.push_back(bill::vector({0.,1.,0.}));
 colors.push_back(bill::vector({0.,0.,1.}));
 colors.push_back(bill::vector({0.,0.,1.}));

 radius = sqrt(0.25*(a*a+b*b+c*c));

}

void box::Draw(){
 bill::vector i ({0.5,0.,0.});
 bill::vector j ({0.,0.5,0.});
 bill::vector k ({0.,0.,0.5});

 bill::vector x = std::get<0>(present);
 bill::quaternion q = std::get<2>(present);

 q.rotateMe(i);
 q.rotateMe(j);
 q.rotateMe(k);

 std::vector<bill::vector> points;

 points.push_back(x-a*i+b*j+c*k);
 points.push_back(x+a*i+b*j+c*k);
 points.push_back(x+a*i+b*j-c*k);
 points.push_back(x-a*i+b*j-c*k);
 points.push_back(x-a*i-b*j+c*k);
 points.push_back(x+a*i-b*j+c*k);
 points.push_back(x+a*i-b*j-c*k);
 points.push_back(x-a*i-b*j-c*k);


  bill::GLaux::drawBox(points,colors,1.);
}
