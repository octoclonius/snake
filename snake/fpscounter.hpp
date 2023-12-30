#ifndef fpscounter_hpp
#define fpscounter_hpp

#include "clock.hpp"
#include <SDL.h>
#include <SDL_ttf.h>

struct TextureRect;

class FPSCounter {
public:
    FPSCounter(SDL_Window* _window, SDL_Renderer* _renderer);
    
    void draw();
    void shutdown();

private:
    struct TextureRect {
        SDL_Texture* texture;
        SDL_Rect rect;
        
        TextureRect(SDL_Texture* _texture, SDL_Rect _rect) : texture(_texture), rect(_rect) {}
    };
    
    TTF_Font* init_font(const char* _fontPath, int _fontPtSize);
    
    TextureRect set_textureRect(const char* _fpsText);
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Color textColor;
    TTF_Font* font;
    TextureRect textureRect;
    std::chrono::time_point<Scene_Clock> prevTime;
    bool prevTimeValid;
    double updateRate;
    int frameCount;
};

#endif /* fpscounter_hpp */
