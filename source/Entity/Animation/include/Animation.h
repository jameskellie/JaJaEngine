#ifndef ANIMATION_H
#define ANIMATION_H

#include "Sequence/Sequence.h"

#include "SDL_render.h"

#include <unordered_map>
#include <memory>
#include <string>

class Resources;

class Animation
{
    int frame;

    // Texture ID
    std::string id,
                state;

    std::unordered_map<std::string, Sequence> states;

public:
    Animation(const std::string id, const std::unordered_map<std::string, Sequence> &states);

    void SetState(const std::string state);
    // Pass through which row the image uses and how many frames the target image has and this function handles iterating through it at the animation speed
    void Draw(std::shared_ptr<Resources> resources, const float x, const float y, const float tileWidth, const float tileHeight);
    void Update();
};

#endif // ANIMATION_H