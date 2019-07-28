#include "Graphs.h"

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

void sort_test()
{
	cout << "int\n";
	Vector<int> v(10);

	for (int i = 10; i > 0; i--)
		v.push_back(i);

	v.print();
	v.sort();
	v.print();

	cout << "float\n";
	Vector<float> f(10);

	for (int i = 10; i > 0; i--)
		f.push_back(i * 1.08);

	f.print();
	f.sort();
	f.print();

	cout << "double\n";
	Vector<double> d(10);

	for (int i = 10; i > 0; i--)
		d.push_back(i * .8);

	d.print();
	d.sort(false);
	d.print();

	cout << "char\n";
	Vector<char> c(10);

	for (int i = 10; i > 0; i--)
		c.push_back(i + 64);

	c.print();
	c.sort();
	c.print();
}

// == unitary test functions == //

// UNITARY TESTS
void tests()
{
	vector_vector();
	_operator();

	Vector<int> teste(10, 5);
	teste.print();

	auto graph = input_adjmatrix();
	equal_operator(*graph);

	sort_test();
}

void bfs_dfs()
{
	bool debug = true;
	
	cout << "Generating adjaceny matrix here: \n";
	auto graph = generate_adjmatrix(15, 0.2);
	bool _continue = true;
	while (_continue)
	{
		int s, e;
		s = input_node(graph->size(), "Starting");
		e = input_node(graph->size(), "End");

		cout << " ===== BFS ALGORITHM =====\n";
		clock_t tStartBFS = clock();
		auto distanceBFS = bfs_shortest_path(*graph, s, e, debug);
		clock_t deltaBFS = clock() - tStartBFS;
		cout << "The distance of the node " << s << " to the node " << e << " is: " << distanceBFS << endl;
		printf("This algorithm took %d ticks (%f s)\n\n", deltaBFS, ((float)deltaBFS) / CLOCKS_PER_SEC);

		cout << " ===== DFS ALGORITHM =====\n";
		clock_t tStartDFS = clock();
		auto distanceDFS = dfs_shortest_path(*graph, s, e, debug);
		clock_t deltaDFS = clock() - tStartDFS;
		cout << "The distance of the node " << s << " to the node " << e << " is: " << distanceDFS << endl;
		printf("This algorithm took %d ticks (%f s)\n\n", deltaDFS, ((float)deltaDFS) / CLOCKS_PER_SEC);

		pause;
		cout << "Try another node? ";
		cin >> _continue;
	}

	delete graph;
}

void a_star()
{
	//auto graph = generate_adjmatrix(9, 0.5, false, 10);
	
	cout << "Paste your adjaceny matrix here: \n";
	//auto graph = input_adjmatrix();
	auto graph = generate_adjmatrix_from_maze();

	bool _continue = true;
	while (_continue)
	{
		int s, e;
		s = input_node(graph->size(), "Starting");
		e = input_node(graph->size(), "End");

		cout << " ===== A* ALGORITHM =====\n";
		clock_t tStartA = clock();
		auto d = aStar(graph, s, e, simpleHeuristic);
		clock_t deltaA = clock() - tStartA;
		cout << "The distance of the node " << s << " to the node " << e << " is: " << d << endl;
		printf("This algorithm took %d ticks (%f s)\n\n", d, ((float)deltaA) / CLOCKS_PER_SEC);
		
		pause;
		cout << "Try another node? ";
		cin >> _continue;
	}
}

int main()
{
	//tests();

	//bfs_dfs();
	a_star();
	//auto g = generate_adjmatrix_from_maze();
	
	pause;
	return 0;
}

/* GRAPH EXAMPLES - https://visualgo.net/en/graphds
0 1 1 0 0 0 0 0
0 0 1 1 0 0 0 0
0 0 0 1 0 1 0 0
0 0 0 0 1 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 1 0

0 1 2 3 4
0 1 10 6 5
1 0 2 7 8
10 2 0 3 9
6 7 3 0 4
5 8 9 4 0

0 2 7 0 0
0 0 4 0 0
0 0 0 1 4
0 0 0 0 2
0 0 0 0 0

0 2 2 0 0
0 0 0 3 2
0 2 0 0 0
0 0 0 0 0
0 0 0 2 0
*/