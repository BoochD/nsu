#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>
#include "../lib/matrix.h"

using std::cin;
using std::cout;
using std::endl;

int main() {
    std::ifstream fin("input_data/input.txt");

    int dimension;
    double elem, alpha, epsilon;

    cout << "Input the size n of the matrix A(matrix of size n x n): ";
    fin >> dimension;

    while (dimension < 0) {
        cout << "Incorrect matrix size. Try again: ";
        fin >> dimension;
    }
    cout << dimension << endl;
    
    Matrix matrix = Matrix(dimension, dimension);

    cout << "Input the matrix: " << endl;
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            fin >> elem;
            matrix.SetElem(i, j, elem);
        }
    }
    matrix.print();
    cout << endl;
    Matrix currX = Matrix(dimension, 1);
    cout << "Input the initial guess: " << endl;
    for (int i = 0; i < dimension; i++) {
        fin >> elem;
        currX.SetElem(i, 0, elem);
    }
    if (currX.isZero() == true) {
        cout << "Initial guess must be non-zero." << endl;
        exit(-1);
    }
    currX.print();
    cout << endl;
    cout << "Input the number closest to the target eigenvalue: " << endl;
    fin >> alpha;
    cout << alpha << endl;


    cout << "Input error: " << endl;
    fin >> epsilon;
    cout << epsilon << endl;
    Matrix I = Matrix(dimension);
    Matrix newMatrix = matrix - I * alpha;
    int k = 0;
    double currC = 1;
    double prevC = 0;
    while (abs(prevC - currC) > epsilon) {
        Matrix m = Matrix(dimension, dimension + 1);
        for (int i = 0; i < dimension; i++) {
            for (int j = 0; j < dimension + 1; j++) {
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
        currX = currY / currY.norm2();
        k++;
    }
    double closerLambda = 1 / currC + alpha;
    cout << "-------------" << endl;
    cout << "Closer eigenvalue: " << std::fixed << std::setprecision(8) << closerLambda << endl;
    cout << "-------------" << endl;
    cout << "Iterations: " << endl;
    cout << k;
    return 0;
}