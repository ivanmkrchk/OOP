#include <thread>
#include <iostream>
#include <mutex>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#define SIZE 6

int a[SIZE][SIZE]; // матрица св€зей
int d[SIZE]; // минимальное рассто€ние
int v[SIZE]; // посещенные вершины
int minindex;

std::mutex main_mutex;
void step() {
	minindex = 10000;
	int min = 10000;
	for (int i = 0; i<SIZE; i++)
	{ // ≈сли вершину ещЄ не обошли и вес меньше min
		if ((v[i] == 1) && (d[i]<min))
		{ // ѕереприсваиваем значени€
			min = d[i];
			minindex = i;
		}
	}
	// ƒобавл€ем найденный минимальный вес
	// к текущему весу вершины
	// и сравниваем с текущим минимальным весом вершины
	if (minindex != 10000)
	{
		for (int i = 0; i<SIZE; i++)
		{
			if (a[minindex][i] > 0)
			{
				int temp = min + a[minindex][i];
				if (temp < d[i])
				{
					d[i] = temp;
				}
			}
		}
		v[minindex] = 0;
	}
}
int main()
{
	std::ifstream fin("data.in");
	
	int temp;
	int min;
	// »нициализаци€ матрицы св€зей
	for (int i = 0; i<SIZE; i++)
	{
		a[i][i] = 0;
		for (int j = i + 1; j<SIZE; j++) {
			//std::cout << "Enter distance between " << i + 1 << " and " << j + 1 << std::endl;
			fin >> temp;
			a[i][j] = temp;
			a[j][i] = temp;
		}
	}
	// ¬ывод матрицы св€зей
	std::cout << "Matrix of connections :" << std::endl;
	for (int i = 0; i<SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
			std::cout << a[i][j] << " ";
		std::cout << std::endl;
	}
	//»нициализаци€ вершин и рассто€ний
	for (int i = 0; i<SIZE; i++)
	{
		d[i] = 10000;
		v[i] = 1;
	}
	d[0] = 0;
	// Ўаг алгоритма
	/////////////////////////////////////////////////////////////////////////////////////////
	do {
		main_mutex.lock();
		step();
		main_mutex.unlock();
		std::thread step_tread(step);
		step_tread.join();
	} while (minindex < 10000);



	// ¬ывод кратчайших рассто€ний до вершин
	std::cout << "Mimum o ristance to vertex: " << std::endl;
	for (int i = 0; i < SIZE; i++)
		std::cout << d[i] << " ";
	// ¬осстановление пути
	int ver[SIZE]; // массив посещенных вершин
	int end = 4; // индекс конечной вершины = 5 - 1
	ver[0] = end + 1; // начальный элемент - конечна€ вершина
	int k = 1; // индекс предыдущей вершины
	int weight = d[end]; // вес конечной вершины
	while (end > 0) // пока не дошли до начальной вершины
	{
		for (int i = 0; i<SIZE; i++) // просматриваем все вершины
			if (a[end][i] != 0)   // если св€зь есть
			{
				int temp = weight - a[end][i]; // определ€ем вес пути из предыдущей вершины
				if (temp == d[i]) // если вес совпал с рассчитанным
				{                 // значит из этой вершины и был переход
					weight = temp; // сохран€ем новый вес
					end = i;       // сохран€ем предыдущую вершину
					ver[k] = i + 1; // и записываем ее в массив
					k++;
				}
			}
	}
	// ¬ывод пути (начальна€ вершина оказалась в конце массива из k элементов)
	std::cout << std::endl << "MINIMAL WAY" << std::endl;
	for (int i = k - 1; i >= 0; i--)
		std::cout << ver[i] << " ";
	std::cout << std::endl;

	//std::thread step_tread(step);
	//step_tread.join();

	getchar(); getchar();
	return 0;
}