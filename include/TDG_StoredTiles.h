#ifndef TDG_STOREDTILES_H
#define TDG_STOREDTILES_H

#include "SDL.h"

class TDG_StoredTiles
{
    public:
        TDG_StoredTiles(SDL_Texture* image, int tileID);
        virtual ~TDG_StoredTiles();

        void setNext(TDG_StoredTiles* next);

        TDG_StoredTiles* getNext();
        int getTileID();
        SDL_Texture* getImage();

    protected:

    private:
        int tileID;
        SDL_Texture* image;

        TDG_StoredTiles* next;
};

#endif // TDG_STOREDTILES_H
