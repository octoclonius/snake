#ifndef snake_hpp
#define snake_hpp

#include "keyboard.hpp"
#include "clock.hpp"
#include <SDL.h>
#include <list>
#include <forward_list>
#include <queue>

enum class Direction;
struct Joint;

class Snake {
public:
    Snake(SDL_Window* _window, SDL_Renderer* _renderer, int _numRows, int _numCols, int _tileSize, SDL_Point _gridOffset);
    
    bool check_collision();
    void set_dir(const Keyboard _keyboard, SDL_Keycode _key);
    void move();
    void draw();
    void reset();

private:
    enum class Direction {
        none,
        up,
        down,
        left,
        right,
    };
    
    struct Joint {
        SDL_Point tile;
        Direction dir;

        Joint(SDL_Point _tile, Direction _dir) : tile(_tile), dir(_dir) {}
    };
    
    std::list<Joint> init_joints(SDL_Point _tile, int _len, Direction _dir);
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    const int numRows;
    const int numCols;
    int tileSize;
    SDL_Point gridOffset;
    std::list<Joint> joints;
    double offset;
    double speed;
    std::forward_list<SDL_Keycode> keyBuffer;
    std::queue<Direction> turnBuffer;
    std::chrono::time_point<Scene_Clock> prevTime;
    bool prevTimeValid;
};

#endif /* snake_hpp */
