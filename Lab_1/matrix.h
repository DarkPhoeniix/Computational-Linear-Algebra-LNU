#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

#include <vector>
#include <string>

using matrix_t = std::vector<Vector>;

class Matrix
{
public:
    Matrix() = default;
    Matrix(int numRows, int numCols);
    Matrix(const Matrix&) = default;
    Matrix(Matrix&&) = default;

    Matrix& operator=(const Matrix&) = default;
    Matrix& operator=(Matrix&&) = default;
    friend Matrix operator+(const Matrix& lhs, const Matrix& rhs);
    friend Matrix operator-(const Matrix& lhs, const Matrix& rhs);
    friend Matrix operator*(const Matrix& lhs, const Matrix& rhs);
    friend std::ostream& operator<<(std::ostream& output, const Matrix& matrix);
    friend std::istream& operator>>(std::istream& output, Matrix& matrix);
    Vector& operator[](size_t index);
    Vector operator[](size_t index) const;

    double& at(size_t rowIndex, size_t columnIndex);
    double at(size_t rowIndex, size_t columnIndex) const;

    size_t getNumRows() const;
    size_t getNumColumns() const;

    void addRow();
    void addColumn();
    void removeRow();
    void removeColumn();

    void reset(int numRows, int numColumns);

    void randomize(double leftBorder, double rightBorder);

    double calculateEuclidianNorm() const;

    static Matrix readFromFile(const std::string& filename);
    static void writeToFile(const Matrix& matrix, const std::string& filename);

private:
    void checkIndex(int rowIndex, int columnIndex) const;
    bool checkRowIndex(int index) const;
    bool checkColumnIndex(int index) const;

    matrix_t matrixValues;
};

#endif // MATRIX_H
