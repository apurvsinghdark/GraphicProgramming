#include "Game.h"

int main()
{
	//Game Class INIT
	Game game("GP_UP", 800, 640, 4, 4, false);
	
	//UpdateLoop
	while (!game.GetWindowShouldClose())
	{
		//UPDATE INPUTS
		game.Update();

		game.Render();
	}

	return 0;
}