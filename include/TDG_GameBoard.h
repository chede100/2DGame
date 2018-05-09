#ifndef TDG_GAMEBOARD_H
#define TDG_GAMEBOARD_H

#include "TDG_EntityHandler.h"
#include "TDG_Player.h"

class TDG_GameBoard
{
    public:
        TDG_GameBoard();
        virtual ~TDG_GameBoard();

        bool init(TDG_GUI* gui, TDG_FileHandler* specs);

        bool render(TDG_GUI* gui);

        void userInput(TDG_EventHandler* event);

        bool throughGate(Gate* enterGate);
        bool changeRoom(TDG_GUI* gui, Gate* enterGate);

        void startTimer();
        void stopTimer();

        void save();

    protected:

    private:
        bool createRoom(TDG_GUI* gui, Room* room);
        bool createPlayer(TDG_GUI* gui, SavePoint* sp);

        bool renderBackground(TDG_GUI* gui);

        string roomName;
        int roomID;

        TDG_Background* backg;
        TDG_View* view;

        TDG_StoredEntityAnimations* entityGraphics;

        TDG_Player* player;
        TDG_EntityHandler* entities;

};

#endif // TDG_GAMEBOARD_H
