#include <omp.h>
#include <time.h>
#include <windows.h>
#include <fstream>
#include "Firefly.h"

#define pause system("PAUSE")

constexpr double beta0 = 1.0;
constexpr double a = 0.0;
constexpr double b = 5.0;
constexpr int dimen = 5;

Firefly* base; // Target Firefly Pointer

// Firefly comparison function for quick sorting (descending)
int compare(const void* a, const void* b)
{
	double a1 = (*(Firefly**)a)->getBrightness();
	double b1 = (*(Firefly**)b)->getBrightness();

	if (a1 < b1) return -1;
	if (a1 > b1) return  1;
	return  0;
}

// Calculates euclidian distance between two Fireflies
double dist(Firefly* a, Firefly* b)
{
	double d = 0;
	for (int i = 0; i < a->dimension(); i++)
		d += pow((b->get(i) - a->get(i)), 2);
	return sqrt(d);
}

// Moves all fireflies
void moveFireflies(Vector<Firefly*>* ff, double alpha, double gamma, char debug=0)
{
	int n = ff->size() - 1;        // number of fireflies
	int d = (*ff)[0]->dimension(); // number of dimensions
	
	for (int i = n; i >= 0; i--)         // for each firefly (from the worst firefly)
		for (int j = i - 1; j >= 0; j--) // against every other firefly in a better ranking
		{
			Firefly interference(d);
			interference.generate(0, 1); // generating a random numbers

			double beta = beta0 * exp(-gamma * pow(dist((*ff)[i], (*ff)[j]), 2)); // calculating beta
			if (debug > 1)
			{
				printf("[%d] -> [%d]:\n\t", i, j);
				(*ff)[i]->print();
				printf("\t");
			}
			for (int k = 0; k < d; k++) // for each dimension
			{
				double temp_i = (*ff)[i]->get(k);               // firefly i in dimension k
				double temp_j = (*ff)[j]->get(k);               // firefly j in dimension k
				double temp_interference = interference.get(k); // interference in dimension k
				// calculates a new position in the dimension k
				(*ff)[i]->set(k,
					temp_i + beta * (temp_j - temp_i) + alpha * (temp_interference - 0.5)
				);
			}
			if (debug > 1) (*ff)[i]->print();
		}
}

// Runs firefly genetic algorithm
Firefly* firefly(int dimension, int numberOfFireflies, int maxGenerations, function brightnessEvaluation, double alpha=1.0, double gamma=1.0, double delta=1.0, int threads=1, char debug=0)
{
	/*
	alpha: randomness coeficient           (how random will be the movement variation)
	gamma: light absorbtion coeficient     (how much the brightness will "dim" as it gets more distant)
	delta: randomness reduction coeficient (how the randomness will be less random at each generation)
	beta0: base atractiveness coeficient   (how attractive a firefly would be if were at a 0 distance from another [usualy beta0 = 1])
	*/
	omp_set_num_threads(threads);                                      // setting up OMP threads
	Vector<Firefly*> fireflies(numberOfFireflies);                     // vector of fireflies
	
	// initializing fireflies and distance matrix
	for (int i = 0; i < numberOfFireflies; i++)
	{
		fireflies.push_back(new Firefly(dimension));
		fireflies[i]->generate(a, b);
	}
	
	std::ofstream file;
	file.open("firefly-"+std::to_string(threads)+".csv");
	file << "GERACAO;BRILHO;TEMPO\n";

	// for each generation
	for (int gen = 0; gen < maxGenerations; gen++)
	{
		printf("GENERATION %3d:\n", gen);
		if (debug > 0) printf("CPU/THRD |  INDX  | BRIGHTNESS\n------------------------------\n");
		double start = omp_get_wtime();

		file << gen << ";";

		#pragma omp parallel for
		for (int i = 0; i < numberOfFireflies; i++) // for each Firefly
		{
			fireflies[i]->evaluate(brightnessEvaluation); // evaluates its brightness
			if(debug > 0) printf("  %d/%d    | [%4d] |   %8lf\n", GetCurrentProcessorNumber(), omp_get_thread_num(), i, fireflies[i]->getBrightness());
		}
		fireflies.sort(compare); // sort fireflies by its brightness

		printf("Best Firefly   : ");
		fireflies[0]->print();
		printf("Best Brightness: %lf\n", fireflies[0]->getBrightness());
		
		file << fireflies[0]->getBrightness() << ";";

		moveFireflies(&fireflies, alpha, gamma, debug);
		alpha *= delta;

		double end = omp_get_wtime();
		printf("Processing Time: %lfs\n\n===================\n\n", end - start);

		file << end - start << "\n";
	}

	Firefly* bestFirefly = new Firefly(dimension);
	bestFirefly->copy(fireflies[0]);
	
	file.close();
	return bestFirefly;
}

double bright(void* f) 
{
	for (int i = 0; i < 1e5; i++) pow(i, 5); // INCREASE TIME TO CALCULATE
	return dist((Firefly*)f, base);
}

int main()
{
	srand((unsigned int)time(NULL)); // Getting Random Seed
	omp_set_dynamic(0); // Disabling dynamic OMP

	base = new Firefly(dimen); // Creating a "Target" Firefly
	base->generate(a, b);      // Generating a random "Target" Firefly
	printf("TARGET FIREFLY: ");
	base->print();

	double start = omp_get_wtime();
	auto f = firefly( // Running Firefly
		dimen, 200, 100, bright,
		1.0, .95, .95, 2,
		0
	);
	double end = omp_get_wtime();

	printf("TARGET FIREFLY: ");
	base->print();
	printf("FOUND FIREFLY:  ");
	f->print();
	printf("BRIGHNESS: %lf\n", f->getBrightness());
	printf("Total Time: %lf\n", end - start);
	
	delete base, f;
	pause;
	
	return 0;
}
