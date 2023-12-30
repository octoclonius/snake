#ifndef grid_hpp
#define grid_hpp

#include <SDL.h>
#include <vector>

class Grid {
public:
    Grid(SDL_Window* _window, SDL_Renderer* _renderer, int _numRows, int _numCols, int _tileSize);
    
    SDL_Point get_grid_size() const;
    SDL_Point get_scene_size() const;
    SDL_Point get_scene_offset() const;
    SDL_Point get_grid_offset() const;
    void draw_grid();
    void draw_walls();

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
};

#endif /* grid_hpp */
