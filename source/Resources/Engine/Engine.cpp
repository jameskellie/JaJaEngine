#include "Engine.h"

#include "SDL/SDL_Properties.h"

#include "SDL_image.h"
#include "SDL_ttf.h"

bool Engine::Initialise(const SDL_Properties &properties)
{
    this->properties = std::make_shared<SDL_Properties>(properties);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }

    int flags    = IMG_INIT_PNG;
    int img_init = IMG_Init(flags);
    if ((img_init & flags) != flags)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL_image: %s", IMG_GetError());
        return false;
    }

    if (TTF_Init() != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL_ttf: %s", TTF_GetError());
        return false;
    }

    window.reset(SDL_CreateWindow(properties.WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, properties.WINDOW_WIDTH, properties.WINDOW_HEIGHT, properties.WINDOW_FLAGS));
    
    if (window == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create Window: %s", SDL_GetError());
        return false;
    }

    SetScale();
    renderer.reset(SDL_CreateRenderer(window.get(), properties.RD_INDEX, properties.RENDERER_FLAGS));

    if (renderer == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create Renderer: %s", SDL_GetError());
        return false;
    }

    // Sets the size of the canvas that will then be scaled to match the current screen resolution.
    SDL_RenderSetLogicalSize(renderer.get(), properties.TARGET_WIDTH, properties.TARGET_HEIGHT);
    SDL_SetRenderDrawBlendMode(renderer.get(), SDL_BLENDMODE_BLEND);

    // Timing
    currentTime  = (float)SDL_GetPerformanceCounter();
    previousTime = 0.0f;
    deltaTime    = 0.0f;

    state          = State::MAINMENU;
    debugMode      = false;
    return running = true;
}

void Engine::UpdateDeltaTime()
{
    previousTime = currentTime;
    currentTime  = (float)SDL_GetPerformanceCounter();
    deltaTime    = ((currentTime - previousTime) * 1000.0f / (float)SDL_GetPerformanceFrequency()) * 0.001f; // dT in seconds
}

void Engine::InvertState()
{
    if (state == State::PAUSE)     state = State::PLAY;
    else if (state == State::PLAY) state = State::PAUSE;
}

void Engine::SetState(const State state)
{
    if (this->state == State::MAINMENU && state == State::PAUSE) return;

    this->state = state;
}

// TODO: Multiple aspect ratio support
void Engine::SetScale()
{
    SDL_GetWindowSize(window.get(), &properties->WINDOW_WIDTH, &properties->WINDOW_HEIGHT);

    // Width
    float floatWindowWidth = properties->WINDOW_WIDTH,
          floatTargetWidth = properties->TARGET_WIDTH;

    properties->RESOLUTION_SCALE_WIDTH = floatWindowWidth / floatTargetWidth;

    // Height
    float floatWindowHeight = properties->WINDOW_HEIGHT,
          floatTargetHeight = properties->TARGET_HEIGHT;

    properties->RESOLUTION_SCALE_HEIGHT = floatWindowHeight / floatTargetHeight;

}

void Engine::Quit()
{
    running = false;
}

Engine::~Engine()
{
    window.reset(nullptr);
    renderer.reset(nullptr);

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}