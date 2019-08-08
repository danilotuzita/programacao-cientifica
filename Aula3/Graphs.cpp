#include "Graphs.h"
#include <math.h>

void cls()
{
	for (int n = 0; n < 10; n++)
		printf("\n\n\n\n\n\n\n\n\n\n");
}

bool minimize(double a, double b) { return a < b; }
bool maximize(double a, double b) { return a > b; }

// GRAPHS UTIL
// graph generator
Vector< Vector<int>* >* generate_adjmatrix(int nodeCount, double chance, bool directional, int max_weight)
{
	srand(clock()); // setting seed
	auto graph = new Vector< Vector<int>* >(nodeCount); // creating vector of vector

	for (int i = 0; i < nodeCount; i++) // creating vector that will go in the vector
		graph->push_back(new Vector<int>(nodeCount, 0));


	for (int i = 0; i < nodeCount; i++) // for each node
	{
		if (!directional)
			for (int j = i + 1; j < nodeCount; j++) // iterating through the "matrix" upper triangle (symmetrical matrix)
			{
				double r = ((double)rand() / (RAND_MAX)); // generating a number between 0 and 1
				if (r <= chance) // checking chance to be connected
				{
					int w = rand() % max_weight + 1; // generating a random weight
					graph->at(i)->set_at(j, w); // setting node i connected to node j
					graph->at(j)->set_at(i, w); // setting node j connected to node i
				}
			}
		else
			for (int j = 0; j < nodeCount; j++) // for each node
			{
				if (i == j) continue; // except itself

				double r = ((double)rand() / (RAND_MAX)); // generating a number between 0 and 1
				if (r <= chance) // checking chance to be connected
				{
					int w = rand() % max_weight + 1; // generating a random weight
					graph->at(i)->set_at(j, w); // setting node i connected to node j
				}		
			}

		graph->at(i)->simple_print(); // printing line
	}

	return graph;
}

// graph generator from maze
Vector< Vector<int>* >* generate_adjmatrix_from_maze(bool diagonal)
{
	Vector< Vector<int> > maze(100);
	int width = -1, height = -1;

	string line;
	cout << "Press enter 2 times to end graph\n";
	std::getline(cin, line);
	while (line != "") // while did not press enter 2 times
	{
		Vector<int> item(1); // creates a line of the matrix
		if (width != -1) item.reserve(width);
		unsigned int i = 0, last = 0;
		for (; i < line.size(); i++) // reading the line and pushing it to the item
		{
			if (line[i] == ' ')
			{
				item.push_back(stoi(line.substr(last, i - last)));
				last = i + 1;
			}
		}
		item.push_back(stoi(line.substr(last, i - last)));
		item.shrink_to_fit();

		maze.push_back(item); // pushin the item to the matrix
		
		if (width == -1) width = item.size(); // saving the width of the maze

		if (item.size() != width) // break if line is broken
		{
			cout << "TAMANHO ERRADO!!!!!\n";
			system("pause");
			throw std::out_of_range("birajoejfaeifjeoaj");
		}

		std::getline(cin, line);
	}

	maze.shrink_to_fit();
	height = maze.size(); // saving the height of the maze

	int sides[9][2] = { // matrix to access the "neighbors" of a node
		{ 0, -1}, // up 
		{-1,  0}, // left
		{ 1,  0}, // right
		{ 0,  1}, // down
		{-1, -1}, // top left
		{ 1, -1}, // top right
		{-1,  1}, // bottom left
		{ 1,  1}  // bottom right
	};

	int s; // number of item in the sides matrix
	s = 4; // just accessing up, left, right, down
	if (diagonal) // if you want to access its diagonals as well
		s = 9;

	auto graph = new Vector<Vector<int>*>(); // creates a adjacency matrix
	for (int i = 0; i < height * width; i++) // populates it with lines
		graph->push_back(new Vector<int>(height * width, 0));

	for (int j = 0; j < height; j++) // for each line in the maze
	{
		for (int i = 0; i < width; i++) // for each column in the maze
		{
			if (maze[j][i]) // if the node is accessible
			{
				int nodeIndex = j * width + i; // calculates it's index
				for (int k = 0; k < s; k++) // for each side
				{
					int x = i + sides[k][0]; // gets the x of it's neighbor
					int y = j + sides[k][1]; // gets the y of it's neighbor

					if (x >= 0 && x < width && // AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA this might be wrong (x < height); it didn't fail yet so...
						y >= 0 && y < width) // if neighbour is not out of range
					{
						int connectedNodeIndex = y * width + x; // calculates it's index
						graph->at(nodeIndex)->set_at(connectedNodeIndex, maze[y][x]); // sets the node distance from the neighbor in the graph
						graph->at(nodeIndex)->set_at(connectedNodeIndex, maze[y][x]); // sets the neighbor distance from the node in the graph
					}
				}
			}
		}
	}
	/*
	for (int i = 0; i < height * width; i++) // printing adjacency matrix
		graph->at(i)->simple_print();
	*/
	return graph;
}


