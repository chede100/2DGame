#include "TDG_ImageList.h"

TDG_ImageList::TDG_ImageList()
{
    this->img = NULL;
    this->next = NULL;
}

TDG_ImageList::~TDG_ImageList()
{
    if(this->img != NULL)
        SDL_DestroyTexture(this->img);
}

void TDG_ImageList::setImg(SDL_Texture* img)
{
    this->img = img;
}

void TDG_ImageList::setNext(TDG_ImageList* next)
{
    this->next = next;
}

SDL_Texture* TDG_ImageList::getImg()
{
    return this->img;
}

TDG_ImageList* TDG_ImageList::getNext()
{
    return this->next;
}
