#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Vector2D/Vector2D.h"

#include <cmath>

// This class is for physics calculations on where an entity should move to based on the given parameters
// NOTE: It is NOT what sets the x and y position of an entity
class RigidBody
{
    float mass,
          gravity,
          angle,
          totalVelocity;

    Vector2D force,
             friction,
             position,
             velocity,
             acceleration;

public:
    RigidBody()
    {
        mass    = 1.0f;
        gravity = 0.0f;
        // gravity = 9.8f;
    }

    // Getters
    inline float    GetMass()         { return mass; }
    inline Vector2D GetPosition()     { return position; }
    inline Vector2D GetVelocity()     { return velocity; }
    inline Vector2D GetAcceleration() { return acceleration; }

    // Setters
    inline void SetMass    (const float mass)     { this->mass    = mass; }
    inline void SetGravity (const float gravity)  { this->gravity = gravity; }
    inline void SetAngle   (const float angle)    { this->angle   = angle; }

    inline void ApplyForce (const Vector2D force) { this->force   = force; }
    inline void RemoveForce()                     { force         = Vector2D(0.0f, 0.0f); }

    inline void ApplyForceX(const float forceX)   { force.x       = forceX; }
    inline void ApplyForceY(const float forceY)   { force.y       = forceY; }

    inline void ApplyFriction(const Vector2D friction) { this->friction = friction; }
    inline void RemoveFriction()                       { friction       = Vector2D(0.0f, 0.0f); }

    // TODO: Rewrite this to be dynamic and not use position
    void Update(const float deltaTime)
    {
        acceleration.x = (force.x + friction.x) / mass;
        acceleration.y = gravity + force.y / mass;
        totalVelocity  = sqrt(pow(acceleration.x, 2.0f) + pow(acceleration.y, 2.0f));
        angle          = asin(acceleration.y / totalVelocity) * (180.0f / (float)M_PI);
        velocity       = acceleration;

        if (0 <= abs(angle) && abs(angle) <= 90)
        {
            velocity.x = velocity.x / (1.0f / cos(abs(angle * ((float)M_PI / 180.0f))));
            velocity.y = velocity.y / (1.0f / sin(abs(angle * ((float)M_PI / 180.0f))));
        }

        position = velocity * deltaTime;
    }
};

#endif // RIGIDBODY_H
