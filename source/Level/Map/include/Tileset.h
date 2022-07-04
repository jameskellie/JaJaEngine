#ifndef TILESET_H
#define TILESET_H

#include <string>

struct Tileset
{
    unsigned int firstID,
                 lastID,
                 rows,
                 columns,
                 tileCount;

    float tileWidth,
          tileHeight,
          spacing;

    std::string id,
                source;
};

#endif // TILESET_H