#include <iostream>
#include "SDL2/include/SDL.h"
#include "Game/Game.h"

int main(int agrc, char* agrv[])
{
	std::cout << "Hello World" << std::endl;

	Game G1;
	G1.InitGame();
	G1.Run();
	G1.Destroy();

	return 0;
}