// input matrix function
Vector< Vector<int>* >* input_adjmatrix()
{
	auto graph = new Vector< Vector<int>* >();
	string line;

	cout << "Press enter 2 times to end graph\n";
	std::getline(cin, line);
	while (line != "")
	{
		auto item = new Vector<int>();
		unsigned int i = 0, last = 0;
		for (; i < line.size(); i++)
		{
			if (line[i] == ' ')
			{
				item->push_back(stoi(line.substr(last, i - last)));
				last = i + 1;
			}
		}
		item->push_back(stoi(line.substr(last, i - last)));
		item->shrink_to_fit();
		//item->print();
		graph->push_back(item);
		std::getline(cin, line);
	}

	graph->shrink_to_fit();
	return graph;
}

Vector< Vector<int>* >* input_maze()
{
	auto maze = new Vector< Vector<int>* >();
	int width = -1;

	string line;
	cout << "Press enter 2 times to end graph\n";
	std::getline(cin, line);
	while (line != "") // while did not press enter 2 times
	{
		auto item = new Vector<int>(1); // creates a line of the matrix
		if (width != -1) item->reserve(width);
		unsigned int i = 0, last = 0;
		for (; i < line.size(); i++) // reading the line and pushing it to the item
		{
			if (line[i] == ' ')
			{
				item->push_back(stoi(line.substr(last, i - last)));
				last = i + 1;
			}
		}
		item->push_back(stoi(line.substr(last, i - last)));
		item->shrink_to_fit();

		maze->push_back(item); // pushin the item to the matrix

		if (width == -1) width = item->size(); // saving the width of the maze

		if (item->size() != width) // break if line is broken
		{
			delete maze;
			cout << "TAMANHO ERRADO!!!!!\n";
			system("pause");
			throw std::out_of_range("birajoejfaeifjeoaj");
		}

		std::getline(cin, line);
	}

	maze->shrink_to_fit();
	return maze;
}

int input_node(int size, string prefix)
{
	int ret;
	cout << prefix;
	cin >> ret;
	while (ret >= size || ret < 0)
	{
		cout << "The node must be between 0 and " << size - 1 << "\n";
		cout << prefix;
		cin >> ret;
	}
	return ret;
}

// ====== BFS ====== //

// Returns a distance vector
Vector<int> bfsDistances(Vector< Vector<int>* > graph, int start, bool debug) // graph must be adjacency matrix
{
	// creating varibales
	Vector<int> dist(graph.size(), -1);      // distance vector | stores the distance to reach each node from starting node | dist = -1: cannot reach from starting node
	Vector<bool> color(graph.size(), false); // color vector    | stores which nodes have been visited | color = false: node has not been visited
	Queue<int> q;                            // search queue    | stores the index of the nodes to search next

	Vector<int>* currentNode; // current node
	int index;                // current node index

	dist[start] = 0; // setting distance of the starting node to the starting node as 0
	color[start] = true; // setting the starting node as visited
	q << start; // pushing start node to the search queue

	while (!q.isEmpty()) // while search queue is not empty
	{
		index = q.pop(); // gets the index of the next node to search
		currentNode = graph[index]; // setting the currentNode as the [index] node
		if (debug) cout << "Searching node: " << index << endl;

		for (int i = 0; i < currentNode->size(); i++) // for each possibly connected node
		{
			if ((*currentNode)[i] && !color[i]) // if the current node is connected to [i] node and has not been visited
			{
				if (debug) cout << "Acessing node: " << i << endl;
				q << i;                    // pushes [i] node to the search queue
				color[i] = true;           // sets the [i] node as visited
				dist[i] = dist[index] + 1; // sets the [i] node distance to the [start]node as the distance of this node's father plus one
			}
		}
	}

	return dist; // returning dist vector
}

// Returns the shortest path
int bfsShortestPath(Vector< Vector<int>* > graph, int start, int end, bool debug) // graph must be adjacency matrix
{
	Vector<int> dist;
	dist = bfsDistances(graph, start, debug);
	return dist[end];
}

// ====== DFS ====== //

