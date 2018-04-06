#include "TDG_StoredEntityAnimations.h"

TDG_StoredEntityAnimations::TDG_StoredEntityAnimations()
{
    this->eAniL = NULL;
}

TDG_StoredEntityAnimations::~TDG_StoredEntityAnimations()
{
    TDG_EntityAnimationsList* tmp = this->eAniL;
    TDG_EntityAnimationsList* next;
    while(tmp != NULL)
    {
        next = tmp->getNext();
        delete tmp;
        tmp = next;
    }
}

bool TDG_StoredEntityAnimations::loadAndAdd(TDG_GUI* gui, EntityTyp typ, int animationID)
{
    //Initial load and add
    if(this->eAniL == NULL)
    {
        this->eAniL = new TDG_EntityAnimationsList();

        TDG_EntityAnimations* newEAni = new TDG_EntityAnimations();
        if(!newEAni->loadAnimations(gui, animationID, typ))
        {
            cout << "Unable to load Animations from entity " << typ << " ID: " << animationID << endl;
            delete this->eAniL;
            this->eAniL = NULL;
            delete newEAni;
            return false;
        }

        this->eAniL->setEntityAnimations(newEAni);
    }
    //If the list was already initialized
    else
    {
        TDG_EntityAnimationsList* tmp = this->eAniL;
        TDG_EntityAnimationsList* next = this->eAniL->getNext();

        while(next != NULL)
        {
            tmp = next;
            next = tmp->getNext();
        }

        TDG_EntityAnimationsList* newEAniL = new TDG_EntityAnimationsList();

        TDG_EntityAnimations* newEAni = new TDG_EntityAnimations();
        if(!newEAni->loadAnimations(gui, animationID, typ))
        {
            cout << "Unable to load Animations from entity " << typ << " ID: " << animationID << endl;
            delete newEAniL;
            delete newEAni;
            return false;
        }

        newEAniL->setEntityAnimations(newEAni);

        tmp->setNext(newEAniL);
    }

    return true;
}

bool TDG_StoredEntityAnimations::remove(EntityTyp typ, int animationID)
{
    if(this->eAniL == NULL)
    {
        cout << "No animations stored that can be removed." << endl;
        return false;
    }

    //Check id the first element is the one which shall be removed
    TDG_EntityAnimationsList* tmp = this->eAniL;
    if(tmp->getEntityAnimations()->getTyp() == typ)
    {
        if(tmp->getEntityAnimations()->getID() == animationID)
        {
            this->eAniL = tmp->getNext();
            delete tmp;
            return true;
        }
    }

    //Search the element in the list which shall be removed
    TDG_EntityAnimationsList* next = this->eAniL->getNext();
    while(next != NULL)
    {
        if(next->getEntityAnimations()->getTyp() == typ)
        {
            if(next->getEntityAnimations()->getID() == animationID)
            {
                tmp->setNext(next->getNext());
                delete next;
                return true;
            }
        }

        tmp = next;
        next = tmp->getNext();
    }

    return false;
}

bool TDG_StoredEntityAnimations::isStored(EntityTyp typ, int animationID)
{
    TDG_EntityAnimationsList* tmp = this->eAniL;
    while(tmp != NULL)
    {
        if(tmp->getEntityAnimations()->getTyp() == typ)
        {
            if(tmp->getEntityAnimations()->getID() == animationID)
            {
                return true;
            }
        }
        tmp = tmp->getNext();
    }
    return false;
}

TDG_EntityAnimations* TDG_StoredEntityAnimations::getEntityAnimations(EntityTyp typ, int animationID)
{
    TDG_EntityAnimationsList* tmp = this->eAniL;
    while(tmp != NULL)
    {
        if(tmp->getEntityAnimations()->getTyp() == typ)
        {
            if(tmp->getEntityAnimations()->getID() == animationID)
            {
                return tmp->getEntityAnimations();
            }
        }
        tmp = tmp->getNext();
    }
    return NULL;
}
