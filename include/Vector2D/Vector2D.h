#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <compare>

// A simple data structure for storing 2 float values
struct Vector2D
{
public:
    float x,
          y;

    Vector2D(const float x = 0.0f, const float y = 0.0f): x(x), y(y) {}

    inline Vector2D operator + (const Vector2D &v)  const { return Vector2D(x + v.x, y + v.y); }
    inline Vector2D operator - (const Vector2D &v)  const { return Vector2D(x - v.x, y - v.y); }
    inline Vector2D operator * (const float scalar) const { return Vector2D(x * scalar, y * scalar); }

    auto operator <=> (const Vector2D& v) const = default;
    
    inline friend void operator += (Vector2D &v1, const Vector2D &v2) { v1.x += v2.x; v1.y += v2.y; }
    inline friend void operator -= (Vector2D &v1, const Vector2D &v2) { v1.x -= v2.x; v1.y -= v2.y; }
    inline friend void operator *= (Vector2D &v1, const Vector2D &v2) { v1.x *= v2.x; v1.y *= v2.y; }
};

#endif // VECTOR2D_H
