#include "grid.hpp"
#include <openssl/rand.h>
#include <openssl/err.h>
#include <cstring>
#include <climits>
#include <cstdlib>

Grid::Grid(SDL_Window* _window, SDL_Renderer* _renderer, int _numRows, int _numCols, int _tileSize) : window(_window), renderer(_renderer), numRows(_numRows), numCols(_numCols), tileSize(_tileSize), gridSize(calc_grid_size()), sceneSize(calc_scene_size()), sceneOffset(calc_scene_offset()), gridOffset(calc_grid_offset()), grid(numRows, std::vector<int>(numCols)), wallRects(calc_wall_rects()), fruitPos(init_fruit({11, 5})) {}

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

SDL_Point Grid::init_fruit(SDL_Point _fruitPos) {
    grid.at(_fruitPos.y).at(_fruitPos.x) = -1;
    return _fruitPos;
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

int Grid::update(SDL_Point _prevPos, SDL_Point _currPos, int _snakeLen) {
    /* Snake joint collision */
    if (grid.at(_currPos.y).at(_currPos.x) > 1) {
        return -1;
    }
    
    /* Empty tile */
    if (grid.at(_prevPos.y).at(_prevPos.x) != -1) {
        /* Decrement all positive tiles */
        for (auto& _row : grid) {
            for (auto& _col : _row) {
                if (_col > 0) {
                    --_col;
                }
            }
        }
        
        grid.at(_prevPos.y).at(_prevPos.x) = _snakeLen;
        return 0;
    }
    
    /* Fruit tile */
    grid.at(_prevPos.y).at(_prevPos.x) = _snakeLen + 1;
    
    /* New fruit tile */
    unsigned int _i;
    unsigned char _buf[sizeof(_i)];
    if (RAND_bytes(_buf, sizeof(_buf)) != 1) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "RAND_bytes() failed: %s", ERR_error_string(ERR_get_error(), nullptr));
        exit(EXIT_FAILURE);
    }
    
    std::memcpy(&_i, _buf, sizeof(_i));
    
    _i = static_cast<unsigned int>(static_cast<double>(_i) / UINT_MAX * (numRows * numCols - (_snakeLen + 1)));
    
    for (int y = 0; y < numRows; ++y) {
        for (int x = 0; x < numCols; ++x) {
            if (grid.at(y).at(x) == 0 && _i-- == 0) {
                fruitPos = SDL_Point{x, y};
                grid.at(fruitPos.y).at(fruitPos.x) = -1;
                return 1;
            }
        }
    }
    
    return -1;
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

void Grid::draw_fruit() {
    if (SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_SetRenderDrawColor() failed: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    
    SDL_Rect _fruitRect = {
        gridOffset.x + fruitPos.x * tileSize,
        gridOffset.y + fruitPos.y * tileSize,
        tileSize,
        tileSize
    };
    SDL_RenderFillRect(renderer, &_fruitRect);
}

void Grid::reset() {
    grid.assign(grid.size(), std::vector<int>(numCols));
    fruitPos = init_fruit({11, 5});
}
