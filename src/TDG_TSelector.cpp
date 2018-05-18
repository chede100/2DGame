#include "TDG_TSelector.h"

TDG_TSelector::TDG_TSelector()
{
    this->width = 0;
    this->hight = 0;
    this->borderWidth = 0;
    this->tileImg = NULL;
    this->tileID = 0;

    this->boundToKey = false;
}

TDG_TSelector::~TDG_TSelector()
{
    if(this->tileImg != NULL)
        SDL_DestroyTexture(this->tileImg);
}

void TDG_TSelector::init(int width, int hight, int borderWidth)
{
    this->width = width;
    this->hight = hight;
    this->borderWidth = borderWidth;
}

void TDG_TSelector::bindToKey(const Uint32 eventTyp, SDL_Scancode key)
{
    this->eventTyp = eventTyp;
    this->key = key;
    this->boundToKey = true;
}

void TDG_TSelector::handleInupt(SDL_Event* event, TDG_Window* win, TDG_View* view, TDG_Mouse* mouse, TDG_Background* bg)
{
    if(this->boundToKey)
    {
        if(event->type == this->eventTyp)
        {
            const Uint8 *state = SDL_GetKeyboardState(NULL);
            if(state[this->key])
            {
                if(this->tileID != 0)
                    mouse->selectTile(win, view, bg, this->tileID);
            }
        }
    }
}

void TDG_TSelector::render(TDG_Window* win, int x, int y)
{
    if(SDL_SetRenderDrawColor(win->getRenderer(), 150, 150, 150, 0) < 0)
        cout << "Unable to set draw color!" << endl;

    SDL_Rect rect = {x, y, this->width + 2*this->borderWidth, this->hight + 2*this->borderWidth};
    if(SDL_RenderFillRect(win->getRenderer(), &rect) < 0)
       cout << "Unable to render rectangle. " << SDL_GetError() << endl;

    if(this->tileImg != NULL)
    {
        SDL_Rect img = {x + this->borderWidth, y + this->borderWidth, this->width, this->hight};
        SDL_RenderCopy(win->getRenderer(), this->tileImg, NULL, &img);
    }

    if(SDL_SetRenderDrawColor(win->getRenderer(), 0, 0, 0, 0) < 0)
        cout << "Unable to set draw color!" << endl;
}

bool TDG_TSelector::store(TDG_Window* win, int tileID)
{
    this->tileID = tileID;

    TDG_SpriteLoader* sprite = new TDG_SpriteLoader();
    if(!sprite->loadSprite("./data/img/room/", "room"))
    {
        cout << "Unable to load background sprite!" << endl;
        return false;
    }

    SDL_Texture* img = sprite->getImage(win, (int) (tileID-1)/sprite->getSpriteMaxColumns() + 1, (int) tileID%(sprite->getSpriteMaxColumns() + 1));
    if(img == NULL)
    {
        cout << "Unable to load image " << tileID << " of sprite sheet ./data/img/room/room.png" << endl;
        delete sprite;
        return false;
    }

    //destroy previous image
    if(this->tileImg != NULL)
        SDL_DestroyTexture(this->tileImg);

    this->tileImg = img;

    delete sprite;

    return true;
}

int TDG_TSelector::getWidth()
{
    return this->width + 2*this->borderWidth;
}

int TDG_TSelector::getHight()
{
    return this->hight + 2*this->borderWidth;
}
