#include <iostream>
#include "Stack.hpp"
#include "Queue.hpp"

using namespace std;

int stack()
{
	cout << "Teste STACK\n";
	Stack<string> boolStack;

	string val;
	cout << "Digite um valor (0 para parar): ";
	cin >> val;

	while (val != "0")
	{
		cout << "Inserindo '" << val << "' na pilha\n";
		boolStack << val;
		cout << "Topo da pilha agora eh: " << boolStack.top() <<endl;

		cout << "Digite um valor (0 para parar): ";
		cin >> val;
	}

	cout << "O tamanho da pilha eh: " << boolStack.count() << endl;
	while (!boolStack.isEmpty())
		cout << "Desemplihando topo: " << --boolStack << endl;


	system("pause");
	return 0;
}

int queue()
{
	cout << "Teste QUEUE\n";
	Queue<string> boolStack;

	string val;
	cout << "Digite um valor (0 para parar): ";
	cin >> val;

	while (val != "0")
	{
		cout << "Inserindo '" << val << "' na fila\n";
		boolStack << val;
		cout << "O primeiro da fila agora eh: " << boolStack.first() << endl;

		cout << "Digite um valor (0 para parar): ";
		cin >> val;
	}

	cout << "O tamanho da fila eh: " << boolStack.count() << endl;
	while (!boolStack.isEmpty())
		cout << "Desenfileirando: " << --boolStack << endl;


	system("pause");
	return 0;
}


int main()
{
	return queue();
}
