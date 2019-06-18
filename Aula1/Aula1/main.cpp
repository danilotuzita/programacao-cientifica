#include <iostream>
#include <fstream>
#include "Stack.hpp"
#include "Queue.hpp"

#include <time.h>
#include <stack>
#include <queue>

using namespace std;

int stackManual()
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

int queueManual()
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


int _stack(ofstream* file)
{
	printf("==============================\n");
	printf("++++++ TESTE MEU STACK ++++++\n");
	clock_t tStart = clock();
	Stack<int> intStack;
	
	printf("Empilhando 1000000 de itens\n");
	for (int i = 0; i < 1000000; i++)
		intStack.push(i);
	
	printf("Desemplilhando 500000 itens\n");
	for (int i = 0; i < 500000; i++)
		intStack.pop();

	printf("Empilhando 1000 de itens\n");
	for (int i = 0; i < 1000; i++)
		intStack.push(i);

	printf("Desemplilhando todos os itens\n");
	while (intStack.count())
		intStack.pop();

	clock_t tEnd = clock();


	printf("\n\n==============================\n");
	printf("++++++ TESTE STACK CPP ++++++\n");
	clock_t tStartCpp = clock();
	stack<int> intStackCpp;

	printf("Empilhando 1000000 de itens\n");
	for (int i = 0; i < 1000000; i++)
		intStackCpp.push(i);

	printf("Desemplilhando 500000 itens\n");
	for (int i = 0; i < 500000; i++)
		intStackCpp.pop();

	printf("Empilhando 1000 de itens\n");
	for (int i = 0; i < 1000; i++)
		intStackCpp.push(i);

	printf("Desemplilhando todos os itens\n");
	while (intStackCpp.size())
		intStackCpp.pop();

	clock_t tEndCpp = clock();

	printf("\n\n==============================\n");
	printf("++++++ RESULTADOS MEU STACK ++++++\n");
	clock_t t0 = tEnd - tStart;
	printf("Levou %d ticks (%f s)\n", t0, ((float)t0) / CLOCKS_PER_SEC);

	printf("\n\n==============================\n");
	printf("++++++ RESULTADOS STACK CPP ++++++\n");
	clock_t t1 = tEndCpp - tStartCpp;
	printf("Levou %d ticks (%f s)\n\n", t1, ((float)t1) / CLOCKS_PER_SEC);


	*file << t0 << "\t" << ((float)t0) / CLOCKS_PER_SEC << "\t" << t1 << "\t" << ((float)t1) / CLOCKS_PER_SEC << "\n";

	printf("\n\n");

	return 0;
}


int _queue(ofstream* file)
{
	printf("==============================\n");
	printf("++++++ TESTE MEU QUEUE ++++++\n");
	clock_t tStart = clock();
	Queue<int> intQueue;

	printf("Enfileirando 1000000 de itens\n");
	for (int i = 0; i < 1000000; i++)
		intQueue.push(i);

	printf("Desenfileirando 500000 itens\n");
	for (int i = 0; i < 500000; i++)
		intQueue.pop();

	printf("Enfileirando 1000 de itens\n");
	for (int i = 0; i < 1000; i++)
		intQueue.push(i);

	printf("Desenfileirando todos os itens\n");
	while (intQueue.count())
		intQueue.pop();

	clock_t tEnd = clock();


	printf("\n\n==============================\n");
	printf("++++++ TESTE QUEUE CPP ++++++\n");
	clock_t tStartCpp = clock();
	queue<int> intQueueCpp;

	printf("Enfileirando 1000000 de itens\n");
	for (int i = 0; i < 1000000; i++)
		intQueueCpp.push(i);

	printf("Desenfileirando 500000 itens\n");
	for (int i = 0; i < 500000; i++)
		intQueueCpp.pop();

	printf("Enfileirando 1000 de itens\n");
	for (int i = 0; i < 1000; i++)
		intQueueCpp.push(i);

	printf("Desenfileirando todos os itens\n");
	while (intQueueCpp.size())
		intQueueCpp.pop();

	clock_t tEndCpp = clock();


	printf("\n\n==============================\n");
	printf("++++++ RESULTADOS MEU QUEUE ++++++\n");
	clock_t t0 = tEnd - tStart;
	printf("Levou %d ticks (%f s)\n", t0, ((float)t0) / CLOCKS_PER_SEC);

	printf("\n\n==============================\n");
	printf("++++++ RESULTADOS QUEUE CPP ++++++\n");
	clock_t t1 = tEndCpp - tStartCpp;
	printf("Levou %d ticks (%f s)\n\n", t1, ((float)t1) / CLOCKS_PER_SEC);

	
	*file << t0 << "\t" << ((float)t0) / CLOCKS_PER_SEC << "\t" << t1 << "\t" << ((float)t1) / CLOCKS_PER_SEC << "\n";

	printf("\n\n");

	return 0;
}


int main()
{
	ofstream stackFile, queueFile;
	stackFile.open("stack.csv");
	queueFile.open("queue.csv");

	stackFile << "ticks[42]\ttempo(s)[42]\tticks[CPP]\ttempo(s)[CPP]\n";
	queueFile << "ticks[42]\ttempo(s)[42]\tticks[CPP]\ttempo(s)[CPP]\n";

	for (int i = 0; i < 100; i++)
		_stack(&stackFile);
	stackFile.close();

	for (int i = 0; i < 100; i++)
		_queue(&queueFile);
	queueFile.close();

	system("PAUSE");
	return 0;
}
