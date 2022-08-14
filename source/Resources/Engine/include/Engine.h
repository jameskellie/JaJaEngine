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
    bool running;

    // Timing
    float deltaTime,
          currentTime,
          previousTime;

    std::unique_ptr<SDL_Window  , SDL_Deleter> window;
    std::unique_ptr<SDL_Renderer, SDL_Deleter> renderer;
    std::unique_ptr<TTF_Font    , SDL_Deleter> font;

public:
    ~Engine();

    bool Initialise(const SDL_Properties &properties);
    void UpdateDeltaTime();
    void InvertState();
    void Quit();

    // Getters
    // Must return a raw pointer for SDL Library functions
    inline SDL_Renderer *GetRenderer() { return renderer.get(); }
    inline TTF_Font     *GetFont()     { return font.get(); }
    inline bool  IsRunning()           { return running; }
    inline float GetDeltaTime()        { return deltaTime; }
    inline State GetState()            { return state; }

    // Setters
    inline void SetState(const State state) { this->state = state; }
};

#endif // ENGINE_H
