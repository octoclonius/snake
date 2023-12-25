#include "snake.hpp"
#include "constants.hpp"
#include <SDL.h>
#include <list>
#include <cmath>
#include <cstdlib>

Snake::Snake(SDL_Renderer* _renderer) : renderer(_renderer), speed(START_SPEED) {
    reset();
}

void Snake::move(Uint64 frameTimeMS) {
    if (MAX_FRAME_RATE <= 0) {
        if (frameTimeMS == 0) {
            return;
        } else {
            speed = TILES_PER_SEC * TILE_SIZE / (1000.0 / frameTimeMS);
        }
    }

    offset += speed;
    if (offset >= TILE_SIZE) {
        for (auto joint = joints.begin(); joint != joints.end(); ++joint) {
            switch (joint->dir) {
                case Direction::UP:
                    joint->tile.y -= static_cast<int>(offset) / TILE_SIZE;
                    break;
                case Direction::DOWN:
                    joint->tile.y += static_cast<int>(offset) / TILE_SIZE;
                    break;
                case Direction::LEFT:
                    joint->tile.x -= static_cast<int>(offset) / TILE_SIZE;
                    break;
                case Direction::RIGHT:
                    joint->tile.x += static_cast<int>(offset) / TILE_SIZE;
                    break;
                default:
                    break;
            }
            if (std::next(joint) != joints.end()) {
                joint->dir = std::next(joint)->dir;
            } else {
                if (turnBuffer != Direction::NONE) {
                    if (turnBuffer == Direction::UP || turnBuffer == Direction::DOWN) {
                        if (joint->dir == Direction::LEFT || joint->dir == Direction::RIGHT) {
                            joint->dir = turnBuffer;
                        }
                    } else {
                        if (joint->dir == Direction::UP || joint->dir == Direction::DOWN) {
                            joint->dir = turnBuffer;
                        }
                    }
                    turnBuffer = Direction::NONE;
                }
            }
        }
        offset = fmod(offset, TILE_SIZE);
    }
}

void Snake::set_dir(Direction dir) {
    turnBuffer = dir;
}

