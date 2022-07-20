#ifndef MAP_H
#define MAP_H

#include "Layer.h"
#include "Object.h"
#include "Tileset.h"

#include "Vector2D/Vector2D.h"

#include <memory>
#include <vector>

class Map
{
    float rows,    // Y / HEIGHT
          columns, // X / WIDTH
          tileWidth,
          tileHeight;
        
    uint8_t depth;

    Vector2D cameraCap; // Maximum x and y that the camera can show

    std::vector<std::shared_ptr<Layer>>   layers;
    std::shared_ptr<Layer>                parallax;
    std::vector<std::shared_ptr<Tileset>> tilesets;

public:
    std::vector<std::shared_ptr<Object>>  objects;

    Map(const float                                  rows,
        const float                                  columns,
        const float                                  tileWidth,
        const float                                  tileHeight,
        const uint8_t                                depth,
        const float                                  cameraCapX,
        const float                                  cameraCapY,
        const std::vector<std::shared_ptr<Layer>>   &layers,
        const std::shared_ptr<Layer>                 parallax,
        const std::vector<std::shared_ptr<Tileset>> &tilesets,
        const std::vector<std::shared_ptr<Object>>  &objects);

    void Render(std::shared_ptr<Resources> resources, const Vector2D &adjustedRows, const Vector2D &adjustedColumns, const bool under = true);

    // Getters
    inline Vector2D &GetMapDimensions()  { return cameraCap; }
    inline Vector2D  GetTileDimensions() { return Vector2D({tileWidth, tileHeight}); }
};

#endif // MAP_H
