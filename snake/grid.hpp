#ifndef grid_hpp
#define grid_hpp

#include <SDL2/SDL.h>
#include <vector>

class Grid {
public:
    Grid(SDL_Window* _window, SDL_Renderer* _renderer, int _numRows, int _numCols, int _tileSize);
    
    SDL_Point init_fruit(SDL_Point _fruitPos);
    SDL_Point get_grid_size() const;
    SDL_Point get_scene_size() const;
    SDL_Point get_scene_offset() const;
    SDL_Point get_grid_offset() const;
    int update(SDL_Point _prevPos, SDL_Point _currPos, int _snakeLen);
    void draw_grid();
    void draw_walls();
    void draw_fruit();
    void reset();

private:
    SDL_Point calc_grid_size() const;
    SDL_Point calc_scene_size() const;
    SDL_Point calc_scene_offset() const;
    SDL_Point calc_grid_offset() const;
    std::vector<SDL_Rect> calc_wall_rects() const;
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    const int numRows;
    const int numCols;
    int tileSize;
    SDL_Point gridSize;
    SDL_Point sceneSize;
    SDL_Point sceneOffset;
    SDL_Point gridOffset;
    std::vector<std::vector<int>> grid;
    std::vector<SDL_Rect> wallRects;
    SDL_Point fruitPos;
};

#endif /* grid_hpp */