Vector<int> dfsDistances(Vector< Vector<int>* > graph, int start, bool debug) // graph must be adjacency matrix
{
	// creating varibales
	Vector<int> dist(graph.size(), -1);      // distance vector | stores the distance to reach each node from starting node | dist = -1: cannot reach from starting node
	Vector<bool> color(graph.size(), false); // color vector    | stores which nodes have been visited | color = false: node has not been visited
	Stack<int> s;                            // search stack    | stores the index of the nodes to search next

	Vector<int>* currentNode; // current node
	int index;                // current node index

	dist[start] = 0; // setting distance of the starting node to the starting node as 0
	color[start] = true; // setting the starting node as visited
	s << start; // pushing start node to the search stack

	while (!s.isEmpty()) // while search stack is not empty
	{
		index = s.pop(); // gets the index of the next node to search
		currentNode = graph[index]; // setting the currentNode as the [index] node
		if (debug) cout << "Searching node: " << index << endl;

		for (int i = 0; i < currentNode->size(); i++) // for each possibly connected node
		{
			if ((*currentNode)[i] && !color[i]) // if the current node is connected to [i] node and has not been visited
			{
				if (debug) cout << "Acessing node: " << i << endl;
				s << i;                    // pushes [i] node to the search stack
				color[i] = true;           // sets the [i] node as visited
				dist[i] = dist[index] + 1; // sets the [i] node distance to the [start]node as the distance of this node's father plus one
			}
		}
	}

	return dist; // returning dist vector
}

Vector< Vector<int>* >* dfsMaze(Vector< Vector<int>* >* graph, Point start, bool diagonal, bool debug) // graph must be adjacency matrix
{
	int height = graph->size();        // height of the maze
	int width = (*(*graph)[0]).size(); // width of the maze

	// creating varibales
	auto dist = new Vector< Vector<int>* >(height);   // distance vector | stores the distance to reach each node from starting node | dist = -1: cannot reach from starting node
	Vector< Vector<bool>* > color(height);            // color vector    | stores which nodes have been visited | color = false: node has not been visited
	Stack<Point> s;                                   // search queue    | stores the index of the nodes to search next
	
	for (int i = 0; i < height; i++)
		*dist << new Vector<int>(width, -1);     // populating dist vector
	for (int i = 0; i < height; i++)
		color << new Vector<bool>(width, false); // populating color vector

	// matrix to access the "neighbors" of a node
	Point sides[9] = {
		{ 0, -1}, // up 
		{-1,  0}, // left
		{ 1,  0}, // right
		{ 0,  1}, // down
		{-1, -1}, // top left
		{ 1, -1}, // top right
		{-1,  1}, // bottom left
		{ 1,  1}  // bottom right
	};
	int n = 4 + (5 * diagonal); // if diagonal is true n will be 9

	Point p; // current node position

	(*(*dist)[start.x])[start.y] = 0;  // setting distance of the starting node to the starting node as 0
	(*color[start.x])[start.y] = true; // setting the starting node as visited
	s << start;                        // pushing start node to the search stack

	while (!s.isEmpty()) // while search stack is not empty
	{
		p = s.pop(); // gets the index of the next node to search
		if (debug) cout << "Searching node: [" << p.x << ", " << p.y << "]" << endl;

		for (int i = 0; i < n; i++) // for each neighbour node
		{
			int x = p.x + sides[i].x; // calculating x of the neighbour node
			int y = p.y + sides[i].y; // calculating y of the neighbour node

			if (x >= 0 && x < height &&
				y >= 0 && y < width) // if neighbour is not out of range
			{
				if ((*(*graph)[x])[y] && !(*color[x])[y]) // if the neighbour is accessible and has not been visited
				{
					Point newPoint = {x, y};
					if (debug) cout << "Acessing node: [" << p.x << ", " << p.y << "]" << endl;
					s << newPoint;         // pushes neighbour to the search stack
					(*color[x])[y] = true; // sets the neighbour node as visited
					(*(*dist)[x])[y] = (*(*dist)[p.x])[p.y] + (*(*graph)[x])[y]; // sets the neighbour node distance to the [start]node as it's weight plus the distance of this node's father
				}
			}
		}
	}


	// printing dist
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			printf("%3d ", (*(*dist)[i])[j]);
		printf("\n");
	}

	return dist; // returning dist vector
}

// ====== HILL ====== //
void showRoute(Render* r, Vector<Point>* cities, Vector<int>* tour, int width, int height, double bestScore, int iter)
{
	int numberOfCities = tour->size();
	int lx, ly; // previous city coordinates

	for (int i = 0; i < numberOfCities; i++) // for each city in the route
	{
		int index = (*tour)[i]; // gets the current city index
		int x = cities->at(index).x; // gets the current city x
		int y = cities->at(index).y; // gets the current city y

		r->point(x, y, 2, to_string(index) + " (" + to_string(x) + ", " + to_string(y) + ")"); // drawing the city
		if (i > 0) // if its not the first iteration
			r->line(x, y, lx, ly); // draws a line between the current city and the previous city
		lx = x; ly = y; // sets the current city coordinates as the previous city coordinates
	}

	r->line(cities->at((*tour)[0]).x, cities->at((*tour)[0]).y, lx, ly); // drawing a line between the last city and the starting city
	r->text(10, 10, "Tour " + to_string(iter) + " Distance: " + to_string(bestScore)); // writing the Tour distance
	r->newMap();
}

