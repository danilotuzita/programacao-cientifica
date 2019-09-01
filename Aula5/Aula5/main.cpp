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

// CALCULATES A FACTORIAL
uint factorial(int n)
{
	if (n < 0)  throw domain_error("Cannot factor negative numbers");
	if (n > 12) throw overflow_error("Cannot fit !" + to_string(n) + " in a uint");
	return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n; // if n <= 1 return 1 else calculates the previous factorial and multiply by n
}

// CALCULATES A BINOMIAL COEFFICIENT
double binomial(int n, int k) { return factorial(n) / (factorial(k) * factorial(n - k)); }

// (DOES NOT WORK) DERIVATES AT ANY DEGREE A FUNCTION ref: https://en.wikipedia.org/wiki/Numerical_differentiation#Higher-order_methods
double derivative(function fnc, double x, int degree=1, double dt=0.000001, bool debug=false)
{
	if (degree  < 0)  throw domain_error("Cannot derivate to negative degree");
	if (degree == 0) return fnc(x);
	// if (degree == 1) return (fnc(x) - fnc(x - dt)) / dt;

	if (debug) printf("Derivating f(%lf) in %d degree, Accuracy of N/A, dt of %lf\n", x, degree, dt);

	double ans = 0;
	for (int k = 0; k < degree; k++)
	{
		double it = pow(-1, k + degree) * binomial(degree, k) * fnc(x + (k*dt));
		ans += it;
		if (debug) printf("k: %2d | f(%6lf): %12lf | ans: %12lf\n", k, x + (dt * k), it, ans);
	}
		
	return ans * (1 / pow(dt, degree));
}

