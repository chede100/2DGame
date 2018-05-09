#include "TDG_Game.h"
#include "TDG_RoomEditor.h"
#include <unistd.h>

int main(int argc, char *argv[])
{
    bool startEditor = false;
    bool startGame = false;
    char co;

    //handle command line arguments
    if(argc > 1)
    {
        while((co = getopt(argc, argv, "eh")) != EOF)
            switch(co)
            {
            case 'e':
                startEditor = true;
                break;
            case 'h':
                cout << "Allowed commands:" << endl;
                cout << "-e     : Start the room editor." << endl;
                cout << "-h     : Show help massage." << endl;
                break;
            case '?':
                startGame = true;
                break;
            default:
                startGame = true;
                break;
            }
    }
    else
        startGame = true;

    if(startGame)
    {
        TDG_Game* game = new TDG_Game();

        if(game->init())
        {
            if(game->start())
                game->gameloop();
            else
                cout << "Game build-up Error!" << endl;
        }
        else
            cout << "Initialization Error!" << endl;

        delete game;
    }
    else if(startEditor)
    {
        TDG_RoomEditor* editor = new TDG_RoomEditor();



        delete editor;
    }

    return 0;
}