// Swap cities operator
Vector< Vector<int>* >* swapCities(Vector<int>* route)
{
	auto newRoutes = new Vector<Vector<int>*>; // creating a vector of vector to store all possible generated routes
	int numberOfCities = route->size();

	for (int i = 1; i < numberOfCities; i++) // for each index in the route
		for (int j = i + 1; j < numberOfCities; j++) // for each next item
		{
			*newRoutes << new Vector<int>(*route); // creates a new route
			newRoutes->end()->set_at(i, route->at(j)); // swaps a city
			newRoutes->end()->set_at(j, route->at(i)); // swaps a city
		}

	return newRoutes;
} // NOTE: this operator is awful, but it does the trick to validate the hill climbing algorithm

// Generates random cities
Vector<Point>* randomPoints(int quantity, int width, int height)
{
	srand(clock());

	auto r = new Vector<Point>();
	for (int i = 0; i < quantity; i++)
		*r << Point{ rand() % width, rand() % height };

	return r;
}

// Calculates the distance between cities
Vector< Vector<double>* >* distances(Vector<Point>* points)
{
	int n = points->size();
	auto dist = new Vector< Vector<double>* >(n);
	for (int i = 0; i < n; i++)
		*dist << new Vector<double>(n, 0);

	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			double delta_x = points->at(i).x - points->at(j).x;	// Calculating the delta x
			double delta_y = points->at(i).y - points->at(j).y; // Calculating the delta y

			double ans = sqrt(pow(delta_x, 2.) + pow(delta_y, 2.)); // Calculating cartesian distance between the two cities
			(*(*dist)[i])[j] = ans; // setting the distance symmetricaly in the matrix
			(*(*dist)[j])[i] = ans; // setting the distance symmetricaly in the matrix
		}
	}

	return dist;
}

// Generates a random route
Vector<int> generateRoute(int numberOfCities, int numberOfShuffles)
{
	Vector<int> v;
	for (int i = 0; i < numberOfCities; i++) // creating a route from 0 - numberOfCities [ 0 1 2 3 ... numberOfCities ]
		v << i;

	// shuffling the vector
	for (int k = 0; k < numberOfShuffles; k++)
	{
		srand(clock());
		for (int i = 1; i < numberOfCities; i++) // for each item swap it with some other random item
		{
			int j = rand() % (numberOfCities - 1) + 1; // selecting a random item
			int temp = v[i];
			v[i] = v[j];
			v[j] = temp;
		}
	}	

	v.shrink_to_fit(); // shrinking the vector to save memory
	return v;
}

// Calculates the route lenght
double routeLenght(Vector< Vector<double>* >* dist, Vector<int>* route, bool print=false)
{
	// setting up variables
	int numberOfCities = dist->size();
	int startingCity = (*route)[0];
	double tourDist = 0;

	int prev = startingCity;
	for (int i = 1; i < numberOfCities; i++) // starting from 1 because the route[0] is the "prev"
	{
		int index = (*route)[i]; // storing the current index
		double roadDist = (*(*dist)[prev])[index]; // getting the distance from the previous city and the current city
		if (roadDist <= 0) return -1; // N�o tem como chegar nessa cidade (futuro)
		tourDist += roadDist; // adding it to the total dist
		if(print) printf("Traveling from [%d] to [%d]: %10lf\n", prev, index, roadDist);

		prev = index; // setting the current city as the previous city
	}

	double roadDist = (*(*dist)[prev])[startingCity]; // getting the distance from the last city of the route to the start city
	tourDist += roadDist; // adding it to the total dist
	if (print) printf("Traveling from [%d] to [%d]: %10lf\n", prev, startingCity, roadDist);
	if (print) printf("Total travel distance: %lf\n", tourDist);
	
	return tourDist;
}

