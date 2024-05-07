#pragma once
#include <cmath>

class Vector2 {
public:
    float x, y;

    Vector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

    Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }

    Vector2 operator*(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    Vector2 operator/(float scalar) const {
        if (scalar == 0.0f) {
            //throw runtime error "Division by zero";
        }
        return Vector2(x / scalar, y / scalar);
    }

    Vector2& operator+=(const Vector2& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Vector2& operator-=(const Vector2& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    Vector2& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    float Length() const {
        return std::sqrt(x * x + y * y);
    }

    Vector2 Normalised() const {
        float length = Length();
        if (length == 0) return Vector2(0.0f, 0.0f);
        return Vector2(x / length, y / length);
    }

    static float Dot(const Vector2& a, const Vector2& b) {
        return a.x * b.x + a.y * b.y;
    }

    bool operator==(const Vector2& other) const {
        return (x == other.x) && (y == other.y);
    }

    // adding more operations as I go

};