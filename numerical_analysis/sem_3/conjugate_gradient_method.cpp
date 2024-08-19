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
	
	cout << "Введите размер n матрицы A (матрица размера n x n): ";
	fin >> dimension;

	while (dimension < 0)
	{
		cout << "Вы ввели некорректный размер матрицы. Попроуйте еще раз: ";
		fin >> dimension;
	}

	Matrix matrix = Matrix(dimension, dimension); //Создал матрицу

	cout << "Введите матрицу: " << endl;
	for (int i = 0; i < dimension; i++) //Ввод элементов матрицы
	{
		for (int j = 0; j < dimension; j++)
		{
			fin >> elem;
			matrix.SetElem(i, j, elem);
		}
	}

	if (matrix.isSymmetrical() == false)
	{
		cout << "Введена несимметричная матрица." << endl;
		exit(-1);
	}

	Matrix stolbec = Matrix(dimension, 1);
	cout << "Введите свободные коэффициенты: " << endl;
	for (int i = 0; i < dimension; i++) //Ввод свободных коэффициентов
	{
		fin >> elem;
		stolbec.SetElem(i, 0, elem);
	}

	Matrix x = Matrix(dimension, 1);
	Matrix r = Matrix(dimension, 1); //невязка
	Matrix z = Matrix(dimension, 1); //ошибка
	Matrix rPrev = Matrix(dimension, 1);
	Matrix xPrev = Matrix(dimension, 1);
	Matrix zPrev = Matrix(dimension, 1);
	cout << "Введите начальное приближение: " << endl;
	for (int i = 0; i < dimension; i++)
	{
		fin >> elem;
		x.SetElem(i, 0, elem);
	}
	cout << "Введите погрешность: " << endl;
	fin >> epsilon;


	//Считаем начальную невязку

	r = stolbec - matrix * x;
	z = r;

	double stolbNorm = stolbec.norm();
	double a, b;
	int k = 0;

	while (epsilon < (r.norm()) / (stolbNorm)) //пока относительная невязка не станет >= заданному числу
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

	cout << "Ответ" << endl;
	x.print();
	cout << "Количество итераций: " << endl;
	cout << k;
	return 0;
}