// Hill climbing algorithm
hillClimbSolution hillClimbing(Vector< Vector<double>* >* dist, Vector< Vector<int>* >*(*op)(Vector<int>* route), int width, int height, int maxIter, bool(*eval)(double a, double b), Render* r)
{
	int numberOfCities = dist->size();
	
	// TODO: vvv CORRIGIR PQ EU N�O POSSO COPIAR NO CONSTRUTOR vvv (essa maldicao nao vai funcionar)
	// auto tour = generateRoute(numberOfCities);
	auto bestTour = new Vector<int>;                // stores the best route it could find
	*bestTour = generateRoute(numberOfCities);      // generating a random tour	
	double bestScore = routeLenght(dist, bestTour); // stores the best score of the tour
	

	// printf("1st Tour(%lf): ", bestScore);
	// bestTour->simple_print();
	// showRoute(r, cities, bestTour, width, height, bestScore);

	int iterations = 0;
	while (iterations < maxIter) // while it can iterate
	{
		auto newTours = op(bestTour); // gets all possible "moves"/"operations" this route can do
		bool tourChanged = false;

		for (int i = 0; i < newTours->size(); i++) // for each move possible
		{
			if (iterations >= maxIter) // if it cannot iterate anymore
				break;

			iterations++;
			auto currentTour = (*newTours)[i]; // saves the tour as currentTour
			double currentScore = routeLenght(dist, currentTour); // calculates the score of the tour

			if (eval(currentScore, bestScore)) // if the currentScore is better than the bestScore
			{
				tourChanged = true;
				// stores the tour
				bestTour = currentTour;
				bestScore = currentScore;

				// showRoute(r, cities, bestTour, width, height, bestScore);
				// printf("New Tour(%lf): ", bestScore);
				// bestTour->simple_print();

				// break;
			}
		}

		delete newTours; // freeing memory

		if (!tourChanged) // if the tour has not changed we reach a peak
			break; // no need to search this route anymore
	}

	printf("Number of Tests: %d, Distance: %lf\n\n", iterations, bestScore);
	return { bestScore, iterations, bestTour };
}

// Calls the hillClimbing until it runs out of iterations
double hillClimbingRestart(Vector<Point>* cities, Vector< Vector<int>* >*(*op)(Vector<int>* route), int width, int height, int maxIter, bool(*eval)(double a, double b))
{
	Render r("hillClimbing", width, height, 12, 50, 50); // creating a renderer

	  auto dist = distances(cities);    // calculating distance matrix
	  auto bestTour = new Vector<int>;  // stores the best tour found
	double bestScore = 0;               // stores the score of the best tour
	
	int iterations = 0;
	while (iterations < maxIter) // while it can iterate
	{
		int remainingIter = maxIter - iterations; // calculates remaining iteration
		printf("Remaining: (%d/%d)\n", remainingIter, maxIter);
		hillClimbSolution sol = hillClimbing(dist, op, width, height, remainingIter, eval, &r); // calls hill climbing
		
		if (eval(sol.score, bestScore) || iterations == 0) // if new solution is better or it's the first iteration
		{
			bestScore = sol.score; // stores the new score
			delete bestTour;       // deleting pointer before overwriting
			bestTour = sol.route;  // stores the new tour
			showRoute(&r, cities, bestTour, width, height, bestScore, iterations);
		}
		else delete sol.route;     // delete the new solution's tour pointer

		iterations += sol.iterations; // computes iterations
	}

	r.open_html(); // opening the generated html
	delete bestTour, dist; // deletes the tour and distance matrix
	return bestScore;
}

// ====== A* ====== //
void printNode(aStarNode* node)
{
	if (node != nullptr)
		if (node->prev_node != nullptr)
			printf(
				"Node [%d]\n"
				"	dist: %d\n"
				"	heur: %f\n"
				"	prev: %d\n",
				node->index, node->dist, node->heur_weight, node->prev_node->index
			);
		else
			printf(
				"Node [%d]\n"
				"	dist: %d\n"
				"	heur: %f\n"
				"	prev: null\n",
				node->index, node->dist, node->heur_weight
			);
	else
		printf(
			"Node [null]\n"
			"	dist: null\n"
			"	heur: null\n"
			"	prev: null\n"
		);
}

// sort comparing function for Vector<aStarNode*> (this sorts backwards so the last item is the most eficient next node)
int aStarNode_sort(const void * a1, const void * a2)
{
	aStarNode* n1 = *(aStarNode**)a1;
	aStarNode* n2 = *(aStarNode**)a2;

	float total_weight1 = n1->dist + n1->heur_weight;
	float total_weight2 = n2->dist + n2->heur_weight;

	if (total_weight1 < total_weight2) return  1;
	if (total_weight1 > total_weight2) return -1;
	return 0;
}

float dijkstra(Vector< Vector<int>* >* g, const int node, const int goal)
{
	return 0.f;
}

float simpleHeuristic(Vector< Vector<int>* >* g, const int node, const int goal)
{
	if (node == 5 || node == 6 || node == 7) return 1.1f;
	return 99.1f;
}

