#include "Level.h"

#include "Quadtree.h"
#include "XMLParser.h"

#include "tinyxml2.h"

#include <iostream>

Level::Level(std::shared_ptr<Subject> subject)
    : subject(subject)
{
    this->subject->Attach(this);
}

void Level::Update(std::shared_ptr<void> collision)
{
    this->collision = std::static_pointer_cast<Object>(collision);

    CollisionReaction();
}

void Level::CollisionReaction()
{
    if (collision->loadZone != "") SetMap(collision->loadZone);
}

bool Level::Load(std::shared_ptr<Resources> resources, const std::string id, const std::string source)
{
    // Don't load the same map twice
    if (maps.find(id) != maps.end())
    {
        std::cerr << "Failed to load map because map already loaded: " << id << std::endl;
        return false;
    } 

    maps[id] = XMLParser::ParseMap(resources, source);

    return (maps[id] == nullptr) ? false : true;
}

bool Level::ParseMaps(std::shared_ptr<Resources> resources, const std::string source)
{
    tinyxml2::XMLDocument xml;
    xml.LoadFile(source.c_str());

    if (xml.Error()) return false;

    tinyxml2::XMLElement *root = xml.RootElement();

    for (auto e = root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
    {
        if (e->Value() == std::string("map"))
        {
            std::string id     = e->Attribute("id");
            std::string source = e->Attribute("source");
            if (!Load(resources, id, source)) return false;
        }
    }

    return true;
}

void Level::FillQuadtree(std::shared_ptr<Quadtree> quadtree)
{ 
    for (auto i : maps[currentMap]->objects)
    {
        quadtree->Insert(i);
    }
}

void Level::Render(std::shared_ptr<Resources> resources, std::shared_ptr<Camera> camera, const bool under)
{
    Vector2D tileDimensions  = maps[currentMap]->GetTileDimensions();
    Vector2D adjustedRows    = {camera->viewBox->x / tileDimensions.x, (camera->viewBox->x + camera->viewBox->w) / tileDimensions.x};
    Vector2D adjustedColumns = {camera->viewBox->y / tileDimensions.y, (camera->viewBox->y + camera->viewBox->h) / tileDimensions.y};

    maps[currentMap]->Render(resources, adjustedRows, adjustedColumns, under);
}