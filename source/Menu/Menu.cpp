#include "Menu.h"

#include "Resources.h"

#include "SDL/SDL_Properties.h"

Menu::Menu()
{
    pauseText.reset(TTF_OpenFont("assets/fonts/pixel.ttf", 50));

    if (pauseText == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create Font: %s", TTF_GetError());
        return;
    }

    buttonText.reset(TTF_OpenFont("assets/fonts/pixel.ttf", 20));

    if (buttonText == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create Font: %s", TTF_GetError());
        return;
    }
}

Menu::~Menu()
{
    pauseText.reset(nullptr);
    buttonText.reset(nullptr);
}

void Menu::Pause(std::shared_ptr<Resources> resources)
{
    SDL_Color white = {255, 255, 255};

    // Mouse position
    int x,
        y;

    SDL_GetMouseState(&x, &y);

    // Variable to be reused for each item of text
    SDL_Surface *surface;
    SDL_Texture *texture;
    int textW;
    int textH;
    SDL_Rect dstRect;

    // "PAUSED" Text
    surface = TTF_RenderText_Solid(GetPauseTextFont(), "PAUSED", white);
    texture = SDL_CreateTextureFromSurface(resources->GetEngine()->GetRenderer(), surface);
    textW   = 0;
    textH   = 0;
    SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
    dstRect = {(resources->GetEngine()->GetProperties()->TARGET_WIDTH - surface->w) / 2, (resources->GetEngine()->GetProperties()->TARGET_HEIGHT - surface->h) / 3, textW, textH};
    SDL_RenderCopy(resources->GetEngine()->GetRenderer(), texture, NULL, &dstRect);

    // "DEBUG MODE" Button
    surface = TTF_RenderText_Solid(GetButtonTextFont(), "Debug Mode", white);
    texture = SDL_CreateTextureFromSurface(resources->GetEngine()->GetRenderer(), surface);
    textW   = 0;
    textH   = 0;
    SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
    dstRect = {static_cast<int>((resources->GetEngine()->GetProperties()->TARGET_WIDTH - surface->w) * 0.25), static_cast<int>((resources->GetEngine()->GetProperties()->TARGET_HEIGHT - surface->h) * 0.6), textW, textH};

    if ((x < (dstRect.x + dstRect.w) * resources->GetEngine()->GetProperties()->RESOLUTION_SCALE_WIDTH && x > (dstRect.x) * resources->GetEngine()->GetProperties()->RESOLUTION_SCALE_WIDTH) && (y < (dstRect.y + dstRect.h) * resources->GetEngine()->GetProperties()->RESOLUTION_SCALE_HEIGHT && y > (dstRect.y) * resources->GetEngine()->GetProperties()->RESOLUTION_SCALE_HEIGHT))
    {
        (resources->GetEngine()->GetDebugMode()) ? SDL_SetTextureColorMod(texture, 0, 0, 255) : SDL_SetTextureColorMod(texture, 255, 0, 0);

        if (resources->GetInputHandler()->LeftClick())
        {
            resources->GetEngine()->ToggleDebugMode();
        }
    }

    SDL_RenderCopy(resources->GetEngine()->GetRenderer(), texture, NULL, &dstRect);

    // "Save & Quit" Button
    surface = TTF_RenderText_Solid(GetButtonTextFont(), "Save & Quit", white);
    texture = SDL_CreateTextureFromSurface(resources->GetEngine()->GetRenderer(), surface);
    textW   = 0;
    textH   = 0;
    SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
    dstRect = {static_cast<int>((resources->GetEngine()->GetProperties()->TARGET_WIDTH - surface->w) * 0.75), static_cast<int>((resources->GetEngine()->GetProperties()->TARGET_HEIGHT - surface->h) * 0.6), textW, textH};

    if ((x < (dstRect.x + dstRect.w) * resources->GetEngine()->GetProperties()->RESOLUTION_SCALE_WIDTH && x > (dstRect.x) * resources->GetEngine()->GetProperties()->RESOLUTION_SCALE_WIDTH) && (y < (dstRect.y + dstRect.h) * resources->GetEngine()->GetProperties()->RESOLUTION_SCALE_HEIGHT && y > (dstRect.y) * resources->GetEngine()->GetProperties()->RESOLUTION_SCALE_HEIGHT))
    {
        SDL_SetTextureColorMod(texture, 255, 0, 0);

        if (resources->GetInputHandler()->LeftClick())
        {
            resources->GetEngine()->SetState(Engine::State::MAINMENU);
        }
    }

    SDL_RenderCopy(resources->GetEngine()->GetRenderer(), texture, NULL, &dstRect);
}

