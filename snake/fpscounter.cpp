#include "fpscounter.hpp"
#include <SDL.h>
#include <SDL_ttf.h>
#include <cmath>
#include <string>
#include <cstdlib>

const char* FONT_PATH = "/System/Library/Fonts/Monaco.ttf";

FPSCounter::FPSCounter(SDL_Renderer* _renderer) : renderer(_renderer), textColor(TEXT_COLOR), font(nullptr), texture(nullptr), windowWidth(WINDOW_SIZE.x), rect(SDL_Rect{0, 0, 0, 0}), updateIntervalMS(UPDATE_MS), frameCount(0), prevMS(0) {
    font = TTF_OpenFont(FONT_PATH, FONT_PT);
    if (font == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "TTF_OpenFont() failed: %s", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Surface* surface = TTF_RenderUTF8_Solid(font, "0", textColor);
    if (surface == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "TTF_RenderUTF8_Solid() failed: %s", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateTextureFromSurface() failed: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    rect = SDL_Rect{windowWidth - surface->w, 0, surface->w, surface->h};
    SDL_FreeSurface(surface);
}

void FPSCounter::draw_fps() {
    auto currMS = SDL_GetTicks64();
    ++frameCount;

    if (SDL_RenderCopy(renderer, texture, nullptr, &rect) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_RenderCopy() failed: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    auto currIntervalMS = currMS - prevMS;
    if (currIntervalMS < updateIntervalMS) {
        return;
    }

    long fps = std::lround(1000.0 * frameCount / currIntervalMS);
    std::string fpsString = std::to_string(fps);
    const char* fpsText = fpsString.c_str();

    SDL_Surface* surface = TTF_RenderUTF8_Solid(font, fpsText, textColor);
    if (surface == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "TTF_RenderUTF8_Solid() failed: %s", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_DestroyTexture(texture);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateTextureFromSurface() failed: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    rect = SDL_Rect{windowWidth - surface->w, 0, surface->w, surface->h};
    SDL_FreeSurface(surface);

    frameCount = 0;
    prevMS = currMS;
}