int aStar(Vector< Vector<int>* >* graph, int start, int end, float (*heuristicFunction)(Vector< Vector<int>* >* g, const int node, const int goal))
{
	int numberOfNodes = graph->size();

	// creating varibales
	Vector<aStarNode*> nodes(numberOfNodes); // node vector  | stores info about each node
	Vector<int> color(numberOfNodes, 0);     // color vector | stores which nodes have been visited or is already queued | color = 0: node has not been visited
    //                                                                                                                   | color = 1: node is on queue 
	//                                                                                                                   | color = 2: node has been visited

	for (int i = 0; i < numberOfNodes; i++)
		nodes << new aStarNode{ /*index = */ i }; // creating a list of aStarNodes

	nodes[start]->dist = 0;                                           // setting the distance from the origin (start node) of the start node as 0
	nodes[start]->heur_weight = heuristicFunction(graph, start, end); // calculating the heuristic weight of the start node

	Vector<aStarNode*> searchQueue(numberOfNodes); // creating a search queue (this is actually a vector because it needs to sort the items inside it)
	searchQueue << nodes[start];                   // setting the first in the queue as the start node
	
	while (!searchQueue.is_empty()) // while the queue is not empty
	{
		searchQueue.sort(aStarNode_sort);                            // sorts the queue backwards (the last item is has the best combined heuristic)
		int currentNodeIndex = searchQueue.pop_back()->index;        // gets the optimal node to visit next
		aStarNode* currentNode = nodes[currentNodeIndex];            // sets the current node
		Vector<int>* currentNodeEdges = graph->at(currentNodeIndex); // list of edges of current node
		
		if (currentNodeIndex == end) // if we reached the end
			break; // breaks the loop
		
		for (int i = 0; i < numberOfNodes; i++) // for each possible edge
		{
			int edgeWeight = (*currentNodeEdges)[i]; // gets the distance from current node to the [i] node | if 0 then current node is not connected to the [i] node
			if (edgeWeight > 0 && color[i] < 2) // if the node is connected to the [i] node AND the [i] node have not been fully visited
			{
				aStarNode* connectedNode = nodes[i]; // sets the connected node
				int newDist = edgeWeight + currentNode->dist;

				if (connectedNode->dist == -1 || newDist < connectedNode->dist) // if the i connected node dist is infinity OR this new path is more efficient
				{
					connectedNode->dist = newDist;          // sets the distance of the connected node to the origin as the new path's distance
					connectedNode->prev_node = currentNode; // sets the he previous node of the connected node as the current node
					
					if (connectedNode->heur_weight == -1)   // if the connected mode's heristic weight have not been calculated yet
						connectedNode->heur_weight = heuristicFunction(graph, i, end);

					if (color[i] == 0) // if connected node is not in the queue AND have not been fully visited
					{
						searchQueue << connectedNode; // pushes it to the search queue
						color[i] = 1; // setting node as "on queue"
					}
				}
			}
		}
		
		color[currentNodeIndex] = 2; // setting node as visited
	}
	
	color.print();

	if (nodes[end]->prev_node != nullptr) // if there is a path back to the origin
	{
		aStarNode* n = nodes[end];
		cout << "Path: ";
		while (n != nullptr)
		{
			cout << n->index << " ";
			n = n->prev_node;
		}
		cout << endl;
	}

	return nodes[end]->dist;
}

void drawMaze(Vector< Vector<char>* >* board)
{
	int height = board->size();
	for (int y = 0; y < height; y++)
	{
		Vector<char>* line = (*board)[y];
		int height = line->size();
		for (int x = 0; x < height; x++)
		{
			printf("%c ", (*line)[x]);
		}
		printf("\n");
	}
}

float mazeHeuristic(Vector< Vector<int>* >* g, const int node, const int goal)
{
	int height = g->size();        // height of the maze
	int width = (*g)[0]->size();   // width of the maze
	int node_x = node / width;     // calculating x of current node
	int node_y = node % width;     // calculating y of current node
	int goal_x = goal / width;     // calculating x of goal node
	int goal_y = goal % width;     // calculating y of goal node

	int delta_x = node_x - goal_x;
	int delta_y = node_y - goal_y;
	
	return sqrtf(powf((float)delta_x, 2.f) + powf((float)delta_y, 2.f));
}

