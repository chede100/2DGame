#ifndef TDG_EDITORBOARD_H
#define TDG_EDITORBOARD_H

#include <windows.h>
#include "TDG_GUI.h"
#include "TDG_EntityHandler.h"
#include "TDG_EventHandler.h"
#include "TDG_Mouse.h"

class TDG_EditorBoard
{
    public:
        TDG_EditorBoard();
        virtual ~TDG_EditorBoard();

        bool init(TDG_Window* win);

        bool render(TDG_Window* win);

        void handleInput(TDG_EventHandler* event);

        bool createRoom(TDG_Window* win, string& name, int id, int rows, int columns);
        bool loadRoom(TDG_Window* win, Room* room);
        bool saveRoom();

        void addTile(int id);
        void addEntity(EntityTyp typ, int id);

        void startTimer();
        void stopTimer();

        bool roomStored();

    protected:

    private:
        bool renderBackground(TDG_Window* win);

        string roomName;
        int roomID;

        TDG_Position* guiPos;
        TDG_GUI* gui;

        TDG_Mouse* mouse;

        TDG_Background* backg;

        TDG_StoredEntityAnimations* entityGraphics;

        TDG_EntityHandler* entities;
};

#endif // TDG_EDITORBOARD_H
