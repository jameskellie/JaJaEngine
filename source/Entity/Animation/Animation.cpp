#include "Animation.h"

#include "Resources.h"

#include "Sequence/Sequence.h"
#include "TextureProperties/TextureProperties.h"

#include "SDL_timer.h"

Animation::Animation(const std::string id, const std::unordered_map<std::string, Sequence> &states)
    : id    (id)
    , states(states) {}

void Animation::SetState(const std::string state)
{
    this->state = state;
}

void Animation::Draw(std::shared_ptr<Resources> resources, const float x, const float y, const float tileWidth, const float tileHeight, const uint8_t opacity)
{
    if (opacity == 255)
        resources->GetTextureManager()->Draw(resources->GetEngine(), TextureProperties(id,
                                                                                       x,
                                                                                       y,
                                                                                       tileWidth,
                                                                                       tileHeight,
                                                                                       states[state].row,
                                                                                       frame,
                                                                                       states[state].flip,
                                                                                       1.0f,
                                                                                       states[state].opacity,
                                                                                       states[state].scaleX,
                                                                                       states[state].scaleY));
    else
        resources->GetTextureManager()->Draw(resources->GetEngine(), TextureProperties(id,
                                                                                       x,
                                                                                       y,
                                                                                       tileWidth,
                                                                                       tileHeight,
                                                                                       states[state].row,
                                                                                       frame,
                                                                                       states[state].flip,
                                                                                       1.0f,
                                                                                       opacity,
                                                                                       states[state].scaleX,
                                                                                       states[state].scaleY));
}

void Animation::Update()
{
    if (states[state].animationSpeed == 0)
    {
        frame = 0;
        return;
    }

    // Determines the current frame of the animation, modulo loops it once it reaches the last frame
    frame = (int)((SDL_GetTicks64() / (unsigned long)states[state].animationSpeed) % (unsigned long)states[state].frames);
}