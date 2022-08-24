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

    bool leftMouseButton,
         rightMouseButton;

    void KeyUpdate();

public:
    InputHandler();

    void  Listen(std::shared_ptr<Engine> engine);
    float GetMovementDirection(const MovementDirection movementDirection);
    void  MousePress(SDL_MouseButtonEvent &mouse);

    // Getters
    bool IsKeyDown(const SDL_Scancode key);
    bool LeftClick()  { return leftMouseButton; }
    bool RightClick() { return rightMouseButton; }
};

#endif // INPUTHANDLER_H