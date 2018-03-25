#include "TDG_Tile.h"

TDG_Tile::TDG_Tile(int id, int rotationDegree, bool impassable)
{
    this->id = id;
    this->rotDegree = rotationDegree;
    this->impassable = impassable;
}

TDG_Tile::~TDG_Tile()
{
    //dtor
}

int TDG_Tile::getID()
{
    return this->id;
}

int TDG_Tile::getRotDegree()
{
    return this->rotDegree;
}

bool TDG_Tile::isImpassable()
{
    return this->impassable;
}
