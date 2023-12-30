#include "keyboard.hpp"

Keyboard::Keyboard() : keymap() {}

bool Keyboard::get_key(SDL_Keycode _key) const {
    return keymap.find(_key) != keymap.end() ? keymap.at(_key) : false;
}

void Keyboard::set_key(SDL_Event* _event) {
    if (_event->type == SDL_KEYDOWN || _event->type == SDL_KEYUP) {
        keymap[_event->key.keysym.sym] = _event->type == SDL_KEYDOWN;
    }
}

void Keyboard::reset() {
    keymap.clear();
}
