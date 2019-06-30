#pragma once
#include <vector>

template <class T>
class Node
{
private:
	char name[32];
	T value;
	std::vector<int> connections;
public:
	Node();
	~Node();
};

