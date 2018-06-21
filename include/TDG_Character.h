#ifndef TDG_CHARACTER_H
#define TDG_CHARACTER_H

#include "TDG_Entity.h"
#include "TDG_EntityList.h"
#include "TDG_Background.h"

class TDG_Character : public TDG_Entity
{
    public:
        TDG_Character();
        virtual ~TDG_Character();

        void init(const Entity* entity, EntityTyp typ, TDG_StoredEntityAnimations* storedGraphics,  bool moveable);

        //move character and check collision, if collision is detected move back
        void moveAndCollision(TDG_EntityList* elist, TDG_Background* bground);

        void adjust(int posX, int posY, MovementStatus status);

        bool isMoveable();

    protected:

    private:
        bool collisionDetection(TDG_EntityList* eList, TDG_Background* bground, int speedX, int speedY);
        bool collisionWith(TDG_Entity* entity, int speedX, int speedY);
        bool collisionWith(TDG_Background* background, int speedX, int speedY);

        double speed;
        bool moveable;
};

#endif // TDG_CHARACTER_H
