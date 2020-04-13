#ifndef VECTOR
#define VECTOR

#include <iostream>

extern const double eps;

struct Vector {
    double x;
    double y;
    double z;

    Vector();
    Vector(double x_, double y_, double z_);

    double len() const;
    Vector normal() const;

    double dot(const Vector &other) const;
    Vector cross(const Vector &other) const;
};

Vector operator+(const Vector &first, const Vector &second);
Vector operator+=(Vector &first, const Vector &second);
Vector operator-(const Vector &first, const Vector &second);
Vector operator*(const Vector &first, const Vector &second);
Vector operator*=(Vector &first, const Vector &second);
Vector operator*(const Vector &first, const double k);
Vector operator*=(Vector &first, const double k);
Vector operator/(const Vector &first, const Vector &second);
Vector operator/(const Vector &first, const double k);
bool operator==(const Vector &first, const Vector &second);
bool operator<(const Vector &first, const Vector &second);

std::ostream& operator<<(std::ostream& output_stream, const Vector& vector);
std::istream& operator>>(std::istream& input_stream, Vector &vec);

Vector oriented(Vector axis, Vector other);

Vector rotx(const Vector vec, double ang);
Vector roty(const Vector vec, double ang);
Vector rotz(const Vector vec, double ang);
Vector rotate(const Vector vec, double dx, double dy, double dz);
Vector rotate(const Vector vec, const Vector rotation);


#endif // VECTOR
