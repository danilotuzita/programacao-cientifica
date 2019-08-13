#include "Graphs.h"
#include "Render.h"
#define pause system("PAUSE")

using namespace std;

int input()
{
	int f;
	system("CLS");
	cout << "SELECT AN ALGORITHM: \n";
	cout << "[0] - BFS           | INPUT: ADJACENCY MATRIX [AUTO GENERATED] | OUTPUT: MIN DISTANCE BETWEEN 2 VERTICES\n";
	cout << "[1] - DFS           | INPUT: MAZE                              | OUTPUT: DISTANCE TO EACH VERTEX\n";
	cout << "[2] - HILL CLIMBING | INPUT: CITIES [AUTO GENERATED]           | OUTPUT: SOLUTION TO TRAVELLING SALESMAN\n";
	cout << "[3] - A*            | INPUT: MAZE                              | OUTPUT: SOLUTION TO MAZE (DJIKSTRA AND EUCLIDIAN DISTANCE HEURISTIC)\n";
	cout << "[ANY OTHER] - QUIT\n";
	cin >> f;
	return f;
}

// == unitary test functions == //
void rend()
{
	Render r("a", 500, 500, 10, 50, 50);
	auto cities = randomPoints(10, 500, 500);
	auto dist = distances(cities);

	for (int i = 0; i < cities->size(); i++)
	{
		int x = cities->at(i).x;
		int y = cities->at(i).y;
		r.point(x, y, 2, to_string(i) + " (" + to_string(x) + ", " + to_string(y) + ")");
	}

	r.open_html();
	delete cities;
}

void resize()
{
	Vector<int> v(15, 0);
	v.print_size();
	v.print();
	printf("\n");

	v.reserve(2);
	v.print_size();
	v.print();
	printf("\n");

	v.reserve(20);
	v.print_size();
	v.print();
	printf("\n");

	v.resize(5, 2222);
	v.print_size();
	v.print();
	printf("\n");

	v.resize(15, 1111);
	v.print_size();
	v.print();
	printf("\n");

	v.resize(5);
	v.print_size();
	v.print();
	printf("\n");

	v << 3333;
	v.print_size();
	v.print();
	printf("\n");

	v.resize(7);
	v.print_size();
	v.print();
	printf("\n");
}

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
		f.push_back(i * 1.08f);

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
	resize();
	
	vector_vector();
	_operator();

	Vector<int> teste(10, 5);
	teste.print();

	auto graph = input_adjmatrix();
	equal_operator(*graph);

	sort_test();
}

void bfs()
{
	cout << " ===== BFS ALGORITHM =====\n";
	cout << "Generating adjaceny matrix here: \n";
	auto graph = generate_adjmatrix(15, 0.2);
	bool _continue = true;
	bool debug = false;
	while (_continue)
	{
		int s, e;
		s = input_node(graph->size(), "Starting node: ");
		e = input_node(graph->size(), "End node: ");

		clock_t tStartBFS = clock();
		auto distanceBFS = bfsShortestPath(*graph, s, e, debug);
		clock_t deltaBFS = clock() - tStartBFS;
		cout << "The distance of the node " << s << " to the node " << e << " is: " << distanceBFS << endl;
		printf("This algorithm took %d ticks (%f s)\n\n", deltaBFS, ((float)deltaBFS) / CLOCKS_PER_SEC);

		pause;
		cout << "Try another node? ";
		cin >> _continue;
	}

	cout << "\n\n\n\n";
	delete graph;
}

void dfs()
{
	cout << " ===== DFS ALGORITHM =====\n";
	cout << "Paste your maze here: \n";
	auto graph = input_maze();
	bool _continue = true;
	bool debug = false;
	while (_continue)
	{
		Point s, e;
		s.x = input_node(graph->size(), "Starting X: ");
		s.y = input_node(graph->size(), "Starting Y: ");
		e.x = input_node(graph->size(), "End X:");
		e.y = input_node(graph->size(), "End Y:");

		clock_t tStartDFS = clock();
		auto distanceDFS = dfsMaze(graph, s, false, debug);
		clock_t deltaDFS = clock() - tStartDFS;
		cout << "The distance of the node [" << s.x << ", "<< s.y << "] to the node [" << e.x << ", " << e.y << "]  is: " << (*(*distanceDFS)[e.x])[e.y] << endl;
		printf("This algorithm took %d ticks (%f s)\n\n", deltaDFS, ((float)deltaDFS) / CLOCKS_PER_SEC);

		delete distanceDFS;
		pause;
		cout << "Try another node? ";
		cin >> _continue;
	}

	cout << "\n\n\n\n";
	delete graph;
}

