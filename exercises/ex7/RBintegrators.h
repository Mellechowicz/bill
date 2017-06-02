#include <iostream>
#include <vector>
#include <tuple>
#include "../../headers/billmaterialpoint.h"
#include "../../headers/billrigidbody.h"
#include "../../headers/billvector.h"

namespace bill{

bill::BillRBIntegrator RBHollyWood = [](std::tuple<bill::vector,bill::vector,bill::quaternion,bill::vector> PhasePoint0, std::tuple<bill::vector,bill::vector,bill::quaternion,bill::vector> PhasePointM, bill::vector Force, bill::vector torque, bill::matrix I, bill::matrix Ip, double step){


bill::vector x = std::get<0>( PhasePoint0 );
bill::vector v = std::get<1>( PhasePoint0 );
bill::quaternion q = std::get<2>( PhasePoint0 );
bill::vector w = std::get<3>( PhasePoint0 );
bill::quaternion qw (0.,w);

q=q+step*0.5*(qw*q);
x+=step*v;

return std::tuple<bill::vector,bill::vector,bill::quaternion,bill::vector>(x,v,q,w);
};

bill::BillRBIntegrator RBModEuler = [](std::tuple<bill::vector,bill::vector,bill::quaternion,bill::vector> PhasePoint0, std::tuple<bill::vector,bill::vector,bill::quaternion,bill::vector> PhasePointM, bill::vector Force, bill::vector torque, bill::matrix I, bill::matrix Ip, double step){


bill::vector x = std::get<0>( PhasePoint0 );
bill::vector v = std::get<1>( PhasePoint0 );
bill::quaternion q = std::get<2>( PhasePoint0 );
bill::vector w = std::get<3>( PhasePoint0 );

v+=step*Force;
x+=step*v;

w+=step*Ip*(torque - (w^(I*w)));
bill::quaternion qw (0.,w);
q=q+step*0.5*(qw*q);

return std::tuple<bill::vector,bill::vector,bill::quaternion,bill::vector>(x,v,q,w);
};

bill::BillRBIntegrator RBVerlet = [](std::tuple<bill::vector,bill::vector,bill::quaternion,bill::vector> PhasePoint0, std::tuple<bill::vector,bill::vector,bill::quaternion,bill::vector> PhasePointM, bill::vector Force, bill::vector torque, bill::matrix I, bill::matrix Ip, double step){


bill::vector x0 = std::get<0>( PhasePoint0 );
bill::vector xm = std::get<0>( PhasePointM );
bill::quaternion q = std::get<2>( PhasePoint0 );
bill::vector w = std::get<3>( PhasePoint0 );

bill::vector xp = 2*x0 - xm + Force*step*step;
bill::vector vp = (xp-xm)/(2.*step);

w+=step*Ip*(torque - (w^(I*w)));
bill::quaternion qw (0.,w);
q=q+step*0.5*(qw*q);

return std::tuple<bill::vector,bill::vector,bill::quaternion,bill::vector>(xp,vp,q,w);
};
} // end of namespace bill

