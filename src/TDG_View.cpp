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

void TDG_View::init(int width, int hight, int moveIntervalX, int moveIntervalY)
{
    this->width = width;
    this->hight = hight;
    this->intervalX = moveIntervalX;
    this->intervalY = moveIntervalY;
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

void TDG_View::updateMovementInterval(int intervalX, int intervalY)
{
    this->intervalX = intervalX;
    this->intervalY = intervalY;
}

int TDG_View::getPosX()
{

    int xCentre = this->pos->getPosX() + xCorrection;
    int posX = xCentre - this->width/2;

    //is the field of view larger than the background?
    //yes -> center the view on the background
    //no -> center the view on the binded position
    if(this->intervalX <= this->width)
        return intervalX/2 - this->width/2;

    //collision with edge of the background / does it leave the interval?
    if(posX < 0)
        return 0;
    else if(posX + this->width > intervalX)
        return intervalX - this->width;

    return posX;
}

int TDG_View::getPosY()
{
    int yCentre = this->pos->getPosY() + yCorrection;
    int posY = yCentre - this->hight/2;

    if(this->intervalY <= this->hight)
        return intervalY/2 - this->hight/2;

    if(posY < 0)
        return 0;
    else if(posY + this->hight > intervalY)
        return intervalY - this->hight;

    return posY;
}

int TDG_View::getWidth()
{
    return this->width;
}

int TDG_View::getHight()
{
    return this->hight;
}
