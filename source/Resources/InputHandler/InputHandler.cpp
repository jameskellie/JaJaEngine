#include "InputHandler.h"

#include "Engine.h"

InputHandler::InputHandler()
{
    keyStates = SDL_GetKeyboardState(nullptr);

    leftMouseButton  = false;
    rightMouseButton = false;
}

void InputHandler::KeyUpdate()
{
    keyStates = SDL_GetKeyboardState(nullptr);
}

void InputHandler::MousePress(SDL_MouseButtonEvent &mouse)
{
    if      (mouse.button == SDL_BUTTON_LEFT)  leftMouseButton  = true;
    else if (mouse.button == SDL_BUTTON_RIGHT) rightMouseButton = true;
}

void InputHandler::Listen(std::shared_ptr<Engine> engine)
{
    leftMouseButton  = false;
    rightMouseButton = false;

    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
            engine->Quit();
            break;

            case SDL_KEYUP:
            KeyUpdate();
            break;
            
            case SDL_KEYDOWN:
            KeyUpdate();
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                engine->InvertState();
            break;

            case SDL_MOUSEBUTTONDOWN:
            MousePress(event.button);
            break;
        }
    }
}

bool InputHandler::IsKeyDown(const SDL_Scancode key)
{
    return (keyStates[key] == 1);
}

float InputHandler::GetMovementDirection(const MovementDirection movementDirection)
{
    switch (movementDirection)
    {
        case MovementDirection::HORIZONTAL:
        if (IsKeyDown(SDL_SCANCODE_A) && !IsKeyDown(SDL_SCANCODE_D)) return -1.0f;
        if (IsKeyDown(SDL_SCANCODE_D) && !IsKeyDown(SDL_SCANCODE_A)) return  1.0f;
        break;

        case MovementDirection::VERTICAL:
        if (IsKeyDown(SDL_SCANCODE_W) && !IsKeyDown(SDL_SCANCODE_S)) return -1.0f;
        if (IsKeyDown(SDL_SCANCODE_S) && !IsKeyDown(SDL_SCANCODE_W)) return  1.0f;
        break;
    }

    return 0;
}