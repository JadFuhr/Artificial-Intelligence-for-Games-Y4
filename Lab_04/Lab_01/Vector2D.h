#pragma once
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <cstdio>

class Vector2D {
public:
    float x, y;

    // Constructors
    Vector2D();
    Vector2D(float xInit, float yInit);
    Vector2D(sf::Vector2f v);

    // Basic setters
    void zero();
    void set(const Vector2D& v);
    void set(float xs, float ys);
    Vector2D clone() const;

    // Angle methods
    void setAngle(float a);
    void setAngleDeg(float a);
    float getAngle() const;
    float getAngleDeg() const;
    void rotateBy(float l);

    // Arithmetic
    void add(const Vector2D& v);
    void subtract(const Vector2D& v);
    void multiply(float s);
    void divide(float s);

    // Normalization / magnitude
    void normalize();
    void setLength(float l);
    float getLength() const;

    // Vector operations
    float dot(const Vector2D& v) const;
    float cross(const Vector2D& v) const;
    float distanceTo(const Vector2D& v) const;
    Vector2D vectorTo(const Vector2D& v) const;
    void lookAt(const Vector2D& v);
    void reflect(const Vector2D& v);
    Vector2D unit() const;
    void invert();

    // Printing
    void print(bool includeEndline = true) const;

    // Operators
    Vector2D operator+ (const Vector2D& v) const;
    Vector2D operator- (const Vector2D& v) const;
    Vector2D operator* (float s) const;
    float operator* (const Vector2D& v) const; // dot product
    float operator^ (const Vector2D& v) const; // cross product
    Vector2D operator/ (float s) const;
    Vector2D operator/ (const Vector2D& v) const;
    bool operator== (const Vector2D& v) const;
    void operator= (const Vector2D& v);
    void operator+= (const Vector2D& v);
    void operator-= (const Vector2D& v);
    void operator*= (float s);
    void operator/= (float s);
    float operator[] (int i) const;

    // SFML conversion
    operator sf::Vector2f() const;
};

// Convenience function
Vector2D lgv(float x, float y);