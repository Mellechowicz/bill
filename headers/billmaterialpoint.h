#ifndef BILLMATERIALPOINT_H
#define BILLMATERIALPOINT_H

#include <iostream>
#include <tuple>
#include <functional>
#include <vector>
#include <memory>
#include "billvector.h"
#include "billGLfunctions.h"

namespace bill{

using BillIntegrator = std::function<std::pair<vector,vector>(std::pair<vector,vector>,std::pair<vector,vector>,vector,double)>;

class BillMaterialPoint{
protected:
	BillIntegrator algorithm;

	std::pair<vector,vector> past; // 0-> position 1-> velocity
	std::pair<vector,vector> present;
	std::pair<vector,vector> future;

	double mass;
	vector color;
	float radius;

	virtual vector Force();

	virtual void Prepare(){};
	void CalculateMove(double step);
	virtual void Act(double step){};
	void Move();
	virtual void Clear(){};

public:
	BillMaterialPoint(BillIntegrator algorithm, vector position=vector({0.,0.,0.}), vector velocity=vector({0.,0.,0.}), double mass=1.0, vector color=vector({1.0,0.0,0.0}));
//	virtual ~BillMaterialPoint();
	virtual void Draw();
	void Go(double step);

	bill::vector position();
	bill::vector x();
	bill::vector velocity();
	bill::vector v();
}; // end of class BillMaterialPoint

class BillSetOfPoints{
private:
	std::vector<std::shared_ptr<BillMaterialPoint>> points;
public:
	BillSetOfPoints(){};
	virtual	~BillSetOfPoints();

	void AddPoint(BillMaterialPoint* p);

	BillMaterialPoint* operator[](size_t n);

	void Go(double step);

	virtual void Draw();
}; // end of class BillSetOfPoints

} //end namespace bill
#endif
