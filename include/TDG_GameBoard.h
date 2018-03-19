#ifndef TDG_GAMEBOARD_H
#define TDG_GAMEBOARD_H

#include "TDG_Background.h"
#include "TDG_GUI.h"
#include "TDG_GameSpecs.h"
#include <string>
#include <iostream>

using namespace std;

class TDG_GameBoard
{
    public:
        TDG_GameBoard();
        virtual ~TDG_GameBoard();

        bool create(TDG_GUI* gui, TDG_GameSpecs* specs);

    protected:

    private:
        string roomName;

        TDG_Background* backg;

};

#endif // TDG_GAMEBOARD_H
