#include "EntityCreator.h"

#include "TextureProperties/TextureProperties.h"

#include "tinyxml2.h"

#include <iostream>
#include <vector>

// Singleton related declarations
EntityCreator *EntityCreator::instance{nullptr};
std::mutex EntityCreator::mutex;

EntityCreator *EntityCreator::GetInstance()
{
    std::lock_guard<std::mutex> lock(mutex);

    if (instance == nullptr) instance = new EntityCreator();

    return instance;
}

void EntityCreator::RegisterEntityToMap(const std::string id, std::function<std::shared_ptr<Entity>(std::shared_ptr<Subject> subject, const std::unordered_map<std::string, Sequence> &states, const TextureProperties &properties)> entity)
{
    entities[id] = entity;
}

std::shared_ptr<Entity> EntityCreator::FactoryMethod(const std::string id, std::shared_ptr<Subject> subject, const std::unordered_map<std::string, Sequence> &states, const TextureProperties &properties)
{
    std::shared_ptr<Entity> entity;
    auto it = entities.find(id);

    if (it != entities.end()) entity = it->second(subject, states, properties);

    return entity;
}

std::shared_ptr<Entity> EntityCreator::ParseEntity(const char *source, std::shared_ptr<Subject> subject)
{
    std::unordered_map<std::string, Sequence> states;

    std::string entityType,
                textureType,
                textureID,
                textureSource;

    float width  = 0.0f,
          height = 0.0f;
                
    tinyxml2::XMLDocument xml;
    xml.LoadFile(source);

    if (xml.Error())
    {
        std::cerr << "Failed to load " << source << std::endl;
        return nullptr;
    }

    for (auto e = xml.RootElement(); e != nullptr; e = e->NextSiblingElement())
    {
        if (e->Value() == std::string("properties"))
        {
            for (auto o = e->FirstChildElement("property"); o != nullptr ; o = o->NextSiblingElement("property"))
            {
                std::string propertyName = o->Attribute("name");

                if      (propertyName == "entityType")  entityType  = o->Attribute("entityType");
                else if (propertyName == "textureType") textureType = o->Attribute("textureType");
            }        
        }
        else if (e->Value() == std::string("states"))
        {
            for (auto o = e->FirstChildElement("sequence"); o != nullptr ; o = o->NextSiblingElement("sequence"))
            {
                if (textureType == "spritesheet")
                {
                    std::string stateID = o->Attribute("id");

                    int row            = o->IntAttribute("row"),
                        frames         = o->IntAttribute("frames", 1),
                        animationSpeed = o->IntAttribute("animationSpeed", 0);

                    uint8_t opacity = (uint8_t)(255 * o->FloatAttribute("opacity", 0));
                            opacity = (opacity == 0) ? 255 : opacity;

                    float scaleX = o->FloatAttribute("scaleX", 1.0f),
                          scaleY = o->FloatAttribute("scaleY", 1.0f);

                    std::string flipString = o->Attribute("flip");
                    SDL_RendererFlip flip;

                    if      (flipString == "none")       flip = SDL_FLIP_NONE;
                    else if (flipString == "horizontal") flip = SDL_FLIP_HORIZONTAL;
                    else if (flipString == "vertical")   flip = SDL_FLIP_VERTICAL;
                    else                                 flip = (SDL_RendererFlip)(SDL_FLIP_VERTICAL | SDL_FLIP_HORIZONTAL);

                    states[stateID] = Sequence(row, frames, animationSpeed, flip, opacity, scaleX, scaleY);
                }
            }
        }
        else if (e->Value() == std::string("textures"))
        {
            for (auto o = e->FirstChildElement("texture"); o != nullptr ; o = o->NextSiblingElement("texture"))
            {
                textureID     = o->Attribute     ("id");
                textureSource = o->Attribute     ("source");
                width         = o->FloatAttribute("width"); 
                height        = o->FloatAttribute("height"); 
            }
        }
    }

    return FactoryMethod(entityType, subject, states, TextureProperties(textureID, 0.0f, 0.0f, width, height));
}

std::vector<std::shared_ptr<Entity>> EntityCreator::ParseEntities(const std::string source, const std::string map, std::shared_ptr<Subject> subject)
{
    std::vector<std::shared_ptr<Entity>> returnEntities;

    tinyxml2::XMLDocument xml;
    xml.LoadFile(source.c_str());

    if (xml.Error())
    {
        std::cerr << "Failed to load " << source << std::endl;
        return returnEntities;
    }

    auto e = xml.RootElement();

    for (auto o = e->FirstChildElement("entity"); o != nullptr ; o = o->NextSiblingElement("entity"))
    {
        if (o->Attribute("map") != map) continue;

        auto entity = ParseEntity(o->Attribute("source"), subject);

        if (entity == nullptr) continue;

        returnEntities.push_back(entity);
    }

    return returnEntities;
}