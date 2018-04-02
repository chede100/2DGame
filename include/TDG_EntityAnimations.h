#ifndef TDG_ENTITYANIMATIONS_H
#define TDG_ENTITYANIMATIONS_H

#include "TDG_AnimationList.h"
#include "TDG_GUI.h"
#include "TDG_SpriteLoader.h"

typedef enum
{
    Player, NPC, Object
}EntityTyp;

class TDG_EntityAnimations
{
    public:
        TDG_EntityAnimations();
        virtual ~TDG_EntityAnimations();

        bool loadAnimations(TDG_GUI* gui, int entityID, EntityTyp typ);

        TDG_Animation* getAnimation(AnimationTyp typ);

    protected:

    private:
        void add(TDG_Animation* ani);

        int entityID;
        EntityTyp typ;

        TDG_AnimationList* aniL;
};

#endif // TDG_ENTITYANIMATIONS_H
