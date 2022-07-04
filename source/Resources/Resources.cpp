#include "Resources.h"

#include <iostream>

Resources::Resources(const SDL_Properties &properties)
{
    engine = std::make_shared<Engine>();

    if (!engine->Initialise(properties))
        std::cerr << "Failed to initialise engine" << std::endl;

    inputHandler   = std::make_shared<InputHandler>();
    textureManager = std::make_shared<TextureManager>();
}