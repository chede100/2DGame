#include "TDG_Item.h"

TDG_Item::TDG_Item()
{
    this->id = 0;
    this->amount = 0;
    this->maxAmount = 0;
    this->pos = 0;
    this->img = NULL;
}

TDG_Item::~TDG_Item()
{
    if(this->img != NULL)
        SDL_DestroyTexture(this->img);
}

bool TDG_Item::create(TDG_Window* win, string name, int id, int amount, int maxAmount)
{
    if((this->img == NULL) && (id > 0))
    {
        this->id = id;
        this->amount = amount;
        this->name = name;

        TDG_SpriteLoader* sprite = new TDG_SpriteLoader();
        if(!sprite->loadSprite("./data/img/item/", "item"))
        {
            cout << "Unable to load sprite for item creation!" << endl;
            return false;
        }

        this->img = sprite->getImage(win, (int) (id-1)/sprite->getSpriteMaxColumns() + 1, (int) id%(sprite->getSpriteMaxColumns() + 1));
        if(this->img == NULL)
        {
            cout << "Unable to load image " << id << " of sprite sheet ./data/img/item/item.png" << endl;
            delete sprite;
            return false;
        }

        delete sprite;

        return true;
    }

    return false;
}

void TDG_Item::render(TDG_Window* win, int x, int y, int w, int h)
{
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderCopy(win->getRenderer(), this->img, NULL, &rect);
}

int TDG_Item::getID()
{
    return this->id;
}

int TDG_Item::getAmount()
{
    return this->amount;
}

int TDG_Item::getPosition()
{
    return this->pos;
}

void TDG_Item::setAmount(int amount)
{
    this->amount = amount;
}

int TDG_Item::getMaxAmount()
{
    return this->maxAmount;
}

void TDG_Item::setPosition(int pos)
{
    this->pos = pos;
}
