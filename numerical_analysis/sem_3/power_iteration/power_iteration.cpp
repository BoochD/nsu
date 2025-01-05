#include <iostream>
#include <limits>
#include <iomanip>
#include <fstream>
#include <cmath>
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
	cout << "Input the size n of the matrix A (matrix of size n x n): ";
	fin >> dimension;
	while (dimension < 0)
	{
		cout << "Incorrect matrix size. Try again: ";
		fin >> dimension;
	}
	cout << dimension << endl;

	Matrix matrix = Matrix(dimension, dimension);
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
	Matrix prevEVector = Matrix(dimension, 1);
	cout << "Input the initial guess: " << endl;
	for (int i = 0; i < dimension; i++)
	{
		fin >> elem;
		prevEVector.SetElem(i, 0, elem);
	}
	if (prevEVector.isZero() == true)
	{
		cout << "The initial guess must be non-zero." << endl;
		exit(-1);
	}
	prevEVector.print();
	cout << endl;

	Matrix firstEVector = prevEVector;
	cout << "Input the error: " << endl;
	fin >> epsilon;
	double prevEValue = prevEVector.norm();
	cout << epsilon << endl;

	Matrix currEVector = matrix * (prevEVector / prevEVector.norm());
	double currEValue = currEVector.norm();

	int k = 0;

	while (abs(currEValue - prevEValue) > epsilon)
	{
		k++;
		prevEVector = currEVector;
		currEVector = matrix * currEVector * (1 / currEVector.norm());
		prevEValue = currEValue;
		currEValue = currEVector.norm();
	}
	double max = currEValue;
	cout << "----------------" << endl;
	cout << "Maximal eigen value: " << std::fixed << std::setprecision(8) << max << endl;
	cout << "Iterations: " << endl;
	cout << k << endl;
	cout << "----------------" << endl;
	k = 0;
	prevEVector = firstEVector;
	Matrix I = Matrix(dimension);
	currEVector = (I * matrix.norm() - matrix) * prevEVector * (1 / prevEVector.norm());
	prevEValue = matrix.norm() - prevEVector.norm();
	currEValue = matrix.norm() - currEVector.norm();

	while (abs(prevEValue - currEValue) > epsilon)
	{
		k++;
		prevEVector = currEVector;
		currEVector = (I * matrix.norm() - matrix) * prevEVector * (1 / prevEVector.norm());
		prevEValue = currEValue;
		currEValue = matrix.norm() - currEVector.norm();
	}
	double min = currEValue;
	cout << "----------------" << endl;
	cout << "Minimal eigen value: " << std::fixed << std::setprecision(8) << min << endl;
	cout << "Iterations: " << endl;
	cout << k << endl;
	cout << "----------------";

	return 0;
}