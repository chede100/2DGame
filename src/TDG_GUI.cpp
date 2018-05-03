#include "TDG_GUI.h"

TDG_GUI::TDG_GUI()
{
   this->m_window = NULL;
   this->m_renderer = NULL;
   this->winWidth = 0;
   this->winHight = 0;
   this->fpsCap = 0;
}

TDG_GUI::~TDG_GUI()
{
    if(this->m_renderer != NULL)
        SDL_DestroyRenderer(this->m_renderer);
    if(this->m_window != NULL)
        SDL_DestroyWindow(this->m_window);

    IMG_Quit();
    SDL_Quit();
}

bool TDG_GUI::init(Options* opt)
{
    this->fpsCap = opt->fpsCap;
    this->winWidth = opt->winWidth;
    this->winHight = opt->winHight;

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
    {
        cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << endl;
        return false;
    }

    if(IMG_Init(IMG_INIT_PNG) < 0)
    {
        cout << "SDL_image could not initialize! " << SDL_GetError() << endl;
        return false;
    }

    if(opt->fullscreen)
        this->m_window = SDL_CreateWindow("TDG",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          this->winWidth, this->winHight,
                                          SDL_WINDOW_FULLSCREEN_DESKTOP);
    else
        this->m_window = SDL_CreateWindow("TDG",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          this->winWidth, this->winHight,
                                          SDL_WINDOW_SHOWN);

    if(this->m_window == NULL)
    {
        cout << "SDL_Window could not be created!" << endl;
        return false;
    }

    this->m_renderer = SDL_CreateRenderer(this->m_window, -1, SDL_RENDERER_ACCELERATED);

    if(this->m_renderer == NULL)
    {
        cout << "SDL_Renderer could not be created!" << endl;
        return false;
    }

    SDL_RenderSetLogicalSize(this->m_renderer, 420, 320);

    return true;
}

SDL_Renderer* TDG_GUI::getRenderer()
{
    return this->m_renderer;
}

int TDG_GUI::getWinWidth()
{
    return this->winWidth;
}

int TDG_GUI::getWinHight()
{
    return this->winHight;
}

int TDG_GUI::getFPSCap()
{
    return this->fpsCap;
}
