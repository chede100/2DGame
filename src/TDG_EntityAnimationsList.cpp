#include "TDG_EntityAnimationsList.h"

TDG_EntityAnimationsList::TDG_EntityAnimationsList()
{
    this->eAni = NULL;
    this->next = NULL;
}

TDG_EntityAnimationsList::~TDG_EntityAnimationsList()
{
    if(this->eAni != NULL)
        delete this->eAni;
}

void TDG_EntityAnimationsList::setEntityAnimations(TDG_EntityAnimations* eAni)
{
    this->eAni = eAni;
}

void TDG_EntityAnimationsList::setNext(TDG_EntityAnimationsList* next)
{
    this->next = next;
}

TDG_EntityAnimations* TDG_EntityAnimationsList::getEntityAnimations()
{
    return this->eAni;
}

TDG_EntityAnimationsList* TDG_EntityAnimationsList::getNext()
{
    return this->next;
}
