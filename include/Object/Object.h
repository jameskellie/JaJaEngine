#ifndef OBJECT_H
#define OBJECT_H

#include "Vector2D/Vector2D.h"

#include "SDL_rect.h"

#include <string>

struct Object
{
    SDL_FRect   hitbox;
    std::string loadZone;
    Vector2D    setMapPos;

    Object() {}

    Object(const SDL_FRect &hitbox)
        : hitbox (hitbox) {}
};

#endif // OBJECT_H