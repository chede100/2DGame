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

        void init(Entity entity, EntityTyp typ, bool moveable);
        bool assignAnimations(TDG_StoredEntityAnimations* storedGraphics);

        void changeMovementStatus(Direction dir);

        bool updateAnimation();

        void move();

        bool collisionWith(TDG_Entity* entity);
        bool collisionWith(TDG_Background* background);

        void render(TDG_GUI* gui, TDG_View* view);

    protected:

    private:

        string name;
        int id;
        int animationID;
        EntityTyp typ;

        TDG_Position* pos;

        double speed;
        bool moveable;
        MovementStatus curStatus;

        TDG_EntityAnimations* animations;
        TDG_Animation* currentAnimation;
        TDG_ImageList* currentImage;

        TDG_CollisionBox* cBox;

        //defines for what movement status which animation should be loaded
        AnimationTyp convertStatusToAnimationTyp(MovementStatus status);
};

#endif // TDG_ENTITY_H
