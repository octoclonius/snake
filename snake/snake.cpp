#include "snake.hpp"
#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <cstdlib>

Snake::Snake(SDL_Window* _window, SDL_Renderer* _renderer, int _numRows, int _numCols, int _tileSize, SDL_Point _gridOffset) : window(_window), renderer(_renderer), numRows(_numRows), numCols(_numCols), tileSize(_tileSize), gridOffset(_gridOffset), joints(init_joints({2, 5}, 3, Direction::right)), offset(tileSize - 0.1), speed(5.0 * tileSize), keyBuffer(), turnBuffer(), prevTime(), prevTimeValid(false) {}

std::list<Snake::Joint> Snake::init_joints(SDL_Point _tile, int _len, Direction _dir) {
    std::list<Joint> _joints;
    for (int i = 0; i < _len; ++i) {
        switch (_dir) {
            case Direction::up:
                _joints.emplace_front(Joint{{_tile.x, _tile.y + i}, _dir});
                break;
                
            case Direction::down:
                _joints.emplace_front(Joint{{_tile.x, _tile.y - i}, _dir});
                break;
                
            case Direction::left:
                _joints.emplace_front(Joint{{_tile.x + i, _tile.y}, _dir});
                break;
                
            case Direction::right:
                _joints.emplace_front(Joint{{_tile.x - i, _tile.y}, _dir});
                break;
                
            default:
                break;
        }
    }
    
    return _joints;
}

SDL_Point Snake::get_pos() const {
    SDL_Point _headPos = joints.back().tile;
    switch (joints.back().dir) {
        case Direction::up:
            --_headPos.y;
            break;
            
        case Direction::down:
            ++_headPos.y;
            break;
            
        case Direction::left:
            --_headPos.x;
            break;
            
        case Direction::right:
            ++_headPos.x;
            break;
            
        default:
            break;
    }
    
    return _headPos;
}

int Snake::get_len() const {
    return static_cast<int>(joints.size());
}

bool Snake::check_collision() {
    SDL_Rect headRect = {
        gridOffset.x + tileSize * joints.back().tile.x,
        gridOffset.y + tileSize * joints.back().tile.y,
        tileSize,
        tileSize
    };

    /* Check walls and include offset */
    switch (joints.back().dir) {
        case Direction::up:
            if (joints.back().tile.y <= 0) {
                return true;
            }
            headRect.y -= static_cast<int>(offset);
            break;
            
        case Direction::down:
            if (joints.back().tile.y >= numRows - 1) {
                return true;
            }
            headRect.y += static_cast<int>(offset);
            break;
            
        case Direction::left:
            if (joints.back().tile.x <= 0) {
                return true;
            }
            headRect.x -= static_cast<int>(offset);
            break;
            
        case Direction::right:
            if (joints.back().tile.x >= numCols - 1) {
                return true;
            }
            headRect.x += static_cast<int>(offset);
            break;
            
        default:
            break;
    }
    
    return false;
}

void Snake::inc_len() {
    switch (joints.front().dir) {
        case Direction::up:
            joints.emplace_front(Joint{{joints.front().tile.x, joints.front().tile.y + 1}, joints.front().dir});
            break;
            
        case Direction::down:
            joints.emplace_front(Joint{{joints.front().tile.x, joints.front().tile.y - 1}, joints.front().dir});
            break;
            
        case Direction::left:
            joints.emplace_front(Joint{{joints.front().tile.x + 1, joints.front().tile.y}, joints.front().dir});
            break;
            
        case Direction::right:
            joints.emplace_front(Joint{{joints.front().tile.x - 1, joints.front().tile.y}, joints.front().dir});
            break;
            
        default:
            break;
    }
}

void Snake::set_dir(const Keyboard _keyboard, SDL_Keycode _key) {
    if (_keyboard.get_key(_key)) {
        if (std::find(keyBuffer.begin(), keyBuffer.end(), _key) == keyBuffer.end()) {
            keyBuffer.push_front(_key);
            switch (_key) {
                case SDLK_w:
                    turnBuffer.push(Direction::up);
                    break;
                    
                case SDLK_a:
                    turnBuffer.push(Direction::left);
                    break;
                    
                case SDLK_s:
                    turnBuffer.push(Direction::down);
                    break;
                    
                case SDLK_d:
                    turnBuffer.push(Direction::right);
                    break;
                    
                default:
                    break;
            }
        }
    } else if (!keyBuffer.empty()) {
        for (auto it = keyBuffer.before_begin(); std::next(it) != keyBuffer.end(); ++it) {
            if (*std::next(it) == _key) {
                keyBuffer.erase_after(it);
                if (!keyBuffer.empty()) {
                    switch (keyBuffer.front()) {
                        case SDLK_w:
                            turnBuffer.push(Direction::up);
                            break;
                            
                        case SDLK_a:
                            turnBuffer.push(Direction::left);
                            break;
                            
                        case SDLK_s:
                            turnBuffer.push(Direction::down);
                            break;
                            
                        case SDLK_d:
                            turnBuffer.push(Direction::right);
                            break;
                            
                        default:
                            break;
                    }
                }
                break;
            }
        }
    }
    
    while (!turnBuffer.empty() && turnBuffer.front() == joints.back().dir) {
        turnBuffer.pop();
    }
}

