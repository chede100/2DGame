#ifndef TDG_ENTITYANIMATIONSLIST_H
#define TDG_ENTITYANIMATIONSLIST_H

#include "TDG_EntityAnimations.h"


class TDG_EntityAnimationsList
{
    public:
        TDG_EntityAnimationsList();
        virtual ~TDG_EntityAnimationsList();

        void setEntityAnimations(TDG_EntityAnimations* eAni);
        void setNext(TDG_EntityAnimationsList* next);

        TDG_EntityAnimations* getEntityAnimations();
        TDG_EntityAnimationsList* getNext();

    protected:

    private:
        TDG_EntityAnimations* eAni;

        TDG_EntityAnimationsList* next;
};

#endif // TDG_ENTITYANIMATIONSLIST_H
