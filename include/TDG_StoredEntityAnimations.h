#ifndef TDG_STOREDENTITYANIMATIONS_H
#define TDG_STOREDENTITYANIMATIONS_H

#include "TDG_EntityAnimationsList.h"

class TDG_StoredEntityAnimations
{
    public:
        TDG_StoredEntityAnimations();
        virtual ~TDG_StoredEntityAnimations();

        bool loadAndAdd(TDG_GUI* gui, EntityTyp typ, int animationID);
        bool remove(EntityTyp typ, int animationID);
        void removeAllExcept(EntityTyp typ, int animationID);

        bool isStored(EntityTyp typ, int animationID);

        TDG_EntityAnimations* getEntityAnimations(EntityTyp typ, int animationID);

    protected:

    private:
        TDG_EntityAnimationsList* eAniL;
};

#endif // TDG_STOREDENTITYANIMATIONS_H
