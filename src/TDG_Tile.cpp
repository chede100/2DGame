#include "TDG_Tile.h"

TDG_Tile::TDG_Tile()
{

}

TDG_Tile::~TDG_Tile()
{
    //dtor
}

void TDG_Tile::init(int id, int rotationDegree, bool impassable, int flip)
{
    this->id = id;
    this->rotDegree = rotationDegree;
    this->impassable = impassable;
    if(flip == 1)
        this->flip = SDL_FLIP_VERTICAL;
    else if(flip == 2)
        this->flip = SDL_FLIP_HORIZONTAL;
    else
        this->flip = SDL_FLIP_NONE;
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

SDL_RendererFlip TDG_Tile::flipTile()
{
    return this->flip;
}
