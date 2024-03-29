#ifndef SDL_DELETER_H
#define SDL_DELETER_H

#include "SDL_render.h"
#include "SDL_ttf.h"

struct SDL_Deleter
{
    void operator() (SDL_Window   *p) const { SDL_DestroyWindow  (p); }
    void operator() (SDL_Renderer *p) const { SDL_DestroyRenderer(p); }
    void operator() (SDL_Surface  *p) const { SDL_FreeSurface    (p); }
    void operator() (SDL_Texture  *p) const { SDL_DestroyTexture (p); }
    void operator() (TTF_Font     *p) const { TTF_CloseFont      (p); }
};

#endif // SDL_DELETER_H