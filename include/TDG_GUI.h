#ifndef TDG_GUI_H
#define TDG_GUI_H

#include <list>
#include "TDG_Window.h"
#include "TDG_View.h"
#include "TDG_GUIElement.h"

class TDG_GUI
{
    public:
        TDG_GUI();
        virtual ~TDG_GUI();

        bool init(TDG_Window* win, TDG_Position* pos);

        bool createEditorGUI();

        bool createGameGUI();

        TDG_View* getView();
        TDG_Position* getPos();

    protected:

    private:

        TDG_Position* pos;
        TDG_View* view;

        list<TDG_GUIElement*> elements;
};

#endif // TDG_GUI_H
