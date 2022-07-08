#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "SDL_render.h"

#include <string>

struct Sequence
{
    int row,            // How many rows of frames
        frame,          // The current frame
        frames,         // How many frames per row
        animationSpeed; // Time between each frame of the animation
    
    uint8_t opacity;
    
    float scaleX,
          scaleY;

    SDL_RendererFlip flip;

    Sequence() {}

    Sequence(const int              row,
             const int              frames         = 1,
             const int              animationSpeed = 0,
             const SDL_RendererFlip flip           = SDL_FLIP_NONE,
             const uint8_t          opacity        = 255,
             const float            scaleX         = 1.0f,
             const float            scaleY         = 1.0f)
        : row           (row)
        , frames        (frames)
        , animationSpeed(animationSpeed)
        , opacity       (opacity)
        , scaleX        (scaleX)
        , scaleY        (scaleY)
        , flip          (flip) {}
};

#endif // SEQUENCE_H