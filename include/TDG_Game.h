#ifndef TDG_GAME_H
#define TDG_GAME_H

#include "TDG_GUI.h"
#include "TDG_GameSpecs.h"
#include "TDG_EventHandler.h"
#include <iostream>

using namespace std;

class TDG_Game
{
    public:
        TDG_Game();
        virtual ~TDG_Game();

        bool init();
        bool start();
        void gameloop();

    protected:

    private:
        TDG_GUI* gui;
        TDG_GameSpecs* specs;
        TDG_EventHandler* e;

};

#endif // TDG_GAME_H
