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
	
	// INTERNAL FUNCTIONS
	void _reserve(int newSize); // reserves a new list (may be more or less than the old maxSize)
	void realloc_if_needed();   // reserves more memory if the user needs it

public:
	// CONSTRUCTOR/DESTRUCTOR
	Vector(int max=10);
	inline ~Vector() { delete[] list; }; // deletes list from memory
	
	// MEMORY UTILS
	void reserve(int newSize);           // reserves MORE memory for the list
	void resize(int newSize, T value=0); // resizes the vector and fills the new memory spaces with 'value'
	void shrink_to_fit();                // resizes the capacity of the vetor to fit its size

	// UTIL
	inline int size()     { return _size + 1; };              // gets the real size of the vector
	inline int capacity() { return maxSize; };                // returns the capacity of the vector (current maxSize)
	inline bool empty()   { return (size() == 0); };          // check if the vector is empty
	inline bool is_full() { return (_size == maxSize - 1); }; // checks if the vector is full

	// BASIC FUNCTIONS
	// definition functions
	void push_back(T value);         // pushes an item at the end of the vector
	void set_at(int index, T value); // sets a value in a specific index; NOTE: calls LHS operator[]
	T pop_back();                    // pops the last value and returns it

	// access functions
	T at(int index) const; // returns the item at index

	inline T end() { return at(_size); };   // returns the last item
	inline T begin() { return at(0); };     // returns the first item
	
		// operators
		inline void operator<<(T value)            { push_back(value); }; // same as push_back
		inline T operator[](const int index) const { return at(index); }; // right-hand side operator[] (RHS)
		T& operator[](const int index);                                   //  left-hand side operator[] (LHS)

		// debug functions
		void print();
		void print_size();
};

template <class T>
Vector<T>::Vector(int max)
{
	if (max < 1) // cannot allocate less than 1 items
		throw std::bad_alloc();

	_size = -1;    // setting the current size of the list
	maxSize = max; // setting the max size of the list
	list = new T[maxSize]; // allocating list
}

// === MEMORY UTIL === //
template<class T>
void Vector<T>::_reserve(int newSize) // Reallocates the list size (Internal-use)
{
	//std::cout << "Reserving Space; From " << maxSize << " to " << newSize << "\n";
	if (newSize < 1)
		throw std::bad_array_new_length();

	T* newList = new T[newSize];    // creating a new list that fits the new size
	for (int i = 0; i < size(); i++) // copying each item from the old list to the new list
		newList[i] = list[i];

	delete[] list;  // deleting the old list from memory
	list = newList; // setting the new list as the current list
	maxSize = newSize; // saving the new size
}

template<class T>
void Vector<T>::reserve(int newSize) // Reallocates the list size (User-use)
{
	if (newSize > maxSize) // checking if the user wants more space
		_reserve(newSize); // reserves more memory
}

template<class T>
void Vector<T>::resize(int newSize, T value)
{
	_reserve(newSize); // resizing the vector to the new size
	for (int i = size(); i < newSize; i++) // populating the new 
		list[i] = value;
}

template<class T>
inline void Vector<T>::realloc_if_needed()
{
	if (_size > maxSize - 1) // if user needs more space
		_reserve(maxSize * maxSize + (maxSize == 1)); // allocates more memory
	// NOTE: (maxSize == 1) fixes a bad allocation if maxSize = 1
}

template <class T>
void Vector<T>::shrink_to_fit()
{
	if (size() < 1)
		throw std::bad_array_new_length();

	_reserve(size());
}

// === BASIC FUNCTIONS === //
template<class T>
void Vector<T>::push_back(T value)
{
	_size++;
	realloc_if_needed(); // check if the vector needs more memory
	list[_size] = value; // sets the item on the vector
}

template<class T>
void Vector<T>::set_at(int index, T value) { (*this)[index] = value; } // calling LHS operator[]

template<class T>
T Vector<T>::pop_back()
{
	if (empty()) // if the vector is empty
		throw std::out_of_range("Vector is empty");

	return at(_size--); // returns the last item and reduce it's size
}

template<class T>
T Vector<T>::at(int index) const
{
	if (index > _size || index < 0)
		throw std::out_of_range("Item index [" + std::to_string(index) + "] does not exist");

	return list[index];
}

template<class T>
void Vector<T>::print()
{
	std::cout << "[ ";
	for (int i = 0; i < maxSize; i++)
	{
		if (i < size())
			std::cout << at(i) << " ";
		else
			std::cout << "# ";
	}
	std::cout << "]\n";
}

template<class T>
inline void Vector<T>::print_size()
{
	std::cout << "(" << size() << "/" << capacity() << ")\n";
}

// OPERATORS
// left-hand side operator[]
template<class T>
T &Vector<T>::operator[](const int index)
{
	if (index > size() - 1 || index < 0)
		throw std::out_of_range("Item index [" + std::to_string(index) + "] does not exist");

	return list[index];
}