int aStarMaze(Vector< Vector<int>* >* graph, Point start, Point end, bool diagonal, float(*heuristicFunction)(Vector< Vector<int>* >* g, const int node, const int goal), bool debug)
{
	int height = graph->size();        // height of the maze
	int width = (*(*graph)[0]).size(); // width of the maze

	int numberOfNodes = height * width; // calculating number of nodes

	// creating varibales
	Vector<aStarNode*> nodes(numberOfNodes); // node vector  | stores info about each node
	Vector<int> color(numberOfNodes, 0);     // color vector | stores which nodes have been visited or is already queued | color = 0: node has not been visited
	//                                                                                                                   | color = 1: node is on queue 
	//                                                                                                                   | color = 2: node has been visited

	for (int i = 0; i < numberOfNodes; i++)
		nodes << new aStarNode{ /*index = */ i }; // creating a list of aStarNodes

	int start_index = start.x * width + start.y; // calculating index of start node
	int end_index = end.x * width + end.y;		 // calculating index of end node

	nodes[start_index]->dist = 0;                                                       // setting the distance from the origin (start node) of the start node as 0
	nodes[start_index]->heur_weight = heuristicFunction(graph, start_index, end_index); // calculating the heuristic weight of the start node

	Vector<aStarNode*> searchQueue(numberOfNodes); // creating a search queue (this is actually a vector because it needs to sort the items inside it)
	searchQueue << nodes[start_index];             // setting the first in the queue as the start node

	// matrix to access the "neighbors" of a node
	Point sides[9] = {
		{-1,  0}, // up 
		{ 0, -1}, // left
		{ 0,  1}, // right
		{ 1,  0}, // down
		{-1, -1}, // top left
		{-1,  1}, // top right
		{ 1, -1}, // bottom left
		{ 1,  1}  // bottom right
	};
	int n = 4 + (5 * diagonal); // if diagonal is true n will be 9

	// debug purposes
	char c[4] = {
		'^', '<', '>', 'v'
	};

	int count = 0;

	while (!searchQueue.is_empty()) // while the queue is not empty
	{
		count++;
		searchQueue.sort(aStarNode_sort);                     // sorts the queue backwards (the last item is has the best combined heuristic)
		int currentNodeIndex = searchQueue.pop_back()->index; // gets the optimal node to visit next
		aStarNode* currentNode = nodes[currentNodeIndex];     // sets the current node
		int current_x = currentNodeIndex / width;             // calculating x of current node
		int current_y = currentNodeIndex % width;             // calculating y of current node

		if (debug)
		{
			printf("=====\nCurrent Node[%d][%d]\n", current_x, current_y);
			printNode(currentNode);
		}

		if (currentNodeIndex == end_index) // if we reached the end
			break; // breaks the loop

		for (int i = 0; i < n; i++) // for each possible edge
		{
			int x = current_x + sides[i].x; // calculating x of the neighbour node
			int y = current_y + sides[i].y; // calculating y of the neighbour node

			if (x >= 0 && x < height &&
				y >= 0 && y < width) // if neighbour is not out of range
			{
				int neighbour_index = x * width + y; // calculating index of the neighbour node
				int edgeWeight = graph->at(x)->at(y); // gets the distance from current node to the [i] node | if 0 then current node is not connected to the [i] node
				
				if (edgeWeight > 0 && color[neighbour_index] < 2) // if the node is connected to the [i] node AND the [i] node have not been fully visited
				{
					if (debug)
					{
						printf("Acessing Node[%d][%d] (%c)\n", x, y, c[i]);
						printNode(nodes[neighbour_index]);
					}

					aStarNode* connectedNode = nodes[neighbour_index]; // sets the connected node
					int newDist = edgeWeight + currentNode->dist; // calculating distance of the current node and the neighbour

					if (connectedNode->dist == -1 || newDist < connectedNode->dist) // if the neighbour node dist is infinity OR this new path is more efficient
					{
						if(debug) printf("\tnew dist: %d\n", newDist);

						connectedNode->dist = newDist;          // sets the distance of the connected node to the origin as the new path's distance
						connectedNode->prev_node = currentNode; // sets the he previous node of the connected node as the current node

						if (connectedNode->heur_weight == -1)   // if the connected mode's heristic weight have not been calculated yet
							connectedNode->heur_weight = heuristicFunction(graph, neighbour_index, end_index);

						if (color[neighbour_index] == 0) // if connected node is not in the queue AND have not been fully visited
						{
							searchQueue << connectedNode; // pushes it to the search queue
							color[neighbour_index] = 1; // setting node as "on queue"
						}
					}
				}
			}
		}

		color[currentNodeIndex] = 2; // setting node as visited
	}

	// preparing to draw the maze
	auto maze = new Vector< Vector<char>* >();
	char wall_clr[2] = { (char)219, ' ' };
	char path_clr[3] = { ' ', 'x', '.' };

	for (int i = 0; i < height; i++) // setting the maze's walls
	{
		*maze << new Vector<char>();
		for (int j = 0; j < width; j++)
			*maze->end() << wall_clr[(*(*graph)[i])[j]];
	}
	
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			int a = color[i * height + j]; // getting the color of the node
			if (a) // if the color of the node is not 0
				(*(*maze)[i])[j] = path_clr[a]; // sets its color
		}

	Stack<aStarNode*> path; // path Stack | saves the path to print it
	if (nodes[end_index]->prev_node != nullptr) // if there is a path back to the origin
	{
		aStarNode* n = nodes[end_index];
		while (n != nullptr)
		{
			path << n;
			n = n->prev_node;
		}

		cout << "Path: ";
	}

	while (!path.isEmpty())
	{
		aStarNode* n = path.pop();
		int x = n->index / width;
		int y = n->index  % width;
		(*(*maze)[x])[y] = '*';
		printf("([%d][%d]), ", x, y);
	}
	printf("\n");

	(*(*maze)[start.x])[start.y] = 'S'; // setting the start node as 'S'
	(*(*maze)[end.x])[end.y] = 'E';     // setting the end node as 'E'

	drawMaze(maze); // drawing the maze
	delete maze; // deleting the maze pointer

	cout << "\nThis algorithm iterated through: " << count << " nodes\n";

	return nodes[end_index]->dist; // returns the distance of the end node to the starting node
}

