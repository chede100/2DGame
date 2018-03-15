#include "TDG_EventHandler.h"

TDG_EventHandler::TDG_EventHandler()
{
    //ctor
}

TDG_EventHandler::~TDG_EventHandler()
{
    //dtor
}

bool TDG_EventHandler::quit()
{
    while(SDL_PollEvent(&this->event) != 0)
    {
        switch(event.type)
        {
        case SDL_WINDOWEVENT_CLOSE:
            return true;
            break;

        case SDL_QUIT:
            return true;
            break;

        case SDL_KEYDOWN:
            {
                switch(event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    return true;
                    break;
                }
            }

        }
    }

    return false;
}
