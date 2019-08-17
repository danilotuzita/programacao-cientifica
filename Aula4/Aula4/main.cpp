#include <iostream>
#include <cmath>
#include <Vector.hpp>

using namespace std;
typedef double(*function)(double, Vector<double>);
typedef bool(*condition)(double, double);

void printPolynomial(Vector<double> params)
{
	int degree = params.size() - 1;
	double ret = 0;
	cout << "f(x) = ";
	char c[2] = {'-', '+'};
	for (int i = degree; i >= 0; i--)
	{
		double p = params[i];
		if (p == 0)
			continue;

		if (i != degree || p < 0)
			cout << c[(p > 0)] << " ";

		p = abs(p);
		if (i)
		{
			if (p != 1) cout << p;
			cout << "x^" << i << " ";
		}
		else
			cout << p;
	}
	cout << endl;
}

// Function that calculates the f(x) of any polynomial
double polynomimal(double x, Vector<double> params) // params: vetor que guarda o multiplicador de cada grau do polinomio
{
	int degree = params.size() - 1;
	double ret = 0;
	for (int i = degree; i >= 0; i--)
		ret += params[i] * pow(x, i);

	return ret;
}

bool gradientDescent(double f, double min) { return (f > min) || (f <= -min); }

double derivative(function fnc, Vector<double> p, double x, double dt=0.000001) { return (fnc(x, p) - fnc(x - dt, p)) / dt; }

double newtonOptimization(function fnc, Vector<double> p, condition cnd, double beta=0.1, double x=100, int max_it=1000, bool debug=false)
{
	int it = 0;
	double f = fnc(x, p);
	double s = derivative(fnc, p, x);
	if (debug)	printf( "Parameters:\n\tbeta: %lf\n\tminimal diference: %lf\nf(%8lf) = %8lf ", beta, 0.1, x, f);
	
	while (cnd(s, 0.0001) && it++ < max_it)
	{
		s = derivative(fnc, p, x);
		x = x - (beta * s);
		if (debug) printf("| f'(x) = %8lf | failed the condition\n", s);
		f = fnc(x, p);
		if (debug) printf("f(%8lf) = %8lf ", x, f);
	}

	if (debug) printf("Iterations: %d out of %d\n", it, max_it);
	// printf("%lf %d %d\n", beta, it - 1, max_it);
	return x;
}

int main()
{
	bool cont0 = true;
	bool cont = false;
	Vector<double> p;
	
	while (cont0)
	{
		int degree;
		double beta;
		while (!cont)
		{
			p.clear();
			cout << "Polynomial degree: ";
			cin >> degree;
			for (int i = degree; i >= 0; i--)
			{
				double temp = 0;
				cout << (char)('a' + i) << ": ";
				cin >> temp;
				p << temp;
			}
			printPolynomial(p);
			cout << "Is the polynomial correct? ";
			cin >> cont;
		}

		p.shrink_to_fit();

		/*cont = true;
		while (cont)
		{
			double in = 0;
			cout << "Digite um x: ";
			cin >> in;
			cout << "f(" << in << ") = " << polinomio(in, p) << endl;
			cout << "Tentar outro x? ";
			cin >> cont;
		}*/

		cont = true;
		while (cont)
		{
			double in = 0;
			cout << " ===== GRADIENT DESCENT ===== \n";
			cout << "Beta: ";
			cin >> beta;
			cout << "Starting x: ";
			cin >> in;
			double r = newtonOptimization(polynomimal, p, gradientDescent, beta, in, 1000, true);
			cout << "Nearest minimum: x = " << r << endl;
			cout << "Try another starting x? ";
			cin >> cont;
		}

		cout << "\n\nTry another polynomial? ";
		cin >> cont0;
	}

	system("PAUSE");
	return 0;
}


/*
Use o metodo de Descida de Gradiente para encontrar o minimo das seguintes funcoes:
a) f(x) = x^2, sendo x_0 = 2.
b) f(x) = x^3 - 2x^2 + 2, sendo x_0 = 2.
Verifique o que acontece mudando a taxa de aprendizado de 0,1 ate 1,0
*/