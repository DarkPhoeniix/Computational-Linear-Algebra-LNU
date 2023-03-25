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
    double& operator[](size_t index);
    double operator[](size_t index) const;

    size_t size() const;

    void pushBack(double value);
    void insert(size_t index, double value);
    void remove(size_t index);

    double calculateEuclidianNorm() const;

    static Vector readFromFile(const std::string& filepath);
    static void writeToFile(const Vector& vector, const std::string& filepath);

    friend Vector operator+(const Vector& lhs, const Vector& rhs);
    friend Vector operator-(const Vector& lhs, const Vector& rhs);
    friend Vector operator*(const Vector& vector, double value);

private:
    void checkIndex(size_t index);

    std::vector<double> data;
};

#endif // VECTOR_H
