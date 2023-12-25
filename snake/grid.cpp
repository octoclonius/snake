#include "grid.hpp"
#include <SDL.h>
#include <array>
#include <cstdlib>

Grid::Grid(SDL_Renderer* _renderer) : wallRects(init_wall_rects()), renderer(_renderer) {
    grid.fill(std::array<int, GRID_COLS>{});
    //grid.at(START_TILE.y).at(START_TILE.x) = START_LEN;
}

std::array<SDL_Rect, 4> constexpr Grid::init_wall_rects() {
   return {{
       {
           SCENE_OFFSET.x,
           SCENE_OFFSET.y,
           SCENE_SIZE.x,
           WALL_SIZE
       },
       {
           SCENE_OFFSET.x,
           GRID_OFFSET.y + GRID_SIZE.y,
           SCENE_SIZE.x,
           WALL_SIZE
       },
       {
           SCENE_OFFSET.x,
           GRID_OFFSET.y,
           WALL_SIZE,
           GRID_SIZE.y
       },
       {
           GRID_OFFSET.x + GRID_SIZE.x,
           GRID_OFFSET.y,
           WALL_SIZE,
           GRID_SIZE.y
       }
   }};
}

void Grid::draw_grid() {
    if (SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_SetRenderDrawColor() failed: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i <= grid.size(); ++i) {
        SDL_RenderDrawLine(renderer,
            GRID_OFFSET.x,
            GRID_OFFSET.y + TILE_SIZE * i,
            GRID_OFFSET.x + GRID_SIZE.x,
            GRID_OFFSET.y + TILE_SIZE * i
        );
    }
    for (int i = 0; i <= grid.front().size(); ++i) {
        SDL_RenderDrawLine(renderer,
            GRID_OFFSET.x + TILE_SIZE * i,
            GRID_OFFSET.y,
            GRID_OFFSET.x + TILE_SIZE * i,
            GRID_OFFSET.y + GRID_SIZE.y
        );
    }
}

void Grid::draw_walls() {
    if (SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_SetRenderDrawColor() failed: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    for (const auto& wallRect : wallRects) {
        SDL_RenderFillRect(renderer, &wallRect);
    }
}
