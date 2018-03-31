#include "TDG_Animation.h"

TDG_Animation::TDG_Animation()
{
    this->imgL = NULL;
    this->typ = none;
}

TDG_Animation::~TDG_Animation()
{
    TDG_ImageList* tmp = this->imgL;
    TDG_ImageList* next;
    while(tmp != NULL)
    {
        next = tmp->getNext();
        delete tmp;
        tmp = next;
    }
}

void TDG_Animation::addImg(SDL_Texture* img)
{
    TDG_ImageList* newImg = new TDG_ImageList();
    newImg->setImg(img);

    if(this->imgL == NULL)
        this->imgL = newImg;
    else
    {
        TDG_ImageList* tmp = this->imgL;
        TDG_ImageList* next = this->imgL->getNext();
        while(next != NULL)
        {
            tmp = next;;
            next = tmp->getNext();
        }

        tmp->setNext(newImg);
    }
}

void TDG_Animation::setTyp(AnimationTyp typ)
{
    this->typ = typ;
}

TDG_ImageList* TDG_Animation::getFirstImg()
{
    return this->imgL;
}

AnimationTyp TDG_Animation::getTyp()
{
    return this->typ;
}