// (DOES NOT WORK) DERIVATES A FUNCTION DEGREES(0 - 4) ref: https://en.wikipedia.org/wiki/Finite_difference_coefficient#Central_finite_difference
double derivativeV2(function fnc, double x, int degree=1, int accuracy=1, double dt=0.000001, bool debug=false)
{
	if (degree < 0)  throw domain_error("Cannot derivate to negative degree");
	if (degree > 4)  throw domain_error("Cannot derivate degree greater than 4");
	if (degree == 0) return fnc(x);
	//if (degree == 1) return (fnc(x) - fnc(x - dt)) / dt;
	
	double vec[6][4][11] = 
	{
		{     // first derivative
			{    (0.),      (0.),      (0.),       (0.),      (-1/2.),      (0.),       (1/2.),      (0.),      (0.),     (0.),      (0.)   }, // accuracy 2
			{    (0.),      (0.),      (0.),      (1/12.),    (-2/3.),      (0.),       (2/3.),    (-1/12.),    (0.),     (0.),      (0.)   }, // accuracy 4
			{    (0.),      (0.),    (-1/60.),    (3/20.),    (-3/4.),      (0.),       (3/4.),    (-3/20.),  (1/60.),    (0.),      (0.)   }, // accuracy 6
			{    (0.),    (1/280.),  (-4/105.),   (1/5.),     (-4/5.),      (0.),       (4/5.),    (-1/5.),   (4/105.), (-1/280.),   (0.)   }  // accuracy 8
		}, { // second derivative
			{    (0.),      (0.),      (0.),       (0.),        (1.),       (-2.),       (1.),       (0.),      (0.),     (0.),      (0.)   }, // accuracy 2
			{    (0.),      (0.),      (0.),     (-1/12.),     (4/3.),     (-5/2.),     (4/3.),    (-1/12.),    (0.),     (0.),      (0.)   }, // accuracy 4
			{    (0.),      (0.),     (1/90.),   (-3/20.),     (3/2.),    (-49/18.),    (3/2.),    (-3/20.),  (1/90.),    (0.),      (0.)   }, // accuracy 6
			{    (0.),    (-1/560.), (8/315.),    (-1/5.),     (8/5.),   (-205/72.),    (8/5.),    (-1/5.),   (8/315.), (-1/560.),   (0.)   }  // accuracy 8
		}, { // third derivative
			{    (0.),      (0.),      (0.),      (-1/2.),      (1.),       (0.),       (-1.),      (1/2.),     (0.),     (0.),      (0.)   }, // accuracy 2
			{    (0.),      (0.),     (1/8.),      (-1.),     (13/8.),      (0.),      (-13/8.),     (1.),    (-1/8.),    (0.),      (0.)   }, // accuracy 4
			{    (0.),    (-7/240.),  (3/10.),  (-169/120.),  (61/30.),     (0.),     (-61/30.),  (169/120.), (-3/10.), (7/240.),    (0.)   }, // accuracy 6
			{    (0.),    (-7/240.),  (3/10.),  (-169/120.),  (61/30.),     (0.),     (-61/30.),  (169/120.), (-3/10.), (7/240.),    (0.)   }  // accuracy 6
		}, { // forth derivative
			{    (0.),      (0.),      (0.),       (1.),       (-4.),       (6.),       (-4.),       (1.),      (0.),     (0.),      (0.)   }, // accuracy 2
			{    (0.),      (0.),     (-1/6.),     (2.),      (-13/2.),    (28/3.),    (-13/2.),     (2.),    (-1/6.),    (0.),      (0.)   }, // accuracy 4
			{    (0.),    (7/240.),   (-2/5.),   (169/60.),  (-122/15.),   (91/8.),   (-122/15.), (169/60.),  (-2/5.),  (7/240.),    (0.)   }, // accuracy 6
			{    (0.),    (7/240.),   (-2/5.),   (169/60.),  (-122/15.),   (91/8.),   (-122/15.), (169/60.),  (-2/5.),  (7/240.),    (0.)   }  // accuracy 6
		}, { // fifth derivative
			{    (0.),      (0.),     (-1/2.),     (2.),      (-5/2.),      (0.),       (5/2.),     (-2.),     (1/2.),    (0.),      (0.)   }, // accuracy 2
			{    (0.),     (1/6.),    (-3/2.),    (13/3.),    (-29/6.),     (0.),      (29/6.),    (-13/3.),   (3/2.),   (-1/6.),    (0.)   }, // accuracy 4
			{ (-13/288.), (19/36.),  (-87/32.),   (13/2.),   (-323/48.),    (0.),     (323/48.),   (-13/2.),  (87/32.), (-19/36.), (13/288.)}, // accuracy 6
			{ (-13/288.), (19/36.),  (-87/32.),   (13/2.),   (-323/48.),    (0.),     (323/48.),   (-13/2.),  (87/32.), (-19/36.), (13/288.)}  // accuracy 6
		}, { // sixth derivative
			{    (0.),      (0.),      (1.),       (-6.),      (15.),      (-20.),      (15.),      (-6.),      (1.),     (0.),      (0.)   }, // accuracy 2
			{    (0.),     (-1/4.),    (3.),      (-13.),      (29.),     (-75/2.),     (29.),      (-13.),     (3.),    (-1/4.),    (0.)   }, // accuracy 4
			{ (13/240.),  (-19/24.), (87/16.),   (-39/2.),    (323/8.),  (-1023/20.),  (323/8.),   (-39/2.),  (87/16.), (-19/24.), (13/240.)}, // accuracy 6
			{ (13/240.),  (-19/24.), (87/16.),   (-39/2.),    (323/8.),  (-1023/20.),  (323/8.),   (-39/2.),  (87/16.), (-19/24.), (13/240.)}  // accuracy 6
		}
	};
	
	if (debug) printf("Derivating f(%lf) in %d degree, Accuracy of %d, dt of %lf\n", x, degree,  2 * accuracy + 2, dt);

	double ans = 0;
	for (int i = -5; i <= 5; i++)
	{
		double smt = vec[degree - 1][accuracy][i + 5];
		if (!smt && i) continue;
		double it = smt * fnc(x + (dt * i));
		ans += it;
		if (debug) printf("k: %2d | f(%6lf): %12lf | ans: %12lf\n", i, x + (dt * i), it, ans);
	}

	ans /= pow(dt, degree);

	return ans;
}

