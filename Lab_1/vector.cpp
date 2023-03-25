#include "vector.h"

#include <filesystem>
#include <stdexcept>
#include <fstream>
#include <cmath>

Vector::Vector(int size)
{
    if (size <= 0)
        throw std::invalid_argument("Invalid vector size");

    data = std::vector(size, 0.0);
}

Vector::Vector(int size, double value)
{
    if (size <= 0)
        throw std::invalid_argument("Invalid vector size");

    data = std::vector(size, value);
}

double& Vector::operator[](size_t index)
{
    return data[index];
}

double Vector::operator[](size_t index) const
{
    return data[index];
}

size_t Vector::size() const
{
    return data.size();
}

void Vector::pushBack(double value)
{
    data.push_back(value);
}

void Vector::insert(size_t index, double value)
{
    checkIndex(index);

    auto it = data.cbegin() + index;
    data.insert(it, value);
}

void Vector::remove(size_t index)
{
    checkIndex(index);

    auto it = data.cbegin() + index;
    data.erase(it);
}

double Vector::calculateEuclidianNorm() const
{
    double result = 0.0;

    for (const auto& value : data)
        result += std::pow(value, 2);

    return std::sqrt(result);
}

Vector Vector::readFromFile(const std::string &filepath)
{
    if (!std::filesystem::exists(filepath))
        throw std::invalid_argument("File doesn't exist");

    std::ifstream file(filepath);
    if (!file.is_open())
        throw std::invalid_argument("Failed to open the file");

    Vector vector;

    std::string line;
    while(std::getline(file, line))
        vector.data.push_back(std::stod(line));

    return vector;
}

void Vector::writeToFile(const Vector &vector, const std::string &filepath)
{
    std::ofstream file(filepath);
    if (!file.is_open())
        throw std::invalid_argument("Failed to create a file");

    for (const auto& value : vector.data)
        file << value << std::endl;
}

void Vector::checkIndex(size_t index)
{
    if (index >= data.size())
        throw std::out_of_range("Vector: Index out of range");
}

Vector operator+(const Vector& lhs, const Vector& rhs)
{
    if (lhs.size() != rhs.size())
        throw std::invalid_argument("Vectors have different sizes");

    Vector result(lhs.size());

    for (size_t index = 0; index < lhs.size(); ++index)
        result[index] = lhs.data[index] + lhs.data[index];

    return result;
}

Vector operator-(const Vector& lhs, const Vector& rhs)
{
    if (lhs.size() != rhs.size())
        throw std::invalid_argument("Vectors have different sizes");

    Vector result(lhs.size());

    for (size_t index = 0; index < lhs.size(); ++index)
        result[index] = lhs.data[index] - lhs.data[index];

    return result;
}

Vector operator*(const Vector& vector, double value)
{
    Vector result(vector.size());

    for (size_t index = 0; index < vector.size(); ++index)
        result[index] = vector.data[index] * value;

    return result;
}
