#include "Camera.h"
#include "Entity.h"
#include "Factory.h"
#include "Level.h"
#include "Menu.h"
#include "Quadtree.h"
#include "Resources.h"

#include "SDL/SDL_Properties.h"

#include <algorithm>
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
    // Load menu + fonts
    auto menu = std::make_unique<Menu>();
    // Game Level
    auto level = std::make_shared<Level>();
    // Camera
    auto camera = std::make_shared<Camera>(SDLProperties);
    resources->GetTextureManager()->SetCamera(camera);
    // Player
    std::shared_ptr<Entity> player;
    // Entities
    std::vector<std::shared_ptr<Entity>> entities;

    // Registers all entities into the factory map - SHOULD ONLY BE CALLED ONCE
    EntityCreator::GetInstance()->RegisterEntities();

    // TODO: Put this in a class instead of having it all in main
    while (resources->GetEngine()->IsRunning())
    {
        // Clear window
        SDL_SetRenderDrawColor(resources->GetEngine()->GetRenderer(), 0, 0, 0, 255);
        SDL_RenderClear(resources->GetEngine()->GetRenderer());

        // Inputs
        resources->GetInputHandler()->Listen(resources->GetEngine());
        
        // Timing
        resources->GetEngine()->UpdateDeltaTime();

        // Reset quadtree
        if (resources->GetEngine()->GetState() == Engine::State::PLAY)
        {
            quadtree->Clear();
            quadtree->SetBounds(camera);
        }

        if (resources->GetEngine()->GetState() != Engine::State::MAINMENU)
        {
            // Game loop
            if (resources->GetEngine()->GetState() != Engine::State::PAUSE)
            {
                if (level->GetMapChanged())
                {
                    entities.clear();
                    entities = EntityCreator::GetInstance()->ParseEntities("assets/levels/level01/entities.xml", level->GetMapName(), quadtree);
                    
                    // TODO: Put in/read from file
                    if (entities.size() == 1) entities[0]->SetPosition(Vector2D(232.0f, 232.0f));
                    if (level->GetMapName() == "forest") entities[0]->SetPosition(Vector2D(232.0f, 232.0f));
                    else entities[0]->SetPosition(Vector2D(550.0f, 111.0f));
                    entities.push_back(player);
                    level->SetMapChanged();
                }

                // Quadtree fill
                level->FillQuadtree(quadtree);
                for (auto i : entities)
                {
                    quadtree->Insert(i);
                }

                // Update
                for (auto i : entities)
                {
                    i->Update(resources);
                }

                // Check collisions
                quadtree->CheckCollisions(level);

                // Move camera
                camera->Update(level->GetCurrentMap()->GetMapDimensions(), level->GetCurrentMap()->GetTileDimensions());

                // Prepare entities for rendering
                std::sort(entities.begin(), entities.end(), Entity::compareY);
            }

            // Render map layers under player
            level->Render(resources, camera);

            // Render
            for (auto i : entities)
            {
                i->Render(resources);
            }

            // Render map layers over player
            level->Render(resources, camera, false);

            // Render debug overlay
            if (resources->GetEngine()->GetDebugMode())
            {
                quadtree->DrawTree(resources, camera);
            }

            // HUD - TODO: Make a class
            menu->HUD(resources, player);

            // In-game menu
            if (resources->GetEngine()->GetState() == Engine::State::PAUSE)
            {
                menu->Pause(resources);
            }
            else if (resources->GetEngine()->GetState() == Engine::State::GAMEOVER)
            {
                menu->GameOver(resources);
            }
        }
        else
        {
            // Main menu
            menu->MainMenu(resources);

            if (resources->GetEngine()->GetState() == Engine::State::PLAY)
            {
                resources->GetTextureManager()->Clean();
                level->Clean();

                // Loads the following maps into memory
                if (!level->ParseMaps(resources, "assets/levels/level01/maps.xml"))
                    std::cerr << "Failed to load maps.xml" << std::endl;

                // Freely swap between loaded maps by setting the current map
                level->SetMap("forest"); // TODO: Current map and above map file should be read from save file
                level->SetMapChanged();

                // Loads the following textures into memory
                // TODO: Load entity textures from entity XML, not level XML
                if (!resources->GetTextureManager()->ParseTextures(resources->GetEngine(), "assets/levels/level01/textures.xml"))
                    std::cerr << "Failed to load textures.xml" << std::endl;

                // Storage of all entities currently created
                player   = EntityCreator::GetInstance()->ParseEntity("assets/entities/player/player.xml", quadtree);
                entities = EntityCreator::GetInstance()->ParseEntities("assets/levels/level01/entities.xml", level->GetMapName(), quadtree);

                // TODO: Put in/read from savefile
                player->SetPosition(Vector2D(250.0f, 250.0f));
                entities[0]->SetPosition(Vector2D(232.0f, 232.0f));
                entities.push_back(player);

                // Camera & related setup
                camera->SetTarget(player->GetOrigin());
            }
        }

        // Output rendered image to screen
        SDL_RenderPresent(resources->GetEngine()->GetRenderer());

    }

    resources->GetTextureManager()->Clean();
    level->Clean();
    
    return 0;
}
