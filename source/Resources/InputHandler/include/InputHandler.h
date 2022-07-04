#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "SDL_events.h"

#include <memory>
#include <cstdint>

class Engine;

enum class MovementDirection
{
    HORIZONTAL,
    VERTICAL
};
    
class InputHandler
{
    const uint8_t* keyStates;

    void KeyUpdate();

public:
    InputHandler();

    void  Listen(std::shared_ptr<Engine> engine);
    bool  IsKeyDown(const SDL_Scancode key);
    float GetMovementDirection(const MovementDirection movementDirection);
};

#endif // INPUTHANDLER_H