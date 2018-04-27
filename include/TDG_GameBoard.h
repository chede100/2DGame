#ifndef TDG_GAMEBOARD_H
#define TDG_GAMEBOARD_H

#include "TDG_EntityHandler.h"

class TDG_GameBoard
{
    public:
        TDG_GameBoard();
        virtual ~TDG_GameBoard();

        bool init(TDG_GUI* gui, TDG_GameSpecs* specs);

        bool render(TDG_GUI* gui);

        void userInput(Direction dir);

        bool throughGate(Gate* enterGate);
        bool changeRoom(TDG_GUI* gui, Room* newRoom, Gate* enterGate);

        void startTimer();
        void stopTimer();

    protected:

    private:
        bool createRoom(TDG_GUI* gui, Room* room);
        bool renderBackground(TDG_GUI* gui);

        string roomName;
        int roomID;

        TDG_Background* backg;
        TDG_View* view;

        TDG_StoredEntityAnimations* entityGraphics;

        TDG_Character* player;
        TDG_EntityHandler* entities;

};

#endif // TDG_GAMEBOARD_H
