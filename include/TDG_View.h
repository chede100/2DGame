#ifndef TDG_VIEW_H
#define TDG_VIEW_H

#include "TDG_Position.h"
#include <iostream>

using namespace std;

class TDG_View
{
    public:
        TDG_View();
        virtual ~TDG_View();


        void init(int width, int hight);

        void bindTo(TDG_Position* pos);

        int getPosX();
        int getPosY();

        int getWidth();
        int getHight();

    protected:

    private:
        TDG_Position* pos;
        int width, hight;
};

#endif // TDG_VIEW_H
