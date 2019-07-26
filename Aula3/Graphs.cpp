#include "Graphs.h"

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

int input_node(int size, string prefix)
{
	int ret;
	cout << prefix << " node: ";
	cin >> ret;
	while (ret >= size || ret < 0)
	{
		cout << "The node must be between 0 and " << size - 1 << "\n";
		cout << prefix << " node: ";
		cin >> ret;
	}
	return ret;
}

// ====== BFS ====== //

// Returns a distance vector
Vector<int> bfs_distances(Vector< Vector<int>* > graph, int start, bool debug) // graph must be adjacency matrix
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
int bfs_shortest_path(Vector< Vector<int>* > graph, int start, int end, bool debug) // graph must be adjacency matrix
{
	Vector<int> dist;
	dist = bfs_distances(graph, start, debug);
	return dist[end];
}

// ====== DFS ====== //

Vector<int> dfs_distances(Vector< Vector<int>* > graph, int start, bool debug) // graph must be adjacency matrix
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

// Returns the shortest path
int dfs_shortest_path(Vector< Vector<int>* > graph, int start, int end, bool debug) // graph must be adjacency matrix
{
	Vector<int> dist;
	dist = dfs_distances(graph, start, debug);
	return dist[end];
}


// ====== A* ====== //

typedef struct aStarNodeT
{
	int index = -1;
	float dist = -1;
	float heur_weight = -1;
	aStarNodeT* prev_node = nullptr;
} aStarNode;

void printNode(aStarNode* node)
{
	if (node != nullptr)
		if (node->prev_node != nullptr)
			printf(
				"Node [%d]\n"
				"	dist: %d\n"
				"	heur: %d\n"
				"	prev: %d\n",
				node->index, node->dist, node->heur_weight, node->prev_node->index
			);
		else
			printf(
				"Node [%d]\n"
				"	dist: %d\n"
				"	heur: %d\n"
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


float simpleHeuristic(Vector< Vector<int>* >* g, const int node, const int goal)
{
	return 0;
}

float aStar(Vector< Vector<int>* >* graph, int start, int end, float (*heuristicFunction)(Vector< Vector<int>* >* g, const int node, const int goal))
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
		
		cout << "searchQueue: [\n" << currentNode->index << " " << currentNode->dist + currentNode->heur_weight << endl;
		for (int i = searchQueue.size() - 1; i > -1; i--)
			cout << searchQueue[i]->index << " " << searchQueue[i]->dist + searchQueue[i]->heur_weight << endl;
		cout << "]\nCurrentNode: " << currentNodeIndex << endl;

		if (currentNodeIndex == end) // if we reached the end
			break; // breaks the loop

		for (int i = 0; i < numberOfNodes; i++) // for each possible edge
		{
			int edgeWeight = (*currentNodeEdges)[i]; // gets the distance from current node to the [i] node | if 0 then current node is not connected to the [i] node
			
			if (edgeWeight > 0 && color[i] < 2) // if the node is connected to the [i] node AND the [i] node have not been fully visited
			{
				aStarNode* connectedNode = nodes[i]; // sets the connected node
				int newDist = edgeWeight + currentNode->dist;

				printNode(connectedNode);
				printf("	ndst: %d\n", newDist);
				
				if (connectedNode->dist == -1 || connectedNode->dist > newDist) // if the i connected node dist is infinity OR this new path is more efficient
				{
					connectedNode->dist = newDist;          // sets the distance of the connected node to the origin as the new path's distance
					connectedNode->prev_node = currentNode; // sets the he previous node of the connected node as the current node
					
					if (connectedNode->heur_weight == -1)   // if the connected mode's heristic weight have not been calculated yet
						connectedNode->heur_weight = heuristicFunction(graph, currentNodeIndex, end);

					if (color[i] == 0) // if connected node is not in the queue AND have been fully visited
						searchQueue << connectedNode; // pushes it to the search queue
				}
			}
		}
	}
	
	if (nodes[end]->prev_node != nullptr) // if there is a path back to the origin
	{
		aStarNode* n = nodes[end];
		cout << "Path: ";
		while (n->prev_node)
		{
			cout << n->index << " ";
			n = n->prev_node;
		}
		cout << endl;
	}

	/*
	for (int i = searchQueue.size(); i > -1; i--)
		printNode(nodes[i]);
	*/

	return nodes[end]->dist;
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