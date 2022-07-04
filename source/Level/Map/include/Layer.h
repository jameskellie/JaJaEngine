#ifndef LAYER_H
#define LAYER_H

#include <memory>
#include <vector>

class Resources;

struct Tileset;
struct Vector2D;

class Layer
{
    std::vector<std::vector<unsigned int>> tilemap;
    uint8_t                                opacity;

public:
    float scrollSpeed;
    
    Layer(const std::vector<std::vector<unsigned int>> tilemap, const uint8_t opacity, const float scrollSpeed);

    void Render(std::shared_ptr<Resources> resources, const Vector2D &adjustedRows, const Vector2D &adjustedColumns, const std::vector<std::shared_ptr<Tileset>> &tilesets);
};

#endif // LAYER_H