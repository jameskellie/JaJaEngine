#ifndef ENGINE_H
#define ENGINE_H

#include "SDL/SDL_Deleter.h"

#include <memory>

struct SDL_Properties;

class Engine
{
    bool running;

    // Timing
    float deltaTime,
          currentTime,
          previousTime;

    std::unique_ptr<SDL_Window  , SDL_Deleter> window;
    std::unique_ptr<SDL_Renderer, SDL_Deleter> renderer;

public:
    ~Engine();

    bool Initialise(const SDL_Properties &properties);
    void UpdateDeltaTime();
    void Quit();

    // Must return a raw pointer for SDL Library functions
    inline SDL_Renderer *GetRenderer() { return renderer.get(); }
    inline bool  IsRunning()           { return running; }
    inline float GetDeltaTime()        { return deltaTime; }
};

#endif // ENGINE_H