void Snake::move() {
    auto _currTime = Scene_Clock::now();
    if (!prevTimeValid) {
        prevTime = _currTime;
        prevTimeValid = true;
        return;
    }
    
    auto _interval = std::chrono::duration_cast<std::chrono::nanoseconds>(_currTime - prevTime).count();
    if (_interval <= 0) {
        return;
    }
    
    offset += speed / (1e9 / _interval);
    prevTime = _currTime;
    
    if (offset >= tileSize) {
        for (auto _joint = joints.begin(); _joint != joints.end(); ++_joint) {
            switch (_joint->dir) {
                case Direction::up:
                    _joint->tile.y -= static_cast<int>(offset) / tileSize;
                    break;
                case Direction::down:
                    _joint->tile.y += static_cast<int>(offset) / tileSize;
                    break;
                case Direction::left:
                    _joint->tile.x -= static_cast<int>(offset) / tileSize;
                    break;
                case Direction::right:
                    _joint->tile.x += static_cast<int>(offset) / tileSize;
                    break;
                default:
                    break;
            }
            
            if (std::next(_joint) != joints.end()) {
                _joint->dir = std::next(_joint)->dir;
            } else {
                if (!turnBuffer.empty()) {
                    if (turnBuffer.front() == Direction::up || turnBuffer.front() == Direction::down) {
                        if (_joint->dir == Direction::left || _joint->dir == Direction::right) {
                            _joint->dir = turnBuffer.front();
                        }
                    } else {
                        if (_joint->dir == Direction::up || _joint->dir == Direction::down) {
                            _joint->dir = turnBuffer.front();
                        }
                    }
                    
                    turnBuffer.pop();
                }
            }
        }
        
        offset = fmod(offset, tileSize);
    }
}

void Snake::draw() {
    int _i = -10;
    for (auto joint = joints.rbegin(); joint != joints.rend(); ++joint) {
        _i += 10;
        _i %= 256 * 6;
        
        int _r, _g, _b;
        if (_i < 256 * 1) {
            _r = 0;
            _g = _i;
            _b = 255;
        } else if (_i < 256 * 2) {
            _r = 0;
            _g = 255;
            _b = 255 - (_i % 256);
        } else if (_i < 256 * 3) {
            _r = _i % 256;
            _g = 255;
            _b = 0;
        } else if (_i < 256 * 4) {
            _r = 255;
            _g = 255 - (_i % 256);
            _b = 0;
        } else if (_i < 256 * 5) {
            _r = 255;
            _g = 0;
            _b = _i % 256;
        } else {
            _r = 255 - (_i % 256);
            _g = 0;
            _b = _i % 256;
        }
        
        if (SDL_SetRenderDrawColor(renderer, _r % 256, _g % 256, _b % 256, SDL_ALPHA_OPAQUE) < 0) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_SetRenderDrawColor() failed: %s", SDL_GetError());
            exit(EXIT_FAILURE);
        }
        
        SDL_Rect jointRect = {
            gridOffset.x + tileSize * joint->tile.x,
            gridOffset.y + tileSize * joint->tile.y,
            tileSize,
            tileSize
        };
        
        auto nextJoint = std::prev(joint);
        if (nextJoint != joints.rend() && joint->dir != nextJoint->dir) {
            SDL_Rect cornerRect = jointRect;
            switch (joint->dir) {
                case Direction::up:
                    jointRect.h -= static_cast<int>(offset);
                    cornerRect.y -= tileSize;
                    if (nextJoint->dir == Direction::left) {
                        cornerRect.x += tileSize - static_cast<int>(offset);
                    }
                    cornerRect.w -= tileSize - static_cast<int>(offset);
                    break;
                    
                case Direction::down:
                    jointRect.y += static_cast<int>(offset);
                    jointRect.h -= static_cast<int>(offset);
                    cornerRect.y += tileSize;
                    if (nextJoint->dir == Direction::left) {
                        cornerRect.x += tileSize - static_cast<int>(offset);
                    }
                    cornerRect.w -= tileSize - static_cast<int>(offset);
                    break;
                    
                case Direction::left:
                    jointRect.w -= static_cast<int>(offset);
                    cornerRect.x -= tileSize;
                    if (nextJoint->dir == Direction::up) {
                        cornerRect.y += tileSize - static_cast<int>(offset);
                    }
                    cornerRect.h -= tileSize - static_cast<int>(offset);
                    break;
                    
                case Direction::right:
                    jointRect.x += static_cast<int>(offset);
                    jointRect.w -= static_cast<int>(offset);
                    cornerRect.x += tileSize;
                    if (nextJoint->dir == Direction::up) {
                        cornerRect.y += tileSize - static_cast<int>(offset);
                    }
                    cornerRect.h -= tileSize - static_cast<int>(offset);
                    break;
                    
                default:
                    break;
            }
            
            SDL_RenderFillRect(renderer, &cornerRect);
        } else {
            switch (joint->dir) {
                case Direction::up:
                    jointRect.y -= static_cast<int>(offset);
                    break;
                    
                case Direction::down:
                    jointRect.y += static_cast<int>(offset);
                    break;
                    
                case Direction::left:
                    jointRect.x -= static_cast<int>(offset);
                    break;
                    
                case Direction::right:
                    jointRect.x += static_cast<int>(offset);
                    break;
                    
                default:
                    break;
            }
        }
        
        SDL_RenderFillRect(renderer, &jointRect);
    }
}

void Snake::reset() {
    joints.clear();
    joints = init_joints({2, 5}, 3, Direction::right);
    offset = tileSize - 0.1;
    keyBuffer.clear();
    while (!turnBuffer.empty()) {
        turnBuffer.pop();
    }
    prevTimeValid = false;
}
