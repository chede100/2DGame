#include "TDG_Position.h"

TDG_Position::TDG_Position(double x, double y)
{
    this->x = x;
    this->y = y;
}

TDG_Position::TDG_Position()
{
    this->x = 0.0;
    this->y = 0.0;
}

TDG_Position::~TDG_Position()
{
    //dtor
}

double TDG_Position::getPosX()
{
    return this->x;
}

double TDG_Position::getPosY()
{
    return this->y;
}

void TDG_Position::setPosX(double newX)
{
    this->x = newX;
}
void TDG_Position::setPosY(double newY)
{
    this->y = newY;
}
