#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>
#include <ctime>
#include <cstdlib>
#include "SDL2/include/SDL.h"

class Game
{
private:
    SDL_Renderer* MyRen = nullptr;
    SDL_Window* MyWind = nullptr;
    SDL_Rect SnakeHead;

    // Snake body container
    std::deque<SDL_Rect> snakebody;

    // Apple container
    std::vector<SDL_Rect> apples;

    enum class Direction { Up, Down, Left, Right };
    Direction CurrentDir = Direction::Right;

    int SnakeSize = 5;
    int SnakeSpeed = 100; // milliseconds per frame
    int WindWidth = 1000;
    int WindHeight = 1000;

    bool IsRunning = false;

public:
    Game();
    ~Game();

    void InitGame();
    void ProcessingInput();
    void Update();
    void Run();
    void Render();
    void Destroy();
    void CollisionDetection();
};
