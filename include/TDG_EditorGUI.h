#ifndef TDG_EDITORGUI_H
#define TDG_EDITORGUI_H

#include <list>
#include "TDG_Window.h"
#include "TDG_View.h"
#include "TDG_TPalette.h"

class TDG_EditorGUI
{
    public:
        TDG_EditorGUI();
        virtual ~TDG_EditorGUI();

        bool init(TDG_Window* win, TDG_Position* pos);

        bool create();

        void addTileToPalette(TDG_Window* win, int tileID, unsigned int selectorNumber);

        void handleKeyboardInput(SDL_Event* event, TDG_Window* win, TDG_Mouse* mouse, TDG_Background* bg);

        void render(TDG_Window* win);

        TDG_View* getView();
        TDG_Position* getPos();

    protected:

    private:
        void addKey(list<SDL_Event>& keys, const Uint32 eventTyp, SDL_Scancode key);

        TDG_Position* pos;
        TDG_View* view;

        TDG_TPalette* tp;

};

#endif // TDG_EDITORGUI_H
