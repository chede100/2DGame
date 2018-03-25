#include "TDG_Position.h"

TDG_Position::TDG_Position(int x, int y)
{
    this->x = x;
    this->y = y;
}

TDG_Position::~TDG_Position()
{
    //dtor
}

int TDG_Position::getPosX()
{
    return this->x;
}

int TDG_Position::getPosY()
{
    return this->y;
}

void TDG_Position::setPosX(int newX)
{
    this->x = newX;
}
void TDG_Position::setPosY(int newY)
{
    this->y = newY;
}
