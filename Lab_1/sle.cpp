#include "sle.h"

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
    // TODO: SLE::solve
}
