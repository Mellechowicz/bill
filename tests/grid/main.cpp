#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <utility>
#include "../../headers/billgrid.h"
#include "../../headers/billvector.h"

int main(){
	std::shared_ptr<bill::BillPoint<double>> p0(new bill::BillPoint<double>());
	std::shared_ptr<bill::BillPoint<double>> p1(new bill::BillPoint<double>());
	std::shared_ptr<bill::BillPoint<double>> p2(new bill::BillPoint<double>());
	std::shared_ptr<bill::BillPoint<double>> p3(new bill::BillPoint<double>());

	p0->set_position(bill::vector({0,0,0}));
	p0->set_value(-1.);
	p1->set_position(bill::vector({1,0,0}));
	p1->set_value(0.);
	p2->set_position(bill::vector({2,0,0}));
	p2->set_value(1.);
	p3->set_position(bill::vector({3,0,0}));
	p3->set_value(0.);

	// druga pochodna d2f_{i} = (1/d^2)*f_{i-1} + (-2/d^2)*f_{i} + (1/d^2)*f_{i+1}, gdzie d to odległość między punktami siatki 
	auto der1 = p1->add_der();
	(*p1)(der1).insert(std::make_pair(0,p0)); // definiujemy nóżki
	(*p1)(der1).insert(std::make_pair(1,p1));
	(*p1)(der1).insert(std::make_pair(0,p2));
	(*p1)[0]=1.;
	(*p1)[1]=-2.;

	// pierwsza pochodna df_{i} = (0.5/d)*f_{i+1} - (0.5/d)*f_{i+1}
	auto der2 = p2->add_der();
	p2->add_point(der2,-0.5,p1);
	p2->add_point(der2,0.5,p3);
	
	// druga pochodna d2f_{i} = (1/d^2)*f_{i-1} + (-2/d^2)*f_{i} + (1/d^2)*f_{i+1}
        auto der3 = p2->add_der();
	p2->add_point(der3,1.,p1);
	p2->add_point(der3,-2.,p2);
	p2->add_point(der3,1.,p3);

	std::cout<<*p0<<*p1<<*p2<<*p3;

        bill::BillGrid<double> grid;
        grid.AddPoint(p0);
        grid.AddPoint(p1);
        grid.AddPoint(p2);
        grid.AddPoint(p3);

	return 0;
}
