#include "TDG_GameBoard.h"

TDG_GameBoard::TDG_GameBoard()
{
    this->backg = NULL;
    this->view = NULL;
}

TDG_GameBoard::~TDG_GameBoard()
{
    if(this->backg != NULL)
        delete this->backg;
    if(this->view != NULL)
        delete this->view;
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

    this->view = new TDG_View();

    // set the field of view to the size of the window
    int viewWidth = specs->getOpt()->winWidth;
    int viewHight = specs->getOpt()->winHight;
    this->view->init(viewWidth, viewHight);

    // Test position!!!!!!!!!!!!!!!
    TDG_Position* newPos = new TDG_Position(320, 240);

    this->view->bindTo(newPos);

    return true;
}

void TDG_GameBoard::render()
{

}
