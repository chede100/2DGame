#ifndef TDG_ENTITYANIMATIONS_H
#define TDG_ENTITYANIMATIONS_H

#include "TDG_AnimationList.h"

class TDG_EntityAnimations
{
    public:
        TDG_EntityAnimations();
        virtual ~TDG_EntityAnimations();

        void loadAnimations(int entityID);

        TDG_Animation* getAnimation(AnimationTyp typ);

    protected:

    private:

        TDG_AnimationList* aniL;
};

#endif // TDG_ENTITYANIMATIONS_H
