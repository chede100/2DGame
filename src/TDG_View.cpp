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

bool TDG_View::bindTo(TDG_Position* pos, int xCorrection, int yCorrection)
{
    if(pos == NULL)
    {
        cout << "TDG_View: tried to bind to invalid entity!" << endl;
        return false;
    }
    else
    {
        this->pos = pos;
        this->xCorrection = xCorrection;
        this->yCorrection = yCorrection;
    }

    return true;
}

int TDG_View::getPosX()
{
    int xCentre = this->pos->getPosX() + xCorrection;
    return xCentre - this->width/2;;
}

int TDG_View::getPosY()
{
    int yCentre = this->pos->getPosY() + yCorrection;
    return yCentre - this->hight/2;;
}

int TDG_View::getWidth()
{
    return this->width;
}

int TDG_View::getHight()
{
    return this->hight;
}
