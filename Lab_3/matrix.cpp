#include "matrix.h"

#include "vector.h"

#include <cmath>
#include <random>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <filesystem>

namespace
{
Vector readMatrixRow(const std::string& line)
{
    Vector values;

    std::istringstream ss(line);
    double num = 0.0;
    while (ss >> num)
        values.pushBack(num);

    return values;
}
}

Matrix::Matrix(int numRows, int numCols, int value)
{
    if ((numRows <= 0) || (numCols <= 0))
        throw std::invalid_argument("Invalid size of matrix");

    data = std::vector<Vector>(numRows, Vector(numCols, 0.0));
    for (size_t i = 0; (i < numRows) && (i < numCols); ++i)
        data[i][i] = value;
}

Vector& Matrix::operator[](size_t index)
{
    if (index >= data.size())
        throw std::out_of_range("Matrix index is out of range");

    return data[index];
}

Vector Matrix::operator[](size_t index) const
{
    if (index >= data.size())
        throw std::out_of_range("Matrix index is out of range");

    return data[index];
}

double& Matrix::at(size_t rowIndex, size_t columnIndex)
{
    if (rowIndex >= data.size())
        throw std::out_of_range("Matrix index is out of range");

    return data[rowIndex][columnIndex];
}

double Matrix::at(size_t rowIndex, size_t columnIndex) const
{
    if (rowIndex >= data.size())
        throw std::out_of_range("Matrix index is out of range");

    return data[rowIndex][columnIndex];
}

size_t Matrix::getNumRows() const
{
    return data.size();
}

size_t Matrix::getNumColumns() const
{
    return data.front().size();
}

void Matrix::addRow(size_t index, double value /*= 0.0*/)
{
    checkRowIndex(index);

    size_t colsNumber = this->data.front().size();
    auto it = data.cbegin()+index;
    this->data.insert(it, Vector(colsNumber, value));
}

void Matrix::addColumn(size_t index, double value /*= 0.0*/)
{
    checkColumnIndex(index);

    for (auto& row : this->data)
        row.insert(index, value);
}

void Matrix::removeRow(size_t index)
{
    checkRowIndex(index);

    auto it = data.cbegin() + index;
    data.erase(it);
}

void Matrix::removeColumn(size_t index)
{
    checkColumnIndex(index);

    for (auto& row : data)
        row.remove(index);
}

void Matrix::reset(int numRows, int numColumns)
{
    if ((numRows <= 0) || (numColumns <= 0))
        throw std::invalid_argument("Invalid size of matrix");

    this->data = std::vector<Vector>(numRows, Vector(numColumns, 0.0));
}

void Matrix::randomize(double leftBorder, double rightBorder)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(leftBorder, rightBorder);

    for (size_t rowIndex = 0; rowIndex < getNumRows(); ++rowIndex)
    {
        for (size_t columnIndex = 0; columnIndex < getNumColumns(); ++columnIndex)
        {
            double randomValue = dis(gen);
            data[rowIndex][columnIndex] = static_cast<double>(std::round(randomValue * 100)) / 100;
        }
    }
}

void Matrix::caclulateLUDecomposition(Matrix& L, Matrix& U) const
{
    int size = getNumRows();

    L = Matrix(size, size);
    U = Matrix(size, size);

    for (int j = 0; j < size; j++)
    {
        for (int i = 0; i < size; i++)
        {
            if (i <= j)
            {
                double sum = 0.0;
                for (int k = 0; k < i; k++)
                    sum += L[i][k] * U[k][j];

                U[i][j] = data[i][j] - sum;
            }

            if (i >= j)
            {
                if (j == 0)
                {
                    L[i][j] = data[i][j] / U[j][j];
                }
                else
                {
                    double sum = 0.0;
                    for (int k = 0; k < j; k++)
                        sum += L[i][k] * U[k][j];

                    L[i][j] = (data[i][j] - sum) / U[j][j];
                }
            }
        }
    }
}

Matrix Matrix::calculateInverse() const
{
    Matrix L, U;
    caclulateLUDecomposition(L, U);

    int size = getNumRows();

    Matrix I(size, size, 1.0);
    Matrix Z(size, size);
    Matrix inverse(size, size);

    for (int col = 0; col < size; col++)
    {
        for (int row = 0; row < size; row++)
        {
            double sum = 0;
            for (int i = 0; i < size; i++)
                if (i != row)
                    sum += L[row][i] * Z[i][col];

            double result = I[row][col] - sum;
            Z[row][col] = result / L[row][row];
        }
    }

    for (int col = 0; col < size; col++) {
        for (int row = size - 1; row >= 0; row--) {
            double sum = 0;
            for (int i = 0; i < size; i++) {
                if (i != row) {
                    sum += U[row][i] * inverse[i][col];
                }
            }

            double result = Z[row][col] - sum;
            inverse[row][col] = result / U[row][row];
        }
    }

    return inverse;
}

