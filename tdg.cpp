#include <iostream>
#include "TDG_Game.h"

int main(int argc, char *argv[])
{
    TDG_Game* game = new TDG_Game();

    if(game->init())
    {
        if(game->start())
            game->gameloop();
        else
            std::cout << "Game build-up Error!" << std::endl;
    }
    else
        std::cout << "Initialization Error!" << std::endl;

    delete game;

    return 0;
}
