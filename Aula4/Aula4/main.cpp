#include <iostream>
#include <cmath>
#include <Vector.hpp>

using namespace std;
typedef double(*function)(double, Vector<double>);
typedef bool(*condition)(double, double);

void printPolinomio(Vector<double> params)
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

		if (i != degree)
			cout << c[(p > 0)] << " ";

		p = abs(p);

		if (i)
			cout << p << "x^" << i << " ";
		else
			cout << p;
	}
	cout << endl;
}

double polinomio(double x, Vector<double> params)
{
	int degree = params.size() - 1;
	double ret = 0;
	for (int i = degree; i >= 0; i--)
		ret += params[i] * pow(x, i);

	return ret;
}

bool myCondition(double x, double f)
{
	return (f > 0.1) || (f < -0.1);
}

double derivative(function fnc, Vector<double> p, double x, double dt=0.0001)
{
	return (fnc(x, p) - fnc(x - dt, p)) / dt;
}

double newton(function fnc, Vector<double> p, condition cnd, double beta=0.1, double x=100, bool debug=false)
{
	double f = fnc(x, p);
	double s = derivative(fnc, p, x);
	
	printPolinomio(p);
	if(debug) cout << "f(" << x << ") = " << f << endl;
	
	while (cnd(f, s))
	{
		if (debug) cout << "\tnao passou na condicao\n";
		s = derivative(fnc, p, x);
		x -= beta * s;
		if (debug) cout << "\tnovo x = " << x << endl;
		f = fnc(x, p);
		if (debug) cout << "f(" << x << ") = " << f << endl;
	}

	return x;
}

int main()
{
	bool cont = false;
	Vector<double> p;
	
	int degree;
	while (!cont)
	{
		p.clear();
		cout << "Qual o grau da funcao? ";
		cin >> degree;
		for (int i = 0; i < degree + 1; i++)
		{
			double temp = 0;
			cout << (char)('a' + i) << ": ";
			cin >> temp;
			p << temp;
		}
		printPolinomio(p);
		cout << "A funcao esta correta? ";
		cin >> cont;
	}

	p.shrink_to_fit();

	cont = true;
	while (cont)
	{
		double in = 0;
		cout << "Digite um x: ";
		cin >> in;
		cout << "f(" << in << ") = " << polinomio(in, p) << endl;
		cout << "Tentar outro x? ";
		cin >> cont;
	}

	cont = true;
	while (cont)
	{
		double in = 0;
		cout << "Digite um x: ";
		cin >> in;
		double r = newton(polinomio, p, myCondition, 0.01, in);
		cout << "Vale mais proximo: " << r << endl;
		cout << "Tentar outro x? ";
		cin >> cont;
	}

	return 0;
}


/*
Use o método de Descida de Gradiente para encontrar o mínimo das seguintes funções:
a) f(x) = x2, sendo x0 = 2.
b) f(x) = x3 – 2x2 +2, sendo x0 = 2.
Verifique o que acontece mudando a taxa de aprendizado de 0,1 até 1,0
*/