#ifndef TDG_ENTITY_H
#define TDG_ENTITY_H

#include "TDG_CollisionBox.h"
#include "TDG_StoredEntityAnimations.h"

class TDG_Entity : public TDG_Position
{
    public:
        TDG_Entity();
        virtual ~TDG_Entity();

        bool init(Entity entity, EntityTyp typ, bool moveable, TDG_StoredEntityAnimations* storedGraphics);

    protected:

    private:

        string name;
        int id;
        EntityTyp typ;

        double speed;
        bool moveable;
        Direction newDir;
        MovementStatus currStatus;

        TDG_EntityAnimations* animations;
        TDG_Animation* currentAnimation;
        TDG_ImageList* currentImage;

        TDG_CollisionBox* cBox;

        //defines for what movement status which animation should be loaded
        AnimationTyp convertStatusToAnimationTyp(MovementStatus status);
};

#endif // TDG_ENTITY_H
