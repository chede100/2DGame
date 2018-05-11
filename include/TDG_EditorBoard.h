#ifndef TDG_EDITORBOARD_H
#define TDG_EDITORBOARD_H

#include <windows.h>
#include "TDG_EntityHandler.h"
#include "TDG_Player.h"
#include "TDG_Mouse.h"

class TDG_EditorBoard
{
    public:
        TDG_EditorBoard();
        virtual ~TDG_EditorBoard();

        bool init();

        bool render(TDG_GUI* gui);

        void handleInput(TDG_EventHandler* event);

        bool createRoom(int id, int width, int hight);
        bool loadRoom(TDG_GUI* gui, Room* room);
        bool saveRoom();

    protected:

    private:
        bool renderBackground(TDG_GUI* gui);

        string roomName;
        int roomID;

        TDG_Position* viewPos;
        TDG_View* view;

        TDG_Mouse* mouse;

        TDG_Background* backg;

        TDG_StoredEntityAnimations* entityGraphics;

        TDG_Player* player;
        TDG_EntityHandler* entities;
};

#endif // TDG_EDITORBOARD_H
