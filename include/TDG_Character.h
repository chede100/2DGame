#ifndef TDG_CHARACTER_H
#define TDG_CHARACTER_H

#include "TDG_Entity.h"

class TDG_Character : public TDG_Entity
{
    public:
        TDG_Character();
        virtual ~TDG_Character();

        void init(Entity entity, EntityTyp typ, bool moveable);

        void move();
        void moveBack();

        bool collisionWith(TDG_Entity* entity);
        bool collisionWith(TDG_Background* background);

        bool isMoveable();

    protected:

    private:
        double speed;
        bool moveable;
};

#endif // TDG_CHARACTER_H
