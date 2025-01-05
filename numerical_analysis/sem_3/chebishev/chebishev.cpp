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
    double elem, epsilon, lambdaMin, lambdaMax;

    cout << "Input the size n of the matrix A (matrix of size n x n): ";
    fin >> dimension;
    while (dimension < 0) {
        cout << "Incorrect matrix size. Try again: ";
        fin >> dimension;
    }

    Matrix matrix = Matrix(dimension, dimension);
    for (size_t i = 0; i < dimension; i++) {
        for (size_t j = 0; j < dimension; j++) {
            fin >> elem;
            matrix.SetElem(i, j, elem);
        }
    }

    Matrix stolbec = Matrix(dimension, 1);
    cout << "Constant vector: " << endl;
    for (size_t i = 0; i < dimension; i++) {
        fin >> elem;
        stolbec.SetElem(i, 0, elem);
    }
    cout << "Input the initial guess: " << endl;
    Matrix xPrevPrev = Matrix(dimension, 1);
    for (size_t i = 0; i < dimension; i++) {
        fin >> elem;
        xPrevPrev.SetElem(i, 0, elem);
    }
    cout << "Input the error: " << endl;
    fin >> epsilon >> lambdaMin >> lambdaMax;
    double stolbNorm = stolbec.norm();
    const double tao = 2 / (lambdaMin + lambdaMax);
    double omegaFirst = -((lambdaMax - lambdaMin) / (lambdaMax + lambdaMin));
    Matrix r = matrix * xPrevPrev - stolbec;
    Matrix xPrev = xPrevPrev - r * tao;
    r = matrix * xPrev - stolbec;
    double omegaPrevPrev = omegaFirst;
    double omegaPrev = 1 / (2 * (1 / omegaFirst) - omegaFirst);
    Matrix xCurr =
        xPrev + (xPrev - xPrevPrev) * (omegaFirst * omegaPrev) - r * (tao * (1 + omegaFirst * omegaPrev));
    r = matrix * xCurr - stolbec;
    double omegaCurr;
    cout << r.norm() << endl;
    cout << stolbNorm << endl;
    size_t k = 0;
    while (abs(r.norm() / stolbNorm) > epsilon) {
        k++;
        omegaCurr = 1 / (2 * (1 / omegaFirst) - omegaPrev);
        xPrevPrev = xPrev;
        xPrev = xCurr;
        xCurr = xPrev + (xPrev - xPrevPrev) * (omegaPrev * omegaCurr) - r * (1 + omegaPrev * omegaCurr) * tao;
        r = matrix * xCurr - stolbec;
        omegaPrev = omegaCurr;
        double norma = r.norm();
    }

    xCurr.print();
    cout << endl;
    cout << "Num of iterations: " << k << endl;

    return 0;
}