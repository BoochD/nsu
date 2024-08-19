#ifndef MATRIX_H
#define MATRIX_H
#define __STDC_LIMIT_MACROS
#include <cmath>
#include <vector>
#include <cstdint>
#include <iostream>
#include <iomanip>

class Matrix {
private:
    double** M;
    int m, n;
    double determ = 1;

public:
    Matrix() {
        n = m = 0;
        M = nullptr;
    }
    Matrix(int n) {
        this->m = n;
        this->n = n;

        M = (double**)new double* [m];

        for (int i = 0; i < m; i++) {
            M[i] = (double*)new double[n];
        }

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (i != j)
                    M[i][j] = 0;
                else
                    M[i][i] = 1;
            }
        }
    }
    Matrix(int m, int n) {
        this->m = m;
        this->n = n;

        M = (double**)new double* [m];

        for (int i = 0; i < m; i++) {
            M[i] = (double*)new double[n];
        }

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                M[i][j] = 0;
            }
        }
    }
    Matrix(const Matrix& Matr) {
        this->m = Matr.m;
        this->n = Matr.n;

        this->M = (double**)new double* [m];

        for (int i = 0; i < m; i++) {
            this->M[i] = (double*)new double[n];
        }

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                this->M[i][j] = Matr.M[i][j];
            }
        }
    }
    double GetElem(int i, int j) {
        if ((this->m > 0 && this->m > i) && (this->n > 0 && this->n > j)) {
            return M[i][j];
        }
        else {
            return 0;
        }
    }
    bool isSymmetrical() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < i; j++) {
                if (M[i][j] != M[j][i]) return false;
            }
        }
        return true;
    }
    std::vector<int> getDimension() {
        if (m == n) {
            return { m };
        }
        else
            return { m, n };
    }
    void SetElem(int i, int j, double a) { M[i][j] = a; }
    void multiplOnElem(int line, double elem) {
        determ /= elem;
        for (int i = 0; i < n; i++) {
            M[line][i] = M[line][i] * elem;
        }
    }
    double norm() {
        double max = -INT16_MAX;
        for (int i = 0; i < m; i++) {
            double currSum = 0;
            for (int j = 0; j < n; j++) {
                currSum += abs(M[i][j]);
            }
            if (currSum > max) max = currSum;
        }
        return max;
    }
    void sumLine(int second, int first) {
        double result;
        for (int i = 0; i < n; i++) {
            result = M[second][i] + M[first][i];
            M[second][i] = result;
        }
    }
    void upperTriang() {
        for (int i = 0; i < m - 1; i++) {
            for (int j = 0; j < m - 1; j++) {
                if (j + 1 != i && M[i][i] != 0 && M[j + 1][i] != 0) {
                    multiplOnElem(i, -(M[j + 1][i] / M[i][i]));
                    sumLine(j + 1, i);
                }
                else if (j + 1 != i && M[i][i] == 0 && M[j + 1][i] != 0) {
                    lineInterchange(j + 1, i);
                }
            }
        }
    }
    void lowerTriang() {
        for (int i = m - 1; i > 0; i--) {
            for (int j = m - 1; j > 0; j--) {
                if (j - 1 != i && M[i][i] != 0 && M[j - 1][i] != 0) {
                    multiplOnElem(i, -(M[j - 1][i] / M[i][i]));
                    sumLine(j - 1, i);
                }
                else if (j - 1 != i && M[i][i] == 0 && M[j - 1][i] != 0)
                    lineInterchange(j - 1, i);
            }
        }
    }
    void diag() {
        upperTriang();
        lowerTriang();
    }
    void print() {
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                std::cout << std::fixed << std::setprecision(8) << GetElem(i, j) << " ";
            }
            std::cout << std::endl;
        }
    }
    void print(int f, int s) {
        for (int i = 0; i < f; i++) {
            for (int j = 0; j < s; j++) {
                if (GetElem(i, j) == 0)
                    std::cout << std::fixed << std::setprecision(8) << GetElem(i, j) << " ";
                else if (GetElem(i, j) == -0)
                    std::cout << std::fixed << std::setprecision(8) << -GetElem(i, j) << " ";
                else
                    std::cout << std::fixed << std::setprecision(8) << -GetElem(i, j) << " ";
            }
            std::cout << std::endl;
        }
    }
    void lineInterchange(unsigned int first, unsigned int second) {
        determ *= -1;
        double elem;
        for (int i = 0; i < n; i++) {
            elem = M[first][i];
            M[first][i] = M[second][i];
            M[second][i] = elem;
        }
    }
    void columnInterchange(unsigned int first, unsigned int second) {
        determ *= -1;
        double elem;
        for (int i = 0; i < m; i++) {
            elem = M[i][first];
            M[i][first] = M[i][second];
            M[i][second] = elem;
        }
    }
    Matrix transpose() {
        Matrix result = Matrix(n, m);
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                result.M[i][j] = this->M[j][i];
            }
        }
        return result;
    }
    double determinant() {
        diag();
        for (int i = 0; i < m; i++) {
            determ *= M[i][i];
        }
        return determ;
    }
    double norm1() {
        double max = -INT16_MAX;
        for (int i = 0; i < m; i++) {
            double currSum = 0;
            for (int j = 0; j < n; j++) {
                currSum += abs(M[i][j]);
            }
            if (currSum > max) max = currSum;
        }
        return max;
    }
    double norm2() {
        double sum = 0;
        for (int i = 0; i < m; i++) {
            sum += pow(M[i][0], 2);
        }
        return pow(sum, 0.5);
    }
    Matrix operator=(const Matrix& Matr) {
        this->m = Matr.m;
        this->n = Matr.n;
        this->M = (double**)new double* [m];
        for (int i = 0; i < m; i++) {
            this->M[i] = (double*)new double[m];
        }
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++) this->M[i][j] = Matr.M[i][j];
        return *this;
    }
    Matrix operator+(const Matrix& matrix) {
        if ((matrix.n == 0 && n == 0) || (matrix.n != n)) {
            Matrix result = Matrix();
            return result;
        }
        Matrix result = Matrix(m, n);
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                result.M[i][j] = M[i][j] + matrix.M[i][j];
            }
        }
        return result;
    }
    Matrix operator-(const Matrix& matrix) {
        if ((matrix.n == 0 && n == 0) || (matrix.n != n)) {
            Matrix result = Matrix();
            return result;
        }
        Matrix result = Matrix(matrix.m, matrix.n);
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                result.M[i][j] = M[i][j] - matrix.M[i][j];
            }
        }
        return result;
    }
    Matrix operator*(const Matrix& matrix) {
        if ((matrix.n == 0 && n == 0) || (n != matrix.m)) {
            Matrix result = Matrix();
            return result;
        }
        Matrix result = Matrix(matrix.m, matrix.n);
        {
            for (int i = 0; i < matrix.m; i++) {
                for (int j = 0; j < matrix.n; j++) {
                    result.M[i][j] = 0;
                    for (int k = 0; k < matrix.m; k++) {
                        if (abs(M[i][k]) < 0.0000001) {
                            result.M[i][j] += 0;
                        }
                        else if (abs(matrix.M[k][j]) < 0.0000001) {
                            result.M[i][j] += 0;
                        }
                        else {
                            result.M[i][j] += M[i][k] * matrix.M[k][j];
                        }
                    }
                }
            }
        }
        return result;
    }
    Matrix operator*(double a) {
        Matrix result = Matrix(m, n);
        double elem;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                elem = M[i][j] * a;
                result.SetElem(i, j, elem);
            }
        }
        return result;
    }
    Matrix operator/(double a) {
        if (a == 0) {
            return 0;
        }
        Matrix result = Matrix(m, n);
        double elem;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                elem = M[i][j] / a;
                result.SetElem(i, j, elem);
            }
        }
        return result;
    }
    double* operator()(int column) {
        double* result = new double[m];
        for (int i = 0; i < m; i++) {
            result[i] = M[i][column];
        }
        return result;
    }
    Matrix operator[](int column) {
        Matrix result = Matrix(m, 1);
        for (int i = 0; i < m; i++) {
            result.SetElem(i, 0, M[i][column]);
        }
        return result;
    }
    bool isZero() {
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (M[i][j] != 0) {
                    return false;
                }
            }
        }
        return true;
    }
    double checkLowerTriangle() {
        double sum = 0;
        for (int i = 1; i < m; i++) {
            for (int j = 0; j < i; j++) {
                if (i != j) {
                    sum += pow(M[i][j], 2);
                }
            }
        }
        return pow(sum, 0.5);
    }
    Matrix Gauss() {
        diag();
        Matrix res = Matrix(m, 1);
        for (int i = 0; i < m; i++) {
            res.SetElem(i, 0, M[i][n - 1] / M[i][i]);
        }
        for (int i = 0; i < m; i++) {
            determ *= M[i][i];
        }
        return res;
    }
};

double dotProduct(double* A, double* B, int n) {
    double result = 0;
    for (int i = 0; i < n; i++) {
        result += A[i] * B[i];
    }
    return result;
}

double stringByColumn(Matrix string, Matrix column) {
    if (string.getDimension()[0] == column.getDimension()[0]) {
        double result = 0;
        for (int i = 0; i < string.getDimension()[0]; i++) {
            result += string.GetElem(i, 0) * column.GetElem(i, 0);
        }
        return result;
    }
    else
        return 0;
}

#endif //MATRIX_H