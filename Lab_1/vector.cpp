#include "vector.h"

#include <filesystem>
#include <stdexcept>
#include <fstream>
#include <cmath>

Vector::Vector(int size)
{
    if (size <= 0)
        throw std::invalid_argument("Invalid vector size");

    vectorValues = std::vector(size, 0.0);
}

Vector::Vector(int size, double value)
{
    if (size <= 0)
        throw std::invalid_argument("Invalid vector size");

    vectorValues = std::vector(size, value);
}

Vector& Vector::operator+(const Vector& other)
{
    if (getSize() != other.getSize())
        throw std::invalid_argument("Vectors have different sizes");

    for (size_t index = 0; index < getSize(); ++index)
        vectorValues[index] += other.vectorValues[index];

    return *this;
}

Vector& Vector::operator-(const Vector& other)
{
    if (getSize() != other.getSize())
        throw std::invalid_argument("Vectors have different sizes");

    for (size_t index = 0; index < getSize(); ++index)
        vectorValues[index] += other.vectorValues[index];

    return *this;
}

Vector& Vector::operator*(double value)
{
    for (size_t index = 0; index < getSize(); ++index)
        vectorValues[index] *= value;

    return *this;
}

double& Vector::operator[](size_t index)
{
    return vectorValues[index];
}

double Vector::operator[](size_t index) const
{
    return vectorValues[index];
}

size_t Vector::getSize() const
{
    return vectorValues.size();
}

void Vector::addValue(double value)
{
    vectorValues.push_back(value);
}

void Vector::removeValue()
{
    vectorValues.erase(vectorValues.end() - 1);
}

double Vector::calculateEuclidianNorm() const
{
    double result = 0.0;

    for (const auto& value : vectorValues)
        result + std::pow(value, 2);

    return std::sqrt(result);
}

Vector Vector::readFromFile(const std::string &filepath)
{
    Vector vector;

    if (!std::filesystem::exists(filepath))
        throw std::invalid_argument("File doesn't exist");

    std::ifstream file(filepath);
    if (!file.is_open())
        throw std::invalid_argument("Failed to open the file");

    std::string line;
    while(std::getline(file, line))
        vector.vectorValues.push_back(std::stod(line));

    return vector;
}

void Vector::writeToFile(const Vector &vector, const std::string &filepath)
{
    std::ofstream file(filepath);
    if (!file.is_open())
        throw std::invalid_argument("Failed to create a file");

    for (const auto& value : vector.vectorValues)
    {
        file << value << std::endl;
    }
}
