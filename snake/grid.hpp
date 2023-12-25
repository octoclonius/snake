#ifndef grid_hpp
#define grid_hpp

#include "constants.hpp"
#include <SDL.h>
#include <array>

class Grid {
public:
    Grid(SDL_Renderer* _renderer);

    void draw_grid();
    void draw_walls();

private:
    static constexpr std::array<SDL_Rect, 4> init_wall_rects();
    
    SDL_Renderer* renderer;
    std::array<std::array<int, GRID_COLS>, GRID_ROWS> grid;
    const std::array<SDL_Rect, 4> wallRects;
};

#endif /* grid_hpp */
