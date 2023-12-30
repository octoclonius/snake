#ifndef game_hpp
#define game_hpp

#include "keyboard.hpp"
#include "grid.hpp"
#include "snake.hpp"
#include <SDL.h>

enum class State;

class Game {
public:
    Game(SDL_Window* _window, SDL_Renderer* _renderer);
    
    bool gameOn() const;
    void poll();
    void step();
    void draw();
    
private:
    enum class State {
        quitGame,
        newGame,
        playGame,
        gameOver,
    };
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    State state;
    Keyboard keyboard;
    const int numRows;
    const int numCols;
    int tileSize;
    Grid grid;
    Snake snake;
};


#endif /* game_hpp */
