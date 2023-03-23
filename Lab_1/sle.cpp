#include "sle.h"

#include <fstream>

namespace
{
constexpr double EPS = 1e-6;

bool sleHasSolution(const Matrix& A)
{
    for (size_t i = 0; i < A.getNumRows(); ++i)
        if (A[i][i] <= EPS)
            return false;

    return true;
}

bool sleHasInfiniteSolutions(const Matrix& A)
{
    for (size_t i = 0; i < A.getNumRows(); ++i)
    {
        bool allZeros = true;
        for (size_t j = 0; j < A.getNumColumns(); ++j)
        {
            if (A[i][j] >= EPS)
            {
                allZeros = false;
                break;
            }
        }

        if (allZeros)
            return true;
    }

    return false;
}

Matrix getMatrixT(const Matrix& matrix, int k)
{
    Matrix T(matrix.getNumRows(), matrix.getNumColumns());

    for (int i = 0; i < T.getNumRows(); ++i)
        T[i][i] = 1;

    T[k][k] = 1.0 / matrix[k][k];

    for (int i = (k + 1); i < T.getNumRows(); ++i)
        T[i][k] = -matrix[i][k] / matrix[k][k];

    return T;
}

void getEchelonForm(Matrix& A, Matrix& B, std::ostream* output = nullptr)
{
    for (int k = 0; k < A.getNumRows(); ++k)
    {
        Matrix T = getMatrixT(A, k);

        if (output)
        {
            *output << "Iteration " << k + 1 << ":\n";
            *output << "T(" << k + 1 << ") = \n" << T;
        }

        A = T * A;
        B = T * B;

        if (output)
        {
            *output << "A(" << k + 1 << ") = \n" << A;
            *output << "B(" << k + 1 << ") = \n" << B << std::endl;
        }
    }
}

Matrix getMatrixV(const Matrix& matrix, int k)
{
    Matrix V(matrix.getNumRows(), matrix.getNumColumns());

    for (int i = 0; i < V.getNumRows(); ++i)
        V[i][i] = 1;

    V[k][k] = 1.0 / matrix[k][k];

    for (int i = 0; i < k; ++i)
        V[i][k] = -matrix[i][k];

    return V;
}

Vector getSolution(Matrix& U, Matrix& C, std::ostream* output = nullptr)
{
    Vector soultion(C.getNumRows());

    for (int k = (U.getNumRows() - 1); k >= 0; --k)
    {
        Matrix V = getMatrixV(U, k);

        if (output)
        {
            *output << "Iteration " << U.getNumRows() - k << ":\n";
            *output << "V(" << k + 1 << ") = \n" << V;
        }

        U = V * U;
        C = V * C;

        if (output)
        {
            *output << "U(" << k + 1 << ") = \n" << U;
            *output << "C(" << k + 1 << ") = \n" << C << std::endl;
        }
    }

    for (int i = 0; i < C.getNumRows(); ++i)
        soultion[i] = C[i][0];

    return soultion;
}

Matrix calculateError(const Matrix& A, const Matrix& B, const Matrix& x)
{
    return ((A * x) - B);
}

double calculateRelativeError(const Matrix& A, const Matrix& B, const Matrix& x)
{
    Matrix residual = calculateError(A, B, x);
    return residual.calculateEuclidianNorm() / B.calculateEuclidianNorm();
}
}

SLE::SLE()
{
}

void SLE::setMatrixA(const Matrix &matrix)
{
    A = std::make_unique<Matrix>(matrix);
}

Matrix SLE::getMatrixA() const
{
    if (!A.get())
        throw std::runtime_error("Matrix A does not exist");

    return *A;
}

Matrix &SLE::getMatrixA()
{
    if (!A.get())
        throw std::runtime_error("Matrix A does not exist");

    return *A;
}

void SLE::setMatrixB(const Matrix &matrix)
{
    B = std::make_unique<Matrix>(matrix);
}

Matrix SLE::getMatrixB() const
{
    if (!B.get())
        throw std::runtime_error("Matrix B does not exist");

    return *B;
}

Matrix &SLE::getMatrixB()
{
    if (!B.get())
        throw std::runtime_error("Matrix B does not exist");

    return *B;
}

Vector SLE::getVectorX() const
{
    if (!x.get())
        throw std::runtime_error("Vector x does not exist");

    return *x;
}

Vector &SLE::getVectorX()
{
    if (!x.get())
        throw std::runtime_error("Vector x does not exist");

    return *x;
}

void SLE::solve()
{
    std::ofstream output("last_solution.txt", std::ios_base::trunc);

    if (!A.get())
        throw std::runtime_error("Matrix A does not exist");
    if (!B.get())
        throw std::runtime_error("Matrix B does not exist");
    if (B->getNumColumns() != 1)
        throw std::runtime_error("Wrong size of B matrix, matrix B should have only 1 column");
    if (!sleHasSolution(*A))
        throw std::runtime_error("SLE has no solution");

    Matrix U(*A);
    Matrix C(*B);

    getEchelonForm(U, C, &output);
    if (sleHasInfiniteSolutions(U))
        throw std::runtime_error("SLE has infinitely many solutions");

    x = std::make_unique<Vector>(getSolution(U, C, &output));

    output << "\nSolution:\n";
    output << C << std::endl;
    output << "\nError:\n";
    output << calculateError(*A, *B, C) << std::endl;
    output << "\nRelative Error:\n";
    output << calculateRelativeError(*A, *B, C) << std::endl;
}
