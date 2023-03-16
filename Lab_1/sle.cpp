#include "sle.h"

#include <fstream>

namespace
{
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

    void getEchelonForm(Matrix& A, Matrix& B)
    {
        for (int k = 0; k < A.getNumRows(); ++k)
        {
            Matrix T = getMatrixT(A, k);
            A = T * A;
            B = T * B;
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

    Vector getSolution(Matrix& U, Matrix& C)
    {
        Vector soultion(C.getNumRows());

        for (int k = (U.getNumRows() - 1); k >= 0; --k)
        {
            Matrix V = getMatrixV(U, k);
            U = V * U;
            C = V * C;
        }

        for (int i = 0; i < C.getNumRows(); ++i)
            soultion[i] = C[i][0];

        return soultion;
    }
}

SLE::SLE()
{

}

void SLE::setMatrixA(const Matrix &matrix)
{
    A = matrix;
}

Matrix SLE::getMatrixA() const
{
    return A;
}

Matrix &SLE::getMatrixA()
{
    return A;
}

void SLE::setMatrixB(const Matrix &matrix)
{
    B = matrix;
}

Matrix SLE::getMatrixB() const
{
    return B;
}

Matrix &SLE::getMatrixB()
{
    return B;
}

Vector SLE::getVectorX() const
{
    return x;
}

Vector &SLE::getVectorX()
{
    return x;
}

void SLE::solve()
{
    std::ofstream output("last_solution.txt", std::ios_base::trunc);

    Matrix U(A);
    Matrix C(B);
    getEchelonForm(U, C);
    x = getSolution(U, C);
}
