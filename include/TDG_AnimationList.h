#ifndef TDG_ANIMATIONLIST_H
#define TDG_ANIMATIONLIST_H

#include "TDG_Animation.h"

class TDG_AnimationList
{
    public:
        TDG_AnimationList();
        virtual ~TDG_AnimationList();

        void setNext(TDG_AnimationList* next);
        void setAnimation(TDG_Animation* ani);

        TDG_AnimationList* getNext();
        TDG_Animation* getAnimation();

    protected:

    private:

        TDG_Animation* ani;

        TDG_AnimationList* next;
};

#endif // TDG_ANIMATIONLIST_H
