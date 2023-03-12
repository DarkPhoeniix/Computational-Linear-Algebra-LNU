#include "matrix.h"

#include "vector.h"

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

Matrix& Matrix::operator+(const Matrix& other)
{
    if ((getNumRows() != other.getNumRows()) || (getNumColumns() != other.getNumColumns()))
        throw std::invalid_argument("Matrices have different sizes");

    for (size_t rowIndex = 0; rowIndex < getNumRows(); ++rowIndex)
        for (size_t colIndex = 0; colIndex < getNumColumns(); ++colIndex)
            matrixValues[rowIndex][colIndex] += other[rowIndex][colIndex];

    return *this;
}

Matrix& Matrix::operator-(const Matrix& other)
{
    if ((getNumRows() != other.getNumRows()) || (getNumColumns() != other.getNumColumns()))
        throw std::invalid_argument("Matrices have different sizes");

    for (size_t rowIndex = 0; rowIndex < getNumRows(); ++rowIndex)
        for (size_t colIndex = 0; colIndex < getNumColumns(); ++colIndex)
            matrixValues[rowIndex][colIndex] -= other[rowIndex][colIndex];

    return *this;
}

Matrix& Matrix::operator*(const Matrix& other)
{
    if (getNumColumns() != other.getNumRows())
        throw std::invalid_argument("Can't multiply matrices with given sizes");

    for (size_t rowIndex = 0; rowIndex < getNumRows(); ++rowIndex)
        for (size_t colIndex = 0; colIndex < other.getNumColumns(); ++colIndex)
            for (size_t k = 0; k < getNumColumns(); ++k)
                matrixValues[rowIndex][colIndex] += matrixValues[rowIndex][k] * other[k][colIndex];

    return *this;
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
    // TODO: calculate euclidian norm for matrix
    return 0.0;
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
