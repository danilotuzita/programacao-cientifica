#pragma once

#include <iostream>
#include <cstdlib>
#include <Vector.hpp>
#include <Queue.hpp>
#include <Stack.hpp>
#include <time.h>

using namespace std;

// GRAPH GENERATION / INPUT UTIL
Vector< Vector<int>* >* generate_adjmatrix(int nodeCount, double chance, bool directional = false, int max_weight = 1);
Vector< Vector<int>* >* generate_adjmatrix_from_maze(bool diagonal = false);
Vector< Vector<int>* >* input_adjmatrix();
int input_node(int size, string prefix = "Starting");

// BFS
Vector<int> bfs_distances(Vector< Vector<int>* > graph, int start, bool debug = false);
int bfs_shortest_path(Vector< Vector<int>* > graph, int start, int end, bool debug = false);

// DFS
Vector<int> dfs_distances(Vector< Vector<int>* > graph, int start, bool debug = false);
int dfs_shortest_path(Vector< Vector<int>* > graph, int start, int end, bool debug = false);

// A*
float simpleHeuristic(Vector< Vector<int>* >* g, const int node, const int goal);
float aStar(Vector< Vector<int>* >* graph, int start, int end, float(*heuristicFunction)(Vector< Vector<int>* >* g, const int node, const int goal));
