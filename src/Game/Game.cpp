#include "Game.h"

Game::Game()
{
    IsRunning = false;
}

Game::~Game()
{
    Destroy();
}

void Game::InitGame()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return;
    }

    MyWind = SDL_CreateWindow("G1_GAME_ENGINE",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WindWidth, WindHeight, SDL_WINDOW_SHOWN);

    if (!MyWind)
    {
        std::cout << "Window creation failed: " << SDL_GetError() << std::endl;
        return;
    }

    MyRen = SDL_CreateRenderer(MyWind, -1, SDL_RENDERER_ACCELERATED);
    if (!MyRen)
    {
        std::cout << "Renderer creation failed: " << SDL_GetError() << std::endl;
        return;
    }

    srand(static_cast<unsigned>(time(nullptr)));

    SnakeHead = { 500, 500, 10, 10 };
    snakebody.push_back(SnakeHead);

    // spawn apples randomly
    for (int i = 0; i < 50; ++i)
    {
        apples.emplace_back(rand() % (WindWidth / 10) * 10,
            rand() % (WindHeight / 10) * 10,
            10, 10);
    }

    IsRunning = true;
}

void Game::ProcessingInput()
{
    SDL_Event evnt;
    while (SDL_PollEvent(&evnt))
    {
        switch (evnt.type)
        {
        case SDL_QUIT:
            IsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (evnt.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                IsRunning = false;
                break;
            case SDLK_w:
                if (CurrentDir != Direction::Down)
                    CurrentDir = Direction::Up;
                break;
            case SDLK_s:
                if (CurrentDir != Direction::Up)
                    CurrentDir = Direction::Down;
                break;
            case SDLK_a:
                if (CurrentDir != Direction::Right)
                    CurrentDir = Direction::Left;
                break;
            case SDLK_d:
                if (CurrentDir != Direction::Left)
                    CurrentDir = Direction::Right;
                break;
            }
            break;
        }
    }
}

void Game::Update()
{
    // Move snake based on current direction
    switch (CurrentDir)
    {
    case Direction::Up:
        SnakeHead.y -= 10;
        break;
    case Direction::Down:
        SnakeHead.y += 10;
        break;
    case Direction::Left:
        SnakeHead.x -= 10;
        break;
    case Direction::Right:
        SnakeHead.x += 10;
        break;
    }

    // Check for window boundary collision
    if (SnakeHead.x < 0 || SnakeHead.x >= WindWidth ||
        SnakeHead.y < 0 || SnakeHead.y >= WindHeight)
    {
        std::cout << "Snake hit the wall! Restarting...\n";
        SnakeSize = 5;
        snakebody.clear();
        SnakeHead = { 500, 500, 10, 10 };
        snakebody.push_back(SnakeHead);
    }

    CollisionDetection();
    SDL_Delay(SnakeSpeed); // control game speed
}

void Game::CollisionDetection()
{
    // Apple collision
    for (auto& apple : apples)
    {
        if (SDL_HasIntersection(&SnakeHead, &apple))
        {
            SnakeSize += 3;
            apple.x = rand() % (WindWidth / 10) * 10;
            apple.y = rand() % (WindHeight / 10) * 10;
        }
    }

    // Self collision
    for (size_t i = 1; i < snakebody.size(); ++i)
    {
        if (SDL_HasIntersection(&SnakeHead, &snakebody[i]))
        {
            std::cout << "Snake bit itself! Restarting...\n";
            SnakeSize = 5;
            snakebody.clear();
            SnakeHead = { 500, 500, 10, 10 };
            snakebody.push_back(SnakeHead);
            return;
        }
    }

    // Add new head and trim tail
    snakebody.push_front(SnakeHead);
    while (snakebody.size() > SnakeSize)
    {
        snakebody.pop_back();
    }
}

void Game::Render()
{
    SDL_SetRenderDrawColor(MyRen, 21, 21, 21, 255);
    SDL_RenderClear(MyRen);

    // Draw snake (white)
    SDL_SetRenderDrawColor(MyRen, 255, 255, 255, 255);
    for (auto& part : snakebody)
    {
        SDL_RenderFillRect(MyRen, &part);
    }

    // Draw apples (red)
    SDL_SetRenderDrawColor(MyRen, 255, 0, 0, 255);
    for (auto& apple : apples)
    {
        SDL_RenderFillRect(MyRen, &apple);
    }

    SDL_RenderPresent(MyRen);
}

void Game::Run()
{
    while (IsRunning)
    {
        ProcessingInput();
        Update();
        Render();
    }
}

void Game::Destroy()
{
    SDL_DestroyRenderer(MyRen);
    SDL_DestroyWindow(MyWind);
    SDL_Quit();
}
