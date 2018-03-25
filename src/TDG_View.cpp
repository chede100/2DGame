#include "TDG_View.h"

TDG_View::TDG_View()
{
    this->pos = NULL;
    this->width = 0;
    this->hight = 0;
}

TDG_View::~TDG_View()
{
    //dtor
}

void TDG_View::init(int width, int hight)
{
    this->width = width;
    this->hight = hight;
}

void TDG_View::bindTo(TDG_Position* pos)
{
    if(pos == NULL)
        cout << "TDG_View: tried to bind to unvalid Postion!" << endl;
    else
        this->pos = pos;
}

int TDG_View::getPosX()
{
    int x = this->pos->getPosX() - this->width/2;
    return x;
}

int TDG_View::getPosY()
{
    int y = this->pos->getPosY() - this->hight/2;
    return y;
}

int TDG_View::getWidth()
{
    return this->width;
}

int TDG_View::getHight()
{
    return this->hight;
}
