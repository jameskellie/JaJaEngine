#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Engine.h"
#include "InputHandler.h"
#include "TextureManager.h"

struct SDL_Properties;

class Resources
{
    std::shared_ptr<Engine>         engine;
    std::shared_ptr<InputHandler>   inputHandler;
    std::shared_ptr<TextureManager> textureManager;

public:
    Resources(const SDL_Properties &properties);
    
    inline std::shared_ptr<Engine>         GetEngine()         { return engine; }
    inline std::shared_ptr<InputHandler>   GetInputHandler()   { return inputHandler; }
    inline std::shared_ptr<TextureManager> GetTextureManager() { return textureManager; }
};

#endif // RESOURCEMANAGER_H
