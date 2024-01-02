#include "game.hpp"
#include <SDL2/SDL.h>
#include <vector>

Game::Game(SDL_Window* _window, SDL_Renderer* _renderer) : window(_window), renderer(_renderer), state(State::newGame), keyboard(), numRows(11), numCols(15), tileSize(150), grid(window, renderer, numRows, numCols, tileSize), snake(window, renderer, numRows, numCols, tileSize, grid.get_grid_offset()) {}

bool Game::gameOn() const {
    return state != State::quitGame;
}

void Game::poll() {
    for (SDL_Event _event; SDL_PollEvent(&_event) != 0;) {
        if (_event.type == SDL_QUIT || (_event.type == SDL_KEYDOWN && _event.key.keysym.sym == SDLK_ESCAPE)) {
            state = State::quitGame;
            return;
        }
        
        if (_event.type == SDL_KEYDOWN || _event.type == SDL_KEYUP) {
            if (_event.key.keysym.sym == SDLK_w || _event.key.keysym.sym == SDLK_a || _event.key.keysym.sym == SDLK_s || _event.key.keysym.sym == SDLK_d) {
                keyboard.set_key(&_event);
                snake.set_dir(keyboard, _event.key.keysym.sym);
            }
        }
    }
}

void Game::step() {
    std::vector<SDL_Keycode> _movementKeys{SDLK_w, SDLK_a, SDLK_s, SDLK_d};
    SDL_Point _prevPos, _currPos;
    switch (state) {
        case State::newGame:
            for (const auto _key : _movementKeys) {
                if (keyboard.get_key(_key)) {
                    state = State::playGame;
                    break;
                }
            }
            break;
            
        case State::playGame:
            _prevPos = snake.get_pos();
            snake.move();
            if (snake.check_collision()) {
                state = State::gameOver;
                break;
            }
            _currPos = snake.get_pos();
            if (_currPos.x != _prevPos.x || _currPos.y != _prevPos.y) {
                int _status = grid.update(_prevPos, _currPos, snake.get_len());
                if (_status == 1) {
                    snake.inc_len();
                } else if (_status != 0) {
                    state = State::gameOver;
                }
            }
            break;
            
        case State::gameOver:
            SDL_Delay(2000);
            for (SDL_Event _event; SDL_PollEvent(&_event) != 0;);
            keyboard.reset();
            snake.reset();
            grid.reset();
            state = State::newGame;
            break;
            
        case State::quitGame:
            break;
            
        default:
            break;
    }
}

void Game::draw() {
    grid.draw_grid();
    grid.draw_fruit();
    snake.draw();
    grid.draw_walls();
}
