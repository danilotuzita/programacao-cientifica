#include <Vector.hpp>
#include <cmath>
#include <stdexcept>

#define e exp(1.)

using namespace std;

typedef double(*function)(double);
typedef double(*method)(function, double, double);
typedef struct { double ans, err; } answer;

// CALCULATES A FACTORIAL
unsigned int factorial(int n)
{
	if (n < 0)  throw domain_error("Cannot factor negative numbers");
	if (n > 12) throw overflow_error("Cannot fit !" + to_string(n) + " in a uint");
	return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n; 
}

// CALCULATES A BINOMIAL COEFFICIENT
double binomial(int n, int k)
{
	unsigned int ans = factorial(k) * factorial(n - k);
	return factorial(n) / ans;
}

// DERIVATES AT ANY DEGREE A FUNCTION ref: https://en.wikipedia.org/wiki/Numerical_differentiation#Higher-order_methods
double derivative(function fnc, double x, int degree=1, double dt = 0.000001, bool debug=false)
{
	if (degree  < 0)  throw domain_error("Cannot derivate to negative degree");
	if (degree == 0) return fnc(x);
	if (degree == 1) return (fnc(x) - fnc(x - dt)) / dt;

	double ans = 0;
	for (int k = 0; k < degree; k++)
	{
		double it = pow(-1, k + degree) * binomial(degree, k) * fnc(x + (k*dt));
		ans += it;
		if (debug) printf("k: %d | it: %lf | ans: %lf\n", k, it, ans);
	}
		
	return ans * (1 / pow(dt, degree));
}

// ==== FUNCTIONS ==== //
// e^x
double f1(double x) { return pow(e, x); }

// sqrt(1 - x^2)
double f2(double x) { return sqrt(1 - pow(x, 2)); }

// e^(-x^2)
double f3(double x) { return pow(e, -pow(x, 2)); }

// ==== METHODS ==== //

// RECTANGLE METHOD
double rectangleMethod(function f, double a, double b) { return (b - a) * f((a + b) / 2); }
double rectangleMethodError(function f, double a, double b) { return (pow((b - a), 3) / 24) * derivative(f, ((b - a) / 2), 2, 0.1); };

// TRAPEZOIDAL METHOD
double trapezoidalRule(function f, double a, double b) { return (b - a) * ((f(a) + f(b)) / 2); }
double trapezoidalRuleError(function f, double a, double b) { return (pow((b - a), 3) / 36) * derivative(f, ((b - a) / 2), 2, 0.1); };

// SIMPSON'S METHOD
double simpsonsRule(function f, double a, double b)
{
	double temp = f(a) + f(b) + (4 * f((a + b) / 2));
	temp /= 6;
	return (b - a) * temp;
}
double simpsonsRuleError(function f, double a, double b) { return -(pow(b - a, 5) / 2880) * derivative(f, (b - a) / 2, 4, 0.1); }


// ==== GENERIC ITERATIVE METHOD ==== //
answer applyMethod(method m, method errm, function f, double a, double b, int steps, bool debug=false)
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

	return { ret, errm(f, a, b) };
}

void main2()
{
	//cout << derivative(f2, 3, 2) << endl;
	//cout << binomial(7, 2) << endl;
	printf("i: %d | %lf\n\n", 0, derivative(f1, .1, 0));
	printf("i: %d | %lf\n\n", 1, derivative(f1, .1, 1));
	printf("i: %d | %lf\n\n", 2, derivative(f1, .1, 2, 0.1));
	printf("i: %d | %lf\n\n", 3, derivative(f1, .1, 3, 1));
	system("PAUSE");
	//cout << f4(10);
}

int main()
{
	//main2();
	function f[] = {f1, f2, f3};
	method m[]  = {rectangleMethod     , trapezoidalRule     , simpsonsRule};
	method me[] = {rectangleMethodError, trapezoidalRuleError, simpsonsRuleError};
	string name[] = {
		" RECTANGLE METHOD ",
		"TRAPEZOIDAL METHOD",
		" SIMPSON'S METHOD "
	};
	string funct[] = { "e^x", "sqrt(1 - x^2)",	"e^(-x^2)" };
	int steps = 4;

	for (int i = 0; i < 3; i++)
	{
		cout << "=== " << funct[i] << " ===" << endl;
		for (int j = 0; j < 3; j++)
		{
			cout << " ++ " << name[j] << " ++ (steps: " << steps << ")" << endl;
			answer a = applyMethod(m[j], me[j], f[i], 0, 1, steps, false);
			printf("f(x) = %lf\nerro = %lf\n\n", a.ans, a.err);
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
