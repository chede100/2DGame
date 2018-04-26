#ifndef TDG_COLLISIONBOX_H
#define TDG_COLLISIONBOX_H

#include "TDG_Position.h"
#include "TDG_Background.h"

class TDG_CollisionBox
{
    public:
        TDG_CollisionBox();
        virtual ~TDG_CollisionBox();

        void bindToPosition(TDG_Position* pos, int xCorrection, int yCorrection);

        bool collisionWith(TDG_CollisionBox* box, int speedX, int speedY);
        bool collisionWith(TDG_Background* background, int speedX, int speedY);

        void setSize(int width, int hight);

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