double Matrix::calculateEuclidianNorm() const
{
    double norm = 0;
    for (int i = 0; i < getNumRows(); i++)
    {
        for (int j = 0; j < getNumColumns(); j++)
        {
            norm += std::pow(data[i][j], 2);
        }
    }

    return std::sqrt(norm);
}

Matrix Matrix::readFromFile(const std::string& filename)
{
    Matrix matrix;

    if (!std::filesystem::exists(filename))
        throw std::invalid_argument("Failed to open the file: " + filename);

    std::ifstream file(filename);
    if (!file.is_open())
        throw std::invalid_argument("Failed to open the file: " + filename);

    std::string line;
    while(std::getline(file, line))
        if (!line.empty())
            matrix.data.push_back(readMatrixRow(line));

    return matrix;
}

void Matrix::writeToFile(const Matrix &matrix, const std::string& filename)
{
    std::ofstream file(filename, std::ios_base::app);
    if (!file.is_open())
        throw std::invalid_argument("Failed to create a file: " + filename);

    for (const auto& row : matrix.data)
    {
        for (size_t index = 0; index < row.size(); ++index)
        {
            file << row[index];
            if (index != (row.size() - 1))
                file << ' ';
        }
        file << std::endl;
    }
}

Matrix operator+(const Matrix& lhs, const Matrix& rhs)
{
    if ((lhs.getNumRows() != rhs.getNumRows()) || (lhs.getNumColumns() != rhs.getNumColumns()))
        throw std::invalid_argument("Matrices have different sizes");

    Matrix result(lhs.getNumRows(), lhs.getNumColumns());

    for (size_t rowIndex = 0; rowIndex < lhs.getNumRows(); ++rowIndex)
        for (size_t colIndex = 0; colIndex < lhs.getNumColumns(); ++colIndex)
            result[rowIndex][colIndex] = lhs[rowIndex][colIndex] + rhs[rowIndex][colIndex];

    return result;
}

Matrix operator-(const Matrix& lhs, const Matrix& rhs)
{
    if ((lhs.getNumRows() != rhs.getNumRows()) || (lhs.getNumColumns() != rhs.getNumColumns()))
        throw std::invalid_argument("Matrices have different sizes");

    Matrix result(lhs.getNumRows(), lhs.getNumColumns());

    for (size_t rowIndex = 0; rowIndex < lhs.getNumRows(); ++rowIndex)
        for (size_t colIndex = 0; colIndex < lhs.getNumColumns(); ++colIndex)
            result[rowIndex][colIndex] = lhs[rowIndex][colIndex] - rhs[rowIndex][colIndex];

    return result;
}

Matrix operator*(const Matrix& lhs, const Matrix& rhs)
{
    if (lhs.getNumColumns() != rhs.getNumRows())
        throw std::invalid_argument("Can't multiply matrices with given sizes");

    Matrix result(lhs.getNumRows(), rhs.getNumColumns());

    for (size_t rowIndex = 0; rowIndex < lhs.getNumRows(); ++rowIndex)
        for (size_t colIndex = 0; colIndex < rhs.getNumColumns(); ++colIndex)
            for (size_t k = 0; k < lhs.getNumColumns(); ++k)
                result[rowIndex][colIndex] += lhs[rowIndex][k] * rhs[k][colIndex];

    return result;
}

std::ostream& operator<<(std::ostream& output, const Matrix& matrix)
{
    for (const auto& row : matrix.data)
    {
        for (size_t index = 0; index < row.size(); ++index)
        {
            output << row[index];
            if (index != (row.size() - 1))
                output << ' ';
        }
        output << std::endl;
    }

    return output;
}

std::istream& operator>>(std::istream& input, Matrix& matrix)
{
    std::string line;
    while(std::getline(input, line))
        if (!line.empty())
            matrix.data.push_back(readMatrixRow(line));

    return input;
}

void Matrix::checkIndex(int rowIndex, int columnIndex) const
{
    if (!checkRowIndex(rowIndex) || !checkColumnIndex(columnIndex))
        throw std::out_of_range("Invalid index of the matrix element: [" + std::to_string(rowIndex) + ", " + std::to_string(columnIndex) + ']');
}

bool Matrix::checkRowIndex(int index) const
{
    return (index >= 0) && (index < this->data.size());
}

bool Matrix::checkColumnIndex(int index) const
{
    return (index >= 0) && (index < this->data.front().size());
}
