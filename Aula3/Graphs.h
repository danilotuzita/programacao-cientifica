#pragma once

#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>
#include "Render.h"
#include <Vector.hpp>
#include <Queue.hpp>
#include <Stack.hpp>
#include <time.h>

using namespace std;

typedef struct
{
	int x;
	int y;
} Point;

typedef struct aStarNodeT
{
	int index = -1;
	int dist = -1;
	float heur_weight = -1;
	aStarNodeT* prev_node = nullptr;
} aStarNode;

typedef struct
{
	double score = 0;
	int iterations = 0;
	Vector<int>* route;
} hillClimbSolution;

// GRAPH GENERATION / INPUT UTIL

Vector< Vector<int>* >* generate_adjmatrix(int nodeCount, double chance, bool directional = false, int max_weight = 1);
Vector< Vector<int>* >* generate_adjmatrix_from_maze(bool diagonal = false);
Vector< Vector<int>* >* input_adjmatrix();
Vector< Vector<int>* >* input_maze();
int input_node(int size, string prefix = "Starting");


// BFS

Vector<int> bfsDistances(Vector< Vector<int>* > graph, int start, bool debug = false);
int bfsShortestPath(Vector< Vector<int>* > graph, int start, int end, bool debug = false);


// DFS

Vector<int> dfsDistances(Vector< Vector<int>* > graph, int start, bool debug = false);
Vector< Vector<int>* >* dfsMaze(Vector< Vector<int>* >* graph, Point start, bool diagonal, bool debug);


// HILL CLIMBING
// util
	Vector<Point>* randomPoints(int quantity, int width, int height);
	Vector< Vector<double>* >* distances(Vector<Point>* points);
	Vector<int> generateRoute(int numberOfCities, int numberOfShuffles=3);

// operators
	bool minimize(double a, double b);
	bool maximize(double a, double b);
	Vector< Vector<int>* >* swapCities(Vector<int>* route);

double hillClimbingRestart(Vector<Point>* cities, Vector< Vector<int>* >*(*op)(Vector<int>* route), int width, int height, int maxIter=100, bool(*eval)(double a, double b)=minimize, bool debug=false);


// A*
// heuristics

	float simpleHeuristic(Vector< Vector<int>* >* g, const int node, const int goal);
	float mazeHeuristic(Vector< Vector<int>* >* g, const int node, const int goal);
	float dijkstra(Vector< Vector<int>* >* g, const int node, const int goal);

// algorithms
	// graph must be a adjency matrix
	int aStar(Vector< Vector<int>* >* graph, int start, int end, float(*heuristicFunction)(Vector< Vector<int>* >* g, const int node, const int goal));
	// graph must be a maze
	int aStarMaze(Vector< Vector<int>* >* graph, Point start, Point end, bool diagonal, float(*heuristicFunction)(Vector< Vector<int>* >* g, const int node, const int goal), bool debug=false);
