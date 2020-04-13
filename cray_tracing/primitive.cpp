#include "primitive.hpp"
#include <iostream>

const int FILL = 2;
const int SQUARED = 3;
const int DISTANT = 4;
const int SPHERE = 6;
const int PLANE = 7;
const int POLYGON = 8;
const std::string POINT = "point";

Intersection Primitive::intersect(Ray &ray) {return {{0, 0, 0}, -1, Vector(0, 0, 0), this};}

Sphere::Sphere() {}

Sphere::Sphere(const Vector center, double radius, const Properties &properties) {
    type = SPHERE;
    c = center;
    r = radius;
    props = properties;
}

Vector Sphere::normal(const Vector &point) {
    return (point - c).normal();
}

Intersection Sphere::intersect(Ray &ray) {
    Vector c_o = ray.o - c;
    double disc = r * r - (c_o.dot(c_o) - ray.d.dot(c_o) * ray.d.dot(c_o));
    if (disc < 0) {
        return {{0, 0, 0}, -1, Vector(0, 0, 0), this};
    }

    double b = -ray.d.dot(c_o);
    double d1 = b - sqrt(disc);
    double d2 = b + sqrt(disc);
    double d = -1;
    if (d1 > 0 && (d2 > d1 || d2 < 0)) {
        d = d1;
    } else if (d2 > 0 && (d1 > d2 || d1 < 0)) {
        d = d2;
    } else {
        return {{0, 0, 0}, -1, Vector(0, 0, 0), this};
    }

    Vector point = ray.cast(d);
    return {point, d, this->normal(point), this};
}

Plane::Plane() {}

Plane::Plane(const Vector point, const Vector normal, const Properties &properties) {
    type = PLANE;
    p = point;
    n = normal;
    props = properties;
}

Intersection Plane::intersect(Ray &ray) {
    double cs = n.dot(ray.d);
    if (fabs(cs) < eps || cs > 0) {
        return {{0, 0, 0}, -1, Vector(0, 0, 0), this};
    }
    double d = (p -  ray.o).dot(n) / cs;

    Vector point = ray.cast(d);
    return {point, d, n, this};
}

Triangle::Triangle() {}

Triangle::Triangle(const Vector point_1, const Vector point_2, const Vector point_3, const Properties &properties) {
    type = POLYGON;
    p1 = point_1;
    p2 = point_2;
    p3 = point_3;
    n = (p2 - p1).cross(p3 - p1).normal();
    pl = Plane(p1, n, properties);
    sq = (p2 - p1).cross(p3 - p1).len() / 2;
    props = properties;
}

bool Triangle::is_point_inside(const Vector &p) {
    if (fabs((p - p1).dot(n)) > eps) {
        return false;
    }
    Vector p_p1 = p1 - p;
    Vector p_p2 = p2 - p;
    Vector p_p3 = p3 - p;
    double square = (p_p1.cross(p_p2).len() + p_p2.cross(p_p3).len() + p_p3.cross(p_p1).len()) / 2;
    return !(fabs(sq - square) > eps);
}

Intersection Triangle::intersect(Ray &ray) {
    auto orig = pl.n;
    pl.n = oriented(ray.d * (-1), pl.n);
    Intersection inter = pl.intersect(ray);
    pl.n = orig;
    if (inter.d == -1 || !is_point_inside(inter.p) || ray.d.dot(inter.p - ray.o) < 0) {
        return {{0, 0, 0}, -1, Vector(0, 0, 0), this};
    } else {
        inter.obj = this;
        return inter;
    }
}

std::istream& operator>>(std::istream& input_stream, Primitive* &prim) {
    std::string type;
    input_stream >> type;
    if (type == "Sphere") {
        Sphere *obj = new Sphere;
        input_stream >> *obj;
        prim = obj;
    } else if (type == "Plane") {
        Plane *obj = new Plane;
        input_stream >> *obj;
        prim = obj;
    } else if (type == "Triangle") {
        Triangle *obj = new Triangle;
        input_stream >> *obj;
        prim = obj;
    }

    return input_stream;
}

std::istream& operator>>(std::istream& input_stream, Sphere &sph) {
    Vector c;
    double r;
    Properties props;
    input_stream >> c >> r >> props;
    sph = Sphere(c, r, props);
    return input_stream;
}

std::istream& operator>>(std::istream& input_stream, Plane &pl) {
    Vector p, n;
    Properties props;
    input_stream >> p >> n >> props;
    pl = Plane(p, n, props);
    return input_stream;
}

std::istream& operator>>(std::istream& input_stream, Triangle &triag) {
    Vector p1, p2, p3;
    Properties props;
    input_stream >> p1 >> p2 >> p3 >> props;
    triag = Triangle(p1, p2, p3, props);
    return input_stream;
}
