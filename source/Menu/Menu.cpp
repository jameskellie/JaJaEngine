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

void Menu::Pause(std::shared_ptr<Resources> resources, const SDL_Properties &properties)
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
    dstRect = {(properties.TARGET_WIDTH - surface->w) / 2, (properties.TARGET_HEIGHT - surface->h) / 3, textW, textH};
    SDL_RenderCopy(resources->GetEngine()->GetRenderer(), texture, NULL, &dstRect);

    // "DEBUG MODE" Button
    surface = TTF_RenderText_Solid(GetButtonTextFont(), "Debug Mode", white);
    texture = SDL_CreateTextureFromSurface(resources->GetEngine()->GetRenderer(), surface);
    textW   = 0;
    textH   = 0;
    SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
    dstRect = {static_cast<int>((properties.TARGET_WIDTH - surface->w) * 0.25), static_cast<int>((properties.TARGET_HEIGHT - surface->h) * 0.6), textW, textH};

    if ((x < (dstRect.x + dstRect.w) * properties.RESOLUTION_SCALE && x > (dstRect.x) * properties.RESOLUTION_SCALE) && (y < (dstRect.y + dstRect.h) * properties.RESOLUTION_SCALE && y > (dstRect.y) * properties.RESOLUTION_SCALE))
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
    dstRect = {static_cast<int>((properties.TARGET_WIDTH - surface->w) * 0.75), static_cast<int>((properties.TARGET_HEIGHT - surface->h) * 0.6), textW, textH};

    if ((x < (dstRect.x + dstRect.w) * properties.RESOLUTION_SCALE && x > (dstRect.x) * properties.RESOLUTION_SCALE) && (y < (dstRect.y + dstRect.h) * properties.RESOLUTION_SCALE && y > (dstRect.y) * properties.RESOLUTION_SCALE))
    {
        SDL_SetTextureColorMod(texture, 255, 0, 0);

        if (resources->GetInputHandler()->LeftClick())
        {
            resources->GetEngine()->Quit();
        }
    }

    SDL_RenderCopy(resources->GetEngine()->GetRenderer(), texture, NULL, &dstRect);
}