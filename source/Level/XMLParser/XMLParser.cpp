#include "XMLParser.h"

#include "Layer.h"
#include "Map.h"
#include "Object.h"
#include "Resources.h"
#include "Tileset.h"

#include "tinyxml2.h"

#include <iostream>
#include <sstream>

std::unique_ptr<Map> XMLParser::ParseMap(std::shared_ptr<Resources> resources, const std::string source)
{
    tinyxml2::XMLDocument xml;
    xml.LoadFile(source.c_str());

    if (xml.Error())
    {
        std::cerr << "Failed to load: " << source << std::endl;
        return nullptr;
    }

    tinyxml2::XMLElement *root = xml.RootElement();

    unsigned int rows     = root->IntAttribute("height"),
                 columns  = root->IntAttribute("width"),
                 depth    = 0;

    float cameraCapX = (float)columns,
          cameraCapY = (float)rows,
          tileWidth  = root->FloatAttribute("tilewidth"),
          tileHeight = root->FloatAttribute("tileheight");

    std::vector<std::shared_ptr<Tileset>> tilesets;
    std::vector<std::shared_ptr<Layer>>   layers;
    std::shared_ptr<Layer>                parallax;
    std::vector<std::shared_ptr<Object>>  objects;
    
    for (auto e = root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
    {
        // Parse tilesets and properties
        if (e->Value() == std::string("tileset"))
        {
            auto tileset = ParseTileset(e);
            resources->GetTextureManager()->Load(resources->GetEngine(), tileset->id, "assets/tilesets/" + tileset->id + ".png");
            tilesets.push_back(std::move(tileset));
        }
        else if (e->Value() == std::string("properties"))
        {
            for (auto o = e->FirstChildElement("property"); o != nullptr ; o = o->NextSiblingElement("property"))
            {
                std::string propertyName = o->Attribute("name");

                if      (propertyName == "depth")      depth      = o->IntAttribute  ("value");
                else if (propertyName == "cameraCapX") cameraCapX = o->FloatAttribute("value");
                else if (propertyName == "cameraCapY") cameraCapY = o->FloatAttribute("value");
            }
        }
        // Parse layers and object layers
        else if (e->Value() == std::string("layer"))
        {
            std::unique_ptr<Layer> layer(ParseLayer(e, rows, columns));

            if (layer->scrollSpeed != 1.0f)
            {
                parallax = std::move(layer);
                continue;
            }

            layers.push_back(std::move(layer));
        }
        else if (e->Value() == std::string("objectgroup"))
        {
            objects = ParseObjects(e);
        }
    }

    return std::make_unique<Map>(rows, columns, tileWidth, tileHeight, depth, cameraCapX, cameraCapY, layers, parallax, tilesets, objects);
}

std::unique_ptr<Tileset> XMLParser::ParseTileset(tinyxml2::XMLElement *xmlTileset)
{
    auto tileset = std::make_unique<Tileset>();

    tileset->id         = xmlTileset->Attribute   ("name");
    tileset->firstID    = xmlTileset->IntAttribute("firstgid");
    tileset->tileCount  = xmlTileset->IntAttribute("tilecount");
    tileset->lastID     = tileset   ->firstID + tileset->tileCount - 1;
    tileset->columns    = xmlTileset->IntAttribute("columns");
    tileset->rows       = tileset   ->tileCount / tileset->columns;
    tileset->tileWidth  = xmlTileset->FloatAttribute("tilewidth");
    tileset->tileHeight = xmlTileset->FloatAttribute("tileheight");
    tileset->spacing    = xmlTileset->FloatAttribute("spacing");

    tinyxml2::XMLElement* image = xmlTileset->FirstChildElement();
    tileset->source             = image     ->Attribute("source");

    return tileset;
}

std::unique_ptr<Layer> XMLParser::ParseLayer(tinyxml2::XMLElement *xmlLayer, const unsigned int rows, const unsigned int columns)
{
    tinyxml2::XMLElement *data = nullptr;

    float scrollSpeed = 1.0f;

    uint8_t opacity  = (uint8_t)(255 * xmlLayer->FloatAttribute("opacity"));
            opacity  = (opacity == 0) ? 255 : opacity;

    for (auto e = xmlLayer->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
    {
        if (e->Value() == std::string("properties"))
        {
            auto o      = e->FirstChildElement();
            scrollSpeed = o->FloatAttribute("value");
        }
        if (e->Value() == std::string("data"))
        {
            data = e;
            break;
        }
    }

    std::string matrix = (data->GetText());
    std::istringstream iss(matrix);
    std::string id;
    std::vector<std::vector<unsigned int>> tilemap(rows, std::vector<unsigned int>(columns, 0));

    for (unsigned int row = 0; row < rows; ++row)
    {
        for (unsigned int column = 0; column < columns; ++column)
        {
            getline(iss, id, ',');
            std::stringstream converter(id);
            converter >> tilemap[row][column];

            if (!iss.good())
                break;
        }
    }

    return std::make_unique<Layer>(tilemap, opacity, scrollSpeed);
}

std::vector<std::shared_ptr<Object>> XMLParser::ParseObjects(tinyxml2::XMLElement* xmlLayer)
{
    std::vector<std::shared_ptr<Object>> objects;

    for (auto e = xmlLayer->FirstChildElement(); e != nullptr; e = e->NextSiblingElement("object"))
    {
        auto object = std::make_unique<Object>();

        object->lastHitbox.x = object->hitbox.x = e->FloatAttribute("x");
        object->lastHitbox.y = object->hitbox.y = e->FloatAttribute("y");
        object->lastHitbox.w = object->hitbox.w = e->FloatAttribute("width");
        object->lastHitbox.h = object->hitbox.h = e->FloatAttribute("height");

        auto o = e->FirstChildElement("properties");

        if (o != nullptr)
        {
            for (o = o->FirstChildElement("property"); o != nullptr ; o = o->NextSiblingElement("property"))
            {
                std::string propertyName = o->Attribute("name");

                if (propertyName == "load")    object->loadZone    = o->Attribute("value");
                if (propertyName == "setMapX") object->setMapPos.x = o->FloatAttribute("value");
                if (propertyName == "setMapY") object->setMapPos.y = o->FloatAttribute("value");
            }
        }

        object->SetOrigin(true);
        objects.push_back(std::move(object));
    }

    return objects;
}