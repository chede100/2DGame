#include "TDG_GUI.h"

TDG_GUI::TDG_GUI()
{
    this->pos = NULL;
    this->view = NULL;
}

TDG_GUI::~TDG_GUI()
{
    if(this->view != NULL)
        delete this->view;
}

bool TDG_GUI::init(TDG_Window* win, TDG_Position* pos)
{
    this->view = new TDG_View();

    // set the field of view to the size of the window
    int viewWidth = win->getLogicalWidth();
    int viewHight = win->getLogicalHight();
    this->view->init(viewWidth, viewHight, 0, 0);

    this->pos = pos;

    //bind the field of view to the players character on the game board
    if(!this->view->bindTo(this->pos, 0, 0))
    {
        cout << "Unable to bind field of view!" << endl;
        return false;
    }

    return true;
}

bool TDG_GUI::createEditorGUI()
{

    return true;
}

bool TDG_GUI::createGameGUI()
{

    return true;
}

TDG_View* TDG_GUI::getView()
{
    return this->view;
}

TDG_Position* TDG_GUI::getPos()
{
    return this->pos;
}
