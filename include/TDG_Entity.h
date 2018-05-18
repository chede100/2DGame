#ifndef TDG_ENTITY_H
#define TDG_ENTITY_H

#include "TDG_CollisionBox.h"
#include "TDG_StoredEntityAnimations.h"
#include "TDG_View.h"

class TDG_Entity
{
    public:
        TDG_Entity();
        virtual ~TDG_Entity();

        void init(const Entity* entity, EntityTyp typ);
        bool assignAnimations(TDG_StoredEntityAnimations* storedGraphics);
        void bindCBox();

        bool updateAnimation();

        void render(TDG_Window* win, TDG_View* view);

        TDG_CollisionBox* getCBox();
        MovementStatus getMovementStatus();
        TDG_Position* getPos();
        EntityTyp getTyp();
        int getImageWidth();
        int getImageHight();
        TDG_ImageList* getCurrentImage();
        int getAnimationID();
        int getID();
        void setMovementStatus(MovementStatus status);

    protected:

    private:

        string name;
        int id;
        int animationID;
        double scale;
        EntityTyp typ;

        TDG_Position* pos;

        MovementStatus curStatus;

        TDG_EntityAnimations* animations;
        TDG_Animation* currentAnimation;
        TDG_ImageList* currentImage;

        TDG_CollisionBox* cBox;

        //defines for what movement status which animation should be loaded
        AnimationTyp convertStatusToAnimationTyp(MovementStatus status);
};

#endif // TDG_ENTITY_H
