#include <omp.h>
#include <cmath>
#include <string>
#include <iostream>
#include <stdexcept>

#define e exp(1.)
#define uint unsigned int

using namespace std;

typedef double(*function)(double);
typedef double(*method)(function, double, double);
typedef struct { double ans, err; } answer;

// DERIVATES A FUNCTION DEGREES(0 - 4) ref: https://en.wikipedia.org/wiki/Finite_difference_coefficient#Forward_finite_difference
double derivativeV3(function fnc, double x, int degree=1, uint accuracy=1, double dt=0.000001, bool debug=false)
{
	if (degree < 0)  throw domain_error("Cannot derivate to negative degree");
	if (degree > 4)  throw domain_error("Cannot derivate degree greater than 4");
	if (degree == 0) return fnc(x);
	if (degree == 1) return (fnc(x) - fnc(x - dt)) / dt;
	
	accuracy %= 6; // asserting accuracy is between 0 and 5
	
	double vec[4][6][9] = 
	{
		{
			{      (-1.),        (1.),          (0.),       (0.),        (0.),        (0.),         (0.),       (0.),        (0.) },
			{    (-3/2.),        (2.),       (-1/2.),       (0.),        (0.),        (0.),         (0.),       (0.),        (0.) },
			{   (-11/6.),        (3.),       (-3/2.),     (1/3.),        (0.),        (0.),         (0.),       (0.),        (0.) },
			{  (-25/12.),        (4.),         (-3.),     (4/3.),     (-1/4.),        (0.),         (0.),       (0.),        (0.) },
			{ (-137/60.),        (5.),         (-5.),    (10/3.),     (-5/4.),      (1/5.),         (0.),       (0.),        (0.) },
			{  (-49/20.),        (6.),      (-15/2.),    (20/3.),    (-15/4.),      (6/5.),      (-1/6.),       (0.),        (0.) }
		}, {
			{       (1.),       (-2.),          (1.),       (0.),        (0.),        (0.),         (0.),       (0.),        (0.) },
			{       (2.),       (-5.),          (4.),      (-1.),        (0.),        (0.),         (0.),       (0.),        (0.) },
			{   (35/12.),    (-26/3.),       (19/2.),   (-14/3.),    (11/12.),        (0.),         (0.),       (0.),        (0.) },
			{    (15/4.),    (-77/6.),      (107/6.),     (-13.),    (61/12.),     (-5/6.),         (0.),       (0.),        (0.) },
			{  (203/45.),    (-87/5.),      (117/4.),  (-254/9.),     (33/2.),    (-27/5.),   (137/180.),       (0.),        (0.) },
			{  (469/90.),  (-223/10.),     (879/20.), (-949/18.),       (41.),  (-201/10.),  (1019/180.),   (-7/10.),        (0.) }
		}, {
			{      (-1.),        (3.),         (-3.),       (1.),        (0.),        (0.),         (0.),       (0.),        (0.) },
			{    (-5/2.),        (9.),        (-12.),       (7.),     (-3/2.),        (0.),         (0.),       (0.),        (0.) },
			{   (-17/4.),     (71/4.),      (-59/2.),    (49/2.),    (-41/4.),      (7/4.),         (0.),       (0.),        (0.) },
			{   (-49/8.),       (29.),     (-461/8.),      (62.),   (-307/8.),       (13.),     (-15/8.),       (0.),        (0.) },
			{(-967/120.),   (638/15.),   (-3929/40.),   (389/3.), (-2545/24.),    (268/5.), (-1849/120.),   (29/15.),        (0.) },
			{ (-801/80.),    (349/6.), (-18353/120.), (2391/10.),  (-1457/6.),  (4891/30.),    (-561/8.),  (527/30.), (-469/240.) }
		}, {
			{       (1.),       (-4.),          (6.),      (-4.),        (1.),        (0.),         (0.),       (0.),        (0.) },
			{       (3.),      (-14.),         (26.),     (-24.),       (11.),       (-2.),         (0.),       (0.),        (0.) },
			{    (35/6.),      (-31.),      (137/2.),  (-242/3.),    (107/2.),      (-19.),      (17/6.),       (0.),        (0.) },
			{    (28/3.),   (-111/2.),        (142.), (-1219/6.),      (176.),   (-185/2.),      (82/3.),    (-7/2.),        (0.) },
			{ (1069/80.), (-1316/15.),   (15289/60.), (-2144/5.), (10993/24.), (-4772/15.),   (2803/20.), (-536/15.),  (967/240.) },
			{ (1069/80.), (-1316/15.),   (15289/60.), (-2144/5.), (10993/24.), (-4772/15.),   (2803/20.), (-536/15.),  (967/240.) }
		}
	};
	
	if (debug) printf("Derivating f(%lf) in %d degree, Accuracy of %d, dt of %lf\n", x, degree, accuracy + 1, dt);

	double ans = 0;
	#pragma omp parallel for reduction(+:ans)
	for (int i = 0; i <= 8; i++)
	{
		double smt = vec[degree - 1][accuracy][i];  // getting multiplier
		if (!smt && i) continue;                    // if multiplier is 0 and i is not 0, go to next iteration
		double it = smt * fnc(x + (dt * i));        // calculating iteration value
		ans += it;                                  // adding all iteration
		if (debug) printf("k: %2d | f(%6lf): %12lf | ans: %12lf (thread: %d)\n", i, x + (dt * i), it, ans, omp_get_thread_num());
	}

	ans /= pow(dt, degree);

	return ans;
}

