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
        PAUSE,
        GAMEOVER,
        MAINMENU
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
    std::shared_ptr<SDL_Properties>            properties;

public:
    ~Engine();

    bool Initialise(const SDL_Properties &properties);
    void UpdateDeltaTime();
    void InvertState();
    void Quit();

    // Getters
    inline std::shared_ptr<SDL_Properties> GetProperties() { return properties; }
    // Must return a raw pointer for SDL Library functions
    inline SDL_Renderer *GetRenderer() { return renderer.get(); }
    inline SDL_Window   *GetWindow()   { return window.get(); }
    inline bool  IsRunning()           { return running; }
    inline bool  GetDebugMode()        { return debugMode; }
    inline float GetDeltaTime()        { return deltaTime; }
    inline State GetState()            { return state; }

    // Setters
           void SetState(const State state);
           void SetScale();
    inline void ToggleDebugMode() { debugMode   = !debugMode; }
    inline void GameOver()        { state       = State::GAMEOVER; }
};

#endif // ENGINE_H
