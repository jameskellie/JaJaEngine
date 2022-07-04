#ifndef CAMERA_H
#define CAMERA_H

#include "SDL/SDL_Properties.h"
#include "Vector2D/Vector2D.h"

#include "SDL_rect.h"

#include <memory>

struct SDL_Properties;

class Camera
{
    std::shared_ptr<Vector2D>  target;

public:
    std::shared_ptr<SDL_FRect> viewBox;

    Camera(const SDL_Properties &properties);

    void Update(const Vector2D &mapDimensions, const Vector2D &tileSize);

    // Setters
    inline void SetTarget(std::shared_ptr<Vector2D> target) { this->target = target; }
};

#endif // CAMERA_H