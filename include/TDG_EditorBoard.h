#ifndef TDG_EDITORBOARD_H
#define TDG_EDITORBOARD_H

#include <windows.h>
#include "TDG_EditorGUI.h"
#include "TDG_EntityHandler.h"
#include "TDG_EventHandler.h"
#include "TDG_Mouse.h"

typedef struct
{
    bool load, create, save;
    int roomID;
    string rName;
    int rows, columns;

    bool addT, addC, addO;
    int addID;
    int palettePos;

}ConsoleStatus;

class TDG_EditorBoard
{
    public:
        TDG_EditorBoard();
        virtual ~TDG_EditorBoard();

        bool init(TDG_Window* win);

        bool render(TDG_Window* win);

        void handleInput(TDG_Window* win, TDG_EventHandler* event, ConsoleStatus* status);

        bool createRoom(TDG_Window* win, string& name, int id, int rows, int columns);
        bool loadRoom(TDG_Window* win, Room* room);
        bool saveRoom();

        void startTimer();
        void stopTimer();

        bool roomStored();
        bool roomExists(const string& roomDirectoryPath);

    protected:

    private:
        bool renderBackground(TDG_Window* win);

        string roomName;
        int roomID;

        TDG_Position* guiPos;
        TDG_EditorGUI* gui;

        TDG_Mouse* mouse;

        TDG_Background* backg;

        TDG_StoredEntityAnimations* entityGraphics;

        TDG_EntityHandler* entities;
};

#endif // TDG_EDITORBOARD_H
