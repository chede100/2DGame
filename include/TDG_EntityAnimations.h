#ifndef TDG_ENTITYANIMATIONS_H
#define TDG_ENTITYANIMATIONS_H

#include "TDG_AnimationList.h"
#include "TDG_Window.h"
#include "TDG_SpriteLoader.h"
#include "TDG_Information.h"

class TDG_EntityAnimations
{
    public:
        TDG_EntityAnimations();
        virtual ~TDG_EntityAnimations();

        bool loadAnimations(TDG_Window* win, int animationID, EntityTyp typ);

        TDG_Animation* getAnimation(AnimationTyp typ);
        EntityTyp getTyp();
        int getID();
        int getImagesWidth();
        int getImagesHight();

    protected:

    private:
        void add(TDG_Animation* ani);

        int animationID;
        EntityTyp typ;

        int image_width, image_hight;

        TDG_AnimationList* aniL;
};

#endif // TDG_ENTITYANIMATIONS_H
