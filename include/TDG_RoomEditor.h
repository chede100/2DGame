#ifndef TDG_ROOMEDITOR_H
#define TDG_ROOMEDITOR_H

#include "TDG_GUI.h"

class TDG_RoomEditor
{
    public:
        TDG_RoomEditor();
        virtual ~TDG_RoomEditor();

        bool init();

    protected:

    private:
        TDG_GUI* gui;
};

#endif // TDG_ROOMEDITOR_H
