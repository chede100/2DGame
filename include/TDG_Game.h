#ifndef TDG_GAME_H
#define TDG_GAME_H

#include "TDG_GUI.h"
#include "TDG_EventHandler.h"
#include "TDG_GameBoard.h"

using namespace std;

class TDG_Game
{
    public:
        TDG_Game();
        virtual ~TDG_Game();

        bool init();
        bool start();
        void gameloop();
        void save();

    protected:

    private:
        TDG_GUI* gui;
        TDG_EventHandler* event;

        TDG_GameBoard* board;

};

#endif // TDG_GAME_H
