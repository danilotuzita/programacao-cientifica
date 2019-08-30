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
double derivative(function fnc, double x, int degree=1, double dt=0.000001, bool debug=true)
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

// DERIVATES A FUNCTION DEGREES(0 - 4) ref: https://en.wikipedia.org/wiki/Finite_difference_coefficient
double derivativeV2(function fnc, double x, int degree=1, int accuracy=1, double dt=0.000001, bool debug=true)
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

// DERIVATES A FUNCTION DEGREES(0 - 4) ref: https://en.wikipedia.org/wiki/Finite_difference_coefficient
double derivativeV3(function fnc, double x, int degree=1, int accuracy=1, double dt=0.000001, bool debug=true)
{
	if (degree < 0)  throw domain_error("Cannot derivate to negative degree");
	if (degree > 4)  throw domain_error("Cannot derivate degree greater than 4");
	if (degree == 0) return fnc(x);
	//if (degree == 1) return (fnc(x) - fnc(x - dt)) / dt;
	
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

// ==== METHODS ==== //

// RECTANGLE METHOD
double rectangleMethod(function f, double a, double b) { return (b - a) * f((a + b) / 2); }
double rectangleMethodError(function f, double a, double b) { return (pow((b - a), 3) / 24) * derivative(f, ((b - a) / 2), 2, .9); };

// TRAPEZOIDAL METHOD
double trapezoidalRule(function f, double a, double b) { return (b - a) * ((f(a) + f(b)) / 2); }
double trapezoidalRuleError(function f, double a, double b) { return (pow((b - a), 3) / 36) * derivative(f, ((b - a) / 2), 2, .9); };

// SIMPSON'S METHOD
double simpsonsRule(function f, double a, double b)
{
	double temp = f(a) + f(b) + (4 * f((a + b) / 2));
	temp /= 6;
	return (b - a) * temp;
}
double simpsonsRuleError(function f, double a, double b) { return -(pow(b - a, 5) / 2880) * derivative(f, (b - a) / 2, 4, 0.9); }


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
	//printf("i: %d | %lf\n\n", 0, derivativeV2(f1, .1, 0));
	//printf("i: %d | %lf\n\n", 1, derivativeV2(f1, .1, 1, .1));
	double inp = 1;
	double x_0 = 1;
	int    acc = 5;
	for (int i = 1; i < 5; i++)
	{
		while (inp)
		{
			cin >> inp;
			printf(" **** f(x_0) = %lf\n\n", derivativeV3(f1, x_0, 0));

			double val = derivative(f1, x_0, i, inp);
			printf(" *V1* f");
			for (int j = 0; j < i; j++) printf("'");
			printf("(x_0) = %lf\n\n", i, val);

			val = derivativeV2(f1, x_0, i, acc - 1, inp);
			printf(" *V2* f");
			for (int j = 0; j < i; j++) printf("'");
			printf("(x_0) = %lf\n\n", i, val);

			val = derivativeV3(f1, x_0, i, acc, inp);
			printf(" *V3* f");
			for (int j = 0; j < i; j++) printf("'");
			printf("(x_0) = %lf\n\n", i, val);
		}
		system("CLS");
		inp = 1;
	}
		
	
	system("PAUSE");
	//cout << f4(10);
}

int main()
{
	main2();
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
