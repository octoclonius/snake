#ifndef snake_hpp
#define snake_hpp

#include "constants.hpp"
#include <SDL.h>
#include <list>

class Snake {
public:
    Snake(SDL_Renderer* renderer_sdl);

    bool check_collision();
    void move(Uint64 frameTimeMS);
    void set_dir(Direction dir);
    void draw();
    void reset();

private:
    SDL_Renderer* renderer;
    double offset;
    double speed;
    std::list<Joint> joints;
    Direction turnBuffer;
};

#endif /* snake_hpp */
