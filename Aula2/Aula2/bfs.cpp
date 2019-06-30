#include <iostream>
#include "Vector.h"

#define pause system("PAUSE")

using namespace std;

int main()
{
	auto graph = new Vector<int>(1);
	
	cout << "Vector Capacity: " << graph->capacity() << endl;
	cout << "Vector Size: " << graph->size() << endl << endl;

	cout << "Inserting 15\n";
	*graph << 15;
	cout << "End: " << graph->end() << endl;
	cout << "Vector Size: " << graph->size() << endl << endl;

	try 
	{
		cout << "Inserting 55 at 1\n";
		graph->set_at(1, 55);
		cout << "End: " << graph->end() << endl;
		cout << "Vector Size: " << graph->size() << endl << endl;
		cout << "Inserting 33 at 3\n";
		graph->set_at(3, 33);
		cout << "End: " << graph->end() << endl;
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
	return 0;
}