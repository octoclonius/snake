#ifndef constants_hpp
#define constants_hpp

#include <SDL.h>
#include <SDL_ttf.h>
#include <list>

enum class Direction {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

struct Joint {
    SDL_Point tile;
    Direction dir;

    Joint(SDL_Point tile, Direction dir) : tile(tile), dir(dir) {}
};

enum class GameState {
    QUIT_GAME,
    NEW_GAME,
    PLAY_GAME,
    GAME_OVER
};

extern const char*  WINDOW_NAME;
constexpr SDL_Point WINDOW_SIZE = {640, 480};

constexpr int GRID_ROWS = 11;
constexpr int GRID_COLS = 15;
constexpr int WALL_SIZE = 35;
constexpr int TILE_SIZE = 35;

constexpr SDL_Point GRID_SIZE = {
    GRID_COLS * TILE_SIZE,
    GRID_ROWS * TILE_SIZE
};
constexpr SDL_Point SCENE_SIZE = {
    GRID_SIZE.x + WALL_SIZE * 2,
    GRID_SIZE.y + WALL_SIZE * 2
};
constexpr SDL_Point SCENE_OFFSET = {
    (WINDOW_SIZE.x - SCENE_SIZE.x) / 2,
    (WINDOW_SIZE.y - SCENE_SIZE.y) / 2
};
constexpr SDL_Point GRID_OFFSET = {
    SCENE_OFFSET.x + WALL_SIZE,
    SCENE_OFFSET.y + WALL_SIZE
};

constexpr int MAX_FRAME_RATE = 0;
constexpr int FRAME_DELAY_MS = MAX_FRAME_RATE > 0 ? 1000 / MAX_FRAME_RATE : 0;

constexpr SDL_Point START_TILE      = {2, 5};
constexpr double    START_OFFSET    = TILE_SIZE - 1.0;
constexpr int       START_LEN       = 3;
constexpr double    TILES_PER_SEC   = 5.0;
constexpr double    START_SPEED     = MAX_FRAME_RATE > 0 ? TILES_PER_SEC * TILE_SIZE / MAX_FRAME_RATE : 0.0;
constexpr Direction START_DIR       = Direction::RIGHT;

extern const char*  FONT_PATH;
constexpr int       FONT_PT     = 12;
constexpr SDL_Color TEXT_COLOR  = {255, 255, 255, SDL_ALPHA_OPAQUE};
constexpr int       UPDATE_MS   = 100;

#endif /* constants_hpp */
