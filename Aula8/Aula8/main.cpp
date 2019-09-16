#include <omp.h>
#include <cmath>
#include <string>
#include <iostream>
#include <stdexcept>
#include <windows.h>

#define e exp(1.)
#define uint unsigned int

typedef double(*function)(double);
typedef double(*method)(function, double, double);
typedef struct { double ans, err; } answer;

// DERIVATES A FUNCTION DEGREES(0 - 4) ref: https://en.wikipedia.org/wiki/Finite_difference_coefficient#Forward_finite_difference
double derivative(function fnc, double x, int degree=1, uint accuracy=1, double dt=0.000001, bool debug=false)
{
	if (degree  < 0) throw std::domain_error("Cannot derivate to negative degree");
	if (degree  > 4) throw std::domain_error("Cannot derivate degree greater than 4");
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


// ==== METHODS ==== //
// RECTANGLE METHOD
double rectangleMethod(function f, double a, double b)      { return (b - a) * f((a + b) / 2); }
double rectangleMethodError(function f, double a, double b) { return (pow((b - a), 3) / 24) * derivative(f, ((b - a) / 2), 2, 5, .00001); };

// TRAPEZOIDAL METHOD
double trapezoidalRule(function f, double a, double b)      { return (b - a) * ((f(a) + f(b)) / 2); }
double trapezoidalRuleError(function f, double a, double b) { return (pow((b - a), 3) / 36) * derivative(f, ((b - a) / 2), 2, 5, .00001); };

// SIMPSON'S METHOD
double simpsonsRule(function f, double a, double b)
{
	double temp = f(a) + f(b) + (4 * f((a + b) / 2));
	temp /= 6;
	return (b - a) * temp;
}
double simpsonsRuleError(function f, double a, double b) { return -(pow(b - a, 5) / 2880) * derivative(f, (b - a) / 2, 4, 5, 0.01); }


// ==== GENERIC ITERATIVE METHOD ==== //
// integrates a function handling a passed method and method error
answer applyMethod(method m, method errm, function f, double a, double b, int steps, bool debug=false)
{
	double quadrantSize = (b - a) / steps;

	double ret = 0;
	double err = errm(f, a, a + quadrantSize) * steps; // calculates the estimated error for quadrant size and multiply by the amount of quadrants

	#pragma omp parallel for reduction(+:ret)
	for (int i = 0; i < steps; i++)  // for each quadrant
	{
		double _a = a + ((i)     * quadrantSize); // calculates quandrant's a
		double _b = a + ((i + 1) * quadrantSize); // calculates quandrant's b
		
		double val = m(f, _a, _b); // calculates the integral using passed method
		ret += val;                // adds the calculated integral to the total area

		if (debug) printf("(thread: %2d cpu: %2d) [a: %6lf, b: %6lf] = %6lf\n", omp_get_thread_num(), GetCurrentProcessorNumber(), _a, _b, val);
	}

	return { ret, abs(err) };
}

// ==== MAIN ==== //
int main()
{
	// setting up omp
	omp_set_dynamic(0);
	
	// setting up functions and methods
	function f[] = {f1, f2, f3};
	method m[]  = {rectangleMethod     , trapezoidalRule     , simpsonsRule};
	method me[] = {rectangleMethodError, trapezoidalRuleError, simpsonsRuleError};
	std::string funct[] = { "e^x", "sqrt(1 - x^2)",	"e^(-x^2)" };
	std::string  name[] = {
		" RECTANGLE METHOD ",
		"TRAPEZOIDAL METHOD",
		" SIMPSON'S METHOD "
	};
	
	// setting up execution variables
	int steps = 10000000; // number of divisions of the interval
	double start = 0; // starting x
	double   end = 1; // end x

	bool csvlike = true;
	int threads[6] = {1, 2, 4, 8, 16, 32};
	
	for (int t = 0; t < 6; t++)
	{
		omp_set_num_threads(threads[t]);
		
		if (!csvlike) printf("NUMBER OF THREADS: %d\n", threads[t]);

		for (int i = 0; i < 3; i++) // for each function
		{
			if (!csvlike) printf("=== [f(x) = %s] ===\n", funct[i].c_str());
			else          printf("f(x) = %s\nThreads|Metodo|Resultado|Erro|Tempo\n", funct[i].c_str());
			
			for (int j = 0; j < 3; j++) // for each method
			{
				if (!csvlike) printf(" ++ %s ++ (steps: %d)\n", name[j].c_str(), steps);
				else          printf("%d|%s|", threads[t], name[j].c_str());
				
				double startTime = omp_get_wtime();
				answer a = applyMethod(m[j], me[j], f[i], start, end, steps, false); // calculating the integral
				double endTime = omp_get_wtime();
				
				if (!csvlike) printf("Integral f(x)[a: %6lf, b: %6lf] = %6lf | err: %6lf\n", start, end, a.ans, a.err);
				if (!csvlike) printf("Time: %6lf\n\n", endTime - startTime);
				else          printf("%lf|%lf|%lf\n", a.ans, a.err, endTime - startTime);
			}
			
			printf("\n");
		}
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
