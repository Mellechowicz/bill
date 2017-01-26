#include <iostream>
#include "../../headers/billvector.h"
#include <cmath>

int main(){
	bill::quaternion q1({2.0,3.0,4.0});
	bill::quaternion q2({0.1,2.0,3.0,4.0});
	bill::quaternion q3(0.1,{1.0,0.0,0.0});
	double theta1=M_PI/3.0;
	bill::vector v1({1.0,0.0,0.0});
	double theta2=M_PI/12.0;
	bill::vector v2({0.0,1.0,0.0});
	bill::quaternion q4(theta1,v1);
	bill::quaternion q5(v1,theta1);
	bill::quaternion q6(v2,theta2);

	std::cout<<"q1= "<<q1<<std::endl;
	std::cout<<"q2= "<<q2<<std::endl;
	std::cout<<"q3= "<<q3<<std::endl;
	std::cout<<"q4= "<<q4<<std::endl;
	std::cout<<"q5= "<<q5<<std::endl;
	std::cout<<"q6= "<<q6<<std::endl;
	std::cout<<"q5+q6 = "<<q5+q6<<std::endl;
	std::cout<<"q5.q6 = "<<q5*q6<<std::endl;
	std::cout<<"sqrt(|q4|^2) = "<<bill::quaternion::norm(q4)<<std::endl;
	q4.normalize();
	std::cout<<"sqrt(|q4|^2) = "<<bill::quaternion::norm(q4)<<std::endl;
	std::cout<<"sqrt(|q5|^2) = "<<bill::quaternion::norm(q5)<<std::endl;
	std::cout<<"sqrt(|q6|^2) = "<<bill::quaternion::norm(q6)<<std::endl;
	std::cout<<"q5.*q5 = "<<q5*(*q5)<<std::endl;
	std::cout<<"q5.!q6 = "<<q5*(!q6)<<std::endl;
	
	
	std::cout<<"#####################################################"<<std::endl;
	std::cout<<"#####################################################"<<std::endl;
	std::cout<<"#####################################################"<<std::endl;
	std::cout<<std::endl;
	
	double theta = 0.01*M_PI;
	bill::vector a ({1.0,0.0,0.0});
	bill::vector axis({0.0,0.0,1.0});
	bill::quaternion q (axis,theta);
	 
	while (a[0] > -0.99 ){
	 q.rotateMe(a);
	 std::cout<<a<<std::endl;
	}

	return 0;
}
