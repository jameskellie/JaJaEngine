#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Vector2D/Vector2D.h"

// This class takes an input (x, y) coord and adds it to the entity's current coordinates,
// meaning you can move the entity relative to its current location by adding and subtracting
// from its current x and y position
class Transform
{
public:
    float x,
          y;

    Transform(const float x = 0.0f, const float y = 0.0f) : x(x), y(y) {}

    inline void TranslateX(const float x)     { this->x += x; }
    inline void TranslateY(const float y)     { this->y += y; }
    inline void Translate (const Vector2D v2) { this->x += v2.x; this->y += v2.y; }
};

#endif // TRANSFORM_H
