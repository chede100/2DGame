#include "TDG_GameBoard.h"

TDG_GameBoard::TDG_GameBoard()
{
    this->backg = NULL;
}

TDG_GameBoard::~TDG_GameBoard()
{
    if(this->backg != NULL)
        delete this->backg;
}

bool TDG_GameBoard::create(TDG_GUI* gui, TDG_GameSpecs* specs)
{
    this->roomName = specs->getRoom()->roomName;

    this->backg = new TDG_Background();
    if(!this->backg->create(gui, specs->getRoom()))
    {
        cout << "Unable to create background!" << endl;
        return false;
    }

    return true;
}
