#include "Map.h"

Map::Map(const float                                  rows,
         const float                                  columns,
         const float                                  tileWidth,
         const float                                  tileHeight,
         const uint8_t                                depth,
         const float                                  cameraCapX,
         const float                                  cameraCapY,
         const std::vector<std::shared_ptr<Layer>>   &layers,
         const std::shared_ptr<Layer>                 parallax,
         const std::vector<std::shared_ptr<Tileset>> &tilesets,
         const std::vector<std::shared_ptr<Object>>  &objects)
    : rows      (rows)
    , columns   (columns)
    , tileWidth (tileWidth)
    , tileHeight(tileHeight)
    , depth     (depth)
    , cameraCap ({cameraCapX, cameraCapY})
    , layers    (layers)
    , parallax  (parallax)
    , tilesets  (tilesets)
    , objects   (objects) {}

void Map::Render(std::shared_ptr<Resources> resources, const Vector2D &adjustedRows, const Vector2D &adjustedColumns, const bool under)
{
    uint8_t i             = 0,
            adjustedDepth = depth;

    if (!under)
    {
        i             = depth;
        adjustedDepth = (uint8_t)layers.size();
    }

    while (i < adjustedDepth)
    {
        layers[i]->Render(resources, adjustedRows, adjustedColumns, tilesets);
        ++i;
    }

    if (!under && parallax != nullptr) parallax->Render(resources, Vector2D({0, columns}), Vector2D({0, rows}), tilesets);
}