void Snake::draw() {
    if (SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_SetRenderDrawColor() failed: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    for (auto joint = joints.begin(); joint != joints.end(); ++joint) {
        SDL_Rect jointRect = {
            GRID_OFFSET.x + TILE_SIZE * joint->tile.x,
            GRID_OFFSET.y + TILE_SIZE * joint->tile.y,
            TILE_SIZE,
            TILE_SIZE
        };
        auto nextJoint = std::next(joint);
        if (nextJoint != joints.end() && joint->dir != nextJoint->dir) {
            SDL_Rect cornerRect = jointRect;
            switch (joint->dir) {
                case Direction::UP:
                    jointRect.h -= static_cast<int>(offset);
                    cornerRect.y -= TILE_SIZE;
                    if (nextJoint->dir == Direction::LEFT) {
                        cornerRect.x += TILE_SIZE - static_cast<int>(offset);
                    }
                    cornerRect.w -= TILE_SIZE - static_cast<int>(offset);
                    break;
                case Direction::DOWN:
                    jointRect.y += static_cast<int>(offset);
                    jointRect.h -= static_cast<int>(offset);
                    cornerRect.y += TILE_SIZE;
                    if (nextJoint->dir == Direction::LEFT) {
                        cornerRect.x += TILE_SIZE - static_cast<int>(offset);
                    }
                    cornerRect.w -= TILE_SIZE - static_cast<int>(offset);
                    break;
                case Direction::LEFT:
                    jointRect.w -= static_cast<int>(offset);
                    cornerRect.x -= TILE_SIZE;
                    if (nextJoint->dir == Direction::UP) {
                        cornerRect.y += TILE_SIZE - static_cast<int>(offset);
                    }
                    cornerRect.h -= TILE_SIZE - static_cast<int>(offset);
                    break;
                case Direction::RIGHT:
                    jointRect.x += static_cast<int>(offset);
                    jointRect.w -= static_cast<int>(offset);
                    cornerRect.x += TILE_SIZE;
                    if (nextJoint->dir == Direction::UP) {
                        cornerRect.y += TILE_SIZE - static_cast<int>(offset);
                    }
                    cornerRect.h -= TILE_SIZE - static_cast<int>(offset);
                    break;
                default:
                    break;
            }
            SDL_RenderFillRect(renderer, &cornerRect);
        } else {
            switch (joint->dir) {
                case Direction::UP:
                    jointRect.y -= static_cast<int>(offset);
                    break;
                case Direction::DOWN:
                    jointRect.y += static_cast<int>(offset);
                    break;
                case Direction::LEFT:
                    jointRect.x -= static_cast<int>(offset);
                    break;
                case Direction::RIGHT:
                    jointRect.x += static_cast<int>(offset);
                    break;
                default:
                    break;
            }
        }
        SDL_RenderFillRect(renderer, &jointRect);
    }
}

bool Snake::check_collision() {
    SDL_Rect headRect = {
        GRID_OFFSET.x + TILE_SIZE * joints.back().tile.x,
        GRID_OFFSET.y + TILE_SIZE * joints.back().tile.y,
        TILE_SIZE,
        TILE_SIZE
    };

    /* Check walls and include offset */
    switch (joints.back().dir) {
        case Direction::UP:
            if (joints.back().tile.y <= 0) {
                return true;
            }
            headRect.y -= static_cast<int>(offset);
            break;
        case Direction::DOWN:
            if (joints.back().tile.y >= GRID_ROWS - 1) {
                return true;
            }
            headRect.y += static_cast<int>(offset);
            break;
        case Direction::LEFT:
            if (joints.back().tile.x <= 0) {
                return true;
            }
            headRect.x -= static_cast<int>(offset);
            break;
        case Direction::RIGHT:
            if (joints.back().tile.x >= GRID_COLS - 1) {
                return true;
            }
            headRect.x += static_cast<int>(offset);
            break;
        default:
            break;
    }

    /* Check joints */
    for (auto joint = joints.begin(); std::next(joint) != joints.end(); ++joint) {
        SDL_Rect jointRect = {
            GRID_OFFSET.x + TILE_SIZE * joint->tile.x,
            GRID_OFFSET.y + TILE_SIZE * joint->tile.y,
            TILE_SIZE,
            TILE_SIZE
        };
        switch (joint->dir) {
            case Direction::UP:
                if (joint->dir != std::next(joint)->dir) {
                    jointRect.h -= static_cast<int>(offset);
                } else {
                    jointRect.y -= static_cast<int>(offset);
                }
                break;
            case Direction::DOWN:
                if (joint->dir != std::next(joint)->dir) {
                    jointRect.h -= static_cast<int>(offset);
                }
                jointRect.y += static_cast<int>(offset);
                break;
            case Direction::LEFT:
                if (joint->dir != std::next(joint)->dir) {
                    jointRect.w -= static_cast<int>(offset);
                } else {
                    jointRect.x += static_cast<int>(offset);
                }
                break;
            case Direction::RIGHT:
                if (joint->dir != std::next(joint)->dir) {
                    jointRect.w -= static_cast<int>(offset);
                }
                jointRect.x += static_cast<int>(offset);
                break;
            default:
                break;
        }

        if (SDL_HasIntersection(&headRect, &jointRect) == SDL_TRUE) {
            return true;
        }
    }

    return false;
}

void Snake::reset() {
    offset = START_OFFSET;
    joints.clear();
    for (int i = 0; i < START_LEN; ++i) {
        switch (START_DIR) {
            case Direction::UP:
                joints.emplace_front(Joint{{START_TILE.x, START_TILE.y + i}, START_DIR});
                break;
            case Direction::DOWN:
                joints.emplace_front(Joint{{START_TILE.x, START_TILE.y - i}, START_DIR});
                break;
            case Direction::LEFT:
                joints.emplace_front(Joint{{START_TILE.x + i, START_TILE.y}, START_DIR});
                break;
            case Direction::RIGHT:
                joints.emplace_front(Joint{{START_TILE.x - i, START_TILE.y}, START_DIR});
                break;
            default:
                break;
        }
    }
    turnBuffer = Direction::NONE;
}
