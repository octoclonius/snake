#ifndef scene_hpp
#define scene_hpp

#include "clock.hpp"
#include "fpscounter.hpp"
#include "game.hpp"
#include <SDL.h>

class Scene {
public:
    Scene();
    
    void run();
    
    ~Scene();
    
private:
    SDL_Window* init_window();
    SDL_Renderer* init_renderer();
    
    void clear_frame();
    void display_frame();
    void delay_frame();
    
    const char* windowName;
    bool vsyncOn;
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::chrono::time_point<Scene_Clock> prevTime;
    bool prevTimeValid;
    double maxRefreshRate;
    FPSCounter fpsCounter;
    Game game;
};

#endif /* scene_hpp */
