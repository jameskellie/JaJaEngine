#ifndef MENU_H
#define MENU_H

#include "SDL/SDL_Deleter.h"

#include <memory>

class Resources;

struct SDL_Properties;

class Menu
{
    std::unique_ptr<TTF_Font, SDL_Deleter> pauseText;
    std::unique_ptr<TTF_Font, SDL_Deleter> buttonText;

public:
    Menu();
    ~Menu();

    void Pause(std::shared_ptr<Resources> resources, const SDL_Properties &properties);

    // Getters
    // Must return a raw pointer for SDL Library functions
    inline TTF_Font *GetPauseTextFont()  { return pauseText.get(); }
    inline TTF_Font *GetButtonTextFont() { return buttonText.get(); }
};

#endif // MENU_H