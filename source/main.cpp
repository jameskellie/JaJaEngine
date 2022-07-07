#include "Camera.h"
#include "Factory.h"
#include "Level.h"
#include "Player.h"
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

    auto level = std::make_shared<Level>(quadtree);

    // Loads the following maps into memory
    if (!level->ParseMaps(resources, "assets/levels/level01/maps.xml"))
        std::cerr << "Failed to load maps.xml" << std::endl;

    // Freely swap between loaded maps by setting the current map
    level->SetMap("forest"); // TODO: Current map and above map file should be read from save file

    // Loads the following textures into memory
    if (!resources->GetTextureManager()->ParseTextures(resources->GetEngine(), "assets/levels/level01/textures.xml"))
        std::cerr << "Failed to load textures.xml" << std::endl;

    static Registrar<Player> registrar("PLAYER"); // TODO: Put this in a level-specific XML so all entities needed for a level are registered
    // Storage of all entities currently created
    auto entities = EntityCreator::ParseEntities("assets/entities/player/player.xml", quadtree);

    // Camera & related setup
    auto camera = std::make_shared<Camera>(SDLProperties);
    camera->SetTarget(entities[0]->GetOrigin());
    resources->GetTextureManager()->SetCamera(camera);

    while (resources->GetEngine()->IsRunning())
    {
        // Inputs
        resources->GetInputHandler()->Listen(resources->GetEngine());

        // Timing
        resources->GetEngine()->UpdateDeltaTime();

        // Update
        for (auto i : entities)
        {
            i->Update(resources, quadtree);
        }

        // Move camera
        camera->Update(level->GetCurrentMap()->GetMapDimensions(), level->GetCurrentMap()->GetTileDimensions());

        // Clear window
        SDL_SetRenderDrawColor(resources->GetEngine()->GetRenderer(), 0, 0, 0, 255);
        SDL_RenderClear(resources->GetEngine()->GetRenderer());

        // Render map layers under player
        level->Render(resources, camera);

        // Render
        for (auto i : entities)
        {
            i->Render(resources);
        }

        // Render map layers over player
        level->Render(resources, camera, false);

        // Quadtree
        quadtree->Clear();
        quadtree->SetBounds(camera);
        level->FillQuadtree(quadtree);
        // quadtree->DrawTree(resources, camera); // DEBUG: Uncomment to see the quadtree as an overlay

        SDL_RenderPresent(resources->GetEngine()->GetRenderer());
    }

    resources->GetTextureManager()->Clean();
    EntityCreator::Clean();
    level->Clean();
    
    return 0;
}
