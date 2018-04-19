#include "TDG_EntityList.h"

TDG_EntityList::TDG_EntityList()
{
    this->entry = NULL;
    this->next = NULL;
    this->previous = NULL;
}

TDG_EntityList::~TDG_EntityList()
{
    if(this->entry != NULL)
        delete this->entry;
}

void TDG_EntityList::setEntity(TDG_Entity* entity)
{
    this->entry = entity;
}

void TDG_EntityList::setNext(TDG_EntityList* next)
{
    this->next = next;
}

void TDG_EntityList::setPrevious(TDG_EntityList* pre)
{
    this->previous = pre;
}

TDG_Entity* TDG_EntityList::getEntity()
{
    return this->entry;
}

TDG_EntityList* TDG_EntityList::getNext()
{
    return this->next;
}

TDG_EntityList* TDG_EntityList::getPrevious()
{
    return this->previous;
}
