#include <iostream>
#include "../../headers/billvector.h"


int main(){
	bill::vectorNd<double,3> tst1;
	bill::vectorNd<double,4> tst2(1.0);
	bill::vectorNd<double,5> tst3({1.0,2.0,3.0,4.0,5.0});
	bill::vector		tst5a({1.0,2.0,3.0});
	bill::vector		tst5b({2.0,3.0,4.0});

	std::cout<<tst1<<std::endl;
	std::cout<<tst2<<std::endl;
	std::cout<<tst3<<std::endl;
	std::cout<<tst5a<<" + "<<tst5b<<" = "<<tst5a + tst5b<<std::endl;
	std::cout<<tst5a<<" - "<<tst5b<<" = "<<tst5a - tst5b<<std::endl;
	std::cout<<tst5a<<" / 2.0 = "<<tst5a/2.0<<std::endl;
	std::cout<<tst5a<<".n = "<<tst5a.versor()<<std::endl;
	
	bill::vectori tst6({1,2,3});

	std::cout<<"|"<<tst6<<"|^2 = "<<bill::vectori::square_form(tst6)<<std::endl;
	
	
	bill::vectori tst7({1,2,3});
	bill::vectorf tst8({1.f,2.f,3.f});
	bill::vectorl tst9({1L,2L,3L});
	bill::vectorll tst10({1LL,2LL,3LL});

	
	
	
	bill::vectorNd<double,10> tst11({1.0,2.0,3.0,4.0,5.0});


	bill::vector a = 1.;
	std::cout<<a<<std::endl;
	bill::vector b = {1.,2.,3.};
	std::cout<<b<<std::endl;


	return 0;
}