// ==== FUNCTIONS ==== //
// e^x
double f1(double x) { return pow(e, x); }

// sqrt(1 - x^2)
double f2(double x) { return sqrt(1 - pow(x, 2)); }

// e^(-x^2)
double f3(double x) { return pow(e, -pow(x, 2)); }

// f2 derivatives
// (sqrt(1 - x^2))'
double f2d1(double x)
{
	return -x / sqrt(1 - (x * x));
}

// (sqrt(1 - x^2))''
double f2d2(double x)
{
	double ans = (1 - (x * x)) * sqrt(1 - (x * x));
	return -1 / ans;
}

// (sqrt(1 - x^2))'''
double f2d3(double x)
{
	return -(3 * x) / pow((1 - (x * x)), 5 / 2);
}

// (sqrt(1 - x^2))''''
double f2d4(double x)
{
	return -(3 * (4 * (x * x) + 1)) / pow((1 - (x * x)), 7 / 2);
}


// ==== METHODS ==== //

// RECTANGLE METHOD
double rectangleMethod(function f, double a, double b)      { return (b - a) * f((a + b) / 2); }
double rectangleMethodError(function f, double a, double b) { return (pow((b - a), 3) / 24) * derivativeV3(f, ((b - a) / 2), 2, 5, .00001); };

// TRAPEZOIDAL METHOD
double trapezoidalRule(function f, double a, double b)      { return (b - a) * ((f(a) + f(b)) / 2); }
double trapezoidalRuleError(function f, double a, double b) { return (pow((b - a), 3) / 36) * derivativeV3(f, ((b - a) / 2), 2, 5, .00001); };

// SIMPSON'S METHOD
double simpsonsRule(function f, double a, double b)
{
	double temp = f(a) + f(b) + (4 * f((a + b) / 2));
	temp /= 6;
	return (b - a) * temp;
}
double simpsonsRuleError(function f, double a, double b) { return -(pow(b - a, 5) / 2880) * derivativeV3(f, (b - a) / 2, 4, 5, 0.01); }


// ==== GENERIC ITERATIVE METHOD ==== //
// integrates a function handling a passed method and method error
answer applyMethod(method m, method errm, function f, double a, double b, int steps, bool debug=false)
{
	double ret = 0;
	double err = 0;
	#pragma omp parallel for reduction(+:ret,err)
	for (int i = 0; i < steps; i++)  // for each quadrant
	{
		double _a = i * (b / steps);       // calculates quandrant's a
		double _b = (i + 1) * (b / steps); // calculates quandrant's b
		double val = m(f, _a, _b);         // calculates the integral using passed method
		double cerr = errm(f, _a, _b);     // calculates the method error

		ret += val;  // adds the calculated integral to the total area
		err += cerr; // adds the calculated error to the total error

		if (debug) printf("(thread: %d)\na: %lf, b: %lf: %lf\n\terr: %lf\n", omp_get_thread_num(), _a, _b, val, cerr);
	}

	return { ret, err };
}


