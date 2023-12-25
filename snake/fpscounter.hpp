#ifndef fpscounter_hpp
#define fpscounter_hpp

#include "constants.hpp"
#include <SDL.h>
#include <SDL_ttf.h>

class FPSCounter {
public:
    FPSCounter(SDL_Renderer* _renderer);

    void draw_fps();

private:
    SDL_Renderer* renderer;
    const SDL_Color textColor;
    TTF_Font* font;
    SDL_Texture* texture;
    const int windowWidth;
    SDL_Rect rect;
    const int updateIntervalMS;
    int frameCount;
    Uint64 prevMS;
};

#endif /* fpscounter_hpp */
