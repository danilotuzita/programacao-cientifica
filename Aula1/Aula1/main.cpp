#include <iostream>
#include "Stack.hpp"

using namespace std;

int stack()
{
	cout << "Teste STACK: \n";
	Stack<string> boolStack;

	string val;
	cout << "Digite um valor inteiro(0 para parar): ";
	cin >> val;

	while (val != "0")
	{
		cout << "Inserindo '" << val << "' na pilha\n";
		boolStack << val;
		cout << "Topo da pilha agora eh: " << boolStack.top() <<endl;

		cout << "Digite um valor inteiro(0 para parar): ";
		cin >> val;
	}

	cout << "O tamanho da pilha eh: " << boolStack.count() << endl;
	while (!boolStack.isEmpty())
		cout << "Desemplihando topo: " << --boolStack << endl;


	system("pause");
	return 0;
}


int main()
{
	return stack();
}