#include <iostream>
#include <limits>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include "../lib/matrix.h"

using std::cout;
using std::cin;
using std::endl;


int main()
{
	std::ifstream fin("input_data/input.txt");

	int dimension;
	double elem, epsilon;
	
	cout << "Input the size n of the matrix A (matrix of size n x n): " << endl;
	fin >> dimension;
	cout << dimension << endl;
	while (dimension < 0)
	{
		cout << "Incorrect matrix size. Try again: " << endl;
		fin >> dimension;
	}
	Matrix matrix = Matrix(dimension, dimension);

	cout << "Input the matrix: " << endl;
	for (int i = 0; i < dimension; i++)
	{
		for (int j = 0; j < dimension; j++)
		{
			fin >> elem;
			matrix.SetElem(i, j, elem);
		}
	}

	matrix.print();
	cout << endl;
	if (matrix.isSymmetrical() == false)
	{
		cout << "The matrix must be symmetrical" << endl;
		exit(-1);
	}

	Matrix column = Matrix(dimension, 1);
	cout << "Input the constant terms: " << endl;
	for (int i = 0; i < dimension; i++)
	{
		fin >> elem;
		column.SetElem(i, 0, elem);
	}
	column.print();
	cout << endl;

	Matrix x = Matrix(dimension, 1);
	Matrix r = Matrix(dimension, 1);
	Matrix z = Matrix(dimension, 1);
	Matrix rPrev = Matrix(dimension, 1);
	Matrix xPrev = Matrix(dimension, 1);
	Matrix zPrev = Matrix(dimension, 1);
	cout << "Input the initial guess: " << endl;
	for (int i = 0; i < dimension; i++)
	{
		fin >> elem;
		x.SetElem(i, 0, elem);
	}
	x.print();
	cout << endl;

	cout << "Input the error: " << endl;
	fin >> epsilon;
	cout << epsilon << endl;

	r = column - matrix * x;
	z = r;

	double colNorm = column.norm();
	double a, b;
	int k = 0;

	while (epsilon < (r.norm()) / (colNorm))
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
	cout << "-------------" << endl;
	cout << "Solution: " << endl;
	x.print();
	cout << endl;
	cout << "-------------" << endl;
	cout << "iterations: " << endl;
	cout << k;
	return 0;
}