void Menu::GameOver(std::shared_ptr<Resources> resources)
{
    SDL_Color white = {255, 255, 255};
    SDL_Color red   = {255, 0  , 0};

    // Mouse position
    int x,
        y;

    SDL_GetMouseState(&x, &y);

    // Variable to be reused for each item of text
    SDL_Surface *surface;
    SDL_Texture *texture;
    int textW;
    int textH;
    SDL_Rect dstRect;

    // "YOU DIED" Text
    surface = TTF_RenderText_Solid(GetPauseTextFont(), "YOU DIED", red);
    texture = SDL_CreateTextureFromSurface(resources->GetEngine()->GetRenderer(), surface);
    textW   = 0;
    textH   = 0;
    SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
    dstRect = {(resources->GetEngine()->GetProperties()->TARGET_WIDTH - surface->w) / 2, (resources->GetEngine()->GetProperties()->TARGET_HEIGHT - surface->h) / 3, textW, textH};
    SDL_RenderCopy(resources->GetEngine()->GetRenderer(), texture, NULL, &dstRect);

    // "Quit" Button
    surface = TTF_RenderText_Solid(GetButtonTextFont(), "Quit", white);
    texture = SDL_CreateTextureFromSurface(resources->GetEngine()->GetRenderer(), surface);
    textW   = 0;
    textH   = 0;
    SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
    dstRect = {static_cast<int>((resources->GetEngine()->GetProperties()->TARGET_WIDTH - surface->w) * 0.5), static_cast<int>((resources->GetEngine()->GetProperties()->TARGET_HEIGHT - surface->h) * 0.6), textW, textH};

    if ((x < (dstRect.x + dstRect.w) * resources->GetEngine()->GetProperties()->RESOLUTION_SCALE_WIDTH && x > (dstRect.x) * resources->GetEngine()->GetProperties()->RESOLUTION_SCALE_WIDTH) && (y < (dstRect.y + dstRect.h) * resources->GetEngine()->GetProperties()->RESOLUTION_SCALE_HEIGHT && y > (dstRect.y) * resources->GetEngine()->GetProperties()->RESOLUTION_SCALE_HEIGHT))
    {
        SDL_SetTextureColorMod(texture, 255, 0, 0);

        if (resources->GetInputHandler()->LeftClick())
        {
            resources->GetEngine()->SetState(Engine::State::MAINMENU);
        }
    }

    SDL_RenderCopy(resources->GetEngine()->GetRenderer(), texture, NULL, &dstRect);
}

