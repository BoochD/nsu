#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>
#include "matrix.h"

using std::cin;
using std::cout;
using std::endl;



int sign(double a) {
    if (a >= 0)
        return 1;
    else if (a < 0)
        return -1;
}

int main() {
    std::ifstream fin("input.txt");
    setlocale(LC_ALL, "Russian");

    int dimension;
    double elem, alpha, epsilon;

    cout << "Введите размер n матрицы A (матрица размера n x n): ";
    fin >> dimension;

    while (dimension < 0) {
        cout << "Вы ввели некорректный размер матрицы. Попроуйте еще раз: ";
        fin >> dimension;
    }

    Matrix matrix = Matrix(dimension, dimension);

    cout << "Введите матрицу: " << endl;
    for (size_t i = 0; i < dimension; i++) {
        for (size_t j = 0; j < dimension; j++) {
            fin >> elem;
            matrix.SetElem(i, j, elem);
        }
    }

    Matrix currX = Matrix(dimension, 1);
    cout << "Введите приближение(ненулевое): " << endl;
    for (size_t i = 0; i < dimension; i++) {
        fin >> elem;
        currX.SetElem(i, 0, elem);
    }
    if (currX.isZero() == true) {
        cout << "Нельзя вводить нулевое приближение!" << endl;
        exit(-1);
    }

    cout << "Введите число, к которому мы будем искать ближайшее собственное "
            "значени: "
         << endl;
    fin >> alpha;

    cout << "Введите epsilon: " << endl;
    fin >> epsilon;
    Matrix I = Matrix(dimension);
    Matrix newMatrix = matrix - I * alpha;
    int k = 0;
    double currC = 1;
    double prevC = 0;
    while (abs(prevC - currC) > epsilon) {
        Matrix m = Matrix(dimension, dimension + 1);
        for (size_t i = 0; i < dimension; i++) {
            for (size_t j = 0; j < dimension + 1; j++) {
                if (j == dimension) {
                    m.SetElem(i, j, currX.GetElem(i, 0));
                } else {
                    m.SetElem(i, j, newMatrix.GetElem(i, j));
                }
            }
        }
        Matrix currY = m.Gauss();
        prevC = currC;
        currC = stringByColumn(currY, currX) / stringByColumn(currX, currX);
        cout << currC << endl;
        currX = currY / currY.norm2();
    }
    double closerLambda = 1 / currC + alpha;
    cout << "Closer eigenvalue is " << std::fixed << std::setprecision(8) << closerLambda << endl;
    return 0;
}