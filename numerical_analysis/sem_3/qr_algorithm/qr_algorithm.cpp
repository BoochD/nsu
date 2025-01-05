#include <iostream>
#include <limits>
#include <iomanip>
#include <fstream>
#include "../lib/matrix.h"

using std::cout;
using std::cin;
using std::endl;

int sign(double a)
{
	int s = 1;
	if (a >= 0) s = 1;
	else if (a < 0) s = -1;
	return s;
}

int main()
{
	std::ifstream fin("input_data/input.txt");

	int dimension;
	double elem;

	cout << "Input the size n of the matrix A(matrix of size n x n): ";
	fin >> dimension;
	cout << dimension << endl;

	while (dimension < 0)
	{
		cout << "Incorrect matrix size. Try again: ";
		fin >> dimension;
	}

	Matrix R = Matrix(dimension, dimension);

	cout << "Input the matrix: " << endl;
	for (int i = 0; i < dimension; i++)
	{
		for (int j = 0; j < dimension; j++)
		{
			fin >> elem;
			R.SetElem(i, j, elem);
		}
	}
	R.print();
	cout << endl;

	Matrix prevEVector = Matrix(dimension, 1);
	cout << "Input the initial guess: " << endl;
	for (int i = 0; i < dimension; i++)
	{
		fin >> elem;
		prevEVector.SetElem(i, 0, elem);
	}
	if (prevEVector.isZero() == true)
	{
		cout << "Initial guess must be non-zero." << endl;
		exit(-1);
	}
	prevEVector.print();
	cout << endl;
	cout << "Input error: " << endl;
	double epsilon;
	fin >> epsilon;
	cout << epsilon << endl;

	double* vectorS = new double[dimension];
	Matrix E = Matrix(dimension);
	Matrix* U = new Matrix[dimension];
	Matrix Q = Matrix(dimension);
	Matrix A;

	int k = 0;
	while (R.checkLowerTriangle() > epsilon)
	{
		Q = E;
		k++;
		for (int i = 0; i < dimension - 1; i++)
		{
			double* vectorH = new double[dimension];
			double scalarProduct1 = 0;
			double scalarProduct2 = 0;
			double b, p;
			for (int j = 0; j < dimension; j++)
			{
				if (i <= j) vectorS[j] = R.GetElem(j, i);
				else vectorS[j] = 0;
				if (i == j) vectorH[j] = 1;
				else vectorH[j] = 0;
			}
			for (int j = 0; j < dimension; j++)
			{
				scalarProduct1 += vectorS[j] * vectorS[j];
				scalarProduct2 += vectorS[j] * vectorH[j];
			}
			b = sign(R.GetElem(i, i)) * pow(scalarProduct1, 0.5);
			p = pow(2 * (scalarProduct1 + b * scalarProduct2), 0.5);
			double* w = new double[dimension];

			for (int j = 0; j < dimension; j++)
			{
				w[j] = (1 / p) * (vectorS[j] + b * vectorH[j]);
			}
			Matrix help = Matrix(dimension);
			for (int j = 0; j < dimension; j++)
			{
				for (int k = 0; k < dimension; k++)
				{
					help.SetElem(j, k, 2 * w[k] * w[j]);
				}
			}
			U[i] = E - help;

			R = U[i] * R;
			Q = Q * U[i];
		}
		A = R * Q;
		R = A;
	}
	cout << "-------------" << endl;
	cout << "Upper triangular matrix R with eigenvalues on diagonal: " << endl;
	R.print();
	cout << endl;
	cout << "-------------" << endl;

	cout << "Iterations: " << endl;
	cout << k;
	return 0;
}