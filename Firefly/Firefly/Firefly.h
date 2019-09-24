#pragma once
#include <stdio.h>
#include <cmath>
#include <stdexcept>
#include "Vector.hpp"

typedef double(*function)(void*);

inline double getRandom(double a, double b) { return ((double)rand() / (RAND_MAX)) * (b - a) + a; }

class Firefly
{
private:
	int dim;
	Vector<double>* values;
	double brightness;
public:
	Firefly(int=3);
	~Firefly();

	void generate(double=0, double=1);
	void evaluate(function);
	double getBrightness() const;

	int dimension() const;
	double get(int) const;
	void set(int, double);
	void print();
	void copy(const Firefly*);
};
