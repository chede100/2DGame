#ifndef TDG_EVENTHANDLER_H
#define TDG_EVENTHANDLER_H

#include <SDL.h>

class TDG_EventHandler
{
    public:
        TDG_EventHandler();
        virtual ~TDG_EventHandler();

        bool quit();

    protected:

    private:
        SDL_Event event;
};

#endif // TDG_EVENTHANDLER_H
