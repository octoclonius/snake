#include "constants.hpp"
#include "scene.hpp"
#include <SDL.h>
#include <SDL_ttf.h>
#include <cstdlib>

Scene::Scene(SDL_Renderer* renderer_sdl) : grid(renderer_sdl), snake(renderer_sdl), gameState(GameState::NEW_GAME), maxFrameRate(MAX_FRAME_RATE), renderer(renderer_sdl), fpsCounter(renderer_sdl) {}

GameState Scene::get_game_state() const {
    return gameState;
}

void Scene::play_frame() {
    //
}

void Scene::draw_frame() {
    clear_frame();
    grid.draw_grid();
    snake.draw();
    grid.draw_walls();
    fpsCounter.draw_fps();
    
    SDL_RenderPresent(renderer);
}

void Scene::wait_frame() {
    //
}

void Scene::clear_frame() {
    if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_SetRenderDrawColor() failed: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    if (SDL_RenderClear(renderer) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_RenderClear() failed: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}
