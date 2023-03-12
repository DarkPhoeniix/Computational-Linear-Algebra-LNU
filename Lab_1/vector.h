#ifndef VECTOR_H
#define VECTOR_H

#include <vector>
#include <string>

class Vector
{
public:
    Vector() = default;
    explicit Vector(int size);
    Vector(int size, double value);
    Vector(const Vector&) = default;
    Vector(Vector&& other) = default;

    Vector& operator=(const Vector&) = default;
    Vector& operator=(Vector&&) = default;
    Vector& operator+(const Vector& other);
    Vector& operator-(const Vector& other);
    Vector& operator*(double value);
    double& operator[](size_t index);
    double operator[](size_t index) const;

    size_t getSize() const;

    void addValue(double value);
    void removeValue();

    double calculateEuclidianNorm() const;

    static Vector readFromFile(const std::string& filepath);
    static void writeToFile(const Vector& vector, const std::string& filepath);

private:
    std::vector<double> vectorValues;
};

#endif // VECTOR_H
