#ifndef ENGINE_H
#define ENGINE_H

#include "SDL/SDL_Deleter.h"

#include <memory>

struct SDL_Properties;

class Engine
{
public:
    enum class State
    {
        PLAY,
        PAUSE
    };

private:
    State state;
    bool running,
         debugMode;

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
    void InvertState();
    void Quit();

    // Getters
    // Must return a raw pointer for SDL Library functions
    inline SDL_Renderer *GetRenderer() { return renderer.get(); }
    inline bool  IsRunning()           { return running; }
    inline bool  GetDebugMode()        { return debugMode; }
    inline float GetDeltaTime()        { return deltaTime; }
    inline State GetState()            { return state; }

    // Setters
    inline void SetState(const State state) { this->state = state; }
    inline void ToggleDebugMode()           { debugMode = !debugMode; }
};

#endif // ENGINE_H
