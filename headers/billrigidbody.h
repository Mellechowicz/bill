#ifndef BILLRIGIDBODY_H
#define BILLRIGIDBODY_H

#include <iostream>
#include <tuple>
#include <functional>
#include <vector>
#include <memory>
#include "billvector.h"
#include "billGLfunctions.h"

namespace bill{

using BillRBIntegrator = std::function<std::tuple<vector,vector,quaternion,vector>(std::tuple<vector,vector,quaternion,vector>,std::tuple<vector,vector,quaternion,vector>,vector,vector,matrix,matrix,double)>;

class BillRigidBody{
protected:
	BillRBIntegrator algorithm;

	double step;

	std::tuple<vector,vector,quaternion,vector> past; // 0-> position 1-> velocity 2-> rotation 3-> angular velocity
	std::tuple<vector,vector,quaternion,vector> present;
	std::tuple<vector,vector,quaternion,vector> future;

	double mass;
	bill::matrix moment_of_inertia;
	bill::matrix inverse_moment_of_inertia;
	vector color;
	float radius;

	bool ENABLE_SIMULATION;

public:
	BillRigidBody(BillRBIntegrator algorithm, vector position=vector({0.,0.,0.}), vector velocity=vector({0.,0.,0.}), matrix moment_of_inertia=matrix({{1.,0.,0.},{0.,1.,0.},{0.,0.,1.}}), quaternion rotation=quaternion({0.,0.,1.,0.}), vector angular=vector({0.1,0.1,0.1}), double mass=1.0, vector color=vector({1.0,0.0,0.0}), double step=0.2);
	virtual ~BillRigidBody();
	virtual void Draw();

	virtual vector Force();
	virtual vector Torque();
	void CalculateMove(bill::vector& force, bill::vector& torque);
	void CalculateMove(bill::BillRBIntegrator algorithm, bill::vector& force, bill::vector& torque);
	void Move();

	double m();

// position and velocity getters
	bill::vector past_position();
	bill::vector xm();
	bill::vector past_velocity();
	bill::vector vm();
	bill::vector position();
	bill::vector x();
	bill::vector velocity();
	bill::vector v();
	bill::vector future_position();
	bill::vector xp();
	bill::vector future_velocity();
	bill::vector vp();
	bill::quaternion past_quaternion();
	bill::quaternion qm();
	bill::vector past_angular();
	bill::vector wm();
	bill::quaternion present_quaternion();
	bill::quaternion q();
	bill::vector angular();
	bill::vector w();
	bill::quaternion future_quaternion();
	bill::quaternion qp();
	bill::vector future_angular();
	bill::vector wp();

// position and velocity setters
	void set_position(bill::vector new_x);
	void x(bill::vector new_x);
	void set_velocity(bill::vector new_v);
	void v(bill::vector new_v);
	void set_future_position(bill::vector new_x);
	void xp(bill::vector new_x);
	void set_future_velocity(bill::vector new_v);
	void vp(bill::vector new_v);
	void set_quaternion(bill::quaternion new_q);
	void q(bill::quaternion new_q);
	void set_angular(bill::vector new_w);
	void w(bill::vector new_w);
	void set_future_quaternion(bill::quaternion new_q);
	void qp(bill::quaternion new_q);
	void set_future_angular(bill::vector new_w);
	void wp(bill::vector new_w);

// get/set moment of inertia
	bill::matrix get_moment_of_inertia();
	bill::matrix get_inverse_moment_of_inertia();
	void set_moment_of_inertia(bill::matrix new_I);
	void I(bill::matrix new_I);

// disable/enable simulation
	void disable();
	void enable();
	bool should_run();

// get radius
	double BS();

}; // end of class BillRigidBody

class BillSetOfRigidBodies{
protected:
	std::vector<std::shared_ptr<BillRigidBody>> bodies;
	
	virtual void Prepare(){};
	virtual void Act(){};
	virtual void Clear(){};

public:
	BillSetOfRigidBodies(){};
	virtual	~BillSetOfRigidBodies();

	void AddBody(std::shared_ptr<BillRigidBody> p);
	void AddBody(BillRigidBody* p);

	std::shared_ptr<BillRigidBody> operator[](size_t n);

	void Go();
	void ResetToCenterMass();

	virtual void Draw();
}; // end of class BillSetOfRigidBodies

} //end namespace bill
#endif
