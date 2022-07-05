#include "Layer.h"

#include "Resources.h"
#include "Tileset.h"

#include "TextureProperties/TextureProperties.h"
#include "Vector2D/Vector2D.h"

#include <climits>

Layer::Layer(const std::vector<std::vector<unsigned int>> tilemap, const uint8_t opacity, const float scrollSpeed)
    : tilemap(tilemap)
    , opacity(opacity)
    , scrollSpeed(scrollSpeed) {}

void Layer::Render(std::shared_ptr<Resources> resources, const Vector2D &adjustedRows, const Vector2D &adjustedColumns, const std::vector<std::shared_ptr<Tileset>> &tilesets)
{
    for (auto i = (long unsigned int)adjustedColumns.x; (float)i < adjustedColumns.y; ++i)
    {
        for (auto j = (long unsigned int)adjustedRows.x; (float)j < adjustedRows.y; ++j)
        {
            unsigned int tileID = tilemap[i][j];

            if (tileID == 0) continue;

            // Tiled maps use integer overflow for flipping tiles, so that is handled here using an unsigned int and logic
            SDL_RendererFlip flip     = SDL_FLIP_NONE;
            unsigned int flipOverflow = (INT_MAX - 1) / 2;

            if (tileID > flipOverflow)
            {
                --tileID -= flipOverflow;
                flip      = SDL_FLIP_VERTICAL;

                if (tileID > flipOverflow)
                {
                    --tileID -= flipOverflow;
                    flip      = SDL_FLIP_HORIZONTAL;
                }

                if (tileID > flipOverflow)
                {
                    --tileID -= flipOverflow;
                    flip      = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
                }
            }

            unsigned int index = 0;

            if (tilesets.size() > 1)
            {
                for (unsigned int k = 0; k < tilesets.size(); ++k)
                {
                    if (tileID >= tilesets[k]->firstID && tileID <= tilesets[k]->lastID)
                    {
                        tileID += tilesets[k]->tileCount - tilesets[k]->lastID;
                        index = k;
                        break;
                    }
                }
            }

            auto tileset    = tilesets[index];
            int  tileRow    = tileID / tileset->columns,
                 tileColumn = tileID % tileset->columns - 1;
        
            // Prevents the last column of a tilesheet from being clipped
            if (tileID % tileset->columns == 0)
            {
                --tileRow;
                tileColumn = tileset->columns - 1;
            }

            resources->GetTextureManager()->Draw(resources->GetEngine(), TextureProperties(tileset->id, (float)j * tileset->tileWidth, (float)i * tileset->tileHeight, tileset->tileWidth + tileset->spacing, tileset->tileWidth + tileset->spacing, tileRow, tileColumn, flip, scrollSpeed, opacity));
        }
    }
}