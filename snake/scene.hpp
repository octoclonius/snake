#ifndef scene_hpp
#define scene_hpp

#include "fpscounter.hpp"
#include "grid.hpp"
#include "snake.hpp"
#include <SDL.h>

class Scene {
public:
    Scene(SDL_Renderer* renderer_sdl);

    GameState get_game_state() const;
    void play_frame();
    void draw_frame();
    void wait_frame();

    Grid grid;
    Snake snake;

private:
    void clear_frame();

    GameState gameState;
    const int maxFrameRate;
    Uint64 prevMS;
    SDL_Renderer* renderer;
    FPSCounter fpsCounter;
};

#endif /* scene_hpp */
