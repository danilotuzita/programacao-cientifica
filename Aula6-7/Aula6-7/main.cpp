#include <iostream>
#include <cmath>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

#define MASTER 0

using namespace std;

typedef double(*function)(double);
typedef bool(*function3d)(double, double, double);
typedef double(*method)(function, double, double);
typedef struct { double a, b; } dimension;

// select a random value between a and b
double getRandom(double a, double b) { return (rand() / (RAND_MAX)) * (b - a) + a; }

// select a random value between a and b
double getRandom(dimension x) { return getRandom(x.a, x.b); }

// 4 / (1 - x^2)
double f1(double x) { return 4 / (1 - pow(x, 2)); }

// sqrt(x + sqrt(x))
double f2(double x) { return sqrt(x + sqrt(x)); }

// returns if passed coordnates are in the torus volume
bool isToroid(double x, double y, double z)
{
	return pow(z, 2) + pow(sqrt(pow(x, 2) + pow(y, 2)) - 3, 2) <= 1 ? true : false;
}

// integrates a function using Monte Carlo's Method
double monteCarlo(function fnc, double a, double b, int n, bool debug=false)
{
	double total = 0;
	for (int i = 0; i < n; i++)
		total += fnc(getRandom(a, b));
	if (debug) printf("total: %lf\n", total);
	return (b - a) * (total / n);
}


double monteCarlo3D(function3d fnc, dimension x, dimension y, dimension z, unsigned int n, bool debug=true)
{
	double deltaX = x.b - x.a;
	double deltaY = y.b - y.a;
	double deltaZ = z.b - z.a;

	double domainVolume = deltaX * deltaY * deltaZ; // calculating domain volume
	unsigned int tenPercent = (n * 10) / 100;

	unsigned int hits = 0;
	for (unsigned int i = 1; i <= n; i++)
	{
		hits += fnc(getRandom(x), getRandom(y), getRandom(z));
		if (debug && !(i % tenPercent)) printf("%3u %% %u iteracoes | %u hits\n", ((i * 100) / n), i, hits);
	}

	return domainVolume * hits / n;
}


void ex1()
{
	for (int i = 100; i < 100000; i *= 10)
		printf("%d: %lf\n", i, monteCarlo(f1, 0, 1, i));
	printf("\n");
	for (int i = 100; i < 100000000; i *= 10)
		printf("%d: %lf\n", i, monteCarlo(f2, 0, 1, i));
}


void ex2()
{
	dimension x = {  1, 4 };
	dimension y = { -3, 4 };
	dimension z = { -2, 2 };

	for (int i = 100; i <= 100000000; i *= 10)
		printf("%d: %lf\n", i, monteCarlo3D(isToroid, x, y, z, i));
}


int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));
	
	int taskId, numTask;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &taskId);
	MPI_Comm_size(MPI_COMM_WORLD, &numTask);


	//ex1();
	ex2();

	MPI_Finalize();
	system("PAUSE");
	return 0;
}