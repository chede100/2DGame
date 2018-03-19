#include "TDG_Tile.h"

TDG_Tile::TDG_Tile(int id, bool impassable)
{
    this->id = id;
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

bool TDG_Tile::isImpassable()
{
    return this->impassable;
}
