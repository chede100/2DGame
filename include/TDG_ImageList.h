#ifndef TDG_IMAGELIST_H
#define TDG_IMAGELIST_H

#include <SDL.h>

class TDG_ImageList
{
    public:
        TDG_ImageList();
        virtual ~TDG_ImageList();

        void setImg(SDL_Texture* img);
        void setNext(TDG_ImageList* next);

        SDL_Texture* getImg();
        TDG_ImageList* getNext();

    protected:

    private:
        SDL_Texture* img;

        TDG_ImageList* next;
};

#endif // TDG_IMAGELIST_H
