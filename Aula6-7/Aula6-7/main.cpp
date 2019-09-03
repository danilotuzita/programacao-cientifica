#include <iostream>
#include <cmath>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

#define pause system("PAUSE")
#define MASTER 0

using namespace std;

typedef double(*function)(double);
typedef bool(*function3d)(double, double, double);
typedef double(*method)(function, double, double);
typedef struct { double a, b; } dimension;

// select a random value between a and b
double getRandom(double a, double b) { return ((double)rand() / (RAND_MAX)) * (b - a) + a; }

// select a random value between a and b
double getRandom(dimension x) { return getRandom(x.a, x.b); }

// 4 / (1 - x^2)
double f1(double x) { return 4 / (1 - pow(x, 2)); }

// sqrt(x + sqrt(x))
double f2(double x) { return sqrt(x + sqrt(x)); }

// returns if passed coordnates are within the torus volume
bool isToroid(double x, double y, double z)
{
	return pow(z, 2) + pow(sqrt(pow(x, 2) + pow(y, 2)) - 3, 2) <= 1 ? true : false;
}

// integrates a function using Monte Carlo's Method
double monteCarlo(function fnc, double a, double b, int n, bool debug=false)
{
	double total = 0;
	if (debug) printf("n: %d\t", n);
	for (int i = 0; i < n; i++)
		total += fnc(getRandom(a, b));
	if (debug) printf("total: %lf\n", total);
	return (b - a) * (total / n);
}


double monteCarlo3D(function3d fnc, dimension x, dimension y, dimension z, unsigned int n, bool debug=false)
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


void ex1(int start, int end, function fnc, int threads=1, int threadId=MASTER, bool debug = false)
{
	if (threadId == MASTER) printf("EX 1 | Number of threads: %d\n", threads);
	for (int i = start; i <= end; i *= 10)
	{
		double totalAverage = 0;
		double threadAvg = monteCarlo(fnc, 0, 1, i / threads);
		if(debug) printf("T: %d | n: %9d | calc: %lf\n", threadId, i, threadAvg);

		MPI_Reduce(&threadAvg, &totalAverage, 1, MPI_DOUBLE, MPI_SUM, MASTER, MPI_COMM_WORLD);
		if (threadId == MASTER)
		{
			totalAverage /= threads;
			printf("%d: %lf\n", i, totalAverage);
		}
	}
}


void ex2(int start, int end, int threads=1, int threadId=MASTER, bool debug=false)
{
	dimension x = { 1, 4 };
	dimension y = { -3, 4 };
	dimension z = { -2, 2 };

	/*for (int i = start; i <= end; i *= 10)
		printf("%d: %lf\n", i, monteCarlo3D(isToroid, x, y, z, i));*/

	if (threadId == MASTER) printf("EX 2 | Number of threads: %d\n", threads);
	for (int i = start; i <= end; i *= 10)
	{
		double totalAverage = 0;
		double threadAvg = monteCarlo3D(isToroid, x, y, z, i / threads);
		if(debug) printf("T: %d | n: %9d | calc: %lf\n", threadId, i, threadAvg);

		MPI_Reduce(&threadAvg, &totalAverage, 1, MPI_DOUBLE, MPI_SUM, MASTER, MPI_COMM_WORLD);
		if (threadId == MASTER)
		{
			totalAverage /= threads;
			printf("%d: %lf\n", i, totalAverage);
		}
	}
}


int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));

	int threads, threadId;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &threadId);
	MPI_Comm_size(MPI_COMM_WORLD, &threads);


	double start, end;
	start = MPI_Wtime();
	ex1(100, 100000000, f1, threads, threadId);
	end = MPI_Wtime();
	if (threadId == MASTER) printf("Time: %lf\n\n", end - start);

	start = MPI_Wtime();
	ex1(100, 100000000, f2, threads, threadId);
	end = MPI_Wtime();
	if (threadId == MASTER) printf("Time: %lf\n\n", end - start);

	start = MPI_Wtime();
	ex2(100, 100000000, threads, threadId);
	end = MPI_Wtime();
	if (threadId == MASTER) printf("Time: %lf\n\n", end - start);

	if (threadId == MASTER) pause;
	MPI_Finalize();

	return 0;
}