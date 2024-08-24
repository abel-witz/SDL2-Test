#include "game.h"

int main(int argc, char *argv[])
{
	auto my_game = Game();
	my_game.Initialize();
	my_game.Destroy();
	return 0;
}