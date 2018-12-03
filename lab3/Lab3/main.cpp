#include <thread>
#include <iostream>
#include <mutex>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#define SIZE 6

int a[SIZE][SIZE]; // ������� ������
int d[SIZE]; // ����������� ����������
int v[SIZE]; // ���������� �������
int minindex;

std::mutex main_mutex;
void step() {
	minindex = 10000;
	int min = 10000;
	for (int i = 0; i<SIZE; i++)
	{ // ���� ������� ��� �� ������ � ��� ������ min
		if ((v[i] == 1) && (d[i]<min))
		{ // ��������������� ��������
			min = d[i];
			minindex = i;
		}
	}
	// ��������� ��������� ����������� ���
	// � �������� ���� �������
	// � ���������� � ������� ����������� ����� �������
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
	// ������������� ������� ������
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
	// ����� ������� ������
	std::cout << "Matrix of connections :" << std::endl;
	for (int i = 0; i<SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
			std::cout << a[i][j] << " ";
		std::cout << std::endl;
	}
	//������������� ������ � ����������
	for (int i = 0; i<SIZE; i++)
	{
		d[i] = 10000;
		v[i] = 1;
	}
	d[0] = 0;
	// ��� ���������
	/////////////////////////////////////////////////////////////////////////////////////////
	do {
		main_mutex.lock();
		step();
		main_mutex.unlock();
		std::thread step_tread(step);
		step_tread.join();
	} while (minindex < 10000);



	// ����� ���������� ���������� �� ������
	std::cout << "Mimum o ristance to vertex: " << std::endl;
	for (int i = 0; i < SIZE; i++)
		std::cout << d[i] << " ";
	// �������������� ����
	int ver[SIZE]; // ������ ���������� ������
	int end = 4; // ������ �������� ������� = 5 - 1
	ver[0] = end + 1; // ��������� ������� - �������� �������
	int k = 1; // ������ ���������� �������
	int weight = d[end]; // ��� �������� �������
	while (end > 0) // ���� �� ����� �� ��������� �������
	{
		for (int i = 0; i<SIZE; i++) // ������������� ��� �������
			if (a[end][i] != 0)   // ���� ����� ����
			{
				int temp = weight - a[end][i]; // ���������� ��� ���� �� ���������� �������
				if (temp == d[i]) // ���� ��� ������ � ������������
				{                 // ������ �� ���� ������� � ��� �������
					weight = temp; // ��������� ����� ���
					end = i;       // ��������� ���������� �������
					ver[k] = i + 1; // � ���������� �� � ������
					k++;
				}
			}
	}
	// ����� ���� (��������� ������� ��������� � ����� ������� �� k ���������)
	std::cout << std::endl << "MINIMAL WAY" << std::endl;
	for (int i = k - 1; i >= 0; i--)
		std::cout << ver[i] << " ";
	std::cout << std::endl;

	//std::thread step_tread(step);
	//step_tread.join();

	getchar(); getchar();
	return 0;
}