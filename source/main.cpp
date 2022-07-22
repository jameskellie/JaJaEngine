#include "Camera.h"
#include "Entity.h"
#include "Factory.h"
#include "Level.h"
#include "Quadtree.h"
#include "Resources.h"

#include "SDL/SDL_Properties.h"

#include <iostream>

int main(int argc, char *argv[])
{
    // Bypass -Werror warning
    (void)argc; (void)argv;
    
    SDL_Properties SDLProperties
    {
        "JaJaEngine",                                         // WINDOW_TITLE
        1920,                                                 // WINDOW_WIDTH
        1080,                                                 // WINDOW_HEIGHT
        480,                                                  // TARGET_WIDTH
        270,                                                  // TARGET_HEIGHT
        -1,                                                   // RD_INDEX
        SDL_WINDOW_RESIZABLE      | SDL_WINDOW_ALLOW_HIGHDPI, // WINDOW_FLAGS
        SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED  // RENDERER_FLAGS
    };

    // Resources = Engine, InputHandler, TextureManager
    auto resources = std::make_shared<Resources>(SDLProperties);
    auto quadtree  = std::make_shared<Quadtree>(Vector2D({0, 0}),
                                                Vector2D({(float)SDLProperties.TARGET_WIDTH,(float)SDLProperties.TARGET_HEIGHT}));

    auto level = std::make_shared<Level>();

    // Loads the following maps into memory
    if (!level->ParseMaps(resources, "assets/levels/level01/maps.xml"))
        std::cerr << "Failed to load maps.xml" << std::endl;

    // Freely swap between loaded maps by setting the current map
    level->SetMap("forest"); // TODO: Current map and above map file should be read from save file

    // Loads the following textures into memory
    if (!resources->GetTextureManager()->ParseTextures(resources->GetEngine(), "assets/levels/level01/textures.xml"))
        std::cerr << "Failed to load textures.xml" << std::endl;

    // Storage of all entities currently created - index [0] should always be the player
    auto entities = EntityCreator::ParseEntities("assets/levels/level01/entities.xml", quadtree);
    entities[0]->SetPosition(Vector2D(250.0f, 250.0f)); // TODO: Put in/read from savefile
    entities[1]->SetPosition(Vector2D(232.0f, 232.0f));

    // Camera & related setup
    auto camera = std::make_shared<Camera>(SDLProperties);
    camera->SetTarget(entities[0]->GetOrigin());
    resources->GetTextureManager()->SetCamera(camera);

    while (resources->GetEngine()->IsRunning())
    {
        // Quadtree fill
        level->FillQuadtree(quadtree);
        for (auto i : entities)
        {
            quadtree->Insert(i);
        }

        // Inputs
        resources->GetInputHandler()->Listen(resources->GetEngine());

        // Timing
        resources->GetEngine()->UpdateDeltaTime();

        // The duplicate for(update)/quadtree calls may seem strange
        // but it's to update and check collision of the X/Y planes independently
        // Update X
        for (auto i : entities)
        {
            i->Update(resources);
        }

        quadtree->CheckCollisions(level);

        // Update Y
        for (auto i : entities)
        {
            i->Update(resources);
        }

        quadtree->CheckCollisions(level);

        // Move camera
        camera->Update(level->GetCurrentMap()->GetMapDimensions(), level->GetCurrentMap()->GetTileDimensions());

        // Clear window
        SDL_SetRenderDrawColor(resources->GetEngine()->GetRenderer(), 0, 0, 0, 255);
        SDL_RenderClear(resources->GetEngine()->GetRenderer());

        // Render map layers under player
        level->Render(resources, camera);

        // Render
        std::sort(entities.begin(), entities.end(), Entity::compareY);
        for (auto i : entities)
        {
            i->Render(resources);
        }

        // Render map layers over player
        level->Render(resources, camera, false);

        // Quadtree reset
        // quadtree->DrawTree(resources, camera); // DEBUG: Uncomment to see the quadtree as an overlay
        quadtree->Clear();
        quadtree->SetBounds(camera);

        SDL_RenderPresent(resources->GetEngine()->GetRenderer());
    }

    resources->GetTextureManager()->Clean();
    level->Clean();
    
    return 0;
}
