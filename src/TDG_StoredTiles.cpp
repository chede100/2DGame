#include "TDG_StoredTiles.h"

TDG_StoredTiles::TDG_StoredTiles(SDL_Texture* img, int tileID)
{
    this->image = NULL;
    this->next = NULL;
    this->tileID = 0;
}

TDG_StoredTiles::~TDG_StoredTiles()
{
    if(this->image != NULL)
        SDL_DestroyTexture(this->image);
}

void TDG_StoredTiles::setNext(TDG_StoredTiles* next)
{
    this->next = next;
}

int TDG_StoredTiles::getTileID()
{
    return this->tileID;
}

SDL_Texture* TDG_StoredTiles::getImage()
{
    return this->image;
}

TDG_StoredTiles* TDG_StoredTiles::getNext()
{
    return this->next;
}
