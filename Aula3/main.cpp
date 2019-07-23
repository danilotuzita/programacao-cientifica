#include <iostream>
#include <Vector.hpp>
#include <Queue.hpp>
#include <time.h>

#define pause system("PAUSE")

using namespace std;

// == unitary test functions == //

void _operator()
{
	auto graph = new Vector<int>(10);

	//Vector<int> graph(1);
	cout << "for" << endl;

	try
	{
		for (int i = 0; i < 10; i++)
			(*graph)[i] = i;
	}
	catch (const std::out_of_range& e)
	{
		cout << e.what() << endl << endl;
	}

	graph->print();

	delete graph;
	pause;
}

void vector()
{
	auto graph = new Vector<int>(1);
	graph->print();

	cout << "Vector Capacity: " << graph->capacity() << endl;
	cout << "Vector Size: " << graph->size() << endl << endl;

	cout << "Inserting 15\n";
	*graph << 15;
	graph->print();
	cout << "Vector Size: " << graph->size() << endl << endl;

	try
	{
		cout << "Inserting 55 at 1\n";
		graph->set_at(1, 55);
		graph->print();
		cout << "Vector Size: " << graph->size() << endl << endl;
		cout << "Inserting 33 at 3\n";
		graph->set_at(3, 33);
		graph->print();
		cout << "Vector Size: " << graph->size() << endl << endl;
	}
	catch (const std::out_of_range& e)
	{
		cout << e.what() << endl << endl;
	}

	cout << "graph->at(0): " << graph->at(0) << endl;
	cout << "Vector Capacity: " << graph->capacity() << endl;
	cout << "Vector Size: " << graph->size() << endl << endl;
	pause;
	delete graph;
	pause;
}

void vector_vector()
{
	auto graph = new Vector< Vector<int>* >(5);
	try
	{
		for (int i = 0; i < 10; i++)
		{
			cout << "     ====== " << i << " ======     \n";
			cout << "Creating new Item\n";
			auto item = new Vector<int>(2);

			cout << "Pushing back 0 to 5\n";
			for (int j = 0; j < 10 - i; j++)
				(*item) << j;

			cout << "Printing item\n";
			item->print();

			cout << "Shrinking item\n";
			item->shrink_to_fit();

			cout << "Pushing it to the graph\n";
			graph->push_back(item);

			cout << "Printing graph size: ";
			graph->print_size();
		}

		cout << "Printing graph:";
		for (int at = 0; at < 11; at++)
		{
			cout << "[" << at << "] ";
			(*graph)[at]->print();
		}
	}
	catch (const std::out_of_range& e)
	{
		cout << e.what() << endl << endl;
	}
	delete graph;
	pause;
}

void equal_operator(Vector< Vector<int>* > graph)
{
	Vector< Vector<int>* > graph2;
	graph2 = graph;
	graph2[3]->print();
	graph2[1]->print();
}

// == unitary test functions == //

// input matrix function
Vector< Vector<int>* >* input_adjmatrix()
{
	auto graph = new Vector< Vector<int>* >();
	string line;

	cout << "Press enter 2 times to end graph\n";
	getline(cin, line);
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
		getline(cin, line);
	}

	graph->shrink_to_fit();
	return graph;
}

int input_node(int size, string prefix="Starting")
{
	int ret;
	cout << prefix << " node: ";
	cin >> ret;
	while (ret > size || ret < 0)
	{
		cout << "The node must be between 0 and " << size << "\n";
		cout << prefix << " node: ";
		cin >> ret;
	}
	return ret;
}

// Returns a distance vector
Vector<int> bfs_distances(Vector< Vector<int>* > graph, int start) // graph must be adjacency matrix
{
	// creating varibales
	Vector<int> dist(graph.size(), -1);      // distance vector | stores the distance to reach each node from starting node | dist = -1: cannot reach from starting node
	Vector<bool> color(graph.size(), false); // color vector    | stores which nodes have been visited | color = false: node has not been visited
	Queue<int> q;                            // search queue    | stores the index of the nodes to search next

	Vector<int>* currentNode; // current node
	int index;                // current node index

	dist[start] = 0; // setting distance of the starting node to the starting node as 0
	q << start; // pushing start node to the search queue

	while (!q.isEmpty()) // while search queue is not empty
	{
		index = q.pop(); // gets the index of the next node to search
		currentNode = graph[index]; // setting the currentNode as the [index] node
		
		for (int i = 0; i < currentNode->size(); i++) // for each possibly connected node
		{
			if ((*currentNode)[i] && !color[i]) // if the current node is connected to [i] node and has not been visited
			{
				q << i;                    // pushes [i] node to the search queue
				color[i] = true;           // sets the [i] node as visited
				dist[i] = dist[index] + 1; // sets the [i] node distance to the [start]node as the distance of this node's father plus one
			}
		}
	}
	//dist.reserve(15);
	//dist.print();
	//auto d = new Vector<int>(dist);
	//d->push_back(999);
	//d->print();
	return dist; // returning dist vector
}

// Returns the shortest path
int bfs_shortest_path(Vector< Vector<int>* > graph, int start, int end) // graph must be adjacency matrix
{
	Vector<int> dist;
	dist = bfs_distances(graph, start);
	return dist[end];
}

int main()
{
	//vector_vector();
	//_operator();

	//Vector<int> teste(10, 5);
	//teste.print();

	//auto graph = input_adjmatrix();
	//equal_operator(*graph);
	cout << "Paste your adjaceny matrix here: \n";
	auto graph = input_adjmatrix();
	bool _continue = true;
	while (_continue)
	{
		int s, e;
		s = input_node(graph->size(), "Starting");
		e = input_node(graph->size(), "End");

		cout << " ===== BFS ALGORITHM =====\n";
		clock_t tStart = clock();
		auto distance = bfs_shortest_path(*graph, s, e);
		clock_t delta = clock() - tStart;
		cout << "The distance of the node " << s << " to the node " << e << " is: " << distance << endl;
		printf("This algorithm took %d ticks (%f s)\n", delta, ((float)delta) / CLOCKS_PER_SEC);
		
		pause;
		cout << "Try another node? ";
		cin >> _continue;
	}
	delete graph;
	return 0;
}

/* GRAPH - https://visualgo.net/en/graphds (DIRECTED / UNWEIGHED)
0 1 1 0 0 0 0 0
0 0 1 1 0 0 0 0
0 0 0 1 0 1 0 0
0 0 0 0 1 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 1 0
*/