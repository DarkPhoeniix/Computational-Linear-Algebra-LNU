#include "sle.h"

#include <algorithm>
#include <numeric>
#include <fstream>
#include <cmath>

namespace
{
constexpr double EPS = 1e-6;
constexpr int MAX_ITERAIONS_NUM = 10000;

enum class SLESolutionType
{
    SolitionExists,
    NoSolution,
    InfiniteSolutions
};

SLESolutionType getSLESolutionType(const Matrix& A, const Matrix& B)
{
    for (size_t i = 0; i < A.getNumRows(); ++i)
    {
        bool allZeros = true;
        for (size_t j = 0; j < A.getNumColumns(); ++j)
        {
            if (std::abs(A[i][j]) >= EPS)
            {
                allZeros = false;
                break;
            }
        }

        if (allZeros && std::abs(B[i][0]) >= EPS)
            return SLESolutionType::NoSolution;
        else if (allZeros && std::abs(B[i][0]) <= EPS)
            return SLESolutionType::InfiniteSolutions;
    }

    return SLESolutionType::SolitionExists;
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
    for (size_t k = 0; k < A.getNumRows(); ++k)
    {
        size_t maxElementIndex = k;

        for (size_t i = k + 1; i < A.getNumRows(); ++i)
        {
            if (std::abs(A[i][k]) > std::abs(A[maxElementIndex][k]))
                maxElementIndex = i;
        }

        std::swap(A[k], A[maxElementIndex]);
        std::swap(B[k], B[maxElementIndex]);

        if (std::abs(A[k][k]) < EPS) {
            continue;
        }

        for (size_t i = k + 1; i < A.getNumRows(); ++i)
        {
            double factor = A[i][k] / A[k][k];

            B[i][0] = B[i][0] - factor * B[k][0];
            for (size_t j = k; j < A.getNumRows(); ++j)
                A[i][j] = A[i][j] - factor * A[k][j];
        }

        if (output)
        {
            *output << "Iteration " << k + 1 << ":\n";
            *output << "U(" << k + 1 << ") = \n" << A;
            *output << "C(" << k + 1 << ") = \n" << B << std::endl;
        }
    }
}

Matrix getMatrixV(const Matrix& matrix, int k)
{
    Matrix V = matrix;

    for (int i = 0; i < V.getNumRows(); ++i)
        V[i][i] = 1;

    V[k][k] = 1.0 / matrix[k][k];

    for (int i = 0; i < k; ++i)
        V[i][k] = -matrix[i][k];

    return V;
}

Vector getSolution(const Matrix& U, Matrix C, std::ostream* output = nullptr)
{
    Vector solution(U.getNumRows());

    for (int i = U.getNumRows() - 1; i >= 0; i--)
    {
        solution[i] = C[i][0] / U[i][i];
        for (int j = i - 1; j >= 0; j--) {
            C[j][0] -= U[j][i] * solution[i];
        }

        if (output)
        {
            *output << "Iteration (" << U.getNumRows() - i << "):\n";
            *output << C << std::endl;
        }
    }

    return solution;
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

void SLE::solveGaussianElimination()
{
    std::ofstream output("last_solution.txt", std::ios_base::trunc);

    if (!A.get())
        throw std::runtime_error("Matrix A does not exist");
    if (!B.get())
        throw std::runtime_error("Matrix B does not exist");
    if (B->getNumColumns() != 1)
        throw std::runtime_error("Wrong size of B matrix, matrix B should have only 1 column");
    if (A->getNumRows() != B->getNumRows())
        throw std::runtime_error("Matrices A and B should have the same number of rows");

    Matrix U = *A;
    Matrix C = *B;

    getEchelonForm(U, C, &output);

    auto solutionType = getSLESolutionType(U, C);
    if (solutionType == SLESolutionType::NoSolution)
        throw std::runtime_error("SLE has no solution");
    else if (solutionType == SLESolutionType::InfiniteSolutions)
        throw std::runtime_error("SLE has infinetly many solution");

    x = std::make_unique<Vector>(getSolution(U, C, &output));
    Matrix solution(x->size(), 1);
    for (size_t i = 0; i < x->size(); ++i)
        solution[i][0] = (*x)[i];

    Matrix lower, upper;
    A->caclulateLUDecomposition(lower, upper);
    Matrix inverse = A->calculateInverse();

    output << "LU decomposition:\n";
    output << lower << '\n' << upper << std::endl;

    output << "Inverse:\n";
    output << inverse << std::endl;

    output << "Solution: \n";
    output << solution << std::endl;
    output << "Error:\n" << calculateError(*A, *B, solution) << std::endl;
    output << "Relative error: " << calculateRelativeError(*A, *B, solution);
}

void SLE::solveGaussSeidelMethod()
{
    std::ofstream output("last_solution.txt", std::ios_base::trunc);

    Matrix U = *A;
    Matrix C = *B;

    getEchelonForm(U, C, &output);

    auto solutionType = getSLESolutionType(U, C);
    if (solutionType == SLESolutionType::NoSolution)
        throw std::runtime_error("SLE has no solution");
    else if (solutionType == SLESolutionType::InfiniteSolutions)
        throw std::runtime_error("SLE has infinetly many solution");

    x = std::make_unique<Vector>(A->getNumRows(), 5.0);

    int iteration = 0;
    double residual = 1.0;

    while ((residual > EPS) && (iteration < MAX_ITERAIONS_NUM))
    {
        for (int i = 0; i < A->getNumRows(); i++)
        {
            double sum = 0.0;
            for (int j = 0; j < A->getNumRows(); j++)
                if (j != i)
                    sum += A->at(i, j) * x->at(j);

            x->at(i) = (B->at(i, 0) - sum) / A->at(i, i);
        }

        residual = 0.0;
        for (int i = 0; i < A->getNumRows(); i++)
        {
            double sum = 0.0;
            for (int j = 0; j < A->getNumRows(); j++)
                sum += A->at(i, j) * x->at(j);

            residual += std::pow(B->at(i, 0) - sum, 2);
        }
        residual = std::sqrt(residual);

        iteration++;

        output << "Iteration (" << iteration << "):\n";
        for (size_t i = 0; i < x->size(); ++i)
            output << x->at(i) << ' ';
        output << std::endl << residual << std::endl;
        output << std::endl << std::endl;
    }

    if (iteration >= MAX_ITERAIONS_NUM)
        throw std::runtime_error("Max iterations number reached");

    Matrix solution(x->size(), 1);
    for (size_t i = 0; i < x->size(); ++i)
        solution[i][0] = (*x)[i];

    output << "Solution: \n";
    output << solution << std::endl;
    output << "Error:\n" << calculateError(*A, *B, solution) << std::endl;
    output << "Relative error: " << calculateRelativeError(*A, *B, solution);
}
