#include "TDG_Game.h"

TDG_Game::TDG_Game()
{
    this->board = NULL;
    this->event = NULL;
    this->win = NULL;
}

TDG_Game::~TDG_Game()
{
    this->board->stopTimer();

    if(this->board != NULL)
        delete this->board;
    if(this->win != NULL)
        delete this->win;
    if(this->event != NULL)
        delete this->event;
}

bool TDG_Game::init()
{
    //Obj to store temporally all game relevant information
    TDG_FileHandler* specs = new TDG_FileHandler();
    if(specs == NULL)
    {
        cout << "Couldnt create file handler!" << endl;
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
    this->win = new TDG_Window();
    if(!this->win->init(specs->getOpt()))
    {
        cout << "Couldnt initialize Window!" << endl;
        delete specs;
        return false;
    }

    //create game board (load background, entity animation images etc.)
    this->board = new TDG_GameBoard();
    if(!this->board->init(this->win, specs))
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
    this->board->startTimer();
    return true;
}

void TDG_Game::gameloop()
{
    bool quit = false;
    while(!this->event->quit() && !quit)
    {
        this->board->userInput(this->event);

        ///////////// FPS //////////////////////////////////////////////////
        int before, delay;                                                //
        before = (int)SDL_GetTicks();                                     //
        ////////////////////////////////////////////////////////////////////

        if(!this->board->render(this->win))
        {
            cout << "Failed on rendering game board!" << endl;
            break;
        }

        //change room on gate collision
        Gate enterGate = {0, 0, 0, noStatus, 0, 0};
        if(this->board->throughGate(&enterGate))
        {
            this->board->stopTimer();

            if(!this->board->changeRoom(this->win, &enterGate))
            {
                cout << "Failed to change room." << endl;
                quit = true;
            }

            this->board->startTimer();
        }

        ////////////////////////////////////////////////////////////////////
        delay = 1000/win->getFPSCap() - ((int) SDL_GetTicks() - before);  //
                                                                          //
        if(delay > 0)                                                     //
            SDL_Delay(delay);                                             //
        ////////////////////////////////////////////////////////////////////
    }
}

void TDG_Game::save()
{
    this->board->save();
}
