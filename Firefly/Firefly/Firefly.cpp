#include "Firefly.h"

Firefly::Firefly(int dimension)
{
	values = new Vector<double>(dimension, 0.0);
	dim = dimension;
	brightness = 0.0;
}

Firefly::~Firefly()
{
	delete values;
}

void Firefly::generate(double a, double b)
{
	for (int i = 0; i < dim; i++)
		(*values)[i] = getRandom(a, b);
}

void Firefly::evaluate(function fnc)
{
	brightness = fnc(this);
}

double Firefly::getBrightness() const
{
	return brightness;
}

int Firefly::dimension() const
{
	return dim;
}

double Firefly::get(int index) const
{
	if (index > dim || index < 0)
		throw std::out_of_range("Index does not Exist");
	return (*values)[index];
}

void Firefly::set(int index, double value)
{
	if (index > dim || index < 0)
		throw std::out_of_range("Index does not Exist");
	(*values)[index] = value;
}

void Firefly::print()
{
	printf("[ ");
	for (int i = 0; i < dim; i++)
		printf("%8lf ", get(i));
	printf("]\n");
}

void Firefly::copy(const Firefly* other)
{
	delete values;
	values = new Vector<double>(dim, 0.0);
	dim = other->dimension();
	for (int i = 0; i < dim; i++)
		set(i, other->get(i));
	brightness = other->getBrightness();
}
//24.438950
//38.412434