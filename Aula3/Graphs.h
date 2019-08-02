#pragma once

#include <iostream>
#include <cstdlib>
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

// GRAPH GENERATION / INPUT UTIL
Vector< Vector<int>* >* generate_adjmatrix(int nodeCount, double chance, bool directional = false, int max_weight = 1);
Vector< Vector<int>* >* generate_adjmatrix_from_maze(bool diagonal = false);
Vector< Vector<int>* >* input_adjmatrix();
Vector< Vector<int>* >* input_maze();
int input_node(int size, string prefix = "Starting");

// BFS
Vector<int> bfs_distances(Vector< Vector<int>* > graph, int start, bool debug = false);
int bfs_shortest_path(Vector< Vector<int>* > graph, int start, int end, bool debug = false);

// DFS
Vector<int> dfs_distances(Vector< Vector<int>* > graph, int start, bool debug = false);
Vector< Vector<int>* >* dfs_maze(Vector< Vector<int>* >* graph, Point start, bool diagonal, bool debug);

// A*
float simpleHeuristic(Vector< Vector<int>* >* g, const int node, const int goal);
float dijkstra(Vector< Vector<int>* >* g, const int node, const int goal);
int aStar(Vector< Vector<int>* >* graph, int start, int end, float(*heuristicFunction)(Vector< Vector<int>* >* g, const int node, const int goal));
int aStarMaze(Vector< Vector<int>* >* graph, Point start, Point end, bool diagonal, float(*heuristicFunction)(Vector< Vector<int>* >* g, const int node, const int goal));
