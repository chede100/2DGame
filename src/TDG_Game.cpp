#include "TDG_Game.h"

TDG_Game::TDG_Game()
{

}

TDG_Game::~TDG_Game()
{
    if(this->gs != NULL)
        delete this->gs;
    if(this->gui != NULL)
        delete this->gui;
    if(this->e != NULL)
        delete this->e;
}

bool TDG_Game::init()
{
    this->gs = new TDG_GameSpecs();
    if(!this->gs->load())
        return false;

    this->gui = new TDG_GUI();
    if(!this->gui->init(this->gs->getOptSpecs()))
        return false;

    this->e = new TDG_EventHandler();
    if(this->e == NULL)
    {
        cout << "Couldnt initialize EventHandler!" << endl;
        return false;
    }

    return true;
}

bool TDG_Game::start()
{
    return true;
}

void TDG_Game::gameloop()
{
    while(!this->e->quit())
    {

    }
}
