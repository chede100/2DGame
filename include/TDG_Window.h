#ifndef TDG_WINDOW_H
#define TDG_WINDOW_H

#include <SDL_image.h>
#include <SDL_ttf.h>
#include "TDG_FileHandler.h"

using namespace std;

class TDG_Window
{
    public:
        TDG_Window();
        virtual ~TDG_Window();

        bool init(Options* opt);

        SDL_Renderer* getRenderer();

        int getWinWidth();
        int getWinHight();

        int getLogicalWidth();
        int getLogicalHight();

        int getFPSCap();

    protected:

    private:
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        int winWidth, winHight;
        int winLogicalWidth, winLogicalHight;
        int fpsCap;
};

#endif // TDG_WINDOW_H
