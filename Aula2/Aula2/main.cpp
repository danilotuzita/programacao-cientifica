#include <iostream>
#include "Vector.hpp"

#define pause system("PAUSE")

using namespace std;

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

Vector< Vector<int>* >* adjlist_to_graph()
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
		item->print();
		graph->push_back(item);
		getline(cin, line);
	}
	
	graph->shrink_to_fit();
	return graph;
}

Vector<int> bfs(Vector< Vector<int>* > graph, int start)
{
	Vector<int> dist(1); // distance vector
	dist.resize(graph.size(), 0); // filling the dist with zeros
	Queue<int> q; // search queue
	
	int index = start;
	Vector<int>* currentNode = graph[index]; // accessing start node
	for (int i = 0; i < currentNode->size(); i++) // for each connected node of the starting node
		q << (*currentNode)[i]; // pushes it to the search queue

	while (!q.isEmpty())
	{
	}

	return dist;
}


int main()
{
	//vector_vector();
	//_operator();
	
	//Vector<int> teste(10, 5);
	//teste.print();

	auto graph = adjlist_to_graph();
	auto distances = bfs(*graph, 0);


	pause;
	return 0;
}

/* GRAPH
1 2
0 2 3 7
0 1 3 4 6 7
1 2 4
2 3 5
4 6 7
2 5 7
1 2 5 6
*/