// ==== TEST FUNCTIONS ==== //
void main3()
{
	bool debug = false;
	double dt = 1;
	double x_0 = .5;
	int    acc = 5;
	cin >> dt;
	
	printf("f(%lf) = %lf\n\n", x_0, f1(x_0));

	printf("ans f'(%lf) = %lf\n", x_0, f2d1(x_0));
	printf(" v3 f'(%lf) = %lf\n\n", x_0, derivativeV3(f2, x_0, 1, 5, dt, debug));

	printf("ans f''(%lf) = %lf\n", x_0, f2d2(x_0));
	printf(" v3 f''(%lf) = %lf\n\n", x_0, derivativeV3(f2, x_0, 2, 5, dt, debug));

	printf("ans f'''(%lf) = %lf\n", x_0, f2d3(x_0));
	printf(" v3 f'''(%lf) = %lf\n\n", x_0, derivativeV3(f2, x_0, 3, 5, dt, debug));

	printf("ans f''''(%lf) = %lf\n", x_0, f2d4(x_0));
	printf(" v3 f''''(%lf) = %lf\n\n", x_0, derivativeV3(f2, x_0, 4, 5, dt, debug));
}

void main2()
{
	double inp = 1;
	double x_0 = .4;
	int    acc = 5;
	double val;
	for (int i = 1; i < 5; i++)
	{
		while (inp)
		{
			cin >> inp;
			printf(" **** f(x_0) = %lf\n\n", derivativeV3(f2, x_0, 0));
			
			val = derivativeV3(f2, x_0, i, acc, inp);
			printf(" *V3* f");
			for (int j = 0; j < i; j++) printf("'");
			printf("(x_0) = %lf\n\n", val);

			printf("f''(x_0) = %lf\n", f2d2(x_0));
		}
		system("CLS");
		inp = 1;
	}
		
	
	system("PAUSE");
}

// ==== MAIN ==== //
int main()
{
	// while(true) main3();
	// setting up functions and methods
	function f[] = {f1, f2, f3};
	method m[]  = {rectangleMethod     , trapezoidalRule     , simpsonsRule};
	method me[] = {rectangleMethodError, trapezoidalRuleError, simpsonsRuleError};
	string name[] = {
		" RECTANGLE METHOD ",
		"TRAPEZOIDAL METHOD",
		" SIMPSON'S METHOD "
	};
	string funct[] = { "e^x", "sqrt(1 - x^2)",	"e^(-x^2)" };
	
	bool _continue = true;

	while (_continue)
	{
		system("CLS");
		int steps = 1;                   // starting count of steps (quadrants) = 2^steps
		int max_steps = (int)pow(2, 10); // max steps until give up
		double max_error = 0.1;          // maximum error allowed

		cout << "Input maximum error: ";
		cin >> max_error;

		double start = 0; // starting x
		double end = 1;   // end x
		for (int i = 0; i < 3; i++) // for each function
		{
			cout << "=== [f(x) = " << funct[i] << "] ===" << endl;
			for (int j = 0; j < 3; j++) // for each method
			{
				answer a;
				while (pow(2, steps) <= max_steps) // while max steps have not been hit
				{
					a = applyMethod(m[j], me[j], f[i], start, end, (int)pow(2, steps), false); // calculates the integral
					if (a.err <= max_error) break; // if the error is in the threshold breaks
					steps++; // adds a step
				}

				printf(" ++ %s ++ (steps: %d/%d)\n", name[j].c_str(), (int)pow(2, steps), (int)max_steps);
				printf("Integral of f(x) = %lf from %lf to %lf\nerror of %lf\n\n", a.ans, start, end, a.err);
			}
			cout << endl;
		}

		cout << "Try again? ";
		cin >> _continue;
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
