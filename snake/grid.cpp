#include "grid.hpp"
#include <cstdlib>

Grid::Grid(SDL_Window* _window, SDL_Renderer* _renderer, int _numRows, int _numCols, int _tileSize) : window(_window), renderer(_renderer), numRows(_numRows), numCols(_numCols), tileSize(_tileSize), gridSize(calc_grid_size()), sceneSize(calc_scene_size()), sceneOffset(calc_scene_offset()), gridOffset(calc_grid_offset()), wallRects(calc_wall_rects()), grid(numRows, std::vector<int>(numCols)) {}

SDL_Point Grid::calc_grid_size() const {
    return {numCols * tileSize, numRows * tileSize};
}

SDL_Point Grid::calc_scene_size() const {
    return {gridSize.x + tileSize * 2, gridSize.y + tileSize * 2};
}

SDL_Point Grid::calc_scene_offset() const {
    SDL_Point _windowSize;
    if (SDL_GetRendererOutputSize(renderer, &_windowSize.x, &_windowSize.y) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_GetRendererOutputSize() failed: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    
    return {(_windowSize.x - sceneSize.x) / 2, (_windowSize.y - sceneSize.y) / 2};
}

SDL_Point Grid::calc_grid_offset() const {
    return {sceneOffset.x + tileSize, sceneOffset.y + tileSize};
}

std::vector<SDL_Rect> Grid::calc_wall_rects() const {
   return {
       {
           sceneOffset.x,
           sceneOffset.y,
           sceneSize.x,
           tileSize
       },
       {
           sceneOffset.x,
           gridOffset.y + gridSize.y,
           sceneSize.x,
           tileSize
       },
       {
           sceneOffset.x,
           gridOffset.y,
           tileSize,
           gridSize.y
       },
       {
           gridOffset.x + gridSize.x,
           gridOffset.y,
           tileSize,
           gridSize.y
       }
   };
}

SDL_Point Grid::get_grid_size() const {
    return gridSize;
}

SDL_Point Grid::get_scene_size() const {
    return sceneSize;
}

SDL_Point Grid::get_scene_offset() const {
    return sceneOffset;
}

SDL_Point Grid::get_grid_offset() const {
    return gridOffset;
}

void Grid::draw_grid() {
    if (SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_SetRenderDrawColor() failed: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i <= numRows; ++i) {
        SDL_RenderDrawLine(renderer,
            gridOffset.x,
            gridOffset.y + tileSize * i,
            gridOffset.x + gridSize.x,
            gridOffset.y + tileSize * i
        );
    }
    for (int i = 0; i <= numCols; ++i) {
        SDL_RenderDrawLine(renderer,
            gridOffset.x + tileSize * i,
            gridOffset.y,
            gridOffset.x + tileSize * i,
            gridOffset.y + gridSize.y
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
