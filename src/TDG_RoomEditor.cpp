#include "TDG_RoomEditor.h"

TDG_RoomEditor::TDG_RoomEditor()
{
    this->event = NULL;
    this->gui = NULL;
    this->board = NULL;
}

TDG_RoomEditor::~TDG_RoomEditor()
{
    if(this->gui != NULL)
        delete this->gui;
    if(this->event != NULL)
        delete this->event;
    if(this->board != NULL)
        delete this->board;
}

bool TDG_RoomEditor::init()
{
    TDG_FileHandler* specs = new TDG_FileHandler();
    if(specs == NULL)
    {
        cout << "Couldnt create file handler!" << endl;
        return false;
    }

    this->gui = new TDG_GUI();
    if(!this->gui->init(specs->getOpt()))
    {
        cout << "Couldnt initialize GUI!" << endl;
        delete specs;
        return false;
    }

    delete specs;

    this->board = new TDG_EditorBoard();

    //create EventHandler
    this->event = new TDG_EventHandler();

    return true;
}

void TDG_RoomEditor::programLoop()
{
    while(!this->event->quit())
    {


        ///////////// FPS //////////////////////////////////////////////////
        int before, delay;                                                //
        before = (int)SDL_GetTicks();                                     //
        ////////////////////////////////////////////////////////////////////



        ////////////////////////////////////////////////////////////////////
        delay = 1000/gui->getFPSCap() - ((int) SDL_GetTicks() - before);  //
                                                                          //
        if(delay > 0)                                                     //
            SDL_Delay(delay);                                             //
        ////////////////////////////////////////////////////////////////////
    }
}
