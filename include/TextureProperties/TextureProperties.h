#ifndef TEXTUREPROPERTIES_H
#define TEXTUREPROPERTIES_H

#include "SDL_render.h"

#include <memory>
#include <cstdint>
#include <string>

struct TextureProperties
{
    std::string id;

    float x,
          y,
          tileWidth,
          tileHeight;

    int row,
        column;
    
    float                       scrollSpeed,
                                scaleX,
                                scaleY;

    uint8_t                     opacity;

    SDL_RendererFlip            flip;
    
    double                      angle;
    
    std::shared_ptr<SDL_FPoint> center;

    TextureProperties(const std::string                 id,
                      const float                       x,
                      const float                       y,
                      const float                       tileWidth,
                      const float                       tileHeight,
                      const int                         row         = 0,
                      const int                         column      = 0,
                      const SDL_RendererFlip            flip        = SDL_FLIP_NONE,
                      const float                       scrollSpeed = 1.0f,
                      const uint8_t                     opacity     = 255,
                      const float                       scaleX      = 1.0f,
                      const float                       scaleY      = 1.0f,
                      const double                      angle       = 0.0,
                      const std::shared_ptr<SDL_FPoint> center      = nullptr)
        : id         (id)
        , x          (x)
        , y          (y)
        , tileWidth  (tileWidth)
        , tileHeight (tileHeight)
        , row        (row)
        , column     (column)
        , scrollSpeed(scrollSpeed)
        , scaleX     (scaleX)
        , scaleY     (scaleY)
        , opacity    (opacity)
        , flip       (flip)
        , angle      (angle)
        , center     (center) {}
};

#endif // TEXTUREPROPERTIES_H