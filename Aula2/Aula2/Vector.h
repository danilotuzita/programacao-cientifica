#pragma once
#include <stdexcept>
#include <string>
#include <iostream>

template <class T>
class Vector
{
private:
	int _size;   // points to the index of the last item included
	int maxSize; // stores the size alocated for the list
	T* list;     // stores the pointer to the start of the list
	
	// internal functions
	void _reserve(int newSize); // reserves a new list (may be more or less than the old maxSize)

public:
	Vector(int max=10);
	~Vector();
	
	// memory util
	void reserve(int newSize);  // reserves MORE memory for the list
	void resize(int newSize, T value=0); // resizes the vector and fills the new memory spaces with 'value'
	void shrink_to_fit(); // resizes the capacity of the vetor to fit its size
	int capacity(); // returns the capacity of the vector (current maxSize)

	// util
	int size();     // get the index of the last item in the vetor (AKA: size - 1)
	bool empty();   // check if the vector is empty
	bool is_full(); // checks if the vector is full

	// basic functions
	void push_back(T value); // pushs a item in the end of the vector
	void set_at(int index, T value); // sets a value in specific index
	T pop_back(); // pops the last value
	T at(int index) const; // returns the item at index []

	T end();   // returns the last item
	T begin(); // returns the first item

		// operators
		void operator<<(T value);
		T operator[](int index) const;
		T &operator[](int index);
};
