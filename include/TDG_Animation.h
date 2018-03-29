#ifndef TDG_ANIMATION_H
#define TDG_ANIMATION_H

#include "TDG_ImageList.h"

typedef enum
{
    stand_north, stand_east, stand_south,
    move_north, move_east, move_south,
    none

}AnimationTyp;

class TDG_Animation
{
    public:
        TDG_Animation();
        virtual ~TDG_Animation();

    protected:

    private:

        AnimationTyp typ;
        TDG_ImageList* imgL;
};

#endif // TDG_ANIMATION_H
