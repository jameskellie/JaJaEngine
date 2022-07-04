#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "Camera.h"

#include "SDL/SDL_Deleter.h"

#include <memory>
#include <string>
#include <unordered_map>

class Engine;

struct TextureProperties;

class TextureManager
{
    std::unordered_map<std::string, std::unique_ptr<SDL_Texture, SDL_Deleter>> textures;
    std::shared_ptr<Camera>                                                    camera;

public:
    // Loads a texture into memory
    bool Load(std::shared_ptr<Engine> engine, const std::string id, const std::string source);
    bool ParseTextures(std::shared_ptr<Engine> engine, const std::string source);

    // Draw an image
    void Draw(std::shared_ptr<Engine> engine, const TextureProperties &properties);

    // Remove an image from memory
    void Drop(const std::string id);
    void Clean();

    // Setters
    inline void SetCamera(std::shared_ptr<Camera> camera) { this->camera = camera; }
};

#endif // TEXTUREMANAGER_H
