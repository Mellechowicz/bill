#include <iostream>
#include "../../headers/billvector.h"


int main(){
	bill::matrixNN<double,3> A;
	bill::matrixNN<double,4> B(1.0);
	bill::matrixNN<double,2> C({{1.,0.},{0.,-1.}});
	bill::matrixNN<double,2> D({{1.,0.}});
	bill::matrixNN<double,3> E({{0.808708, 0.764771, 0.920095}, {0.46054, 0.571231, 0.167933}, {0.32033, 0.306926, 0.207253}});
	bill::vectorNd<double,2> v({1.,2.});

	std::cout<<"A: "<<std::endl<<A<<std::endl;
	std::cout<<"B: "<<std::endl<<B<<std::endl;
	std::cout<<"C: "<<std::endl<<C<<std::endl;
	std::cout<<"D: "<<std::endl<<D<<std::endl;
	std::cout<<"D: "<<std::endl<<E<<std::endl;
	std::cout<<"v: "<<v<<std::endl<<std::endl;

	std::cout<<std::endl<<"***************************************************"<<std::endl<<std::endl;

	D(0,0)=2.;
	D(1,1)=2.;
	std::cout<<"D(0,0)=2.,D(1,1)=2.: "<<std::endl<<D<<std::endl;

	std::cout<<std::endl<<"***************************************************"<<std::endl<<std::endl;

	std::cout<<"add: C+D"<<std::endl<<C+D<<std::endl;
	std::cout<<"sub: C-D"<<std::endl<<C-D<<std::endl;

	std::cout<<std::endl<<"***************************************************"<<std::endl<<std::endl;

	std::cout<<"mul: C*0.1"<<std::endl<<C*0.1<<std::endl;
	std::cout<<"mul: 0.1*C"<<std::endl<<0.1*C<<std::endl;
	std::cout<<"mul: C*v"<<std::endl<<C*v<<std::endl;
	std::cout<<"mul: v(^T)*C"<<std::endl<<v*C<<std::endl;
	std::cout<<"mul: C*D"<<std::endl<<C*D<<std::endl;

	std::cout<<std::endl<<"***************************************************"<<std::endl<<std::endl;

	std::cout<<"det(C)= "<<bill::matrixNN<double,2>::det(C)<<std::endl;
	std::cout<<"det(B)= "<<bill::matrixNN<double,4>::det(B)<<std::endl;
	std::cout<<"det(E)= "<<bill::matrixNN<double,3>::det(E)<<std::endl;

	std::cout<<std::endl<<"***************************************************"<<std::endl<<std::endl;

	std::cout<<"C^(-1)= "<<std::endl<<bill::matrixNN<double,2>::inverse(C)<<std::endl;
	std::cout<<"B^(-1)= "<<std::endl<<bill::matrixNN<double,4>::inverse(B)<<std::endl;
	std::cout<<"E^(-1)= "<<std::endl<<bill::matrixNN<double,3>::inverse(E)<<std::endl;

	return 0;
}
