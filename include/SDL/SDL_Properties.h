#ifndef SDL_PROPERTIES_H
#define SDL_PROPERTIES_H

#include <cstdint>

struct SDL_Properties
{
    const char    *WINDOW_TITLE;
    const int      WINDOW_WIDTH;   // TODO: set to user's screen values
    const int      WINDOW_HEIGHT;
    const int      TARGET_WIDTH;   // TODO: set to user's aspect ratio
    const int      TARGET_HEIGHT;
    const int      RD_INDEX;
    const uint32_t WINDOW_FLAGS;
    const uint32_t RENDERER_FLAGS; // TODO: vsync toggle

    SDL_Properties(const char    *WINDOW_TITLE,
                   const int      WINDOW_WIDTH,
                   const int      WINDOW_HEIGHT,
                   const int      TARGET_WIDTH,
                   const int      TARGET_HEIGHT,
                   const int      RD_INDEX,
                   const uint32_t WINDOW_FLAGS,
                   const uint32_t RENDERER_FLAGS)
        : WINDOW_TITLE(WINDOW_TITLE)
        , WINDOW_WIDTH(WINDOW_WIDTH)
        , WINDOW_HEIGHT(WINDOW_HEIGHT)
        , TARGET_WIDTH(TARGET_WIDTH)
        , TARGET_HEIGHT(TARGET_HEIGHT)
        , RD_INDEX(RD_INDEX)
        , WINDOW_FLAGS(WINDOW_FLAGS)
        , RENDERER_FLAGS(RENDERER_FLAGS) {}
};

#endif // SDL_PROPERTIES_H