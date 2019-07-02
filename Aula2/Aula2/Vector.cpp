/*
#include "Vector.h"

template <class T>
Vector<T>::Vector(int max)
{
	if (max < 1)
		throw std::bad_alloc();

	_size = -1;    // setting the current size of the list
	maxSize = max; // setting the max size of the list
	list = new T[maxSize]; // allocating list
}

template <class T>
Vector<T>::~Vector() { delete[] list; } // deleting the list from memory

// === MEMORY UTIL === //
template<class T>
void Vector<T>::_reserve(int newSize) // Reallocates the list size (Internal-use)
{
	std::cout << "Reserving more space from: " << maxSize << " to: " << newSize << "\n";
	if (newSize < 1)
		throw std::bad_array_new_length();

	T* newList = new T[newSize];    // creating a new list that fits the new size
	for (int i = 0; i < _size; i++) // copying each item from the old list to the new list
		newList[i] = list[i];
	
	delete[] list;  // deleting the old list from memory
	list = newList; // setting the new list as the current list
	maxSize = newSize; // saving the new size
}

template<class T>
void Vector<T>::reserve(int newSize) // Reallocates the list size (User-use)
{
	if (newSize > maxSize) // checking if the user wants more space
		_reserve(newSize);
}

template<class T>
void Vector<T>::resize(int newSize, T value)
{
	_reserve(newSize); // resizing the vector to the new size
	for (int i = size(); i < newSize; i++) // populating the new 
		list[i] = value;
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
	if (++_size > maxSize - 1) // if user needs more space
		_reserve(maxSize * maxSize + (maxSize == 1)); // allocates more memory
	// NOTE: (maxSize == 1) fixes a bad allocation if maxSize = 1

	list[_size] = value; // sets the item on the vector
}

template<class T>
void Vector<T>::set_at(int index, T value)
{
	if (index > size() || index < 0)
		throw std::out_of_range("Item index [" + std::to_string(index) + "] does not exist, you may want to resize first");

	if (index == size()) // check if user wants to create a new item
		push_back(value);
	else // otherwise, overwrites the list[index]
		list[index] = value;
}

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
T Vector<T>::end() { return at(_size); }

template<class T>
T Vector<T>::begin() { return at(0); }

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

//		operators
template<class T>
void Vector<T>::operator<<(T value) { push_back(value); }

template<class T>
T Vector<T>::operator[](int index) const { return at(index); }

//template<class T>
//T &Vector<T>::operator[](int index) { return at(index); }

// === UTIL === //
template<class T>
int Vector<T>::size() { return _size + 1; } // real size of the vector

template<class T>
int Vector<T>::capacity() { return maxSize; }

template<class T>
bool Vector<T>::empty() { return (size() == 0); }

template<class T>
bool Vector<T>::is_full() { return (_size == maxSize - 1); }


template class Vector<int>;
template class Vector<float>;
template class Vector<double>;
template class Vector<char>;
template class Vector<bool>;
template class Vector<std::string>;
*/