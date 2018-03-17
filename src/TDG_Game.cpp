#include "TDG_Game.h"

TDG_Game::TDG_Game()
{

}

TDG_Game::~TDG_Game()
{
    if(this->specs != NULL)
        delete this->specs;
    if(this->gui != NULL)
        delete this->gui;
    if(this->e != NULL)
        delete this->e;
}

bool TDG_Game::init()
{
    this->specs = new TDG_GameSpecs();
    if(!this->specs->load())
    {
        cout << "Couldnt load data from files to build the game!" << endl;
        return false;
    }

    this->gui = new TDG_GUI();
    if(!this->gui->init(this->specs->getOpt()))
    {
        cout << "Couldnt initialize GUI!" << endl;
        return false;
    }

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