void Menu::MainMenu(std::shared_ptr<Resources> resources)
{
    SDL_Color white = {255, 255, 255};

    // Mouse position
    int x,
        y;

    SDL_GetMouseState(&x, &y);

    // Variable to be reused for each item of text
    SDL_Surface *surface;
    SDL_Texture *texture;
    int textW;
    int textH;
    SDL_Rect dstRect;

    // "JaJaEngine" Text
    surface = TTF_RenderText_Solid(GetPauseTextFont(), "JaJaEngine", white);
    texture = SDL_CreateTextureFromSurface(resources->GetEngine()->GetRenderer(), surface);
    textW   = 0;
    textH   = 0;
    SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
    dstRect = {(resources->GetEngine()->GetProperties()->TARGET_WIDTH - surface->w) / 2, (resources->GetEngine()->GetProperties()->TARGET_HEIGHT - surface->h) / 3, textW, textH};
    SDL_RenderCopy(resources->GetEngine()->GetRenderer(), texture, NULL, &dstRect);

    // "New" Button
    surface = TTF_RenderText_Solid(GetButtonTextFont(), "New", white);
    texture = SDL_CreateTextureFromSurface(resources->GetEngine()->GetRenderer(), surface);
    textW   = 0;
    textH   = 0;
    SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
    dstRect = {static_cast<int>((resources->GetEngine()->GetProperties()->TARGET_WIDTH - surface->w) * 0.5), static_cast<int>((resources->GetEngine()->GetProperties()->TARGET_HEIGHT - surface->h) * 0.6), textW, textH};

    if ((x < (dstRect.x + dstRect.w) * resources->GetEngine()->GetProperties()->RESOLUTION_SCALE_WIDTH && x > (dstRect.x) * resources->GetEngine()->GetProperties()->RESOLUTION_SCALE_WIDTH) && (y < (dstRect.y + dstRect.h) * resources->GetEngine()->GetProperties()->RESOLUTION_SCALE_HEIGHT && y > (dstRect.y) * resources->GetEngine()->GetProperties()->RESOLUTION_SCALE_HEIGHT))
    {
        SDL_SetTextureColorMod(texture, 255, 0, 0);

        if (resources->GetInputHandler()->LeftClick())
        {
            resources->GetEngine()->SetState(Engine::State::PLAY);
        }
    }

    SDL_RenderCopy(resources->GetEngine()->GetRenderer(), texture, NULL, &dstRect);

    // "Load" Button
    surface = TTF_RenderText_Solid(GetButtonTextFont(), "Load", white);
    texture = SDL_CreateTextureFromSurface(resources->GetEngine()->GetRenderer(), surface);
    textW   = 0;
    textH   = 0;
    SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
    dstRect = {static_cast<int>((resources->GetEngine()->GetProperties()->TARGET_WIDTH - surface->w) * 0.5), static_cast<int>((resources->GetEngine()->GetProperties()->TARGET_HEIGHT - surface->h) * 0.7), textW, textH};

    if ((x < (dstRect.x + dstRect.w) * resources->GetEngine()->GetProperties()->RESOLUTION_SCALE_WIDTH && x > (dstRect.x) * resources->GetEngine()->GetProperties()->RESOLUTION_SCALE_WIDTH) && (y < (dstRect.y + dstRect.h) * resources->GetEngine()->GetProperties()->RESOLUTION_SCALE_HEIGHT && y > (dstRect.y) * resources->GetEngine()->GetProperties()->RESOLUTION_SCALE_HEIGHT))
    {
        SDL_SetTextureColorMod(texture, 255, 0, 0);

        if (resources->GetInputHandler()->LeftClick())
        {
            // resources->GetEngine()->Load();
        }
    }

    SDL_RenderCopy(resources->GetEngine()->GetRenderer(), texture, NULL, &dstRect);

    // "Quit" Button
    surface = TTF_RenderText_Solid(GetButtonTextFont(), "Quit", white);
    texture = SDL_CreateTextureFromSurface(resources->GetEngine()->GetRenderer(), surface);
    textW   = 0;
    textH   = 0;
    SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
    dstRect = {static_cast<int>((resources->GetEngine()->GetProperties()->TARGET_WIDTH - surface->w) * 0.5), static_cast<int>((resources->GetEngine()->GetProperties()->TARGET_HEIGHT - surface->h) * 0.8), textW, textH};

    if ((x < (dstRect.x + dstRect.w) * resources->GetEngine()->GetProperties()->RESOLUTION_SCALE_WIDTH && x > (dstRect.x) * resources->GetEngine()->GetProperties()->RESOLUTION_SCALE_WIDTH) && (y < (dstRect.y + dstRect.h) * resources->GetEngine()->GetProperties()->RESOLUTION_SCALE_HEIGHT && y > (dstRect.y) * resources->GetEngine()->GetProperties()->RESOLUTION_SCALE_HEIGHT))
    {
        SDL_SetTextureColorMod(texture, 255, 0, 0);

        if (resources->GetInputHandler()->LeftClick())
        {
            resources->GetEngine()->Quit();
        }
    }

    SDL_RenderCopy(resources->GetEngine()->GetRenderer(), texture, NULL, &dstRect);
}