void hill_climbing()
{
	cout << " ===== HILL CLIMBING ALGORITHM =====\n";
	bool _continue = true;
	bool debug = false;
	while (_continue)
	{
		int numberOfCities;
		cout << "Insert number of cities to be generated(Recomended 25): ";
		cin >> numberOfCities;

		int width = numberOfCities * 30;
		int height = numberOfCities * 30;
		int guesses = numberOfCities * 5000;

		auto cities = randomPoints(numberOfCities, width, height);
		double dist = hillClimbingRestart(cities, swapCities, width, height, guesses, minimize, debug);
		printf("Total distance: %lf\n", dist);

		delete cities;
		pause;
		cout << "Try again? ";
		cin >> _continue;
	}

	cout << "\n\n\n\n";
}

void a_star_maze()
{
	cout << " ===== A* ALGORITHM =====\n";
	cout << "Paste your maze here: \n";
	auto graph = input_maze();
	
	bool _continue = true;
	bool debug = false;
	while (_continue)
	{
		Point s, e;
		s.x = input_node(graph->size(), "Starting X: ");
		s.y = input_node(graph->size(), "Starting Y: ");
		e.x = input_node(graph->size(), "End X:");
		e.y = input_node(graph->size(), "End Y:");

		cout << " ===== NO HEURISTIC (DJIKSTRA ALGORITHM)\n";
		clock_t tStartD = clock();
		auto dj = aStarMaze(graph, s, e, false, dijkstra, debug);
		clock_t deltaD = clock() - tStartD;
		cout << "The distance of the node [" << s.x << ", " << s.y << "] to the node [" << e.x << ", " << e.y << "]  is: " << dj << endl;
		printf("This algorithm took %d ticks (%f s)\n\n", deltaD, ((float)deltaD) / CLOCKS_PER_SEC);

		cout << " ===== EUCLIDIAN DISTANCE HEURISTIC\n";
		clock_t tStartA = clock();
		auto d = aStarMaze(graph, s, e, false, mazeHeuristic, debug);
		clock_t deltaA = clock() - tStartA;
		cout << "The distance of the node [" << s.x << ", " << s.y << "] to the node [" << e.x << ", " << e.y << "]  is: " << dj << endl;
		printf("This algorithm took %d ticks (%f s)\n\n", deltaA, ((float)deltaA) / CLOCKS_PER_SEC);
		
		pause;
		cout << "Try another node? ";
		cin >> _continue;
	}

	cout << "\n\n\n\n";
	delete graph;
}

void a_star()
{
	//auto graph = generate_adjmatrix(9, 0.5, false, 10);

	cout << "Paste your adjaceny matrix here: \n";
	auto graph = input_adjmatrix();
	//auto graph = generate_adjmatrix_from_maze();

	bool _continue = true;
	while (_continue)
	{
		int s, e;
		s = input_node(graph->size(), "Starting node: ");
		e = input_node(graph->size(), "End node: ");

		cout << " ===== DJIKSTRA ALGORITHM =====\n";
		clock_t tStartD = clock();
		auto dj = aStar(graph, s, e, dijkstra);
		clock_t deltaD = clock() - tStartD;
		cout << "The distance of the node " << s << " to the node " << e << " is: " << dj << endl;
		printf("This algorithm took %d ticks (%f s)\n\n", deltaD, ((float)deltaD) / CLOCKS_PER_SEC);


		cout << " ===== A* ALGORITHM =====\n";
		clock_t tStartA = clock();
		auto d = aStar(graph, s, e, simpleHeuristic);
		clock_t deltaA = clock() - tStartA;
		cout << "The distance of the node " << s << " to the node " << e << " is: " << d << endl;
		printf("This algorithm took %d ticks (%f s)\n\n", deltaA, ((float)deltaA) / CLOCKS_PER_SEC);

		pause;
		cout << "Try another node? ";
		cin >> _continue;
	}
}

typedef void(*fncs)();

Vector<char[3][3]> generateBoards(char b[3][3])
{
	Vector<char[3][3]> ans;
	return ans;
}


void printBoard(char b[3][3])
{
	cout << b << endl;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			printf("%d ", b[i][j]);
		}
		printf("\n");
	}
}

int main()
{
	/*while (true)
	{
		rend();
		pause;
	}*/
	//auto g = generate_adjmatrix_from_maze();
	/*while (true)
	{
		system("CLS");
		for (int i = 0; i < 50; i++)
			generateRoute(10).print();
		pause;
	}*/
	//tests();

	/*
	fncs functions[] = {bfs, dfs, hill_climbing, a_star_maze};
	
	int f = input();	

	while (f >= 0 && f <= 3)
	{
		functions[f]();
		f = input();
	}
	*/
	
	/*
	bfs();
	dfs();
	//a_star();
	hill_climbing();
	a_star_maze();	
	*/

	char board[3][3];

	for (char i = 0; i < 3; i++)
		for (char j = 0; j < 3; j++)
			board[i][j] = i * 3 + j + 1;
	
	board[1][1] = 0;
	cout << &board << endl;
	printBoard(board);

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