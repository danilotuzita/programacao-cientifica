#include <Vector.hpp>
#include <cmath>

#define e exp(1.)

using namespace std;

typedef double(*function)(double);
typedef double(*method)(function, double, double);

// ==== FUNCTIONS ==== //
// e^x
double f1(double x) { return pow(e, x); }

// sqrt(1 - x^2)
double f2(double x) { return sqrt(1 - pow(x, 2)); }

// e^(-x^2)
double f3(double x) { return pow(e, -pow(x, 2)); }

// ==== METHODS ==== //

double rectangleMethod(function f, double a, double b) { return (b - a) * f((a + b) / 2); }

double trapezoidalRule(function f, double a, double b) { return (b - a) * ((f(a) + f(b)) / 2); }

double simpsonsRule(function f, double a, double b)
{
	double temp = f(a) + f(b) + (4 * f((a + b) / 2));
	temp /= 6;
	return (b - a) * temp;
}

// ==== GENERIC ITERATIVE METHOD ==== //
double applyMethod(method m, function f, double a, double b, int steps, bool debug = false)
{
	double ret = 0;
	for (int i = 0; i < steps; i++)
	{
		double _a = i * (b / steps);
		double _b = (i + 1) * (b / steps);
		double val = m(f, _a, _b);

		if (debug) printf("a: %lf, b: %lf: %lf\n", _a, _b, val);
		ret += val;
	}

	return ret;
}

int main()
{
	function f[] = {f1, f2, f3};
	method m[] = {rectangleMethod, trapezoidalRule, simpsonsRule};
	string name[] = { "rectangleMethod", "trapezoidalRule", "simpsonsRule" };
	string funct[] = { "e^x", "sqrt(1 - x^2)",	"e^(-x^2)" };
	int steps = 4;


	for (int i = 0; i < 3; i++)
	{
		cout << funct[i] << endl;
		for (int j = 0; j < 3; j++)
		{
			cout << name[j] << endl;
			cout << applyMethod(m[j], f[i], 0, 1, steps, true) << endl;
		}
		cout << endl;
	}

	system("PAUSE");
	return 0;
}

/*
[0 1] e^x dx
[0 1] sqrt(1-x^2) dx
[0 1] e^(-x^2) dx
*/


/*
https://www.emathhelp.net/calculators/calculus-2/midpoint-rule-calculator/?f=e%5E%28x%29&a=0&b=1&n=4&steps=on
https://www.emathhelp.net/calculators/calculus-2/trapezoidal-rule-calculator/?f=e%5E%28x%29&a=0&b=1&n=4&steps=on
https://www.emathhelp.net/calculators/calculus-2/simpsons-rule-calculator/?f=e%5E%28x%29&a=0&b=1&n=4&steps=on
*/
