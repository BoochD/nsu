#include <iostream>
#include <limits>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <vector>
#include "matrix.h"

using std::cout;
using std::cin;
using std::endl;


int main()
{
	cout << "HELLLLLOOOO";
	std::ifstream fin("input.txt");
	setlocale(LC_ALL, "Russian");

	int dimension;
	double elem, epsilon;
	
	cout << "������� ������ n ������� A (������� ������� n x n): ";
	fin >> dimension;

	while (dimension < 0)
	{
		cout << "�� ����� ������������ ������ �������. ��������� ��� ���: ";
		fin >> dimension;
	}

	Matrix matrix = Matrix(dimension, dimension); //������ �������

	cout << "������� �������: " << endl;
	for (int i = 0; i < dimension; i++) //���� ��������� �������
	{
		for (int j = 0; j < dimension; j++)
		{
			fin >> elem;
			matrix.SetElem(i, j, elem);
		}
	}

	if (matrix.isSymmetrical() == false)
	{
		cout << "������� �������������� �������." << endl;
		exit(-1);
	}

	Matrix stolbec = Matrix(dimension, 1);
	cout << "������� ��������� ������������: " << endl;
	for (int i = 0; i < dimension; i++) //���� ��������� �������������
	{
		fin >> elem;
		stolbec.SetElem(i, 0, elem);
	}

	Matrix x = Matrix(dimension, 1);
	Matrix r = Matrix(dimension, 1); //�������
	Matrix z = Matrix(dimension, 1); //������
	Matrix rPrev = Matrix(dimension, 1);
	Matrix xPrev = Matrix(dimension, 1);
	Matrix zPrev = Matrix(dimension, 1);
	cout << "������� ��������� �����������: " << endl;
	for (int i = 0; i < dimension; i++)
	{
		fin >> elem;
		x.SetElem(i, 0, elem);
	}
	cout << "������� �����������: " << endl;
	fin >> epsilon;


	//������� ��������� �������

	r = stolbec - matrix * x;
	z = r;

	double stolbNorm = stolbec.norm();
	double a, b;
	int k = 0;

	while (epsilon < (r.norm()) / (stolbNorm)) //���� ������������� ������� �� ������ >= ��������� �����
	{
		k++;
		rPrev = r;
		xPrev = x;
		zPrev = z;

		a = (dotProduct(rPrev(0), rPrev(0), dimension)) / (dotProduct((matrix * zPrev)(0), zPrev(0), dimension));
		x = xPrev + zPrev * a;
		r = rPrev - (matrix * zPrev) * a;

		b = (dotProduct(r(0), r(0), dimension)) / (dotProduct(rPrev(0), rPrev(0), dimension));
		z = r + zPrev * b;
	}

	cout << "�����" << endl;
	x.print();
	cout << "���������� ��������: " << endl;
	cout << k;
	return 0;
}