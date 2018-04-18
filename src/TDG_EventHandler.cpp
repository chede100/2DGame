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

Direction TDG_EventHandler::playerMovement()
{
    Direction dir = noDirection;
    int keysPressed = 0;
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;

    const Uint8 *state = SDL_GetKeyboardState(NULL);

    if(state[SDL_SCANCODE_UP])
    {
        up = true;
        keysPressed++;
    }
    if(state[SDL_SCANCODE_DOWN])
    {
        down = true;
        keysPressed++;
    }
    if(state[SDL_SCANCODE_LEFT])
    {
        left = true;
        keysPressed++;
    }
    if(state[SDL_SCANCODE_RIGHT])
    {
        right =  true;
        keysPressed++;
    }

    if(keysPressed == 1)
    {
        if(up)
            dir = north;
        else if(down)
            dir = south;
        else if(right)
            dir = east;
        else if(left)
            dir = west;
    }
    else if(keysPressed == 2)
    {
        if(up && left)
            dir = north_west;
        else if(up && right)
            dir = north_east;
        else if(down && left)
            dir = south_west;
        else if(down && right)
            dir = south_east;
    }

    return dir;
}
