#ifndef TDG_TILE_H
#define TDG_TILE_H

#include <SDL.h>

class TDG_Tile
{
    public:
        TDG_Tile(int id, int rotationDegree, bool impassable, int flip);
        virtual ~TDG_Tile();

        int getID();
        int getRotDegree();
        bool isImpassable();
        SDL_RendererFlip flipTile();

    protected:

    private:
        int id;
        int rotDegree;
        bool impassable;
        SDL_RendererFlip flip;

};

#endif // TDG_TILE_H