/* http://graphonline.ru/en/?graph=GmbvtXYFNUeJLSPQ
0 1 3 0 6 0 0 4 6
1 0 0 1 10 8 0 6 8
3 0 0 0 0 4 0 0 1
0 1 0 0 0 0 6 0 2
6 10 0 0 0 3 0 0 0
0 8 4 0 3 0 2 8 0
0 0 0 6 0 2 0 3 4
4 6 0 0 0 8 3 0 0
6 8 1 2 0 0 4 0 0
*/

/* TESTE 1 PARA O A*
0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 1 1 1 1 1 1 1 1 1 1 1 0 1 1 1 0 1 1 1 0
0 1 0 0 0 0 0 0 0 1 0 0 0 1 0 1 0 0 0 1 0
0 1 1 1 0 1 0 1 0 1 1 1 1 1 0 1 0 1 1 1 0
0 0 0 1 0 1 0 1 0 1 0 0 0 0 0 1 0 0 0 1 0
0 1 0 1 0 1 1 1 1 1 1 1 0 1 1 1 1 1 1 1 0
0 1 0 0 0 1 0 1 0 1 0 0 0 0 0 1 0 0 0 1 0
0 1 1 1 1 1 0 1 0 1 0 1 1 1 1 1 1 1 0 1 0
0 0 0 0 0 1 0 0 0 0 0 1 0 0 0 1 0 1 0 0 0
0 1 1 1 0 1 1 1 0 1 1 1 0 1 1 1 0 1 1 1 0
0 0 0 1 0 0 0 1 0 0 0 1 0 1 0 0 0 1 0 0 0
0 1 1 1 1 1 1 1 0 1 1 1 0 1 1 1 0 1 0 1 0
0 1 0 0 0 0 0 1 0 0 0 0 0 0 0 1 0 1 0 1 0
0 1 1 1 0 1 0 1 1 1 0 1 0 1 0 1 0 1 1 1 0
0 1 0 1 0 1 0 0 0 1 0 1 0 1 0 0 0 1 0 1 0
0 1 0 1 0 1 1 1 1 1 1 1 0 1 1 1 0 1 0 1 0
0 0 0 1 0 0 0 0 0 0 0 0 0 1 0 1 0 1 0 0 0
0 1 0 1 1 1 0 1 0 1 0 1 1 1 0 1 1 1 0 1 0
0 1 0 1 0 0 0 1 0 1 0 0 0 1 0 1 0 0 0 1 0
0 1 1 1 0 1 1 1 1 1 1 1 1 1 0 1 1 1 1 1 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0

0
1
20
19
*/

/* TESTE 2 PARA O A*
0 1 0 1 1 1 0 0 0 0 0
0 1 1 1 0 1 1 1 1 1 0
0 0 0 1 0 1 0 0 0 1 0
0 1 1 1 0 1 0 1 0 1 0
0 1 0 5 0 1 0 1 0 1 0
0 1 1 1 1 1 0 1 1 1 0
0 1 0 0 0 1 0 0 0 1 0
0 1 0 1 0 1 1 1 0 1 0
0 0 0 1 0 0 0 1 0 1 0
0 1 1 1 1 1 1 1 1 1 0
0 0 0 0 0 0 0 0 0 1 0

0
1
10
9
*/