#include "TextureManager.h"

#include "Engine.h"

#include "TextureProperties/TextureProperties.h"

#include "SDL_image.h"
#include "tinyxml2.h"

#include <iostream>

bool TextureManager::Load(std::shared_ptr<Engine> engine, const std::string id, const std::string source)
{
    // Don't load the same texture twice
    if (textures.find(id) != textures.end())
    {
        std::cerr << "Failed to load texture because texture already loaded: " << id << std::endl;
        return false;
    }

    std::unique_ptr<SDL_Surface, SDL_Deleter> surface(IMG_Load(source.c_str()));

    if (surface == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s, %s", source.c_str(), IMG_GetError());
        return false;
    }

    std::unique_ptr<SDL_Texture, SDL_Deleter> texture(SDL_CreateTextureFromSurface(engine->GetRenderer(), surface.get()));

    if (texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create texture from surface: %s", IMG_GetError());
        return false;
    }

    textures[id] = std::move(texture);

    return true;
}

bool TextureManager::ParseTextures(std::shared_ptr<Engine> engine, const std::string source)
{
    tinyxml2::XMLDocument xml;
    xml.LoadFile(source.c_str());

    if (xml.Error()) return false;

    tinyxml2::XMLElement *root = xml.RootElement();

    for (auto e = root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
    {
        if (e->Value() == std::string("texture"))
        {
            std::string id     = e->Attribute("id");
            std::string source = e->Attribute("source");
            
            if (!Load(engine, id, source)) return false;
        }
    }

    return true;
}

void TextureManager::Draw(std::shared_ptr<Engine> engine, const TextureProperties &properties)
{
    SDL_Rect srcRect  = {(int)properties.tileWidth * properties.column,
                         (int)properties.tileHeight * properties.row,
                         (int)properties.tileWidth, (int)properties.tileHeight};
    SDL_FRect dstRect = {properties.x - (camera->viewBox->x * properties.scrollSpeed),
                         properties.y - (camera->viewBox->y * properties.scrollSpeed),
                         properties.tileWidth * properties.scaleX,
                         properties.tileHeight * properties.scaleY};

    // SDL_SetTextureBlendMode(textures[properties.id].get(), SDL_BLENDMODE_BLEND); // TODO: Was told I would need this for opacity and render_accelerated, but it seems to be alright so far
    SDL_SetTextureAlphaMod(textures[properties.id].get(), properties.opacity);
    SDL_RenderCopyExF(engine->GetRenderer(),
                      textures[properties.id].get(),
                      &srcRect,
                      &dstRect,
                      properties.angle,
                      properties.center.get(),
                      properties.flip);
}

void TextureManager::Drop(const std::string id)
{
    textures.erase(id);
}

void TextureManager::Clean()
{
    textures.clear();
}

