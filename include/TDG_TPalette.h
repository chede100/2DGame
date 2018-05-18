#ifndef TDG_TPALETTE_H
#define TDG_TPALETTE_H

#include <list>
#include "TDG_EventHandler.h"
#include "TDG_Mouse.h"
#include "TDG_Window.h"
#include "TDG_View.h"
#include "TDG_TSelector.h"

typedef enum
{
    Left, hCenter, Right
}HorizontalPosition;

typedef enum
{
    Top, vCenter, Bottom
}VerticalPosition;

typedef enum
{
    TopDown, LeftRight
}PaletteAlignment;

class TDG_TPalette
{
    public:
        TDG_TPalette();
        virtual ~TDG_TPalette();

        void create(int numberOfButtons, int bWidth, int bHight, int borderWidth);

        void setPosition(HorizontalPosition hp, VerticalPosition vp, PaletteAlignment pa);

        void bindToKeys(list<SDL_Event>& e);

        void handleInupt(SDL_Event* event, TDG_Window* win, TDG_View* view, TDG_Mouse* mouse, TDG_Background* bg);

        void render(TDG_Window* win, TDG_View* view);

        void store(TDG_Window* win, int tileID, unsigned int selectorNumber);

        int getWidth();
        int getHight();

    protected:

    private:
        HorizontalPosition hPos;
        VerticalPosition vPos;
        PaletteAlignment pa;

        list<TDG_TSelector*> palette;
};

#endif // TDG_TPALETTE_H