// DERIVATES A FUNCTION DEGREES(0 - 4) ref: https://en.wikipedia.org/wiki/Finite_difference_coefficient#Forward_finite_difference
double derivativeV3(function fnc, double x, int degree=1, uint accuracy=1, double dt=0.000001, bool debug=false)
{
	if (degree < 0)  throw domain_error("Cannot derivate to negative degree");
	if (degree > 4)  throw domain_error("Cannot derivate degree greater than 4");
	if (degree == 0) return fnc(x);
	//if (degree == 1) return (fnc(x) - fnc(x - dt)) / dt;
	
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
	for (int i = 0; i <= 8; i++)
	{
		double smt = vec[degree - 1][accuracy][i];
		if (!smt && i) continue;
		double it = smt * fnc(x + (dt * i));
		ans += it;
		if (debug) printf("k: %2d | f(%6lf): %12lf | ans: %12lf\n", i, x + (dt * i), it, ans);
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
	for (int i = 0; i < steps; i++)  // for each quadrant
	{
		double _a = i * (b / steps);       // calculates quandrant's a
		double _b = (i + 1) * (b / steps); // calculates quandrant's b
		double val = m(f, _a, _b);         // calculates the integral using passed method
		double cerr = errm(f, _a, _b);     // calculates the method error

		ret += val;  // adds the calculated integral to the total area
		err += cerr; // adds the calculated error to the total error

		if (debug) printf("a: %lf, b: %lf: %lf\n", _a, _b, val);
		if (debug) printf("\terr: %lf\n", cerr);
	}

	return { ret, err };
}


// ==== TEST FUNCTIONS ==== //
void main3()
{
	double dt = 1;
	double x_0 = .5;
	int    acc = 5;
	cin >> dt;
	
	printf("f(%lf) = %lf\n\n", x_0, f1(x_0));

	printf("ans f'(%lf) = %lf\n", x_0, f2d1(x_0));
	printf(" v1 f'(%lf) = %lf\n", x_0, derivative(f2, x_0, 1, dt, false));
	// printf(" v2 f'(%lf) = %lf\n", x_0, derivativeV2(f2, x_0, 1, 5, dt, false));
	printf(" v3 f'(%lf) = %lf\n\n", x_0, derivativeV3(f2, x_0, 1, 5, dt, false));

	printf("ans f''(%lf) = %lf\n", x_0, f2d2(x_0));
	printf(" v1 f''(%lf) = %lf\n", x_0, derivative(f2, x_0, 2, dt, false));
	// printf(" v2 f''(%lf) = %lf\n", x_0, derivativeV2(f2, x_0, 2, 5, dt, false));
	printf(" v3 f''(%lf) = %lf\n\n", x_0, derivativeV3(f2, x_0, 2, 5, dt, false));

	printf("ans f'''(%lf) = %lf\n", x_0, f2d3(x_0));
	printf(" v1 f'''(%lf) = %lf\n", x_0, derivative(f2, x_0, 3, dt, false));
	// printf(" v2 f''(%lf) = %lf\n", x_0, derivativeV2(f2, x_0, 2, 5, dt, false));
	printf(" v3 f'''(%lf) = %lf\n\n", x_0, derivativeV3(f2, x_0, 3, 5, dt, false));

	printf("ans f''''(%lf) = %lf\n", x_0, f2d4(x_0));
	printf(" v1 f''''(%lf) = %lf\n", x_0, derivative(f2, x_0, 4, dt, false));
	// printf(" v2 f''(%lf) = %lf\n", x_0, derivativeV2(f2, x_0, 4, 5, dt, false));
	printf(" v3 f''''(%lf) = %lf\n\n", x_0, derivativeV3(f2, x_0, 4, 5, dt, false));
}

void main2()
{
	//cout << derivative(f2, 3, 2) << endl;
	//cout << binomial(7, 2) << endl;
	//printf("i: %d | %lf\n\n", 0, derivativeV2(f1, .1, 0));
	//printf("i: %d | %lf\n\n", 1, derivativeV2(f1, .1, 1, .1));
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
			
			val = derivative(f2, x_0, i, inp);
			printf(" *V1* f");
			for (int j = 0; j < i; j++) printf("'");
			printf("(x_0) = %lf\n\n", val);

			val = derivativeV2(f2, x_0, i, acc - 1, inp);
			printf(" *V2* f");
			for (int j = 0; j < i; j++) printf("'");
			printf("(x_0) = %lf\n\n", val);
			
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
	//cout << f4(10);
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
