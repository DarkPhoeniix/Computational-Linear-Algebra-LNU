#ifndef SLE_H
#define SLE_H

#include "matrix.h"
#include "vector.h"

class SLE
{
public:
    SLE();
    SLE(const Matrix& A, const Matrix& B, const Vector& x);

    void setMatrixA(const Matrix& matrix);
    Matrix getMatrixA() const;
    Matrix& getMatrixA();

    void setMatrixB(const Matrix& matrix);
    Matrix getMatrixB() const;
    Matrix& getMatrixB();

    Vector getVectorX() const;
    Vector& getVectorX();

    void solve();

private:
    Matrix A;
    Matrix B;
    Vector x;
};

#endif // SLE_H
