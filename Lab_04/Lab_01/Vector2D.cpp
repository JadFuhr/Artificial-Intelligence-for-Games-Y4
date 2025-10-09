#include "Vector2D.h"

// Constructors
Vector2D::Vector2D() : x(0), y(0) {}
Vector2D::Vector2D(float xInit, float yInit) : x(xInit), y(yInit) {}
Vector2D::Vector2D(sf::Vector2f v) : x(v.x), y(v.y) {}

// Basic setters
void Vector2D::zero() { x = 0; y = 0; }
void Vector2D::set(const Vector2D& v) { x = v.x; y = v.y; }
void Vector2D::set(float xs, float ys) { x = xs; y = ys; }
Vector2D Vector2D::clone() const { return Vector2D(x, y); }

// Angle methods
void Vector2D::setAngle(float a) {
    float length = getLength();
    x = cos(a) * length;
    y = sin(a) * length;
}
void Vector2D::setAngleDeg(float a) {
    a *= 0.0174532925f;
    setAngle(a);
}
float Vector2D::getAngle() const { return atan2(y, x); }
float Vector2D::getAngleDeg() const { return getAngle() * 57.2957f; }
void Vector2D::rotateBy(float l) {
    float angle = getAngle();
    float length = getLength();
    x = cos(angle + l) * length;
    y = sin(angle + l) * length;
}

// Arithmetic
void Vector2D::add(const Vector2D& v) { x += v.x; y += v.y; }
void Vector2D::subtract(const Vector2D& v) { x -= v.x; y -= v.y; }
void Vector2D::multiply(float s) { x *= s; y *= s; }
void Vector2D::divide(float s) { x /= s; y /= s; }

// Normalization / magnitude
void Vector2D::normalize() {
    float length = getLength();
    if (length == 0) { x = 0; y = 0; return; }
    x /= length; y /= length;
}
void Vector2D::setLength(float l) {
    float length = getLength();
    if (length == 0) { x = 0; y = l; return; }
    x = (x / length) * l;
    y = (y / length) * l;
}
float Vector2D::getLength() const { return sqrt(x * x + y * y); }

// Vector operations
float Vector2D::dot(const Vector2D& v) const { return x * v.x + y * v.y; }
float Vector2D::cross(const Vector2D& v) const { return x * v.y - y * v.x; }
float Vector2D::distanceTo(const Vector2D& v) const { return vectorTo(v).getLength(); }
Vector2D Vector2D::vectorTo(const Vector2D& v) const { return Vector2D(v.x - x, v.y - y); }
void Vector2D::lookAt(const Vector2D& v) { setAngle(vectorTo(v).getAngle()); }
void Vector2D::reflect(const Vector2D& v) {
    Vector2D unitNormal = v.unit();
    *this = *this - (unitNormal * (2 * unitNormal.dot(*this)));
}
Vector2D Vector2D::unit() const {
    Vector2D u = clone();
    u.normalize();
    return u;
}
void Vector2D::invert() { x = -x; y = -y; }

// Printing
void Vector2D::print(bool includeEndline) const {
    if (includeEndline) printf("[%f,%f]\n", x, y);
    else printf("[%f,%f]", x, y);
}

// Operators
Vector2D Vector2D::operator+ (const Vector2D& v) const { return Vector2D(x + v.x, y + v.y); }
Vector2D Vector2D::operator- (const Vector2D& v) const { return Vector2D(x - v.x, y - v.y); }
Vector2D Vector2D::operator* (float s) const { return Vector2D(x * s, y * s); }
float Vector2D::operator* (const Vector2D& v) const { return dot(v); } // dot product
float Vector2D::operator^ (const Vector2D& v) const { return cross(v); } // cross product
Vector2D Vector2D::operator/ (float s) const { return Vector2D(x / s, y / s); }
Vector2D Vector2D::operator/ (const Vector2D& v) const { return Vector2D(x / v.x, y / v.y); }
bool Vector2D::operator== (const Vector2D& v) const { return x == v.x && y == v.y; }
void Vector2D::operator= (const Vector2D& v) { x = v.x; y = v.y; }
void Vector2D::operator+= (const Vector2D& v) { x += v.x; y += v.y; }
void Vector2D::operator-= (const Vector2D& v) { x -= v.x; y -= v.y; }
void Vector2D::operator*= (float s) { x *= s; y *= s; }
void Vector2D::operator/= (float s) { x /= s; y /= s; }
float Vector2D::operator[] (int i) const { return (i == 0) ? x : y; }

// SFML conversion
Vector2D::operator sf::Vector2f() const { return sf::Vector2f(x, y); }

// Convenience
Vector2D lgv(float x, float y) { return Vector2D(x, y); }