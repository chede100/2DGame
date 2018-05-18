#include "TDG_TPalette.h"

TDG_TPalette::TDG_TPalette()
{
    this->pa = LeftRight;
    this->hPos = hCenter;
    this->vPos = Bottom;
}

TDG_TPalette::~TDG_TPalette()
{
    for(list<TDG_TSelector*>::const_iterator it = this->palette.begin(), end = this->palette.end(); it != end; it++)
    {
        if(*it != NULL)
            delete *it;
    }
}

void TDG_TPalette::create(int numberOfButtons, int bWidth, int bHight, int borderWidth)
{
    if(this->palette.size() == 0)
    {
        for(int i = 0; i < numberOfButtons; i++)
        {
            TDG_TSelector* s = new TDG_TSelector();
            s->init(bWidth, bHight, borderWidth);
            this->palette.push_back(s);
        }
    }
    else
        cout << "Palette already created!" << endl;
}

void TDG_TPalette::bindToKeys(list<SDL_Event>& e)
{
    for(list<TDG_TSelector*>::const_iterator it = this->palette.begin(), end = this->palette.end(); it != end; it++)
    {
        if(e.size() != 0)
        {
            if(*it != NULL)
            {
                if(e.begin() != e.end())
                {
                    (*it)->bindToKey(e.front().type, e.front().key.keysym.scancode);
                    e.erase(e.begin());
                }
            }
        }
    }
}

void TDG_TPalette::setPosition(HorizontalPosition hp, VerticalPosition vp, PaletteAlignment pa)
{
    this->hPos = hp;
    this->vPos = vp;
    this->pa = pa;
}

void TDG_TPalette::handleInupt(SDL_Event* event, TDG_Window* win, TDG_View* view, TDG_Mouse* mouse, TDG_Background* bg)
{
    for(list<TDG_TSelector*>::const_iterator it = this->palette.begin(), end = this->palette.end(); it != end; it++)
    {
        (*it)->handleInupt(event, win, view, mouse, bg);
    }
}

void TDG_TPalette::render(TDG_Window* win, TDG_View* view)
{
    int sWidth = this->palette.front()->getWidth();
    int sHight = this->palette.front()->getHight();
    int paletteL = this->palette.size()*sWidth;
    int x = 0;
    int y = 0;
    if(this->hPos == Left)
        x = 0;
    else if((this->hPos == hCenter) && (this->pa == LeftRight))
        x = view->getWidth()/2 - paletteL/2;
    else if((this->hPos == hCenter) && (this->pa == TopDown))
        x = view->getWidth()/2 - sWidth/2;
    else if((this->hPos == Right) && (this->pa == LeftRight))
        x = view->getWidth() - paletteL;
    else if((this->hPos == Right) && (this->pa == TopDown))
        x = view->getWidth() - sWidth;

    if(this->vPos == Top)
        y = 0;
    else if((this->vPos == vCenter) && (this->pa == TopDown))
        y = view->getHight()/2 - paletteL/2;
    else if((this->vPos == vCenter) && (this->pa == LeftRight))
        y = view->getHight()/2 - sHight/2;
    else if((this->vPos == Bottom) && (this->pa == TopDown))
        y = view->getHight() - paletteL;
    else if((this->vPos == Bottom) && (this->pa == LeftRight))
        y = view->getHight() - sHight;

    if(this->pa == TopDown)
    {
        for(list<TDG_TSelector*>::const_iterator it = this->palette.begin(), end = this->palette.end(); it != end; it++)
        {
            (*it)->render(win, x, y);
            y = y + sHight;
        }
    }
    else
    {
        for(list<TDG_TSelector*>::const_iterator it = this->palette.begin(), end = this->palette.end(); it != end; it++)
        {
            (*it)->render(win, x, y);
            x = x + sWidth;
        }
    }

}

void TDG_TPalette::store(TDG_Window* win, int tileID, unsigned int selectorNumber)
{
    if(tileID != 0)
    {
        if((selectorNumber == 0) || (selectorNumber > this->palette.size()))
            cout << "Error: Tried to store tile out of bound of palette!" << endl;
        else
        {
            for(list<TDG_TSelector*>::const_iterator it = this->palette.begin(), end = this->palette.end(); it != end; it++)
            {
                selectorNumber--;
                if(selectorNumber == 0)
                {
                    (*it)->store(win, tileID);
                }
            }
        }
    }
    else
        cout << "Error: Invalid input! Not allowed to store 0 in TPalette!" << endl;
}

int TDG_TPalette::getWidth()
{
    if(this->pa == LeftRight)
        return this->palette.size()*this->palette.front()->getWidth();
    else if(this->pa == TopDown)
        return this->palette.front()->getWidth();

    return 0;
}

int TDG_TPalette::getHight()
{
    if(this->pa == LeftRight)
        return this->palette.front()->getHight();
    else if(this->pa == TopDown)
        return this->palette.size()*this->palette.front()->getHight();

    return 0;
}
