#ifndef TDG_ROOMEDITOR_H
#define TDG_ROOMEDITOR_H

#include "TDG_GUI.h"
#include "TDG_EventHandler.h"
#include "TDG_EditorBoard.h"

class TDG_RoomEditor
{
    public:
        TDG_RoomEditor();
        virtual ~TDG_RoomEditor();

        bool init();

        void programLoop();

    protected:

    private:
        TDG_GUI* gui;
        TDG_EventHandler* event;

        TDG_EditorBoard* board;
};

#endif // TDG_ROOMEDITOR_H
