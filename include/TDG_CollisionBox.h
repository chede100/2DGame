#ifndef TDG_COLLISIONBOX_H
#define TDG_COLLISIONBOX_H

#include "TDG_Position.h"
#include "TDG_Background.h"

class TDG_CollisionBox
{
    public:
        TDG_CollisionBox();
        virtual ~TDG_CollisionBox();

        void bindToPosition(TDG_Position* pos, int xCorrection, int yCorrection, int width, int hight);

        bool collisionWith(TDG_CollisionBox* box);
        bool collisionWith(TDG_Background* background);

        int getWidth();
        int getHight();

        int getPosX();
        int getPosY();

    protected:

    private:
        TDG_Position* pos;
        int xCorrection, yCorrection;
        int width, hight;
};

#endif // TDG_COLLISIONBOX_H
