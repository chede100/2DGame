#ifndef TDG_GAMEBOARD_H
#define TDG_GAMEBOARD_H

#include "TDG_Background.h"
#include "TDG_GUI.h"
#include "TDG_GameSpecs.h"
#include "TDG_View.h"
#include "TDG_Position.h"
#include "TDG_StoredEntityAnimations.h"

class TDG_GameBoard
{
    public:
        TDG_GameBoard();
        virtual ~TDG_GameBoard();

        bool create(TDG_GUI* gui, TDG_GameSpecs* specs);

        bool render(TDG_GUI* gui, int scaleFaktor);

    protected:

    private:
        bool renderBackground(TDG_GUI* gui, int scaleFaktor);

        string roomName;

        TDG_Background* backg;
        TDG_View* view;

        TDG_StoredEntityAnimations* entityGraphics;

};

#endif // TDG_GAMEBOARD_H
