#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <memory>
#include <string>
#include <vector>

class Layer;
class Map;
class Object;
class Resources;

struct Tileset;

namespace tinyxml2
{
    class XMLElement;
}

class XMLParser
{
    static std::unique_ptr<Tileset>             ParseTileset(tinyxml2::XMLElement* xmlTileset);
    static std::unique_ptr<Layer>               ParseLayer  (tinyxml2::XMLElement* xmlLayer, const unsigned int rows, const unsigned int columns);
    static std::vector<std::shared_ptr<Object>> ParseObjects(tinyxml2::XMLElement* xmlLayer);

public:
    static std::unique_ptr<Map> ParseMap(std::shared_ptr<Resources> resources, const std::string source);
};

#endif // XMLPARSER_H