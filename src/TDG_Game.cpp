#include "TDG_Game.h"

TDG_Game::TDG_Game()
{
    this->board = NULL;
    this->event = NULL;
    this->gui = NULL;
}

TDG_Game::~TDG_Game()
{
    if(this->gui != NULL)
        delete this->gui;
    if(this->event != NULL)
        delete this->event;
    if(this->board != NULL)
        delete this->board;
}

bool TDG_Game::init()
{
    //Obj to store temporally all game relevant information
    TDG_GameSpecs* specs = new TDG_GameSpecs();
    if(specs == NULL)
    {
        cout << "Couldnt create game specification object!" << endl;
        return false;
    }

    //get all game relevant information from files
    if(!specs->load())
    {
        cout << "Couldnt load data from files to build the game!" << endl;
        delete specs;
        return false;
    }

    //create Window and Renderer
    this->gui = new TDG_GUI();
    if(!this->gui->init(specs->getOpt()))
    {
        cout << "Couldnt initialize GUI!" << endl;
        delete specs;
        return false;
    }

    //SDL_RenderSetLogicalSize(gui->getRenderer(), 640, 480);

    //create game board (load background, entity animation images etc.)
    this->board = new TDG_GameBoard();
    if(!this->board->create(this->gui, specs))
    {
        cout << "Unable to create game board!" << endl;
        delete specs;
        return false;
    }

    //create EventHandler
    this->event = new TDG_EventHandler();

    delete specs;

    return true;
}

bool TDG_Game::start()
{
    return true;
}

void TDG_Game::gameloop()
{
    while(!this->event->quit())
    {
        if(!this->board->render(this->gui, SCALING_FAKTOR))
        {
            cout << "Failed on rendering game board!" << endl;
            break;
        }

        SDL_Delay(100);
    }
}
