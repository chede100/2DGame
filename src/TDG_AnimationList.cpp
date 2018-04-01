#include "TDG_AnimationList.h"

TDG_AnimationList::TDG_AnimationList()
{
    this->ani = NULL;
    this->next = NULL;
}

TDG_AnimationList::~TDG_AnimationList()
{
    if(this->ani != NULL)
        delete this->ani;
}

void TDG_AnimationList::setNext(TDG_AnimationList* next)
{
    this->next = next;
}

void TDG_AnimationList::setAnimation(TDG_Animation* ani)
{
    this->ani = ani;
}

TDG_AnimationList* TDG_AnimationList::getNext()
{
    return this->next;
}

TDG_Animation* TDG_AnimationList::getAnimation()
{
    return this->ani;
}
