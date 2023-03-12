#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <string>

class Vector;

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
    Matrix& operator+(const Matrix& other);
    Matrix& operator-(const Matrix& other);
    Matrix& operator*(const Matrix& other);
    Vector& operator[](size_t index);
    Vector operator[](size_t index) const;

    size_t getNumRows() const;
    size_t getNumColumns() const;

    void addRow();
    void addColumn();
    void removeRow();
    void removeColumn();

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
