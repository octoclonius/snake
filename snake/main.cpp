#include "main.hpp"
#include "constants.hpp"
#include "scene.hpp"
#include <SDL.h>
#include <SDL_ttf.h>
#include <cstdlib>

const char* WINDOW_NAME = "Snake Game";

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Init() failed: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    if (TTF_Init() == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "TTF_Init() failed: %s", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Window* window = SDL_CreateWindow(WINDOW_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_SIZE.x, WINDOW_SIZE.y, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateWindow() failed: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateRenderer() failed: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    Scene scene = Scene(renderer);
    
    scene.draw_frame();

    bool gameOn = true;
    bool gameStart = false;
    int frameTimeMS = 0;
    while (gameOn) {
        /* Start frame */
        auto startMS = SDL_GetTicks64();

        /* Poll input */
        for (SDL_Event event; !gameStart || SDL_PollEvent(&event) != 0;) {
            while (!gameStart) {
                for (; SDL_PollEvent(&event) != 0;) {
                    if (event.type == SDL_KEYDOWN || event.type == SDL_QUIT) {
                        gameStart = true;
                        break;
                    }
                }
                if (gameStart) {
                    break;
                }

                /* Delay next frame */
                if (FRAME_DELAY_MS > 0) {
                    frameTimeMS = static_cast<int>(SDL_GetTicks64() - startMS);
                    if (frameTimeMS < FRAME_DELAY_MS) {
                        SDL_Delay(FRAME_DELAY_MS - frameTimeMS);
                    }
                }
                startMS = SDL_GetTicks64();
            }

            switch (event.type) {
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_w:
                            if (event.key.state == SDL_PRESSED) {
                                scene.snake.set_dir(Direction::UP);
                            }
                            break;
                        case SDLK_a:
                            if (event.key.state == SDL_PRESSED) {
                                scene.snake.set_dir(Direction::LEFT);
                            }
                            break;
                        case SDLK_s:
                            if (event.key.state == SDL_PRESSED) {
                                scene.snake.set_dir(Direction::DOWN);
                            }
                            break;
                        case SDLK_d:
                            if (event.key.state == SDL_PRESSED) {
                                scene.snake.set_dir(Direction::RIGHT);
                            }
                            break;
                        case SDLK_q:
                            if (event.key.state == SDL_PRESSED) {
                                gameOn = false;
                            }
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_QUIT:
                    gameOn = false;
                    break;
                default:
                    break;
            }
            if (!gameOn) {
                break;
            }
        }
        if (!gameOn) {
            break;
        }

        /* Play frame */
        if (scene.snake.check_collision()) {
            auto startDelayMS = SDL_GetTicks64();
            gameStart = false;

            while (SDL_GetTicks64() - startDelayMS < 1000) {
                for (SDL_Event event; gameOn && SDL_PollEvent(&event) != 0;) {
                    switch (event.type) {
                        case SDL_KEYDOWN:
                            if (event.key.keysym.sym == SDLK_q && event.key.state == SDL_PRESSED) {
                                gameOn = false;
                            }
                            break;
                        case SDL_QUIT:
                            gameOn = false;
                            break;
                        default:
                            break;
                    }
                }
                if (!gameOn) {
                    break;
                }

                /* Delay next frame */
                if (FRAME_DELAY_MS > 0) {
                    frameTimeMS = static_cast<int>(SDL_GetTicks64() - startMS);
                    if (frameTimeMS < FRAME_DELAY_MS) {
                        SDL_Delay(FRAME_DELAY_MS - frameTimeMS);
                    }
                }
                startMS = SDL_GetTicks64();
            }
            if (!gameOn) {
                break;
            }

            frameTimeMS = 0;
            scene.snake.reset();
            scene.draw_frame();
            continue;
        }
        scene.snake.move(frameTimeMS);

        scene.draw_frame();

        /* Delay next frame */
        frameTimeMS = static_cast<int>(SDL_GetTicks64() - startMS);
        if (FRAME_DELAY_MS > 0 && frameTimeMS < FRAME_DELAY_MS) {
            SDL_Delay(FRAME_DELAY_MS - frameTimeMS);
        }
    }

    scene.~Scene();
    if (renderer != nullptr) {
        SDL_DestroyRenderer(renderer);
    }
    if (window != nullptr) {
        SDL_DestroyWindow(window);
    }
    TTF_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}
