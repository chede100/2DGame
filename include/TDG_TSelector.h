#ifndef TDG_TSELECTOR_H
#define TDG_TSELECTOR_H

#include "TDG_Mouse.h"
#include "TDG_Window.h"
#include "TDG_View.h"

class TDG_TSelector
{
    public:
        TDG_TSelector();
        virtual ~TDG_TSelector();

        void init(int width, int hight, int borderWidth);

        void bindToKey(const Uint32 eventTyp, SDL_Scancode key);

        void handleInupt(SDL_Event* event, TDG_Window* win, TDG_View* view, TDG_Mouse* mouse, TDG_Background* bg);

        void render(TDG_Window* win, int x, int y);

        bool store(TDG_Window* win, int tileID);

        int getWidth();
        int getHight();

    protected:

    private:
        bool boundToKey;
        Uint32 eventTyp;
        SDL_Keycode key;

        int width, hight;
        int borderWidth;

        int tileID;
        SDL_Texture* tileImg;
};

#endif // TDG_TSELECTOR_H
