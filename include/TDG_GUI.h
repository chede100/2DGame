#ifndef TDG_GUI_H
#define TDG_GUI_H

#include <SDL_image.h>
#include "TDG_FileHandler.h"

using namespace std;

class TDG_GUI
{
    public:
        TDG_GUI();
        virtual ~TDG_GUI();

        bool init(Options* opt);

        SDL_Renderer* getRenderer();

        int getWinWidth();
        int getWinHight();

        int getFPSCap();

    protected:

    private:
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        int winWidth, winHight;
        int fpsCap;
};

#endif // TDG_GUI_H
