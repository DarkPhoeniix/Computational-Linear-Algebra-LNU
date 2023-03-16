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
            values.addValue(num);

        return values;
    }
}

Matrix::Matrix(int numRows, int numCols)
{
    if ((numRows <= 0) || (numCols <= 0))
        throw std::invalid_argument("Invalid size of matrix");

    this->matrixValues = std::vector<Vector>(numRows, Vector(numCols, 0.0));
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

Vector& Matrix::operator[](size_t index)
{
    return matrixValues[index];
}

Vector Matrix::operator[](size_t index) const
{
    return matrixValues[index];
}

size_t Matrix::getNumRows() const
{
    return matrixValues.size();
}

size_t Matrix::getNumColumns() const
{
    return matrixValues.front().getSize();
}

void Matrix::addRow()
{
    size_t colsNumber = this->matrixValues.front().getSize();
    this->matrixValues.push_back(Vector(colsNumber, 0.0));
}

void Matrix::addColumn()
{
    for (auto& row : this->matrixValues)
        row.addValue(0.0);
}

void Matrix::removeRow()
{
    matrixValues.erase(matrixValues.end() - 1);
}

void Matrix::removeColumn()
{
    for (auto& row : matrixValues)
        row.removeValue();
}

void Matrix::reset(int numRows, int numColumns)
{
    if ((numRows <= 0) || (numColumns <= 0))
        throw std::invalid_argument("Invalid size of matrix");

    this->matrixValues = std::vector<Vector>(numRows, Vector(numColumns, 0.0));
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
            matrixValues[rowIndex][columnIndex] = static_cast<double>(std::round(randomValue * 100)) / 100;
        }
    }
}

double Matrix::calculateEuclidianNorm() const
{
    double norm = 0;
    for (int i = 0; i < getNumRows(); i++)
    {
        for (int j = 0; j < getNumColumns(); j++)
        {
            norm += std::pow(matrixValues[i][j], 2);
        }
    }

    return std::sqrt(norm);
}

Matrix Matrix::readFromFile(const std::string& filename)
{
    Matrix matrix;

    if (!std::filesystem::exists(filename))
        throw std::invalid_argument("File doesn't exist");

    std::ifstream file(filename);
    if (!file.is_open())
        throw std::invalid_argument("Failed to open the file");

    std::string line;
    while(std::getline(file, line))
        if (!line.empty())
            matrix.matrixValues.push_back(readMatrixRow(line));

    return matrix;
}

void Matrix::writeToFile(const Matrix &matrix, const std::string& filename)
{
    std::ofstream file(filename, std::ios_base::app);
    if (!file.is_open())
        throw std::invalid_argument("Failed to create a file");

    for (const auto& row : matrix.matrixValues)
    {
        for (size_t index = 0; index < row.getSize(); ++index)
        {
            file << row[index];
            if (index != (row.getSize() - 1))
                file << ' ';
        }
        file << std::endl;
    }
}

void Matrix::checkIndex(int rowIndex, int columnIndex) const
{
    if (!checkRowIndex(rowIndex) || !checkColumnIndex(columnIndex))
        throw std::out_of_range("Invalid index of the matrix element: [" + std::to_string(rowIndex) + ", " + std::to_string(columnIndex) + ']');
}

bool Matrix::checkRowIndex(int index) const
{
    return (index >= 0) && (index < this->matrixValues.size());
}

bool Matrix::checkColumnIndex(int index) const
{
    return (index >= 0) && (index < this->matrixValues.front().getSize());
}
