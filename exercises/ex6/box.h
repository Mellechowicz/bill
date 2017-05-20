#ifndef BOX_H
#define BOX_H

#include "../../headers/billrigidbody.h"
#include "../../headers/billvector.h"

class box : public bill::BillRigidBody{
protected:
	double a,b,c; // długości boków
 	std::vector<bill::vector> colors;

public:
	box(bill::BillRBIntegrator algorithm, double _a = .2, double _b = .2, double _c = .2, bill::vector position = bill::vector({0.,0.,0.}),bill::vector velocity = bill::vector({0.,0.,0.}), bill::quaternion rotation = bill::quaternion({0.,0.,1.,0.}),bill::vector angular=bill::vector({0.2,0.2,0.}));
	virtual ~box(){};

	void Draw();

}; // end of class box

#endif //end of box.h
